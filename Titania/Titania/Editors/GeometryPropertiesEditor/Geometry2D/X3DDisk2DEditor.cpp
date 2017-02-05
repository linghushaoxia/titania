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

#include "X3DDisk2DEditor.h"

#include <Titania/X3D/Browser/Geometry2D/Disk2DOptions.h>
#include <Titania/X3D/Components/Geometry2D/Disk2D.h>
#include <Titania/X3D/Components/Shape/X3DShapeNode.h>

namespace titania {
namespace puck {

X3DDisk2DEditor::X3DDisk2DEditor () :
	X3DGeometryPropertiesEditorInterface (),
	                         innerRadius (this, getDisk2DInnerRadiusAdjustment (), getDisk2DInnerRadiusSpinButton (), "innerRadius"),
	                         outerRadius (this, getDisk2DOuterRadiusAdjustment (), getDisk2DOuterRadiusSpinButton (), "outerRadius"),
	                           dimension (this, getDisk2DDimensionAdjustment (), getDisk2DDimensionSpinButton (), "dimension")
{ }

void
X3DDisk2DEditor::addShapes ()
{
	for (const auto & shapeNode : getShapes ())
		shapeNode -> geometry () .addInterest (&X3DDisk2DEditor::set_geometry, this);

	set_geometry ();
}

void
X3DDisk2DEditor::removeShapes ()
{
	for (const auto & shapeNode : getShapes ())
		shapeNode -> geometry () .removeInterest (&X3DDisk2DEditor::set_geometry, this);
}

void
X3DDisk2DEditor::set_geometry ()
{
	const auto node   = getOneSelection <X3D::Disk2D> (getShapes (), "geometry");
	const auto nodes  = node ? X3D::MFNode ({ node }) : X3D::MFNode ();
	const auto global = node ? X3D::MFNode ({ getCurrentBrowser () -> getDisk2DOptions () }) : X3D::MFNode ();

	getDisk2DExpander () .set_visible (node);

	innerRadius .setNodes (nodes);
	outerRadius .setNodes (nodes);
	dimension   .setNodes (global);
}

X3DDisk2DEditor::~X3DDisk2DEditor ()
{ }

} // puck
} // titania
