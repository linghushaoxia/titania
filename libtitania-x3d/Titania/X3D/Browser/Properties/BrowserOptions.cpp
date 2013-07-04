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

#include "BrowserOptions.h"

#include "../../Execution/X3DExecutionContext.h"
#include "../Geometry3D/QuadSphereProperties.h"
#include <Titania/String/Join.h>
#include <Titania/Physics/Constants.h>

extern "C"
{
#include <GL/gl.h>
}

namespace titania {
namespace X3D {

// Table 9.2 — Browser options
// Name                    Type/valid range                         Default                      Description
// SplashScreen            Boolean                                  Implementation-dependent     Display browser splash screen on startup
// Dashboard               Boolean                                  False                        Specified by bound NavigationInfo in content. Display browser navigation user interface.
// Rubberband              Boolean                                  True                         Specified by bound NavigationInfo in content. Display rubber band navigation hint.
// EnableInlineViewpoints  Boolean                                  True                         Viewpoints from Inline nodes are included in list of viewpoints if made available by the Inline node.
// Antialiased             Boolean                                  False                        Render using hardware antialiasing if available
// TextureQuality          Low, Medium, High                        Medium                       Quality of texture map display
// PrimitiveQuality        Low, Medium, High                        Medium                       Render quality (tesselation level) for Box, Cone, Cylinder, Sphere
// QualityWhenMoving       Low, Medium, High,                       Same (as while stationary)   Render quality while camera is moving
// Shading                 Point, Wireframe, Flat, Gouraud, Phong   Gouraud                      Specify shading mode for all objects
// MotionBlur              Boolean                                  False                        Render animations with motion blur
// MotionBlurIntesity      Number                                   0.25                         Motion blur intesity in the range (0, 1)
// AnimateStairWalks       Boolean                                  False                        Animate stair walks. This can give unexpected results when the floor is animated.
// Gravity                 Number                                   g                            Gravitational acceleration. The standard value is the acceleration of the earth.

BrowserOptions::Fields::Fields (X3DExecutionContext* const executionContext) :
	splashScreen (new SFBool (false)),
	dashboard (new SFBool ()),
	rubberBand (new SFBool (true)),
	enableInlineViewpoints (new SFBool (true)),
	antialiased (new SFBool ()),
	textureQuality (new SFString ("MEDIUM")),
	primitiveQuality (new SFString ("MEDIUM")),
	qualityWhenMoving (new SFString ("MEDIUM")),
	shading (new SFString ("GOURAUD")),
	animateStairWalks (new SFBool ()),
	gravity (new SFFloat (P_GN)),
	motionBlurProperties (new MotionBlur (executionContext)),
	textureProperties (new TextureProperties (executionContext)),
	arc2DProperties (new Arc2DProperties (executionContext)),
	arcClose2DProperties (new ArcClose2DProperties (executionContext)),
	circle2DProperties (new Circle2DProperties (executionContext)),
	disc2DProperties (new Disk2DProperties (executionContext)),
	rectangle2DProperties (new Rectangle2DProperties (executionContext)),
	boxProperties (new BoxProperties (executionContext)),
	sphereProperties (new QuadSphereProperties (executionContext)),
	fontStyle (new FontStyle (executionContext))
{ }

BrowserOptions::BrowserOptions (X3DExecutionContext* const executionContext) :
	    X3DBaseNode (executionContext -> getBrowser (), executionContext), 
	X3DPropertyNode (),                                                    
	         fields (executionContext)                                     
{
	setComponent ("Browser"),
	setTypeName ("BrowserOptions");

	addField (inputOutput, "SplashScreen",           splashScreen ());
	addField (inputOutput, "Dashboard",              dashboard ());
	addField (inputOutput, "Rubberband",             rubberBand ());
	addField (inputOutput, "EnableInlineViewpoints", enableInlineViewpoints ());
	addField (inputOutput, "Antialiased",            antialiased ());
	addField (inputOutput, "TextureQuality",         textureQuality ());
	addField (inputOutput, "PrimitiveQuality",       primitiveQuality ());
	addField (inputOutput, "QualityWhenMoving",      qualityWhenMoving ());
	addField (inputOutput, "Shading",                shading ());
	addField (inputOutput, "MotionBlur",             motionBlur ());
	addField (inputOutput, "MotionBlurIntensity",    motionBlurIntensity ());
	addField (inputOutput, "AnimateStairWalks",      animateStairWalks ());
	addField (inputOutput, "Gravity",                gravity ());
	
	addField ("AntiAliased", "Antialiased");

	setChildren (motionBlurProperties (),
	             textureProperties (),
	             arc2DProperties (),
	             arcClose2DProperties (),
	             circle2DProperties (),
	             disc2DProperties (),
	             rectangle2DProperties (),
	             boxProperties (),
	             sphereProperties (),
	             fontStyle ());
}

BrowserOptions*
BrowserOptions::create (X3DExecutionContext* const executionContext)  const
{
	return new BrowserOptions (executionContext);
}

void
BrowserOptions::initialize ()
{
	X3DPropertyNode::initialize ();

	motionBlurProperties ()  -> setup ();
	textureProperties ()     -> setup ();
	arc2DProperties ()       -> setup ();
	arcClose2DProperties ()  -> setup ();
	circle2DProperties ()    -> setup ();
	disc2DProperties ()      -> setup ();
	rectangle2DProperties () -> setup ();
	boxProperties ()         -> setup ();
	sphereProperties ()      -> setup ();
	fontStyle ()             -> setup ();

	primitiveQuality () .addInterest (this, &BrowserOptions::set_primitiveQuality);
	shading ()          .addInterest (this, &BrowserOptions::set_shading);

	set_textureQuality ();
	set_primitiveQuality ();
	set_shading ();
}

SFBool &
BrowserOptions::motionBlur ()
{
	return motionBlurProperties () -> enabled ();
}

const SFBool &
BrowserOptions::motionBlur () const
{
	return motionBlurProperties () -> enabled ();
}

SFFloat &
BrowserOptions::motionBlurIntensity ()
{
	return motionBlurProperties () -> intensity ();
}

const SFFloat &
BrowserOptions::motionBlurIntensity () const
{
	return motionBlurProperties () -> intensity ();
}

void
BrowserOptions::set_textureQuality ()
{
	textureProperties () -> magnificationFilter () = "NICEST";
	textureProperties () -> minificationFilter ()  = "NICEST";
	textureProperties () -> textureCompression ()  = "NICEST";
	textureProperties () -> generateMipMaps ()     = true;

}

void
BrowserOptions::set_primitiveQuality ()
{
	std::clog << "Setting primitive quality to " << primitiveQuality () << "." << std::endl;

	if (primitiveQuality () == "LOW")
	{
		arc2DProperties ()      -> minAngle () = M_PI / 10;
		arcClose2DProperties () -> minAngle () = M_PI / 10;
		circle2DProperties ()   -> segments () = 20;
		disc2DProperties ()     -> segments () = 20;

		auto quadSphereProperties = dynamic_cast <QuadSphereProperties*> (sphereProperties () .getValue ());

		if (quadSphereProperties)
		{
			quadSphereProperties -> uDimension () = 20;
			quadSphereProperties -> vDimension () = 10;
		}
	}
	else if (primitiveQuality () == "MEDIUM")
	{
		arc2DProperties ()      -> minAngle () = M_PI / 20;
		arcClose2DProperties () -> minAngle () = M_PI / 20;
		circle2DProperties ()   -> segments () = 60;
		disc2DProperties ()     -> segments () = 60;

		auto quadSphereProperties = dynamic_cast <QuadSphereProperties*> (sphereProperties () .getValue ());

		if (quadSphereProperties)
		{
			quadSphereProperties -> uDimension () = 40;
			quadSphereProperties -> vDimension () = 20;
		}
	}
	else if (primitiveQuality () == "HIGH")
	{
		arc2DProperties ()      -> minAngle () = M_PI / 40;
		arcClose2DProperties () -> minAngle () = M_PI / 40;
		circle2DProperties ()   -> segments () = 100;
		disc2DProperties ()     -> segments () = 100;

		auto quadSphereProperties = dynamic_cast <QuadSphereProperties*> (sphereProperties () .getValue ());

		if (quadSphereProperties)
		{
			quadSphereProperties -> uDimension () = 80;
			quadSphereProperties -> vDimension () = 40;
		}
	}
}

void
BrowserOptions::set_shading ()
{
	std::clog << "Setting shading to " << shading () << "." << std::endl;

	if (shading () == "PHONG")
	{
		glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
		glShadeModel (GL_SMOOTH);
	}
	else if (shading () == "GOURAUD")
	{
		glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
		glShadeModel (GL_SMOOTH);
	}
	else if (shading () == "FLAT")
	{
		glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
		glShadeModel (GL_FLAT);
	}
	else if (shading () == "WIREFRAME")
	{
		glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
		glShadeModel (GL_SMOOTH);
	}
	else if (shading () == "POINTSET")
	{
		glPolygonMode (GL_FRONT_AND_BACK, GL_POINT);
		glShadeModel (GL_SMOOTH);
	}
}

void
BrowserOptions::dispose ()
{
	motionBlurProperties ()  .dispose ();
	textureProperties ()     .dispose ();
	arc2DProperties ()       .dispose ();
	arcClose2DProperties ()  .dispose ();
	circle2DProperties ()    .dispose ();
	disc2DProperties ()      .dispose ();
	rectangle2DProperties () .dispose ();
	boxProperties ()         .dispose ();
	sphereProperties ()      .dispose ();
	fontStyle ()             .dispose ();
}

} // X3D
} // titania
