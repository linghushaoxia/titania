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
 ******************************************************************************/

#include "X3DBrowserContext.h"

namespace titania {
namespace X3D {

X3DBrowserContext::X3DBrowserContext () :
	X3DExecutionContext (),
	     navigationInfos (),       
	         backgrounds (),       
	                fogs (),       
	          viewpoints ()        
{ }

// Layer handling

void
X3DBrowserContext::pushLayer (X3DLayerNode* const layer)
{
	layers .push (layer);
}

void
X3DBrowserContext::popLayer ()
{
	layers .pop ();
}

X3DLayerNode*
X3DBrowserContext::getLayer () const
{
	return layers .top ();
}

// NavigationInfo list handling

void
X3DBrowserContext::addNavigationInfo (NavigationInfo* const navigationInfo)
{
	navigationInfos .push_back (navigationInfo);
}

void
X3DBrowserContext::removeNavigationInfo (NavigationInfo* const navigationInfo)
{
	navigationInfos .erase (navigationInfo);
}

const NavigationInfoList &
X3DBrowserContext::getNavigationInfos () const
{
	return navigationInfos;
}

// Background list handling

void
X3DBrowserContext::addBackground (X3DBackgroundNode* const background)
{
	backgrounds .push_back (background);
}

void
X3DBrowserContext::removeBackground (X3DBackgroundNode* const background)
{
	backgrounds .erase (background);
}

const BackgroundList &
X3DBrowserContext::getBackgrounds () const
{
	return backgrounds;
}

// Fog list handling

void
X3DBrowserContext::addFog (Fog* const fog)
{
	fogs .push_back (fog);
}

void
X3DBrowserContext::removeFog (Fog* const fog)
{
	fogs .erase (fog);
}

const FogList &
X3DBrowserContext::getFogs () const
{
	return fogs;
}

// Viewpoint list handling

void
X3DBrowserContext::addViewpoint (X3DViewpointNode* const viewpoint)
{
	viewpoints .push_back (viewpoint);
}

void
X3DBrowserContext::removeViewpoint (X3DViewpointNode* const viewpoint)
{
	viewpoints .erase (viewpoint);
}

const ViewpointList &
X3DBrowserContext::getViewpoints () const
{
	return viewpoints;
}

// Texture list handling

void
X3DBrowserContext::addTexture (const std::string & URL, GLuint textureId)
{
	textures [URL] = std::make_pair (textureId, 1);
}

bool
X3DBrowserContext::removeTexture (const std::string & URL, GLuint textureId)
{
	auto texture = textures .find (URL);

	if (texture not_eq textures .end ())
	{
		if (-- texture -> second .second == 0)
		{
			textures .erase (texture);
			return true;
		}
	}

	return false;
}

GLuint
X3DBrowserContext::getTexture (const std::string & URL)
{
	auto texture = textures .find (URL);

	if (texture not_eq textures .end ())
	{
		++ texture -> second .second;
		return texture -> second .first;
	}

	return 0;
}

const TextureIndex &
X3DBrowserContext::getTextures () const
{
	return textures;
}

/// @name Sensors

void
X3DBrowserContext::addSensor (X3DSensorNode* const sensor)
{
	std::clog << "\tAdding sensor " << sensor -> getTypeName () << " to world: " << sensors .size () << " are registered until now." << std::endl;
	sensors .addInterest (sensor, &X3DSensorNode::update);
	std::clog << "\tNow are " << sensors .size () << " registered." << std::endl;
}

void
X3DBrowserContext::removeSensor (X3DSensorNode* const sensor)
{
	std::clog << "\tRemoving sensor " << sensor -> getTypeName () << " from world: " << sensors .size () << " are registered until now." << std::endl;
	sensors .removeInterest (sensor, &X3DSensorNode::update);
	std::clog << "\tNow are " << sensors .size () << " registered." << std::endl;
}

void
X3DBrowserContext::updateSensors ()
{
	sensors .processInterests ();
}

void
X3DBrowserContext::dispose ()
{
	navigationInfos .dispose ();
	backgrounds     .dispose ();
	viewpoints      .dispose ();
	fogs            .dispose ();

	//X3DBaseNode::dispose ();
	//X3DExecutionContext::dispose ();
}

} // X3D
} // titania
