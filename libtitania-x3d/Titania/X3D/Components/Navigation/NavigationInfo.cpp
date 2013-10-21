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

#include "NavigationInfo.h"

#include "../../Browser/X3DBrowser.h"
#include "../../Execution/BindableNodeList.h"
#include "../../Execution/BindableNodeStack.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../Layering/X3DLayerNode.h"

#include <limits>

namespace titania {
namespace X3D {

const std::string NavigationInfo::componentName  = "Navigation";
const std::string NavigationInfo::typeName       = "NavigationInfo";
const std::string NavigationInfo::containerField = "children";

NavigationInfo::Fields::Fields () :
	              type (new MFString ({ "EXAMINE", "ANY" })),
	        avatarSize (new MFFloat ({ 0.25, 1.6, 0.75 })),
	             speed (new SFFloat (1)),
	         headlight (new SFBool (true)),
	   visibilityLimit (new SFFloat ()),
	    transitionType (new MFString ({ "LINEAR" })),
	    transitionTime (new SFTime (1)),
	   transitionStart (),
	transitionComplete (new SFBool ())
{ }

NavigationInfo::NavigationInfo (X3DExecutionContext* const executionContext) :
	     X3DBaseNode (executionContext -> getBrowser (), executionContext),
	 X3DBindableNode (),
	          fields (),
	directionalLight (new DirectionalLight (executionContext)),
	           light ()
{
	addField (inputOutput, "metadata",           metadata ());
	addField (inputOnly,   "set_bind",           set_bind ());
	addField (inputOutput, "type",               type ());
	addField (inputOutput, "avatarSize",         avatarSize ());
	addField (inputOutput, "speed",              speed ());
	addField (inputOutput, "headlight",          headlight ());
	addField (inputOutput, "visibilityLimit",    visibilityLimit ());
	addField (inputOutput, "transitionType",     transitionType ());
	addField (inputOutput, "transitionTime",     transitionTime ());
	addField (outputOnly,  "transitionComplete", transitionComplete ());
	addField (outputOnly,  "isBound",            isBound ());
	addField (outputOnly,  "bindTime",           bindTime ());

	addChildren (transitionStart (), directionalLight);
}

X3DBaseNode*
NavigationInfo::create (X3DExecutionContext* const executionContext) const
{
	return new NavigationInfo (executionContext);
}

void
NavigationInfo::initialize ()
{
	X3DBindableNode::initialize ();

	directionalLight -> setup ();

	headlight () .addInterest (this, &NavigationInfo::set_headlight);

	set_headlight ();
}

void
NavigationInfo::set_headlight ()
{
	if (headlight ())
		light .reset (new LightContainer (Matrix4f (), directionalLight));

	else
		light .reset ();
}

float
NavigationInfo::getCollisionRadius () const
{
	if (avatarSize () .size () > 0)
		return avatarSize () [0];

	return 0.25;
}

float
NavigationInfo::getAvatarHeight () const
{
	if (avatarSize () .size () > 1)
		return avatarSize () [1];

	return 1.6;
}

float
NavigationInfo::getStepHeight () const
{
	if (avatarSize () .size () > 2)
		return avatarSize () [2];

	return 0.75;
}

float
NavigationInfo::getNearPlane () const
{
	float zNear = getCollisionRadius ();

	if (zNear == 0)
		zNear = std::numeric_limits <float>::epsilon () * 100;

	else
		zNear /= 2;

	return zNear;
}

float
NavigationInfo::getFarPlane () const
{
	return visibilityLimit () ? visibilityLimit () : 100000;
}

TransitionType
NavigationInfo::getTransitionType () const
{
	for (const auto & type : transitionType ())
	{
		if (type == "TELEPORT")
			return TransitionType::TELEPORT;

		if (type == "LINEAR")
			return TransitionType::LINEAR;

		if (type == "ANIMATE")
			return TransitionType::ANIMATE;
	}

	return TransitionType::LINEAR;
}

void
NavigationInfo::bindToLayer (X3DLayerNode* const layer)
{
	layer -> getNavigationInfoStack () -> push (this);
}

void
NavigationInfo::unbindFromLayer (X3DLayerNode* const layer)
{
	layer -> getNavigationInfoStack () -> pop (this);
}

void
NavigationInfo::enable ()
{
	if (headlight ())
		light -> enable ();
}

void
NavigationInfo::disable ()
{
	if (headlight ())
		light -> disable ();
}

void
NavigationInfo::traverse (const TraverseType type)
{
	switch (type)
	{
		case TraverseType::COLLECT:
		{
			getCurrentLayer () -> getNavigationInfos () -> push_back (this);
			break;
		}
		default:
			break;
	}
}

void
NavigationInfo::dispose ()
{
	directionalLight .dispose ();

	X3DBindableNode::dispose ();
}

} // X3D
} // titania
