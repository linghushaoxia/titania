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

#ifndef __TITANIA_X3D_H__
#define __TITANIA_X3D_H__

// Include Browser at first.
#include "X3D/Browser/Browser.h"
#include "X3D/Browser/BrowserApplication.h"

#include "X3D/Bits/Cast.h"
#include "X3D/Bits/Error.h"
#include "X3D/Bits/Traverse.h"
#include "X3D/Components.h"
#include "X3D/Prototype/ExternProto.h"
#include "X3D/Prototype/Proto.h"

#include "X3D/Execution/BindableNodeList.h"
#include "X3D/Execution/BindableNodeStack.h"
#include "X3D/Execution/ExportedNode.h"
#include "X3D/Execution/ImportedNode.h"
#include "X3D/Execution/NamedNode.h"

#include "X3D/Browser/Notification.h"
#include "X3D/Browser/PointingDeviceSensor/Selection.h"
#include "X3D/Browser/Properties/BrowserOptions.h"
#include "X3D/Browser/Properties/BrowserProperties.h"
#include "X3D/Browser/Properties/MotionBlur.h"
#include "X3D/Browser/Properties/RenderingProperties.h"
#include "X3D/InputOutput/Loader.h"
#include "X3D/Miscellaneous/GoldenGate.h"
#include "X3D/Miscellaneous/Keys.h"
#include "X3D/Miscellaneous/MediaStream.h"
#include "X3D/Parser/Filter.h"
#include "X3D/Parser/RegEx.h"

#include "X3D/Rendering/Context.h"

namespace titania {
namespace X3D {

const BrowserApplicationPtr &
getBrowser (/* parameter */)
throw (Error <BROWSER_UNAVAILABLE>);

BrowserPtr
createBrowser ()
throw (Error <BROWSER_UNAVAILABLE>);

BrowserPtr
createBrowser (const BrowserPtr &)
throw (Error <INVALID_NODE>,
       Error <BROWSER_UNAVAILABLE>);

void
removeBrowser (BrowserPtr &)
noexcept (true);

} // X3D
} // titania

#endif
