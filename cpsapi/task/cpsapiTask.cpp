#include "cpsapi/task/cpsapiTask.h"

#include <copasi/utilities/CMethodFactory.h>

CPSAPI_NAMESPACE_USE

// static
const cpsapiTask::Properties cpsapiTask::SupportedProperties =
  {};

cpsapiTask::cpsapiTask(wrapped * pWrapped)
  : base(pWrapped, Type::Task)
{
  assertData(Data(*std::static_pointer_cast< base::Data >(mpData)));

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

bool cpsapiTask::setProperty(const cpsapiTask::Property & property, const cpsapiVariant & value, const CCore::Framework & framework)
{
  return setProperty(static_cast< const cpsapiProperty::Type >(property), value, framework);
}

cpsapiVariant cpsapiTask::getProperty(const cpsapiTask::Property & property, const CCore::Framework & framework) const
{
  return getProperty(static_cast< const cpsapiProperty::Type >(property), framework);
}

// virtual
bool cpsapiTask::setProperty(const cpsapiProperty::Type & property, const cpsapiVariant & value, const CCore::Framework & framework)
{
  if (!operator bool())
    return false;

  if (!isValidProperty< cpsapiTask >(property))
    return base::setProperty(property, value, CCore::Framework::__SIZE);

  return false;
}

// virtual
cpsapiVariant cpsapiTask::getProperty(const cpsapiProperty::Type & property, const CCore::Framework & framework) const
{
  if (!operator bool())
    return cpsapiVariant();

  if (!isValidProperty< cpsapiTask >(property))
    return base::getProperty(property, CCore::Framework::__SIZE);

  return cpsapiVariant();
}
