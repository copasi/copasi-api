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

#include "cpsapi/task/cpsapiProblem.h"

CPSAPI_NAMESPACE_USE

// static
const cpsapiProblem::Properties cpsapiProblem::SupportedProperties =
  {
    cpsapiProperty::Type::TASK_TYPE
  };

cpsapiProblem::cpsapiProblem(wrapped * pWrapped)
  : base(pWrapped, cpsapiObjectData::Type::Problem)
{}

// virtual
cpsapiProblem::~cpsapiProblem()
{}

bool cpsapiProblem::setProperty(const cpsapiProblem::Property & property, const cpsapiData & value, const CCore::Framework & framework)
{
  return setProperty(static_cast< const cpsapiProperty::Type >(property), value, framework);
}

cpsapiData cpsapiProblem::getProperty(const cpsapiProblem::Property & property, const CCore::Framework & framework) const
{
  return getProperty(static_cast< const cpsapiProperty::Type >(property), framework);
}

// virtual
bool cpsapiProblem::setProperty(const cpsapiProperty::Type & property, const cpsapiData & value, const CCore::Framework & /* framework */)
{
  if (!isValid())
    return false;

  if (!isImplementedProperty< cpsapiProblem >(property))
    return base::setProperty(property, value, CCore::Framework::__SIZE);

  return false;
}

// virtual
cpsapiData cpsapiProblem::getProperty(const cpsapiProperty::Type & property, const CCore::Framework & /* framework */) const
{
  if (!isValid())
    return cpsapiData();

  if (!isImplementedProperty< cpsapiProblem >(property))
    return base::getProperty(property, CCore::Framework::__SIZE);

  switch (property)
  {
    case cpsapiProperty::TASK_TYPE:
      return CTaskEnum::TaskName[WRAPPED->getType()];
      break;

    default:
      break;
  }

  return cpsapiData();
}
