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
  if (mpObject)
    cpsapiTransaction::beginTransaction(static_cast< CModel * >(*mpObject));
}

void cpsapiModel::endTransaction() const
{
  if (mpObject)
    cpsapiTransaction::endTransaction(static_cast< CModel * >(*mpObject));
}

bool cpsapiModel::synchronize(std::set< const CDataObject * > & changedObjects)
{
  if (!mpObject)
    return false;

  static_cast< CModel * >(*mpObject)->updateInitialValues(changedObjects);

  return true; 
}

cpsapiCompartment cpsapiModel::addCompartment(const std::string & name)
{
  if (!mpObject)
    return nullptr;

  CCompartment * pCompartment = static_cast< CModel * >(*mpObject)->createCompartment(name);

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
  if (!mpObject)
    return std::vector< cpsapiCompartment >();

  return convertVector< cpsapiCompartment >(static_cast< CModel * >(*mpObject)->getCompartments());
}

CCompartment * cpsapiModel::__compartment(const std::string & name) const
{
  if (!mpObject)
    return nullptr;
    
  if (name.empty())
    return mpDefaultCompartment;

  size_t Index = static_cast< CModel * >(*mpObject)->getCompartments().getIndex(name);

  if (Index == C_INVALID_INDEX)
    return nullptr;

  return  &static_cast< CModel * >(*mpObject)->getCompartments()[Index];
}

cpsapiSpecies cpsapiModel::addSpecies(const std::string & name, const std::string & compartment)
{
  if (!mpObject)
    return nullptr;

  CCompartment * pCompartment = __compartment(compartment);

  if (pCompartment == nullptr)
    return nullptr;

  return static_cast< CModel * >(*mpObject)->createMetabolite(name, compartment);
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
  if (!mpObject)
    return std::vector< cpsapiSpecies >();

  return convertVector< cpsapiSpecies >(static_cast< CModel * >(*mpObject)->getMetabolites());
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
  if (!mpObject)
    return;
  
  static_cast< CModel * >(*mpObject)->compileIfNecessary(nullptr);

  CModel::DataObjectSet DependentReactions;
  CModel::DataObjectSet DependentMetabolites;
  CModel::DataObjectSet DependentCompartments;
  CModel::DataObjectSet DependentModelValues;
  CModel::DataObjectSet DependentEvents;
  CModel::DataObjectSet DependentEventAssignments;

  static_cast< CModel * >(*mpObject)->appendAllDependents(*pContainer,
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