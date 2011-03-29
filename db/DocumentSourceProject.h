/**
 * Copyright 2011 (c) 10gen Inc.
 *
 * This program is free software: you can redistribute it and/or  modify
 * it under the terms of the GNU Affero General Public License, version 3,
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "pch.h"
#include "DocumentSource.h"

namespace mongo
{
    class Expression;
    class Value;
    class ValueIterator;

    class DocumentSourceProject :
        public DocumentSource,
	public boost::enable_shared_from_this<DocumentSourceProject>
    {
    public:
	// virtuals from DocumentSource
	virtual ~DocumentSourceProject();
	virtual bool eof();
	virtual bool advance();
	virtual shared_ptr<Document> getCurrent();


	/*
	  Create a new DocumentSource that can implement projection.
	*/
	static shared_ptr<DocumentSourceProject> create(
	    shared_ptr<DocumentSource> pSource);

	/*
	  Add an Expression to the projection.

	  BSON document fields are ordered, so the new field will be 
	  appended to the existing set.

	  @param fieldName the name of the field as it will appear
	  @param pExpression the expression used to compute the field
	  @param ravelArray if the result of the expression is an array value,
	      the projection will create one Document per value in the array;
	      a sequence of documents is generated, with each one containing one
	      value from the array.  Note there can only be one raveled field.
	*/
	void addField(string fieldName, shared_ptr<Expression> pExpression,
	    bool ravelArray);

    private:
	DocumentSourceProject(shared_ptr<DocumentSource> pSource);

	// configuration state
	shared_ptr<DocumentSource> pSource; // underlying source
	vector<string> vFieldName; // inclusion field names
	vector<shared_ptr<Expression>> vpExpression; // inclusions
	int ravelWhich; // index of raveled field, if any, otherwise -1

	// iteration state
	shared_ptr<Document> pNoRavelDocument; // document to return, pre-ravel
	shared_ptr<const Value> pRavelArray; // field being raveled
	shared_ptr<ValueIterator> pRavel; // iterator used for raveling
	shared_ptr<const Value> pRavelValue; // current value
    };
}