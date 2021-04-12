// BEGIN: Copyright 
// Copyright (C) 2021 by Pedro Mendes, Rector and Visitors of the 
// University of Virginia, University of Heidelberg, and University 
// of Connecticut School of Medicine. 
// All rights reserved 
// END: Copyright 

// BEGIN: License 
// Licensed under the Apache License, Version 2.0 (the "License"); 
// you may not use this file except in compliance with the License. 
// You may obtain a copy of the License at 
//   http://www.apache.org/licenses/LICENSE-2.0 
// END: License 

#include "cpsapi/core/cpsapiPointer.h"

#include <algorithm>

CPSAPI_NAMESPACE_USE

// static 
cpsapiPointer::Map cpsapiPointer::Manager;

// static 
void cpsapiPointer::deleted(const CDataObject * pObject)
{
  if (pObject == nullptr)
    return;

  Map::iterator found = Manager.find(const_cast< CDataObject * >(pObject));
  
  if (found == Manager.end())
    return;
  
  Map::iterator foundNull = Manager.insert(std::make_pair(nullptr, std::set< cpsapiPointer * >())).first;
  
  std::for_each(found->second.begin(), found->second.end(), [foundNull](cpsapiPointer * pPointer)
  {
    pPointer->mpObject = foundNull;
  });

  foundNull->second.insert(found->second.begin(), found->second.end());

  Manager.erase(found);
}

cpsapiPointer::cpsapiPointer(CDataObject * pObject)
  : mpObject()
{
  mpObject = Manager.insert(std::make_pair(pObject, std::set< cpsapiPointer * >())).first;
  mpObject->second.insert(this);
}

cpsapiPointer::cpsapiPointer(const cpsapiPointer & src)
  : mpObject(src.mpObject)
{
  mpObject->second.insert(this);
}

cpsapiPointer::~cpsapiPointer()
{
  mpObject->second.erase(this);
  
  if (mpObject->second.empty())
    Manager.erase(mpObject);
}

cpsapiPointer & cpsapiPointer::operator= (const cpsapiPointer & rhs)
{
  if (this == &rhs) return *this;

  mpObject->second.erase(this);
  
  if (mpObject->second.empty())
    Manager.erase(mpObject);

  mpObject = rhs.mpObject;
  mpObject->second.insert(this);

  return *this;
}

cpsapiPointer & cpsapiPointer::operator= (CDataObject * pObject)
{
  if (mpObject->first == pObject)
    return *this;

  mpObject->second.erase(this);
  
  if (mpObject->second.empty())
    Manager.erase(mpObject);

  mpObject = Manager.insert(std::make_pair(pObject, std::set< cpsapiPointer * >())).first;
  mpObject->second.insert(this);

  return *this;
}

CDataObject * cpsapiPointer::operator->() const
{
  return mpObject->first;
}

CDataObject * cpsapiPointer::operator*() const
{
  return mpObject->first;
}

cpsapiPointer::operator bool() const
{
  return mpObject->first != nullptr;
}