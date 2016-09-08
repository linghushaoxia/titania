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

#ifndef __TITANIA_BROWSER_BROWSER_SELECTION_H__
#define __TITANIA_BROWSER_BROWSER_SELECTION_H__

#include "../Base/X3DBaseInterface.h"

#include <Titania/X3D/Browser/Selection.h>

namespace titania {
namespace puck {

class BrowserSelection :
	public X3DBaseInterface
{
public:

	///  @name Construction

	BrowserSelection (X3DBrowserWindow* const);

	///  @name Member access

	void
	isEnabled (const bool);

	bool
	isEnabled () const
	{ return enabled; }

	void
	setMode (const X3D::Selection::SelectionType);

	const X3D::Selection::SelectionType
	getMode () const
	{ return mode; }

	void
	setSelectLowest (const bool);

	const bool
	getSelectLowest () const
	{ return selectLowest; }

	void
	setSelectGeometry (const bool);

	const bool
	getSelectGeometry () const
	{ return selectGeometry; }

	const X3D::SFBool &
	isOver () const
	{ return over; }

	const X3D::SFBool &
	isActive () const
	{ return active; }

	const X3D::SFBool &
	getPickedTime () const
	{ return touchTime; }

	bool
	isSelected (const X3D::SFNode & node) const
	{ return browser -> getSelection () -> isSelected (node); }

	void
	setChildren (const X3D::MFNode & children)
	{ browser -> getSelection () -> setChildren (children); }

	const X3D::MFNode &
	getChildren () const
	{ return children; }

	///  @name Operations

	void
	addChildren (const X3D::MFNode &, const X3D::UndoStepPtr &) const;

	void
	removeChildren (const X3D::MFNode &, const X3D::UndoStepPtr &) const;

	void
	setChildren (const X3D::MFNode &, const X3D::UndoStepPtr &) const;

	void
	clear (const X3D::UndoStepPtr &) const;

	void
	undoRestoreSelection (const X3D::UndoStepPtr & undoStep) const;

	void
	redoRestoreSelection (const X3D::UndoStepPtr & undoStep) const;

	///  @name Destruction

	virtual
	~BrowserSelection ();


private:

	///  @name Event handlers

	void
	set_browser ();

	///  @name Members

	bool                          enabled;
	X3D::Selection::SelectionType mode;
	bool                          selectLowest;
	bool                          selectGeometry;

	X3D::SFBool     over;
	X3D::SFBool     active;
	X3D::SFBool     touchTime;
	X3D::MFNode     children;
	X3D::BrowserPtr browser;

};

} // puck
} // titania

#endif
