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

#include "InlineEditor.h"

#include "../../Browser/BrowserSelection.h"
#include "../../Configuration/config.h"

namespace titania {
namespace puck {

InlineEditor::InlineEditor (BrowserWindow* const browserWindow) :
	        X3DBaseInterface (browserWindow, browserWindow -> getBrowser ()),
	X3DInlineEditorInterface (get_ui ("Dialogs/InlineEditor.xml"), gconf_dir ()),
	                    load (browserWindow, getLoadCheckButton (),  "load"),
	                bboxSize (getBrowserWindow (),
	                          getBBoxSizeXAdjustment (),
	                          getBBoxSizeYAdjustment (),
	                          getBBoxSizeZAdjustment (),
	                          getBBoxSizeBox (),
	                          "bboxSize"),
	              bboxCenter (getBrowserWindow (),
	                          getBBoxCenterXAdjustment (),
	                          getBBoxCenterYAdjustment (),
	                          getBBoxCenterZAdjustment (),
	                          getBBoxCenterBox (),
	                          "bboxCenter"),
	              inlineNode ()
{
	setup ();
}

void
InlineEditor::initialize ()
{
	X3DInlineEditorInterface::initialize ();

	getBrowser () -> getSelection () -> getChildren () .addInterest (this, &InlineEditor::set_selection);

	set_selection ();
}

void
InlineEditor::set_selection ()
{
	getConvertMasterSelectionButton () .set_sensitive (not getBrowser () -> getSelection () -> getChildren () .empty ());

	inlineNode = getBrowser () -> getSelection () -> getChildren () .empty ()
	             ? nullptr
					 : getBrowser () -> getSelection () -> getChildren () .back ();

	const auto nodes = inlineNode ? X3D::MFNode ({ inlineNode }) : X3D::MFNode ();

	getInlineBox () .set_sensitive (inlineNode);

	load       .setNodes (nodes);
	bboxSize   .setNodes (nodes);
	bboxCenter .setNodes (nodes);
}

void
InlineEditor::on_convert_master_selection_clicked ()
{
	__LOG__ << std::endl;
}

void
InlineEditor::on_update_bounding_box_fields_activate ()
{
	__LOG__ << std::endl;
}

void
InlineEditor::on_fold_back_into_scene_clicked ()
{
	__LOG__ << std::endl;

	const auto undoStep = std::make_shared <UndoStep> (_ ("Fold Inline Back Into Scene"));
	const auto nodes    = getBrowserWindow () -> importScene (inlineNode -> getInternalScene (), undoStep);

	if (nodes .empty ())
		return;

	const auto group = getBrowserWindow () -> groupNodes ("Group", nodes, undoStep);
	const auto name  = inlineNode -> getName ();

	getBrowserWindow () -> replaceNodes (X3D::SFNode (inlineNode), group, undoStep);

	if (not name .empty ())
	{
		undoStep -> addUndoFunction (&X3D::X3DExecutionContext::removeNamedNode, getExecutionContext (), name);
		undoStep -> addRedoFunction (&X3D::X3DExecutionContext::updateNamedNode, getExecutionContext (), name, group);
		getExecutionContext () -> updateNamedNode (name, group);
	}

	getBrowserWindow () -> getSelection () -> setChildren ({ group }, undoStep);
	getBrowserWindow () -> addUndoStep (undoStep);

	getBrowserWindow () -> expandNodes (X3D::MFNode ({ group }));
}

InlineEditor::~InlineEditor ()
{
	dispose ();
}

} // puck
} // titania
