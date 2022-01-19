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

#include "cpsapi/task/cpsapiTask.h"

#include <copasi/utilities/CMethodFactory.h>

CPSAPI_NAMESPACE_USE

// static
const cpsapiTask::Properties cpsapiTask::SupportedProperties =
  {
    cpsapiProperty::Type::TASK_TYPE
  };

cpsapiTask::cpsapiTask(wrapped * pWrapped)
  : base(pWrapped, Type::Task)
{
  assertData< cpsapiTask >(pWrapped);

  if (operator bool())
    {
      DATA->mMethod = WRAPPED->getMethod();
      DATA->mProblem = WRAPPED->getProblem();
    }
}

cpsapiTask::cpsapiTask(const cpsapiTask & src)
  : base(src)
{}

// virtual
cpsapiTask::~cpsapiTask()
{}

// virtual
void cpsapiTask::accept(cpsapiVisitor & visitor)
{
  if (!operator bool())
    return;

  visitor.visit(this, Type::Task);
  base::accept(visitor);
}

cpsapiMethod cpsapiTask::createMethod(const CTaskEnum::Method & type) const
{
  if (!operator bool())
    return nullptr;

  return CMethodFactory::create(WRAPPED->getType(), type, WRAPPED);
}

bool cpsapiTask::setMethod(const cpsapiMethod & newMethod)
{
  if (!operator bool())
    return false;

  CTaskEnum::Method NewType = newMethod ? static_cast< CCopasiMethod * >(*newMethod)->getSubType() : CTaskEnum::Method::UnsetMethod;

  if (!CCopasiTask::isValidMethod(NewType, WRAPPED->getValidMethods()))
    return false;

  CTaskEnum::Method OldType = DATA->mMethod ? static_cast< CCopasiMethod * >(*DATA->mMethod)->getSubType() : CTaskEnum::Method::UnsetMethod;

  if (NewType != OldType)
    {
      deleted(*DATA->mMethod);
      WRAPPED->setMethodType(NewType);
    }

  *WRAPPED->getMethod() = *static_cast< CCopasiMethod * >(*newMethod);

  DATA->mMethod = WRAPPED->getMethod();

  return true;
}

cpsapiMethod cpsapiTask::method() const
{
  return DATA->mMethod;
}

cpsapiProblem cpsapiTask::problem() const
{
  return DATA->mProblem;
}

bool cpsapiTask::setProperty(const cpsapiTask::Property & property, const cpsapiData & value, const CCore::Framework & framework)
{
  return setProperty(static_cast< const cpsapiProperty::Type >(property), value, framework);
}

cpsapiData cpsapiTask::getProperty(const cpsapiTask::Property & property, const CCore::Framework & framework) const
{
  return getProperty(static_cast< const cpsapiProperty::Type >(property), framework);
}

// virtual
bool cpsapiTask::setProperty(const cpsapiProperty::Type & property, const cpsapiData & value, const CCore::Framework & framework)
{
  if (!operator bool())
    return false;

  if (!isImplementedProperty< cpsapiTask >(property))
    return base::setProperty(property, value, CCore::Framework::__SIZE);

  return false;
}

// virtual
cpsapiData cpsapiTask::getProperty(const cpsapiProperty::Type & property, const CCore::Framework & framework) const
{
  if (!operator bool())
    return cpsapiData();

  if (!isImplementedProperty< cpsapiTask >(property))
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
