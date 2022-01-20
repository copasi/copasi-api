// BEGIN: Copyright 
// Copyright (C) 2022 by Pedro Mendes, Rector and Visitors of the 
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

#include "cpsapi/task/cpsapiMethod.h"

CPSAPI_NAMESPACE_USE

// static
const cpsapiMethod::Properties cpsapiMethod::SupportedProperties =
  {
    cpsapiProperty::Type::METHOD_TYPE
  };

cpsapiMethod::cpsapiMethod(wrapped * pWrapped)
  : base(pWrapped, Type::Method)
{}

// virtual
cpsapiMethod::~cpsapiMethod()
{}

// virtual
void cpsapiMethod::accept(cpsapiVisitor & visitor)
{
  if (!operator bool())
    return;

  visitor.visit(this, Type::Method);
  base::accept(visitor);
}

bool cpsapiMethod::setProperty(const cpsapiMethod::Property & property, const cpsapiData & value, const CCore::Framework & framework)
{
  return setProperty(static_cast< const cpsapiProperty::Type >(property), value, framework);
}

cpsapiData cpsapiMethod::getProperty(const cpsapiMethod::Property & property, const CCore::Framework & framework) const
{
  return getProperty(static_cast< const cpsapiProperty::Type >(property), framework);
}

// virtual
bool cpsapiMethod::setProperty(const cpsapiProperty::Type & property, const cpsapiData & value, const CCore::Framework & /* framework */)
{
  if (!operator bool())
    return false;

  if (!isImplementedProperty< cpsapiMethod >(property))
    return base::setProperty(property, value, CCore::Framework::__SIZE);


  return false;
}

// virtual
cpsapiData cpsapiMethod::getProperty(const cpsapiProperty::Type & property, const CCore::Framework & /* framework */) const
{
  if (!operator bool())
    return cpsapiData();

  if (!isImplementedProperty< cpsapiMethod >(property))
    return base::getProperty(property, CCore::Framework::__SIZE);

  switch (property)
  {
    case cpsapiProperty::METHOD_TYPE:
      return CTaskEnum::MethodName[WRAPPED->getSubType()];
      break;

    default:
      break;
  }
  
  return cpsapiData();
}
