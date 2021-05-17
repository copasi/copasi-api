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
void cpsapiPointer::deleted(const CDataObject * pObject)
{
  if (pObject == nullptr)
    return;

  Map::iterator found = Manager.find(const_cast< CDataObject * >(pObject));
  
  if (found == Manager.end())
    return;
  
  Map::iterator foundNull = Manager.insert(std::make_pair(nullptr, std::set< cpsapiPointer * >())).first;

  std::for_each(found->second.begin(), found->second.end(), [foundNull](cpsapiPointer * pPointer) {
    pPointer->mpObject = foundNull;
  });

  foundNull->second.insert(found->second.begin(), found->second.end());

  Manager.erase(found);
}

cpsapiPointer::cpsapiPointer(CDataObject * pObject)
  : mpObject(Manager.end())
{
  if (pObject != nullptr)
    {
      mpObject = Manager.insert(std::make_pair(pObject, std::set< cpsapiPointer * >())).first;
      mpObject->second.insert(this);
    }
}

cpsapiPointer::cpsapiPointer(const cpsapiPointer & src)
  : mpObject(src.mpObject)
{
  if (mpObject != Manager.end())
    mpObject->second.insert(this);
}

cpsapiPointer::~cpsapiPointer()
{
  if (mpObject != Manager.end())
    {
      mpObject->second.erase(this);

      if (mpObject->second.empty())
        Manager.erase(mpObject);
    }
}

cpsapiPointer & cpsapiPointer::operator= (const cpsapiPointer & rhs)
{
  if (this == &rhs) return *this;

  if (mpObject != Manager.end())
    {
      mpObject->second.erase(this);

      if (mpObject->second.empty())
        Manager.erase(mpObject);
    }

  mpObject = rhs.mpObject;

  if (mpObject != Manager.end())
    mpObject->second.insert(this);

  return *this;
}

cpsapiPointer & cpsapiPointer::operator= (CDataObject * pObject)
{
  if ((mpObject == Manager.end() && pObject == nullptr)
      || mpObject->first == pObject)
    return *this;

  if (mpObject != Manager.end())
    {
      mpObject->second.erase(this);

      if (mpObject->second.empty())
        Manager.erase(mpObject);
    }

  mpObject = Manager.end();

  if (pObject != nullptr)
    {
      mpObject = Manager.insert(std::make_pair(pObject, std::set< cpsapiPointer * >())).first;
      mpObject->second.insert(this);
    }

  return *this;
}

CDataObject * cpsapiPointer::operator->() const
{
  if (mpObject != Manager.end())
    return mpObject->first;

  return nullptr;
}

CDataObject * cpsapiPointer::operator*() const
{
  if (mpObject != Manager.end())
    return mpObject->first;

  return nullptr;
}

cpsapiPointer::operator bool() const
{
  if (mpObject != Manager.end())
    return mpObject->first != nullptr;

  return false;
}