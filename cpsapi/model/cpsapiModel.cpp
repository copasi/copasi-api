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

#include "cpsapi/model/cpsapiModel.h"
#include "cpsapi/model/cpsapiCompartment.h"

#include <copasi/model/CModel.h>

CPSAPI_NAMESPACE_BEGIN

cpsapiModel::cpsapiModel(CModel * pModel)
  : cpsapiModelEntity(pModel)
{
  // TODO CRITICAL Implement me!
}

cpsapiModel::cpsapiModel(const cpsapiModel & src)
  : cpsapiModelEntity(src)
{
  // TODO CRITICAL Implement me!
}

// virtual
cpsapiModel::~cpsapiModel()
{
  // TODO CRITICAL Implement me!
}

void cpsapiModel::beginTransaction() const
{
  // TODO CRITICAL Implement me!
}

void cpsapiModel::endTransaction() const
{
  // TODO CRITICAL Implement me!
}

bool cpsapiModel::synchronize(const std::set< CDataObject * > & changedObjects)
{
  // TODO CRITICAL Implement me!

  return false;
}

bool cpsapiModel::addCompartment(const std::string & name)
{
  // TODO CRITICAL Implement me!

  return false;
}

bool cpsapiModel::deleteCompartment(const std::string & name)
{
  // TODO CRITICAL Implement me!

  return false;
}

bool cpsapiModel::selectCompartment(const std::string & name)
{
  // TODO CRITICAL Implement me!

  return false;
}

CCompartment * cpsapiModel::compartment(const std::string & name)
{
  // TODO CRITICAL Implement me!

  return nullptr;
}

std::vector< cpsapiCompartment > cpsapiModel::getCompartments() const
{
  // TODO CRITICAL Implement me!

  return std::vector< cpsapiCompartment >();
}

void cpsapiModel::changeCompartment(CCompartment * pCompartment)
{
  // TODO CRITICAL Implement me!
}

void cpsapiModel::assertDefaultCompartment()
{
  // TODO CRITICAL Implement me!
}

CPSAPI_NAMESPACE_END