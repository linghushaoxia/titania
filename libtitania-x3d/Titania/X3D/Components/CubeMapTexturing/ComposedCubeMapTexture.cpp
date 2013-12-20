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

#include "ComposedCubeMapTexture.h"

#include "../../Bits/Cast.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../Texturing/X3DTexture2DNode.h"

namespace titania {
namespace X3D {

const std::string ComposedCubeMapTexture::componentName  = "CubeMapTexturing";
const std::string ComposedCubeMapTexture::typeName       = "ComposedCubeMapTexture";
const std::string ComposedCubeMapTexture::containerField = "texture";

ComposedCubeMapTexture::Fields::Fields () :
	 front (new SFNode ()),
	  back (new SFNode ()),
	  left (new SFNode ()),
	 right (new SFNode ()),
	bottom (new SFNode ()),
	   top (new SFNode ())
{ }

ComposedCubeMapTexture::ComposedCubeMapTexture (X3DExecutionContext* const executionContext) :
	              X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DEnvironmentTextureNode (),
	                   fields (),
	              transparent (false),
	               components (0)
{
	addField (inputOutput, "metadata", metadata ());
	addField (inputOutput, "front",    front ());
	addField (inputOutput, "back",     back ());
	addField (inputOutput, "left",     left ());
	addField (inputOutput, "right",    right ());
	addField (inputOutput, "bottom",   bottom ());
	addField (inputOutput, "top",      top ());
}

X3DBaseNode*
ComposedCubeMapTexture::create (X3DExecutionContext* const executionContext) const
{
	return new ComposedCubeMapTexture (executionContext);
}

void
ComposedCubeMapTexture::initialize ()
{
	X3DEnvironmentTextureNode::initialize ();

	if (glXGetCurrentContext ())
	{
		glBindTexture (GL_TEXTURE_CUBE_MAP, getTextureId ());

		glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_REPEAT);
		glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glBindTexture (GL_TEXTURE_CUBE_MAP, 0);

		front ()    .addInterest (this, &ComposedCubeMapTexture::set_texture);
		back ()     .addInterest (this, &ComposedCubeMapTexture::set_texture);
		left ()     .addInterest (this, &ComposedCubeMapTexture::set_texture);
		right ()    .addInterest (this, &ComposedCubeMapTexture::set_texture);
		bottom ()   .addInterest (this, &ComposedCubeMapTexture::set_texture);
		top ()      .addInterest (this, &ComposedCubeMapTexture::set_texture);
		notified () .addInterest (this, &ComposedCubeMapTexture::set_texture);
	
		set_texture ();
	}
}

void
ComposedCubeMapTexture::set_texture ()
{
	transparent = false;
	components  = 0;

	setTexture (GL_TEXTURE_CUBE_MAP_POSITIVE_Z, front ());
	setTexture (GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, back ());
	setTexture (GL_TEXTURE_CUBE_MAP_NEGATIVE_X, right ());
	setTexture (GL_TEXTURE_CUBE_MAP_POSITIVE_X, left ());
	setTexture (GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, bottom ());
	setTexture (GL_TEXTURE_CUBE_MAP_POSITIVE_Y, top ());
}

void
ComposedCubeMapTexture::setTexture (GLenum target, const SFNode & node)
{
	auto texture = x3d_cast <X3DTexture2DNode*> (node);

	if (texture)
	{
		// Get texture 2d data

		transparent = transparent or texture -> isTransparent ();
		components  = std::max (components, texture -> getComponents ());

		size_t width  = texture -> getWidth ();
		size_t height = texture -> getHeight ();
	
		std::vector <char> image (width * height * 4);

		glBindTexture (GL_TEXTURE_2D, texture -> getTextureId ());
		glGetTexImage (GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, image .data ());
		glBindTexture (GL_TEXTURE_2D, 0);

		// Transfer image
		// Important: width and height must be equal, and all images must be of the same size!!!

		glBindTexture (GL_TEXTURE_CUBE_MAP, getTextureId ());
		glTexImage2D (target, 0, GL_RGBA, width, height, false, GL_RGBA, GL_UNSIGNED_BYTE, image .data ());
		glBindTexture (GL_TEXTURE_CUBE_MAP, 0);
	}
	else
	{
		glBindTexture (GL_TEXTURE_CUBE_MAP, getTextureId ());
		glTexImage2D (target, 0, GL_RGBA, 0, 0, false, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glBindTexture (GL_TEXTURE_CUBE_MAP, 0);
	}
}

void
ComposedCubeMapTexture::draw ()
{
	glMatrixMode (GL_TEXTURE);
	glScalef (-1, -1, 1);
	glMatrixMode (GL_MODELVIEW);

	X3DEnvironmentTextureNode::draw (GL_TEXTURE_CUBE_MAP, components);
}

} // X3D
} // titania
