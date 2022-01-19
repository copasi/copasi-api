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

#include "cpsapi/model/cpsapiModel.h"
#include "cpsapi/model/cpsapiTransaction.h"

#include <copasi/utilities/CValidatedUnit.h>

CPSAPI_NAMESPACE_USE

// static
const cpsapiModel::Properties cpsapiModel::SupportedProperties =
  {
    cpsapiProperty::Type::UNIT, // Synonym for TIME_UNIT
    cpsapiProperty::Type::VOLUME_UNIT,
    cpsapiProperty::Type::AREA_UNIT,
    cpsapiProperty::Type::LENGTH_UNIT,
    cpsapiProperty::Type::TIME_UNIT,
    cpsapiProperty::Type::QUANTITY_UNIT,
    cpsapiProperty::Type::MODEL_TYPE,
    cpsapiProperty::Type::AVOGADRO_NUMBER};

// static
const cpsapiModel::Properties cpsapiModel::HiddenProperties =
  {
    cpsapiProperty::Type::INITIAL_EXPRESSION,
    cpsapiProperty::Type::EXPRESSION,
    cpsapiProperty::Type::SIMULATION_TYPE,
    cpsapiProperty::Type::ADD_NOISE,
    cpsapiProperty::Type::NOISE_EXPRESSION};

cpsapiModel::cpsapiModel(wrapped * pWrapped)
  : base(pWrapped, Type::Model)
{
  assertData< cpsapiModel >(pWrapped);
}

cpsapiModel::cpsapiModel(const cpsapiModel & src)
  : base(src)
{}

// virtual
cpsapiModel::~cpsapiModel()
{}

// virtual
void cpsapiModel::accept(cpsapiVisitor & visitor)
{
  if (!operator bool())
    return;

  visitor.visit(this, Type::Model);
  base::accept(visitor);
}

void cpsapiModel::beginTransaction() const
{
  if (operator bool())
    cpsapiTransaction::beginTransaction(WRAPPED);
}

void cpsapiModel::endTransaction() const
{
  if (operator bool())
    cpsapiTransaction::endTransaction(WRAPPED);
}

bool cpsapiModel::synchronize(std::set< const CDataObject * > & /* changedObjects */, const CCore::Framework & framework)
{
  if (!operator bool())
    return false;

  WRAPPED->updateInitialValues(framework != CCore::Framework::__SIZE ? framework : CCore::Framework::Concentration);

  return true;
}

bool cpsapiModel::compile()
{
  if (!operator bool())
    return false;

  return WRAPPED->compileIfNecessary(nullptr);
}

cpsapiCompartment cpsapiModel::addCompartment(const std::string & name)
{
  if (!operator bool())
    return nullptr;

  cpsapiTransaction::beginStructureChange(WRAPPED);

  CCompartment * pCompartment = WRAPPED->createCompartment(name);

  if (pCompartment == nullptr)
    return nullptr;

  updateDefaultCompartment(pCompartment);

  return DATA->mDefaultCompartment;
}

bool cpsapiModel::deleteCompartment(const std::string & name)
{
  CCompartment * pCompartment = static_cast< CCompartment *>(*__compartment(name));

  if (pCompartment == nullptr)
    return false;

  if (*DATA->mDefaultCompartment == pCompartment)
    updateDefaultCompartment(nullptr);

  cpsapiTransaction::beginStructureChange(WRAPPED);

  deleteAllDependents(pCompartment);
  deleted(pCompartment);
  delete pCompartment;

  return true;
}

cpsapiCompartment cpsapiModel::compartment(const std::string & name)
{
  cpsapiCompartment Compartment = __compartment(name);

  if (!Compartment)
    return nullptr;

  if (*DATA->mDefaultCompartment != *Compartment)
    updateDefaultCompartment(Compartment);

  return DATA->mDefaultCompartment;
}

cpsapiVector< cpsapiCompartment > cpsapiModel::getCompartments() const
{
  if (!operator bool())
    return cpsapiVector< cpsapiCompartment >();

  return cpsapiVector< cpsapiCompartment >(&WRAPPED->getCompartments());
}

cpsapiCompartment cpsapiModel::__compartment(const std::string & name) const
{
  if (!operator bool())
    return nullptr;

  if (name.empty())
    return DATA->mDefaultCompartment;

  size_t Index = WRAPPED->getCompartments().getIndex(name);

  if (Index == C_INVALID_INDEX)
    return nullptr;

  return &WRAPPED->getCompartments()[Index];
}

void cpsapiModel::updateDefaultCompartment(const cpsapiCompartment & compartment)
{
  DATA->mDefaultCompartment = compartment;
}

cpsapiSpecies cpsapiModel::addSpecies(const std::string & name, const std::string & compartment)
{
  if (!operator bool())
    return nullptr;

  cpsapiCompartment Compartment(__compartment(compartment));

  if (!Compartment)
    return nullptr;

  if (*DATA->mDefaultCompartment != *Compartment)
    updateDefaultCompartment(Compartment);

  cpsapiTransaction::beginStructureChange(WRAPPED);
  CMetab * pMetab = WRAPPED->createMetabolite(name, compartment);

  if (pMetab != nullptr)
    DATA->mDefaultCompartment.species(name);

  return pMetab;
}

bool cpsapiModel::deleteSpecies(const std::string & name, const std::string & compartment)
{
  return __compartment(compartment).deleteSpecies(name);
}

cpsapiSpecies cpsapiModel::species(const std::string & name, const std::string & compartment)
{
  return __species(name, compartment);
}

cpsapiVector< cpsapiSpecies > cpsapiModel::getSpecies() const
{
  if (!operator bool())
    return cpsapiVector< cpsapiSpecies >();

  return cpsapiVector< cpsapiSpecies >(&WRAPPED->getMetabolites());
}

cpsapiSpecies cpsapiModel::__species(const std::string & name, const std::string & compartment) const
{
  return __compartment(compartment).species(name);
}

cpsapiGlobalQuantity cpsapiModel::addGlobalQuantity(const std::string & name)
{
  if (!operator bool())
    return nullptr;

  cpsapiTransaction::beginStructureChange(WRAPPED);

  CModelValue * pGlobalQuantity = WRAPPED->createModelValue(name);

  if (pGlobalQuantity != nullptr)
    updateDefaultGlobalQuantity(pGlobalQuantity);

  return pGlobalQuantity;
}

bool cpsapiModel::deleteGlobalQuantity(const std::string & name)
{
  CModelValue * pGlobalQuantity = static_cast< CModelValue *>(*__globalQuantity(name));;

  if (pGlobalQuantity == nullptr)
    return false;

  if (*DATA->mDefaultGlobalQuantity == pGlobalQuantity)
    updateDefaultGlobalQuantity(nullptr);

  cpsapiTransaction::beginStructureChange(WRAPPED);

  deleteAllDependents(pGlobalQuantity);
  deleted(pGlobalQuantity);
  delete pGlobalQuantity;

  return true;
}

cpsapiGlobalQuantity cpsapiModel::globalQuantity(const std::string & name)
{
  cpsapiGlobalQuantity GlobalQuantity = __globalQuantity(name);

  if (!GlobalQuantity)
    return nullptr;

  if (*DATA->mDefaultGlobalQuantity != *GlobalQuantity)
    updateDefaultGlobalQuantity(GlobalQuantity);

  return GlobalQuantity;
}

cpsapiVector< cpsapiGlobalQuantity > cpsapiModel::getGlobalQuantities() const
{
  if (!operator bool())
    return cpsapiVector< cpsapiGlobalQuantity >();

  return cpsapiVector< cpsapiGlobalQuantity >(&WRAPPED->getModelValues());
}

cpsapiGlobalQuantity cpsapiModel::__globalQuantity(const std::string & name) const
{
  if (!operator bool())
    return nullptr;

  if (name.empty())
    return DATA->mDefaultGlobalQuantity;

  size_t Index = WRAPPED->getModelValues().getIndex(name);

  if (Index == C_INVALID_INDEX)
    return nullptr;

  return &WRAPPED->getModelValues()[Index];
}

void cpsapiModel::updateDefaultGlobalQuantity(const cpsapiGlobalQuantity & globalQuantity)
{
  DATA->mDefaultGlobalQuantity = globalQuantity;
}

cpsapiReaction cpsapiModel::addReaction(const std::string & name)
{
  if (!operator bool())
    return nullptr;

  cpsapiTransaction::beginStructureChange(WRAPPED);

  CReaction * pReaction = WRAPPED->createReaction(name);

  if (pReaction != nullptr)
    updateDefaultReaction(pReaction);

  return pReaction;
}

bool cpsapiModel::deleteReaction(const std::string & name)
{
  CReaction * pReaction = static_cast< CReaction *>(*__reaction(name));

  if (pReaction == nullptr)
    return false;

  if (*DATA->mDefaultReaction == pReaction)
    updateDefaultReaction(nullptr);

  cpsapiTransaction::beginStructureChange(WRAPPED);

  deleteAllDependents(pReaction);
  deleted(pReaction);
  delete pReaction;

  return true;
}

cpsapiReaction cpsapiModel::reaction(const std::string & name)
{
  cpsapiReaction Reaction = __reaction(name);

  if (!Reaction)
    return nullptr;

  if (*DATA->mDefaultReaction != *Reaction)
    updateDefaultReaction(Reaction);

  return Reaction;
}

cpsapiVector< cpsapiReaction > cpsapiModel::getReactions() const
{
  if (!operator bool())
    return cpsapiVector< cpsapiReaction >();

  return cpsapiVector< cpsapiReaction >(&WRAPPED->getReactions());
}

cpsapiReaction cpsapiModel::__reaction(const std::string & name) const
{
  if (!operator bool())
    return nullptr;

  if (name.empty())
    return DATA->mDefaultReaction;

  size_t Index = WRAPPED->getReactions().getIndex(name);

  if (Index == C_INVALID_INDEX)
    return nullptr;

  return &WRAPPED->getReactions()[Index];
}

void cpsapiModel::updateDefaultReaction(const cpsapiReaction & reaction)
{
  DATA->mDefaultReaction = reaction;
}

void cpsapiModel::deleteDependents(const CDataObject::DataObjectSet & set)
{
  for (const CDataObject * pObject : set)
    {
      deleted(pObject);
      delete pObject;
    }
}

void cpsapiModel::deleteAllDependents(CDataContainer * pContainer)
{
  if (!operator bool())
    return;

  WRAPPED->compileIfNecessary(nullptr);

  wrapped::DataObjectSet DependentReactions;
  wrapped::DataObjectSet DependentMetabolites;
  wrapped::DataObjectSet DependentCompartments;
  wrapped::DataObjectSet DependentModelValues;
  wrapped::DataObjectSet DependentEvents;
  wrapped::DataObjectSet DependentEventAssignments;

  WRAPPED->appendAllDependents(*pContainer,
                               DependentReactions,
                               DependentMetabolites,
                               DependentCompartments,
                               DependentModelValues,
                               DependentEvents,
                               DependentEventAssignments,
                               true);

  deleteDependents(DependentReactions);
  deleteDependents(DependentMetabolites);
  deleteDependents(DependentCompartments);
  deleteDependents(DependentModelValues);
  deleteDependents(DependentEventAssignments);
  deleteDependents(DependentEvents);
}

bool cpsapiModel::setProperty(const cpsapiModel::Property & property, const cpsapiData & value, const CCore::Framework & framework)
{
  return setProperty(static_cast< const cpsapiProperty::Type >(property), value, framework);
}

cpsapiData cpsapiModel::getProperty(const cpsapiModel::Property & property, const CCore::Framework & framework) const
{
  return getProperty(static_cast< const cpsapiProperty::Type >(property), framework);
}

// virtual
bool cpsapiModel::setProperty(const cpsapiProperty::Type & property, const cpsapiData & value, const CCore::Framework & framework)
{
  if (!operator bool())
    return false;

  if (!isImplementedProperty< cpsapiModel >(property))
    return base::setProperty(property, value, CCore::Framework::__SIZE);

  CCore::Framework Framework(framework);

  wrapped * pWrapped = WRAPPED;
  const CDataObject * pChangedObject = pWrapped;

  bool success = cpsapiTransaction::endStructureChange(pWrapped);

  switch (property)
    {
    case cpsapiProperty::Type::VOLUME_UNIT:
      if (value.getType() == cpsapiData::Type::String)
        {
          CUnit Unit;
          success &= !Unit.setExpression(value.toString());
          success &= Unit.isEquivalent(CUnit("m^3")) || Unit.isDimensionless();

          if (success)
            success &= pWrapped->setVolumeUnit(value.toString());
        }
      else
        success = false;

      break;

    case cpsapiProperty::Type::AREA_UNIT:
      if (value.getType() == cpsapiData::Type::String)
        {
          CUnit Unit;
          success &= !Unit.setExpression(value.toString());
          success &= Unit.isEquivalent(CUnit("m^2")) || Unit.isDimensionless();

          if (success)
            success &= pWrapped->setAreaUnit(value.toString());
        }
      else
        success = false;

      break;

    case cpsapiProperty::Type::LENGTH_UNIT:
      if (value.getType() == cpsapiData::Type::String)
        {
          CUnit Unit;
          success &= !Unit.setExpression(value.toString());
          success &= Unit.isEquivalent(CUnit("m")) || Unit.isDimensionless();

          if (success)
            success &= pWrapped->setLengthUnit(value.toString());
        }
      else
        success = false;

      break;

    case cpsapiProperty::Type::UNIT:
    case cpsapiProperty::Type::TIME_UNIT:
      if (value.getType() == cpsapiData::Type::String)
        {
          CUnit Unit;
          success &= !Unit.setExpression(value.toString());
          success &= Unit.isEquivalent(CUnit("s")) || Unit.isDimensionless();

          if (success)
            success &= pWrapped->setTimeUnit(value.toString());
        }
      else
        success = false;

      break;

    case cpsapiProperty::Type::QUANTITY_UNIT:
      if (value.getType() == cpsapiData::Type::String)
        {
          if (Framework == CCore::Framework::__SIZE)
            Framework = CCore::Framework::Concentration;

          pChangedObject = CObjectInterface::DataObject(pWrapped->getObject(CCommonName ("Reference=Quantity Conversion Factor")));

          CUnit Unit;
          success &= !Unit.setExpression(value.toString());
          success &= Unit.isEquivalent(CUnit("#")) || Unit.isDimensionless();

          if (success)
            success &= pWrapped->setQuantityUnit(value.toString(), Framework);
        }
      else
        success = false;

      break;

    case cpsapiProperty::Type::MODEL_TYPE:
      if (value.getType() == cpsapiData::Type::String && wrapped::ModelTypeNames.toEnum(value.toString()) != wrapped::ModelType::__SIZE)
        pWrapped->setModelType(wrapped::ModelTypeNames.toEnum(value.toString()));
      else
        success = false;

      break;

    case cpsapiProperty::Type::AVOGADRO_NUMBER:
      if (value.getType() == cpsapiData::Type::Double)
        {
          if (Framework == CCore::Framework::__SIZE)
            Framework = CCore::Framework::Concentration;

          pChangedObject = CObjectInterface::DataObject(pWrapped->getObject(CCommonName ("Reference=Avogadro Constant")));
          pWrapped->setAvogadro(value.toDouble(), Framework);
        }
      else
        success = false;

      break;

    default:
      break;
    }

  return success && cpsapiTransaction::synchronize(pChangedObject, Framework);
}

// virtual
cpsapiData cpsapiModel::getProperty(const cpsapiProperty::Type & property, const CCore::Framework & framework) const
{
  if (!operator bool())
    return cpsapiData();

  if (!isImplementedProperty< cpsapiModel >(property))
    return base::getProperty(property, CCore::Framework::__SIZE);

  CCore::Framework Framework(framework);
  bool success = false;

  wrapped * pWrapped = WRAPPED;
  CDataObject * pChangedObject = pWrapped;

  switch (property)
    {
    case cpsapiProperty::Type::UNIT:
      return pWrapped->getUnits();
      break;

    case cpsapiProperty::Type::VOLUME_UNIT:
      return pWrapped->getVolumeUnit();
      break;

    case cpsapiProperty::Type::AREA_UNIT:
      return pWrapped->getAreaUnit();
      break;

    case cpsapiProperty::Type::LENGTH_UNIT:
      return pWrapped->getLengthUnit();
      break;

    case cpsapiProperty::Type::TIME_UNIT:
      return pWrapped->getTimeUnit();
      break;

    case cpsapiProperty::Type::QUANTITY_UNIT:
      return pWrapped->getQuantityUnit();
      break;

    case cpsapiProperty::Type::MODEL_TYPE:
      return pWrapped->wrapped::ModelTypeNames[pWrapped->getModelType()];
      break;

    case cpsapiProperty::Type::AVOGADRO_NUMBER:
      return pWrapped->getAvogadro();
      break;

    default:
      break;
    }

  return cpsapiData();
}
