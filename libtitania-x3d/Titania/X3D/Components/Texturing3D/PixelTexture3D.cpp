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

#include "PixelTexture3D.h"

#include "../../Browser/ContextLock.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

constexpr size_t OFFSET     = 4;
constexpr size_t COMPONENTS = 0;
constexpr size_t WIDTH      = 1;
constexpr size_t HEIGHT     = 2;
constexpr size_t DEPTH      = 3;

const ComponentType PixelTexture3D::component      = ComponentType::TEXTURING_3D;
const std::string   PixelTexture3D::typeName       = "PixelTexture3D";
const std::string   PixelTexture3D::containerField = "texture";

PixelTexture3D::Fields::Fields () :
	image (new MFInt32 ({ 0, 0, 0, 0 }))
{ }

PixelTexture3D::PixelTexture3D (X3DExecutionContext* const executionContext) :
	     X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DTexture3DNode (),
	          fields (),
	       loadState (NOT_STARTED_STATE)
{
	addType (X3DConstants::PixelTexture3D);

	addField (inputOutput,    "metadata",          metadata ());
	addField (initializeOnly, "repeatS",           repeatS ());
	addField (initializeOnly, "repeatT",           repeatT ());
	addField (initializeOnly, "repeatR",           repeatR ());
	addField (inputOutput,    "image",             image ());
	addField (initializeOnly, "textureProperties", textureProperties ());
	
	addChildObjects (loadState);
}

X3DBaseNode*
PixelTexture3D::create (X3DExecutionContext* const executionContext) const
{
	return new PixelTexture3D (executionContext);
}

void
PixelTexture3D::initialize ()
{
	X3DTexture3DNode::initialize ();

	image () .addInterest (&PixelTexture3D::update, this);

	update ();
}

size_t
PixelTexture3D::getImageWidth () const
{
	if (image () .size () > WIDTH)
		return std::max <int32_t> (0, image () [WIDTH]);

	return 0;
}

size_t
PixelTexture3D::getImageHeight () const
{
	if (image () .size () > HEIGHT)
		return std::max <int32_t> (0, image () [HEIGHT]);

	return 0;
}

void
PixelTexture3D::update ()
{
	if (image () .size () < OFFSET)
	{
		setTexture (Texture3DPtr ());
		return;	
	}

	const size_t components  = std::max <int32_t> (0, image () [COMPONENTS]);
	const size_t width       = std::max <int32_t> (0, image () [WIDTH]);
	const size_t height      = std::max <int32_t> (0, image () [HEIGHT]);
	const size_t depth       = std::max <int32_t> (0, image () [DEPTH]);
	const size_t size        = width * height;
	const size_t size3D      = width * height * depth;

	if (components < 1 or components > 4 or width < 1 or height < 1 or depth < 1)
	{
		setTexture (Texture3DPtr ());
		return;
	}

	if (image () .size () < OFFSET + size3D)
		image () .resize (OFFSET + size3D);

	MagickImageArrayPtr mimages (new MagickImageArray ());

	for (size_t d = 0; d < depth; ++ d)
	{
		const size_t first = OFFSET + d * size;
	
		mimages -> emplace_back ();
		mimages -> back () .depth (8);
		mimages -> back () .size (Magick::Geometry (width, height));

		switch (components)
		{
			case 1:
			{
				// Copy and flip image vertically.

				std::vector <uint8_t> pixels;
				pixels .reserve (size);

				for (size_t h = 0; h < height; ++ h)
				{
					const auto row = h * width;

					for (size_t w = 0; w < width; ++ w)
						pixels .emplace_back (image () [first + row + w]);
				}

				Magick::Blob blob (pixels .data (), pixels .size ());
				mimages -> back () .magick ("GRAY");
				mimages -> back () .read (blob);
				break;
			}
			case 2:
			{
				// Copy and flip image vertically.

				constexpr auto components = 4;
			
				std::vector <uint8_t> pixels;
				pixels .reserve (size * components);

				for (size_t h = 0; h < height; ++ h)
				{
					const auto row = h * width;

					for (size_t w = 0; w < width; ++ w)
					{
						const auto & pixel = image () [first + row + w];

						const uint8_t color = pixel >> 8;
						pixels .emplace_back (color);
						pixels .emplace_back (color);
						pixels .emplace_back (color);
						pixels .emplace_back (pixel);
					}
				}

				Magick::Blob blob (pixels .data (), pixels .size ());
				mimages -> back () .magick ("RGBA");
				mimages -> back () .read (blob);
				break;
			}
			case 3:
			{
				// Copy and flip image vertically.

				std::vector <uint8_t> pixels;
				pixels .reserve (size * components);

				for (size_t h = 0; h < height; ++ h)
				{
					const auto row = h * width;

					for (size_t w = 0; w < width; ++ w)
					{
						const auto & pixel = image () [first + row + w];
				
						pixels .emplace_back (pixel >> 16);
						pixels .emplace_back (pixel >> 8);
						pixels .emplace_back (pixel);
					}
				}

				Magick::Blob blob (pixels .data (), pixels .size ());
				mimages -> back () .magick ("RGB");
				mimages -> back () .read (blob);
				break;
			}
			case 4:
			{
				// Copy and flip image vertically.

				std::vector <uint8_t> pixels;
				pixels .reserve (size * components);

				for (size_t h = 0; h < height; ++ h)
				{
					const auto row = h * width;

					for (size_t w = 0; w < width; ++ w)
					{
						const auto & pixel = image () [first + row + w];
				
						pixels .emplace_back (pixel >> 24);
						pixels .emplace_back (pixel >> 16);
						pixels .emplace_back (pixel >> 8);
						pixels .emplace_back (pixel);
					}
				}

				Magick::Blob blob (pixels .data (), pixels .size ());
				mimages -> back () .magick ("RGBA");
				mimages -> back () .read (blob);
				break;
			}
			default:
				break;
		}
	}

	Texture3DPtr texture (new Texture3D (std::move (mimages)));

	texture -> process (getBrowser () -> getMinTextureSize (),
	                    getBrowser () -> getMaxTextureSize ());

	texture -> setComponents (components);

	setTexture (texture);

	loadState = COMPLETE_STATE;
}

void
PixelTexture3D::setImage (const X3D::X3DPtr <X3D::X3DTexture3DNode> & texture3DNode)
throw (Error <INVALID_NODE>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	if (not texture3DNode)
		throw Error <INVALID_NODE> ("Node is NULL.");

	ContextLock lock (texture3DNode -> getBrowser ());

	const auto   width      = texture3DNode -> getWidth ();
	const auto   height     = texture3DNode -> getHeight ();
	const auto   depth      = texture3DNode -> getDepth ();
	const auto   components = texture3DNode -> getComponents ();

	X3D::MFInt32 & array = image ();

	array .resize (OFFSET);

	array [WIDTH]      = width;
	array [HEIGHT]     = height;
	array [DEPTH]      = depth;
	array [COMPONENTS] = components;

	switch (components)
	{
		case 1:
		{
			// Copy and flip image vertically.

			const auto stride    = 3;
			const auto rowStride = width * stride;
			const auto size      = width * height * stride;

			std::vector <uint8_t> images (size * depth);

			glBindTexture (GL_TEXTURE_3D, texture3DNode -> getTextureId ());
			glGetTexImage (GL_TEXTURE_3D, 0, GL_RGB, GL_UNSIGNED_BYTE, images .data ());
			glBindTexture (GL_TEXTURE_3D, 0);

			for (size_t i = 0; i < depth; ++ i)
			{
				const uint8_t* first = images .data () + i * size;
		
				for (size_t h = 0; h < height; ++ h)
				{
					const auto row = h * rowStride;
	
					for (size_t w = 0; w < rowStride; w += stride)
					{
						auto p = first + (row + w);
	
						array .emplace_back (*p);
					}
				}
			}

			break;
		}
		case 2:
		{
			// Copy and flip image vertically.

			const auto stride    = 4;
			const auto rowStride = width * stride;
			const auto size      = width * height * stride;

			std::vector <uint8_t> images (size * depth);

			glBindTexture (GL_TEXTURE_3D, texture3DNode -> getTextureId ());
			glGetTexImage (GL_TEXTURE_3D, 0, GL_RGBA, GL_UNSIGNED_BYTE, images .data ());
			glBindTexture (GL_TEXTURE_3D, 0);

			for (size_t i = 0; i < depth; ++ i)
			{
				const uint8_t* first = images .data () + i * size;
	
				for (size_t h = 0; h < height; ++ h)
				{
					const auto row = h * rowStride;
	
					for (size_t w = 0; w < rowStride; w += stride)
					{
						auto p = first + (row + w);
	
						uint32_t point = *p << 8; // The value is in the red channel.
						p     += 3;
						point |= *p;
	
						array .emplace_back (point);
					}
				}
			}

			break;
		}
		case 3:
		{
			// Copy and flip image vertically.

			const auto stride    = components;
			const auto rowStride = width * stride;
			const auto size      = width * height * stride;

			std::vector <uint8_t> images (size * depth);

			glBindTexture (GL_TEXTURE_3D, texture3DNode -> getTextureId ());
			glGetTexImage (GL_TEXTURE_3D, 0, GL_RGB, GL_UNSIGNED_BYTE, images .data ());
			glBindTexture (GL_TEXTURE_3D, 0);

			for (size_t i = 0; i < depth; ++ i)
			{
				const uint8_t* first = images .data () + i * size;
	
				for (size_t h = 0; h < height; ++ h)
				{
					const auto row = h * rowStride;
	
					for (size_t w = 0; w < rowStride; w += stride)
					{
						auto p = first + (row + w);
	
						uint32_t point = *p ++ << 16;
						point |= *p ++ << 8;
						point |= *p;
	
						array .emplace_back (point);
					}
				}
			}

			break;
		}
		case 4:
		{
			// Copy and flip image vertically.

			const auto stride    = components;
			const auto rowStride = width * stride;
			const auto size      = width * height * stride;

			std::vector <uint8_t> images (size * depth);

			glBindTexture (GL_TEXTURE_3D, texture3DNode -> getTextureId ());
			glGetTexImage (GL_TEXTURE_3D, 0, GL_RGBA, GL_UNSIGNED_BYTE, images .data ());
			glBindTexture (GL_TEXTURE_3D, 0);

			for (size_t i = 0; i < depth; ++ i)
			{
				const uint8_t* first = images .data () + i * size;
	
				for (size_t h = 0; h < height; ++ h)
				{
					const auto row = h * rowStride;
	
					for (size_t w = 0; w < rowStride; w += stride)
					{
						auto p = first + (row + w);
	
						uint32_t point = *p ++ << 24;
						point |= *p ++ << 16;
						point |= *p ++ << 8;
						point |= *p;
	
						array .emplace_back (point);
					}
				}
			}

			break;
		}
		default:
			break;
	}
}

} // X3D
} // titania
