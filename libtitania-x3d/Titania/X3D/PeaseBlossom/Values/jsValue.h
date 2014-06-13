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
 * For Silvio, Joy and Adi.
 *
 ******************************************************************************/

#ifndef __TITANIA_X3D_PEASE_BLOSSOM_VALUES_JS_VALUE_H__
#define __TITANIA_X3D_PEASE_BLOSSOM_VALUES_JS_VALUE_H__

#include "../Bits/Exception.h"
#include "../Base/jsChildType.h"
#include "../Base/jsOutputStreamType.h"
#include "../Values/var.h"
#include "../Values/ValueType.h"

namespace titania {
namespace pb {

/**
 *  Class to represent a JavaScript value. This is the base class for all JavaScript values.
 */
class jsValue :
	public jsChildType,
	public jsOutputStreamType
{
public:
	
	///  @name Member access

	virtual
	ValueType
	getType () const = 0;

	///  @name Operations

	virtual
	bool
	isPrimitive () const;

	virtual
	var
	toPrimitive () const
	{ return var (const_cast <jsValue*> (this)); }

	virtual
	bool
	toBoolean () const = 0;

	virtual
	uint16_t
	toUInt16 () const = 0;

	virtual
	int32_t
	toInt32 () const = 0;

	virtual
	uint32_t
	toUInt32 () const = 0;

	virtual
	double
	toNumber () const = 0;

	virtual
	var
	toObject () const
	throw (TypeError) = 0;


protected:

	///  @name Construction

	jsValue () :
		       jsChildType (),
		jsOutputStreamType ()
	{ }

};

inline
bool
jsValue::isPrimitive () const
{
	switch (getType ())
	{
		case UNDEFINED:
		case BOOLEAN:
		case NUMBER:
		case STRING:
		case NULL_OBJECT:
		case BOOLEAN_OBJECT:
		case NUMBER_OBJECT:
		case STRING_OBJECT:
			return true;
		default:
			return false;
	}
}

} // pb
} // titania

#endif
