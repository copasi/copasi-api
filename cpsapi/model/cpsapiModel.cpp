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

CPSAPI_NAMESPACE_BEGIN

cpsapiModel::cpsapiModel(CModel * pModel)
  : base(pModel)
  , mpDefaultCompartment(nullptr)
  , mpDefaultReaction(nullptr)
  , mpDefaultGlobalQuantity(nullptr)
  , mpDefaultEvent(nullptr)
{
  if (dynamic_cast< CModel * >(mpObject) == nullptr)
    mpObject = nullptr;
}

cpsapiModel::cpsapiModel(const cpsapiModel & src)
  : base(src)
  , mpDefaultCompartment(src.mpDefaultCompartment)
  , mpDefaultReaction(src.mpDefaultReaction)
  , mpDefaultGlobalQuantity(src.mpDefaultGlobalQuantity)
  , mpDefaultEvent(src.mpDefaultEvent)
{
  // TODO CRITICAL Implement me!
}

// virtual
cpsapiModel::~cpsapiModel()
{}

void cpsapiModel::beginTransaction() const
{
  if (mpObject != nullptr)
    cpsapiTransaction::beginTransaction(static_cast< CModel * >(mpObject));
}

void cpsapiModel::endTransaction() const
{
  if (mpObject != nullptr)
    cpsapiTransaction::endTransaction(static_cast< CModel * >(mpObject));
}

bool cpsapiModel::synchronize(std::set< const CDataObject * > & changedObjects)
{
  if (mpObject == nullptr)
    return false;

  static_cast< CModel * >(mpObject)->updateInitialValues(changedObjects);

  return true; 
}

cpsapiCompartment cpsapiModel::addCompartment(const std::string & name)
{
  if (mpObject == nullptr)
    return nullptr;

  CCompartment * pCompartment = static_cast< CModel * >(mpObject)->createCompartment(name);

  if (pCompartment != nullptr)
    mpDefaultCompartment = pCompartment;

  return pCompartment;
}

bool cpsapiModel::deleteCompartment(const std::string & name)
{
  if (mpObject == nullptr)
    return false;

  CCompartment * pCompartment = __compartment(name);

  if (pCompartment == nullptr)
    return false;

  if (mpDefaultCompartment == pCompartment)
    mpDefaultCompartment = nullptr;

  deleteAllDependents(pCompartment);
  delete pCompartment;
  
  return true;
}

cpsapiCompartment cpsapiModel::compartment(const std::string & name)
{
  CCompartment * pCompartment = __compartment(name);

  if (pCompartment != nullptr)
    mpDefaultCompartment = pCompartment;

  return pCompartment;
}

std::vector< cpsapiCompartment > cpsapiModel::getCompartments() const
{
  if (mpObject == nullptr)
    return std::vector< cpsapiCompartment >();

  CDataVectorNS< CCompartment > & Compartments = static_cast< CModel * >(mpObject)->getCompartments();
  std::vector< cpsapiCompartment > Result(Compartments.size());

  std::vector< cpsapiCompartment >::iterator it = Result.begin();
  std::vector< cpsapiCompartment >::iterator end = Result.end();
  CDataVectorNS< CCompartment >::iterator itSrc = Compartments.begin();

  for (; it != end; ++it, ++itSrc)
    *it = &*itSrc;

  return Result;
}

CCompartment * cpsapiModel::__compartment(const std::string & name) const
{
  if (mpObject == nullptr)
    return nullptr;
    
  if (name.empty())
    return mpDefaultCompartment;

  size_t Index = static_cast< CModel * >(mpObject)->getCompartments().getIndex(name);

  if (Index == C_INVALID_INDEX)
    return nullptr;

  return  &static_cast< CModel * >(mpObject)->getCompartments()[Index];
}

void cpsapiModel::deleteAllDependents(CDataContainer * pContainer)
{
  CModel::DataObjectSet DependentReactions;
  CModel::DataObjectSet DependentMetabolites;
  CModel::DataObjectSet DependentCompartments;
  CModel::DataObjectSet DependentModelValues;
  CModel::DataObjectSet DependentEvents;
  CModel::DataObjectSet DependentEventAssignments;

  static_cast< CModel * >(mpObject)->appendAllDependents(*pContainer,
                                                         DependentReactions,
                                                         DependentMetabolites,
                                                         DependentCompartments,
                                                         DependentModelValues,
                                                         DependentEvents,
                                                         DependentEventAssignments,
                                                         true);

  CDataObject * pObject = nullptr;

  deleteDependents(mpDefaultReaction, DependentReactions);
  deleteDependents(mpDefaultReaction, DependentMetabolites);
  deleteDependents(pObject, DependentCompartments);
  deleteDependents(mpDefaultGlobalQuantity, DependentModelValues);
  deleteDependents(mpDefaultEvent, DependentEvents);
  deleteDependents(pObject, DependentEventAssignments);

}

CPSAPI_NAMESPACE_END