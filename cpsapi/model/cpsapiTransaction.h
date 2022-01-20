// BEGIN: Copyright 
// Copyright (C) 2021 - 2022 by Pedro Mendes, Rector and Visitors of the 
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

#pragma GCC diagnostic push
#include <copasi/core/CCore.h>
#pragma GCC diagnostic pop

class CModel;
class CDataObject;

CPSAPI_NAMESPACE_BEGIN

class cpsapiTransaction
{
  /**
   * Transactions
   */ 
friend class cpsapiObject;
friend class cpsapiModelEntity;
friend class cpsapiCompartment;
friend class cpsapiSpecies;
friend class cpsapiGlobalQuantity;
friend class cpsapiReaction;
friend class cpsapiKineticLawVariable;
friend class cpsapiModel;

private:
  struct sChangeInfo {
    std::set< const CDataObject * > changed;
    CCore::Framework framework = CCore::Framework::__SIZE;
  };

  typedef std::map< CModel *, sChangeInfo > Map; 

  static Map Transactions;

  typedef std::map< CModel *, bool > MapStructureChange; 

  static MapStructureChange StructureChange;

private:
  static bool beginTransaction(CModel * pModel);

  static bool endTransaction(CModel * pModel);
  
  static bool synchronize(const CDataObject * pObject, const CCore::Framework & framework);

  static bool beginStructureChange(CModel * pModel);

  static bool endStructureChange(CModel * pModel);
};

CPSAPI_NAMESPACE_END