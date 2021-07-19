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

#include "cpsapi/model/cpsapiGlobalQuantity.h"
#include "cpsapi/model/cpsapiSpecies.h"
#include "cpsapi/model/cpsapiModel.h"
#include "cpsapi/model/cpsapiTransaction.h"

#include "copasi/model/CModelValue.h"

CPSAPI_NAMESPACE_USE

// static
const cpsapiGlobalQuantity::Properties cpsapiGlobalQuantity::SupportedProperties =
  {
    CData::Property::UNIT
  };

cpsapiGlobalQuantity::cpsapiGlobalQuantity(CModelValue * pGlobalQuantity)
  : base(pGlobalQuantity)
{}

cpsapiGlobalQuantity::cpsapiGlobalQuantity(const cpsapiGlobalQuantity & src)
  : base(src)
{}

// virtual
cpsapiGlobalQuantity::~cpsapiGlobalQuantity()
{}

// virtual 
void cpsapiGlobalQuantity::accept(cpsapiVisitor & visitor)
{
  if (!mpObject)
    return;

  visitor.visit(this, cpsapiVisitor::TypeId::cpsapiGlobalQuantity);
  base::accept(visitor);
}

bool cpsapiGlobalQuantity::set(const cpsapiGlobalQuantity::Property & property, const CDataValue & value, const CCore::Framework & framework)
{
  return set(static_cast< const CData::Property >(property), value, framework);
}

CDataValue cpsapiGlobalQuantity::get(const cpsapiGlobalQuantity::Property & property, const CCore::Framework & framework) const
{
  return get(static_cast< const CData::Property >(property), framework);
}

// virtual
bool cpsapiGlobalQuantity::set(const CData::Property & property, const CDataValue & value, const CCore::Framework & framework)
{
  if (!mpObject)
    return false;

  if (!isValidProperty<cpsapiGlobalQuantity>(property))
    return base::set(property, value, CCore::Framework::__SIZE);

  CCore::Framework Framework(framework);

  CModelValue * pGlobalQuantity = static_cast< CModelValue * >(*mpObject);
  CDataObject * pChangedObject = pGlobalQuantity;
  bool success = cpsapiTransaction::endStructureChange(pGlobalQuantity->getModel());

  switch (property)
    {
    case CData::Property::UNIT:
      return pGlobalQuantity->setUnitExpression(value.toString());
      break;

    default:
      break;
    }

  return success && cpsapiTransaction::synchronize(pChangedObject, Framework);
}

// virtual
CDataValue cpsapiGlobalQuantity::get(const CData::Property & property, const CCore::Framework & framework) const
{
  if (!mpObject)
    return CDataValue();

  if (!isValidProperty<cpsapiGlobalQuantity>(property))
    return base::get(property, CCore::Framework::__SIZE);

  CModelValue * pGlobalQuantity = static_cast< CModelValue * >(*mpObject);

  switch (property)
    {
    case CData::Property::UNIT:
      return pGlobalQuantity->getUnits();
      break;
      
    default:
      break;
    }

  return CDataValue();
}
