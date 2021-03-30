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

class CModel;
class CDataObject;

CPSAPI_NAMESPACE_BEGIN

class cpsapiTransaction
{
private:
  typedef std::map< CModel *, std::set< CDataObject * > > Map; 
  static Map Transactions;

public:
  static bool beginTransaction(CModel * pModel);

  static bool endTransaction(CModel * pModel);
  
  static bool synchronize(CDataObject * pObject);
};

CPSAPI_NAMESPACE_END