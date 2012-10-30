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
 ******************************************************************************/

#ifndef __TITANIA_X3D_JAVA_SCRIPT_H__
#define __TITANIA_X3D_JAVA_SCRIPT_H__

#include "../Browser/X3DFieldDefinition.h"
#include <iostream>
#include <jsapi.h>
#include <string>

namespace titania {
namespace X3D {

class Browser;
class X3DBasicNode;

class JavaScript
{
public:

	JavaScript (X3DBasicNode*, const std::string &);

	std::string
	getVersion ();

	static std::string
	getImplementationVersion ();

	virtual
	void
	initialize ();

	void
	prepareEvents ();

	void
	eventsProcessed ();

	virtual
	void
	shutdown ();

	virtual
	~JavaScript ();


private:

	static void
	error (JSContext* context, const char* message, JSErrorReport* report);

	void
	initContext ();

	static void defineProperty (JSContext *, JSObject *, const std::string &, int8, X3DFieldDefinition *, JSPropertyOp, JSPropertyOp, JSPropertyOp, uintN);

	void
	initNode ();

	void
	evaluate (const std::string &, const std::string & = "<inline>");

	static JSBool getSharedProperty (JSContext *, JSObject *, jsval, jsval*);

	static JSBool setSharedProperty (JSContext *, JSObject *, jsval, jsval*);

	static JSBool setProperty (JSContext *, JSObject *, jsval, jsval*);

	void
	callFunction (const std::string &);

	void
	set_field (X3DFieldDefinition* const, const time_type);

	/* The class of the global object. */
	static JSClass global_class;
	JSRuntime*     runtime;
	JSContext*     context;
	JSObject*      global;
	Browser*       browser;
	X3DBasicNode*  node;

};

} // X3D
} // titania

#endif
