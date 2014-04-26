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

#include "Disk2D.h"

#include "../../Browser/Geometry2D/Disk2DOptions.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

const std::string Disk2D::componentName  = "Geometry2D";
const std::string Disk2D::typeName       = "Disk2D";
const std::string Disk2D::containerField = "geometry";

Disk2D::Fields::Fields () :
	innerRadius (new SFFloat ()),
	outerRadius (new SFFloat (1)),
	      solid (new SFBool (true))
{ }

Disk2D::Disk2D (X3DExecutionContext* const executionContext) :
	    X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DGeometryNode (),
	         fields (),
	   lineGeometry (false)
{
	addField (inputOutput,    "metadata",    metadata ());
	addField (inputOutput,    "innerRadius", innerRadius ());
	addField (inputOutput,    "outerRadius", outerRadius ());
	addField (initializeOnly, "solid",       solid ());
}

X3DBaseNode*
Disk2D::create (X3DExecutionContext* const executionContext) const
{
	return new Disk2D (executionContext);
}

void
Disk2D::initialize ()
{
	X3DGeometryNode::initialize ();

	getBrowser () -> getDisk2DOptions () .addInterest (this, &Disk2D::update);
}

Box3f
Disk2D::createBBox ()
{
	const auto diameter = std::abs (std::max (innerRadius (), outerRadius ())) * 2;

	return Box3f (Vector3f (diameter, diameter, 0), Vector3f ());
}

void
Disk2D::build ()
{
	const Disk2DOptions* const options = getBrowser () -> getDisk2DOptions ();

	if (innerRadius () == outerRadius ())
	{
		// Circle

		const auto radius = std::abs (outerRadius ());

		if (radius == 1)
			getVertices () = options -> getVertices ();

		else
		{
			getVertices () .reserve (options -> getVertices () .size ());

			for (const auto & vertex : options -> getVertices ())
				getVertices () .emplace_back (vertex * radius);
		}

		addElements (GL_LINE_LOOP, getVertices () .size ());
		setSolid (false);

		lineGeometry = true;

		return;
	}

	if (innerRadius () == 0.0f or outerRadius () == 0.0f)
	{
		// Disk

		const auto radius = std::abs (std::max (innerRadius (), outerRadius ()));

		const size_t elements = solid () ? 1 : 2;

		getTexCoords () .emplace_back ();
		getTexCoords () [0] .reserve (elements * options -> getTexCoords () .size ());
		getTexCoords () [0] = options -> getTexCoords ();

		getNormals () .reserve (elements * options -> getNormals  () .size ());
		getNormals () = options -> getNormals  ();

		getVertices () .reserve (elements * options -> getVertices () .size ());

		if (radius == 1)
			getVertices () = options -> getVertices ();

		else
		{
			getVertices () .reserve (options -> getVertices () .size ());

			for (const auto & vertex : options -> getVertices ())
				getVertices () .emplace_back (vertex * radius);
		}

		addElements (options -> getVertexMode (), getVertices () .size ());
		setSolid (true);
		setTextureCoordinate (nullptr);

		if (not solid ())
			addMirrorVertices (options -> getVertexMode (), true);

		lineGeometry = false;

		return;
	}

	// Disk with hole

	const size_t elements = solid () ? 1 : 2;

	getTexCoords () .emplace_back ();
	getTexCoords () [0] .reserve (elements * (options -> getTexCoords () .size () + 2));

	getNormals  () .reserve (elements * (options -> getNormals  () .size () + 2));
	getVertices () .reserve (elements * (options -> getVertices () .size () + 2));

	// Texture Coordinates

	const auto maxRadius = std::abs (std::max (innerRadius (), outerRadius ()));
	const auto minRadius = std::abs (std::min (innerRadius (), outerRadius ()));
	const auto scale     = minRadius / maxRadius;

	for (const auto & texCoord : options -> getTexCoords ())
	{
		getTexCoords () [0] .emplace_back (texCoord .x () * scale + (1 - scale) / 2, texCoord .y () * scale + (1 - scale) / 2, 0, 1);
		getTexCoords () [0] .emplace_back (texCoord);
	}

	// Normals

	for (const auto & normal : options -> getNormals ())
	{
		getNormals () .emplace_back (normal);
		getNormals () .emplace_back (normal);
	}

	// Vertices

	for (const auto & vertex : options -> getVertices ())
	{
		getVertices () .emplace_back (vertex * minRadius);
		getVertices () .emplace_back (vertex * maxRadius);
	}

	// The last two vertices are the first two.
	getTexCoords () [0] .emplace_back (getTexCoords () [0] [0]);
	getTexCoords () [0] .emplace_back (getTexCoords () [0] [1]);
	getNormals  () .emplace_back (getNormals  () [0]);
	getNormals  () .emplace_back (getNormals  () [1]);
	getVertices () .emplace_back (getVertices () [0]);
	getVertices () .emplace_back (getVertices () [1]);

	addElements (GL_QUAD_STRIP, getVertices () .size ());
	setSolid (true);
	setTextureCoordinate (nullptr);

	if (not solid ())
		addMirrorVertices (GL_QUAD_STRIP, true);

	lineGeometry = false;
}

void
Disk2D::draw ()
{
	if (innerRadius () == outerRadius ())
		glDisable (GL_LIGHTING);

	X3DGeometryNode::draw ();
}

} // X3D
} // titania
