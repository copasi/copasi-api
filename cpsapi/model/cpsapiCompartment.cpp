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

#include "cpsapi/model/cpsapiCompartment.h"
#include "cpsapi/model/cpsapiSpecies.h"
#include "cpsapi/model/cpsapiModel.h"
#include "cpsapi/model/cpsapiTransaction.h"

CPSAPI_NAMESPACE_USE

// static
const cpsapiCompartment::Properties cpsapiCompartment::SupportedProperties =
  {
    cpsapiProperty::DIMENSIONALITY,
    cpsapiProperty::INITIAL_VALUE,
    cpsapiProperty::UNIT // READ ONLY
  };

cpsapiCompartment::cpsapiCompartment(cpsapiCompartment::wrapped * pWrapped)
  : base(pWrapped, cpsapiObjectData::Type::Compartment)
{
  cpsapiObjectData::assertDataType< cpsapiCompartment >(mpData);
}

// virtual
cpsapiCompartment::~cpsapiCompartment()
{}

cpsapiSpecies cpsapiCompartment::addSpecies(const std::string & name)
{
  if (!isValid())
    return nullptr;
  
  cpsapiSpecies Species = cpsapiModel(WRAPPED->getModel()).addSpecies(name, getObject()->getObjectName());

  if (!Species)
    return nullptr;

  if (*DATA->mDefaultSpecies != *Species)
    updateDefaultSpecies(Species);

  return DATA->mDefaultSpecies;
}

bool cpsapiCompartment::deleteSpecies(const std::string & name)
{
  CMetab * pSpecies = static_cast< CMetab * >(*__species(name));

  if (pSpecies == nullptr)
    return false;
  
  if (*DATA->mDefaultSpecies == pSpecies)
    updateDefaultSpecies(nullptr);
  
  cpsapiTransaction::beginStructureChange(WRAPPED->getModel());

  cpsapiModel(WRAPPED->getModel()).deleteAllDependents(pSpecies);
  cpsapiObjectData::deleted(pSpecies);
  pdelete(pSpecies);

  return true;
}

cpsapiSpecies cpsapiCompartment::species(const std::string & name)
{
  cpsapiSpecies Species(__species(name));

  if (!Species)
    return Species;

  if (*DATA->mDefaultSpecies != *Species)
    updateDefaultSpecies(Species);

  return DATA->mDefaultSpecies;
}

cpsapiVector< cpsapiSpecies > cpsapiCompartment::getSpecies() const
{
  if (!isValid())
    return cpsapiVector< cpsapiSpecies >();

  return cpsapiVector< cpsapiSpecies >(&WRAPPED->getMetabolites());
}

cpsapiSpecies cpsapiCompartment::__species(const std::string & name) const
{ 
  if (!isValid())
    return nullptr;
    
  if (name.empty())
    return DATA->mDefaultSpecies;

  size_t Index = WRAPPED->getMetabolites().getIndex(name);

  if (Index == C_INVALID_INDEX)
    return nullptr;

  return  &WRAPPED->getMetabolites()[Index];
}

void cpsapiCompartment::updateDefaultSpecies(const cpsapiSpecies & species)
{
  DATA->mDefaultSpecies = species;
}

bool cpsapiCompartment::setProperty(const cpsapiCompartment::Property & property, const cpsapiData & value, const CCore::Framework & framework)
{
  return setProperty(static_cast< const cpsapiProperty::Type >(property), value, framework);
}

cpsapiData cpsapiCompartment::getProperty(const cpsapiCompartment::Property & property, const CCore::Framework & framework) const
{
  return getProperty(static_cast< const cpsapiProperty::Type >(property), framework);
}

// virtual
bool cpsapiCompartment::setProperty(const cpsapiProperty::Type & property, const cpsapiData & value, const CCore::Framework & framework)
{
  if (!isValid())
    return false;

  if (!isImplementedProperty<cpsapiCompartment>(property))
    return base::setProperty(property, value, CCore::Framework::__SIZE);

  CCore::Framework Framework(framework);

  wrapped * pWrapped = WRAPPED;
  CDataObject * pChangedObject = pWrapped;
  bool success = cpsapiTransaction::endStructureChange(pWrapped->getModel());

  switch (property)
    {
    case cpsapiProperty::DIMENSIONALITY:
      Framework = CCore::Framework::__SIZE;
      
      if (value.getType() == cpsapiData::Type::Int32)
        success = pWrapped->setDimensionality(value.toInt32());
      else if (value.getType() == cpsapiData::Type::UnsignedInt32)
        success = pWrapped->setDimensionality(value.toUnsignedInt32());

      break;

    case cpsapiProperty::INITIAL_VALUE:
      if (Framework == CCore::Framework::__SIZE)
        Framework = CCore::Framework::Concentration;

      if (value.getType() == cpsapiData::Type::Double)
        {
          pChangedObject = pWrapped->getInitialValueReference();
          pWrapped->setInitialValue(value.toDouble());
          success = true;
        }
      else
        success = false;
        
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
cpsapiData cpsapiCompartment::getProperty(const cpsapiProperty::Type & property, const CCore::Framework & /* framework */) const
{
  if (!isValid())
    return cpsapiData();

  if (!isImplementedProperty<cpsapiCompartment>(property))
    return base::getProperty(property, CCore::Framework::__SIZE);

  wrapped * pWrapped = WRAPPED;

  switch (property)
    {
    case cpsapiProperty::DIMENSIONALITY:
      return pWrapped->getDimensionality();
      break;

    case cpsapiProperty::INITIAL_VALUE:
      return pWrapped->getInitialValue();
      break;

    case cpsapiProperty::UNIT:
      return pWrapped->getUnits();
      break;
      
    default:
      break;
    }

  return cpsapiData();
}
