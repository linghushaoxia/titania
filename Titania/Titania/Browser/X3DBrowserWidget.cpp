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

#include "X3DBrowserWidget.h"

#include "../Browser/IconFactory.h"
#include "../Browser/RecentView.h"
#include "../Browser/UserData.h"
#include "../Browser/X3DBrowserWindow.h"
#include "../BrowserNotebook/NotebookPage/NotebookPage.h"
#include "../Configuration/config.h"
#include "../Dialogs/MessageDialog/MessageDialog.h"
#include "../Editors/HistoryEditor/History.h"

#include <Titania/X3D/Bits/Traverse.h>
#include <Titania/X3D/Browser/RenderingProperties.h>
#include <Titania/X3D/Editing/X3DEditor.h>
#include <Titania/X3D/InputOutput/FileGenerator.h>

#include <Titania/OS/cwd.h>
#include <Titania/String.h>
#include <Titania/Stream/GZStream.h>
#include <fstream>

// DEBUG rooted objects
//#include <Titania/X3D/Debug.h>

namespace titania {
namespace puck {

X3DBrowserWidget::X3DBrowserWidget (const X3D::BrowserPtr & defaultBrowser) :
	X3DBrowserWindowInterface (),
	                    pages (),
	                     page (),
	              recentPages (),
	            masterBrowser (defaultBrowser),
	                  browser (X3D::createBrowser (defaultBrowser)),
	                    scene (browser -> getExecutionContext ()),
	         executionContext (browser -> getExecutionContext ()),
	           worldURLOutput (),
	              iconFactory (new IconFactory (getBrowserWindow ())),
	               recentView (new RecentView (getBrowserWindow ())),
	                  history (new History ())
{
	// Allways add fields, otherwise there is a Xlib error in destruction.

	addChildObjects (masterBrowser,
	                 browser,
	                 scene,
	                 executionContext);
}

void
X3DBrowserWidget::initialize ()
{
	X3DBrowserWindowInterface::initialize ();

	// Master browser

	masterBrowser -> setAntialiasing (4);
	masterBrowser -> show ();
	getLogoBox () .pack_start (*getMasterBrowser (), true, true, 0);

	// History

	if (not getConfig () -> hasItem ("rememberHistory"))
		getConfig () -> setItem ("rememberHistory", -1);

	getHistory () -> setSize (getConfig () -> getInteger ("rememberHistory"));

	recentView -> initialize ();

	// Collect already open pages.

	std::set <basic::uri> urlIndex;

	for (const auto & page : pages)
		urlIndex .emplace (page -> getWorldURL ());

	// Restore pages.

	const auto empty     = pages .empty ();
	auto       worldURLs = std::vector <std::string> ();
	auto       histories = std::vector <std::string> ();
	auto       recent    = std::vector <std::string> ();

	basic::split (std::back_inserter (worldURLs), getConfig () -> getString ("worldURL"), "\n");
	basic::split (std::back_inserter (histories), getConfig () -> getString ("history"),  "\n");
	basic::split (std::back_inserter (recent), getConfig () -> getString ("recent"),  "\n");

	if (worldURLs .empty () and empty)
		worldURLs .emplace_back (get_page ("about/home.x3dv"));

	for (size_t i = 0; i < worldURLs .size (); ++ i)
	{
		if (urlIndex .count (worldURLs [i]))
			continue;

		const auto page = append (worldURLs [i]);

		if (i < histories .size ())
			page -> getBrowserHistory () .fromString (histories [i]);
	}

	if (empty)
	{
		const size_t pageNumber = getConfig () -> getInteger ("currentPage");

		if (pageNumber < pages .size ())
			getBrowserNotebook () .set_current_page (pageNumber);
	}
	else
		getBrowserNotebook () .set_current_page (0);

	for (const auto & URL : recent)
	{
		try
		{
			recentPages .emplace_back (getPage (URL));
		}
		catch (const std::out_of_range &)
		{ }
	}

	// 

	getCurrentScene ()  .addInterest (&X3DBrowserWidget::set_history, this);
	worldURL_changed () .addInterest (&X3DBrowserWidget::set_history, this);

	getBrowserNotebook () .set_visible (true);
}

void
X3DBrowserWidget::configure ()
{
	X3DBrowserWindowInterface::configure ();

	// Restore Menu Configuration from Config


	// Meta data

	if (not getConfig () -> hasItem ("addStandardMetaData"))
		getConfig () -> setItem ("addStandardMetaData", true);


	// ToolBar

	if (not getConfig () -> hasItem ("menubar"))
		getConfig () -> setItem ("menubar", true);

	if (not getConfig () -> hasItem ("menubarFullscreen"))
		getConfig () -> setItem ("menubarFullscreen", false);
	

	// ToolBar

	if (not getConfig () -> hasItem ("toolbar"))
		getConfig () -> setItem ("toolbar", true);

	if (not getConfig () -> hasItem ("toolbarFullscreen"))
		getConfig () -> setItem ("toolbarFullscreen", false);

	
	// SideBar

	if (not getConfig () -> hasItem ("sidebar"))
		getConfig () -> setItem ("sidebar", true);

	if (not getConfig () -> hasItem ("sidebarFullscreen"))
		getConfig () -> setItem ("sidebarFullscreen", false);


	// Footer

	if (not getConfig () -> hasItem ("footer"))
		getConfig () -> setItem ("footer", true);

	if (not getConfig () -> hasItem ("footerFullscreen"))
		getConfig () -> setItem ("footerFullscreen", false);


	// TAbs

	if (not getConfig () -> hasItem ("tabs"))
		getConfig () -> setItem ("tabs", true);

	if (not getConfig () -> hasItem ("tabsFullscreen"))
		getConfig () -> setItem ("tabsFullscreen", false);

	
	// RenderingProperties

	if (getConfig () -> hasItem ("renderingProperties"))
		getRenderingPropertiesAction () -> set_active (getConfig () -> getBoolean ("renderingProperties"));

	
	// Rubberband

	if (getConfig () -> hasItem ("rubberBand"))
		getRubberbandAction () -> set_active (getConfig () -> getBoolean ("rubberBand"));

	if (not getConfig () -> hasItem ("isLive"))
		getConfig () -> setItem ("isLive", true);

	
	// isLive

	isLive (isLive ());


	// VPaned

	if (getConfig () -> hasItem ("vPaned"))
		getVPaned () .set_position (getConfig () -> getInteger ("vPaned"));

	
	// HPaned

	if (getConfig () -> hasItem ("hPaned"))
		getHPaned () .set_position (getConfig () -> getInteger ("hPaned"));

	getStraightenHorizonButton () .set_active (getConfig () -> getBoolean ("straightenHorizon"));
}

void
X3DBrowserWidget::set_fullscreen (const bool value)
{
	if (value)
	{
		getMenubarAction () -> set_active (getConfig () -> getBoolean ("menubarFullscreen"));
		getToolbarAction () -> set_active (getConfig () -> getBoolean ("toolbarFullscreen"));
		getSidebarAction () -> set_active (getConfig () -> getBoolean ("sidebarFullscreen"));
		getFooterAction ()  -> set_active (getConfig () -> getBoolean ("footerFullscreen"));
		getTabsAction ()    -> set_active (getConfig () -> getBoolean ("tabsFullscreen"));
	}
	else
	{
		getMenubarAction () -> set_active (getConfig () -> getBoolean ("menubar"));
		getToolbarAction () -> set_active (getConfig () -> getBoolean ("toolbar"));
		getSidebarAction () -> set_active (getConfig () -> getBoolean ("sidebar"));
		getFooterAction ()  -> set_active (getConfig () -> getBoolean ("footer"));
		getTabsAction ()    -> set_active (getConfig () -> getBoolean ("tabs"));
	}
}

void
X3DBrowserWidget::store ()
{
	getConfig () -> setItem ("vPaned",            getVPaned () .get_position ());
	getConfig () -> setItem ("hPaned",            getHPaned () .get_position ());
	getConfig () -> setItem ("straightenHorizon", getStraightenHorizonButton () .get_active ());

	X3DBrowserWindowInterface::store ();
}

/***
 *  Return an iterator to a page widget determined by @a URL.
 */
NotebookPagePtr
X3DBrowserWidget::getPage (const basic::uri & URL) const
throw (std::out_of_range)
{
	const auto iter = std::find_if (pages .begin (), pages .end (), [&] (const NotebookPagePtr & page)
	{
		return page -> getMasterSceneURL () == URL;
	});

	if (iter == pages .end ())
		throw std::out_of_range ("X3DBrowserWidget::getPage");

	return *iter;
}

void
X3DBrowserWidget::setPage (const NotebookPagePtr & value)
{
	browser -> initialized () .removeInterest (&X3DBrowserWidget::set_executionContext, this);
	browser -> getUrlError () .removeInterest (&X3DBrowserWidget::set_urlError, this);

	page             = value;
	browser          = page -> getMainBrowser ();
	scene            = browser -> getExecutionContext () -> getMasterScene ();
	executionContext = browser -> getExecutionContext ();

	setBrowser (browser);
	
	browser -> initialized () .addInterest (&X3DBrowserWidget::set_executionContext, this);
	browser -> getUrlError () .addInterest (&X3DBrowserWidget::set_urlError, this);

	browser -> setStraightenHorizon (getStraightenHorizonButton () .get_active ());

	isLive (isLive ());
}

void
X3DBrowserWidget::setCurrentContext (const X3D::X3DExecutionContextPtr & value)
{
	try
	{
		getCurrentBrowser () -> replaceWorld (value);
	}
	catch (const X3D::X3DError &)
	{ }
}

void
X3DBrowserWidget::isLive (const bool value)
{
	getConfig () -> setItem ("isLive", value);

	getPlayPauseButton () .set_stock_id (Gtk::StockID (value ? "gtk-media-pause" : "gtk-media-play"));

	if (value)
		getCurrentBrowser () -> beginUpdate ();
	else
		getCurrentBrowser () -> endUpdate ();
}

bool
X3DBrowserWidget::isLive () const
{
	return getConfig () -> getBoolean ("isLive");
}

void
X3DBrowserWidget::openRecent ()
{
	recentView -> loadPreview (getCurrentBrowser ());
	recentView -> open ();
}

void
X3DBrowserWidget::open (const basic::uri & URL_)
{
	auto URL = URL_;

	if (URL .is_relative ())
		URL = basic::uri (os::cwd ()) .transform (URL);

	try
	{
		getBrowserNotebook () .set_current_page (getPage (URL) -> getPageNumber ());
	}
	catch (const std::out_of_range &)
	{
		append (URL);
		getBrowserNotebook () .set_current_page (pages .size () - 1);
	}
}

NotebookPagePtr
X3DBrowserWidget::append (const basic::uri & URL)
{
	const auto page    = std::make_shared <NotebookPage> (getBrowserWindow (), URL);
	const auto browser = page -> getMainBrowser ();

	pages .emplace_back (page);

	const auto text = URL .empty () ? _ ("New Scene") : URL .basename ();

	page -> getTabImage () .set (Gtk::StockID (URL .filename () .str ()), Gtk::IconSize (Gtk::ICON_SIZE_MENU));
	page -> getTabLabel () .set_text (text);
	page -> getTabLabel () .set_tooltip_text (URL .filename () .str ());
	page -> getTabCloseButton () .signal_clicked () .connect (sigc::bind (sigc::mem_fun (this, &X3DBrowserWidget::close), page));

	getBrowserNotebook () .append_page (page -> getWidget (), page -> getTabWidget ());
	getBrowserNotebook () .set_tab_reorderable (page -> getWidget (), true);
	getBrowserNotebook () .set_menu_label_text (page -> getWidget (), text);
	getBrowserNotebook () .set_show_tabs (getShowTabs ());

	return page;
}

bool
X3DBrowserWidget::getShowTabs () const
{
	const bool showTabs = isFullscreen () ? getConfig () -> getBoolean ("tabsFullscreen") : getConfig () -> getBoolean ("tabs");

	return pages .size () > 1 and showTabs;
}

void
X3DBrowserWidget::load (const basic::uri & URL)
{
	if (URL .empty ())
		return;

	recentView -> loadPreview (getCurrentBrowser ());
	getCurrentBrowser () -> loadURL ({ URL .str () }, { });
}

bool
X3DBrowserWidget::save (const basic::uri & worldURL, const std::string & outputStyle, const bool copy)
{
	return save (getCurrentScene (), worldURL, outputStyle, copy);
}

bool
X3DBrowserWidget::save (const X3D::X3DScenePtr & scene, const basic::uri & worldURL, const std::string & outputStyle, const bool copy)
{
	try
	{
		// Known file type must be also added to BrowserWindow::on_save_activated.

		const auto undoStep = std::make_shared <X3D::UndoStep> ("Save A Copy Undo Step");

		recentView -> loadPreview (scene -> getBrowser ());

		if (getConfig () -> getBoolean ("addStandardMetaData"))
			scene -> addStandardMetaData ();

		setOutputStyle (scene, outputStyle);
		X3D::X3DEditor::setWorldURL (scene, worldURL, undoStep);

		X3D::FileGenerator::write (scene, worldURL, outputStyle);

		if (copy)
			undoStep -> undo ();

		worldURL_changed () .processInterests ();

		return true;
	}
	catch (const X3D::Error <X3D::NOT_SUPPORTED> & error)
	{
		const auto dialog = std::dynamic_pointer_cast <MessageDialog> (addDialog ("MessageDialog", false));
	
		dialog -> setType (Gtk::MESSAGE_ERROR);
		dialog -> setMessage (_ ("Couldn't save file!"));
		dialog -> setText (_ ("The given filename does not have any known file extension. Please enter a known file extension like .x3d or .x3dv."));
		dialog -> run ();

		return false;
	}
	catch (const X3D::Error <X3D::INVALID_URL> & error)
	{
		const auto dialog = std::dynamic_pointer_cast <MessageDialog> (addDialog ("MessageDialog", false));
	
		dialog -> setType (Gtk::MESSAGE_ERROR);
		dialog -> setMessage (_ ("Couldn't save file!"));
		dialog -> setText (_ ("Tip: check file and folder permissions."));
		dialog -> run ();
	
		return false;
	}
}

void
X3DBrowserWidget::setOutputStyle (const X3D::X3DScenePtr & scene, const std::string & outputStyle)
{
	scene -> removeMetaData ("outputStyle"); // TODO: remove this line.

	if (getConfig () -> getBoolean ("addStandardMetaData"))
	{
		if (outputStyle == "nicest")
			scene -> removeMetaData ("titania-output-style");
		else
			scene -> setMetaData ("titania-output-style", outputStyle);
	}
}

std::string
X3DBrowserWidget::getOutputStyle (const X3D::X3DScenePtr & scene) const
{
	try
	{
		return basic::tolower (scene -> getMetaData ("outputStyle"), std::locale::classic ());
	}
	catch (const X3D::Error <X3D::INVALID_NAME> &)
	{ }

	try
	{
		return basic::tolower (scene -> getMetaData ("titania-output-style"), std::locale::classic ());
	}
	catch (const X3D::Error <X3D::INVALID_NAME> &)
	{
		return "nicest";
	}
}

void
X3DBrowserWidget::reload ()
{
	load (getCurrentScene () -> getWorldURL ());
}

void
X3DBrowserWidget::close (const NotebookPagePtr page)
{
	const auto browser = page -> getMainBrowser ();

	// Open recent browser if browser is the currentBrowser.

	if (browser == getCurrentBrowser ())
	{
		recentView -> loadPreview (browser);

		if (not recentPages .empty ())
		{
			const auto iter = std::find (pages .cbegin (), pages .cend (), recentPages .back ());
		
			if (iter not_eq pages .cend ())
				getBrowserNotebook () .set_current_page (iter - pages .cbegin ());
		}
	}

	// Remove browser completely.

	page -> shutdown ();

	pages .erase (std::remove (pages .begin (), pages .end (), page), pages .end ());
	recentPages .erase (std::remove (recentPages .begin (), recentPages .end (), page), recentPages .end ());

	if (pages .empty ())
		openRecent ();

	getBrowserNotebook () .remove_page (page -> getWidget ());
	getBrowserNotebook () .set_show_tabs (getShowTabs ());

	// DEBUG rooted objects
	//const auto b = browser .getValue ();
	//const_cast <X3D::BrowserPtr &> (browser_) = nullptr;
	//const_cast <X3D::BrowserPtr &> (browser)  = nullptr;
	//X3D::debug_roots (b);
}

bool
X3DBrowserWidget::quit ()
{
	recentView -> loadPreview (getCurrentBrowser ());

	std::deque <std::string> worldURLs;
	std::deque <std::string> browserHistories;
	std::deque <std::string> recent;

	for (const auto & page : recentPages)
	{
		const auto URL = page -> getMasterSceneURL ();

		if (not URL .empty ())
			recent .emplace_back (URL);
	}

	for (const auto & page : pages)
	{
		const auto URL = page -> getMasterSceneURL ();

		if (not URL .empty ())
			worldURLs .emplace_back (URL);

		browserHistories .emplace_back (page -> getBrowserHistory () .toString ());

		page -> shutdown ();
	}

	auto currentPage = getBrowserNotebook () .get_current_page ();

	// Check if current scene is an empty scene;
	if (pages [currentPage] -> getMasterSceneURL () .empty ())
		currentPage = 0;

	getConfig () -> setItem ("currentPage", currentPage);
	getConfig () -> setItem ("worldURL",    basic::join (worldURLs, "\n"));
	getConfig () -> setItem ("history",     basic::join (browserHistories, "\n"));
	getConfig () -> setItem ("recent",      basic::join (recent, "\n"));

	X3DBrowserWindowInterface::quit ();
	return false;
}

void
X3DBrowserWidget::on_switch_page (Gtk::Widget*, guint pageNumber)
{
	recentView -> loadPreview (getCurrentBrowser ());

	const auto page = pages [pageNumber];

	setPage (page);

	recentPages .erase (std::remove (recentPages .begin (), recentPages .end (), page), recentPages .end ());
	recentPages .emplace_back (page);
}

void
X3DBrowserWidget::on_page_reordered (Gtk::Widget* widget, guint pageNumber)
{
	const auto iter = std::find_if (pages .begin (), pages .end (), [&] (const NotebookPagePtr & page) { return &page -> getWidget () == widget; });

	if (iter == pages .end ())
		return;

	auto page = std::move (*iter);

	pages .erase (iter);
	pages .emplace (pages .begin () + pageNumber, std::move (page));
}

void
X3DBrowserWidget::set_executionContext ()
{
	if (getCurrentBrowser () -> getExecutionContext () == executionContext)
	   return;

	if (getCurrentBrowser () -> getExecutionContext () -> isType ({ X3D::X3DConstants::X3DScene }))
		scene = getCurrentBrowser () -> getExecutionContext ();
	else
		scene = getCurrentBrowser () -> getExecutionContext () -> getScene ();

	executionContext = getCurrentBrowser () -> getExecutionContext ();
}

void
X3DBrowserWidget::set_history ()
{
	const auto title    = getCurrentScene () -> getTitle ();
	const auto worldURL = getCurrentScene () -> getWorldURL ();

	if (worldURL .empty ())
		return;

	try
	{
		if (getCurrentScene () -> getMetaData ("titania-history") == "FALSE")
			return;
	}
	catch (const X3D::X3DError &)
	{ }

	// Update history.

	getHistory () -> setItem (title, worldURL, getIconFactory () -> getIcon (worldURL, Gtk::IconSize (Gtk::ICON_SIZE_MENU)));
}

void
X3DBrowserWidget::set_urlError ()
{
	try
	{
		std::ostringstream error;

		for (const auto & string : getCurrentBrowser () -> getUrlError ())
			error << string .str ();

		std::ostringstream osstream;

		osstream
			<< get_page ("about/url_error.x3dv")
			<< "?type=" << basic::to_string (21, std::locale::classic ())
			<< ";what=" << Glib::uri_escape_string (error .str ());

		const auto scene = getCurrentBrowser () -> createX3DFromURL ({ osstream .str () });

		scene -> setWorldURL (getCurrentBrowser () -> getWorldURL ());

		getCurrentBrowser () -> replaceWorld (scene);
	}
	catch (const X3D::X3DError &)
	{
		try
		{
			browser -> replaceWorld (nullptr);
		}
		catch (const X3D::X3DError &)
		{ }
	}
}

void
X3DBrowserWidget::setTransparent (const bool value)
{
	if (value)
	{
		Glib::RefPtr <Gdk::Visual> visual = getWindow () .get_screen () -> get_rgba_visual ();

		if (visual)
			gtk_widget_set_visual (GTK_WIDGET (getWindow () .gobj ()), visual -> gobj ());
	}
}

void
X3DBrowserWidget::dispose ()
{
	pages       .clear ();
	recentPages .clear ();
	iconFactory .reset ();
	recentView  .reset ();
	history     .reset ();

	X3DBrowserWindowInterface::dispose ();
}

X3DBrowserWidget::~X3DBrowserWidget ()
{
	__LOG__ << std::endl;
}

} // puck
} // titania
