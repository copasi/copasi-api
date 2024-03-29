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

#include "cpsapi/model/cpsapiSpecies.h"
#include "cpsapi/model/cpsapiTransaction.h"

#pragma GCC diagnostic push
#include <copasi/model/CMetab.h>
#pragma GCC diagnostic pop

CPSAPI_NAMESPACE_USE

// static
const cpsapiSpecies::Properties cpsapiSpecies::SupportedProperties =
  {
    cpsapiProperty::INITIAL_VALUE,
    cpsapiProperty::UNIT // READ ONLY
  };

cpsapiSpecies::cpsapiSpecies(wrapped * pWrapped)
  : base(pWrapped, cpsapiObjectData::Type::Species)
{}

// virtual
cpsapiSpecies::~cpsapiSpecies()
{}

bool cpsapiSpecies::setProperty(const cpsapiSpecies::Property & property, const cpsapiData & value, const CCore::Framework & framework)
{
  return setProperty(static_cast< const cpsapiProperty::Type >(property), value, framework);
}

cpsapiData cpsapiSpecies::getProperty(const Property & property, const CCore::Framework & framework) const
{
  return getProperty(static_cast< const cpsapiProperty::Type >(property), framework);
}

// virtual
bool cpsapiSpecies::setProperty(const cpsapiProperty::Type & property, const cpsapiData & value, const CCore::Framework & framework)
{
  if (operator=(nullptr))
    return false;

  if (!isImplementedProperty<cpsapiSpecies>(property))
    return base::setProperty(property, value, CCore::Framework::__SIZE);

  CCore::Framework Framework(framework);

  wrapped * pWrapped = WRAPPED;
  CDataObject * pChangedObject = pWrapped;
  bool success = cpsapiTransaction::endStructureChange(pWrapped->getModel());

  switch (property)
    {
    case cpsapiProperty::INITIAL_VALUE:
      if (Framework == CCore::Framework::__SIZE)
        Framework = CCore::Framework::Concentration;

      if (value.getType() == cpsapiData::Type::Double)
        switch (Framework)
          {
          case CCore::Framework::Concentration:
            pChangedObject = pWrapped->getInitialConcentrationReference();
            pWrapped->setInitialConcentration(value.toDouble());
            success = true;
            break;

          case CCore::Framework::ParticleNumbers:
            pChangedObject = pWrapped->getInitialValueReference();
            pWrapped->setInitialValue(value.toDouble());
            success = true;
            break;

          case CCore::Framework::__SIZE:
            success = false;
            break;
          }

      break;

    case cpsapiProperty::UNIT:
      success = false;
      break;

    default:
      break;
    }

  return success && cpsapiTransaction::synchronize(pChangedObject, Framework);
}

// virtual 
cpsapiData cpsapiSpecies::getProperty(const cpsapiProperty::Type & property, const CCore::Framework & framework) const
{
  if (!isValid())
    return cpsapiData();

  if (!isImplementedProperty<cpsapiSpecies>(property))
    return base::getProperty(property, CCore::Framework::__SIZE);

  wrapped * pWrapped = WRAPPED;

  switch (property)
    {
    case cpsapiProperty::INITIAL_VALUE:
      if (framework == CCore::Framework::ParticleNumbers)
        return pWrapped->getInitialValue();
      
      return pWrapped->getInitialConcentration();
      break;

    case cpsapiProperty::UNIT:
      return pWrapped->getUnits();
      break;
      
    default:
      break;
    }

  return cpsapiData();
}