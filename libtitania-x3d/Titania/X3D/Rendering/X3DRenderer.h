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

#ifndef __TITANIA_X3D_RENDERING_X3DRENDERER_H__
#define __TITANIA_X3D_RENDERING_X3DRENDERER_H__

#include "../Base/Output.h"
#include "../Components/Core/X3DNode.h"
#include "../Rendering/ClipPlaneContainer.h"
#include "../Rendering/CollisionArray.h"
#include "../Rendering/CollisionContainer.h"
#include "../Rendering/LightContainer.h"
#include "../Rendering/ShapeContainer.h"
#include "../Rendering/ViewVolumeStack.h"
#include "../Rendering/X3DCollectableObject.h"

#include <memory>

namespace titania {
namespace X3D {

class FrameBuffer;
class X3DFogObject;
class X3DGroupingNode;

class X3DRenderer :
	virtual public X3DNode
{
public:

	///  @name Common members

	virtual
	void
	setExecutionContext (X3DExecutionContext* const)
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>) override;

	///  @name Member access

	virtual
	NavigationInfo*
	getNavigationInfo () const = 0;

	virtual
	X3DViewpointNode*
	getViewpoint () const = 0;

	virtual
	X3DBackgroundNode*
	getBackground () const = 0;

	virtual
	X3DFogObject*
	getFog () const = 0;

	ViewVolumeStack &
	getViewVolumes ()
	{ return viewVolumeStack; }

	LightContainerArray &
	getGlobalLights ()
	{ return globalLights; }

	CollectableObjectArray &
	getLocalObjects ()
	{ return localObjects; }

	ClipPlaneContainerArray &
	getClipPlanes ()
	{ return clipPlanes; }

	LightContainerArray &
	getLocalLights ()
	{ return localLights; }

	LightContainerArray &
	getLights ()
	{ return lights; }

	CollisionArray &
	getCollisions ()
	{ return collisions; }

	size_t
	getNumOpaqueShapes () const
	{ return numOpaqueShapes; }

	const ShapeContainerArray &
	getOpaqueShapes () const
	{ return opaqueShapes; }

	size_t
	getNumTransparentShapes () const
	{ return numTransparentShapes; }

	const ShapeContainerArray &
	getTransparentShapes () const
	{ return transparentShapes; }

	void
	addShape (X3DShapeNode* const);

	void
	addCollisionShape (X3DShapeNode* const);

	void
	addDepthShape (X3DShapeNode* const);

	Vector3d
	constrainTranslation (const Vector3d &) const;

	void
	render (const std::function <void (const TraverseType)> &, const TraverseType);

	///  @name Destruction

	virtual
	void
	dispose () override;

	virtual
	~X3DRenderer ();


protected:

	///  @name Construction

	X3DRenderer ();

	virtual
	void
	initialize () override;


private:

	///  @name Operations

	double
	getDistance (const Vector3d &) const;

	double
	getDepth () const;

	void
	collide ();

	void
	gravite ();

	void
	display ();

	void
	depth (const CollisionContainerArray &, const size_t);

	///  @name Members

	ViewVolumeStack         viewVolumeStack;
	LightContainerArray     globalLights;
	CollectableObjectArray  localObjects;
	ClipPlaneContainerArray clipPlanes;
	LightContainerArray     localLights;
	LightContainerArray     lights;
	CollisionArray          collisions;

	ShapeContainerArray      opaqueShapes;
	ShapeContainerArray      transparentShapes;
	CollisionContainerArray  collisionShapes;
	std::vector <Collision*> activeCollisions;
	CollisionContainerArray  depthShapes;

	std::unique_ptr <FrameBuffer> depthBuffer;
	double                        speed;

	size_t numOpaqueShapes;
	size_t numTransparentShapes;
	size_t numCollisionShapes;
	size_t numDepthShapes;

};

} // X3D
} // titania

#endif
