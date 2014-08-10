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
#ifndef __TMP_GLAD2CPP_LIGHT_EDITOR_H__
#define __TMP_GLAD2CPP_LIGHT_EDITOR_H__

#include "../Base/X3DEditorInterface.h"
#include <gtkmm.h>
#include <string>

namespace titania {
namespace puck {

using namespace Gtk;

class X3DLightEditorInterface :
	public X3DEditorInterface
{
public:

	template <class ... Arguments>
	X3DLightEditorInterface (const std::string & filename, const Arguments & ... arguments) :
		X3DEditorInterface (m_widgetName, arguments ...),
		          filename (filename)
	{ create (filename); }

	const Glib::RefPtr <Gtk::Builder> &
	getBuilder () const { return m_builder; }

	const std::string &
	getWidgetName () const { return m_widgetName; }

	void
	updateWidget (const Glib::ustring & name) const
	{ getBuilder () -> add_from_file (filename, name); }

	void
	updateWidgets (const std::vector <Glib::ustring> & names) const
	{ getBuilder () -> add_from_file (filename, names); }

	template <class Type>
	Type*
	getWidget (const std::string & name) const
	{
		Type* widget = nullptr;

		m_builder -> get_widget (name, widget);
		widget -> set_name (name);
		return widget;
	}

	const Glib::RefPtr <Gtk::Adjustment> &
	getAmbientIntensityAdjustment () const
	{ return m_AmbientIntensityAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getColorAdjustment () const
	{ return m_ColorAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getDirectionalLightDirectionXAdjustment () const
	{ return m_DirectionalLightDirectionXAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getDirectionalLightDirectionYAdjustment () const
	{ return m_DirectionalLightDirectionYAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getDirectionalLightDirectionZAdjustment () const
	{ return m_DirectionalLightDirectionZAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getIntensityAdjustment () const
	{ return m_IntensityAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getPointLightAttenuationXAdjustment () const
	{ return m_PointLightAttenuationXAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getPointLightAttenuationYAdjustment () const
	{ return m_PointLightAttenuationYAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getPointLightAttenuationZAdjustment () const
	{ return m_PointLightAttenuationZAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getPointLightLocationXAdjustment () const
	{ return m_PointLightLocationXAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getPointLightLocationYAdjustment () const
	{ return m_PointLightLocationYAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getPointLightLocationZAdjustment () const
	{ return m_PointLightLocationZAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getPointLightRadiusAdjustment () const
	{ return m_PointLightRadiusAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getSpotLightAttenuationXAdjustment () const
	{ return m_SpotLightAttenuationXAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getSpotLightAttenuationYAdjustment () const
	{ return m_SpotLightAttenuationYAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getSpotLightAttenuationZAdjustment () const
	{ return m_SpotLightAttenuationZAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getSpotLightBeamWidthAdjustment () const
	{ return m_SpotLightBeamWidthAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getSpotLightCutOffAngleAdjustment () const
	{ return m_SpotLightCutOffAngleAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getSpotLightDirectionXAdjustment () const
	{ return m_SpotLightDirectionXAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getSpotLightDirectionYAdjustment () const
	{ return m_SpotLightDirectionYAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getSpotLightDirectionZAdjustment () const
	{ return m_SpotLightDirectionZAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getSpotLightLocationXAdjustment () const
	{ return m_SpotLightLocationXAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getSpotLightLocationYAdjustment () const
	{ return m_SpotLightLocationYAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getSpotLightLocationZAdjustment () const
	{ return m_SpotLightLocationZAdjustment; }

	const Glib::RefPtr <Gtk::Adjustment> &
	getSpotLightRadiusAdjustment () const
	{ return m_SpotLightRadiusAdjustment; }

	Gtk::Window &
	getWindow () const
	{ return *m_Window; }

	Gtk::Box &
	getWidget () const
	{ return *m_Widget; }

	Gtk::Expander &
	getLightExpander () const
	{ return *m_LightExpander; }

	Gtk::Grid &
	getLightBox () const
	{ return *m_LightBox; }

	Gtk::CheckButton &
	getGlobalCheckButton () const
	{ return *m_GlobalCheckButton; }

	Gtk::CheckButton &
	getOnCheckButton () const
	{ return *m_OnCheckButton; }

	Gtk::Box &
	getColorBox () const
	{ return *m_ColorBox; }

	Gtk::Button &
	getColorButton () const
	{ return *m_ColorButton; }

	Gtk::Box &
	getIntensityBox () const
	{ return *m_IntensityBox; }

	Gtk::Scale &
	getIntensityScale () const
	{ return *m_IntensityScale; }

	Gtk::Box &
	getAmbientIntensityBox () const
	{ return *m_AmbientIntensityBox; }

	Gtk::Scale &
	getAmbientIntensityScale () const
	{ return *m_AmbientIntensityScale; }

	Gtk::Expander &
	getDirectionalLightExpander () const
	{ return *m_DirectionalLightExpander; }

	Gtk::Box &
	getDirectionalLightDirectionBox () const
	{ return *m_DirectionalLightDirectionBox; }

	Gtk::SpinButton &
	getDirectionalLightDirectionXSpinButton () const
	{ return *m_DirectionalLightDirectionXSpinButton; }

	Gtk::SpinButton &
	getDirectionalLightDirectionYSpinButton () const
	{ return *m_DirectionalLightDirectionYSpinButton; }

	Gtk::SpinButton &
	getDirectionalLightDirectionZSpinButton () const
	{ return *m_DirectionalLightDirectionZSpinButton; }

	Gtk::Box &
	getDirectionalLightDirectionToolBox () const
	{ return *m_DirectionalLightDirectionToolBox; }

	Gtk::Expander &
	getPointLightExpander () const
	{ return *m_PointLightExpander; }

	Gtk::Box &
	getPointLightLocationBox () const
	{ return *m_PointLightLocationBox; }

	Gtk::SpinButton &
	getPointLightLocationXSpinButton () const
	{ return *m_PointLightLocationXSpinButton; }

	Gtk::SpinButton &
	getPointLightLocationYSpinButton () const
	{ return *m_PointLightLocationYSpinButton; }

	Gtk::SpinButton &
	getPointLightLocationZSpinButton () const
	{ return *m_PointLightLocationZSpinButton; }

	Gtk::Box &
	getPointLightAttenuationBox () const
	{ return *m_PointLightAttenuationBox; }

	Gtk::SpinButton &
	getPointLightAttenuationXSpinButton () const
	{ return *m_PointLightAttenuationXSpinButton; }

	Gtk::SpinButton &
	getPointLightAttenuationYSpinButton () const
	{ return *m_PointLightAttenuationYSpinButton; }

	Gtk::SpinButton &
	getPointLightAttenuationZSpinButton () const
	{ return *m_PointLightAttenuationZSpinButton; }

	Gtk::SpinButton &
	getPointLightRadiusSpinButton () const
	{ return *m_PointLightRadiusSpinButton; }

	Gtk::Expander &
	getSpotLightExpander () const
	{ return *m_SpotLightExpander; }

	Gtk::Box &
	getSpotLightAttenuationBox () const
	{ return *m_SpotLightAttenuationBox; }

	Gtk::SpinButton &
	getSpotLightAttenuationXSpinButton () const
	{ return *m_SpotLightAttenuationXSpinButton; }

	Gtk::SpinButton &
	getSpotLightAttenuationYSpinButton () const
	{ return *m_SpotLightAttenuationYSpinButton; }

	Gtk::SpinButton &
	getSpotLightAttenuationZSpinButton () const
	{ return *m_SpotLightAttenuationZSpinButton; }

	Gtk::Box &
	getSpotLightLocationBox () const
	{ return *m_SpotLightLocationBox; }

	Gtk::SpinButton &
	getSpotLightLocationXSpinButton () const
	{ return *m_SpotLightLocationXSpinButton; }

	Gtk::SpinButton &
	getSpotLightLocationYSpinButton () const
	{ return *m_SpotLightLocationYSpinButton; }

	Gtk::SpinButton &
	getSpotLightLocationZSpinButton () const
	{ return *m_SpotLightLocationZSpinButton; }

	Gtk::SpinButton &
	getSpotLightRadiusSpinButton () const
	{ return *m_SpotLightRadiusSpinButton; }

	Gtk::SpinButton &
	getSpotLightBeamWidthSpinButton () const
	{ return *m_SpotLightBeamWidthSpinButton; }

	Gtk::SpinButton &
	getSpotLightCutOffAngleSpinButton () const
	{ return *m_SpotLightCutOffAngleSpinButton; }

	Gtk::Box &
	getSpotLightDirectionBox () const
	{ return *m_SpotLightDirectionBox; }

	Gtk::SpinButton &
	getSpotLightDirectionXSpinButton () const
	{ return *m_SpotLightDirectionXSpinButton; }

	Gtk::SpinButton &
	getSpotLightDirectionYSpinButton () const
	{ return *m_SpotLightDirectionYSpinButton; }

	Gtk::SpinButton &
	getSpotLightDirectionZSpinButton () const
	{ return *m_SpotLightDirectionZSpinButton; }

	Gtk::Box &
	getSpotLightDirectionToolBox () const
	{ return *m_SpotLightDirectionToolBox; }

	virtual
	~X3DLightEditorInterface ();


private:

	void
	create (const std::string &);

	static const std::string m_widgetName;

	std::string                    filename;
	Glib::RefPtr <Gtk::Builder>    m_builder;
	Glib::RefPtr <Gtk::Adjustment> m_AmbientIntensityAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_ColorAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_DirectionalLightDirectionXAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_DirectionalLightDirectionYAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_DirectionalLightDirectionZAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_IntensityAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_PointLightAttenuationXAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_PointLightAttenuationYAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_PointLightAttenuationZAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_PointLightLocationXAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_PointLightLocationYAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_PointLightLocationZAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_PointLightRadiusAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_SpotLightAttenuationXAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_SpotLightAttenuationYAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_SpotLightAttenuationZAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_SpotLightBeamWidthAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_SpotLightCutOffAngleAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_SpotLightDirectionXAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_SpotLightDirectionYAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_SpotLightDirectionZAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_SpotLightLocationXAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_SpotLightLocationYAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_SpotLightLocationZAdjustment;
	Glib::RefPtr <Gtk::Adjustment> m_SpotLightRadiusAdjustment;
	Gtk::Window*                   m_Window;
	Gtk::Box*                      m_Widget;
	Gtk::Expander*                 m_LightExpander;
	Gtk::Grid*                     m_LightBox;
	Gtk::CheckButton*              m_GlobalCheckButton;
	Gtk::CheckButton*              m_OnCheckButton;
	Gtk::Box*                      m_ColorBox;
	Gtk::Button*                   m_ColorButton;
	Gtk::Box*                      m_IntensityBox;
	Gtk::Scale*                    m_IntensityScale;
	Gtk::Box*                      m_AmbientIntensityBox;
	Gtk::Scale*                    m_AmbientIntensityScale;
	Gtk::Expander*                 m_DirectionalLightExpander;
	Gtk::Box*                      m_DirectionalLightDirectionBox;
	Gtk::SpinButton*               m_DirectionalLightDirectionXSpinButton;
	Gtk::SpinButton*               m_DirectionalLightDirectionYSpinButton;
	Gtk::SpinButton*               m_DirectionalLightDirectionZSpinButton;
	Gtk::Box*                      m_DirectionalLightDirectionToolBox;
	Gtk::Expander*                 m_PointLightExpander;
	Gtk::Box*                      m_PointLightLocationBox;
	Gtk::SpinButton*               m_PointLightLocationXSpinButton;
	Gtk::SpinButton*               m_PointLightLocationYSpinButton;
	Gtk::SpinButton*               m_PointLightLocationZSpinButton;
	Gtk::Box*                      m_PointLightAttenuationBox;
	Gtk::SpinButton*               m_PointLightAttenuationXSpinButton;
	Gtk::SpinButton*               m_PointLightAttenuationYSpinButton;
	Gtk::SpinButton*               m_PointLightAttenuationZSpinButton;
	Gtk::SpinButton*               m_PointLightRadiusSpinButton;
	Gtk::Expander*                 m_SpotLightExpander;
	Gtk::Box*                      m_SpotLightAttenuationBox;
	Gtk::SpinButton*               m_SpotLightAttenuationXSpinButton;
	Gtk::SpinButton*               m_SpotLightAttenuationYSpinButton;
	Gtk::SpinButton*               m_SpotLightAttenuationZSpinButton;
	Gtk::Box*                      m_SpotLightLocationBox;
	Gtk::SpinButton*               m_SpotLightLocationXSpinButton;
	Gtk::SpinButton*               m_SpotLightLocationYSpinButton;
	Gtk::SpinButton*               m_SpotLightLocationZSpinButton;
	Gtk::SpinButton*               m_SpotLightRadiusSpinButton;
	Gtk::SpinButton*               m_SpotLightBeamWidthSpinButton;
	Gtk::SpinButton*               m_SpotLightCutOffAngleSpinButton;
	Gtk::Box*                      m_SpotLightDirectionBox;
	Gtk::SpinButton*               m_SpotLightDirectionXSpinButton;
	Gtk::SpinButton*               m_SpotLightDirectionYSpinButton;
	Gtk::SpinButton*               m_SpotLightDirectionZSpinButton;
	Gtk::Box*                      m_SpotLightDirectionToolBox;

};

} // puck
} // titania

#endif