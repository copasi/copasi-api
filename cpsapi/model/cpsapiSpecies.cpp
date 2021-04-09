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

#include "cpsapi/model/cpsapiSpecies.h"

#include "copasi/model/CMetab.h"

CPSAPI_NAMESPACE_USE

// static
cpsapiSpecies::Properties cpsapiSpecies::SupportedProperties =
  {
    CData::Property::INITIAL_VALUE
  };

cpsapiSpecies::cpsapiSpecies(CMetab * pSpecies)
  : base(pSpecies)
{
  if (dynamic_cast< CMetab * >(mpObject) == nullptr)
    mpObject = nullptr;

  mpSupportedProperties = &SupportedProperties;
}

cpsapiSpecies::cpsapiSpecies(const cpsapiSpecies & src)
  : base(src)
{}

// virtual
cpsapiSpecies::~cpsapiSpecies()
{}

bool cpsapiSpecies::set(const cpsapiSpecies::Property & property, const CDataValue & value, const CCore::Framework & framework)
{
  return set(static_cast< const CData::Property >(property), value, framework);
}

// virtual
bool cpsapiSpecies::set(const CData::Property & property, const CDataValue & value, const CCore::Framework & framework)
{
  if (mpObject == nullptr)
    return false;

  if (!isValidProperty(property))
    return base::set(property, value, CCore::Framework::__SIZE);

  CCore::Framework Framework(framework);

  CMetab * pSpecies = static_cast< CMetab * >(mpObject);
  bool success = false;

  CDataObject * pChangedObject = pSpecies;

  switch (property)
    {
    case CData::Property::INITIAL_VALUE:
      if (Framework == CCore::Framework::__SIZE)
        Framework = CCore::Framework::Concentration;

      if (value.getType() == CData::Type::DOUBLE)
        switch (Framework)
          {
          case CCore::Framework::Concentration:
            pChangedObject = pSpecies->getInitialConcentrationReference();
            pSpecies->setInitialConcentration(value.toDouble());
            success = true;
            break;

          case CCore::Framework::ParticleNumbers:
            pChangedObject = pSpecies->getInitialValueReference();
            pSpecies->setInitialValue(value.toDouble());
            success = true;
            break;
          }

      break;

    default:
      break;
    }

  return success && cpsapiTransaction::synchronize(pChangedObject, Framework);
}

