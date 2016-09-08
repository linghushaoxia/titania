/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstraße 31a, Leipzig, Germany 2011.
 *
 * All rights reserved. Holger Seelig <holger.seelig@yahoo.de>.
 *
 * THIS IS UNPUBLISHED SOURCE CODE OF create3000.
 *
 * The copyright notice above does not evidence any actual of intended
 * publication of such source code, and is an unpublished work by create3000.
 * This material contains CONFIDENTIAL INFORMATION that is the property of
 * create3000.
 *
 * No permission is granted to copy, distribute, or create derivative works from
 * the contents of this software, in whole or in part, without the prior written
 * permission of create3000.
 *
 * NON-MILITARY USE ONLY
 *
 * All create3000 software are effectively free software with a non-military use
 * restriction. It is free. Well commented source is provided. You may reuse the
 * source in any way you please with the exception anything that uses it must be
 * marked to indicate is contains 'non-military use only' components.
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright 1999, 2016 Holger Seelig <holger.seelig@yahoo.de>.
 *
 * This file is part of the Titania Project.
 *
 * Titania is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License version 3 only, as published by the
 * Free Software Foundation.
 *
 * Titania is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU General Public License version 3 for more
 * details (a copy is included in the LICENSE file that accompanied this code).
 *
 * You should have received a copy of the GNU General Public License version 3
 * along with Titania.  If not, see <http://www.gnu.org/licenses/gpl.html> for a
 * copy of the GPLv3 License.
 *
 * For Silvio, Joy and Adi.
 *
 ******************************************************************************/

#ifndef __TITANIA_STREAM_INPUT_URL_STREAM_H__
#define __TITANIA_STREAM_INPUT_URL_STREAM_H__

#include "../Stream/UrlStreamBuf.h"

namespace titania {
namespace basic {

class iurlstream :
	public std::istream
{
public:

	typedef urlstreambuf::headers_type headers_type;
	typedef size_t                     status_type;

	/// @name Constructors

	iurlstream ();

	iurlstream (const basic::uri &, size_t = 60000);

	iurlstream (iurlstream &&);

	/// @name Properties

	const basic::uri &
	url () const
	{ return buf -> url (); }

	/// @name Connection handling

	void
	open (const basic::uri &, size_t = 60000);

	void
	send ();

	void
	close ();

	void
	stop ();

	bool
	stopping () const;

	/// @name Header handling

	void
	request_header (const std::string &, const std::string &);

	void
	request_headers (const headers_type & value)
	{ request_headers_map = value; }

	const headers_type &
	request_headers () const
	{ return request_headers_map; }

	const headers_type &
	response_headers () const
	{ return response_headers_map; }

	/// @name Element access

	const std::string &
	http_version ()
	{ return response_http_version; }

	status_type
	status () const
	{ return response_status; }

	const std::string &
	reason () const
	{ return response_reason; }

	/// @name Destructor

	virtual
	~iurlstream ();


private:

	iurlstream (const iurlstream &) = delete;

	iurlstream &
	operator = (const iurlstream &) = delete;

	bool
	parse_status_line ();

	void
	parse_response_headers ();

	void
	parse_response_header ();

	///  @name Members

	urlstreambuf* buf;

	headers_type request_headers_map;
	headers_type response_headers_map;

	std::string response_http_version;
	status_type response_status;
	std::string response_reason;

};

} // basic
} // titania

#endif
