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
 return Transactions.insert(std::make_pair(pModel, std::set< CDataObject * >())).second;
}

// static 
bool cpsapiTransaction::endTransaction(CModel * pModel)
{
  Map::iterator found = Transactions.find(pModel);

  if (found != Transactions.end())
    {
      return cpsapiModel(found->first).synchronize(found->second);
    }
  
  return false;
}

// static
bool cpsapiTransaction::synchronize(CDataObject * pObject)
{
  CModel * pModel = dynamic_cast< CModel * >(pObject->getObjectAncestor("Model"));

  Map::iterator found = Transactions.find(pModel);

  if (found != Transactions.end())
    {
      found->second.insert(pObject);
      return true;
    }

  std::set< CDataObject * > ChangedObjects = {pObject};
  return cpsapiModel(pModel).synchronize(ChangedObjects);
}

CPSAPI_NAMESPACE_END