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
#include "X3DNodeEditorInterface.h"

namespace titania {
namespace puck {

void
X3DNodeEditorInterface::create (const std::string & filename)
{
	// Create Builder.
	m_builder = Gtk::Builder::create_from_file (filename);

	create ();
}

void
X3DNodeEditorInterface::create (std::initializer_list <std::string> filenames)
{
	// Create Builder.
	m_builder = Gtk::Builder::create ();

	for (const auto & filename : filenames)
		m_builder -> add_from_file (filename);

	create ();
}

void
X3DNodeEditorInterface::create ()
{
	// Get objects.

	// Get widgets.
	m_builder -> get_widget ("Window", m_Window);
	m_builder -> get_widget ("Widget", m_Widget);
	m_builder -> get_widget ("Label", m_Label);
	m_builder -> get_widget ("Notebook", m_Notebook);
	m_builder -> get_widget ("NodePropertiesEditorBox", m_NodePropertiesEditorBox);
	m_builder -> get_widget ("AppearanceEditorBox", m_AppearanceEditorBox);
	m_builder -> get_widget ("TextureEditorBox", m_TextureEditorBox);
	m_builder -> get_widget ("GeometryPropertiesEditorBox", m_GeometryPropertiesEditorBox);
	m_builder -> get_widget ("TextEditorBox", m_TextEditorBox);
	m_builder -> get_widget ("LayerEditorBox", m_LayerEditorBox);
	m_builder -> get_widget ("BindableNodeEditorBox", m_BindableNodeEditorBox);
	m_builder -> get_widget ("LightEditorBox", m_LightEditorBox);
	m_builder -> get_widget ("SoundEditorBox", m_SoundEditorBox);
	m_builder -> get_widget ("InlineEditorBox", m_InlineEditorBox);
	m_builder -> get_widget ("PrecisionPlacementPanelBox", m_PrecisionPlacementPanelBox);
	m_builder -> get_widget ("SculpToolEditorBox", m_SculpToolEditorBox);

	// Connect object Gtk::Window with id 'Window'.
	m_Window -> signal_map () .connect (sigc::mem_fun (this, &X3DNodeEditorInterface::on_map_window));

	// Connect object Gtk::Notebook with id 'Notebook'.
	m_Notebook -> signal_switch_page () .connect (sigc::mem_fun (this, &X3DNodeEditorInterface::on_switch_page));
}

X3DNodeEditorInterface::~X3DNodeEditorInterface ()
{
	delete m_Window;
}

} // puck
} // titania
