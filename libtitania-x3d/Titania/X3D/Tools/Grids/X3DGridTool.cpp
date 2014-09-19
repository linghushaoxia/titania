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

#include "X3DGridTool.h"

#include "../../Browser/X3DBrowser.h"

namespace titania {
namespace X3D {

X3DGridTool::X3DGridTool () :
	    X3DNode (),
	 inlineNode (new Inline (getBrowser () -> getEmptyScene ())),
	activeLayer ()
{
	addChildren (inlineNode, activeLayer);
}

void
X3DGridTool::initialize ()
{
	X3DNode::initialize ();

	inlineNode -> setup ();

	getBrowser () -> getActiveLayer () .addInterest (this, &X3DGridTool::set_activeLayer);

	set_activeLayer ();
}

void
X3DGridTool::setExecutionContext (X3DExecutionContext* const value)
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)

{
	__LOG__ << std::endl;

	getBrowser () -> getActiveLayer () .removeInterest (this, &X3DGridTool::set_activeLayer);

	inlineNode -> setExecutionContext (value -> getBrowser () -> getEmptyScene ());

	X3DNode::setExecutionContext (value);

	getBrowser () -> getActiveLayer () .addInterest (this, &X3DGridTool::set_activeLayer);

	set_activeLayer ();
}

void
X3DGridTool::set_activeLayer ()
{
	__LOG__ << std::endl;

	if (activeLayer)
		activeLayer -> getFriends () .remove (inlineNode .getValue ());

	activeLayer = getBrowser () -> getActiveLayer ();

	if (activeLayer)
		activeLayer -> getFriends () .emplace_back (inlineNode);
}

void
X3DGridTool::dispose ()
{
	__LOG__ << std::endl;

	if (activeLayer)
		activeLayer -> getFriends () .remove (inlineNode .getValue ());

	X3DNode::dispose ();
}

} // X3D
} // titania