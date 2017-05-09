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

#ifndef __TITANIA_X3D_BROWSER_X3DBROWSER_SURFACE_H__
#define __TITANIA_X3D_BROWSER_X3DBROWSER_SURFACE_H__

#include <Titania/OpenGL/Surface.h>

#include "../Browser/X3DBrowser.h"

namespace titania {
namespace X3D {

class BackgroundTexture;
class KeyDevice;
class PointingDevice;
class X3DViewer;
class X3DSelector;

class Browser :
	public X3DBrowser,
	public opengl::Surface
{
public:

	///  @name Construction

	Browser (const MFString & url, const MFString & parameter);

	Browser (const Browser & other, const MFString & url, const MFString & parameter);

	Browser*
	create (X3DExecutionContext* const executionContext) const;

	virtual
	void
	setup () final override
	{ X3DBrowser::setup (); }

	///  @name Member access

	void
	setCursor (const std::string & value)
	{ cursor = value; }

	const SFString &
	getCursor () const
	{ return cursor; }

	const X3DPtr <X3DViewer> &
	getViewer () const
	{ return viewer; }

	///  @name Operations

	void
	setAntialiasing (const int32_t samples)
	noexcept (true);

	virtual
	bool
	makeCurrent ()
	noexcept (true) final override;

	virtual
	void
	renderBackground () final override;

	virtual
	void
	swapBuffers ()
	noexcept (true) final override
	{ opengl::Surface::swapBuffers (); }

	///  @name Destruction

	virtual
	void
	dispose () final override;
	
	virtual
	~Browser () final override;


protected:

	///  @name Construction

	virtual
	void
	initialize () override;

	///  @name Event handlers

	virtual
	void
	on_style_updated () override;


private:

	///  @name Event handler

	virtual
	void
	on_map () override;

	virtual
	void
	on_unmap () override;

	void
	set_cursor (const String &);

	void
	set_viewer ();

	///  @name Operations

	virtual
	void
	reshape (const Vector4i & viewport)
	noexcept (true) final override;
	
	virtual
	void
	update ()
	noexcept (true) final override;

	///  @name Members

	X3DPtr <X3DViewer>                  viewer;
	X3DPtr <KeyDevice>                  keyDevice;
	X3DPtr <PointingDevice>             pointingDevice;
	SFString                            cursor;
	std::unique_ptr <BackgroundTexture> background;

};

} // X3D
} // titania

#endif
