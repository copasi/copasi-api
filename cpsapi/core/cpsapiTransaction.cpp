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

#include "cpsapi/core/cpsapiTransaction.h"
#include "cpsapi/model/cpsapiModel.h"

#include <copasi/core/CDataObject.h>
#include <copasi/model/CModel.h>

CPSAPI_NAMESPACE_USE

// static
cpsapiTransaction::Map cpsapiTransaction::Transactions;

// static
cpsapiTransaction::MapStructureChange cpsapiTransaction::StructureChange;

// static
bool cpsapiTransaction::beginTransaction(CModel * pModel)
{
  return endStructureChange(pModel)
         && Transactions.insert(std::make_pair(pModel, sChangeInfo())).second;
}

// static 
bool cpsapiTransaction::endTransaction(CModel * pModel)
{
  bool success = false;
  Map::iterator found = Transactions.find(pModel);

  if (found != Transactions.end()
      && !found->second.changed.empty())
    {
      success = cpsapiModel(found->first).synchronize(found->second.changed, found->second.framework);
      Transactions.erase(found);
    }
  
  return success;
}

// static
bool cpsapiTransaction::synchronize(CDataObject * pObject, const CCore::Framework & framework)
{
  CModel * pModel = dynamic_cast< CModel * >(pObject->getObjectAncestor("Model"));
  Map::iterator found = Transactions.find(pModel);

  if (found == Transactions.end())
    {
      std::set< const CDataObject * > Changed;
      Changed.insert(pObject);
      
      return cpsapiModel(pModel).synchronize(Changed, framework);
    }

  std::pair< MapStructureChange::iterator, bool > result = StructureChange.insert(std::make_pair(pModel, false));
  bool success = (result.first->second == false);

  if (!success)
    {
      success &= cpsapiModel(pModel).compile();
      result.first->second = false;
    }

  if (framework == CCore::Framework::__SIZE
      || framework == found->second.framework)
    {
      found->second.changed.insert(pObject);
    }
  else if (found->second.framework == CCore::Framework::__SIZE)
    {
      found->second.changed.insert(pObject);
      found->second.framework = framework;
    }
  else // Conflicting frame work: complete transaction and start new
    {
      bool success = endTransaction(pModel);
      success &= beginTransaction(pModel);
      success &= synchronize(pObject, framework);
    }

  return success;
}

// static 
bool cpsapiTransaction::beginStructureChange(CModel * pModel)
{
  bool success = true;
  Map::iterator found = Transactions.find(pModel);

  if (found != Transactions.end()
      && !found->second.changed.empty())
    {
      success &= endTransaction(pModel);
      success &= Transactions.insert(std::make_pair(pModel, sChangeInfo())).second;
    }

  StructureChange.insert(std::make_pair(pModel, true)).first->second = true;
  return success;
}

// static 
bool cpsapiTransaction::endStructureChange(CModel * pModel)
{
  MapStructureChange::iterator found = StructureChange.find(pModel);

  if (found == StructureChange.end()
      || found->second == false)
    return true;

  // We must not have any changes scheduled
  bool success = (Transactions.find(pModel) == Transactions.end());

  success &= cpsapiModel(pModel).compile();
  found->second = false;
  
  return success;
}
