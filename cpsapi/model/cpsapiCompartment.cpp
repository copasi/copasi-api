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

CPSAPI_NAMESPACE_USE

// static
const cpsapiCompartment::Properties cpsapiCompartment::SupportedProperties =
  {
    cpsapiProperty::Type::DIMENSIONALITY,
    cpsapiProperty::Type::INITIAL_VALUE,
    cpsapiProperty::Type::UNIT // READ ONLY
  };

cpsapiCompartment::cpsapiCompartment(cpsapiCompartment::wrapped * pWrapped)
  : base(pWrapped, Type::cpsapiCompartment)
  , mpDefaultSpecies()
{
  for (cpsapiObject * pReference : references())
    if (this != pReference
        && pReference->getType() == Type::cpsapiCompartment)
      {
        mpDefaultSpecies = static_cast< cpsapiCompartment * >(pReference)->mpDefaultSpecies;
        break;
      }

  if (!mpDefaultSpecies)
    mpDefaultSpecies = cpsapiFactory::make_shared< cpsapiSpecies >(nullptr);
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
  
  cpsapiSpecies Species = cpsapiModel(static_cast< wrapped * >(getObject())->getModel()).addSpecies(name, getObject()->getObjectName());

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
  
  cpsapiTransaction::beginStructureChange(static_cast< wrapped * >(getObject())->getModel());

  cpsapiModel(static_cast< wrapped * >(getObject())->getModel()).deleteAllDependents(pSpecies);
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

cpsapiVector< cpsapiSpecies > cpsapiCompartment::getSpecies() const
{
  if (!operator bool())
    return cpsapiVector< cpsapiSpecies >();

  return cpsapiVector< cpsapiSpecies >(&static_cast< wrapped * >(getObject())->getMetabolites());
}

cpsapiSpecies cpsapiCompartment::__species(const std::string & name) const
{ 
  if (!operator bool())
    return nullptr;
    
  if (name.empty())
    return *mpDefaultSpecies;

  size_t Index = static_cast< wrapped * >(getObject())->getMetabolites().getIndex(name);

  if (Index == C_INVALID_INDEX)
    return nullptr;

  return  &static_cast< wrapped * >(getObject())->getMetabolites()[Index];
}

void cpsapiCompartment::updateDefaultSpecies(const cpsapiSpecies & species)
{
  std::shared_ptr< cpsapiSpecies > Default = cpsapiFactory::make_shared< cpsapiSpecies >(species);

  for (cpsapiObject * pReference : references())
    if (pReference->getType() == Type::cpsapiCompartment)
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

  wrapped * pWrapped = static_cast< wrapped * >(getObject());
  CDataObject * pChangedObject = pWrapped;
  bool success = cpsapiTransaction::endStructureChange(pWrapped->getModel());

  switch (property)
    {
    case cpsapiProperty::Type::DIMENSIONALITY:
      Framework = CCore::Framework::__SIZE;
      
      if (value.getType() == CDataValue::Type::UINT)
        success = pWrapped->setDimensionality(value.toUint());
      else if (value.getType() == CDataValue::Type::INT)
        success = pWrapped->setDimensionality(value.toInt());

      break;

    case cpsapiProperty::Type::INITIAL_VALUE:
      if (Framework == CCore::Framework::__SIZE)
        Framework = CCore::Framework::Concentration;

      if (value.getType() == CDataValue::Type::DOUBLE)
        {
          pChangedObject = pWrapped->getInitialValueReference();
          pWrapped->setInitialValue(value.toDouble());
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

  wrapped * pWrapped = static_cast< wrapped * >(getObject());

  switch (property)
    {
    case cpsapiProperty::Type::DIMENSIONALITY:
      return pWrapped->getDimensionality();
      break;

    case cpsapiProperty::Type::INITIAL_VALUE:
      return pWrapped->getInitialValue();
      break;

    case cpsapiProperty::Type::UNIT:
      return pWrapped->getUnits();
      break;
      
    default:
      break;
    }

  return CDataValue();
}
