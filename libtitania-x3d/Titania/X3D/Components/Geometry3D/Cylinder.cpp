/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstra�e 31a, Leipzig, Germany 2011.
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

#include "Cylinder.h"

#include "../../Browser/Geometry3D/CylinderOptions.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../../Components/Geometry3D/IndexedFaceSet.h"
#include "../../Components/Rendering/Coordinate.h"
#include "../../Components/Texturing/TextureCoordinate.h"

#include <complex>

namespace titania {
namespace X3D {

const ComponentType Cylinder::component      = ComponentType::GEOMETRY_3D;
const std::string   Cylinder::typeName       = "Cylinder";
const std::string   Cylinder::containerField = "geometry";

Cylinder::Fields::Fields () :
	   top (new SFBool (true)),
	  side (new SFBool (true)),
	bottom (new SFBool (true)),
	height (new SFFloat (2)),
	radius (new SFFloat (1)),
	 solid (new SFBool (true))
{ }

Cylinder::Cylinder (X3DExecutionContext* const executionContext) :
	    X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DGeometryNode (),
	         fields ()
{
	addType (X3DConstants::Cylinder);

	addField (inputOutput,    "metadata", metadata ());
	addField (initializeOnly, "top",      top ());
	addField (initializeOnly, "side",     side ());
	addField (initializeOnly, "bottom",   bottom ());
	addField (initializeOnly, "height",   height ());
	addField (initializeOnly, "radius",   radius ());
	addField (initializeOnly, "solid",    solid ());

	height () .setUnit (UnitCategory::LENGTH);
	radius () .setUnit (UnitCategory::LENGTH);
}

X3DBaseNode*
Cylinder::create (X3DExecutionContext* const executionContext) const
{
	return new Cylinder (executionContext);
}

void
Cylinder::initialize ()
{
	X3DGeometryNode::initialize ();

	getBrowser () -> getCylinderOptions () .addInterest (this, &Cylinder::update);
}

void
Cylinder::setExecutionContext (X3DExecutionContext* const executionContext)
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	if (isInitialized ())
		getBrowser () -> getCylinderOptions () .removeInterest (this, &Cylinder::update);

	X3DGeometryNode::setExecutionContext (executionContext);

	if (isInitialized ())
		getBrowser () -> getCylinderOptions () .addInterest (this, &Cylinder::update);
}

Box3d
Cylinder::createBBox ()
{
	const double diameter = radius () * 2;

	if (not top () and not side () and not bottom ())
		return Box3d ();

	else if (not top () and not side ())
		return Box3d (Vector3d (diameter, 0, diameter), Vector3d (0, -height () / 2, 0));

	else if (not bottom () and not side ())
		return Box3d (Vector3d (diameter, 0, diameter), Vector3d (0, height () / 2, 0));

	else
		return Box3d (Vector3d (diameter, height (), diameter), Vector3d ());
}

void
Cylinder::build ()
{
	const auto & options    = getBrowser () -> getCylinderOptions ();
	const double vDimension = options -> vDimension ();

	getTexCoords () .emplace_back ();

	const double y1 = height () / 2;
	const double y2 = -y1;

	if (side ())
	{
		for (int32_t i = 0; i < vDimension; ++ i)
		{
			const double u1     = i / vDimension;
			const double theta1 = 2 * M_PI * u1;
			const auto   n1     = std::polar <double> (-1, theta1);
			const auto   p1     = n1 * double (radius () .getValue ());

			const double u2     = (i + 1) / vDimension;
			const double theta2 = 2 * M_PI * u2;
			const auto   n2     = std::polar <double> (-1, theta2);
			const auto   p2     = n2 * double (radius () .getValue ());

			// p1 - p4
			//  |   |
			// p2 - p3

			// p1
			getTexCoords () [0] .emplace_back (u1, 1, 0, 1);
			getNormals  () .emplace_back (n1 .imag (), 0, n1 .real ());
			getVertices () .emplace_back (p1 .imag (), y1, p1 .real ());

			// p2
			getTexCoords () [0] .emplace_back (u1, 0, 0, 1);
			getNormals  () .emplace_back (n1 .imag (), 0, n1 .real ());
			getVertices () .emplace_back (p1 .imag (), y2, p1 .real ());

			// p3
			getTexCoords () [0] .emplace_back (u2, 0, 0, 1);
			getNormals  () .emplace_back (n2 .imag (), 0, n2 .real ());
			getVertices () .emplace_back (p2 .imag (), y2, p2 .real ());

			// p4
			getTexCoords () [0] .emplace_back (u2, 1, 0, 1);
			getNormals  () .emplace_back (n2 .imag (), 0, n2 .real ());
			getVertices () .emplace_back (p2 .imag (), y1, p2 .real ());
		}

		addElements (GL_QUADS, vDimension * 4);
	}

	if (top ())
	{
		for (int32_t i = 0; i < vDimension; ++ i)
		{
			const double u     = i / vDimension;
			const double theta = 2 * M_PI * u;
			const auto   t     = std::polar <double> (-1, theta);
			const auto   p     = t * double (radius () .getValue ());

			getTexCoords () [0] .emplace_back ((t .imag () + 1) / 2, -(t .real () - 1) / 2, 0, 1);
			getNormals  () .emplace_back (0, 1, 0);
			getVertices () .emplace_back (p .imag (), y1, p .real ());
		}

		addElements (GL_POLYGON, vDimension);
	}

	if (bottom ())
	{
		for (int32_t i = vDimension - 1; i > -1; -- i)
		{
			const double u     = i / vDimension;
			const double theta = 2 * M_PI * u;
			const auto   t     = std::polar <double> (-1, theta);
			const auto   p     = t * double (radius () .getValue ());

			getTexCoords () [0] .emplace_back ((t .imag () + 1) / 2, (t .real () + 1) / 2, 0, 1);
			getNormals  () .emplace_back (0, -1, 0);
			getVertices () .emplace_back (p .imag (), y2, p .real ());
		}

		addElements (GL_POLYGON, vDimension);
	}

	setSolid (solid ());
}

SFNode
Cylinder::toPrimitive () const
throw (Error <NOT_SUPPORTED>,
       Error <DISPOSED>)
{
	const auto & options    = getBrowser () -> getCylinderOptions ();
	const double vDimension = options -> vDimension ();

	const auto texCoord = getExecutionContext () -> createNode <TextureCoordinate> ();
	const auto coord    = getExecutionContext () -> createNode <Coordinate> ();
	const auto geometry = getExecutionContext () -> createNode <IndexedFaceSet> ();

	geometry -> metadata ()    = metadata ();
	geometry -> solid ()       = solid ();
	geometry -> creaseAngle () = 1;
	geometry -> texCoord ()    = texCoord;
	geometry -> coord ()       = coord;

	const double y1 = height () / 2;
	const double y2 = -y1;

	if (top () or side ())
	{
		for (int32_t i = 0; i < vDimension; ++ i)
		{
			const double u     = i / vDimension;
			const double theta = 2 * M_PI * u;
			const auto  t     = std::polar <double> (-1, theta);
			const auto  p     = t * double (radius () .getValue ());

			if (top ())
				texCoord -> point () .emplace_back ((t .imag () + 1) / 2, -(t .real () - 1) / 2);

			coord -> point () .emplace_back (p .imag (), y1, p .real ());
		}
	}

	if (bottom () or side ())
	{
		for (int32_t i = 0; i < vDimension; ++ i)
		{
			const double u     = i / vDimension;
			const double theta = 2 * M_PI * u;
			const auto   t     = std::polar <double> (-1, theta);
			const auto   p     = t * double (radius () .getValue ());

			if (bottom ())
				texCoord -> point () .emplace_back ((t .imag () + 1) / 2, (t .real () + 1) / 2);

			coord -> point () .emplace_back (p .imag (), y2, p .real ());
		}
	}

	if (side ())
	{
		for (int32_t i = 0; i < vDimension; ++ i)
		{
			const double u = i / vDimension;

			texCoord -> point () .emplace_back (u, 1);
			texCoord -> point () .emplace_back (u, 0);
		}

		texCoord -> point () .emplace_back (1, 1);
		texCoord -> point () .emplace_back (1, 0);
	}

	int32_t t = 0;
	int32_t c = 0;

	if (top ())
	{
		for (int32_t i = 0; i < vDimension; ++ i)
		{
			geometry -> texCoordIndex () .emplace_back (i);
			geometry -> coordIndex ()    .emplace_back (i);
		}
		
		geometry -> texCoordIndex () .emplace_back (-1);
		geometry -> coordIndex ()    .emplace_back (-1);

		t += vDimension;
		c += vDimension;
	}
	else if (side ())
		c += vDimension;

	if (bottom ())
	{
		for (int32_t i = 0, ts = t + vDimension - 1, cs = c + vDimension - 1; i < vDimension; ++ i)
		{
			geometry -> texCoordIndex () .emplace_back (ts - i);
			geometry -> coordIndex ()    .emplace_back (cs - i);
		}

		geometry -> texCoordIndex () .emplace_back (-1);
		geometry -> coordIndex ()    .emplace_back (-1);
	
		t += vDimension;
	}

	if (side ())
	{
		for (int32_t i = 0, size = vDimension - 1; i < size; ++ i)
		{
			const int32_t i2 = i * 2;
		
			geometry -> texCoordIndex () .emplace_back (t + i2);
			geometry -> texCoordIndex () .emplace_back (t + i2 + 1);
			geometry -> texCoordIndex () .emplace_back (t + i2 + 3);
			geometry -> texCoordIndex () .emplace_back (t + i2 + 2);
			geometry -> texCoordIndex () .emplace_back (-1);

			geometry -> coordIndex () .emplace_back (i);
			geometry -> coordIndex () .emplace_back (i + vDimension);
			geometry -> coordIndex () .emplace_back (i + vDimension + 1);
			geometry -> coordIndex () .emplace_back (i + 1);
			geometry -> coordIndex () .emplace_back (-1);
		}

		const int32_t vDimension2 = vDimension * 2;

		geometry -> texCoordIndex () .emplace_back (t + vDimension2 - 2);
		geometry -> texCoordIndex () .emplace_back (t + vDimension2 - 1);
		geometry -> texCoordIndex () .emplace_back (t + vDimension2 + 1);
		geometry -> texCoordIndex () .emplace_back (t + vDimension2);
		geometry -> texCoordIndex () .emplace_back (-1);

		geometry -> coordIndex () .emplace_back (vDimension - 1);
		geometry -> coordIndex () .emplace_back (vDimension - 1 + vDimension);
		geometry -> coordIndex () .emplace_back (vDimension);
		geometry -> coordIndex () .emplace_back (0);
		geometry -> coordIndex () .emplace_back (-1);
	}

//	if (top ())
//	{
//		for (int32_t i = 0; i < vDimension; ++ i)
//		{
//			const double u     = i / vDimension;
//			const double theta = 2 * M_PI * u;
//			const double x     = -std::sin (theta);
//			const double z     = -std::cos (theta);
//
//			texCoord -> point () .emplace_back ((x + 1) / 2, -(z - 1) / 2);
//			coord -> point ()    .emplace_back (x * radius (), y1, z * radius ());
//		}
//	}
//
//	if (bottom ())
//	{
//		for (int32_t i = 0; i < vDimension; ++ i)
//		{
//			const double u     = i / vDimension;
//			const double theta = 2 * M_PI * u;
//			const double x     = -std::sin (theta);
//			const double z     = -std::cos (theta);
//
//			texCoord -> point () .emplace_back ((x + 1) / 2, (z + 1) / 2);
//			coord -> point ()    .emplace_back (x * radius (), y2, z * radius ());
//		}
//	}
//
//	if (side ())
//	{
//		for (int32_t i = 0; i < vDimension; ++ i)
//		{
//			const double u     = i / vDimension;
//			const double theta = 2 * M_PI * u;
//			const double x     = -std::sin (theta);
//			const double z     = -std::cos (theta);
//
//			texCoord -> point () .emplace_back (u, 1);
//			texCoord -> point () .emplace_back (u, 0);
//
//			coord -> point () .emplace_back (x * radius (), y1, z * radius ());
//			coord -> point () .emplace_back (x * radius (), y2, z * radius ());
//		}
//
//		texCoord -> point () .emplace_back (1, 1);
//		texCoord -> point () .emplace_back (1, 0);
//	}
//	
//	int32_t first = 0;
//
//	if (top ())
//	{
//		for (int32_t i = first; i < vDimension; ++ i)
//		{
//			geometry -> texCoordIndex () .emplace_back (i);
//			geometry -> coordIndex ()    .emplace_back (i);
//		}
//		
//		geometry -> texCoordIndex () .emplace_back (-1);
//		geometry -> coordIndex ()    .emplace_back (-1);
//		
//		first += vDimension;
//	}
//
//	if (bottom ())
//	{
//		for (int32_t i = first + vDimension - 1; i >= first; -- i)
//		{
//			geometry -> texCoordIndex () .emplace_back (i);
//			geometry -> coordIndex ()    .emplace_back (i);
//		}
//
//		geometry -> texCoordIndex () .emplace_back (-1);
//		geometry -> coordIndex ()    .emplace_back (-1);
//
//		first += vDimension;
//	}
//
//	if (side ())
//	{
//		const int32_t vDimension2 = vDimension * 2;
//	
//		for (int32_t i = 0, size = vDimension2 - 2; i < size; i += 2)
//		{
//			geometry -> texCoordIndex () .emplace_back (first + i);
//			geometry -> texCoordIndex () .emplace_back (first + i + 1);
//			geometry -> texCoordIndex () .emplace_back (first + i + 3);
//			geometry -> texCoordIndex () .emplace_back (first + i + 2);
//			geometry -> texCoordIndex () .emplace_back (-1);
//	
//			geometry -> coordIndex () .emplace_back (first + i);
//			geometry -> coordIndex () .emplace_back (first + i + 1);
//			geometry -> coordIndex () .emplace_back (first + i + 3);
//			geometry -> coordIndex () .emplace_back (first + i + 2);
//			geometry -> coordIndex () .emplace_back (-1);
//		}
//
//		geometry -> texCoordIndex () .emplace_back (first + vDimension2 - 2);
//		geometry -> texCoordIndex () .emplace_back (first + vDimension2 - 1);
//		geometry -> texCoordIndex () .emplace_back (first + vDimension2 + 1);
//		geometry -> texCoordIndex () .emplace_back (first + vDimension2);
//		geometry -> texCoordIndex () .emplace_back (-1);
//
//		geometry -> coordIndex () .emplace_back (first + vDimension2 - 2);
//		geometry -> coordIndex () .emplace_back (first + vDimension2 - 1);
//		geometry -> coordIndex () .emplace_back (first + 1);
//		geometry -> coordIndex () .emplace_back (first + 0);
//		geometry -> coordIndex () .emplace_back (-1);
//	}

	getExecutionContext () -> realize ();
	return SFNode (geometry);
}

} // X3D
} // titania
