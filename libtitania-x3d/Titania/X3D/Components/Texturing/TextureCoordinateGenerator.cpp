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

#include "TextureCoordinateGenerator.h"

#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

const ComponentType TextureCoordinateGenerator::component      = ComponentType::TEXTURING;
const std::string   TextureCoordinateGenerator::typeName       = "TextureCoordinateGenerator";
const std::string   TextureCoordinateGenerator::containerField = "texCoord";

TextureCoordinateGenerator::Fields::Fields () :
	     mode (new SFString ("SPHERE")),
	parameter (new MFFloat ())
{ }

TextureCoordinateGenerator::TextureCoordinateGenerator (X3DExecutionContext* const executionContext) :
	             X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DTextureCoordinateNode (),
	                  fields (),
	                modeType (SPHERE)
{
	addType (X3DConstants::TextureCoordinateGenerator);

	addField (inputOutput, "metadata",  metadata ());
	addField (inputOutput, "mode",      mode ());
	addField (inputOutput, "parameter", parameter ());
}

X3DBaseNode*
TextureCoordinateGenerator::create (X3DExecutionContext* const executionContext) const
{
	return new TextureCoordinateGenerator (executionContext);
}

void
TextureCoordinateGenerator::initialize ()
{
	X3DTextureCoordinateNode::initialize ();

	mode () .addInterest (this, &TextureCoordinateGenerator::set_mode);

	set_mode ();
}
//	static const std::map <std::string, GLenum> modes = {
//		std::make_pair ("SPHERE",                      GL_SPHERE_MAP),     // Ok
//		std::make_pair ("SPHERE-LOCAL",                GL_SPHERE_MAP),     // Not supported
//		std::make_pair ("SPHERE-REFLECT",              GL_REFLECTION_MAP), // Not supported
//		std::make_pair ("SPHERE-REFLECT-LOCAL",        GL_REFLECTION_MAP), // Not supported
//		std::make_pair ("CAMERASPACENORMAL",           GL_NORMAL_MAP),     // Ok
//		std::make_pair ("CAMERASPACEPOSITION",         GL_EYE_LINEAR),     // Like COORD-EYE ???
//		std::make_pair ("CAMERASPACEREFLECTIONVECTOR", GL_REFLECTION_MAP), // Ok
//		std::make_pair ("COORD-EYE",                   GL_EYE_LINEAR),     // Ok
//		std::make_pair ("COORD",                       GL_OBJECT_LINEAR),  // Ok
//		std::make_pair ("NOISE-EYE",                   GL_SPHERE_MAP),     // Not supported
//		std::make_pair ("NOISE",                       GL_SPHERE_MAP)      // Not supported
//	};

void
TextureCoordinateGenerator::set_mode ()
{
	static const std::map <std::string, ModeType> modes = {
		std::make_pair ("SPHERE",                      SPHERE),
		std::make_pair ("SPHERE-LOCAL",                SPHERE_LOCAL),
		std::make_pair ("SPHERE-REFLECT",              SPHERE_REFLECT),
		std::make_pair ("SPHERE-REFLECT-LOCAL",        SPHERE_REFLECT_LOCAL),
		std::make_pair ("CAMERASPACENORMAL",           CAMERASPACENORMAL),
		std::make_pair ("CAMERASPACEPOSITION",         CAMERASPACEPOSITION),
		std::make_pair ("CAMERASPACEREFLECTIONVECTOR", CAMERASPACEREFLECTIONVECTOR),
		std::make_pair ("COORD-EYE",                   COORD_EYE),
		std::make_pair ("COORD",                       COORD),
		std::make_pair ("NOISE-EYE",                   NOISE_EYE),
		std::make_pair ("NOISE",                       NOISE)
	};

	try
	{
		modeType = modes .at (mode ());
	}
	catch (const std::out_of_range &)
	{
		modeType = SPHERE;
	}
}

void
TextureCoordinateGenerator::init (TexCoordArray & texCoords, const size_t) const
{
	texCoords .emplace_back ();
}

void
TextureCoordinateGenerator::enable (const int32_t unit, const size_t, const std::vector <GLuint> &) const
{
	glActiveTexture (GL_TEXTURE0 + unit);

	static constexpr Vector4f sPlane (1, 0, 0, 0);
	static constexpr Vector4f tPlane (0, 1, 0, 0);
	static constexpr Vector4f rPlane (0, 0, 0, 0);
	static constexpr Vector4f qPlane (0, 0, 0, 0);

	switch (modeType)
	{
		case SPHERE:
		{
			// Ok
			glTexGeni (GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
			glTexGeni (GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
			glEnable (GL_TEXTURE_GEN_S);
			glEnable (GL_TEXTURE_GEN_T);
			break;
		}
		case SPHERE_LOCAL:
		{
			// Not supported
			glTexGeni (GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
			glTexGeni (GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
			glEnable (GL_TEXTURE_GEN_S);
			glEnable (GL_TEXTURE_GEN_T);
			break;
		}
		case SPHERE_REFLECT:
		{
			// Not supported
			glTexGeni (GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
			glTexGeni (GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
			glEnable (GL_TEXTURE_GEN_S);
			glEnable (GL_TEXTURE_GEN_T);
			break;
		}
		case SPHERE_REFLECT_LOCAL:
		{
			// Not supported
			glTexGeni (GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
			glTexGeni (GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
			glEnable (GL_TEXTURE_GEN_S);
			glEnable (GL_TEXTURE_GEN_T);
			break;
		}
		case CAMERASPACENORMAL:
		{
			// Ok
			glTexGeni (GL_S, GL_TEXTURE_GEN_MODE, GL_NORMAL_MAP);
			glTexGeni (GL_T, GL_TEXTURE_GEN_MODE, GL_NORMAL_MAP);
			glTexGeni (GL_R, GL_TEXTURE_GEN_MODE, GL_NORMAL_MAP);
			glEnable (GL_TEXTURE_GEN_S);
			glEnable (GL_TEXTURE_GEN_T);
			glEnable (GL_TEXTURE_GEN_R);
			break;
		}
		case CAMERASPACEPOSITION:
		{
			// Not supported
			glTexGeni (GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
			glTexGeni (GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
			glTexGeni (GL_R, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
			//glTexGeni (GL_Q, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
			glEnable (GL_TEXTURE_GEN_S);
			glEnable (GL_TEXTURE_GEN_T);
			glEnable (GL_TEXTURE_GEN_R);
			//glEnable (GL_TEXTURE_GEN_Q);
			break;
		}
		case CAMERASPACEREFLECTIONVECTOR:
		{
			// Ok
			glTexGeni (GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
			glTexGeni (GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
			glTexGeni (GL_R, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
			glEnable (GL_TEXTURE_GEN_S);
			glEnable (GL_TEXTURE_GEN_T);
			glEnable (GL_TEXTURE_GEN_R);
			break;
		}
		case COORD_EYE:
		{
			// Ok
			glTexGeni (GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
			glTexGeni (GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
			glTexGeni (GL_R, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
			//glTexGeni (GL_Q, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
			glEnable (GL_TEXTURE_GEN_S);
			glEnable (GL_TEXTURE_GEN_T);
			glEnable (GL_TEXTURE_GEN_R);
			//glEnable (GL_TEXTURE_GEN_Q);
			break;
		}
		case COORD:
		{
			// Ok
			glTexGeni (GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
			glTexGeni (GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
			glTexGeni (GL_R, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
			//glTexGeni (GL_Q, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
			glEnable (GL_TEXTURE_GEN_S);
			glEnable (GL_TEXTURE_GEN_T);
			glEnable (GL_TEXTURE_GEN_R);
			//glEnable (GL_TEXTURE_GEN_Q);
			break;
		}
		case NOISE_EYE:
		{
			// Not supported
			glTexGeni (GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
			glTexGeni (GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
			glEnable (GL_TEXTURE_GEN_S);
			glEnable (GL_TEXTURE_GEN_T);
			break;
		}
		case NOISE:
		{
			// Not supported
			glTexGeni (GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
			glTexGeni (GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
			glEnable (GL_TEXTURE_GEN_S);
			glEnable (GL_TEXTURE_GEN_T);
			break;
		}
	}

//	glTexGenfv (GL_S, GL_EYE_PLANE, sPlane .data ());
//	glTexGenfv (GL_T, GL_EYE_PLANE, tPlane .data ());
//	glTexGenfv (GL_R, GL_EYE_PLANE, rPlane .data ());
//	glTexGenfv (GL_Q, GL_EYE_PLANE, qPlane .data ());

//	glTexGenfv (GL_S, GL_OBJECT_PLANE, sPlane .data ());
//	glTexGenfv (GL_T, GL_OBJECT_PLANE, tPlane .data ());
//	glTexGenfv (GL_R, GL_OBJECT_PLANE, rPlane .data ());
//	glTexGenfv (GL_Q, GL_OBJECT_PLANE, qPlane .data ());
}

void
TextureCoordinateGenerator::disable (const int32_t unit) const
{
	glActiveTexture (GL_TEXTURE0 + unit);

	glDisable (GL_TEXTURE_GEN_S);
	glDisable (GL_TEXTURE_GEN_T);
	glDisable (GL_TEXTURE_GEN_R);
	glDisable (GL_TEXTURE_GEN_Q);
}

} // X3D
} // titania
