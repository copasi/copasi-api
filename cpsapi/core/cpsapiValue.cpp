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

#include "cpsapi/core/cpsapiValue.h"

CPSAPI_NAMESPACE_USE

cpsapiValue::cpsapiValue(CDataObject * pValue)
  : base(pValue)
{
  if (!getObject()->hasFlag(CDataObject::Reference)
      || getType() == CDataValue::Type::INVALID)
    cpsapiPointer::operator=(nullptr);
}


cpsapiValue::cpsapiValue(const cpsapiValue & src)
  : base(src)
{}

cpsapiValue::~cpsapiValue()
{}

// virtual 
void cpsapiValue::accept(cpsapiVisitor & visitor)
{
  if (!operator bool())
    return;

  visitor.visit(this, cpsapiVisitor::TypeId::cpsapiValue);
}

bool cpsapiValue::setValue(const CDataValue & value)
{
  switch (getType())
  {
    case CDataValue::Type::DOUBLE:
      if (value.getType() == CDataValue::Type::DOUBLE)
        {
          *(double *) getObject()->getValuePointer() = value.toDouble();
          return true;
        }
      break;

    case CDataValue::Type::INT:
      if (value.getType() == CDataValue::Type::INT)
        {
          *(int *) getObject()->getValuePointer() = value.toInt();
          return true;
        }
      break;
      
    case CDataValue::Type::BOOL:
      if (value.getType() == CDataValue::Type::BOOL)
        {
          *(bool *) getObject()->getValuePointer() = value.toBool();
          return true;
        }
      break;
      
    default:
      break;      
  }

  return false;
}

CDataValue cpsapiValue::getValue() const
{
  if (!operator bool())
    return CDataValue();

  switch (getType())
  {
    case CDataValue::Type::DOUBLE:
      return *(double *) getObject()->getValuePointer();
      break;

    case CDataValue::Type::INT:
      return *(int *) getObject()->getValuePointer();
      break;
      
    case CDataValue::Type::BOOL:
      return *(bool *) getObject()->getValuePointer();
      break;
      
    default:
      break;      
  }

  return CDataValue();
}

cpsapiValue::operator CDataValue() const
{
  return getValue();
}

bool cpsapiValue::valid() const
{
  return getType() != CDataValue::Type::INVALID;
}

CDataValue::Type cpsapiValue::getType() const
{
  if (!operator bool())
    return CDataValue::Type::INVALID;

  if (getObject()->hasFlag(CDataObject::ValueDbl))
    return CDataValue::Type::DOUBLE;

  if (getObject()->hasFlag(CDataObject::ValueInt))
    return CDataValue::Type::INT;

  if (getObject()->hasFlag(CDataObject::ValueBool))
    return CDataValue::Type::BOOL;

  return CDataValue::Type::INVALID;
}
