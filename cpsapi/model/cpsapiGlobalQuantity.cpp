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
    cpsapiProperty::Type::UNIT
  };

cpsapiGlobalQuantity::cpsapiGlobalQuantity(wrapped * pWrapped)
  : base(pWrapped, Type::cpsapiGlobalQuantity)
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
  if (!operator bool())
    return;

  visitor.visit(this, Type::cpsapiGlobalQuantity);
  base::accept(visitor);
}

bool cpsapiGlobalQuantity::setProperty(const cpsapiGlobalQuantity::Property & property, const CDataValue & value, const CCore::Framework & framework)
{
  return setProperty(static_cast< const cpsapiProperty::Type >(property), value, framework);
}

CDataValue cpsapiGlobalQuantity::getProperty(const cpsapiGlobalQuantity::Property & property, const CCore::Framework & framework) const
{
  return getProperty(static_cast< const cpsapiProperty::Type >(property), framework);
}

// virtual
bool cpsapiGlobalQuantity::setProperty(const cpsapiProperty::Type & property, const CDataValue & value, const CCore::Framework & framework)
{
  if (!operator bool())
    return false;

  if (!isValidProperty<cpsapiGlobalQuantity>(property))
    return base::setProperty(property, value, CCore::Framework::__SIZE);

  CCore::Framework Framework(framework);

  wrapped * pWrapped = static_cast< wrapped * >(getObject());
  CDataObject * pChangedObject = pWrapped;
  bool success = cpsapiTransaction::endStructureChange(pWrapped->getModel());

  switch (property)
    {
    case cpsapiProperty::Type::UNIT:
      if (value.getType() == CDataValue::Type::STRING)
        success &= pWrapped->setUnitExpression(value.toString());
      break;

    default:
      success = false;
      break;
    }

  return success && cpsapiTransaction::synchronize(pChangedObject, Framework);
}

// virtual
CDataValue cpsapiGlobalQuantity::getProperty(const cpsapiProperty::Type & property, const CCore::Framework & framework) const
{
  if (!operator bool())
    return CDataValue();

  if (!isValidProperty<cpsapiGlobalQuantity>(property))
    return base::getProperty(property, CCore::Framework::__SIZE);

  wrapped * pWrapped = static_cast< wrapped * >(getObject());

  switch (property)
    {
    case cpsapiProperty::Type::UNIT:
      return pWrapped->getUnitExpression();
      break;
      
    default:
      break;
    }

  return CDataValue();
}
