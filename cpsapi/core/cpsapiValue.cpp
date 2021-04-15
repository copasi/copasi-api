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
  if (!mpObject->hasFlag(CDataObject::Reference)
      || getType() == CDataValue::Type::INVALID)
    mpObject = nullptr;
}


cpsapiValue::cpsapiValue(const cpsapiValue & src)
  : base(src)
{}

cpsapiValue::~cpsapiValue()
{}

// virtual 
void cpsapiValue::accept(cpsapiVisitor& v)
{
  if (!mpObject)
    return;

  v.visit(*this);
}

bool cpsapiValue::setValue(const CDataValue & value)
{
  switch (getType())
  {
    case CDataValue::Type::DOUBLE:
      if (value.getType() == CDataValue::Type::DOUBLE)
        {
          *(double *) mpObject->getValuePointer() = value.toDouble();
          return true;
        }
      break;

    case CDataValue::Type::INT:
      if (value.getType() == CDataValue::Type::INT)
        {
          *(int *) mpObject->getValuePointer() = value.toInt();
          return true;
        }
      break;
      
    case CDataValue::Type::BOOL:
      if (value.getType() == CDataValue::Type::BOOL)
        {
          *(bool *) mpObject->getValuePointer() = value.toBool();
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
  if (!mpObject)
    return CDataValue();

  switch (getType())
  {
    case CDataValue::Type::DOUBLE:
      return *(double *) mpObject->getValuePointer();
      break;

    case CDataValue::Type::INT:
      return *(int *) mpObject->getValuePointer();
      break;
      
    case CDataValue::Type::BOOL:
      return *(bool *) mpObject->getValuePointer();
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
  if (!mpObject)
    return CDataValue::Type::INVALID;

  if (mpObject->hasFlag(CDataObject::ValueDbl))
    return CDataValue::Type::DOUBLE;

  if (mpObject->hasFlag(CDataObject::ValueInt))
    return CDataValue::Type::INT;

  if (mpObject->hasFlag(CDataObject::ValueBool))
    return CDataValue::Type::BOOL;

  return CDataValue::Type::INVALID;
}
