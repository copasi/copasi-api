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
    CData::Property::DIMENSIONALITY,
    CData::Property::INITIAL_VALUE,
    CData::Property::UNIT // READ ONLY
  };

cpsapiCompartment::cpsapiCompartment(CCompartment * pCompartment)
  : base(pCompartment)
  , mpDefaultSpecies(nullptr)
{}

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
  if (!mpObject)
    return;

  visitor.visit(this, cpsapiVisitor::TypeId::cpsapiCompartment);
  base::accept(visitor);
}

cpsapiSpecies cpsapiCompartment::addSpecies(const std::string & name)
{
  if (!mpObject)
    return nullptr;
  
  cpsapiSpecies Species = cpsapiModel(static_cast< CCompartment * >(*mpObject)->getModel()).addSpecies(name, mpObject->getObjectName());

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
  
  cpsapiTransaction::beginStructureChange(static_cast< CCompartment * >(*mpObject)->getModel());

  cpsapiModel(static_cast< CCompartment * >(*mpObject)->getModel()).deleteAllDependents(pSpecies);
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
  if (!mpObject)
    return std::vector< cpsapiSpecies >();

  return convertVector< cpsapiSpecies >(static_cast< CCompartment * >(*mpObject)->getMetabolites());
}

CMetab * cpsapiCompartment::__species(const std::string & name) const
{ 
  if (!mpObject)
    return nullptr;
    
  if (name.empty())
    return mpDefaultSpecies;

  size_t Index = static_cast< CCompartment * >(*mpObject)->getMetabolites().getIndex(name);

  if (Index == C_INVALID_INDEX)
    return nullptr;

  return  &static_cast< CCompartment * >(*mpObject)->getMetabolites()[Index];
}

bool cpsapiCompartment::set(const cpsapiCompartment::Property & property, const CDataValue & value, const CCore::Framework & framework)
{
  return set(static_cast< const CData::Property >(property), value, framework);
}

CDataValue cpsapiCompartment::get(const cpsapiCompartment::Property & property, const CCore::Framework & framework) const
{
  return get(static_cast< const CData::Property >(property), framework);
}

// virtual
bool cpsapiCompartment::set(const CData::Property & property, const CDataValue & value, const CCore::Framework & framework)
{
  if (!mpObject)
    return false;

  if (!isValidProperty<cpsapiCompartment>(property))
    return base::set(property, value, CCore::Framework::__SIZE);

  CCore::Framework Framework(framework);

  CCompartment * pCompartment = static_cast< CCompartment * >(*mpObject);
  CDataObject * pChangedObject = pCompartment;
  bool success = cpsapiTransaction::endStructureChange(pCompartment->getModel());

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

      if (value.getType() == CDataValue::Type::DOUBLE)
        {
          pChangedObject = pCompartment->getInitialValueReference();
          pCompartment->setInitialValue(value.toDouble());
          success = true;
        }
      else
        success = false;
        
      break;

    case CData::Property::UNIT:
      success = false;
      break;

    default:
      break;
    }

  return success && cpsapiTransaction::synchronize(pChangedObject, Framework);
}

// virtual
CDataValue cpsapiCompartment::get(const CData::Property & property, const CCore::Framework & framework) const
{
  if (!mpObject)
    return CDataValue();

  if (!isValidProperty<cpsapiCompartment>(property))
    return base::get(property, CCore::Framework::__SIZE);

  CCompartment * pCompartment = static_cast< CCompartment * >(*mpObject);

  switch (property)
    {
    case CData::Property::DIMENSIONALITY:
      return pCompartment->getDimensionality();
      break;

    case CData::Property::INITIAL_VALUE:
      return pCompartment->getInitialValue();
      break;

    case CData::Property::UNIT:
      return pCompartment->getUnits();
      break;
      
    default:
      break;
    }

  return CDataValue();
}
