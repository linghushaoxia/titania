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

#include "HAnimJoint.h"

#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

const ComponentType HAnimJoint::component      = ComponentType::H_ANIM;
const std::string   HAnimJoint::typeName       = "HAnimJoint";
const std::string   HAnimJoint::containerField = "children";

HAnimJoint::Fields::Fields () :
	center (new SFVec3f ()),
	displacers (new MFNode ()),
	limitOrientation (new SFRotation ()),
	llimit (new MFFloat ()),
	name (new SFString ()),
	rotation (new SFRotation ()),
	scale (new SFVec3f (1, 1, 1)),
	scaleOrientation (new SFRotation ()),
	skinCoordIndex (new MFInt32 ()),
	skinCoordWeight (new MFFloat ()),
	stiffness (new MFFloat ({ 0, 0, 0 })),
	translation (new SFVec3f ()),
	ulimit (new MFFloat ())
{ }

HAnimJoint::HAnimJoint (X3DExecutionContext* const executionContext) :
	    X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DGroupingNode (),
	         fields ()
{
	addType (X3DConstants::HAnimJoint);

	addField (inputOutput,    "metadata",         metadata ());
	addField (initializeOnly, "bboxSize",         bboxSize ());
	addField (initializeOnly, "bboxCenter",       bboxCenter ());
	addField (inputOnly,      "addChildren",      addChildren ());
	addField (inputOnly,      "removeChildren",   removeChildren ());
	addField (inputOutput,    "children",         children ());
	addField (inputOutput,    "center",           center ());
	addField (inputOutput,    "displacers",       displacers ());
	addField (inputOutput,    "limitOrientation", limitOrientation ());
	addField (inputOutput,    "llimit",           llimit ());
	addField (inputOutput,    "name",             name ());
	addField (inputOutput,    "rotation",         rotation ());
	addField (inputOutput,    "scale",            scale ());
	addField (inputOutput,    "scaleOrientation", scaleOrientation ());
	addField (inputOutput,    "skinCoordIndex",   skinCoordIndex ());
	addField (inputOutput,    "skinCoordWeight",  skinCoordWeight ());
	addField (inputOutput,    "stiffness",        stiffness ());
	addField (inputOutput,    "translation",      translation ());
	addField (inputOutput,    "ulimit",           ulimit ());
}

X3DBaseNode*
HAnimJoint::create (X3DExecutionContext* const executionContext) const
{
	return new HAnimJoint (executionContext);
}

} // X3D
} // titania
