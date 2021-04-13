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

CDataValue cpsapiSpecies::get(const Property & property, const CCore::Framework & framework) const
{
  return get(static_cast< const CData::Property >(property), framework);
}

// virtual
bool cpsapiSpecies::set(const CData::Property & property, const CDataValue & value, const CCore::Framework & framework)
{
  if (*mpObject == nullptr)
    return false;

  if (!isValidProperty(property))
    return base::set(property, value, CCore::Framework::__SIZE);

  CCore::Framework Framework(framework);

  CMetab * pSpecies = static_cast< CMetab * >(*mpObject);
  CDataObject * pChangedObject = pSpecies;
  bool success = cpsapiTransaction::endStructureChange(pSpecies->getModel());

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

// virtual 
CDataValue cpsapiSpecies::get(const CData::Property & property, const CCore::Framework & framework) const
{
  if (*mpObject == nullptr)
    return CDataValue();

  if (!isValidProperty(property))
    return base::get(property, CCore::Framework::__SIZE);

  CMetab * pSpecies = static_cast< CMetab * >(*mpObject);

  switch (property)
    {
    case CData::Property::INITIAL_VALUE:
      if (framework == CCore::Framework::ParticleNumbers)
        return base::get(property, framework);
      
      return pSpecies->getInitialConcentration();
      break;

    default:
      break;
    }

  return CDataValue();
}