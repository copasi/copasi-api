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
#include "cpsapi/model/cpsapiTransaction.h"

#include <copasi/utilities/CValidatedUnit.h>

CPSAPI_NAMESPACE_USE

// static
const cpsapiModel::Properties cpsapiModel::SupportedProperties =
  {
    CData::Property::UNIT, // Synonym for TIME_UNIT 
    CData::Property::VOLUME_UNIT,
    CData::Property::AREA_UNIT,
    CData::Property::LENGTH_UNIT,
    CData::Property::TIME_UNIT,
    CData::Property::QUANTITY_UNIT,
    CData::Property::MODEL_TYPE,
    CData::Property::AVOGADRO_NUMBER
  };

// static
const cpsapiModel::Properties cpsapiModel::HiddenProperties =
  {
    CData::Property::INITIAL_EXPRESSION,
    CData::Property::EXPRESSION,
    CData::Property::SIMULATION_TYPE,
    CData::Property::ADD_NOISE,
    CData::Property::NOISE_EXPRESSION
  };

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
{}

// virtual
cpsapiModel::~cpsapiModel()
{}

// virtual 
void cpsapiModel::accept(cpsapiVisitor & visitor)
{
  if (!mpObject)
    return;

  visitor.visit(this, cpsapiVisitor::TypeId::cpsapiModel);
  base::accept(visitor);
}

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

  if (!isValidProperty<cpsapiModel>(property))
    return base::set(property, value, CCore::Framework::__SIZE);

  CCore::Framework Framework(framework);

  CModel * pModel = static_cast< CModel * >(*mpObject);
  const CDataObject * pChangedObject = pModel;

  bool success = cpsapiTransaction::endStructureChange(pModel);

  switch (property)
    {
    case CData::Property::VOLUME_UNIT:
      if (value.getType() == CDataValue::STRING)
        {
          CUnit Unit;
          success &= !Unit.setExpression(value.toString());
          success &= Unit.isEquivalent(CUnit("m^3")) || Unit.isDimensionless();

          if (success)
            success &= pModel->setVolumeUnit(value.toString());
        }
      else 
        success = false;
        
      break;

    case CData::Property::AREA_UNIT:
      if (value.getType() == CDataValue::STRING)
        {
          CUnit Unit;
          success &= !Unit.setExpression(value.toString());
          success &= Unit.isEquivalent(CUnit("m^2")) || Unit.isDimensionless();

          if (success)
            success &= pModel->setAreaUnit(value.toString());
        }
      else 
        success = false;
        
      break;

    case CData::Property::LENGTH_UNIT:
      if (value.getType() == CDataValue::STRING)
        {
          CUnit Unit;
          success &= !Unit.setExpression(value.toString());
          success &= Unit.isEquivalent(CUnit("m")) || Unit.isDimensionless();

          if (success)
            success &= pModel->setLengthUnit(value.toString());
        }
      else 
        success = false;
        
      break;

    case CData::Property::UNIT:
    case CData::Property::TIME_UNIT:
      if (value.getType() == CDataValue::STRING)
        {
          CUnit Unit;
          success &= !Unit.setExpression(value.toString());
          success &= Unit.isEquivalent(CUnit("s")) || Unit.isDimensionless();

          if (success)
            success &= pModel->setTimeUnit(value.toString());
        }
      else 
        success = false;
        
      break;

    case CData::Property::QUANTITY_UNIT:
      if (value.getType() == CDataValue::STRING)
        {
          if (Framework == CCore::Framework::__SIZE)
            Framework = CCore::Framework::Concentration;

          pChangedObject = CObjectInterface::DataObject(pModel->getObject(CCommonName("Reference=Quantity Conversion Factor")));

          CUnit Unit;
          success &= !Unit.setExpression(value.toString());
          success &= Unit.isEquivalent(CUnit("#")) || Unit.isDimensionless();

          if (success)
            success &= pModel->setQuantityUnit(value.toString(), Framework);
        }
      else 
        success = false;
        
      break;

    case CData::Property::MODEL_TYPE:
      if (value.getType() == CDataValue::STRING &&
          CModel::ModelTypeNames.toEnum(value.toString()) != CModel::ModelType::__SIZE)
        pModel->setModelType(CModel::ModelTypeNames.toEnum(value.toString()));
      else 
        success = false;

      break;

    case CData::Property::AVOGADRO_NUMBER:
      if (value.getType() == CDataValue::DOUBLE)
        {
          if (Framework == CCore::Framework::__SIZE)
            Framework = CCore::Framework::Concentration;

          pChangedObject = CObjectInterface::DataObject(pModel->getObject(CCommonName("Reference=Avogadro Constant")));
          pModel->setAvogadro(value.toDouble(), Framework);
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
CDataValue cpsapiModel::get(const CData::Property & property, const CCore::Framework & framework) const
{
  if (!mpObject)
    return CDataValue();

  if (!isValidProperty<cpsapiModel>(property))
    return base::get(property, CCore::Framework::__SIZE);

  CCore::Framework Framework(framework);
  bool success = false;

  CModel * pModel = static_cast< CModel * >(*mpObject);
  CDataObject * pChangedObject = pModel;

  switch (property)
    {
    case CData::Property::UNIT:
      return pModel->getUnits();
      break;

    case CData::Property::VOLUME_UNIT:
      return pModel->getVolumeUnit();
      break;

    case CData::Property::AREA_UNIT:
      return pModel->getAreaUnit();
      break;

    case CData::Property::LENGTH_UNIT:
      return pModel->getLengthUnit();
      break;

    case CData::Property::TIME_UNIT:
      return pModel->getTimeUnit();
      break;

    case CData::Property::QUANTITY_UNIT:
      return pModel->getQuantityUnit();
      break;

    case CData::Property::MODEL_TYPE:
      return pModel->CModel::ModelTypeNames[pModel->getModelType()];
      break;

    case CData::Property::AVOGADRO_NUMBER:
      return pModel->getAvogadro();
      break;

    default:
      break;
    }

  return CDataValue();
}
