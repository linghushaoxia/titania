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
#ifndef __TMP_GLAD2CPP_BINDABLE_NODE_LIST_H__
#define __TMP_GLAD2CPP_BINDABLE_NODE_LIST_H__

#include "../Base/X3DUserInterface.h"
#include <gtkmm.h>
#include <string>

namespace titania {
namespace puck {

/**
 *  Gtk Interface for BindableNodeList.
 */
class X3DBindableNodeListInterface :
	public X3DUserInterface
{
public:

	///  @name Construction

	X3DBindableNodeListInterface () :
		X3DUserInterface ()
	{ }

	template <class ... Arguments>
	X3DBindableNodeListInterface (const std::string & filename, const Arguments & ... arguments) :
		X3DUserInterface (m_widgetName, arguments ...),
		        filename (filename)
	{ create (filename); }

	///  @name Member access

	const Glib::RefPtr <Gtk::Builder> &
	getBuilder () const
	{ return m_builder; }

	virtual
	const std::string &
	getWidgetName () const
	{ return m_widgetName; }

	template <class Type>
	Type*
	createWidget (const std::string & name) const
	{
		getBuilder () -> add_from_file (filename, name);

		Type* widget = nullptr;
		m_builder -> get_widget (name, widget);
		return widget;
	}

	const Glib::RefPtr <Gtk::ListStore> &
	getListStore () const
	{ return m_ListStore; }

	const Glib::RefPtr <Gtk::TreeViewColumn> &
	getTypeNameColumn () const
	{ return m_TypeNameColumn; }

	const Glib::RefPtr <Gtk::CellRendererText> &
	getTypeNameCellRenderer () const
	{ return m_TypeNameCellRenderer; }

	const Glib::RefPtr <Gtk::TreeViewColumn> &
	getNameColumn () const
	{ return m_NameColumn; }

	const Glib::RefPtr <Gtk::CellRendererText> &
	getNameCellRenderer () const
	{ return m_NameCellRenderer; }

	const Glib::RefPtr <Gtk::TreeViewColumn> &
	getDescriptionColumn () const
	{ return m_DescriptionColumn; }

	const Glib::RefPtr <Gtk::CellRendererText> &
	getDescriptionCellRenderer () const
	{ return m_DescriptionCellRenderer; }

	const Glib::RefPtr <Gtk::TreeViewColumn> &
	getPadColumn () const
	{ return m_PadColumn; }

	const Glib::RefPtr <Gtk::TreeViewColumn> &
	getBindColumn () const
	{ return m_BindColumn; }

	const Glib::RefPtr <Gtk::CellRendererPixbuf> &
	getBindCellRenderer () const
	{ return m_BindCellRenderer; }

	Gtk::Window &
	getWindow () const
	{ return *m_Window; }

	Gtk::Box &
	getWidget () const
	{ return *m_Widget; }

	Gtk::Label &
	getLabel () const
	{ return *m_Label; }

	Gtk::ScrolledWindow &
	getScrolledWindow () const
	{ return *m_ScrolledWindow; }

	Gtk::TreeView &
	getTreeView () const
	{ return *m_TreeView; }

	///  @name Signal handlers

	virtual
	void
	on_map () = 0;

	virtual
	void
	on_unmap () = 0;

	virtual
	bool
	on_button_release_event (GdkEventButton* event) = 0;

	virtual
	void
	on_row_activated (const Gtk::TreeModel::Path &, Gtk::TreeViewColumn*) = 0;

	///  @name Destruction

	virtual
	~X3DBindableNodeListInterface ();


private:

	///  @name Construction

	virtual
	void
	construct () final override
	{ X3DUserInterface::construct (); }

	void
	create (const std::string &);

	///  @name Static members

	static const std::string m_widgetName;

	///  @name Members

	std::string                            filename;
	Glib::RefPtr <Gtk::Builder>            m_builder;
	Glib::RefPtr <Gtk::ListStore>          m_ListStore;
	Glib::RefPtr <Gtk::TreeViewColumn>     m_TypeNameColumn;
	Glib::RefPtr <Gtk::CellRendererText>   m_TypeNameCellRenderer;
	Glib::RefPtr <Gtk::TreeViewColumn>     m_NameColumn;
	Glib::RefPtr <Gtk::CellRendererText>   m_NameCellRenderer;
	Glib::RefPtr <Gtk::TreeViewColumn>     m_DescriptionColumn;
	Glib::RefPtr <Gtk::CellRendererText>   m_DescriptionCellRenderer;
	Glib::RefPtr <Gtk::TreeViewColumn>     m_PadColumn;
	Glib::RefPtr <Gtk::TreeViewColumn>     m_BindColumn;
	Glib::RefPtr <Gtk::CellRendererPixbuf> m_BindCellRenderer;
	Gtk::Window*                           m_Window;
	Gtk::Box*                              m_Widget;
	Gtk::Label*                            m_Label;
	Gtk::ScrolledWindow*                   m_ScrolledWindow;
	Gtk::TreeView*                         m_TreeView;

};

} // puck
} // titania

#endif