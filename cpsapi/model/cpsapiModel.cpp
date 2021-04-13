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

// static
cpsapiModel::Properties cpsapiModel::SupportedProperties =
  {
  };

cpsapiModel::cpsapiModel(CModel * pModel)
  : base(pModel)
  , mpDefaultCompartment(nullptr)
  , mpDefaultReaction(nullptr)
  , mpDefaultGlobalQuantity(nullptr)
  , mpDefaultEvent(nullptr)
{
  mpSupportedProperties = &SupportedProperties;
}

cpsapiModel::cpsapiModel(const cpsapiModel & src)
  : base(src)
  , mpDefaultCompartment(src.mpDefaultCompartment)
  , mpDefaultReaction(src.mpDefaultReaction)
  , mpDefaultGlobalQuantity(src.mpDefaultGlobalQuantity)
  , mpDefaultEvent(src.mpDefaultEvent)
{
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

bool cpsapiModel::synchronize(std::set< const CDataObject * > & /* changedObjects */, const CCore::Framework & framework)
{
  if (!mpObject)
    return false;

  static_cast< CModel * >(*mpObject)->updateInitialValues(framework != CCore::Framework::__SIZE ? framework : CCore::Framework::Concentration);

  return true; 
}

bool cpsapiModel::compile()
{
  if (!mpObject)
    return false;

  return static_cast< CModel * >(*mpObject)->compileIfNecessary(nullptr);
}

cpsapiCompartment cpsapiModel::addCompartment(const std::string & name)
{
  if (!mpObject)
    return nullptr;

  cpsapiTransaction::beginStructureChange(static_cast< CModel * >(*mpObject));

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

  cpsapiTransaction::beginStructureChange(static_cast< CModel * >(*mpObject));

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

  cpsapiTransaction::beginStructureChange(static_cast< CModel * >(*mpObject));
  CMetab * pMetab = static_cast< CModel * >(*mpObject)->createMetabolite(name, compartment);

  return pMetab;
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

bool cpsapiModel::set(const cpsapiModel::Property & property, const CDataValue & value, const CCore::Framework & framework)
{
  return set(static_cast<const CData::Property >(property), value, framework);
}

CDataValue cpsapiModel::get(const cpsapiModel::Property & property, const CCore::Framework & framework) const
{
  return get(static_cast<const CData::Property >(property), framework);
}

// virtual 
bool cpsapiModel::set(const CData::Property & property, const CDataValue & value, const CCore::Framework & framework)
{
  if (!mpObject)
    return false;

  if (!isValidProperty(property))
    return base::set(property, value, CCore::Framework::__SIZE);

  CCore::Framework Framework(framework);

  CModel * pModel = static_cast< CModel * >(*mpObject);
  CDataObject * pChangedObject = pModel;
  bool success = cpsapiTransaction::endStructureChange(pModel);

  switch (property)
    {
    default:
      break;
    }

  return success && cpsapiTransaction::synchronize(pChangedObject, Framework);
}

// virtual 
CDataValue cpsapiModel::get(const CData::Property & property, const CCore::Framework & framework) const
{
  if (!mpObject)
    return CDataValue();

  if (!isValidProperty(property))
    return base::get(property, CCore::Framework::__SIZE);

  CCore::Framework Framework(framework);
  bool success = false;

  CModel * pModel = static_cast< CModel * >(*mpObject);
  CDataObject * pChangedObject = pModel;

  switch (property)
    {
    default:
      break;
    }

  return CDataValue();
}
CPSAPI_NAMESPACE_END