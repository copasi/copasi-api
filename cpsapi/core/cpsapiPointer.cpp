// BEGIN: Copyright 
// Copyright (C) 2021 by Pedro Mendes, Rector and Visitors of the 
// University of Virginia, University of Heidelberg, and University 
// of Connecticut School of Medicine. 
// All rights reserved 
// END: Copyright 

// BEGIN: License 
// Licensed under the Artistic License 2.0 (the "License"); 
// you may not use this file except in compliance with the License. 
// You may obtain a copy of the License at 
//   https://opensource.org/licenses/Artistic-2.0 
// END: License 

#include "cpsapi/core/cpsapiPointer.h"

#include <algorithm>

CPSAPI_NAMESPACE_USE

// static 
cpsapiPointer::Map cpsapiPointer::Manager;

// static 
cpsapiPointer::Map::iterator cpsapiPointer::findOrInsert(const CDataObject * pObject)
{
  Map::iterator found = Manager.insert(std::make_pair(pObject, nullptr)).first;
  
  if (found->second == nullptr)
    found->second = new References;

  return found;
}

// static 
void cpsapiPointer::deleted(const CDataObject * pObject)
{
  if (pObject == nullptr)
    return;

  Map::iterator found = Manager.find(const_cast< CDataObject * >(pObject));
  
  if (found == Manager.end())
    return;
  
  Map::iterator foundNull = findOrInsert(pObject);

  std::for_each(found->second->begin(), found->second->end(), [foundNull](cpsapiPointer * pPointer) {
    pPointer->mpObject = nullptr;
    pPointer->mpReferences = foundNull->second;
  });

  foundNull->second->insert(found->second->begin(), found->second->end());

  Manager.erase(found);
}

cpsapiPointer::cpsapiPointer(CDataObject * pObject)
  : mpObject(pObject)
  , mpReferences(nullptr)
{
  addToReferences();
}

cpsapiPointer::cpsapiPointer(const cpsapiPointer & src)
  : mpObject(src.mpObject)
  , mpReferences(nullptr)
{
  addToReferences();
}

cpsapiPointer::~cpsapiPointer()
{
  removeFromReferences();
}

cpsapiPointer & cpsapiPointer::operator= (const cpsapiPointer & rhs)
{
  if (this == &rhs) return *this;

  removeFromReferences();

  mpObject = rhs.mpObject;

  addToReferences();

  return *this;
}

cpsapiPointer & cpsapiPointer::operator= (CDataObject * pObject)
{
  if (mpObject == pObject)
    return *this;

  removeFromReferences();

  mpObject = pObject;

  addToReferences();

  return *this;
}

CDataObject * cpsapiPointer::operator->() const
{
  return mpObject;
}

CDataObject * cpsapiPointer::operator*() const
{
  return mpObject;
}

cpsapiPointer::operator bool() const
{
  return mpObject != nullptr;
}

void cpsapiPointer::addToReferences()
{
  if (mpObject != nullptr)
    {
      mpReferences = findOrInsert(mpObject)->second;
      mpReferences->insert(this);
    }
}

void cpsapiPointer::removeFromReferences()
{
  if (mpReferences != nullptr)
    {
      mpReferences->erase(this);

      if (mpReferences->empty())
        Manager.erase(mpObject);

      mpReferences = nullptr;
    }
}

