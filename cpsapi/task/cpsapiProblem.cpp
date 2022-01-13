#include "cpsapi/task/cpsapiProblem.h"

CPSAPI_NAMESPACE_USE

// static
const cpsapiProblem::Properties cpsapiProblem::SupportedProperties =
  {};

cpsapiProblem::cpsapiProblem(wrapped * pWrapped)
  : base(pWrapped, Type::Problem)
{}

cpsapiProblem::cpsapiProblem(const cpsapiProblem & src)
  : base(src)
{}

// virtual
cpsapiProblem::~cpsapiProblem()
{}

// virtual
void cpsapiProblem::accept(cpsapiVisitor & visitor)
{
  if (!operator bool())
    return;

  visitor.visit(this, Type::Problem);
  base::accept(visitor);
}

bool cpsapiProblem::setProperty(const cpsapiProblem::Property & property, const cpsapiVariant & value, const CCore::Framework & framework)
{
  return setProperty(static_cast< const cpsapiProperty::Type >(property), value, framework);
}

cpsapiVariant cpsapiProblem::getProperty(const cpsapiProblem::Property & property, const CCore::Framework & framework) const
{
  return getProperty(static_cast< const cpsapiProperty::Type >(property), framework);
}

// virtual
bool cpsapiProblem::setProperty(const cpsapiProperty::Type & property, const cpsapiVariant & value, const CCore::Framework & framework)
{
  if (!operator bool())
    return false;

  if (!isValidProperty< cpsapiProblem >(property))
    return base::setProperty(property, value, CCore::Framework::__SIZE);

  return false;
}

// virtual
cpsapiVariant cpsapiProblem::getProperty(const cpsapiProperty::Type & property, const CCore::Framework & framework) const
{
  if (!operator bool())
    return cpsapiVariant();

  if (!isValidProperty< cpsapiProblem >(property))
    return base::getProperty(property, CCore::Framework::__SIZE);

  return cpsapiVariant();
}
