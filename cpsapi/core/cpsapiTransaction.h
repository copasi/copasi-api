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

#pragma once

#include <set>
#include <map>

#include "cpsapi/cpsapiConfig.h"

#include <copasi/core/CCore.h>

class CModel;
class CDataObject;

CPSAPI_NAMESPACE_BEGIN

class cpsapiTransaction
{
friend class cpsapiObject;
friend class cpsapiModelEntity;
friend class cpsapiCompartment;
friend class cpsapiSpecies;
friend class cpsapiModel;

private:
  struct sChangeInfo {
    std::set< const CDataObject * > changed;
    CCore::Framework framework = CCore::Framework::__SIZE;
  };

  typedef std::map< CModel *, sChangeInfo > Map; 

  static Map Transactions;

private:
  static bool beginTransaction(CModel * pModel);

  static bool endTransaction(CModel * pModel);
  
  static bool synchronize(CDataObject * pObject, const CCore::Framework & framework);
};

CPSAPI_NAMESPACE_END