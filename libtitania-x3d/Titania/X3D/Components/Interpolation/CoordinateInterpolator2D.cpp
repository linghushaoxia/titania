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

#include "CoordinateInterpolator2D.h"

#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

CoordinateInterpolator2D::CoordinateInterpolator2D (X3DExecutionContext* const executionContext) :
	        X3DBaseNode (executionContext -> getBrowser (), executionContext), 
	X3DInterpolatorNode (),                                                    
	           keyValue (),                                                    // MFVec2f [in,out] keyValue       [ ]       (-∞,∞)
	      value_changed ()                                                     // MFVec2f [out]    value_changed
{
	setComponent ("Interpolation");
	setTypeName ("CoordinateInterpolator2D");

	addField (inputOutput, "metadata",      metadata);
	addField (inputOnly,   "set_fraction",  set_fraction);
	addField (inputOutput, "key",           key);
	addField (inputOutput, "keyValue",      keyValue);
	addField (outputOnly,  "value_changed", value_changed);
}

X3DBaseNode*
CoordinateInterpolator2D::create (X3DExecutionContext* const executionContext) const
{
	return new CoordinateInterpolator2D (executionContext);
}

void
CoordinateInterpolator2D::initialize ()
{
	X3DInterpolatorNode::initialize ();

	keyValue .addInterest (this, &CoordinateInterpolator2D::set_keyValue);
}

void
CoordinateInterpolator2D::set_keyValue ()
{
	value_changed .resize (key .size () ? keyValue .size () / key .size () : 0);
}

void
CoordinateInterpolator2D::interpolate (size_t index0, size_t index1, float weight)
{
	index0 *= value_changed .size ();

	index1 = index0 + value_changed .size ();

	for (size_t i = 0; i < value_changed .size (); ++ i)
	{
		value_changed [i] = math::lerp <Vector2f> (keyValue [index0 + i],
		                                           keyValue [index1 + i],
		                                           weight);
	}
}

} // X3D
} // titania
