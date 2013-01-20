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

#include "jsMFNode.h"

namespace titania {
namespace X3D {

template <>
JSClass jsX3DArrayField <jsSFNode, MFNode <X3DBaseNode>>::static_class = {
	"MFNode", JSCLASS_HAS_PRIVATE | JSCLASS_NEW_ENUMERATE,
	JS_PropertyStub, JS_PropertyStub, get1Value, set1Value,
	(JSEnumerateOp) enumerate, JS_ResolveStub, JS_ConvertStub, finalize,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

template <>
JSBool
jsX3DArrayField <jsSFNode, MFNode <X3DBaseNode>>::construct (JSContext* context, uintN argc, jsval* vp)
{
	if (argc == 0)
	{
		return create (context, new MFNode <X3DBaseNode> (), &JS_RVAL (context, vp));
	}
	else
	{
		MFNode <X3DBaseNode>* field = new MFNode <X3DBaseNode> ();

		jsval* argv = JS_ARGV (context, vp);

		for (uintN i = 0; i < argc; ++ i)
		{
			JSObject* value;

			if (not JS_ValueToObject (context, argv [i], &value))
				return JS_FALSE;

			if (not JS_InstanceOf (context, value, jsSFNode::getClass (), NULL))
			{
				JS_ReportError (context, "Type of argument %d is invalid - should be SFNode, is %s", i, JS_GetClass (context, value) -> name);
				return JS_FALSE;
			}

			field -> emplace_back (*(X3DField <X3DBaseNode*>*)JS_GetPrivate (context, value));
		}

		return create (context, field, &JS_RVAL (context, vp));
	}

	return JS_FALSE;
}

template <>
JSBool
jsX3DArrayField <jsSFNode, MFNode <X3DBaseNode>>::get1Value (JSContext* context, JSObject* obj, jsid id, jsval* vp)
{
	if (not JSID_IS_INT (id))
		return JS_TRUE;

	int32 index = JSID_TO_INT (id);

	if (index < 0)
	{
		JS_ReportError (context, "index out of range");
		return JS_FALSE;
	}

	X3DArray* field = (X3DArray*) JS_GetPrivate (context, obj);

	return jsSFNode::create (context, new SFNode <X3DBaseNode> (*(X3DField <X3DBaseNode*>*) & field -> get1Value (index)), vp);

	return JS_TRUE;
}

template <>
JSBool
jsX3DArrayField <jsSFNode, MFNode <X3DBaseNode>>::set1Value (JSContext* context, JSObject* obj, jsid id, JSBool strict, jsval* vp)
{
	if (not JSID_IS_INT (id))
		return JS_TRUE;

	int32 index = JSID_TO_INT (id);

	if (index < 0)
	{
		JS_ReportError (context, "index out of range");
		return JS_FALSE;
	}

	JSObject* value;

	if (not JS_ValueToObject (context, *vp, &value))
		return JS_FALSE;

	if (not JS_InstanceOf (context, value, jsSFNode::getClass (), NULL))
	{
		JS_ReportError (context, "Type of argument is invalid - should be SFNode, is %s", JS_GetClass (context, value) -> name);
		return JS_FALSE;
	}

	X3DArray* field = (X3DArray*) JS_GetPrivate (context, obj);

	field -> set1Value (index, *(X3DField <X3DBaseNode*>*)JS_GetPrivate (context, value));

	*vp = JSVAL_VOID;

	return JS_TRUE;
}

template class jsX3DArrayField <jsSFNode, MFNode <X3DBaseNode>>;

} // X3D
} // titania
