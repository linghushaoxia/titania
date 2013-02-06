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

#include "Arc2D.h"

#include "../../Browser/Geometry2D/Arc2DProperties.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

Arc2D::Arc2D (X3DExecutionContext* const executionContext) :
	    X3DBaseNode (executionContext -> getBrowser (), executionContext), 
	X3DGeometryNode (),                                                    
	     startAngle (),                                                    // SFFloat [ ] startAngle  0          [-2π,2π]
	       endAngle (1.570796),                                            // SFFloat [ ] endAngle    π/2        [-2π,2π]
	         radius (1)                                                    // SFFloat [ ] radius      1          (0,∞)
{
	setComponent ("Geometry2D");
	setTypeName ("Arc2D");

	addField (inputOutput,    "metadata",   metadata);
	addField (initializeOnly, "startAngle", startAngle);
	addField (initializeOnly, "endAngle",   endAngle);
	addField (initializeOnly, "radius",     radius);
}

X3DBaseNode*
Arc2D::create (X3DExecutionContext* const executionContext) const
{
	return new Arc2D (executionContext);
}

void
Arc2D::initialize ()
{
	X3DGeometryNode::initialize ();

	getBrowser () -> getBrowserOptions () -> arcClose2DProperties .addInterest (this, &Arc2D::set_properties);
}

float
Arc2D::getAngle ()
{
	float start = math::interval <float> (startAngle, 0, M_PI2);
	float end   = math::interval <float> (endAngle,   0, M_PI2);
	
	if (start == end)
		return M_PI2;
		
	float difference = std::min (std::abs (end - start), float (M_PI2));
	
	if (start > end)
		return M_PI2 - difference;
		
	return difference;
}

void
Arc2D::set_properties ()
{
	update ();
}

void
Arc2D::build ()
{
	const Arc2DProperties* properties = getBrowser () -> getBrowserOptions () -> arc2DProperties;
	
	float  difference = getAngle ();
	size_t segments   = std::ceil (difference / properties -> minAngle);
	float  angle      = difference / segments;

	getVertices () .reserve (segments + 1);

	if (difference < float (2 * M_PI))
	{
		++ segments;
		setVertexMode (GL_LINE_STRIP);
	}
	else
		setVertexMode (GL_LINE_LOOP);
	
	for (size_t n = 0; n < segments; ++ n)
	{
		float theta = startAngle + angle * n;
	
		std::complex <float> point = std::polar <float> (radius, theta);

		getVertices () .emplace_back (point .real (), point .imag (), 0);
	}

	setSolid (false);
}

void
Arc2D::display ()
{
	glDisable (GL_LIGHTING);
	X3DGeometryNode::display ();
}

void
Arc2D::dispose ()
{
	getBrowser () -> getBrowserOptions () -> arcClose2DProperties .removeInterest (this, &Arc2D::set_properties);

	X3DGeometryNode::dispose ();
}

} // X3D
} // titania
