// /* -*- Mode: C++; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*- */
// /*************************************************************************
//  *
//  * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
//  *
//  * Copyright 1999, 2012 Holger Seelig <holger.seelig@yahoo.de>.
//  *
//  * Titania - a multi-platform office productivity suite
//  *
//  * This file is part of the Titania Project.
//  *
//  * Titania is free software: you can redistribute it and/or modify
//  * it under the terms of the GNU Lesser General Public License version 3
//  * only, as published by the Free Software Foundation.
//  *
//  * Titania is distributed in the hope that it will be useful,
//  * but WITHOUT ANY WARRANTY; without even the implied warranty of
//  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  * GNU Lesser General Public License version 3 for more details
//  * (a copy is included in the LICENSE file that accompanied this code).
//  *
//  * You should have received a copy of the GNU Lesser General Public License
//  * version 3 along with Titania.  If not, see
//  * <http://www.gnu.org/licenses/lgpl.html>
//  * for a copy of the LGPLv3 License.
//  *
//  ************************************************************************/

#include "InputFileStream.h"

#include <Titania/OS/IsFile.h>
#include <Titania/OS/FileSize.h>
#include <Titania/LOG.h>
#include <giomm.h>
#include <iostream>
#include <utility>

namespace titania {
namespace basic {

const std::string ifilestream::reasons [2] = { "OK", "FAILED" };
const std::string ifilestream::empty_string;

ifilestream::ifilestream () :
	         std::istream (),
	         data_istream (NULL),
	         file_istream (NULL),
	         http_istream (NULL),
	              istream (NULL),
	 file_request_headers (),
	file_response_headers (),
	                m_url ()
{ }

ifilestream::ifilestream (const http::method method, const basic::uri & url) :
	         std::istream (),
	         data_istream (NULL),
	         file_istream (NULL),
	         http_istream (NULL),
	              istream (NULL),
	 file_request_headers (),
	file_response_headers (),
	                m_url (url)
{
	open (method, url);
}

ifilestream::ifilestream (ifilestream && other) :
	          ifilestream ()
{
	*this = std::move (other);
}

ifilestream &
ifilestream::operator = (ifilestream && other)
{
	data_istream          = other .data_istream;
	file_istream          = other .file_istream;
	http_istream          = other .http_istream;
	istream               = other .istream;
	file_request_headers  = std::move (other .file_request_headers);
	file_response_headers = std::move (other .file_response_headers);
	m_url                 = std::move (other .m_url);

	other .data_istream = NULL;
	other .file_istream = NULL;
	other .http_istream = NULL;
	other .istream      = NULL;

	init (istream -> rdbuf ());

	clear (other .rdstate ());
	other .clear (std::ios::badbit);

	return *this;
}

// Connection handling

void
ifilestream::open (const http::method method, const basic::uri & url)
{
	close ();

	if (url .scheme () == "data")
	{
		std::string::size_type first  = std::string::npos;
		std::string::size_type length = 0;
		std::string::size_type comma  = url .path () .find (',');

		if (comma not_eq std::string::npos)
		{
			first  = comma + 1;
			length = url .path () .size () - first;
		}

		file_response_headers .insert (std::make_pair ("Content-Type",   url .path () .substr (0, comma)));
		file_response_headers .insert (std::make_pair ("Content-Length", std::to_string (length)));

		istream = data_istream = new std::istringstream (url .path () .substr (first));
	}
	else if (url .is_local ())
	{
		istream = file_istream = new std::ifstream ();

		if (os::is_file (url .path ()))
			file_istream -> open (url .path ());

		else
			file_istream -> setstate (std::ios::failbit);
	}
	else
	{
		istream = http_istream = new ihttpstream ();
		http_istream -> open (method, url);
	}

	init (istream -> rdbuf ());
	clear (istream -> rdstate ());
}

void
ifilestream::send ()
{
	if (http_istream and *http_istream)
		http_istream -> send ();
		
	else if (file_istream)
	{		
		// Guess content type

		char data [64];
		size_t data_size = file_istream -> rdbuf () -> sgetn (data, 64);
		
		bool result_uncertain;
		std::string content_type = Gio::content_type_guess (url () .path (), (guchar*) data, data_size, result_uncertain);
	
		file_response_headers .insert (std::make_pair ("Content-Type",   content_type));
		file_response_headers .insert (std::make_pair ("Content-Length", std::to_string (os::file_size (url () .path ()))));

		// Reset stream

		file_istream -> seekg (0, std::ios_base::beg);
	}	
}

void
ifilestream::close ()
{
	if (http_istream)
	{
		http_istream = NULL;
	}
	else if (file_istream)
	{
		file_istream = NULL;
	}
	else if (data_istream)
	{
		data_istream = NULL;
	}

	delete istream;
	clear (std::ios::badbit);
}

void
ifilestream::request_header (const std::string & header, const std::string & value)
{
	if (http_istream)
		return http_istream -> request_header (header, value);

	file_request_headers .insert (std::make_pair (header, value));
}

const ifilestream::headers_type &
ifilestream::request_headers ()
{
	if (http_istream)
		return http_istream -> request_headers ();

	return file_request_headers;
}

const ifilestream::headers_type &
ifilestream::response_headers ()
{
	if (http_istream)
		return http_istream -> response_headers ();

	return file_response_headers;
}

// SocketStreamBuffer

const std::string &
ifilestream::http_version ()
{
	if (http_istream)
		return http_istream -> http_version ();

	return empty_string;
}

ifilestream::status_type
ifilestream::status ()
{
	if (http_istream)
		return http_istream -> status ();

	return *istream ? 200 : 0;
}

const std::string &
ifilestream::reason ()
{
	if (http_istream)
		return http_istream -> reason ();

	return *istream ? reasons [0] : reasons [1];
}

// Buffer

std::streambuf*
ifilestream::rdbuf ()
{
	if (istream)
		return istream -> rdbuf ();

	return NULL;
}

ifilestream::~ifilestream ()
{
	close ();
}

} // basic
} // titania
