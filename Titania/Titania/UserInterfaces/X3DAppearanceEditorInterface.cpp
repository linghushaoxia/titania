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
#include "X3DAppearanceEditorInterface.h"

namespace titania {
namespace puck {

const std::string X3DAppearanceEditorInterface::m_widgetName = "AppearanceEditor";

void
X3DAppearanceEditorInterface::create (const std::string & filename)
{
	// Create Builder.
	m_builder = Gtk::Builder::create_from_file (filename);

	// Get objects.
	m_AmbientIntensityAdjustment                   = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("AmbientIntensityAdjustment"));
	m_BackAmbientIntensityAdjustment               = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("BackAmbientIntensityAdjustment"));
	m_BackDiffuseColorAdjustment                   = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("BackDiffuseColorAdjustment"));
	m_BackEmissiveColorAdjustment                  = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("BackEmissiveColorAdjustment"));
	m_BackShininessAdjustment                      = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("BackShininessAdjustment"));
	m_BackSpecularColorAdjustment                  = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("BackSpecularColorAdjustment"));
	m_BackTransparencyAdjustment                   = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("BackTransparencyAdjustment"));
	m_DiffuseColorAdjustment                       = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("DiffuseColorAdjustment"));
	m_EmissiveColorAdjustment                      = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("EmissiveColorAdjustment"));
	m_FillPropertiesHatchColorAdjustment           = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("FillPropertiesHatchColorAdjustment"));
	m_FillPropertiesHatchStyleAdjustment           = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("FillPropertiesHatchStyleAdjustment"));
	m_LinePropertiesLinetypeAdjustment             = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("LinePropertiesLinetypeAdjustment"));
	m_LinePropertiesLinewidthScaleFactorAdjustment = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("LinePropertiesLinewidthScaleFactorAdjustment"));
	m_ShininessAdjustment                          = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("ShininessAdjustment"));
	m_SpecularColorAdjustment                      = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("SpecularColorAdjustment"));
	m_TransparencyAdjustment                       = Glib::RefPtr <Gtk::Adjustment>::cast_dynamic (m_builder -> get_object ("TransparencyAdjustment"));

	// Get widgets.
	m_builder -> get_widget ("Window", m_Window);
	m_builder -> get_widget ("Widget", m_Widget);
	m_builder -> get_widget ("EditMenuItem", m_EditMenuItem);
	m_builder -> get_widget ("CopyMenuItem", m_CopyMenuItem);
	m_builder -> get_widget ("PasteMenuItem", m_PasteMenuItem);
	m_builder -> get_widget ("PreviewBox", m_PreviewBox);
	m_builder -> get_widget ("SphereButton", m_SphereButton);
	m_builder -> get_widget ("ModelButton", m_ModelButton);
	m_builder -> get_widget ("AppearanceChildNotebook", m_AppearanceChildNotebook);
	m_builder -> get_widget ("MaterialBox", m_MaterialBox);
	m_builder -> get_widget ("MaterialComboBoxText", m_MaterialComboBoxText);
	m_builder -> get_widget ("MaterialUnlinkButton", m_MaterialUnlinkButton);
	m_builder -> get_widget ("FrontExpander", m_FrontExpander);
	m_builder -> get_widget ("FrontBox", m_FrontBox);
	m_builder -> get_widget ("AmbientIntensityBox", m_AmbientIntensityBox);
	m_builder -> get_widget ("AmbientIntensityScale", m_AmbientIntensityScale);
	m_builder -> get_widget ("ShininessBox", m_ShininessBox);
	m_builder -> get_widget ("ShininessScale", m_ShininessScale);
	m_builder -> get_widget ("TransparencyBox", m_TransparencyBox);
	m_builder -> get_widget ("TransparencyScale", m_TransparencyScale);
	m_builder -> get_widget ("DiffuseColorBox", m_DiffuseColorBox);
	m_builder -> get_widget ("DiffuseColorButton", m_DiffuseColorButton);
	m_builder -> get_widget ("SpecularColorBox", m_SpecularColorBox);
	m_builder -> get_widget ("SpecularColorButton", m_SpecularColorButton);
	m_builder -> get_widget ("EmissiveColorBox", m_EmissiveColorBox);
	m_builder -> get_widget ("EmissiveColorButton", m_EmissiveColorButton);
	m_builder -> get_widget ("BackExpander", m_BackExpander);
	m_builder -> get_widget ("BackBox", m_BackBox);
	m_builder -> get_widget ("SeparateBackColorCheckButton", m_SeparateBackColorCheckButton);
	m_builder -> get_widget ("BackAmbientIntensityBox", m_BackAmbientIntensityBox);
	m_builder -> get_widget ("BackAmbientIntensityScale", m_BackAmbientIntensityScale);
	m_builder -> get_widget ("BackShininessBox", m_BackShininessBox);
	m_builder -> get_widget ("BackShininessScale", m_BackShininessScale);
	m_builder -> get_widget ("BackTransparencyBox", m_BackTransparencyBox);
	m_builder -> get_widget ("BackTransparencyScale", m_BackTransparencyScale);
	m_builder -> get_widget ("BackDiffuseColorBox", m_BackDiffuseColorBox);
	m_builder -> get_widget ("BackDiffuseColorButton", m_BackDiffuseColorButton);
	m_builder -> get_widget ("BackSpecularColorBox", m_BackSpecularColorBox);
	m_builder -> get_widget ("BackSpecularColorButton", m_BackSpecularColorButton);
	m_builder -> get_widget ("BackEmissiveColorBox", m_BackEmissiveColorBox);
	m_builder -> get_widget ("BackEmissiveColorButton", m_BackEmissiveColorButton);
	m_builder -> get_widget ("FillPropertiesExpander", m_FillPropertiesExpander);
	m_builder -> get_widget ("FillPropertiesMainBox", m_FillPropertiesMainBox);
	m_builder -> get_widget ("FillPropertiesCheckButton", m_FillPropertiesCheckButton);
	m_builder -> get_widget ("FillPropertiesUnlinkButton", m_FillPropertiesUnlinkButton);
	m_builder -> get_widget ("FillPropertiesBox", m_FillPropertiesBox);
	m_builder -> get_widget ("FillPropertiesFilledCheckButton", m_FillPropertiesFilledCheckButton);
	m_builder -> get_widget ("FillPropertiesHatchedCheckButton", m_FillPropertiesHatchedCheckButton);
	m_builder -> get_widget ("FillPropertiesHatchColorBox", m_FillPropertiesHatchColorBox);
	m_builder -> get_widget ("FillPropertiesHatchColorButton", m_FillPropertiesHatchColorButton);
	m_builder -> get_widget ("FillPropertiesHatchStyleSpinButton", m_FillPropertiesHatchStyleSpinButton);
	m_builder -> get_widget ("LinePropertiesExpander", m_LinePropertiesExpander);
	m_builder -> get_widget ("LinePropertiesMainBox", m_LinePropertiesMainBox);
	m_builder -> get_widget ("LinePropertiesCheckButton", m_LinePropertiesCheckButton);
	m_builder -> get_widget ("LinePropertiesUnlinkButton", m_LinePropertiesUnlinkButton);
	m_builder -> get_widget ("LinePropertiesBox", m_LinePropertiesBox);
	m_builder -> get_widget ("LinePropertiesFilledCheckButton", m_LinePropertiesFilledCheckButton);
	m_builder -> get_widget ("LinePropertiesLinetypeSpinButton", m_LinePropertiesLinetypeSpinButton);
	m_builder -> get_widget ("LinePropertiesLinewidthScaleFactorSpinButton", m_LinePropertiesLinewidthScaleFactorSpinButton);

	// Connect object Gtk::ImageMenuItem with id 'CopyMenuItem'.
	m_CopyMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DAppearanceEditorInterface::on_copy));
	m_PasteMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DAppearanceEditorInterface::on_paste));

	// Connect object Gtk::Button with id 'SphereButton'.
	m_SphereButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DAppearanceEditorInterface::on_sphere_clicked));
	m_ModelButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DAppearanceEditorInterface::on_model_clicked));

	// Connect object Gtk::ComboBoxText with id 'MaterialComboBoxText'.
	m_MaterialComboBoxText -> signal_changed () .connect (sigc::mem_fun (*this, &X3DAppearanceEditorInterface::on_material_changed));

	// Connect object Gtk::Button with id 'MaterialUnlinkButton'.
	m_MaterialUnlinkButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DAppearanceEditorInterface::on_material_unlink_clicked));

	// Connect object Gtk::CheckButton with id 'FillPropertiesCheckButton'.
	m_FillPropertiesCheckButton -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DAppearanceEditorInterface::on_fillProperties_toggled));

	// Connect object Gtk::Button with id 'FillPropertiesUnlinkButton'.
	m_FillPropertiesUnlinkButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DAppearanceEditorInterface::on_fillProperties_unlink_clicked));

	// Connect object Gtk::CheckButton with id 'LinePropertiesCheckButton'.
	m_LinePropertiesCheckButton -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DAppearanceEditorInterface::on_lineProperties_toggled));

	// Connect object Gtk::Button with id 'LinePropertiesUnlinkButton'.
	m_LinePropertiesUnlinkButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DAppearanceEditorInterface::on_lineProperties_unlink_clicked));

	// Call construct handler of base class.
	construct ();
}

X3DAppearanceEditorInterface::~X3DAppearanceEditorInterface ()
{
	delete m_Window;
}

} // puck
} // titania