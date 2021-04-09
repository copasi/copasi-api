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

#include "cpsapi/core/cpsapiTransaction.h"
#include "cpsapi/model/cpsapiModel.h"

#include <copasi/core/CDataObject.h>
#include <copasi/model/CModel.h>

CPSAPI_NAMESPACE_BEGIN
CPSAPI_NAMESPACE_USE

// static
cpsapiTransaction::Map cpsapiTransaction::Transactions;

// static
bool cpsapiTransaction::beginTransaction(CModel * pModel)
{
  return Transactions.insert(std::make_pair(pModel, sChangeInfo())).second;
}

// static 
bool cpsapiTransaction::endTransaction(CModel * pModel)
{
  Map::iterator found = Transactions.find(pModel);

  if (found != Transactions.end())
    {
      return cpsapiModel(found->first).synchronize(found->second.changed);
    }
  
  return false;
}

// static
bool cpsapiTransaction::synchronize(CDataObject * pObject, const CCore::Framework & framework)
{
  CModel * pModel = dynamic_cast< CModel * >(pObject->getObjectAncestor("Model"));

  Map::iterator found = Transactions.find(pModel);

  if (found != Transactions.end())
    if (framework == CCore::Framework::__SIZE
        || framework == found->second.framework)
      {
        found->second.changed.insert(pObject);
        return true;
      }
    else if (found->second.framework == CCore::Framework::__SIZE)
      {
        found->second.framework = framework;
      }
    else // Conflicting frame work: complete transaction and start new
      {
        bool success = endTransaction(pModel);
        success &= beginTransaction(pModel);
        success &= synchronize(pObject, framework);
        return success;
      }

  std::set< const CDataObject * > Changed;
  Changed.insert(pObject);
  
  return cpsapiModel(pModel).synchronize(Changed);
}

CPSAPI_NAMESPACE_END