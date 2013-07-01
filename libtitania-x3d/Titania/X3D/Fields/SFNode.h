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

#ifndef __TITANIA_X3D_FIELDS_SFNODE_H__
#define __TITANIA_X3D_FIELDS_SFNODE_H__

#include "../Basic/X3DBaseNode.h"
#include "../Basic/X3DField.h"

namespace titania {
namespace X3D {

extern template class X3DField <X3DBaseNode*>;

template <class ValueType>
class SFNode :
	public X3DField <ValueType*>
{
public:

	typedef ValueType* value_type;
	typedef ValueType* scalar_type;

	using X3DField <ValueType*>::addInterest;
	using X3DField <ValueType*>::setValue;
	using X3DField <ValueType*>::getValue;

	///  @name Constructors

	SFNode ();

	SFNode (const SFNode &);

	//explicit
	SFNode (ValueType* const);

	///  @name Copy

	virtual
	SFNode*
	clone () const final
	{ return new SFNode (*this); }

	virtual
	SFNode*
	clone (X3DExecutionContext* const) const final;

	virtual
	void
	clone (X3DExecutionContext* const, X3DFieldDefinition*) const final;

	///  @name Field services

	virtual
	X3DConstants::FieldType
	getType () const final
	{ return X3DConstants::SFNode; }

	virtual
	const std::string &
	getTypeName () const final
	{ return typeName; }

	///  @name Node services

	const std::string &
	getNodeTypeName () const;

	const X3DBaseNode*
	getNodeType () const;

	void
	setNodeName (const std::string &) const;

	const std::string &
	getNodeName () const;

	const FieldDefinitionArray &
	getFieldDefinitions () const;

	X3DFieldDefinition*
	getField (const std::string &) const
	throw (Error <INVALID_NAME>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	///  @name Set value services

	virtual
	void
	set (const value_type &) final;

	///  @name Boolean operator

	operator bool () const
	{ return getValue (); }

	///  @name Access operators

	ValueType*
	operator -> () const
	{ return getValue (); }

	ValueType*
	operator * () const
	{ return getValue (); }

	///  @name 6.7.7 Add field interest.

	template <class Class>
	void
	addInterest (Class* object, void (Class::* memberFunction) (const SFNode &)) const
	{ addInterest (object, memberFunction, *this); }

	template <class Class>
	void
	addInterest (Class & object, void (Class::* memberFunction) (const SFNode &)) const
	{ addInterest (object, memberFunction, *this); }

	void
	addInterest (void (* requester) (const SFNode &)) const
	{ addInterest (requester, *this); }

	///  @name Input operator.
	virtual
	void
	fromStream (std::istream &)
	throw (Error <INVALID_X3D>,
	       Error <NOT_SUPPORTED>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>) final;

	///  @name Output operator.
	virtual
	void
	toStream (std::ostream &) const final;

	///  @name Dispose

	virtual
	void
	dispose () final;

	virtual
	~SFNode ();


private:

	using X3DField <ValueType*>::reset;

	void
	addNode (ValueType* const);

	void
	removeNode (ValueType* const);

	///  TypeName identifer for X3DFields.
	static const std::string typeName;

};

template <class ValueType>
const std::string SFNode <ValueType>::typeName ("SFNode");

template <class ValueType>
SFNode <ValueType>::SFNode () :
	X3DField <ValueType*> ()
{ }

template <class ValueType>
SFNode <ValueType>::SFNode (const SFNode & field) :
	X3DField <ValueType*> (field)
{
	if (getValue ())
		getValue () -> addParent (this);
}

template <class ValueType>
SFNode <ValueType>::SFNode (ValueType* const value) :
	X3DField <ValueType*> (value)
{
	if (getValue ())
		getValue () -> addParent (this);
}

template <class ValueType>
SFNode <ValueType>*
SFNode <ValueType>::clone (X3DExecutionContext* const executionContext) const
{
	SFNode* field = new SFNode ();
	
	clone (executionContext, field);
	
	return field;
}

template <class ValueType>
void
SFNode <ValueType>::clone (X3DExecutionContext* const executionContext, X3DFieldDefinition* fieldDefinition) const
{
	SFNode* field = static_cast <SFNode*> (fieldDefinition);

	if (getValue ())
	{
		try
		{
			field -> set (dynamic_cast <ValueType*> (getValue () -> clone (executionContext)));
		}
		catch (const Error <INVALID_NAME> &)
		{
			field -> set (dynamic_cast <ValueType*> (getValue () -> copy (executionContext)));
		}
	}
	else
		field -> set (nullptr);
}

template <class ValueType>
const std::string &
SFNode <ValueType>::getNodeTypeName () const
{
	return getValue () -> getTypeName ();
}

template <class ValueType>
const X3DBaseNode*
SFNode <ValueType>::getNodeType () const
{
	return getValue () -> getType ();
}

template <class ValueType>
void
SFNode <ValueType>::setNodeName (const std::string & name) const
{
	return getValue () -> setName (name);
}

template <class ValueType>
const std::string &
SFNode <ValueType>::getNodeName () const
{
	return getValue () -> getName ();
}

template <class ValueType>
const FieldDefinitionArray &
SFNode <ValueType>::getFieldDefinitions () const
{
	return getValue () -> getFieldDefinitions ();
}

template <class ValueType>
X3DFieldDefinition*
SFNode <ValueType>::getField (const std::string & name) const
throw (Error <INVALID_NAME>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	return getValue () -> getField (name);
}

template <class ValueType>
void
SFNode <ValueType>::set (const value_type & value)
{
	addNode (value);
	X3DField <ValueType*>::set (value);
}

template <class ValueType>
void
SFNode <ValueType>::addNode (ValueType* const value)
{
	if (getValue () not_eq value)
	{
		removeNode (getValue ());

		if (value)
			value -> addParent (this);
	}
}

template <class ValueType>
void
SFNode <ValueType>::removeNode (ValueType* const value)
{
	if (value)
	{
		reset ();

		value -> removeParent (this);
	}
}

template <class ValueType>
inline
void
SFNode <ValueType>::fromStream (std::istream & istream)
throw (Error <INVALID_X3D>,
       Error <NOT_SUPPORTED>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{ }

template <class ValueType>
inline
void
SFNode <ValueType>::toStream (std::ostream & ostream) const
{
	if (getValue ())
		getValue () -> toStream (ostream);
		
	else
		ostream << "NULL";
}

template <class ValueType>
void
SFNode <ValueType>::dispose ()
{
	removeNode (getValue ());

	X3DField <ValueType*>::dispose ();
}

template <class ValueType>
SFNode <ValueType>::~SFNode ()
{
	removeNode (getValue ());
}

template <class Type>
inline
bool
operator < (const SFNode <Type> & lhs, const SFNode <Type> & rhs)
{
	return lhs .getValue () < rhs .getValue ();
}

//
extern template class SFNode <X3DBaseNode>;

} // X3D
} // titania

#endif
