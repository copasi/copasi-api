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

#include "cpsapi/model/cpsapiCompartment.h"
#include "cpsapi/model/cpsapiSpecies.h"
#include "cpsapi/model/cpsapiModel.h"
#include "cpsapi/model/cpsapiTransaction.h"

#include "copasi/model/CCompartment.h"

CPSAPI_NAMESPACE_USE

// static
const cpsapiCompartment::Properties cpsapiCompartment::SupportedProperties =
  {
    cpsapiProperty::Type::DIMENSIONALITY,
    cpsapiProperty::Type::INITIAL_VALUE,
    cpsapiProperty::Type::UNIT // READ ONLY
  };

cpsapiCompartment::cpsapiCompartment(CCompartment * pCompartment)
  : base(pCompartment, Type::cpsapiCompartment)
  , mpDefaultSpecies()
{
  for (cpsapiObject * pReference : references())
    if (this != pReference)
      {
        mpDefaultSpecies = static_cast< cpsapiCompartment * >(pReference)->mpDefaultSpecies;
        break;
      }

  if (!mpDefaultSpecies)
    mpDefaultSpecies = std::make_shared< cpsapiSpecies >(nullptr);
}

cpsapiCompartment::cpsapiCompartment(const cpsapiCompartment & src)
  : base(src)
  , mpDefaultSpecies(src.mpDefaultSpecies)
{}

// virtual
cpsapiCompartment::~cpsapiCompartment()
{}

// virtual 
void cpsapiCompartment::accept(cpsapiVisitor & visitor)
{
  if (!operator bool())
    return;

  visitor.visit(this, Type::cpsapiCompartment);
  base::accept(visitor);
}

cpsapiSpecies cpsapiCompartment::addSpecies(const std::string & name)
{
  if (!operator bool())
    return nullptr;
  
  cpsapiSpecies Species = cpsapiModel(static_cast< CCompartment * >(getObject())->getModel()).addSpecies(name, getObject()->getObjectName());

  if (!Species)
    return nullptr;

  if (mpDefaultSpecies->getObject() != Species.getObject())
    updateDefaultSpecies(Species);

  return *mpDefaultSpecies;
}

bool cpsapiCompartment::deleteSpecies(const std::string & name)
{
  CMetab * pSpecies = static_cast< CMetab * >(__species(name).getObject());

  if (pSpecies == nullptr)
    return false;
  
  if (mpDefaultSpecies->getObject() == pSpecies)
    updateDefaultSpecies(nullptr);
  
  cpsapiTransaction::beginStructureChange(static_cast< CCompartment * >(getObject())->getModel());

  cpsapiModel(static_cast< CCompartment * >(getObject())->getModel()).deleteAllDependents(pSpecies);
  deleted(pSpecies);
  pdelete(pSpecies);

  return true;
}

cpsapiSpecies cpsapiCompartment::species(const std::string & name)
{
  cpsapiSpecies Species(__species(name));

  if (!Species)
    return Species;

  if (mpDefaultSpecies->getObject() != Species.getObject())
    updateDefaultSpecies(Species);

  return *mpDefaultSpecies;
}

std::vector< cpsapiSpecies > cpsapiCompartment::getSpecies() const
{
  if (!operator bool())
    return std::vector< cpsapiSpecies >();

  return convertVector< cpsapiSpecies >(static_cast< CCompartment * >(getObject())->getMetabolites());
}

cpsapiSpecies cpsapiCompartment::__species(const std::string & name) const
{ 
  if (!operator bool())
    return nullptr;
    
  if (name.empty())
    return *mpDefaultSpecies;

  size_t Index = static_cast< CCompartment * >(getObject())->getMetabolites().getIndex(name);

  if (Index == C_INVALID_INDEX)
    return nullptr;

  return  &static_cast< CCompartment * >(getObject())->getMetabolites()[Index];
}

void cpsapiCompartment::updateDefaultSpecies(const cpsapiSpecies & species)
{
  std::shared_ptr< cpsapiSpecies > Default = std::make_shared< cpsapiSpecies >(species);

  for (cpsapiObject * pReference : references())
    {
      static_cast< cpsapiCompartment * >(pReference)->mpDefaultSpecies = Default;
    }
}

bool cpsapiCompartment::setProperty(const cpsapiCompartment::Property & property, const CDataValue & value, const CCore::Framework & framework)
{
  return setProperty(static_cast< const cpsapiProperty::Type >(property), value, framework);
}

CDataValue cpsapiCompartment::getProperty(const cpsapiCompartment::Property & property, const CCore::Framework & framework) const
{
  return getProperty(static_cast< const cpsapiProperty::Type >(property), framework);
}

// virtual
bool cpsapiCompartment::setProperty(const cpsapiProperty::Type & property, const CDataValue & value, const CCore::Framework & framework)
{
  if (!operator bool())
    return false;

  if (!isValidProperty<cpsapiCompartment>(property))
    return base::setProperty(property, value, CCore::Framework::__SIZE);

  CCore::Framework Framework(framework);

  CCompartment * pCompartment = static_cast< CCompartment * >(getObject());
  CDataObject * pChangedObject = pCompartment;
  bool success = cpsapiTransaction::endStructureChange(pCompartment->getModel());

  switch (property)
    {
    case cpsapiProperty::Type::DIMENSIONALITY:
      Framework = CCore::Framework::__SIZE;
      
      if (value.getType() == CDataValue::Type::UINT)
        success = pCompartment->setDimensionality(value.toUint());
      else if (value.getType() == CDataValue::Type::INT)
        success = pCompartment->setDimensionality(value.toInt());

      break;

    case cpsapiProperty::Type::INITIAL_VALUE:
      if (Framework == CCore::Framework::__SIZE)
        Framework = CCore::Framework::Concentration;

      if (value.getType() == CDataValue::Type::DOUBLE)
        {
          pChangedObject = pCompartment->getInitialValueReference();
          pCompartment->setInitialValue(value.toDouble());
          success = true;
        }
      else
        success = false;
        
      break;

    case cpsapiProperty::Type::UNIT:
      success = false;
      break;

    default:
      break;
    }

  return success && cpsapiTransaction::synchronize(pChangedObject, Framework);
}

// virtual
CDataValue cpsapiCompartment::getProperty(const cpsapiProperty::Type & property, const CCore::Framework & framework) const
{
  if (!operator bool())
    return CDataValue();

  if (!isValidProperty<cpsapiCompartment>(property))
    return base::getProperty(property, CCore::Framework::__SIZE);

  CCompartment * pCompartment = static_cast< CCompartment * >(getObject());

  switch (property)
    {
    case cpsapiProperty::Type::DIMENSIONALITY:
      return pCompartment->getDimensionality();
      break;

    case cpsapiProperty::Type::INITIAL_VALUE:
      return pCompartment->getInitialValue();
      break;

    case cpsapiProperty::Type::UNIT:
      return pCompartment->getUnits();
      break;
      
    default:
      break;
    }

  return CDataValue();
}
