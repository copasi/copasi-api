// BEGIN: Copyright 
// Copyright (C) 2022 by Pedro Mendes, Rector and Visitors of the 
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

#include "cpsapi/model/cpsapiKineticLawVariable.h"
#include "cpsapi/model/cpsapiTransaction.h"
#include "cpsapi/model/cpsapiModel.h"

#pragma GCC diagnostic push
#include <copasi/model/CModel.h>
#include <copasi/model/CReaction.h>
#pragma GCC diagnostic pop

CPSAPI_NAMESPACE_USE

// static
const cpsapiKineticLawVariable::Properties cpsapiKineticLawVariable::SupportedProperties =
  {
    cpsapiProperty::PARAMETER_ROLE,
    cpsapiProperty::PARAMETER_VALUE,
    cpsapiProperty::PARAMETER_MAPPING
  };

// static
const cpsapiKineticLawVariable::Properties cpsapiKineticLawVariable::HiddenProperties = 
  {};

// static
const cpsapiKineticLawVariable::References cpsapiKineticLawVariable::SupportedReferences =
  {
    cpsapiProperty::PARAMETER_VALUE
  };


// static
const cpsapiKineticLawVariable::References cpsapiKineticLawVariable::HiddenReferences
  {};

cpsapiKineticLawVariable::cpsapiKineticLawVariable(wrapped * pWrapped)
  : base(pWrapped, cpsapiObjectData::Type::ReactionParameter)
{
  cpsapiObjectData::assertDataType< cpsapiKineticLawVariable >(mpData);
}

// virtual
cpsapiKineticLawVariable::~cpsapiKineticLawVariable()
{}

// virtual 
bool cpsapiKineticLawVariable::isValid() const
{
  return (base::isValid()
          && dynamic_cast< CReaction * >(WRAPPED->getObjectParent()) != nullptr
          && WRAPPED->getObjectName() != "@");
}

bool cpsapiKineticLawVariable::setProperty(const cpsapiKineticLawVariable::Property & property, const cpsapiData & value, const CCore::Framework & framework)
{
  return setProperty(static_cast< const cpsapiProperty::Type >(property), value, framework);
}

cpsapiData cpsapiKineticLawVariable::getProperty(const cpsapiKineticLawVariable::Property & property, const CCore::Framework & framework) const
{
  return getProperty(static_cast< const cpsapiProperty::Type >(property), framework);
}

// virtual
bool cpsapiKineticLawVariable::setProperty(const cpsapiProperty::Type & property, const cpsapiData & value, const CCore::Framework & framework)
{
  if (!isValid()
      || isHiddenProperty< cpsapiKineticLawVariable >(property))
    return false;

  if (!isImplementedProperty< cpsapiKineticLawVariable >(property))
    return base::setProperty(property, value, CCore::Framework::__SIZE);

  CCore::Framework Framework(framework);

  CReaction * pReaction = static_cast< CReaction * >(WRAPPED->getObjectParent());

  const CFunctionParameter * pParameter;
  size_t Index = pReaction->getFunctionParameters().findParameterByName(WRAPPED->getObjectName(), &pParameter);

  if (Index == C_INVALID_INDEX
      || pParameter ->getType() >= CFunctionParameter::DataType::VINT32)
    return false;

  bool success = cpsapiTransaction::endStructureChange(static_cast< CModel * >(pReaction->getObjectAncestor("Model")));

  switch (property)
    {
    case cpsapiProperty::PARAMETER_ROLE:
      success = false; // The role of a reaction parameter must not be changed
      break;

    case cpsapiProperty::PARAMETER_VALUE:
      if (WRAPPED->mpMappedObject != nullptr
          && WRAPPED->mpMappedObject->getType() == cpsapiObjectData::Type::Parameter)
        success &= static_cast< CCopasiParameter * >(**WRAPPED->mpMappedObject)->setValue(value.toDouble());
      break;

    case cpsapiProperty::PARAMETER_MAPPING:
      switch (pParameter->getUsage())
        {
        case CFunctionParameter::Role::SUBSTRATE:
        case CFunctionParameter::Role::PRODUCT:
        case CFunctionParameter::Role::MODIFIER:
          if (value.toObject< cpsapiSpecies >()
              && value.toObject< cpsapiSpecies >() != *WRAPPED->mpMappedObject)
            success &= pReaction->setParameterObject(Index, *value.toObject< cpsapiSpecies >());
          else
            success = false;

          break;

        case CFunctionParameter::Role::PARAMETER:
          if (value.toObject< cpsapiGlobalQuantity >()
              && value.toObject< cpsapiGlobalQuantity >() != *WRAPPED->mpMappedObject)
            success &= pReaction->setParameterObject(Index, *value.toObject< cpsapiGlobalQuantity >());
          else if (value.getType() != cpsapiData::Type::Object)
            success &= pReaction->setParameterObject(Index, pReaction->getParameters().getParameter(WRAPPED->getObjectName()));
          else
            success = false;

          break;

        case CFunctionParameter::Role::VOLUME:
          if (value.toObject< cpsapiCompartment >()
              && value.toObject< cpsapiCompartment >() != *WRAPPED->mpMappedObject)
            success &= pReaction->setParameterObject(Index, *value.toObject< cpsapiCompartment >());
          else
            success = false;

          break;

        case CFunctionParameter::Role::TIME:
          if (value.toObject< cpsapiModel >()
              && value.toObject< cpsapiModel >() != *WRAPPED->mpMappedObject)
            success &= pReaction->setParameterObject(Index, *value.toObject< cpsapiModel >());
          else
            success = false;

          break;

        default:
          success = false;
          break;
        }
      break;

      default:
        success = false;
        break;
    }

  if (success
      && property == cpsapiProperty::PARAMETER_MAPPING)
    WRAPPED->updateMappedObject();

  return success && cpsapiTransaction::synchronize(pReaction, Framework);
}

// virtual
cpsapiData cpsapiKineticLawVariable::getProperty(const cpsapiProperty::Type & property, const CCore::Framework & /* framework */) const
{
  if (!isValid()
      || isHiddenProperty< cpsapiKineticLawVariable >(property))
    return cpsapiData();

  if (!isImplementedProperty< cpsapiKineticLawVariable >(property))
    return base::getProperty(property, CCore::Framework::__SIZE);

  CReaction * pReaction = static_cast< CReaction * >(WRAPPED->getObjectParent());

  const CFunctionParameter * pParameter;
  size_t Index = pReaction->getFunctionParameters().findParameterByName(WRAPPED->getObjectName(), &pParameter);

  if (Index == C_INVALID_INDEX)
    return cpsapiData();

  switch (property)
    {
    case cpsapiProperty::PARAMETER_ROLE:
      if (pParameter != nullptr)
        return CFunctionParameter::RoleNameXML[pParameter->getUsage()];
      break;

    case cpsapiProperty::PARAMETER_VALUE:
      if (WRAPPED->mpMappedObject != nullptr
          && WRAPPED->mpMappedObject->getType() == cpsapiObjectData::Type::Parameter)
        return static_cast< CCopasiParameter * >(**WRAPPED->mpMappedObject)->getValue< C_FLOAT64 >();
      break;

    case cpsapiProperty::PARAMETER_MAPPING:
      if (pParameter != nullptr
          && WRAPPED->mpMappedObject != nullptr)
        switch (pParameter->getUsage())
          {
          case CFunctionParameter::Role::SUBSTRATE:
          case CFunctionParameter::Role::PRODUCT:
          case CFunctionParameter::Role::MODIFIER:
          case CFunctionParameter::Role::PARAMETER:
          case CFunctionParameter::Role::VOLUME:
          case CFunctionParameter::Role::TIME:
            return *WRAPPED->mpMappedObject;
            break;

          default:
            break;
          }
      break;

    default:
      break;
    }

  return cpsapiData();
}

CCommonName cpsapiKineticLawVariable::getDataCN(const cpsapiKineticLawVariable::Reference & reference, const CCore::Framework & framework) const
{
  return getDataCN(static_cast< const cpsapiReference::Type >(reference), framework);
}

// virtual 
CCommonName cpsapiKineticLawVariable::getDataCN(const cpsapiReference::Type & reference, const CCore::Framework & framework) const
{
  if (!isValid()
      || isHiddenReference< cpsapiKineticLawVariable >(reference))
    return Invalid;

  if (!isImplementedReference< cpsapiKineticLawVariable >(reference))
    return base::getDataCN(reference, framework);

  switch (reference)
    {
    case cpsapiReference::Type::PARAMETER_VALUE:
      if (WRAPPED->mpMappedObject != nullptr
          && WRAPPED->mpMappedObject->getType() == cpsapiObjectData::Type::Parameter)
        return static_cast< CCopasiParameter * >(**WRAPPED->mpMappedObject)->getValueReference()->getCN();

      break;

    default:
      break;
    }

  return Invalid;
}

// static
cpsapiKineticLawVariable::KineticLawVariable * cpsapiKineticLawVariable::KineticLawVariable::fromData(const CData & /* data */, CUndoObjectInterface * /* pParent */)
{
  return nullptr;
}

cpsapiKineticLawVariable::KineticLawVariable::KineticLawVariable(CReaction * pReaction, const std::string & name)
  : CDataObject(name.empty() ? "@" : name, pReaction, "Variable")
  , mpMappedObject(nullptr)
{
  updateMappedObject();
}

cpsapiKineticLawVariable::KineticLawVariable::KineticLawVariable(const KineticLawVariable & src, CDataContainer * pParent)
  : CDataObject(src, pParent)
  , mpMappedObject(src.mpMappedObject != nullptr ? cpsapiFactory::copy(*src.mpMappedObject) : nullptr)
{}

// virtual
cpsapiKineticLawVariable::KineticLawVariable::~KineticLawVariable()
{
  if (mpMappedObject != nullptr)
    delete mpMappedObject;
}

void cpsapiKineticLawVariable::KineticLawVariable::updateMappedObject()
{
  CReaction * pReaction = dynamic_cast< CReaction * >(getObjectParent());

  if (pReaction == nullptr
      || getObjectName() == "@")
    {
      if (mpMappedObject != nullptr)
        {
          delete mpMappedObject;
          mpMappedObject = nullptr;
        }

      return;
    }

  const std::vector< const CDataObject * > & Objects = pReaction->getParameterObjects(getObjectName());

  if (Objects.size() != 1
      || Objects[0] == nullptr)
    {
      if (mpMappedObject != nullptr)
        {
          delete mpMappedObject;
          mpMappedObject = nullptr;
        }

      return;
    }

  if (mpMappedObject == nullptr)
    {
      mpMappedObject = cpsapiFactory::create(const_cast< CDataObject * >(Objects[0]));
    }
  else if (**mpMappedObject != Objects[0])
    {
      delete mpMappedObject;
      mpMappedObject = cpsapiFactory::create(const_cast< CDataObject * >(Objects[0]));
    }
}


