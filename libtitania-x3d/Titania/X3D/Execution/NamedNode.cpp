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

#include "NamedNode.h"

#include "../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

const ComponentType NamedNode::component      = ComponentType::TITANIA;
const std::string   NamedNode::typeName       = "NamedNode";
const std::string   NamedNode::containerField = "namedNode";

NamedNode::NamedNode (X3DExecutionContext* const executionContext, const SFNode & node_) :
	 X3DBaseNode (executionContext -> getBrowser (), executionContext),
	        node (node_),
	        name (node_ -> getName ())
{
	addType (X3DConstants::NamedNode);

	addChildren (node);
}

X3DBaseNode*
NamedNode::create (X3DExecutionContext* const executionContext) const
{
	throw Error <NOT_SUPPORTED> ("Fabricating named nodes is not supported.");
}

NamedNode*
NamedNode::copy (X3DExecutionContext* const executionContext, const CopyType) const
throw (Error <INVALID_NAME>,
	    Error <NOT_SUPPORTED>)
{
	throw Error <NOT_SUPPORTED> ("Copying named nodes into execution context is not supported.");
}

void
NamedNode::initialize ()
{
	X3DBaseNode::initialize ();

	node .addInterest (this, &NamedNode::set_node);

	shutdown () .addInterest (node, &X3DWeakPtr <X3DBaseNode>::dispose);

	set_node ();
}

SFNode
NamedNode::getLocalNode () const
throw (Error <DISPOSED>)
{
	if (node)
		return SFNode (node);

	throw Error <DISPOSED> ("NamedNode::getLocalNode: Node named '" + name + "' is already disposed.");
}

void
NamedNode::set_node ()
{
	if (not node)
		getExecutionContext () -> removeNamedNode (name);
}

void
NamedNode::toStream (std::ostream & ostream) const
{
	ostream << name;
}

void
NamedNode::toXMLStream (std::ostream & ostream) const
{
	ostream << name;
}

} // X3D
} // titania
