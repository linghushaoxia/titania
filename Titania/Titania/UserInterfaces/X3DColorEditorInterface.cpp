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
#include "X3DColorEditorInterface.h"

namespace titania {
namespace puck {

const std::string X3DColorEditorInterface::m_widgetName = "ColorEditor";

void
X3DColorEditorInterface::create (const std::string & filename)
{
	// Create Builder.
	m_builder = Gtk::Builder::create_from_file (filename);

	// Get objects.
	m_ColorAdjustment = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("ColorAdjustment"));

	// Get widgets.
	m_builder -> get_widget ("ShadingMenu", m_ShadingMenu);
	m_builder -> get_widget ("PhongMenuItem", m_PhongMenuItem);
	m_builder -> get_widget ("GouraudMenuItem", m_GouraudMenuItem);
	m_builder -> get_widget ("FlatMenuItem", m_FlatMenuItem);
	m_builder -> get_widget ("WireFrameMenuItem", m_WireFrameMenuItem);
	m_builder -> get_widget ("PointSetMenuItem", m_PointSetMenuItem);
	m_builder -> get_widget ("Window", m_Window);
	m_builder -> get_widget ("Widget", m_Widget);
	m_builder -> get_widget ("UndoMenuItem", m_UndoMenuItem);
	m_builder -> get_widget ("RedoMenuItem", m_RedoMenuItem);
	m_builder -> get_widget ("RemoveUnusedColorsMenuItem", m_RemoveUnusedColorsMenuItem);
	m_builder -> get_widget ("UndoButton", m_UndoButton);
	m_builder -> get_widget ("RedoButton", m_RedoButton);
	m_builder -> get_widget ("PreviewBox", m_PreviewBox);
	m_builder -> get_widget ("HandButton", m_HandButton);
	m_builder -> get_widget ("ArrowButton", m_ArrowButton);
	m_builder -> get_widget ("CheckerBoardButton", m_CheckerBoardButton);
	m_builder -> get_widget ("ShadingButton", m_ShadingButton);
	m_builder -> get_widget ("TextureButton", m_TextureButton);
	m_builder -> get_widget ("LookAtAllButton", m_LookAtAllButton);
	m_builder -> get_widget ("LookAtButton", m_LookAtButton);
	m_builder -> get_widget ("SelectColorButton", m_SelectColorButton);
	m_builder -> get_widget ("ColorButton", m_ColorButton);
	m_builder -> get_widget ("AddColorButton", m_AddColorButton);
	m_builder -> get_widget ("RemoveColorButton", m_RemoveColorButton);
	m_builder -> get_widget ("SingleVertexButton", m_SingleVertexButton);
	m_builder -> get_widget ("AdjacentVerticesButton", m_AdjacentVerticesButton);
	m_builder -> get_widget ("SingleFaceButton", m_SingleFaceButton);
	m_builder -> get_widget ("WholeObjectButton", m_WholeObjectButton);
	m_builder -> get_widget ("ColorsScrolledWindow", m_ColorsScrolledWindow);
	m_builder -> get_widget ("RemoveButton", m_RemoveButton);
	m_builder -> get_widget ("ApplyButton", m_ApplyButton);

	// Connect object Gtk::RadioMenuItem with id 'PhongMenuItem'.
	m_PhongMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DColorEditorInterface::on_phong_activate));
	m_GouraudMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DColorEditorInterface::on_gouraud_activate));
	m_FlatMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DColorEditorInterface::on_flat_activate));
	m_WireFrameMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DColorEditorInterface::on_wireframe_activate));
	m_PointSetMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DColorEditorInterface::on_pointset_activate));

	// Connect object Gtk::ImageMenuItem with id 'UndoMenuItem'.
	m_UndoMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DColorEditorInterface::on_undo));
	m_RedoMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DColorEditorInterface::on_redo));
	m_RemoveUnusedColorsMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DColorEditorInterface::on_remove_unused_colors_activate));

	// Connect object Gtk::ToolButton with id 'UndoButton'.
	m_UndoButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DColorEditorInterface::on_undo));
	m_RedoButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DColorEditorInterface::on_redo));

	// Connect object Gtk::RadioToolButton with id 'HandButton'.
	m_HandButton -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DColorEditorInterface::on_hand_toggled));
	m_ArrowButton -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DColorEditorInterface::on_arrow_toggled));

	// Connect object Gtk::ToggleToolButton with id 'CheckerBoardButton'.
	m_CheckerBoardButton -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DColorEditorInterface::on_checkerboard_toggled));
	m_TextureButton -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DColorEditorInterface::on_texture_toggled));

	// Connect object Gtk::ToolButton with id 'LookAtAllButton'.
	m_LookAtAllButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DColorEditorInterface::on_look_at_all_clicked));

	// Connect object Gtk::ToggleToolButton with id 'LookAtButton'.
	m_LookAtButton -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DColorEditorInterface::on_look_at_toggled));

	// Connect object Gtk::RadioButton with id 'SingleVertexButton'.
	m_SingleVertexButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DColorEditorInterface::on_single_vertex_clicked));
	m_AdjacentVerticesButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DColorEditorInterface::on_adjacent_vertices_clicked));
	m_SingleFaceButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DColorEditorInterface::on_single_face_clicked));
	m_WholeObjectButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DColorEditorInterface::on_whole_object_clicked));

	// Connect object Gtk::Button with id 'RemoveButton'.
	m_RemoveButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DColorEditorInterface::on_remove_clicked));
	m_ApplyButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DColorEditorInterface::on_apply_clicked));

	// Call construct handler of base class.
	construct ();
}

X3DColorEditorInterface::~X3DColorEditorInterface ()
{
	delete m_Window;
}

} // puck
} // titania
