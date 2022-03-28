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

#include "cpsapi/model/cpsapiGlobalQuantity.h"
#include "cpsapi/model/cpsapiSpecies.h"
#include "cpsapi/model/cpsapiModel.h"
#include "cpsapi/model/cpsapiTransaction.h"

#pragma GCC diagnostic push
#include <copasi/model/CModelValue.h>
#pragma GCC diagnostic pop

CPSAPI_NAMESPACE_USE

// static
const cpsapiGlobalQuantity::Properties cpsapiGlobalQuantity::SupportedProperties =
  {
    cpsapiProperty::Type::UNIT
  };

cpsapiGlobalQuantity::cpsapiGlobalQuantity(wrapped * pWrapped)
  : base(pWrapped, cpsapiObjectData::Type::GlobalQuantity)
{}

// virtual
cpsapiGlobalQuantity::~cpsapiGlobalQuantity()
{}

bool cpsapiGlobalQuantity::setProperty(const cpsapiGlobalQuantity::Property & property, const cpsapiData & value, const CCore::Framework & framework)
{
  return setProperty(static_cast< const cpsapiProperty::Type >(property), value, framework);
}

cpsapiData cpsapiGlobalQuantity::getProperty(const cpsapiGlobalQuantity::Property & property, const CCore::Framework & framework) const
{
  return getProperty(static_cast< const cpsapiProperty::Type >(property), framework);
}

// virtual
bool cpsapiGlobalQuantity::setProperty(const cpsapiProperty::Type & property, const cpsapiData & value, const CCore::Framework & framework)
{
  if (!isValid())
    return false;

  if (!isImplementedProperty<cpsapiGlobalQuantity>(property))
    return base::setProperty(property, value, CCore::Framework::__SIZE);

  CCore::Framework Framework(framework);

  wrapped * pWrapped = WRAPPED;
  CDataObject * pChangedObject = pWrapped;
  bool success = cpsapiTransaction::endStructureChange(pWrapped->getModel());

  switch (property)
    {
    case cpsapiProperty::Type::UNIT:
      if (value.getType() == cpsapiData::Type::String)
        success &= pWrapped->setUnitExpression(value.toString());
      break;

    default:
      success = false;
      break;
    }

  return success && cpsapiTransaction::synchronize(pChangedObject, Framework);
}

// virtual
cpsapiData cpsapiGlobalQuantity::getProperty(const cpsapiProperty::Type & property, const CCore::Framework & /* framework */) const
{
  if (!isValid())
    return cpsapiData();

  if (!isImplementedProperty<cpsapiGlobalQuantity>(property))
    return base::getProperty(property, CCore::Framework::__SIZE);

  wrapped * pWrapped = WRAPPED;

  switch (property)
    {
    case cpsapiProperty::Type::UNIT:
      return pWrapped->getUnitExpression();
      break;
      
    default:
      break;
    }

  return cpsapiData();
}
