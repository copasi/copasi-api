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

#include "cpsapi/core/cpsapiGroup.h"

CPSAPI_NAMESPACE_USE

// static
const cpsapiGroup::Properties cpsapiGroup::SupportedProperties = {};

// static
const cpsapiGroup::Properties cpsapiGroup::HiddenProperties = 
{
  cpsapiProperty::Type::PARAMETER_VALUE
};

// static
const cpsapiGroup::References cpsapiGroup::SupportedReferences = {};

// static
const cpsapiGroup::References cpsapiGroup::HiddenReferences = 
{
  cpsapiProperty::Type::PARAMETER_VALUE
};

cpsapiGroup::cpsapiGroup(wrapped * pWrapped, const Type & type)
  : base(pWrapped, type)
{
  assertData< cpsapiGroup >(pWrapped);
}

// virtual
cpsapiGroup::~cpsapiGroup()
{}

// virtual
void cpsapiGroup::accept(cpsapiVisitor & visitor)
{
  if (!isValid())
    return;

  visitor.visit(this, Type::Group);
  base::accept(visitor);
}

cpsapiParameter cpsapiGroup::addParameter(const std::string & name, const CDataValue & value, CCopasiParameter::Type type)
{
  cpsapiParameter Parameter(nullptr, Type::Parameter);

  if (!isValid())
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

        default:
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

        default:
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

        default:
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

        default:
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

        default:
          break;
        }
      break;

      default:
        break;
    }

  if (pParameter != nullptr)
    {
      WRAPPED->addParameter(pParameter);
      Parameter = cpsapiParameter(pParameter, Type::Parameter);

      updateDefaultParameter(Parameter);
    }

  return Parameter;
}

cpsapiGroup cpsapiGroup::addGroup(const std::string & name)
{
  cpsapiGroup Group(nullptr);

  if (!isValid())
    return Group;

  wrapped * pWrapped = new wrapped(name);

  if (pWrapped != nullptr)
    {
      WRAPPED->addParameter(pWrapped);
      Group = cpsapiGroup(pWrapped);

      updateDefaultParameter(Group);
    }

  return Group;
}

bool cpsapiGroup::deleteParameter(const std::string & name)
{
  CCopasiParameter * pParameter = static_cast< CCopasiParameter * >(*__parameter(name));

  if (pParameter == nullptr)
    return false;

  if (*DATA->mDefaultParameter == pParameter)
    updateDefaultParameter(cpsapiParameter(nullptr, Type::Parameter));

  deleted(pParameter);
  pdelete(pParameter);

  return true;
}

cpsapiParameter cpsapiGroup::parameter(const std::string & name)
{
  cpsapiParameter Parameter(__parameter(name));

  if (!Parameter)
    return Parameter;

  if (*DATA->mDefaultParameter != *Parameter)
    updateDefaultParameter(Parameter);

  return DATA->mDefaultParameter;
}

std::vector< cpsapiParameter > cpsapiGroup::getParameters() const
{
  std::vector< cpsapiParameter > Parameters;

  if (operator bool())
    {
      for (CCopasiParameter * pParameter : *static_cast< wrapped::elements * >(WRAPPED->getValuePointer()))
        {
          if (pParameter == nullptr)
            continue;

          if (pParameter->getType() != CCopasiParameter::Type::GROUP)
            Parameters.push_back(cpsapiParameter(pParameter, Type::Parameter));
          else
            Parameters.push_back(cpsapiGroup(static_cast< wrapped * >(pParameter)));
        }
    }

  return Parameters;
}

bool cpsapiGroup::setProperty(const cpsapiGroup::Property & property, const cpsapiData & value, const CCore::Framework & framework)
{
  return setProperty(static_cast<const cpsapiProperty::Type >(property), value, framework);
}

cpsapiData cpsapiGroup::getProperty(const Property & property, const CCore::Framework & framework) const
{
  return getProperty(static_cast<const cpsapiProperty::Type >(property), framework);
}

// virtual
bool cpsapiGroup::setProperty(const cpsapiProperty::Type & property, const cpsapiData & value, const CCore::Framework & framework)
{
  if (!isValid()
      || isHiddenProperty< cpsapiGroup >(property))
    return false;

  if (!isImplementedProperty< cpsapiGroup >(property))
    return base::setProperty(property, value, framework);

  return false;
}

// virtual 
cpsapiData cpsapiGroup::getProperty(const cpsapiProperty::Type & property, const CCore::Framework & framework) const
{
  if (!isValid()
      || isHiddenProperty< cpsapiGroup >(property))
    return cpsapiData();

  if (!isImplementedProperty< cpsapiGroup >(property))
    return base::getProperty(property, framework);

  return cpsapiData();
}

CCommonName cpsapiGroup::getDataCN(const cpsapiGroup::Reference & reference, const CCore::Framework & framework) const
{
  return getDataCN(static_cast< cpsapiReference::Type >(reference), framework);
}

// virtual
CCommonName cpsapiGroup::getDataCN(const cpsapiReference::Type & reference, const CCore::Framework & framework) const
{
  if (!isValid()
      || isHiddenReference< cpsapiGroup >(reference))
    return Invalid;

  if (!isImplementedReference< cpsapiGroup >(reference))
    return base::getDataCN(reference, framework);

  return Invalid;
}

cpsapiParameter cpsapiGroup::__parameter(const std::string & name) const
{
  if (!isValid())
    return cpsapiParameter(nullptr, Type::Parameter);

  if (name.empty())
    return DATA->mDefaultParameter;

  CCopasiParameter * pParameter = WRAPPED->getParameter(name);

  if (pParameter == nullptr
      || pParameter->getType() != CCopasiParameter::Type::GROUP)
    return cpsapiParameter(pParameter, Type::Parameter);
  
  return cpsapiGroup(static_cast< wrapped * >(pParameter));
}

void cpsapiGroup::updateDefaultParameter(const cpsapiParameter & parameter)
{
  DATA->mDefaultParameter = parameter;
}
