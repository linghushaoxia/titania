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
 * Copyright 1999, 2012 Holger Seelig <holger.seelig@yahoo.de>.
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
 ******************************************************************************/

#ifndef __TITANIA_X3D_BITS_ERROR_H__
#define __TITANIA_X3D_BITS_ERROR_H__

#include <exception>
#include <string>

namespace titania {
namespace X3D {

class X3DError :
	public std::exception
{
public:

	explicit
	X3DError (const std::string &);

	virtual
	const char*
	what () const
	throw ();

	virtual
	const std::string &
	toString () const
	throw ();

	virtual
	~X3DError ()
	throw ();


private:

	std::string message;

};

enum ErrorType
{
	BROWSER_UNAVAILABLE,
	CONNECTION_ERROR,
	DISPOSED,
	IMPORTED_NODE,
	INITIALIZED_ERROR,
	INSUFFICIENT_CAPABILITIES,
	INVALID_ACCESS_TYPE,
	INVALID_BROWSER,
	INVALID_DOCUMENT,
	INVALID_EXECUTION_CONTEXT,
	INVALID_FIELD,
	INVALID_NAME,
	INVALID_NODE,
	INVALID_OPERATION_TIMING,
	INVALID_SCENE,
	INVALID_URL,
	INVALID_X3D,
	NODE_IN_USE,
	NODE_NOT_AVAILABLE,
	NOT_SHARED,
	NOT_SUPPORTED,
	URL_UNAVAILABLE

};

///  5.3.1 Error
template <ErrorType Type>
class Error :
	public X3DError
{
public:

	explicit
	Error (const std::string & message) :
		X3DError (message .c_str ())
	{ }

	virtual
	~Error ()
	throw ()
	{ }

};

extern template class Error <BROWSER_UNAVAILABLE>;
extern template class Error <CONNECTION_ERROR>;
extern template class Error <DISPOSED>;
extern template class Error <IMPORTED_NODE>;
extern template class Error <INITIALIZED_ERROR>;
extern template class Error <INSUFFICIENT_CAPABILITIES>;
extern template class Error <INVALID_ACCESS_TYPE>;
extern template class Error <INVALID_BROWSER>;
extern template class Error <INVALID_DOCUMENT>;
extern template class Error <INVALID_EXECUTION_CONTEXT>;
extern template class Error <INVALID_FIELD>;
extern template class Error <INVALID_NAME>;
extern template class Error <INVALID_NODE>;
extern template class Error <INVALID_OPERATION_TIMING>;
extern template class Error <INVALID_SCENE>;
extern template class Error <INVALID_URL>;
extern template class Error <INVALID_X3D>;
extern template class Error <NODE_IN_USE>;
extern template class Error <NODE_NOT_AVAILABLE>;
extern template class Error <NOT_SHARED>;
extern template class Error <NOT_SUPPORTED>;
extern template class Error <URL_UNAVAILABLE>;

} // X3D
} // titania

#endif
