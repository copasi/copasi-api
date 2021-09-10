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

cpsapiSpecies::cpsapiSpecies(wrapped * pWrapped)
  : base(pWrapped, Type::Species)
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

  visitor.visit(this, Type::Species);
  base::accept(visitor);
}

bool cpsapiSpecies::setProperty(const cpsapiSpecies::Property & property, const cpsapiVariant & value, const CCore::Framework & framework)
{
  return setProperty(static_cast< const cpsapiProperty::Type >(property), value, framework);
}

cpsapiVariant cpsapiSpecies::getProperty(const Property & property, const CCore::Framework & framework) const
{
  return getProperty(static_cast< const cpsapiProperty::Type >(property), framework);
}

// virtual
bool cpsapiSpecies::setProperty(const cpsapiProperty::Type & property, const cpsapiVariant & value, const CCore::Framework & framework)
{
  if (operator=(nullptr))
    return false;

  if (!isValidProperty<cpsapiSpecies>(property))
    return base::setProperty(property, value, CCore::Framework::__SIZE);

  CCore::Framework Framework(framework);

  wrapped * pWrapped = static_cast< wrapped * >(getObject());
  CDataObject * pChangedObject = pWrapped;
  bool success = cpsapiTransaction::endStructureChange(pWrapped->getModel());

  switch (property)
    {
    case cpsapiProperty::Type::INITIAL_VALUE:
      if (Framework == CCore::Framework::__SIZE)
        Framework = CCore::Framework::Concentration;

      if (value.getType() == cpsapiVariant::Type::Double)
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

    case cpsapiProperty::Type::UNIT:
      success = false;
      break;

    default:
      break;
    }

  return success && cpsapiTransaction::synchronize(pChangedObject, Framework);
}

// virtual 
cpsapiVariant cpsapiSpecies::getProperty(const cpsapiProperty::Type & property, const CCore::Framework & framework) const
{
  if (!operator bool())
    return cpsapiVariant();

  if (!isValidProperty<cpsapiSpecies>(property))
    return base::getProperty(property, CCore::Framework::__SIZE);

  wrapped * pWrapped = static_cast< wrapped * >(getObject());

  switch (property)
    {
    case cpsapiProperty::Type::INITIAL_VALUE:
      if (framework == CCore::Framework::ParticleNumbers)
        return pWrapped->getInitialValue();
      
      return pWrapped->getInitialConcentration();
      break;

    case cpsapiProperty::Type::UNIT:
      return pWrapped->getUnits();
      break;
      
    default:
      break;
    }

  return cpsapiVariant();
}