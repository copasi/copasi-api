#include "cpsapi/task/cpsapiMethod.h"

CPSAPI_NAMESPACE_USE

// static
const cpsapiMethod::Properties cpsapiMethod::SupportedProperties =
  {};

cpsapiMethod::cpsapiMethod(wrapped * pWrapped)
  : base(pWrapped, Type::Method)
{}

cpsapiMethod::cpsapiMethod(const cpsapiMethod & src)
  : base(src)
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

bool cpsapiMethod::setProperty(const cpsapiMethod::Property & property, const cpsapiVariant & value, const CCore::Framework & framework)
{
  return setProperty(static_cast< const cpsapiProperty::Type >(property), value, framework);
}

cpsapiVariant cpsapiMethod::getProperty(const cpsapiMethod::Property & property, const CCore::Framework & framework) const
{
  return getProperty(static_cast< const cpsapiProperty::Type >(property), framework);
}

// virtual
bool cpsapiMethod::setProperty(const cpsapiProperty::Type & property, const cpsapiVariant & value, const CCore::Framework & framework)
{
  if (!operator bool())
    return false;

  if (!isValidProperty< cpsapiMethod >(property))
    return base::setProperty(property, value, CCore::Framework::__SIZE);


  return false;
}

// virtual
cpsapiVariant cpsapiMethod::getProperty(const cpsapiProperty::Type & property, const CCore::Framework & framework) const
{
  if (!operator bool())
    return cpsapiVariant();

  if (!isValidProperty< cpsapiMethod >(property))
    return base::getProperty(property, CCore::Framework::__SIZE);

  return cpsapiVariant();
}
