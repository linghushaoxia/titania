/*
 * FTGL - OpenGL font library
 *
 * Copyright (c) 2001-2004 Henry Maddocks <ftgl@opengl.geek.nz>
 * Copyright (c) 2008 Éric Beets <ericbeets@free.fr>
 * Copyright (c) 2008 Sam Hocevar <sam@zoy.org>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "PolygonGlyph.h"

#include "../Vectorizer.h"

namespace titania {
namespace FTGL {

//
//  PolygonGlyph
//

PolygonGlyph::PolygonGlyph (FT_GlyphSlot glyph, const double outset, const size_t bezierSteps) :
	     Glyph (glyph),
	vectoriser (new Vectorizer (glyph, bezierSteps)),
	    outset (outset)
{
	if (glyph -> format not_eq ft_glyph_format_outline)
	{
		setError (0x14); // Invalid_Outline
		return;
	}
}

const Vector3d &
PolygonGlyph::triangulate (const Vector3d & pen,
                           std::vector <size_t> & indices,
                           std::vector <Vector3d> & points) const
{
	vectoriser -> triangulate (1, 1, outset, pen, indices, points);

	return getAdvance ();
}

PolygonGlyph::~PolygonGlyph ()
{ }

} // FTGL
} // titania
