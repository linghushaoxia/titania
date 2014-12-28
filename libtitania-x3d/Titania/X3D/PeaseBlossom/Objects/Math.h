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

#ifndef __TITANIA_X3D_PEASE_BLOSSOM_OBJECTS_MATH_H__
#define __TITANIA_X3D_PEASE_BLOSSOM_OBJECTS_MATH_H__

#include "../Objects/pbObject.h"

namespace titania {
namespace pb {

/**
 *  Class to represent an Math.
 */
class Math :
	public pbObject
{
public:

	///  @name Construction

	///  Constructs new Math.
	Math () :
		pbObject ()
	{
		addPropertyDescriptor ("E",       M_E,       NONE);
		addPropertyDescriptor ("LN10",    M_LN10,    NONE);
		addPropertyDescriptor ("LN2",     M_LN2,     NONE);
		addPropertyDescriptor ("LOG2E",   M_LOG2E,   NONE);
		addPropertyDescriptor ("LOG10E",  M_LOG10E,  NONE);
		addPropertyDescriptor ("PI",      M_PI,      NONE);
		addPropertyDescriptor ("SQRT1_2", M_SQRT1_2, NONE);
		addPropertyDescriptor ("SQRT2",   M_SQRT2,   NONE);
	
		addPropertyDescriptor ("abs",   new NativeFunction ("abs", abs), NONE);
	}

	///  Constructs new Math.
	virtual
	ptr <pbBaseObject>
	copy (pbExecutionContext* const executionContext) const
	throw (pbException,
	       pbControlFlowException) final override
	{ return pbObject::copy (executionContext, new Math ()); }


	static
	var
	abs (const ptr <pbObject> & object, const std::vector <var> & arguments)
	{
		if (arguments .size () not_eq 1)
			throw Error ("wrong number of arguments");

		return std::abs (arguments [0] .toNumber ());
	}

};

} // pb
} // titania

#endif
