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

#include "TwoSidedMaterial.h"

#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

const ComponentType TwoSidedMaterial::component      = ComponentType::SHAPE;
const std::string   TwoSidedMaterial::typeName       = "TwoSidedMaterial";
const std::string   TwoSidedMaterial::containerField = "material";

TwoSidedMaterial::Fields::Fields () :
	   separateBackColor (new SFBool ()),
	    ambientIntensity (new SFFloat (0.2)),
	        diffuseColor (new SFColor (0.8, 0.8, 0.8)),
	       specularColor (new SFColor ()),
	       emissiveColor (new SFColor ()),
	           shininess (new SFFloat (0.2)),
	        transparency (new SFFloat ()),
	backAmbientIntensity (new SFFloat (0.2)),
	    backDiffuseColor (new SFColor (0.8, 0.8, 0.8)),
	   backSpecularColor (new SFColor ()),
	   backEmissiveColor (new SFColor ()),
	       backShininess (new SFFloat (0.2)),
	    backTransparency (new SFFloat ())
{ }

TwoSidedMaterial::TwoSidedMaterial (X3DExecutionContext* const executionContext) :
	        X3DBaseNode (executionContext -> getBrowser (), executionContext),
	    X3DMaterialNode (),
	             fields (),
	     glAmbientColor (),
	     glDiffuseColor (),
	    glSpecularColor (),
	    glEmissiveColor (),
	        glShininess (),
	 glBackAmbientColor (),
	 glBackDiffuseColor (),
	glBackSpecularColor (),
	glBackEmissiveColor (),
	    glBackShininess ()
{
	addType (X3DConstants::TwoSidedMaterial);

	addField (inputOutput, "metadata",             metadata ());

	addField (inputOutput, "separateBackColor",    separateBackColor ());

	addField (inputOutput, "ambientIntensity",     ambientIntensity ());
	addField (inputOutput, "diffuseColor",         diffuseColor ());
	addField (inputOutput, "specularColor",        specularColor ());
	addField (inputOutput, "emissiveColor",        emissiveColor ());
	addField (inputOutput, "shininess",            shininess ());
	addField (inputOutput, "transparency",         transparency ());

	addField (inputOutput, "backAmbientIntensity", backAmbientIntensity ());
	addField (inputOutput, "backDiffuseColor",     backDiffuseColor ());
	addField (inputOutput, "backSpecularColor",    backSpecularColor ());
	addField (inputOutput, "backEmissiveColor",    backEmissiveColor ());
	addField (inputOutput, "backShininess",        backShininess ());
	addField (inputOutput, "backTransparency",     backTransparency ());
}

X3DBaseNode*
TwoSidedMaterial::create (X3DExecutionContext* const executionContext) const
{
	return new TwoSidedMaterial (executionContext);
}

void
TwoSidedMaterial::initialize ()
{
	X3DMaterialNode::initialize ();

	addInterest (this, &TwoSidedMaterial::eventsProcessed);

	eventsProcessed ();
}

void
TwoSidedMaterial::eventsProcessed ()
{
	// Front

	const float alpha = 1 - math::clamp <float> (transparency (), 0, 1);

	glAmbientColor [0] = ambientIntensity () * diffuseColor () .getRed ();
	glAmbientColor [1] = ambientIntensity () * diffuseColor () .getGreen ();
	glAmbientColor [2] = ambientIntensity () * diffuseColor () .getBlue ();
	glAmbientColor [3] = alpha;

	glDiffuseColor [0] = diffuseColor () .getRed ();
	glDiffuseColor [1] = diffuseColor () .getGreen ();
	glDiffuseColor [2] = diffuseColor () .getBlue () + 0.0001;
	glDiffuseColor [3] = alpha;

	glSpecularColor [0] = specularColor () .getRed ();
	glSpecularColor [1] = specularColor () .getGreen ();
	glSpecularColor [2] = specularColor () .getBlue ();
	glSpecularColor [3] = alpha;

	glEmissiveColor [0] = emissiveColor () .getRed ();
	glEmissiveColor [1] = emissiveColor () .getGreen ();
	glEmissiveColor [2] = emissiveColor () .getBlue ();
	glEmissiveColor [3] = alpha;

	glShininess = math::clamp <float> (shininess (), 0, 1) * 128;

	// Back
	
	if (separateBackColor ())
	{
		const float backAlpha = 1 - math::clamp <float> (backTransparency (), 0, 1);

		glBackAmbientColor [0] = backAmbientIntensity () * backDiffuseColor () .getRed ();
		glBackAmbientColor [1] = backAmbientIntensity () * backDiffuseColor () .getGreen ();
		glBackAmbientColor [2] = backAmbientIntensity () * backDiffuseColor () .getBlue ();
		glBackAmbientColor [3] = backAlpha;

		glBackDiffuseColor [0] = backDiffuseColor () .getRed ();
		glBackDiffuseColor [1] = backDiffuseColor () .getGreen ();
		glBackDiffuseColor [2] = backDiffuseColor () .getBlue () + 0.0001;
		glBackDiffuseColor [3] = backAlpha;

		glBackSpecularColor [0] = backSpecularColor () .getRed ();
		glBackSpecularColor [1] = backSpecularColor () .getGreen ();
		glBackSpecularColor [2] = backSpecularColor () .getBlue ();
		glBackSpecularColor [3] = backAlpha;

		glBackEmissiveColor [0] = backEmissiveColor () .getRed ();
		glBackEmissiveColor [1] = backEmissiveColor () .getGreen ();
		glBackEmissiveColor [2] = backEmissiveColor () .getBlue ();
		glBackEmissiveColor [3] = backAlpha;

		glBackShininess = math::clamp <float> (backShininess (), 0, 1) * 128;
	}
	else
	{
		glBackAmbientColor [0] = ambientIntensity () * diffuseColor () .getRed ();
		glBackAmbientColor [1] = ambientIntensity () * diffuseColor () .getGreen ();
		glBackAmbientColor [2] = ambientIntensity () * diffuseColor () .getBlue ();
		glBackAmbientColor [3] = alpha;

		glBackDiffuseColor [0] = diffuseColor () .getRed ();
		glBackDiffuseColor [1] = diffuseColor () .getGreen ();
		glBackDiffuseColor [2] = diffuseColor () .getBlue ();
		glBackDiffuseColor [3] = alpha;

		glBackSpecularColor [0] = specularColor () .getRed ();
		glBackSpecularColor [1] = specularColor () .getGreen ();
		glBackSpecularColor [2] = specularColor () .getBlue ();
		glBackSpecularColor [3] = alpha;

		glBackEmissiveColor [0] = emissiveColor () .getRed ();
		glBackEmissiveColor [1] = emissiveColor () .getGreen ();
		glBackEmissiveColor [2] = emissiveColor () .getBlue ();
		glBackEmissiveColor [3] = alpha;

		glBackShininess = math::clamp <float> (shininess (), 0, 1) * 128;
	}
}

void
TwoSidedMaterial::draw ()
{
	glEnable (GL_LIGHTING);

	glMaterialfv (GL_FRONT, GL_AMBIENT,   glAmbientColor);
	glMaterialfv (GL_FRONT, GL_DIFFUSE,   glDiffuseColor);
	glMaterialfv (GL_FRONT, GL_SPECULAR,  glSpecularColor);
	glMaterialfv (GL_FRONT, GL_EMISSION,  glEmissiveColor);
	glMaterialf  (GL_FRONT, GL_SHININESS, glShininess);

	glMaterialfv (GL_BACK, GL_AMBIENT,   glBackAmbientColor);
	glMaterialfv (GL_BACK, GL_DIFFUSE,   glBackDiffuseColor);
	glMaterialfv (GL_BACK, GL_SPECULAR,  glBackSpecularColor);
	glMaterialfv (GL_BACK, GL_EMISSION,  glBackEmissiveColor);
	glMaterialf  (GL_BACK, GL_SHININESS, glBackShininess);

	glColor4fv (glEmissiveColor); // for lines and points

}

} // X3D
} // titania
