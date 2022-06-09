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

#include "cpsapi/core/cpsapiObject.h"
#include "cpsapi/core/cpsapiVector.h"
#include "cpsapi/model/cpsapiModel.h"

CPSAPI_NAMESPACE_USE

cpsapiData::cpsapiData()
  : base()
{}

cpsapiData::cpsapiData(const C_FLOAT64 & value)
  : base(value)
{}

cpsapiData::cpsapiData(const C_INT32 & value)
  : base(value)
{}

cpsapiData::cpsapiData(const unsigned C_INT32 & value)
  : base(value)
{}

cpsapiData::cpsapiData(const size_t & value)
  : base(value)
{}

cpsapiData::cpsapiData(const bool & value)
  : base(value)
{}

cpsapiData::cpsapiData(const std::string & value)
  : base(value)
{}

cpsapiData::cpsapiData(const char * value)
  : base(std::string(value))
{}

cpsapiData::cpsapiData(const CCommonName & value)
  : base(value)
{}

cpsapiData::cpsapiData(const cpsapiData::Vector & value)
  : base(value)
{}

cpsapiData::cpsapiData(const cpsapiData::Type & type, void * pValue)
  : base()
{
  switch (type)
    {
    case Type::Double:
      base::operator=(*static_cast< C_FLOAT64 * >(pValue));
      break;

    case Type::Int32:
      base::operator=(*static_cast< C_INT32 * >(pValue));
      break;

    case Type::UnsignedInt32:
      base::operator=(*static_cast< unsigned C_INT32 * >(pValue));
      break;

    case Type::SizeType:
      base::operator=(*static_cast< size_t * >(pValue));
      break;

    case Type::Bool:
      base::operator=(*static_cast< bool * >(pValue));
      break;

    case Type::String:
      base::operator=(*static_cast< std::string * >(pValue));
      break;

    case Type::CommonName:
      base::operator=(*static_cast< CCommonName * >(pValue));
      break;

    case Type::Vector:
      base::operator=(*static_cast< cpsapiData::Vector * >(pValue));
      break;

    case Type::Object:
      base::operator=(*static_cast< const cpsapiObject * >(pValue));
      break;

    case Type::__SIZE:
      break;
    }
}

cpsapiData::cpsapiData(const cpsapiData & src)
  : base(src)
{}

cpsapiData & cpsapiData::operator = (const cpsapiData & rhs)
{
  base::operator=(rhs);
  return *this;
}
  
cpsapiData::~cpsapiData()
{}

C_FLOAT64 cpsapiData::toDouble() const
{
  if (std::holds_alternative< C_FLOAT64 >(*this))
    return std::get< C_FLOAT64 >(*this);

  return std::numeric_limits< C_FLOAT64 >::quiet_NaN();
}

C_INT32 cpsapiData::toInt32() const
{
  if (std::holds_alternative< C_INT32 >(*this))
    return std::get< C_INT32 >(*this);

  return 0;
}

unsigned C_INT32 cpsapiData::toUnsignedInt32() const
{
  if (std::holds_alternative< unsigned C_INT32 >(*this))
    return std::get< unsigned C_INT32 >(*this);

  return 0;
}

size_t cpsapiData::toSizeType() const
{
  if (std::holds_alternative< size_t >(*this))
    return std::get< size_t >(*this);

  return 0;
}

bool cpsapiData::toBool() const
{
  if (std::holds_alternative< bool >(*this))
    return std::get< bool >(*this);

  return false;
}

std::string cpsapiData::toString() const
{
  if (std::holds_alternative< std::string >(*this))
    return std::get< std::string >(*this);

  return std::string();
}

CCommonName cpsapiData::toCommonName() const
{
  if (std::holds_alternative< CRegisteredCommonName >(*this))
    return std::get< CRegisteredCommonName >(*this);

  return std::string();
}

const cpsapiData::Vector & cpsapiData::toData() const
{
  static const cpsapiData::Vector Invalid;

  if (std::holds_alternative< cpsapiData::Vector >(*this))
    return std::get< cpsapiData::Vector >(*this);


  return Invalid;
}  

cpsapiData::Type cpsapiData::getType() const
{
  return static_cast< Type >(index());
}

