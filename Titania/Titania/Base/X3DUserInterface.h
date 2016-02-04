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

#ifndef __TITANIA_BASE_X3DUSER_INTERFACE_H__
#define __TITANIA_BASE_X3DUSER_INTERFACE_H__

#include "../Base/X3DBaseInterface.h"
#include "../Configuration/Configuration.h"
#include <gtkmm.h>
#include <string>

namespace titania {
namespace puck {

class DialogFactory;

class X3DUserInterface :
	virtual public X3DBaseInterface
{
public:

	///  @name Member access

	///  Returns the type name of this object.
	virtual
	const std::string &
	getTypeName () const
	throw (X3D::Error <X3D::DISPOSED>) final override
	{ return getWidgetName (); }

	virtual
	void
	setName (const std::string &) override;

	virtual
	const std::string &
	getWidgetName () const = 0;

	virtual
	Gtk::Window &
	getWindow () const = 0;

	virtual
	Gtk::Widget &
	getWidget () const = 0;

	/// @name Dialog handling
	
	bool
	hasDialog (const std::string &) const;

	std::shared_ptr <X3DUserInterface>
	addDialog (const std::string &, const bool = true);

	///  @name Operations

	void
	reparent (Gtk::Box &, Gtk::Window &);

	/// @name Destruction

	virtual
	bool
	quit ();

	virtual
	~X3DUserInterface ();


protected:

	/// @name Construction

	X3DUserInterface ();

	X3DUserInterface (const std::string &, const std::string &);

	virtual
	void
	construct ();

	virtual
	void
	initialize ()
	{ }

	virtual
	void
	restoreSession ();

	virtual
	void
	saveSession ();

	bool
	isInitialized () const
	{ return not constructed_connection .connected (); }

	///  @name Configuration handling

	const std::shared_ptr <Configuration> &
	getConfig () const
	{ return gconf; }

	/// @name Member access
	
	bool
	isMaximized () const
	{ return getConfig () -> getBoolean ("maximized"); }

	bool
	isFullscreen () const
	{ return getConfig () -> getBoolean ("fullscreen"); }

	virtual
	void
	set_fullscreen (const bool)
	{ }


private:

	using UserInterfaceArray = std::list <X3DUserInterface*>;
	using DialogIndex        = std::map <std::string, std::shared_ptr <X3DUserInterface>>;

	///  @name Construction

	X3DUserInterface (const X3DUserInterface &) = delete;

	///  @name Event handlers

	void
	on_constructed ();

	void
	on_map ();
	
	bool
	on_window_state_event (GdkEventWindowState*);

	bool
	on_delete_event (GdkEventAny*);

	///  @name Operations

	void
	removeDialog (const std::string &);

	void
	removeDialogImpl (const std::string &);

	void
	restoreInterface ();

	void
	saveInterfaces ();

	void
	saveInterface ();

	///  @name Static members

	static const std::unique_ptr <DialogFactory> dialogFactory;
	static const std::set <std::string>          restorableDialogs;
	static UserInterfaceArray                    userInterfaces;

	///  @name Members

	std::shared_ptr <Configuration> gconf;
	sigc::connection                constructed_connection;
	UserInterfaceArray::iterator    userInterface;
	std::shared_ptr <DialogIndex>   dialogs;


};

} // puck
} // titania

#endif
