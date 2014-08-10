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

#ifndef __TITANIA_OUTLINE_EDITOR_OUTLINE_TREE_H__
#define __TITANIA_OUTLINE_EDITOR_OUTLINE_TREE_H__

#include "OutlineTreeData.h"
#include <deque>
#include <vector>
#include <gtkmm.h>
#include <memory>

namespace titania {
namespace puck {

class OutlineNode
{
public:

	OutlineNode () :
		    data (),
		children ()
	{ }

	void
	set_data (OutlineTreeData* value)
	{ data .reset (value); }

	OutlineTreeData*
	get_data () const
	{ return data .get (); }

	std::deque <OutlineNode> &
	get_children ()
	{ return children; }

	const std::deque <OutlineNode> &
	get_children () const
	{ return children; }

	void
	clear ()
	{ children .clear (); }

	size_t
	size () const
	{ return children .size (); }

	virtual
	~OutlineNode ()
	{ }


protected:

	void
	find (X3D::X3DChildObject* const object, std::vector <const OutlineNode*> & nodes) const
	{
		if (data and data -> is (object))
			nodes .emplace_back (this);

		for (const auto & child : children)
			child .find (object, nodes);
	}

private:

	std::unique_ptr <OutlineTreeData> data;
	std::deque <OutlineNode>          children;

};

class OutlineTree :
	public OutlineNode
{
public:

	using OutlineNode::clear;

	OutlineTree () :
		OutlineNode ()
	{ }

	std::vector <OutlineTreeData*>
	get_parents (const Gtk::TreePath & path) const
	throw (std::out_of_range)
	{
		std::vector <OutlineTreeData*> parents;

		const OutlineNode* node = this;

		for (const size_t index : path)
		{
			if (index < node -> get_children () .size ())
			{
				node = &node -> get_children () [index];
				parents .emplace_back (node -> get_data ());
			}
			else
				throw std::out_of_range ("OutlineTree::getParents: path '" + path .to_string () + "' does not exists.");
		}

		parents .pop_back ();

		return parents;
	}

	OutlineNode &
	get_node (const Gtk::TreePath & path, bool create = false)
	throw (std::out_of_range)
	{
		OutlineNode* node = this;

		for (const auto & index : path)
			node = &get_child (node, index, create);

		return *node;
	}

	const OutlineNode &
	get_node (const Gtk::TreePath & path) const
	throw (std::out_of_range)
	{
		return const_cast <OutlineTree*> (this) -> get_node (path, false);
	}

	std::vector <const OutlineNode*>
	find (X3D::X3DChildObject* const object) const
	{
		std::vector <const OutlineNode*> nodes;

		find (object, nodes);

		return nodes;
	}

	void
	clear (const Gtk::TreePath & path)
	{
		try
		{
			get_node (path) .clear ();
		}
		catch (const std::out_of_range &)
		{ }
	}

private:

	using OutlineNode::find;

	OutlineNode &
	get_child (OutlineNode* parent, size_t index, bool create)
	throw (std::out_of_range)
	{
		if (index < parent -> get_children () .size ())
			return parent -> get_children () [index];

		if (create)
		{
			parent -> get_children () .resize (index + 1);
			return parent -> get_children () [index];
		}

		throw std::out_of_range ("OutlineTree::getChild: path does not exists.");
	}

};

} // puck
} // titania

#endif