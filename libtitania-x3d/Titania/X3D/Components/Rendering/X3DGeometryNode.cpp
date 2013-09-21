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

#include "X3DGeometryNode.h"

#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

X3DGeometryNode::X3DGeometryNode () :
	                   X3DNode (),
	                      bbox (),
	                 texCoords (),
	textureCoordinateGenerator (nullptr),
	                    colors (),
	                colorsRGBA (),
	                   normals (),
	                  vertices (),
	                     solid (true),
	                       ccw (GL_CCW),
	                  elements ()
{
	addNodeType (X3DConstants::X3DGeometryNode);
}

void
X3DGeometryNode::setup ()
{
	X3DNode::setup ();

	// Update only initalized nodes
	if (not getExecutionContext () -> isProto ())
		update ();
}

void
X3DGeometryNode::eventsProcessed ()
{
	X3DNode::eventsProcessed ();

	update ();
}

const Box3f
X3DGeometryNode::getBBox ()
{
	if (bbox == Box3f ())
		return bbox = createBBox ();

	return bbox;
}

Box3f
X3DGeometryNode::createBBox ()
{
	if (vertices .size ())
	{
		Vector3f min = vertices [0];
		Vector3f max = min;

		for (const auto & vertex : vertices)
		{
			min = math::min (min, vertex);
			max = math::max (max, vertex);
		}

		return Box3f (min, max, true);
	}

	return Box3f ();
}

bool
X3DGeometryNode::intersect (const Line3f & line, std::deque <IntersectionPtr> & intersections) const
{
	bool     intersected = false;
	Vector3f temp;

	if (bbox .intersect (line, temp))
	{
		float u, v, t;

		size_t first = 0;

		for (const auto & element : elements)
		{
			switch (element .vertexMode)
			{
				case GL_TRIANGLES :
					{
						for (size_t i = first, size = first + element .count; i < size; i += 3)
						{
							if (line .intersect (vertices [i], vertices [i + 1], vertices [i + 2], u, v, t))
							{
								Vector3f texCoord = i < texCoords .size () ? (1 - u - v) * texCoords [i] + u * texCoords [i + 1] + v * texCoords [i + 2] : Vector3f ();

								intersections .emplace_back (new Intersection { texCoord,
								                                                (1 - u - v) * normals  [i] + u * normals  [i + 1] + v * normals  [i + 2],
								                                                (1 - u - v) * vertices [i] + u * vertices [i + 1] + v * vertices [i + 2] });
								intersected = true;
							}
						}

						break;
					}
				case GL_QUADS:
				{
					for (size_t i = first, size = first + element .count; i < size; i += 4)
					{
						if (line .intersect (vertices [i], vertices [i + 1], vertices [i + 2], u, v, t))
						{
							Vector3f texCoord = i < texCoords .size () ? (1 - u - v) * texCoords [i] + u * texCoords [i + 1] + v * texCoords [i + 2] : Vector3f ();

							intersections .emplace_back (new Intersection { texCoord,
							                                                (1 - u - v) * normals  [i] + u * normals  [i + 1] + v * normals  [i + 2],
							                                                (1 - u - v) * vertices [i] + u * vertices [i + 1] + v * vertices [i + 2] });
							intersected = true;
						}

						if (line .intersect (vertices [i], vertices [i + 2], vertices [i + 3], u, v, t))
						{
							Vector3f texCoord = i < texCoords .size () ? (1 - u - v) * texCoords [i] + u * texCoords [i + 2] + v * texCoords [i + 3] : Vector3f ();

							intersections .emplace_back (new Intersection { texCoord,
							                                                (1 - u - v) * normals  [i] + u * normals  [i + 2] + v * normals  [i + 3],
							                                                (1 - u - v) * vertices [i] + u * vertices [i + 2] + v * vertices [i + 3] });
							intersected = true;
						}
					}

					break;
				}
				case GL_POLYGON:
				{
					for (int32_t i = first + 1, size = first + element .count - 1; i < size; ++ i)
					{
						if (line .intersect (vertices [first], vertices [i], vertices [i + 1], u, v, t))
						{
							Vector3f texCoord = (size_t) i < texCoords .size () ? (1 - u - v) * texCoords [first] + u * texCoords [i] + v * texCoords [i + 1] : Vector3f ();

							intersections .emplace_back (new Intersection { texCoord,
							                                                (1 - u - v) * normals  [first] + u * normals  [i] + v * normals  [i + 1],
							                                                (1 - u - v) * vertices [first] + u * vertices [i] + v * vertices [i + 1] });
							intersected = true;
						}
					}

					break;
				}
				default:
					break;
			}

			first += element .count;
		}
	}

	return intersected;
}

bool
X3DGeometryNode::intersect (const Matrix4f & matrix, const Sphere3f & sphere) const
{
	if ((bbox * matrix) .intersect (sphere))
	{
		size_t first = 0;

		for (const auto & element : elements)
		{
			switch (element .vertexMode)
			{
				case GL_TRIANGLES :
					{
						for (size_t i = first, size = first + element .count; i < size; i += 3)
						{
							if (sphere .intersect (vertices [i] * matrix, vertices [i + 1] * matrix, vertices [i + 2] * matrix))
								return true;
						}

						break;
					}
				case GL_QUADS:
				{
					for (size_t i = first, size = first + element .count; i < size; i += 4)
					{
						if (sphere .intersect (vertices [i] * matrix, vertices [i + 1] * matrix, vertices [i + 2] * matrix))
							return true;

						if (sphere .intersect (vertices [i] * matrix, vertices [i + 2] * matrix, vertices [i + 3] * matrix))
							return true;
					}

					break;
				}
				case GL_POLYGON:
				{
					for (int32_t i = first + 1, size = first + element .count - 1; i < size; ++ i)
					{
						if (sphere .intersect (vertices [first] * matrix, vertices [i] * matrix, vertices [i + 1] * matrix))
							return true;
					}

					break;
				}
				default:
					break;
			}

			first += element .count;
		}
	}

	return false;
}

void
X3DGeometryNode::buildTexCoord ()
{
	Vector3f min;

	float Ssize;
	int   Sindex, Tindex;

	getTexCoordParams (min, Ssize, Sindex, Tindex);

	getTexCoord () .reserve (getVertices () .size ());

	for (const auto & vertex : getVertices ())
	{
		getTexCoord () .emplace_back ((vertex [Sindex] - min [Sindex]) / Ssize,
		                              (vertex [Tindex] - min [Tindex]) / Ssize,
		                              0);
	}
}

void
X3DGeometryNode::getTexCoordParams (Vector3f & min, float & Ssize, int & Sindex, int & Tindex)
{
	Box3f bbox = getBBox ();

	min = bbox .min ();

	float Xsize = bbox .size () .x ();
	float Ysize = bbox .size () .y ();
	float Zsize = bbox .size () .z ();

	if ((Xsize >= Ysize) and (Xsize >= Zsize))
	{
		// X size largest
		Ssize = Xsize; Sindex = 0;

		if (Ysize >= Zsize)
			Tindex = 1;
		else
			Tindex = 2;
	}
	else if ((Ysize >= Xsize) and (Ysize >= Zsize))
	{
		// Y size largest
		Ssize = Ysize; Sindex = 1;

		if (Xsize >= Zsize)
			Tindex = 0;
		else
			Tindex = 2;
	}
	else
	{
		// Z is the largest
		Ssize = Zsize; Sindex = 2;

		if (Xsize >= Ysize)
			Tindex = 0;
		else
			Tindex = 1;
	}
}

/*
 *  normalIndex: a map of vertices with an array of the normals associated to this vertex
 *
 *    [vertexIndex] -> [normalIndex1, normalIndex2, ... ]
 *
 *  normals: an array of a face normal for each vertex
 *
 *  Assume we have two polygons where two points (p2, p3) share more than one vertex.
 *
 *  p1                        p3
 *     v1 ------------- v3 v5
 *      | n1         n3  /|
 *      |              /  |
 *      |            / n5 |
 *      |          /      |
 *      |        /        |
 *      |      /          |
 *      | n2 /            |
 *      |  /  n4          |
 *      |/            n6  |
 *     v2 v4 ------------- v6
 *  p2                        p4
 *
 *  For these two polygons the normalIndex and the normal array would look like this:
 *
 *  normalIndex:
 *    [p1] -> [n1]
 *    [p2] -> [n2, n4]
 *    [p3] -> [n3, n5]
 *    [p4] -> [n6]
 *
 *  normals:
 *    [n1, n2, n3, n4, n5, n6]
 */

void
X3DGeometryNode::refineNormals (const NormalIndex & normalIndex, std::vector <Vector3f> & normals, float creaseAngle, bool ccw)
{
	if (not ccw)
	{
		for (auto & normal : normals)
			normal .negate ();
	}

	if (creaseAngle == 0.0f)
		return;

	float cosCreaseAngle = std::cos (creaseAngle);

	std::vector <Vector3f> _normals (normals .size ());

	for (const auto & point : normalIndex)
	{
		for (const auto & index : point .second)
		{
			Vector3f         n;
			const Vector3f & m = normals [index];

			for (const auto & index : point .second)
			{
				if (dot (normals [index], m) >= cosCreaseAngle)
					n += normals [index];
			}

			_normals [index] = n .normalize ();
		}
	}

	normals .swap (_normals);
}

/*
 * Adds all vertices, normals and texCoordss mirrors onto the XY-plane to the arrays.
 * If the shape is not convext, the this not convex one point must be the first point in the arrays.
 */

void
X3DGeometryNode::addMirrorVertices (GLenum vertexMode, const bool convex)
{
	addElements (vertexMode, getVertices () .size ());

	switch (vertexMode)
	{
		case GL_QUAD_STRIP:
		{
			for (int32_t i = getTexCoord () .size () - 2; i >= 0; i -= 2)
			{
				const auto & texCoord1 = getTexCoord () [i];
				const auto & texCoord0 = getTexCoord () [i + 1];
				getTexCoord () .emplace_back (1 - texCoord1 .x (), texCoord1 .y (), texCoord1 .z ());
				getTexCoord () .emplace_back (1 - texCoord0 .x (), texCoord0 .y (), texCoord0 .z ());
			}

			for (int32_t i = getVertices () .size () - 2; i >= 0; i -= 2)
			{
				getNormals  () .emplace_back (0, 0, -1);
				getNormals  () .emplace_back (0, 0, -1);
				getVertices () .emplace_back (getVertices () [i]);
				getVertices () .emplace_back (getVertices () [i + 1]);
			}

			break;
		}

		default:
		{
			size_t offset = convex ? 0 : 1;

			if (not convex)
			{
				getTexCoord () .emplace_back (1 - getTexCoord () .front () .x (), getTexCoord () .front () .y (), getTexCoord () .front () .z ());
				getNormals  () .emplace_back (0, 0, -1);
				getVertices () .emplace_back (getVertices () .front ());
			}

			for (const auto & texCoord : basic::adapter (getTexCoord () .crbegin () + offset, getTexCoord () .crend () - offset))
			{
				getTexCoord () .emplace_back (1 - texCoord .x (), texCoord .y (), texCoord .z ());
			}

			for (const auto & vertex : basic::adapter (getVertices () .crbegin () + offset, getVertices () .crend () - offset))
			{
				getNormals  () .emplace_back (0, 0, -1);
				getVertices () .emplace_back (vertex);
			}
		}
	}
}

void
X3DGeometryNode::update ()
{
	clear ();
	build ();
}

void
X3DGeometryNode::clear ()
{
	bbox = Box3f ();

	texCoords .clear ();
	textureCoordinateGenerator = nullptr;
	colors     .clear ();
	colorsRGBA .clear ();
	normals    .clear ();
	vertices   .clear ();
	elements   .clear ();
}

void
X3DGeometryNode::draw ()
{
	draw (solid, glIsEnabled (GL_TEXTURE_2D) or glIsEnabled (GL_TEXTURE_CUBE_MAP), glIsEnabled (GL_LIGHTING));
}

void
X3DGeometryNode::draw (bool solid, bool texture, bool lighting)
{
	if (not vertices .size ())
		return;

	if (solid)
		glEnable (GL_CULL_FACE);

	else
		glDisable (GL_CULL_FACE);

	glFrontFace (ccw);

	if (texture)
	{
		if (textureCoordinateGenerator)
			textureCoordinateGenerator -> enable ();

		else if (texCoords .size ())
		{
			glEnableClientState (GL_TEXTURE_COORD_ARRAY);
			glTexCoordPointer (3, GL_FLOAT, 0, texCoords .data ());
		}
	}

	if (colors .size ())
	{
		if (lighting)
			glEnable (GL_COLOR_MATERIAL);

		glEnableClientState (GL_COLOR_ARRAY);
		glColorPointer (3, GL_FLOAT, 0, colors .data ());
	}
	else if (colorsRGBA .size ())
	{
		if (lighting)
			glEnable (GL_COLOR_MATERIAL);

		glEnableClientState (GL_COLOR_ARRAY);
		glColorPointer (4, GL_FLOAT, 0, colorsRGBA .data ());
	}

	if (lighting)
	{
		if (normals .size ())
		{
			glEnableClientState (GL_NORMAL_ARRAY);
			glNormalPointer (GL_FLOAT, 0, normals .data ());
		}
	}

	glEnableClientState (GL_VERTEX_ARRAY);
	glVertexPointer (3, GL_FLOAT, 0, vertices .data ());

	size_t first = 0;

	for (const auto & element : elements)
	{
		glDrawArrays (element .vertexMode, first, element .count);
		first += element .count;
	}

	if (textureCoordinateGenerator)
		textureCoordinateGenerator -> disable ();

	glDisableClientState (GL_TEXTURE_COORD_ARRAY);
	glDisableClientState (GL_COLOR_ARRAY);
	glDisableClientState (GL_NORMAL_ARRAY);
	glDisableClientState (GL_VERTEX_ARRAY);
}

} // X3D
} // titania
