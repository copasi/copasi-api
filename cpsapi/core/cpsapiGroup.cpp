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

#include "cpsapi/core/cpsapiGroup.h"

CPSAPI_NAMESPACE_USE

// static
const cpsapiGroup::Properties cpsapiGroup::SupportedProperties = {};

cpsapiGroup::cpsapiGroup(wrapped * pObject)
  : base(pObject, Type::cpsapiGroup)
{
  assertData(Data(*std::static_pointer_cast< base::Data >(mpData)));
}

cpsapiGroup::cpsapiGroup(const cpsapiGroup & src)
  : base(src)
{}

// virtual
cpsapiGroup::~cpsapiGroup()
{}

// virtual
void cpsapiGroup::accept(cpsapiVisitor & visitor)
{
  if (!operator bool())
    return;

  visitor.visit(this, Type::cpsapiGroup);
  base::accept(visitor);
}

cpsapiParameter cpsapiGroup::addParameter(const std::string & name, const CDataValue & value, CCopasiParameter::Type type)
{
  cpsapiParameter Parameter(nullptr, Type::cpsapiParameter);

  if (!operator bool())
    return Parameter;

  CCopasiParameter * pParameter = nullptr;

  switch (value.getType())
    {
    case CDataValue::Type::DOUBLE:
      switch (type)
        {
        case CCopasiParameter::Type::DOUBLE:
        case CCopasiParameter::Type::__SIZE:
          pParameter = new CCopasiParameter(name, CCopasiParameter::Type::DOUBLE, value.toVoidPointer());
          break;

        case CCopasiParameter::Type::UDOUBLE:
          pParameter = new CCopasiParameter(name, CCopasiParameter::Type::UDOUBLE, value.toVoidPointer());
          break;
        }
      break;

    case CDataValue::Type::INT:
      switch (type)
        {
        case CCopasiParameter::Type::INT:
        case CCopasiParameter::Type::__SIZE:
          pParameter = new CCopasiParameter(name, CCopasiParameter::Type::INT, value.toVoidPointer());
          break;
        }
      break;

    case CDataValue::Type::UINT:
      switch (type)
        {
        case CCopasiParameter::Type::UINT:
        case CCopasiParameter::Type::__SIZE:
          pParameter = new CCopasiParameter(name, CCopasiParameter::Type::UINT, value.toVoidPointer());
          break;
        }
      break;

    case CDataValue::Type::BOOL:
      switch (type)
        {
        case CCopasiParameter::Type::BOOL:
        case CCopasiParameter::Type::__SIZE:
          pParameter = new CCopasiParameter(name, CCopasiParameter::Type::BOOL, value.toVoidPointer());
          break;
        }
      break;

    case CDataValue::Type::STRING:
      switch (type)
        {
        case CCopasiParameter::Type::STRING:
        case CCopasiParameter::Type::__SIZE:
          pParameter = new CCopasiParameter(name, CCopasiParameter::Type::STRING, value.toVoidPointer());
          break;

        case CCopasiParameter::Type::KEY:
        case CCopasiParameter::Type::FILE:
        case CCopasiParameter::Type::EXPRESSION:
        case CCopasiParameter::Type::CN:
          pParameter = new CCopasiParameter(name, type, value.toVoidPointer());
          break;
        }
      break;

    default:
      break;
    }

  if (pParameter != nullptr)
    {
      static_cast< wrapped * >(getObject())->addParameter(pParameter);
      Parameter = cpsapiParameter(pParameter, Type::cpsapiParameter);

      updateDefaultParameter(Parameter);
    }

  return Parameter;
}

cpsapiGroup cpsapiGroup::addGroup(const std::string & name)
{
  cpsapiGroup Group(nullptr);

  if (!operator bool())
    return Group;

  wrapped * pWrapped = new wrapped(name);

  if (pWrapped != nullptr)
    {
      static_cast< wrapped * >(getObject())->addParameter(pWrapped);
      Group = cpsapiGroup(pWrapped);

      updateDefaultParameter(Group);
    }

  return Group;
}

bool cpsapiGroup::deleteParameter(const std::string & name)
{
  CCopasiParameter * pParameter = static_cast< CCopasiParameter * >(__parameter(name).getObject());

  if (pParameter == nullptr)
    return false;

  if (DATA(mpData)->mDefaultParameter.getObject() == pParameter)
    updateDefaultParameter(cpsapiParameter(nullptr, Type::cpsapiParameter));

  deleted(pParameter);
  pdelete(pParameter);

  return true;
}

cpsapiParameter cpsapiGroup::parameter(const std::string & name)
{
  cpsapiParameter Parameter(__parameter(name));

  if (!Parameter)
    return Parameter;

  if (DATA(mpData)->mDefaultParameter.getObject() != Parameter.getObject())
    updateDefaultParameter(Parameter);

  return DATA(mpData)->mDefaultParameter;
}

std::vector< cpsapiParameter > cpsapiGroup::getParameters() const
{
  std::vector< cpsapiParameter > Parameters;

  if (operator bool())
    {
      wrapped::elements::iterator it = static_cast< wrapped::elements * >(static_cast< wrapped * >(getObject())->getValuePointer())->begin();
      wrapped::elements::iterator end = static_cast< wrapped::elements * >(static_cast< wrapped * >(getObject())->getValuePointer())->end();

      for (CCopasiParameter * pParameter : *static_cast< wrapped::elements * >(static_cast< wrapped * >(getObject())->getValuePointer()))
        {
          if (pParameter == nullptr)
            continue;

          if (pParameter->getType() != CCopasiParameter::Type::GROUP)
            Parameters.push_back(cpsapiParameter(pParameter, Type::cpsapiParameter));
          else
            Parameters.push_back(cpsapiGroup(static_cast< wrapped * >(pParameter)));
        }
    }

  return Parameters;
}

cpsapiParameter cpsapiGroup::__parameter(const std::string & name) const
{
  if (!operator bool())
    return cpsapiParameter(nullptr, Type::cpsapiParameter);

  if (name.empty())
    return DATA(mpData)->mDefaultParameter;

  CCopasiParameter * pParameter = static_cast< wrapped * >(getObject())->getParameter(name);

  if (pParameter == nullptr
      || pParameter->getType() != CCopasiParameter::Type::GROUP)
    return cpsapiParameter(pParameter, Type::cpsapiParameter);
  
  return cpsapiGroup(static_cast< wrapped * >(pParameter));
}

void cpsapiGroup::updateDefaultParameter(const cpsapiParameter & parameter)
{
  DATA(mpData)->mDefaultParameter = parameter;
}
