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

cpsapiModel::cpsapiModel(CModel * pModel)
  : base(pModel)
  , mDefaultCompartment(nullptr)
  , mpDefaultReaction(nullptr)
  , mDefaultGlobalQuantity(nullptr)
  , mpDefaultEvent(nullptr)
{
  for (cpsapiPointer * reference : references())
    if (this != reference)
      {
        mDefaultCompartment = static_cast< cpsapiModel * >(reference)->mDefaultCompartment;
        mDefaultGlobalQuantity = static_cast< cpsapiModel * >(reference)->mDefaultGlobalQuantity;
        break;
      }
}

cpsapiModel::cpsapiModel(const cpsapiModel & src)
  : base(src)
  , mDefaultCompartment(src.mDefaultCompartment)
  , mpDefaultReaction(src.mpDefaultReaction)
  , mDefaultGlobalQuantity(src.mDefaultGlobalQuantity)
  , mpDefaultEvent(src.mpDefaultEvent)
{}

// virtual
cpsapiModel::~cpsapiModel()
{}

// virtual
void cpsapiModel::accept(cpsapiVisitor & visitor)
{
  if (!operator bool())
    return;

  visitor.visit(this, cpsapiVisitor::TypeId::cpsapiModel);
  base::accept(visitor);
}

void cpsapiModel::beginTransaction() const
{
  if (operator bool())
    cpsapiTransaction::beginTransaction(static_cast< CModel * >(getObject()));
}

void cpsapiModel::endTransaction() const
{
  if (operator bool())
    cpsapiTransaction::endTransaction(static_cast< CModel * >(getObject()));
}

bool cpsapiModel::synchronize(std::set< const CDataObject * > & /* changedObjects */, const CCore::Framework & framework)
{
  if (!operator bool())
    return false;

  static_cast< CModel * >(getObject())->updateInitialValues(framework != CCore::Framework::__SIZE ? framework : CCore::Framework::Concentration);

  return true;
}

bool cpsapiModel::compile()
{
  if (!operator bool())
    return false;

  return static_cast< CModel * >(getObject())->compileIfNecessary(nullptr);
}

cpsapiCompartment cpsapiModel::addCompartment(const std::string & name)
{
  if (!operator bool())
    return nullptr;

  cpsapiTransaction::beginStructureChange(static_cast< CModel * >(getObject()));

  CCompartment * pCompartment = static_cast< CModel * >(getObject())->createCompartment(name);

  if (pCompartment == nullptr)
    return nullptr;

  updateDefaultCompartment(pCompartment);

  return mDefaultCompartment;
}

bool cpsapiModel::deleteCompartment(const std::string & name)
{
  CCompartment * pCompartment = static_cast< CCompartment *>(__compartment(name).getObject());

  if (pCompartment == nullptr)
    return false;

  if (mDefaultCompartment.getObject() == pCompartment)
    updateDefaultCompartment(nullptr);

  cpsapiTransaction::beginStructureChange(static_cast< CModel * >(getObject()));

  deleteAllDependents(pCompartment);
  cpsapiPointer::deleted(pCompartment);
  delete pCompartment;

  return true;
}

cpsapiCompartment cpsapiModel::compartment(const std::string & name)
{
  cpsapiCompartment Compartment = static_cast< CCompartment *>(__compartment(name).getObject());

  if (!Compartment)
    return nullptr;

  if (mDefaultCompartment.getObject() != Compartment.getObject())
    updateDefaultCompartment(Compartment);

  return mDefaultCompartment;
}

std::vector< cpsapiCompartment > cpsapiModel::getCompartments() const
{
  if (!operator bool())
    return std::vector< cpsapiCompartment >();

  return convertVector< cpsapiCompartment >(static_cast< CModel * >(getObject())->getCompartments());
}

cpsapiCompartment cpsapiModel::__compartment(const std::string & name) const
{
  if (!operator bool())
    return nullptr;

  if (name.empty())
    return mDefaultCompartment;

  size_t Index = static_cast< CModel * >(getObject())->getCompartments().getIndex(name);

  if (Index == C_INVALID_INDEX)
    return nullptr;

  return &static_cast< CModel * >(getObject())->getCompartments()[Index];
}

void cpsapiModel::updateDefaultCompartment(const cpsapiCompartment & compartment)
{
  for (cpsapiPointer * reference : references())
    {
      static_cast< cpsapiModel * >(reference)->mDefaultCompartment = compartment;
    }
}

cpsapiSpecies cpsapiModel::addSpecies(const std::string & name, const std::string & compartment)
{
  if (!operator bool())
    return nullptr;

  cpsapiCompartment Compartment(__compartment(compartment));

  if (!Compartment)
    return nullptr;

  if (mDefaultCompartment.getObject() != Compartment.getObject())
    updateDefaultCompartment(Compartment);

  cpsapiTransaction::beginStructureChange(static_cast< CModel * >(getObject()));
  CMetab * pMetab = static_cast< CModel * >(getObject())->createMetabolite(name, compartment);

  if (pMetab != nullptr)
    {
      mDefaultCompartment.species(name);
    }

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

std::vector< cpsapiSpecies > cpsapiModel::getSpecies() const
{
  if (!operator bool())
    return std::vector< cpsapiSpecies >();

  return convertVector< cpsapiSpecies >(static_cast< CModel * >(getObject())->getMetabolites());
}

cpsapiSpecies cpsapiModel::__species(const std::string & name, const std::string & compartment) const
{
  return __compartment(compartment).species(name);
}

cpsapiGlobalQuantity cpsapiModel::addGlobalQuantity(const std::string & name)
{
  if (!operator bool())
    return nullptr;

  cpsapiTransaction::beginStructureChange(static_cast< CModel * >(getObject()));

  CModelValue * pGlobalQuantity = static_cast< CModel * >(getObject())->createModelValue(name);

  if (pGlobalQuantity != nullptr)
    updateDefaultGlobalQuantity(pGlobalQuantity);

  return pGlobalQuantity;
}

bool cpsapiModel::deleteGlobalQuantity(const std::string & name)
{
  CModelValue * pGlobalQuantity = static_cast< CModelValue *>(__globalQuantity(name).getObject());;

  if (pGlobalQuantity == nullptr)
    return false;

  if (mDefaultGlobalQuantity.getObject() == pGlobalQuantity)
    updateDefaultGlobalQuantity(nullptr);

  cpsapiTransaction::beginStructureChange(static_cast< CModel * >(getObject()));

  deleteAllDependents(pGlobalQuantity);
  cpsapiPointer::deleted(pGlobalQuantity);
  delete pGlobalQuantity;

  return true;
}

cpsapiGlobalQuantity cpsapiModel::globalQuantity(const std::string & name)
{
  cpsapiGlobalQuantity GlobalQuantity = __globalQuantity(name);

  if (!GlobalQuantity)
    return nullptr;

  if (mDefaultGlobalQuantity.getObject() != GlobalQuantity.getObject())
    updateDefaultGlobalQuantity(GlobalQuantity);

  return GlobalQuantity;
}

std::vector< cpsapiGlobalQuantity > cpsapiModel::getGlobalQuantities() const
{
  if (!operator bool())
    return std::vector< cpsapiGlobalQuantity >();

  return convertVector< cpsapiGlobalQuantity >(static_cast< CModel * >(getObject())->getModelValues());
}

cpsapiGlobalQuantity cpsapiModel::__globalQuantity(const std::string & name) const
{
  if (!operator bool())
    return nullptr;

  if (name.empty())
    return mDefaultGlobalQuantity;

  size_t Index = static_cast< CModel * >(getObject())->getModelValues().getIndex(name);

  if (Index == C_INVALID_INDEX)
    return nullptr;

  return &static_cast< CModel * >(getObject())->getModelValues()[Index];
}

void cpsapiModel::updateDefaultGlobalQuantity(const cpsapiGlobalQuantity & globalQuantity)
{
  for (cpsapiPointer * reference : references())
    {
      static_cast< cpsapiModel * >(reference)->mDefaultGlobalQuantity = globalQuantity;
    }
}

void cpsapiModel::deleteDependents(const CDataObject::DataObjectSet & set)
{
  for (const CDataObject * pObject : set)
    {
      cpsapiPointer::deleted(pObject);
      delete pObject;
    }
}

void cpsapiModel::deleteAllDependents(CDataContainer * pContainer)
{
  if (!operator bool())
    return;

  static_cast< CModel * >(getObject())->compileIfNecessary(nullptr);

  CModel::DataObjectSet DependentReactions;
  CModel::DataObjectSet DependentMetabolites;
  CModel::DataObjectSet DependentCompartments;
  CModel::DataObjectSet DependentModelValues;
  CModel::DataObjectSet DependentEvents;
  CModel::DataObjectSet DependentEventAssignments;

  static_cast< CModel * >(getObject())->appendAllDependents(*pContainer,
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

bool cpsapiModel::set(const cpsapiModel::Property & property, const CDataValue & value, const CCore::Framework & framework)
{
  return set(static_cast< const cpsapiProperty::Type >(property), value, framework);
}

CDataValue cpsapiModel::get(const cpsapiModel::Property & property, const CCore::Framework & framework) const
{
  return get(static_cast< const cpsapiProperty::Type >(property), framework);
}

// virtual
bool cpsapiModel::set(const cpsapiProperty::Type & property, const CDataValue & value, const CCore::Framework & framework)
{
  if (!operator bool())
    return false;

  if (!isValidProperty< cpsapiModel >(property))
    return base::set(property, value, CCore::Framework::__SIZE);

  CCore::Framework Framework(framework);

  CModel * pModel = static_cast< CModel * >(getObject());
  const CDataObject * pChangedObject = pModel;

  bool success = cpsapiTransaction::endStructureChange(pModel);

  switch (property)
    {
    case cpsapiProperty::Type::VOLUME_UNIT:
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

    case cpsapiProperty::Type::AREA_UNIT:
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

    case cpsapiProperty::Type::LENGTH_UNIT:
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

    case cpsapiProperty::Type::UNIT:
    case cpsapiProperty::Type::TIME_UNIT:
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

    case cpsapiProperty::Type::QUANTITY_UNIT:
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

    case cpsapiProperty::Type::MODEL_TYPE:
      if (value.getType() == CDataValue::STRING && CModel::ModelTypeNames.toEnum(value.toString()) != CModel::ModelType::__SIZE)
        pModel->setModelType(CModel::ModelTypeNames.toEnum(value.toString()));
      else
        success = false;

      break;

    case cpsapiProperty::Type::AVOGADRO_NUMBER:
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
CDataValue cpsapiModel::get(const cpsapiProperty::Type & property, const CCore::Framework & framework) const
{
  if (!operator bool())
    return CDataValue();

  if (!isValidProperty< cpsapiModel >(property))
    return base::get(property, CCore::Framework::__SIZE);

  CCore::Framework Framework(framework);
  bool success = false;

  CModel * pModel = static_cast< CModel * >(getObject());
  CDataObject * pChangedObject = pModel;

  switch (property)
    {
    case cpsapiProperty::Type::UNIT:
      return pModel->getUnits();
      break;

    case cpsapiProperty::Type::VOLUME_UNIT:
      return pModel->getVolumeUnit();
      break;

    case cpsapiProperty::Type::AREA_UNIT:
      return pModel->getAreaUnit();
      break;

    case cpsapiProperty::Type::LENGTH_UNIT:
      return pModel->getLengthUnit();
      break;

    case cpsapiProperty::Type::TIME_UNIT:
      return pModel->getTimeUnit();
      break;

    case cpsapiProperty::Type::QUANTITY_UNIT:
      return pModel->getQuantityUnit();
      break;

    case cpsapiProperty::Type::MODEL_TYPE:
      return pModel->CModel::ModelTypeNames[pModel->getModelType()];
      break;

    case cpsapiProperty::Type::AVOGADRO_NUMBER:
      return pModel->getAvogadro();
      break;

    default:
      break;
    }

  return CDataValue();
}
