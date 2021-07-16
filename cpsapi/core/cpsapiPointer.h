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

#pragma once

#include <set>
#include <map>

#include "cpsapi/cpsapiConfig.h"

class CDataObject;

CPSAPI_NAMESPACE_BEGIN

class cpsapiPointer
{
private:
  typedef std::set< cpsapiPointer * > References;
  typedef std::map< const CDataObject *, References * > Map; 

  static Map Manager;

  static Map::iterator findOrInsert(const CDataObject * pObject);
  
public:
  static void deleted(const CDataObject * pObject);

  cpsapiPointer(CDataObject * pObject = nullptr);

  cpsapiPointer(const cpsapiPointer & src);

  ~cpsapiPointer();

  cpsapiPointer & operator= (const cpsapiPointer & rhs);

  cpsapiPointer & operator= (CDataObject * pObject);

  CDataObject * operator-> () const;

  CDataObject * operator* () const;

  operator bool () const;

private:
  void addToReferences();

  void removeFromReferences();

  CDataObject * mpObject;
  References * mpReferences;
};

CPSAPI_NAMESPACE_END
