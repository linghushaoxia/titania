/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstra�e 31a, Leipzig, Germany 2011.
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

#include "ColorPerVertexEditor.h"

#include "../../Browser/X3DBrowserWindow.h"
#include "../../Configuration/config.h"

namespace titania {
namespace puck {

ColorPerVertexEditor::ColorPerVertexEditor (X3DBrowserWindow* const browserWindow) :
	                X3DBaseInterface (browserWindow, browserWindow -> getBrowser ()),
	X3DColorPerVertexEditorInterface (get_ui ("Editors/ColorPerVertexEditor.xml"), gconf_dir ()),
	                         preview (X3D::createBrowser (getBrowserWindow () -> getBrowser ())),
	                       selection (),
	                           coord (),
	                  indexedFaceSet (),
	                           color (),
	                     colorButton (getBrowserWindow (), getColorButton (), getColorAdjustment (), getColorButton (), "color")
{
	preview -> set_antialiasing (4);
	colorButton .setUndo (false);

	setup ();
}

void
ColorPerVertexEditor::initialize ()
{
	X3DColorPerVertexEditorInterface::initialize ();

	getPreviewBox () .pack_start (*preview, true, true, 0);

	preview -> show ();
	preview -> initialized () .addInterest (this, &ColorPerVertexEditor::set_initialized);

	getBrowserWindow () -> getSelection () -> getChildren () .addInterest (this, &ColorPerVertexEditor::set_selection);
}

void
ColorPerVertexEditor::set_initialized ()
{
	preview -> initialized () .removeInterest (this, &ColorPerVertexEditor::set_initialized);

	try
	{
		preview -> loadURL ({ get_ui ("Editors/ColorPerVertexEditorPreview.x3dv") });

		const auto shape       = preview -> getExecutionContext () -> getNamedNode <X3D::Shape> ("Shape");
		const auto touchSensor = preview -> getExecutionContext () -> getNamedNode <X3D::TouchSensor> ("TouchSensor");

		shape -> geometry ()               .addInterest (this, &ColorPerVertexEditor::on_look_at_all_clicked);
		touchSensor -> hitPoint_changed () .addInterest (this, &ColorPerVertexEditor::set_hitPoint);

		set_selection ();
	}
	catch (const X3D::X3DError &)
	{ }
}

void
ColorPerVertexEditor::set_selection ()
{
	try
	{
		const auto indexedFaceSets = getSelection <X3D::IndexedFaceSet> ({ X3D::X3DConstants::IndexedFaceSet });
		const auto shape           = preview -> getExecutionContext () -> getNamedNode <X3D::Shape> ("Shape");
		const auto cross           = preview -> getExecutionContext () -> getNamedNode <X3D::Transform> ("Cross");

		cross -> translation () = X3D::Vector3f ();

		if (indexedFaceSets .empty ())
		{
			selection            = nullptr;
			indexedFaceSet       = nullptr;
			shape -> geometry () = nullptr;
			colorButton .setNodes ({ });
		}
		else
		{
			selection      = indexedFaceSets .back ();
			coord          = selection -> coord ();
			indexedFaceSet = preview -> getExecutionContext () -> createNode <X3D::IndexedFaceSet> ();

			indexedFaceSet -> solid ()           = selection -> solid ();
			indexedFaceSet -> ccw ()             = selection -> ccw ();
			indexedFaceSet -> creaseAngle ()     = selection -> creaseAngle ();
			indexedFaceSet -> normalPerVertex () = selection -> normalPerVertex ();
			indexedFaceSet -> normalIndex ()     = selection -> normalIndex ();
			indexedFaceSet -> coordIndex ()      = selection -> coordIndex ();
			indexedFaceSet -> normal ()          = selection -> normal ();
			indexedFaceSet -> coord ()           = selection -> coord ();

			shape -> geometry () = indexedFaceSet;

			// Generate color and colorIndex.

			for (const auto & index : indexedFaceSet -> coordIndex ())
				indexedFaceSet -> colorIndex () .emplace_back (index < 0 ? -1 : 0);

			color = preview -> getExecutionContext () -> createNode <X3D::ColorRGBA> ();
			color -> color () .emplace_back (X3D::Color4f (1, 1, 1, 1));
			indexedFaceSet -> color () = color;

			colorButton .setIndex (0);
			colorButton .setNodes ({ color });

			// Initialize all.

			preview -> getExecutionContext () -> realize ();
		}
	}
	catch (const X3D::X3DError &)
	{ }
}

void
ColorPerVertexEditor::on_look_at_all_clicked ()
{
	if (preview -> getActiveLayer ())
		preview -> getActiveLayer () -> lookAt ();
}

void
ColorPerVertexEditor::set_hitPoint (const X3D::Vector3f & hitPoint)
{
	if (not coord)
		return;

	set_cross (hitPoint);
}

void
ColorPerVertexEditor::set_cross (const X3D::Vector3f & hitPoint)
{
	float  min   = std::numeric_limits <float>::infinity ();
	size_t index = 0;

	for (size_t i = 0, size = coord -> getSize (); i < size; ++ i)
	{
		const auto distance = math::abs (hitPoint - X3D::Vector3f (coord -> get1Point (i)));

		if (distance < min)
		{
			min   = distance;
			index = i;
		}
	}

	const auto cross = preview -> getExecutionContext () -> getNamedNode <X3D::Transform> ("Cross");

	cross -> translation () = coord -> get1Point (index);
}

ColorPerVertexEditor::~ColorPerVertexEditor ()
{
	dispose ();
}

} // puck
} // titania