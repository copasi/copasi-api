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
  if (dynamic_cast< CCompartment * >(mpObject) == nullptr)
    mpObject = nullptr;

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
  if (mpObject == nullptr)
    return nullptr;
    
  return cpsapiSpecies(static_cast< CCompartment * >(mpObject)->createMetabolite(name));
}

bool cpsapiCompartment::deleteSpecies(const std::string & name)
{
  if (mpObject == nullptr)
    return false;

  CDataVectorNS< CMetab > & Species = static_cast< CCompartment * >(mpObject)->getMetabolites();

  size_t Index = Species.getIndex(name);
  if (Index == C_INVALID_INDEX)
    return false;

  CMetab * pSpecies = &Species[Index];
  pdelete(pSpecies);

  return true;
}

cpsapiSpecies cpsapiCompartment::species(const std::string & name)
{
  if (name.empty())
    return cpsapiSpecies(mpDefaultSpecies);

  if (mpObject == nullptr)
    return nullptr;

  CDataVectorNS< CMetab > & Species = static_cast< CCompartment * >(mpObject)->getMetabolites();


  size_t Index = Species.getIndex(name);

  if (Index == C_INVALID_INDEX)
    return nullptr;
  
  mpDefaultSpecies = &Species[Index];
  return mpDefaultSpecies;
}

std::vector< cpsapiSpecies > cpsapiCompartment::getSpecies() const
{
  CDataVectorNS< CMetab > & Species = static_cast< CCompartment * >(mpObject)->getMetabolites();
  std::vector< cpsapiSpecies > Result(Species.size());

  std::vector< cpsapiSpecies >::iterator it = Result.begin();
  std::vector< cpsapiSpecies >::iterator end = Result.end();
  CDataVectorNS< CMetab >::iterator itSrc = Species.begin();

  for (; it != end; ++it, ++itSrc)
    *it = &*itSrc;

  return Result;
}

bool cpsapiCompartment::set(const cpsapiCompartment::Property & property, const CDataValue & value, const CCore::Framework & framework)
{
  return set(static_cast< const CData::Property >(property), value, framework);
}

// virtual
bool cpsapiCompartment::set(const CData::Property & property, const CDataValue & value, const CCore::Framework & framework)
{
  if (mpObject == nullptr)
    return false;

  if (!isValidProperty(property))
    return base::set(property, value, CCore::Framework::__SIZE);

  CCore::Framework Framework(framework);

  CCompartment * pCompartment = static_cast< CCompartment * >(mpObject);
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
