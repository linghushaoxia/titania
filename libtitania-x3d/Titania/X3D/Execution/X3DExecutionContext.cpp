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

#include "X3DExecutionContext.h"

#include "../Bits/Cast.h"
#include "../Browser/X3DBrowser.h"
#include "../Components/Core/X3DPrototypeInstance.h"
#include "../Components/Navigation/X3DViewpointNode.h"
#include "../Components/Networking/Inline.h"
#include "../Prototype/ExternProto.h"
#include "../Prototype/Proto.h"

#include <Titania/Utility/Adapter.h>

#include <algorithm>
#include <iostream>

namespace titania {
namespace X3D {

X3DExecutionContext::X3DExecutionContext () :
	             X3DNode (),              
	            worldURL (),              
	            encoding ("X3D"),         
	specificationVersion ("3.0"),         
	   characterEncoding ("utf8"),        
	             comment ("Titania 0.1"), 
	          components (),              
	             profile (NULL),          
	          namedNodes (),              
	       importedNodes (),
	       importedNames (),              
	              protos (),              
	        externProtos (),              
	              routes (),              
	           rootNodes (),
	               nodes ()               
{
	setChildren (rootNodes);
}

void
X3DExecutionContext::setup ()
{
	X3DNode::setup ();

	// Add rootNodes here as child. This prevents X3DProtoypeInstances from being disposed on construction.
}

void
X3DExecutionContext::assign1 (const X3DExecutionContext* const executionContext)
{
	setEncoding             (executionContext -> getEncoding ());
	setSpecificationVersion (executionContext -> getSpecificationVersion ());
	setCharacterEncoding    (executionContext -> getCharacterEncoding ());
	setComment              (executionContext -> getComment ());

	setWorldURL (executionContext -> getWorldURL ());

	addComponents (components);
	setProfile (profile);

	for (const auto & externProto : executionContext -> getExternProtoDeclarations ())
		updateExternProtoDeclaration (externProto .getNodeName (), externProto);

	for (const auto & proto : executionContext -> getProtoDeclarations ())
		updateProtoDeclaration (proto .getNodeName (), proto);

	for (const auto & rootNode : executionContext -> getRootNodes ())
	{
		try
		{
			getRootNodes () .emplace_back (rootNode -> clone (this));
		}
		catch (const Error <INVALID_NAME> &)
		{
			getRootNodes () .emplace_back (rootNode -> copy (this));
		}
	}
}

void
X3DExecutionContext::assign2 (const X3DExecutionContext* const executionContext)
{
	setupNodes ();

	for (const auto & importedNode : executionContext -> getImportedNodes ())
		importedNode -> clone (this);

	for (const auto & route : executionContext -> getRoutes ())
		route -> clone (this);
}

void
X3DExecutionContext::addComponents (const ComponentInfoArray & value)
{
	for (const auto & component : value)
		components .push_back (component -> getName (), component);
}

SFNode <X3DBaseNode>
X3DExecutionContext::createNode (const std::string & name)
throw (Error <INVALID_NAME>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	const X3DBaseNode* declaration = getBrowser () -> getNode (name);

	if (getProfile () or getComponents () .size ())
	{
		if (getProfile ())
		{
			try
			{
				profile -> getComponents () .rfind (declaration -> getComponentName ());

				return declaration -> create (this);
			}
			catch (const std::out_of_range &)
			{ }
		}

		try
		{
			components .rfind (declaration -> getComponentName ());

			return declaration -> create (this);
		}
		catch (const std::out_of_range &)
		{ }

		throw Error <INVALID_NAME> ("Node type '" + name + "' not supported by profile or component specification.");
	}

	return declaration -> create (this);
}

SFNode <X3DPrototypeInstance>
X3DExecutionContext::createProtoInstance (const std::string & name)
throw (Error <INVALID_NAME>,
       Error <INVALID_X3D>,
       Error <INVALID_FIELD>,
       Error <INVALID_ACCESS_TYPE>,
       Error <URL_UNAVAILABLE>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	return getX3DProtoDeclaration (name) -> createInstance (this);
}

X3DProto*
X3DExecutionContext::getX3DProtoDeclaration (const std::string & name)
throw (Error <INVALID_NAME>,
       Error <INVALID_X3D>,
       Error <URL_UNAVAILABLE>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	try
	{
		return protos .rfind (name) .getValue ();
	}
	catch (const std::out_of_range &)
	{
		try
		{
			return externProtos .rfind (name) .getValue ();
		}
		catch (const std::out_of_range &)
		{
			if (not isScene ())
				return getExecutionContext () -> getX3DProtoDeclaration (name);

			throw Error <INVALID_NAME> ("Unknown proto or externproto type '" + name + "'.");
		}
	}
}

void
X3DExecutionContext::setupNodes ()
{
	for (auto & node : nodes)
		node -> setup ();

	nodes .clear ();
}

// Named node handling

const SFNode <X3DBaseNode> &
X3DExecutionContext::getNode (const std::string & name) const
throw (Error <INVALID_NAME>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	try
	{
		return getNamedNode (name);
	}
	catch (const Error <INVALID_NAME> &)
	{
		try
		{
			return getImportedNode (name);
		}
		catch (const Error <INVALID_NAME> &)
		{
			throw Error <INVALID_NAME> ("Unknown named node '" + name + "'.");
		}
	}
}

void
X3DExecutionContext::addNamedNode (const std::string & name, const SFNode <X3DBaseNode> & node)
throw (Error <NODE_IN_USE>,
       Error <IMPORTED_NODE>,
       Error <INVALID_NODE>,
       Error <INVALID_NAME>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	if (namedNodes .find (name) == namedNodes .end ())
		updateNamedNode (name, node);
		
	else
		throw Error <NODE_IN_USE> ("Couldn't add named node: Node named '" + name + "' already exists.");
}

void
X3DExecutionContext::updateNamedNode (const std::string & name, const SFNode <X3DBaseNode> & node)
throw (Error <IMPORTED_NODE>,
       Error <INVALID_NODE>,
       Error <INVALID_NAME>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	if (not node)
		throw Error <INVALID_NODE> ("Couldn't update named node: node IS NULL.");

	if (node -> getExecutionContext () not_eq this)
		throw Error <IMPORTED_NODE> ("Couldn't update named node: the node does not belong to this execution context.");

	if (name .length ())
	{
		namedNodes .erase (node -> getName ());

		namedNodes [name] = node;
		namedNodes [name] .isTainted (true);
		namedNodes [name] .addParent (this);

		node -> setName (name);
	}
	else
		throw Error <INVALID_NAME> ("Couldn't update named node: node name is empty.");
}

void
X3DExecutionContext::removeNamedNode (const std::string & name)
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	const auto namedNode = namedNodes .find (name);

	if (namedNode not_eq namedNodes .end ())
	{
		namedNode -> second -> setName ("");

		namedNodes .erase (namedNode);
	}
}

const SFNode <X3DBaseNode> &
X3DExecutionContext::getNamedNode (const std::string & name) const
throw (Error <INVALID_NAME>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	const auto namedNode = namedNodes .find (name);

	if (namedNode not_eq namedNodes .end ())
		return namedNode -> second;

	throw Error <INVALID_NAME> ("Named node '" + name + "' not found.");
}

// Imported nodes handling

const SFNode <ImportedNode> &
X3DExecutionContext::addImportedNode (const SFNode <Inline> & inlineNode, const std::string & exportedName, std::string importedName)
throw (Error <INVALID_NODE>,
       Error <INVALID_NAME>,
       Error <NODE_IN_USE>,
       Error <URL_UNAVAILABLE>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	if (not inlineNode)
		throw Error <INVALID_NODE> ("Couldn't add imported node: inline node is NULL.");

	if (exportedName .empty ())
		throw Error <INVALID_NAME> ("Couldn't add imported node: exported node name is empty.");
		
	if (importedName .empty ())
		importedName = exportedName;

	try
	{
		importedNodes .rfind (importedName);
		throw Error <NODE_IN_USE> ("Couldn't add imported node: imported name '" + importedName + "' already exists.");
	}
	catch (const std::out_of_range &)
	{
		inlineNode -> setup ();

		importedNodes .push_back (importedName, new ImportedNode (this, inlineNode, exportedName, importedName));
		importedNodes .back () .isTainted (true);
		importedNodes .back () .addParent (this);

		importedNames [inlineNode -> getExportedNode (exportedName)] = importedName;

		return importedNodes .back ();
	}
}

void
X3DExecutionContext::removeImportedNode (const std::string & importedName)
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	importedNodes .erase (importedName);
}

void
X3DExecutionContext::updateImportedNode (const SFNode <Inline> & inlineNode, const std::string & exportedName, std::string importedName)
throw (Error <INVALID_NODE>,
       Error <INVALID_NAME>,
       Error <URL_UNAVAILABLE>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	if (not inlineNode)
		throw Error <INVALID_NODE> ("Couldn't update imported node: inline node is NULL.");

	if (exportedName .empty ())
		throw Error <INVALID_NAME> ("Couldn't update imported node: exported node name is empty.");
		
	try
	{
		inlineNode -> setup ();

		importedNodes .erase (importedNames .at (inlineNode -> getExportedNode (exportedName)));
	}
	catch (const std::out_of_range &)
	{ }

	importedNodes .erase (importedName);

	addImportedNode (inlineNode, exportedName, importedName);
}

const SFNode <X3DBaseNode> &
X3DExecutionContext::getImportedNode (const std::string & importedName) const
throw (Error <INVALID_NAME>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	try
	{
		return importedNodes .rfind (importedName) -> getExportedNode ();
	}
	catch (const std::out_of_range &)
	{
		throw Error <INVALID_NAME> ("Imported node '" + importedName + "' not found.");
	}
}

const std::string &
X3DExecutionContext::getLocalName (const SFNode <X3DBaseNode> & node) const
throw (Error <INVALID_NODE>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	auto importedName = importedNames .find (node);
	
	if (importedName not_eq importedNames .end ())
		return importedName -> second;
		
	if (node -> getExecutionContext () == this)
		return node -> getName ();
		
	throw Error <INVALID_NODE> ("Couldn' get local name.");
}

//	Proto declaration handling

const SFNode <Proto> &
X3DExecutionContext::addProtoDeclaration (const std::string & name, const FieldDefinitionArray & interfaceDeclarations)
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	protos .push_back (name, createProtoDeclaration (name, interfaceDeclarations));
	protos .back () .isTainted (true);
	protos .back () .addParent (this);

	return protos .back ();
}

void
X3DExecutionContext::updateProtoDeclaration (const std::string & name, const SFNode <Proto> & protoDeclaration)
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	protos .erase (protoDeclaration -> getName ());
	protos .push_back (name, protoDeclaration);
	protos .back () .isTainted (true);
	protos .back () .addParent (this);

	protoDeclaration -> setName (name);
}

const SFNode <Proto> &
X3DExecutionContext::getProtoDeclaration (const std::string & name)
throw (Error <INVALID_NAME>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	try
	{
		return protos .rfind (name);
	}
	catch (const std::out_of_range &)
	{
		throw Error <INVALID_NAME> ("PROTO '" + name + "' not found.");
	}
}

SFNode <Proto>
X3DExecutionContext::createProtoDeclaration (const std::string & name, const FieldDefinitionArray & interfaceDeclarations)
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	SFNode <Proto> proto = new Proto (this);

	proto -> setName (name);

	for (const auto & field : interfaceDeclarations)
	{
		proto -> addUserDefinedField (field -> getAccessType (),
		                              field -> getName (),
		                              field);
	}

	proto -> setup ();

	return proto;
}

//	externprotoDeclarationHandling

const SFNode <ExternProto> &
X3DExecutionContext::addExternProtoDeclaration (const std::string & name, const FieldDefinitionArray & externInterfaceDeclarations, const MFString & URLList)
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	externProtos .push_back (name, createExternProtoDeclaration (name, externInterfaceDeclarations, URLList));
	externProtos .back () .isTainted (true);
	externProtos .back () .addParent (this);

	return externProtos .back ();
}

void
X3DExecutionContext::updateExternProtoDeclaration (const std::string & name, const SFNode <ExternProto> & externProtoDeclaration)
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	externProtos .erase (externProtoDeclaration -> getName ());
	externProtos .push_back (name, externProtoDeclaration);
	externProtos .back () .isTainted (true);
	externProtos .back () .addParent (this);

	externProtoDeclaration -> setName (name);
}

const
SFNode <ExternProto> &
X3DExecutionContext::getExternProtoDeclaration (const std::string & name)
throw (Error <INVALID_NAME>,
       Error <URL_UNAVAILABLE>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	try
	{
		return externProtos .rfind (name);
	}
	catch (const std::out_of_range &)
	{
		throw Error <INVALID_NAME> ("EXTERNPROTO '" + name + "' not found.");
	}
}

SFNode <ExternProto>
X3DExecutionContext::createExternProtoDeclaration (const std::string & name, const FieldDefinitionArray & externInterfaceDeclarations, const MFString & URLList)
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	SFNode <ExternProto> externProto = new ExternProto (this);

	externProto -> setName (name);

	for (const auto & field : externInterfaceDeclarations)
	{
		externProto -> addUserDefinedField (field -> getAccessType (),
		                                    field -> getName (),
		                                    field);
	}

	externProto -> url () = URLList;

	externProto -> setup ();

	return externProto;
}

//	Dynamic route handling

const SFNode <Route> &
X3DExecutionContext::addRoute (const SFNode <X3DBaseNode> & sourceNode,      const std::string & sourceFieldId,
                               const SFNode <X3DBaseNode> & destinationNode, const std::string & destinationFieldId)
throw (Error <INVALID_NODE>,
       Error <INVALID_FIELD>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	auto fields = getRouteId (sourceNode, sourceFieldId, destinationNode, destinationFieldId);

	try
	{
		// Silently return if route already exists.

		return routes .rfind (fields);
	}
	catch (const std::out_of_range &)
	{
		// Add route.

		routes .push_back (fields, new Route (this, sourceNode, fields .first, destinationNode, fields .second));
		routes .back () .isTainted (true);
		routes .back () .addParent (this);

		return routes .back ();
	}
}

void
X3DExecutionContext::deleteRoute (const SFNode <X3DBaseNode> & sourceNode,      const std::string & sourceFieldId,
                                  const SFNode <X3DBaseNode> & destinationNode, const std::string & destinationFieldId)
throw (Error <INVALID_NODE>,
       Error <INVALID_FIELD>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	try
	{
		auto routeId = getRouteId (sourceNode, sourceFieldId, destinationNode, destinationFieldId);

		routes .rfind (routeId) -> disconnect ();
		routes .erase (routeId);
	}
	catch (const std::out_of_range &)
	{
		// Silently return if route not exists.
	}
}

void
X3DExecutionContext::deleteRoute (Route* route)
throw (Error <INVALID_NODE>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	if (not route)
		throw Error <INVALID_NODE> ("Bad ROUTE specification: route is NULL in deleteRoute.");		

	route -> disconnect ();
	routes .erase (route -> getId ());
}

RouteId
X3DExecutionContext::getRouteId (const SFNode <X3DBaseNode> & sourceNode,      const std::string & sourceFieldId,
                                 const SFNode <X3DBaseNode> & destinationNode, const std::string & destinationFieldId)
throw (Error <INVALID_NODE>,
       Error <INVALID_FIELD>)
{
	if (not sourceNode .getValue ())
		throw Error <INVALID_NODE> ("Bad ROUTE specification: source node is NULL.");

	if (not destinationNode .getValue ())
		throw Error <INVALID_NODE> ("Bad ROUTE specification: destination node is NULL.");

	X3DFieldDefinition* sourceField = NULL;
	
	try
	{
		sourceField = sourceNode .getField (sourceFieldId);
	}
	catch (const Error <INVALID_NAME> &)
	{
		throw Error <INVALID_FIELD> ("Bad ROUTE specification: Unknown eventOut '" + sourceFieldId + "' in node '" + sourceNode .getNodeName () + "'.");
	}

	X3DFieldDefinition* destinationField = NULL;
	
	try
	{
		destinationField = destinationNode .getField (destinationFieldId);
	}
	catch (const Error <INVALID_NAME> &)
	{
		throw Error <INVALID_FIELD> ("Bad ROUTE specification: Unknown eventIn '" + destinationFieldId + "' in node '" + destinationNode .getNodeName () + "'.");
	}

	return std::make_pair (sourceField, destinationField);
}

void
X3DExecutionContext::changeViewpoint (const std::string & name)
throw (Error <INVALID_NAME>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	try
	{
		auto viewpoint = x3d_cast <X3DViewpointNode*> (getNamedNode (name));

		if (viewpoint)
			viewpoint -> set_bind () = true;

		else
			throw Error <INVALID_NAME> ("Warning: Node named '" + name + "' is not a viewpoint node.");
	}
	catch (const Error <INVALID_NAME> & error)
	{
		if (not isScene ())
			getExecutionContext () -> changeViewpoint (name);
		else
			throw error;
	}
}

void
X3DExecutionContext::toStream (std::ostream & ostream) const
{
	Generator::PushContext ();

	for (const auto & externProto : getExternProtoDeclarations ())
	{
		ostream
			<< externProto
			<< Generator::TidyBreak
			<< Generator::TidyBreak;
	}

	for (const auto & proto : getProtoDeclarations ())
	{
		ostream
			<< proto
			<< Generator::TidyBreak
			<< Generator::TidyBreak;
	}

	if (getRootNodes () .size ())
	{
		for (const auto & child : basic::adapter (getRootNodes () .cbegin (), getRootNodes () .cend () - 1))
		{
			ostream
				<< Generator::Indent
				<< child
				<< Generator::TidyBreak
				<< Generator::TidyBreak;
		}

		ostream
			<< Generator::Indent
			<< getRootNodes () .back ()
			<< Generator::TidyBreak;
	}

	if (getImportedNodes () .size ())
		ostream << Generator::TidyBreak;

	for (const auto & importedNode : getImportedNodes ())
		ostream << importedNode;

	if (getRoutes () .size ())
		ostream << Generator::TidyBreak;

	for (const auto & route : getRoutes ())
	{
		ostream
			<< route
			<< Generator::Break;
	}

	if (getInnerComments () .size ())
	{
		ostream << Generator::TidyBreak;

		for (const auto & comment : getInnerComments ())
		{
			ostream
				<< Generator::Indent
				<< Generator::Comment
				<< comment
				<< Generator::Break;
		}
	}

	Generator::PopContext ();
}

void
X3DExecutionContext::clear ()
{
	profile = NULL;
	components .clear ();

	namedNodes    .clear ();
	importedNodes .clear ();
	importedNames .clear ();
	protos        .clear ();
	externProtos  .clear ();
	routes        .clear ();

	rootNodes .clear ();
}

void
X3DExecutionContext::dispose ()
{
	profile = NULL;
	components .clear ();

	namedNodes    .clear ();
	importedNodes .clear ();
	importedNames .clear ();
	protos        .clear ();
	externProtos  .clear ();
	routes        .clear ();

	rootNodes .dispose ();

	X3DNode::dispose ();
}

X3DExecutionContext::~X3DExecutionContext ()
{ }

} // X3D
} // titania
