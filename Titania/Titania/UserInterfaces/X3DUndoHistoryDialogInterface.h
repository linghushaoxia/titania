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
#ifndef __TMP_GLAD2CPP_UNDO_HISTORY_DIALOG_H__
#define __TMP_GLAD2CPP_UNDO_HISTORY_DIALOG_H__

#include "../Base/X3DDialogInterface.h"
#include <gtkmm.h>
#include <string>

namespace titania {
namespace puck {

/**
 *  Gtk Interface for UndoHistoryDialog.
 */
class X3DUndoHistoryDialogInterface :
	public X3DDialogInterface
{
public:

	///  @name Construction

	X3DUndoHistoryDialogInterface () :
		X3DDialogInterface ()
	{ }

	template <class ... Arguments>
	X3DUndoHistoryDialogInterface (const std::string & filename, const Arguments & ... arguments) :
		X3DDialogInterface (arguments ...),
		          filename (filename)
	{ create (filename); }

	///  @name Member access

	const Glib::RefPtr <Gtk::Builder> &
	getBuilder () const
	{ return m_builder; }

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
	getNumberColumn () const
	{ return m_NumberColumn; }

	const Glib::RefPtr <Gtk::TreeViewColumn> &
	getDescriptionColumn () const
	{ return m_DescriptionColumn; }

	Gtk::Window &
	getWindow () const
	{ return *m_Window; }

	Gtk::Box &
	getWidget () const
	{ return *m_Widget; }

	Gtk::HeaderBar &
	getHeaderBar () const
	{ return *m_HeaderBar; }

	Gtk::ScrolledWindow &
	getScrolledWindow () const
	{ return *m_ScrolledWindow; }

	Gtk::TreeView &
	getTreeView () const
	{ return *m_TreeView; }

	Gtk::Button &
	getUndoButton () const
	{ return *m_UndoButton; }

	Gtk::Button &
	getRedoButton () const
	{ return *m_RedoButton; }

	///  @name Signal handlers

	virtual
	void
	on_row_activated (const Gtk::TreeModel::Path &, Gtk::TreeViewColumn*) = 0;

	virtual
	void
	on_undo_clicked () = 0;

	virtual
	void
	on_redo_clicked () = 0;

	///  @name Destruction

	virtual
	~X3DUndoHistoryDialogInterface () override;


private:

	///  @name Construction

	virtual
	void
	construct () final override
	{ X3DDialogInterface::construct (); }

	void
	create (const std::string &);

	///  @name Static members

	///  @name Members

	std::string                        filename;
	Glib::RefPtr <Gtk::Builder>        m_builder;
	Glib::RefPtr <Gtk::ListStore>      m_ListStore;
	Glib::RefPtr <Gtk::TreeViewColumn> m_NumberColumn;
	Glib::RefPtr <Gtk::TreeViewColumn> m_DescriptionColumn;
	Gtk::Window*                       m_Window;
	Gtk::Box*                          m_Widget;
	Gtk::HeaderBar*                    m_HeaderBar;
	Gtk::ScrolledWindow*               m_ScrolledWindow;
	Gtk::TreeView*                     m_TreeView;
	Gtk::Button*                       m_UndoButton;
	Gtk::Button*                       m_RedoButton;

};

} // puck
} // titania

#endif