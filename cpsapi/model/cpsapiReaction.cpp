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

#include "cpsapi/model/cpsapiReaction.h"
#include "cpsapi/model/cpsapiTransaction.h"

#pragma GCC diagnostic push
#include <copasi/model/CReaction.h>
#include <copasi/model/CReactionInterface.h>
#include <copasi/model/CModel.h>
#pragma GCC diagnostic pop

CPSAPI_NAMESPACE_USE

// static
const cpsapiReaction::Properties cpsapiReaction::SupportedProperties =
  {
    cpsapiProperty::CHEMICAL_EQUATION,
    cpsapiProperty::KINETIC_LAW,
    cpsapiProperty::KINETIC_LAW_EXPRESSION,
    cpsapiProperty::KINETIC_LAW_UNIT_TYPE,
    cpsapiProperty::SCALING_COMPARTMENT,
    cpsapiProperty::ADD_NOISE,
    cpsapiProperty::NOISE_EXPRESSION
  };

// static
const cpsapiReaction::Properties cpsapiReaction::HiddenProperties = {};

// static
const cpsapiReaction::References cpsapiReaction::SupportedReferences =
  {
    cpsapiReference::Type::FLUX,
    cpsapiReference::Type::PARTICLE_FLUX,
    cpsapiReference::Type::INITIAL_FLUX,
    cpsapiReference::Type::INITIAL_PARTICLE_FLUX,
    cpsapiReference::Type::NOISE,
    cpsapiReference::Type::PROPENSITY
  };

// static
const cpsapiReaction::References cpsapiReaction::HiddenReferences = {};

cpsapiReaction::cpsapiReaction(cpsapiReaction::wrapped * pWrapped)
  : base(pWrapped, cpsapiObjectData::Type::Reaction)
{
  cpsapiObjectData::assertDataType< cpsapiReaction >(mpData);
}

// virtual
cpsapiReaction::~cpsapiReaction()
{}

cpsapiKineticLawVariable::KineticLawVariable * cpsapiReaction::assertVariable(const std::string & name)
{
  VariableManager::iterator found = DATA->mVariableManager.find(name);

  if (found == DATA->mVariableManager.end())
    found = DATA->mVariableManager.insert(std::make_pair(name, cpsapiKineticLawVariable(new cpsapiKineticLawVariable::KineticLawVariable(WRAPPED, name)))).first;

  return static_cast< cpsapiKineticLawVariable::KineticLawVariable * >(*found->second);
}

cpsapiKineticLawVariable cpsapiReaction::variable(const std::string & name)
{
  if (!isValid())
    return nullptr;

  if (!name.empty() &&
      name != DATA->mDefaultVariable.getProperty(cpsapiKineticLawVariable::Property::NAME).toString())
    DATA->mDefaultVariable = cpsapiKineticLawVariable(assertVariable(name));
    
  return DATA->mDefaultVariable;
}

cpsapiVector< cpsapiKineticLawVariable > cpsapiReaction::variables()
{
  if (DATA->mpVector == nullptr)
    DATA->mpVector = new VariableVector();
  else
    DATA->mpVector->clear();

  if (operator bool())
    {
      wrapped * pWrapped = WRAPPED;

      for (const CFunctionParameter & Parameter : pWrapped->getFunctionParameters())
        DATA->mpVector->add(assertVariable(Parameter.getObjectName()), false);
    }

  return cpsapiVector< cpsapiKineticLawVariable >(DATA->mpVector);
}

bool cpsapiReaction::setProperty(const cpsapiReaction::Property & property, const cpsapiData & value, const CCore::Framework & framework)
{
  return setProperty(static_cast< const cpsapiProperty::Type >(property), value, framework);
}

cpsapiData cpsapiReaction::getProperty(const cpsapiReaction::Property & property, const CCore::Framework & framework) const
{
  return getProperty(static_cast< const cpsapiProperty::Type >(property), framework);
}

// virtual
bool cpsapiReaction::setProperty(const cpsapiProperty::Type & property, const cpsapiData & value, const CCore::Framework & framework)
{
  if (!isValid())
    return false;

  if (!isImplementedProperty< cpsapiReaction >(property))
    return base::setProperty(property, value, CCore::Framework::__SIZE);

  CCore::Framework Framework(framework);

  wrapped * pWrapped = WRAPPED;
  CDataObject * pChangedObject = pWrapped;
  bool success = cpsapiTransaction::endStructureChange(static_cast< CModel * >(pWrapped->getObjectAncestor("Model")));

  CReactionInterface Reaction;
  Reaction.init(*pWrapped);
  bool isValid = Reaction.isValid();

  switch (property)
    {
    case cpsapiProperty::CHEMICAL_EQUATION:
      Reaction.setChemEqString(value.toString(), Reaction.getFunctionName());
      success &= Reaction.createMetabolites();
      break;

    case cpsapiProperty::KINETIC_LAW:
      Reaction.setFunctionAndDoMapping(value.toString());
      break;

    case cpsapiProperty::KINETIC_LAW_EXPRESSION:
      {
        CFunction * pFunction = pWrapped->createFunctionFromExpression(value.toString());

        if (pFunction != nullptr)
          Reaction.setFunctionAndDoMapping(pFunction->getObjectName());
        else
          success = false;
      }
      break;

    case cpsapiProperty::KINETIC_LAW_UNIT_TYPE:
      Reaction.setKineticLawUnitType(CReaction::KineticLawUnitTypeName.toEnum(value.toString()));
      break;

    case cpsapiProperty::SCALING_COMPARTMENT:
      Reaction.setScalingCompartment(value.toString());
      break;

    case cpsapiProperty::ADD_NOISE:
      Reaction.setHasNoise(value.toBool());
      break;

    case cpsapiProperty::NOISE_EXPRESSION:
      Reaction.setNoiseExpression(value.toString());
      break;

    default:
      break;
    }

  isValid = (!isValid || Reaction.isValid());
  Reaction.writeBackToReaction();

  if (DATA->mpVector != nullptr)
    variables();
    
  return success && isValid && cpsapiTransaction::synchronize(pChangedObject, Framework);
}

// virtual
cpsapiData cpsapiReaction::getProperty(const cpsapiProperty::Type & property, const CCore::Framework & /* framework */) const
{
  if (!isValid())
    return cpsapiData();

  if (!isImplementedProperty<cpsapiReaction>(property))
    return base::getProperty(property, CCore::Framework::__SIZE);

  wrapped * pWrapped = WRAPPED;
  CReactionInterface Reaction;
  Reaction.init(*pWrapped);

  switch (property)
    {
    case cpsapiProperty::CHEMICAL_EQUATION:
      return Reaction.getChemEqString();
      break;

    case cpsapiProperty::KINETIC_LAW:
      return Reaction.getFunctionName();
      break;

    case cpsapiProperty::KINETIC_LAW_EXPRESSION:
      return Reaction.getFunction().getInfix();
      break;

    case cpsapiProperty::KINETIC_LAW_UNIT_TYPE:
      return CReaction::KineticLawUnitTypeName[Reaction.getKineticLawUnitType()];
      break;

    case cpsapiProperty::SCALING_COMPARTMENT:
      return Reaction.getScalingCompartment();
      break;

    case cpsapiProperty::ADD_NOISE:
      return Reaction.hasNoise();
      break;

    case cpsapiProperty::NOISE_EXPRESSION:
      return Reaction.getNoiseExpression();
      break;

    default:
      break;
    }

  return cpsapiData();
}

CCommonName cpsapiReaction::getDataCN(const cpsapiReaction::Reference & reference, const CCore::Framework & framework) const
{
  return getDataCN(static_cast< cpsapiReference::Type >(reference), framework);
}

// virtual
CCommonName cpsapiReaction::getDataCN(const cpsapiReference::Type & reference, const CCore::Framework & framework) const
{
  if (!isValid()
      || isHiddenReference< cpsapiReaction >(reference))
    return Invalid;

  if (!isImplementedReference< cpsapiReaction >(reference))
    return base::getDataCN(reference, framework);

  switch (reference)
    {
    case cpsapiReference::Type::FLUX:
      break;

    case cpsapiReference::Type::PARTICLE_FLUX:
      break;

    case cpsapiReference::Type::INITIAL_FLUX:
      break;

    case cpsapiReference::Type::INITIAL_PARTICLE_FLUX:
      break;

    case cpsapiReference::Type::NOISE:
      break;

    case cpsapiReference::Type::PROPENSITY:
      break;

    default:
      break;
    }

  return Invalid;
}
