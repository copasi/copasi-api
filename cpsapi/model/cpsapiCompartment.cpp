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

#include "cpsapi/model/cpsapiCompartment.h"
#include "cpsapi/model/cpsapiSpecies.h"
#include "cpsapi/model/cpsapiModel.h"

#include "copasi/model/CCompartment.h"

CPSAPI_NAMESPACE_USE

// static
cpsapiCompartment::Properties cpsapiCompartment::SupportedProperties =
  {
    CData::Property::DIMENSIONALITY,
    CData::Property::INITIAL_VALUE
  };

cpsapiCompartment::cpsapiCompartment(CCompartment * pCompartment)
  : base(pCompartment)
  , mpDefaultSpecies(nullptr)
{
  mpSupportedProperties = &SupportedProperties;
}

cpsapiCompartment::cpsapiCompartment(const cpsapiCompartment & src)
  : base(src)
  , mpDefaultSpecies(src.mpDefaultSpecies)
{}

// virtual
cpsapiCompartment::~cpsapiCompartment()
{}

cpsapiSpecies cpsapiCompartment::addSpecies(const std::string & name)
{
  if (!mObject)
    return nullptr;
  
  cpsapiSpecies Species = cpsapiModel(static_cast< CCompartment * >(*mObject)->getModel()).addSpecies(name, mObject->getObjectName());

  if (Species)
    mpDefaultSpecies = static_cast< CMetab * >(Species.getObject());

  return Species;
}

bool cpsapiCompartment::deleteSpecies(const std::string & name)
{
  CMetab * pSpecies = __species(name);

  if (pSpecies == nullptr)
    return false;
  
  if (mpDefaultSpecies == pSpecies)
    mpDefaultSpecies = nullptr;
  
  cpsapiModel(static_cast< CCompartment * >(*mObject)->getModel()).deleteAllDependents(pSpecies);
  cpsapiPointer::deleted(pSpecies);
  pdelete(pSpecies);

  return true;
}

cpsapiSpecies cpsapiCompartment::species(const std::string & name)
{
  CMetab * pSpecies = __species(name);

  if (pSpecies != nullptr)
    mpDefaultSpecies = pSpecies;

  return pSpecies;
}

std::vector< cpsapiSpecies > cpsapiCompartment::getSpecies() const
{
  if (!mObject)
    return std::vector< cpsapiSpecies >();

  return convertVector< cpsapiSpecies >(static_cast< CCompartment * >(*mObject)->getMetabolites());
}

CMetab * cpsapiCompartment::__species(const std::string & name) const
{ 
  if (!mObject)
    return nullptr;
    
  if (name.empty())
    return mpDefaultSpecies;

  size_t Index = static_cast< CCompartment * >(*mObject)->getMetabolites().getIndex(name);

  if (Index == C_INVALID_INDEX)
    return nullptr;

  return  &static_cast< CCompartment * >(*mObject)->getMetabolites()[Index];
}

bool cpsapiCompartment::set(const cpsapiCompartment::Property & property, const CDataValue & value, const CCore::Framework & framework)
{
  return set(static_cast< const CData::Property >(property), value, framework);
}

// virtual
bool cpsapiCompartment::set(const CData::Property & property, const CDataValue & value, const CCore::Framework & framework)
{
  if (!mObject)
    return false;

  if (!isValidProperty(property))
    return base::set(property, value, CCore::Framework::__SIZE);

  CCore::Framework Framework(framework);

  CCompartment * pCompartment = static_cast< CCompartment * >(*mObject);
  bool success = false;

  CDataObject * pChangedObject = pCompartment;

  switch (property)
    {
    case CData::Property::DIMENSIONALITY:
      Framework = CCore::Framework::__SIZE;

      if (value.getType() == CDataValue::Type::UINT)
        success = pCompartment->setDimensionality(value.toUint());
      else if (value.getType() == CDataValue::Type::INT)
        success = pCompartment->setDimensionality(value.toInt());

      break;

    case CData::Property::INITIAL_VALUE:
      if (Framework == CCore::Framework::__SIZE)
        Framework = CCore::Framework::Concentration;

      success = base::set(property, value, Framework);
      break;

    default:
      break;
    }

  return success && cpsapiTransaction::synchronize(pChangedObject, Framework);
}
