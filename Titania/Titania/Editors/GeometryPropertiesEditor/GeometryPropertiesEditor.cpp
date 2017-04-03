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

#include "GeometryPropertiesEditor.h"

#include "../../Configuration/config.h"

#include <Titania/X3D/Components/Geometry3D/IndexedFaceSet.h>
#include <Titania/X3D/Components/Geometry3D/ElevationGrid.h>
#include <Titania/X3D/Components/Geospatial/GeoElevationGrid.h>
#include <Titania/X3D/Components/Shape/X3DShapeNode.h>

namespace titania {
namespace puck {

using math::pi;

GeometryPropertiesEditor::GeometryPropertiesEditor (X3DBrowserWindow* const browserWindow) :
	                     X3DBaseInterface (browserWindow, browserWindow -> getCurrentBrowser ()),
	 X3DGeometryPropertiesEditorInterface (get_ui ("Editors/GeometryPropertiesEditor.glade")),
	                       X3DArc2DEditor (),
	                  X3DArcClose2DEditor (),
	                    X3DCircle2DEditor (),
	                      X3DDisk2DEditor (),
	                 X3DRectangle2DEditor (),
	                         X3DBoxEditor (),
	                        X3DConeEditor (),
	                    X3DCylinderEditor (),
	               X3DElevationGridEditor (),
	                   X3DExtrusionEditor (),
	                      X3DSphereEditor (),
	            X3DGeoElevationGridEditor (),
	                  X3DNurbsCurveEditor (),
	           X3DNurbsPatchSurfaceEditor (),
	         X3DNurbsTrimmedSurfaceEditor (),
	              X3DIndexedLineSetEditor (),
	              X3DPrimitiveCountEditor (),
	                      X3DGeometryTool (),
	                                solid (this, getSolidCheckButton (),  "solid"),
	                                  ccw (this, getCCWCheckButton (),    "ccw"),
	                               convex (this, getConvexCheckButton (), "convex"),
	                          creaseAngle (this, getCreaseAngleAdjustment (), getCreaseAngleBox (), "creaseAngle"),
	                    creaseAngleDouble (this, getCreaseAngleDoubleAdjustment (), getCreaseAngleDoubleBox (), "creaseAngle"),
	                       colorPerVertex (this, getColorPerVertexCheckButton (), "colorPerVertex"),
	                      normalPerVertex (this, getNormalPerVertexCheckButton (), "normalPerVertex"),
	                                color (this,
	                                      getColorButton (),
	                                      getColorAdjustment (),
	                                      getColorGrid (),
	                                      getAddColorButton (),
	                                      getRemoveColorButton (),
	                                      getColorScrolledWindow (),
	                                      "color"),
	                           colorRGBA (this,
	                                      getColorRGBAButton (),
	                                      getColorRGBAAdjustment (),
	                                      getColorRGBAGrid (),
	                                      getAddColorRGBAButton (),
	                                      getRemoveColorRGBAButton (),
	                                      getColorRGBAScrolledWindow (),
	                                      "color"),
	                        geometryNodes (),
	                         geometryNode (),
	                           shapeNodes (),
	                           colorNodes (),
	                       colorRGBANodes (),
	                          nodesBuffer (),
	                          colorBuffer (),
	                            changing (false)
{
	addChildObjects (geometryNodes, geometryNode, shapeNodes, colorNodes, colorRGBANodes, nodesBuffer, colorBuffer);

	nodesBuffer .addInterest (&GeometryPropertiesEditor::set_buffer, this);
	colorBuffer .addInterest (&GeometryPropertiesEditor::set_color_buffer, this);

	getCreaseAngleAdjustment ()       -> set_upper (pi <double>);
	getCreaseAngleDoubleAdjustment () -> set_upper (pi <double>);

	creaseAngle       .setHide (true);
	creaseAngleDouble .setHide (true);
	color             .setHide (true);
	colorRGBA         .setHide (true);

	setup ();
}

void
GeometryPropertiesEditor::initialize ()
{
	X3DGeometryPropertiesEditorInterface::initialize ();
	X3DElevationGridEditor::initialize ();
	X3DGeoElevationGridEditor::initialize ();
	X3DPrimitiveCountEditor::initialize ();
	X3DGeometryTool::initialize ();
}

void
GeometryPropertiesEditor::configure ()
{
	X3DGeometryPropertiesEditorInterface::configure ();
	X3DRectangle2DEditor::configure ();
	X3DBoxEditor::configure ();
	X3DIndexedLineSetEditor::configure ();
	X3DPrimitiveCountEditor::configure ();
	X3DGeometryTool::configure ();

	getGeometryChildNotebook () .set_current_page (getConfig () -> getInteger ("currentPage"));
}

void
GeometryPropertiesEditor::set_selection (const X3D::MFNode & selection)
{
	X3DGeometryPropertiesEditorInterface::set_selection (selection);

	for (const auto & shapeNode : shapeNodes)
		shapeNode -> geometry () .removeInterest (&GeometryPropertiesEditor::set_geometry, this);

	shapeNodes = getNodes <X3D::X3DShapeNode> (selection, { X3D::X3DConstants::X3DShapeNode });

	for (const auto & shapeNode : shapeNodes)
		shapeNode -> geometry () .addInterest (&GeometryPropertiesEditor::set_geometry, this);

	set_geometry ();
}

void
GeometryPropertiesEditor::on_geometry_changed ()
{
	if (changing)
		return;

	if (getGeometryComboBoxText () .get_active_row_number () > 0)
	{
	   try
	   {
			const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Change Field »geometry«"));
		   auto       node     = getCurrentContext () -> createNode (getGeometryComboBoxText () .get_active_text ());

		   if (geometryNode and geometryNode -> getType () .back () == node -> getType () .back ())
				node = geometryNode;

			for (const auto & shapeNode : shapeNodes)
			{
				auto & field = shapeNode -> geometry ();

				X3D::X3DEditor::replaceNode (getCurrentContext (), shapeNode, field, node, undoStep);
			}

			getBrowserWindow () -> addUndoStep (undoStep);
		}
		catch (const X3D::X3DError & error)
		{
			__LOG__ << error .what () << std::endl;
		}
	}
	else if (getGeometryComboBoxText () .get_active_row_number () == 0)
	{
		const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Change Field »geometry«"));

		for (const auto & shapeNode : shapeNodes)
		{
			auto & field = shapeNode -> geometry ();

			X3D::X3DEditor::removeNode (getCurrentContext (), shapeNode, field, undoStep);
		}

		getBrowserWindow () -> addUndoStep (undoStep);
	}
}

void
GeometryPropertiesEditor::on_geometry_unlink_clicked ()
{
	X3D::UndoStepPtr undoStep;

	unlinkClone (shapeNodes, "geometry", undoStep);
}

void
GeometryPropertiesEditor::set_geometry ()
{
	nodesBuffer .addEvent ();
}

void
GeometryPropertiesEditor::connectGeometry (const X3D::SFNode & field)
{
	field .removeInterest (&GeometryPropertiesEditor::connectGeometry, this);
	field .addInterest (&GeometryPropertiesEditor::set_geometry, this);
}

void
GeometryPropertiesEditor::set_buffer ()
{
	X3DArc2DEditor::set_geometry ();
	X3DArcClose2DEditor::set_geometry ();
	X3DCircle2DEditor::set_geometry ();
	X3DDisk2DEditor::set_geometry ();
	X3DRectangle2DEditor::set_geometry ();
	X3DBoxEditor::set_geometry ();
	X3DConeEditor::set_geometry ();
	X3DCylinderEditor::set_geometry ();
	X3DElevationGridEditor::set_geometry ();
	X3DExtrusionEditor::set_geometry ();
	X3DSphereEditor::set_geometry ();
	X3DGeoElevationGridEditor::set_geometry ();
	X3DNurbsCurveEditor::set_geometry ();
	X3DNurbsPatchSurfaceEditor::set_geometry ();
	X3DNurbsTrimmedSurfaceEditor::set_geometry ();
	X3DIndexedLineSetEditor::set_geometry ();

	changing = true;

	for (const auto & node : geometryNodes)
	{
		try
		{
			node -> getField <X3D::SFNode> ("color") .removeInterest (&GeometryPropertiesEditor::set_color, this);
		}
		catch (const X3D::X3DError &)
		{ }

		try
		{
			node -> getField <X3D::SFNode> ("normal") .removeInterest (&GeometryPropertiesEditor::set_normal, this);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	// Find X3DGeometryNodes.

	size_t numGeometryNodes = 0;

	for (const auto & shapeNode : shapeNodes)
		numGeometryNodes += shapeNode -> getGeometry ();

	geometryNodes = getNodes <X3D::X3DBaseNode> (shapeNodes, { X3D::X3DConstants::X3DGeometryNode });

	if (geometryNodes .empty ())
		geometryNodes = getSelection <X3D::X3DBaseNode> ({ X3D::X3DConstants::X3DGeometryNode });

	geometryNode = geometryNodes .empty () ? nullptr : geometryNodes .back ();

	const auto count = geometryNode
	                   ? std::count_if (geometryNodes .begin (),
	                                    geometryNodes .end (),
	                                    [&] (const X3D::SFNode & node) { return node -> getType () .back () == geometryNode -> getType () .back (); })
	                   : 0;

	const auto allSameType = count and count == geometryNodes .size ();

	// Adjust widgets.

	const auto widgets  = getGeometryStack () .get_children ();
	const auto visibles = std::count_if (widgets .begin (), widgets .end (), [ ] (const Gtk::Widget* widget) { return widget -> get_visible (); });

	solid             .setNodes (geometryNodes);
	ccw               .setNodes (geometryNodes);
	convex            .setNodes (geometryNodes);
	creaseAngle       .setNodes (geometryNodes);
	creaseAngleDouble .setNodes (geometryNodes);
	colorPerVertex    .setNodes (geometryNodes);
	normalPerVertex   .setNodes (geometryNodes);

	// Normals Box

	getGeometryComboBoxText () .set_sensitive (not shapeNodes .empty ());

	if (allSameType and numGeometryNodes == shapeNodes .size ())
		getGeometryComboBoxText () .set_active_text (geometryNode -> getTypeName ());
	else if (geometryNodes .empty ())
		getGeometryComboBoxText () .set_active (0);
	else
		getGeometryComboBoxText () .set_active (-1);

	getSelectGeometryBox ()    .set_sensitive (not shapeNodes .empty ());
	getGeometryUnlinkButton () .set_sensitive (geometryNodes .size () == 1 and geometryNode -> getCloneCount () > 1);
	getGeometryStack ()        .set_visible (visibles == 1 and allSameType and numGeometryNodes == shapeNodes .size ());
	getNormalsBox ()           .set_sensitive (false);

	for (const auto & node : geometryNodes)
	{
		try
		{
			node -> getField <X3D::SFNode> ("color") .addInterest (&GeometryPropertiesEditor::set_color, this);
		}
		catch (const X3D::X3DError &)
		{ }

		try
		{
			node -> getField <X3D::SFNode> ("normal") .addInterest (&GeometryPropertiesEditor::set_normal, this);

			getNormalsBox () .set_sensitive (true);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	changing = false;

	set_color ();
	set_normal ();
}

void
GeometryPropertiesEditor::on_color_changed ()
{
	if (changing)
		return;

	if (getColorTypeButton () .get_active_row_number () > 0)
	{
	   try
	   {
			const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Change Field »color«"));

		   X3D::X3DPtr <X3D::X3DColorNode> node (getCurrentContext () -> createNode (getColorTypeButton () .get_active_text ()));

			if (colorNodes .size () and node -> getType () .back () == X3D::X3DConstants::Color)
				node = X3D::X3DPtr <X3D::X3DColorNode> (colorNodes .back ());

			else if (colorRGBANodes .size () and node -> getType () .back () == X3D::X3DConstants::ColorRGBA)
				node = X3D::X3DPtr <X3D::X3DColorNode> (colorRGBANodes .back ());

			else
			{
				// Assign and convert existing color node if switched from Color to ColorRGBA or back.

				if (colorNodes .size () and node -> getType () .back () == X3D::X3DConstants::ColorRGBA)
				{
					const X3D::X3DPtr <X3D::X3DColorNode> colorNode (colorNodes .back ());

					for (size_t i = 0, size = colorNode -> getSize (); i < size; ++ i)
						node -> set1Color (i, colorNode -> get1Color (i));
				}
				else if (colorRGBANodes .size () and node -> getType () .back () == X3D::X3DConstants::Color)
				{
					const X3D::X3DPtr <X3D::X3DColorNode> colorRGBANode (colorRGBANodes .back ());

					for (size_t i = 0, size = colorRGBANode -> getSize (); i < size; ++ i)
						node -> set1Color (i, colorRGBANode -> get1Color (i));
				}
			}

			for (const auto & geometryNode : geometryNodes)
			{
				try
				{
					auto & field = geometryNode -> getField <X3D::SFNode> ("color");

					X3D::X3DEditor::replaceNode (getCurrentContext (), geometryNode, field, node, undoStep);
				}
				catch (const X3D::X3DError &)
				{ }
			}

			getBrowserWindow () -> addUndoStep (undoStep);
		}
		catch (const X3D::X3DError & error)
		{
			__LOG__ << error .what () << std::endl;
		}
	}
	else if (getColorTypeButton () .get_active_row_number () == 0)
	{
		const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Change Field »color«"));

		for (const auto & geometryNode : geometryNodes)
		{
			try
			{
				auto & field = geometryNode -> getField <X3D::SFNode> ("color");

				X3D::X3DEditor::removeNode (getCurrentContext (), geometryNode, field, undoStep);
			}
			catch (const X3D::X3DError &)
			{ }
		}

		getBrowserWindow () -> addUndoStep (undoStep);
	}
}

void
GeometryPropertiesEditor::on_color_unlink_clicked ()
{
	X3D::UndoStepPtr undoStep;

	unlinkClone (geometryNodes, "color", undoStep);
}

void
GeometryPropertiesEditor::set_color ()
{
	colorBuffer .addEvent ();
}

void
GeometryPropertiesEditor::set_color_buffer ()
{
	changing = true;

	colorNodes     = getNodes <X3D::X3DBaseNode> (shapeNodes, { X3D::X3DConstants::Color });
	colorRGBANodes = getNodes <X3D::X3DBaseNode> (shapeNodes, { X3D::X3DConstants::ColorRGBA });

	size_t numColorFields    = 0;
	size_t numColorNodes     = 0;
	size_t numColorRGBANodes = 0;

	for (const auto & node : geometryNodes)
	{
		try
		{
			const auto & colorNode = node -> getField <X3D::SFNode> ("color");

			++ numColorFields;

			if (not colorNode)
				continue;

			if (colorNode -> isType ({ X3D::X3DConstants::Color }))
				++ numColorNodes;

			else if (colorNode -> isType ({ X3D::X3DConstants::ColorRGBA }))
				++ numColorRGBANodes;
		}
		catch (const X3D::X3DError &)
		{ }
	}

	getColorsBox () .set_visible (numColorFields == geometryNodes .size ());

	if (numColorNodes == geometryNodes .size ())
	{
		// Color
		getColorTypeButton ()   .set_active (1);
		getColorUnlinkButton () .set_sensitive (colorNodes .size () == 1 and colorNodes .back () -> getCloneCount () > 1);
	}
	else if (numColorRGBANodes == geometryNodes .size ())
	{
		// ColorRGBA
		getColorTypeButton ()   .set_active (2);
		getColorUnlinkButton () .set_sensitive (colorRGBANodes .size () == 1 and colorRGBANodes .back () -> getCloneCount () > 1);
	}
	else if (numColorNodes or numColorRGBANodes)
	{
		// Inconsitent
		getColorTypeButton ()   .set_active (-1);
		getColorUnlinkButton () .set_sensitive (false);
	}
	else
	{
		// None
		getColorTypeButton ()   .set_active (0);
		getColorUnlinkButton () .set_sensitive (false);
	}

	color     .setNodes (numColorNodes     == geometryNodes .size () ? colorNodes     : X3D::MFNode ());
	colorRGBA .setNodes (numColorRGBANodes == geometryNodes .size () ? colorRGBANodes : X3D::MFNode ());

	changing = false;
}

void
GeometryPropertiesEditor::set_normal ()
{
	bool normal = false;

	for (const auto & node : geometryNodes)
	{
		try
		{
			if (normal or_eq node -> getField <X3D::SFNode> ("normal"))
				break;
		}
		catch (const X3D::X3DError &)
		{ }
	}

	getRemoveNormalsButton () .set_sensitive (normal);
}

void
GeometryPropertiesEditor::on_add_normals_clicked ()
{
	const auto geometries = getSelection <X3D::X3DBaseNode> ({ X3D::X3DConstants::X3DGeometryNode });
	const auto undoStep   = std::make_shared <X3D::UndoStep> (_ ("Add Normals"));

	for (const auto & geometry : geometries)
	{
		for (const auto & type : basic::make_reverse_range (geometry -> getType ()))
		{
			switch (type)
			{
				case X3D::X3DConstants::ElevationGrid:
				{
					const auto elevationGrid = dynamic_cast <X3D::ElevationGrid*> (geometry .getValue ());

					undoStep -> addObjects (geometry);
					undoStep -> addUndoFunction (&X3D::SFBool::setValue, std::ref (elevationGrid -> normalPerVertex ()), elevationGrid -> normalPerVertex ());
					undoStep -> addRedoFunction (&X3D::SFBool::setValue, std::ref (elevationGrid -> normalPerVertex ()), true);
					elevationGrid -> normalPerVertex () = true;

					X3D::X3DEditor::replaceNode (getCurrentContext (), geometry, elevationGrid -> normal (), nullptr, undoStep);

					elevationGrid -> addNormals ();

					undoStep -> addRedoFunction (&X3D::SFNode::setValue, std::ref (elevationGrid -> normal ()), elevationGrid -> normal ());
					break;
				}
				case X3D::X3DConstants::GeoElevationGrid:
				{
					const auto geoElevationGrid = dynamic_cast <X3D::GeoElevationGrid*> (geometry .getValue ());

					undoStep -> addObjects (geometry);
					undoStep -> addUndoFunction (&X3D::SFBool::setValue, std::ref (geoElevationGrid -> normalPerVertex ()), geoElevationGrid -> normalPerVertex ());
					undoStep -> addRedoFunction (&X3D::SFBool::setValue, std::ref (geoElevationGrid -> normalPerVertex ()), true);
					geoElevationGrid -> normalPerVertex () = true;

					X3D::X3DEditor::replaceNode (getCurrentContext (), geometry, geoElevationGrid -> normal (), nullptr, undoStep);

					geoElevationGrid -> addNormals ();

					undoStep -> addRedoFunction (&X3D::SFNode::setValue, std::ref (geoElevationGrid -> normal ()), geoElevationGrid -> normal ());
					break;
				}
				case X3D::X3DConstants::IndexedFaceSet:
				{
					const auto indexedFaceSet = dynamic_cast <X3D::IndexedFaceSet*> (geometry .getValue ());

					undoStep -> addObjects (geometry);
					undoStep -> addUndoFunction (&X3D::SFBool::setValue, std::ref (indexedFaceSet -> normalPerVertex ()), indexedFaceSet -> normalPerVertex ());
					undoStep -> addRedoFunction (&X3D::SFBool::setValue, std::ref (indexedFaceSet -> normalPerVertex ()), true);
					indexedFaceSet -> normalPerVertex () = true;

					undoStep -> addUndoFunction (&X3D::MFInt32::setValue, std::ref (indexedFaceSet -> normalIndex ()), indexedFaceSet -> normalIndex ());
					X3D::X3DEditor::replaceNode (getCurrentContext (), geometry, indexedFaceSet -> normal (), nullptr, undoStep);

					indexedFaceSet -> addNormals ();

					undoStep -> addRedoFunction (&X3D::MFInt32::setValue, std::ref (indexedFaceSet -> normalIndex ()), indexedFaceSet -> normalIndex ());
					undoStep -> addRedoFunction (&X3D::SFNode::setValue,  std::ref (indexedFaceSet -> normal ()),      indexedFaceSet -> normal ());
					break;
				}
				case X3D::X3DConstants::X3DComposedGeometryNode:
				{
					const auto composedGeometryNode = dynamic_cast <X3D::X3DComposedGeometryNode*> (geometry .getValue ());

					undoStep -> addObjects (geometry);
					X3D::X3DEditor::replaceNode (getCurrentContext (), geometry, composedGeometryNode -> normal (), nullptr, undoStep);

					composedGeometryNode -> addNormals ();

					undoStep -> addRedoFunction (&X3D::SFNode::setValue, std::ref (composedGeometryNode -> normal ()), composedGeometryNode -> normal ());
					break;
				}
				default:
					continue;
			}

			break;
		}
	}

	getBrowserWindow () -> addUndoStep (undoStep);
}

void
GeometryPropertiesEditor::on_remove_normals_clicked ()
{
	const auto geometries = getSelection <X3D::X3DBaseNode> ({ X3D::X3DConstants::X3DGeometryNode });
	const auto undoStep   = std::make_shared <X3D::UndoStep> (_ ("Remove Normals"));

	for (const auto & geometry : geometries)
	{
		try
		{
			auto & normalIndex = geometry -> getField <X3D::MFInt32> ("normalIndex");

			undoStep -> addObjects (geometry);
			undoStep -> addUndoFunction (&X3D::MFInt32::setValue, std::ref (normalIndex), normalIndex);
			undoStep -> addRedoFunction (&X3D::MFInt32::clear, std::ref (normalIndex));
			normalIndex .clear ();
		}
		catch (const X3D::X3DError &)
		{ }

		try
		{
			auto & normal = geometry -> getField <X3D::SFNode> ("normal");

			X3D::X3DEditor::replaceNode (getCurrentContext (), geometry, normal, nullptr, undoStep);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	getBrowserWindow () -> addUndoStep (undoStep);
}

void
GeometryPropertiesEditor::store ()
{
	getConfig () -> setItem ("currentPage", getGeometryChildNotebook () .get_current_page ());

	X3DGeometryTool::store ();
	X3DPrimitiveCountEditor::store ();
	X3DIndexedLineSetEditor::store ();
	X3DBoxEditor::store ();
	X3DRectangle2DEditor::store ();
	X3DGeometryPropertiesEditorInterface::store ();
}

GeometryPropertiesEditor::~GeometryPropertiesEditor ()
{
	dispose ();
}

} // puck
} // titania
