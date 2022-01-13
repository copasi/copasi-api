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
  : mType(Type::__SIZE)
  , mpData(nullptr, &cpsapiFactory::free_unique< void >)
{}

cpsapiData::cpsapiData(const C_FLOAT64 & value)
  : mType(Type::Double)
  , mpData(new C_FLOAT64(value), &cpsapiFactory::free_unique< C_FLOAT64 >)
{}

cpsapiData::cpsapiData(const C_INT32 & value)
  : mType(Type::Int32)
  , mpData(new C_INT32(value), &cpsapiFactory::free_unique< C_INT32 >)
{}

cpsapiData::cpsapiData(const unsigned C_INT32 & value)
  : mType(Type::UnsignedInt32)
  , mpData(new unsigned C_INT32(value), &cpsapiFactory::free_unique< unsigned C_INT32 >)
{}

cpsapiData::cpsapiData(const size_t & value)
  : mType(Type::SizeType)
  , mpData(new size_t(value), &cpsapiFactory::free_unique< size_t >)
{}

cpsapiData::cpsapiData(const bool & value)
  : mType(Type::Bool)
  , mpData(new bool(value), &cpsapiFactory::free_unique< bool >)
{}

cpsapiData::cpsapiData(const std::string & value)
  : mType(Type::String)
  , mpData(new std::string(value), &cpsapiFactory::free_unique< std::string >)
{}

cpsapiData::cpsapiData(const char * value)
  : mType(Type::String)
  , mpData(new std::string(value), &cpsapiFactory::free_unique< std::string >)
{}

cpsapiData::cpsapiData(const CRegisteredCommonName & value)
  : mType(Type::CommonName)
  , mpData(new CRegisteredCommonName(value), &cpsapiFactory::free_unique< CRegisteredCommonName >)
{}

cpsapiData::cpsapiData(const cpsapiDataVector & value)
  : mType(Type::Data)
  , mpData(new cpsapiDataVector(value), &cpsapiFactory::free_unique< cpsapiDataVector >)
{}

cpsapiData::cpsapiData(const cpsapiData::Type & type, void * pValue)
  : mType(type)
  , mpData(nullptr, &cpsapiFactory::free_unique< void >)
{
  switch (mType)
    {
    case Type::Double:
      mpData = DataPointer(new C_FLOAT64(*static_cast< C_FLOAT64 * >(pValue)), &cpsapiFactory::free_unique< C_FLOAT64 >);
      break;

    case Type::Int32:
      mpData = DataPointer(new C_INT32(*static_cast< C_INT32 * >(pValue)), &cpsapiFactory::free_unique< C_INT32 >);
      break;

    case Type::UnsignedInt32:
      mpData = DataPointer(new unsigned C_INT32(*static_cast< unsigned C_INT32 * >(pValue)), &cpsapiFactory::free_unique< unsigned C_INT32 >);
      break;

    case Type::SizeType:
      mpData = DataPointer(new size_t(*static_cast< size_t * >(pValue)), &cpsapiFactory::free_unique< size_t >);
      break;

    case Type::Bool:
      mpData = DataPointer(new bool(*static_cast< bool * >(pValue)), &cpsapiFactory::free_unique< bool >);
      break;

    case Type::String:
      mpData = DataPointer(new std::string(*static_cast< std::string * >(pValue)), &cpsapiFactory::free_unique< std::string >);
      break;

    case Type::CommonName:
      mpData = DataPointer(new CRegisteredCommonName(*static_cast< CRegisteredCommonName * >(pValue)), &cpsapiFactory::free_unique< CRegisteredCommonName >);
      break;

    case Type::Data:
      mpData = DataPointer(new cpsapiDataVector(*static_cast< cpsapiDataVector * >(pValue)), &cpsapiFactory::free_unique< cpsapiDataVector >);
      break;

    case Type::Object:
      mpData = cpsapiFactory::make_unique< cpsapiObject >(*static_cast< const cpsapiObject * >(pValue));
      break;

    case Type::__SIZE:
      break;
    }
}

cpsapiData::cpsapiData(const cpsapiData & src)
  : mType(src.mType)
  , mpData(src.copyData())
{}

cpsapiData::~cpsapiData()
{}

C_FLOAT64 cpsapiData::toDouble() const
{
  switch (mType)
  {
    case Type::Double:
      return *static_cast< const C_FLOAT64 * >(mpData.get());
      break;

    case Type::Int32:
      return *static_cast< const C_INT32 * >(mpData.get());
      break;
      
    case Type::UnsignedInt32:
      return *static_cast< const unsigned C_INT32 * >(mpData.get());
      break;
      
    case Type::Bool:
      return *static_cast< const bool * >(mpData.get());
      break;
      
    case Type::SizeType:
      return *static_cast< const size_t * >(mpData.get());
      break;
      
    default:
      break;
  }

  return std::numeric_limits< C_FLOAT64 >::quiet_NaN();
}

C_INT32 cpsapiData::toInt32() const
{
  switch (mType)
  {
    case Type::Double:
      return *static_cast< const C_FLOAT64 * >(mpData.get());
      break;

    case Type::Int32:
      return *static_cast< const C_INT32 * >(mpData.get());
      break;
      
    case Type::UnsignedInt32:
      return *static_cast< const unsigned C_INT32 * >(mpData.get());
      break;
      
    case Type::Bool:
      return *static_cast< const bool * >(mpData.get());
      break;

    case Type::SizeType:
      return *static_cast< const size_t * >(mpData.get());
      break;
      
    default:
      break;
  }

  return 0;
}

unsigned C_INT32 cpsapiData::toUnsignedInt32() const
{
  switch (mType)
  {
    case Type::Double:
      return *static_cast< const C_FLOAT64 * >(mpData.get());
      break;

    case Type::Int32:
      return *static_cast< const C_INT32 * >(mpData.get());
      break;
      
    case Type::UnsignedInt32:
      return *static_cast< const unsigned C_INT32 * >(mpData.get());
      break;
      
    case Type::Bool:
      return *static_cast< const bool * >(mpData.get());
      break;
      
    case Type::SizeType:
      return *static_cast< const size_t * >(mpData.get());
      break;
      
    default:
      break;
  }

  return 0;
}

size_t cpsapiData::toSizeType() const
{
  switch (mType)
  {
    case Type::Double:
      return *static_cast< const C_FLOAT64 * >(mpData.get());
      break;

    case Type::Int32:
      return *static_cast< const C_INT32 * >(mpData.get());
      break;
      
    case Type::UnsignedInt32:
      return *static_cast< const unsigned C_INT32 * >(mpData.get());
      break;
      
    case Type::Bool:
      return *static_cast< const bool * >(mpData.get());
      break;
      
    case Type::SizeType:
      return *static_cast< const size_t * >(mpData.get());
      break;
      
    default:
      break;
  }

  return 0;
}

bool cpsapiData::toBool() const
{
  switch (mType)
  {
    case Type::Double:
      return *static_cast< const C_FLOAT64 * >(mpData.get());
      break;

    case Type::Int32:
      return *static_cast< const C_INT32 * >(mpData.get());
      break;
      
    case Type::UnsignedInt32:
      return *static_cast< const unsigned C_INT32 * >(mpData.get());
      break;
      
    case Type::Bool:
      return *static_cast< const bool * >(mpData.get());
      break;
      
    case Type::SizeType:
      return *static_cast< const size_t * >(mpData.get());
      break;
      
    default:
      break;
  }

  return 0;
}
std::string cpsapiData::toString() const
{
  switch (mType)
  {
    case Type::String:
      return *static_cast< const std::string * >(mpData.get());
      break;

    case Type::CommonName:
      return *static_cast< const CCommonName * >(mpData.get());
      break;
      
    default:
      break;
  }

  return std::string();
}

CRegisteredCommonName cpsapiData::toCommonName() const
{
  switch (mType)
  {
    case Type::String:
      return *static_cast< const std::string * >(mpData.get());
      break;

    case Type::CommonName:
      return *static_cast< const CCommonName * >(mpData.get());
      break;
      
    default:
      break;
  }

  return std::string();
}

const cpsapiDataVector & cpsapiData::toData() const
{
  static const cpsapiDataVector Invalid;

  switch (mType)
  {
    case Type::Data:
      return *static_cast< const cpsapiDataVector * >(mpData.get());
      break;

    default:
      break;
  }

  return Invalid;
}  

const cpsapiData::Type & cpsapiData::getType() const
{
  return mType;
}

cpsapiData::DataPointer cpsapiData::copyData() const
{
  switch (mType)
    {
    case Type::Double:
      return DataPointer(new C_FLOAT64(*static_cast< C_FLOAT64 * >(mpData.get())), &cpsapiFactory::free_unique< C_FLOAT64 >);
      break;

    case Type::Int32:
      return DataPointer(new C_INT32(*static_cast< C_INT32 * >(mpData.get())), &cpsapiFactory::free_unique< C_INT32 >);
      break;

    case Type::UnsignedInt32:
      return DataPointer(new unsigned C_INT32(*static_cast< unsigned C_INT32 * >(mpData.get())), &cpsapiFactory::free_unique< unsigned C_INT32 >);
      break;

    case Type::SizeType:
      return DataPointer(new size_t(*static_cast< size_t * >(mpData.get())), &cpsapiFactory::free_unique< size_t >);
      break;

    case Type::Bool:
      return DataPointer(new bool(*static_cast< bool * >(mpData.get())), &cpsapiFactory::free_unique< bool >);
      break;

    case Type::String:
      return DataPointer(new std::string(*static_cast< std::string * >(mpData.get())), &cpsapiFactory::free_unique< std::string >);
      break;

    case Type::CommonName:
      return DataPointer(new CRegisteredCommonName(*static_cast< CRegisteredCommonName * >(mpData.get())), &cpsapiFactory::free_unique< CRegisteredCommonName >);
      break;

    case Type::Data:
      return DataPointer(new cpsapiDataVector(*static_cast< cpsapiDataVector * >(mpData.get())), &cpsapiFactory::free_unique< cpsapiDataVector >);
      break;

    case Type::Object:
      return cpsapiFactory::make_unique< cpsapiObject >(*static_cast< const cpsapiObject * >(mpData.get()));
      break;

    case Type::__SIZE:
      break;
    }

}
