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
#include "cpsapi/model/cpsapiTransaction.h"

#include "copasi/model/CMetab.h"

CPSAPI_NAMESPACE_USE

// static
const cpsapiSpecies::Properties cpsapiSpecies::SupportedProperties =
  {
    cpsapiProperty::Type::INITIAL_VALUE,
    cpsapiProperty::Type::UNIT // READ ONLY
  };

cpsapiSpecies::cpsapiSpecies(CMetab * pSpecies)
  : base(pSpecies, Type::cpsapiSpecies)
{}

cpsapiSpecies::cpsapiSpecies(const cpsapiSpecies & src)
  : base(src)
{}

// virtual
cpsapiSpecies::~cpsapiSpecies()
{}

// virtual 
void cpsapiSpecies::accept(cpsapiVisitor & visitor)
{
  if (!operator bool())
    return;

  visitor.visit(this, Type::cpsapiSpecies);
  base::accept(visitor);
}

bool cpsapiSpecies::setProperty(const cpsapiSpecies::Property & property, const CDataValue & value, const CCore::Framework & framework)
{
  return setProperty(static_cast< const cpsapiProperty::Type >(property), value, framework);
}

CDataValue cpsapiSpecies::getProperty(const Property & property, const CCore::Framework & framework) const
{
  return getProperty(static_cast< const cpsapiProperty::Type >(property), framework);
}

// virtual
bool cpsapiSpecies::setProperty(const cpsapiProperty::Type & property, const CDataValue & value, const CCore::Framework & framework)
{
  if (operator=(nullptr))
    return false;

  if (!isValidProperty<cpsapiSpecies>(property))
    return base::setProperty(property, value, CCore::Framework::__SIZE);

  CCore::Framework Framework(framework);

  CMetab * pSpecies = static_cast< CMetab * >(getObject());
  CDataObject * pChangedObject = pSpecies;
  bool success = cpsapiTransaction::endStructureChange(pSpecies->getModel());

  switch (property)
    {
    case cpsapiProperty::Type::INITIAL_VALUE:
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

          case CCore::Framework::__SIZE:
            success = false;
            break;
          }

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
CDataValue cpsapiSpecies::getProperty(const cpsapiProperty::Type & property, const CCore::Framework & framework) const
{
  if (!operator bool())
    return CDataValue();

  if (!isValidProperty<cpsapiSpecies>(property))
    return base::getProperty(property, CCore::Framework::__SIZE);

  CMetab * pSpecies = static_cast< CMetab * >(getObject());

  switch (property)
    {
    case cpsapiProperty::Type::INITIAL_VALUE:
      if (framework == CCore::Framework::ParticleNumbers)
        return pSpecies->getInitialValue();
      
      return pSpecies->getInitialConcentration();
      break;

    case cpsapiProperty::Type::UNIT:
      return pSpecies->getUnits();
      break;
      
    default:
      break;
    }

  return CDataValue();
}