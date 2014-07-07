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

#include "GeometryPropertiesEditor.h"

#include "../Configuration/config.h"

namespace titania {
namespace puck {

GeometryPropertiesEditor::GeometryPropertiesEditor (BrowserWindow* const browserWindow) :
	                    X3DBaseInterface (browserWindow, browserWindow -> getBrowser ()),
	X3DGeometryPropertiesEditorInterface (get_ui ("Dialogs/GeometryPropertiesEditor.xml"), gconf_dir ()),
	                       geometryNodes (),
	          textureCoordinateGenerator (),
	                            undoStep (),
	                        currentField (),
	                            changing (false)
{ }

void
GeometryPropertiesEditor::initialize ()
{
	X3DGeometryPropertiesEditorInterface::initialize ();

	getBrowser () -> getSelection () -> getChildren () .addInterest (this, &GeometryPropertiesEditor::set_selection);

	set_selection ();
}

void
GeometryPropertiesEditor::set_selection ()
{
	for (const auto & geometry : geometryNodes)
	{
		try
		{
			geometry -> getField <X3D::SFFloat> ("solid") .removeInterest (this, &GeometryPropertiesEditor::set_solid);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	for (const auto & geometry : geometryNodes)
	{
		try
		{
			geometry -> getField <X3D::SFFloat> ("ccw") .removeInterest (this, &GeometryPropertiesEditor::set_ccw);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	for (const auto & geometry : geometryNodes)
	{
		try
		{
			geometry -> getField <X3D::SFFloat> ("convex") .removeInterest (this, &GeometryPropertiesEditor::set_convex);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	for (const auto & geometry : geometryNodes)
	{
		try
		{
			geometry -> getField <X3D::SFFloat> ("creaseAngle") .removeInterest (this, &GeometryPropertiesEditor::set_creaseAngle);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	for (const auto & geometry : geometryNodes)
	{
		try
		{
			geometry -> getField <X3D::SFFloat> ("texCoord") .removeInterest (this, &GeometryPropertiesEditor::set_textureCoordinateGenerator);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	undoStep .reset ();

	// Find X3DGeometryNodes

	auto selection = getBrowser () -> getSelection () -> getChildren ();

	geometryNodes .clear ();

	X3D::traverse (selection, [&] (X3D::SFNode & node)
	               {
	                  for (const auto & type: node -> getType ())
	                  {
	                     switch (type)
	                     {
									case X3D::X3DConstants::X3DGeometryNode:
										{
										   geometryNodes .emplace_back (node);
										   return true;
										}
									default:
										break;
								}
							}

	                  return true;
						});

	for (const auto & geometry : geometryNodes)
	{
		try
		{
			geometry -> getField <X3D::SFFloat> ("solid") .addInterest (this, &GeometryPropertiesEditor::set_solid);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	for (const auto & geometry : geometryNodes)
	{
		try
		{
			geometry -> getField <X3D::SFFloat> ("ccw") .addInterest (this, &GeometryPropertiesEditor::set_ccw);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	for (const auto & geometry : geometryNodes)
	{
		try
		{
			geometry -> getField <X3D::SFFloat> ("convex") .addInterest (this, &GeometryPropertiesEditor::set_convex);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	for (const auto & geometry : geometryNodes)
	{
		try
		{
			geometry -> getField <X3D::SFFloat> ("creaseAngle") .addInterest (this, &GeometryPropertiesEditor::set_creaseAngle);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	for (const auto & geometry : geometryNodes)
	{
		try
		{
			geometry -> getField <X3D::SFFloat> ("texCoord") .addInterest (this, &GeometryPropertiesEditor::set_textureCoordinateGenerator);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	set_solid ();
	set_ccw ();
	set_convex ();
	set_creaseAngle ();
	set_textureCoordinateGenerator ();
}

void
GeometryPropertiesEditor::on_solid_toggled ()
{
	if (changing)
		return;

	addUndoFunction <X3D::SFBool> ("solid");

	getSolidCheckButton () .set_inconsistent (false);

	for (const auto & geometry : geometryNodes)
	{
		try
		{
			auto & field = geometry -> getField <X3D::SFBool> ("solid");

			field = getSolidCheckButton () .get_active ();

			field .removeInterest (this, &GeometryPropertiesEditor::set_solid);
			field .addInterest (this, &GeometryPropertiesEditor::connectSolid);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	addRedoFunction <X3D::SFBool> ("solid");
}

void
GeometryPropertiesEditor::set_solid ()
{
	changing = true;

	getSolidCheckButton () .set_active (false);

	// Find first �solid� field.

	bool hasField = false;
	int  active   = -1;

	for (const auto & geometry : geometryNodes)
	{
		try
		{
			auto & field = geometry -> getField <X3D::SFBool> ("solid");

			if (active < 0)
			{
				hasField = true;
				active   = field;
			}
			else if (field .getValue () not_eq active)
			{
				active = -1;
				break;
			}
		}
		catch (const X3D::X3DError &)
		{ }
	}

	getSolidCheckButton () .set_sensitive (hasField);
	getSolidCheckButton () .set_active (active > 0);
	getSolidCheckButton () .set_inconsistent (active < 0);

	changing = false;
}

void
GeometryPropertiesEditor::connectSolid (const X3D::SFBool & field)
{
	field .removeInterest (this, &GeometryPropertiesEditor::connectSolid);
	field .addInterest (this, &GeometryPropertiesEditor::set_solid);
}

void
GeometryPropertiesEditor::on_ccw_toggled ()
{
	if (changing)
		return;

	addUndoFunction <X3D::SFBool> ("ccw");

	getCCWCheckButton () .set_inconsistent (false);

	for (const auto & geometry : geometryNodes)
	{
		try
		{
			auto & field = geometry -> getField <X3D::SFBool> ("ccw");

			field = getCCWCheckButton () .get_active ();

			field .removeInterest (this, &GeometryPropertiesEditor::set_ccw);
			field .addInterest (this, &GeometryPropertiesEditor::connectCCW);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	addRedoFunction <X3D::SFBool> ("ccw");
}

void
GeometryPropertiesEditor::set_ccw ()
{
	changing = true;

	getCCWCheckButton () .set_active (false);

	// Find first �ccw� field.

	bool hasField = false;
	int  active   = -1;

	for (const auto & geometry : geometryNodes)
	{
		try
		{
			auto & field = geometry -> getField <X3D::SFBool> ("ccw");

			if (active < 0)
			{
				hasField = true;
				active   = field;
			}
			else if (field .getValue () not_eq active)
			{
				active = -1;
				break;
			}
		}
		catch (const X3D::X3DError &)
		{ }
	}

	getCCWCheckButton () .set_sensitive (hasField);
	getCCWCheckButton () .set_active (active > 0);
	getCCWCheckButton () .set_inconsistent (active < 0);

	changing = false;
}

void
GeometryPropertiesEditor::connectCCW (const X3D::SFBool & field)
{
	field .removeInterest (this, &GeometryPropertiesEditor::connectCCW);
	field .addInterest (this, &GeometryPropertiesEditor::set_ccw);
}

void
GeometryPropertiesEditor::on_convex_toggled ()
{
	if (changing)
		return;

	addUndoFunction <X3D::SFBool> ("convex");

	getConvexCheckButton () .set_inconsistent (false);

	for (const auto & geometry : geometryNodes)
	{
		try
		{
			auto & field = geometry -> getField <X3D::SFBool> ("convex");

			field = not getConvexCheckButton () .get_active ();

			field .removeInterest (this, &GeometryPropertiesEditor::set_convex);
			field .addInterest (this, &GeometryPropertiesEditor::connectConvex);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	addRedoFunction <X3D::SFBool> ("convex");
}

void
GeometryPropertiesEditor::set_convex ()
{
	changing = true;

	getConvexCheckButton () .set_active (false);

	// Find first �convex� field.

	bool hasField = false;
	int  active   = -1;

	for (const auto & geometry : geometryNodes)
	{
		try
		{
			auto & field = geometry -> getField <X3D::SFBool> ("convex");

			if (active < 0)
			{
				hasField = true;
				active   = field;
			}
			else if (field .getValue () not_eq active)
			{
				active = -1;
				break;
			}
		}
		catch (const X3D::X3DError &)
		{ }
	}

	getConvexCheckButton () .set_sensitive (hasField);
	getConvexCheckButton () .set_active (not (active > 0));
	getConvexCheckButton () .set_inconsistent (active < 0);

	changing = false;
}

void
GeometryPropertiesEditor::connectConvex (const X3D::SFBool & field)
{
	field .removeInterest (this, &GeometryPropertiesEditor::connectConvex);
	field .addInterest (this, &GeometryPropertiesEditor::set_convex);
}

void
GeometryPropertiesEditor::on_creaseAngle_changed ()
{
	if (changing)
		return;

	addUndoFunction <X3D::SFFloat> ("creaseAngle");

	for (const auto & geometry : geometryNodes)
	{
		try
		{
			auto & field = geometry -> getField <X3D::SFFloat> ("creaseAngle");

			field = math::radians (getCreaseAngleScale () .get_value ());

			field .removeInterest (this, &GeometryPropertiesEditor::set_creaseAngle);
			field .addInterest (this, &GeometryPropertiesEditor::connectCreaseAngle);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	addRedoFunction <X3D::SFFloat> ("creaseAngle");
}

void
GeometryPropertiesEditor::set_creaseAngle ()
{
	changing = true;

	getCreaseAngleScale () .set_value (0);

	// Find first �creaseAngle� field.

	bool hasField = false;

	for (const auto & geometry : geometryNodes)
	{
		try
		{
			getCreaseAngleScale () .set_value (math::degrees <float> (geometry -> getField <X3D::SFFloat> ("creaseAngle")));
			hasField = true;
			break;
		}
		catch (const X3D::X3DError &)
		{ }
	}

	getCreaseAngleBox () .set_sensitive (hasField);

	changing = false;
}

void
GeometryPropertiesEditor::connectCreaseAngle (const X3D::SFFloat & field)
{
	field .removeInterest (this, &GeometryPropertiesEditor::connectCreaseAngle);
	field .addInterest (this, &GeometryPropertiesEditor::set_creaseAngle);
}

void
GeometryPropertiesEditor::on_textureCoordinateGenerator_toggled ()
{
	if (changing)
		return;

	addUndoFunction <X3D::SFNode> ("texCoord");

	getTextureCoordinateGeneratorCheckButton () .set_inconsistent (false);
	getTextureCoordinateGeneratorModeButton ()  .set_sensitive (getTextureCoordinateGeneratorCheckButton () .get_active ());

	for (const auto & geometry : geometryNodes)
	{
		try
		{
			auto & field = geometry -> getField <X3D::SFNode> ("texCoord");

			if (getTextureCoordinateGeneratorCheckButton () .get_active ())
				field = textureCoordinateGenerator;
			else
				field = nullptr;

			field .removeInterest (this, &GeometryPropertiesEditor::set_textureCoordinateGenerator);
			field .addInterest (this, &GeometryPropertiesEditor::connectTextureCoordinateGenerator);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	addRedoFunction <X3D::SFNode> ("texCoord");
}

void
GeometryPropertiesEditor::set_textureCoordinateGenerator ()
{
	changing = true;

	getTextureCoordinateGeneratorCheckButton () .set_active (false);

	// Find first �texCoord� field.
	
	textureCoordinateGenerator = nullptr;

	bool hasField = false;
	int  active   = -1;

	for (const auto & geometry : basic::reverse_adapter (geometryNodes))
	{
		try
		{
			auto & field = geometry -> getField <X3D::SFNode> ("texCoord");

			if (not textureCoordinateGenerator and field and field -> getType () .back () == X3D::X3DConstants::TextureCoordinateGenerator)
				textureCoordinateGenerator = field;

			if (active < 0)
			{
				hasField = true;
				active   = bool (textureCoordinateGenerator);
			}
			else if (field not_eq textureCoordinateGenerator)
			{
				active = -1;
				break;
			}
		}
		catch (const X3D::X3DError &)
		{ }
	}

	if (not textureCoordinateGenerator)
	{
		textureCoordinateGenerator = new X3D::TextureCoordinateGenerator (getExecutionContext ());
		textureCoordinateGenerator -> setup ();
	}

	getTextureCoordinateGeneratorBox ()         .set_sensitive (hasField);
	getTextureCoordinateGeneratorCheckButton () .set_active (active > 0);
	getTextureCoordinateGeneratorCheckButton () .set_inconsistent (active < 0);
	getTextureCoordinateGeneratorModeButton ()  .set_sensitive (active > 0);

	getModeLabel () .set_text (textureCoordinateGenerator -> mode ());

	changing = false;
}

void
GeometryPropertiesEditor::connectTextureCoordinateGenerator (const X3D::SFNode & field)
{
	field .removeInterest (this, &GeometryPropertiesEditor::connectTextureCoordinateGenerator);
	field .addInterest (this, &GeometryPropertiesEditor::set_textureCoordinateGenerator);
}

void
GeometryPropertiesEditor::setTextureCoordinateGeneratorFields ()
{
}

GeometryPropertiesEditor::~GeometryPropertiesEditor ()
{ }

} // puck
} // titania
