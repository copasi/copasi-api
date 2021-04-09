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
#include "cpsapi/model/cpsapiSpecies.h"

CPSAPI_NAMESPACE_BEGIN

cpsapiModel::cpsapiModel(CModel * pModel)
  : base(pModel)
  , mpDefaultCompartment(nullptr)
  , mpDefaultReaction(nullptr)
  , mpDefaultGlobalQuantity(nullptr)
  , mpDefaultEvent(nullptr)
{}

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
  if (mObject)
    cpsapiTransaction::beginTransaction(static_cast< CModel * >(*mObject));
}

void cpsapiModel::endTransaction() const
{
  if (mObject)
    cpsapiTransaction::endTransaction(static_cast< CModel * >(*mObject));
}

bool cpsapiModel::synchronize(std::set< const CDataObject * > & changedObjects)
{
  if (!mObject)
    return false;

  static_cast< CModel * >(*mObject)->updateInitialValues(changedObjects);

  return true; 
}

cpsapiCompartment cpsapiModel::addCompartment(const std::string & name)
{
  if (!mObject)
    return nullptr;

  CCompartment * pCompartment = static_cast< CModel * >(*mObject)->createCompartment(name);

  if (pCompartment != nullptr)
    mpDefaultCompartment = pCompartment;

  return pCompartment;
}

bool cpsapiModel::deleteCompartment(const std::string & name)
{
  CCompartment * pCompartment = __compartment(name);

  if (pCompartment == nullptr)
    return false;

  if (mpDefaultCompartment == pCompartment)
    mpDefaultCompartment = nullptr;

  deleteAllDependents(pCompartment);
  cpsapiPointer::deleted(pCompartment);
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
  if (!mObject)
    return std::vector< cpsapiCompartment >();

  return convertVector< cpsapiCompartment >(static_cast< CModel * >(*mObject)->getCompartments());
}

CCompartment * cpsapiModel::__compartment(const std::string & name) const
{
  if (!mObject)
    return nullptr;
    
  if (name.empty())
    return mpDefaultCompartment;

  size_t Index = static_cast< CModel * >(*mObject)->getCompartments().getIndex(name);

  if (Index == C_INVALID_INDEX)
    return nullptr;

  return  &static_cast< CModel * >(*mObject)->getCompartments()[Index];
}

cpsapiSpecies cpsapiModel::addSpecies(const std::string & name, const std::string & compartment)
{
  if (!mObject)
    return nullptr;

  CCompartment * pCompartment = __compartment(compartment);

  if (pCompartment == nullptr)
    return nullptr;

  return static_cast< CModel * >(*mObject)->createMetabolite(name, compartment);
}

bool cpsapiModel::deleteSpecies(const std::string & name, const std::string & compartment)
{
  cpsapiCompartment Compartment(__compartment(compartment));

  if (!Compartment)
    return false;
  
  return Compartment.deleteSpecies(name);
}

cpsapiSpecies cpsapiModel::species(const std::string & name, const std::string & compartment)
{
  return __species(name, compartment);
}

std::vector< cpsapiSpecies > cpsapiModel::getSpecies() const
{
  if (!mObject)
    return std::vector< cpsapiSpecies >();

  return convertVector< cpsapiSpecies >(static_cast< CModel * >(*mObject)->getMetabolites());
}

CMetab * cpsapiModel::__species(const std::string & name, const std::string & compartment) const
{
  cpsapiCompartment Compartment(__compartment(compartment));

  if (!Compartment)
    return nullptr;
  
  return static_cast< CMetab * >(Compartment.species(name).getObject());
}

void cpsapiModel::deleteAllDependents(CDataContainer * pContainer)
{
  if (!mObject)
    return;
  
  static_cast< CModel * >(*mObject)->compileIfNecessary(nullptr);

  CModel::DataObjectSet DependentReactions;
  CModel::DataObjectSet DependentMetabolites;
  CModel::DataObjectSet DependentCompartments;
  CModel::DataObjectSet DependentModelValues;
  CModel::DataObjectSet DependentEvents;
  CModel::DataObjectSet DependentEventAssignments;

  static_cast< CModel * >(*mObject)->appendAllDependents(*pContainer,
                                                         DependentReactions,
                                                         DependentMetabolites,
                                                         DependentCompartments,
                                                         DependentModelValues,
                                                         DependentEvents,
                                                         DependentEventAssignments,
                                                         true);

  CDataObject * pObject = nullptr;

  deleteDependents(mpDefaultReaction, DependentReactions);
  deleteDependents(pObject, DependentMetabolites);
  deleteDependents(mpDefaultCompartment, DependentCompartments);
  deleteDependents(mpDefaultGlobalQuantity, DependentModelValues);
  deleteDependents(mpDefaultEvent, DependentEvents);
  deleteDependents(pObject, DependentEventAssignments);
}

CPSAPI_NAMESPACE_END