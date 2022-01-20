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

#pragma once

#include <string>
#include <memory>

#pragma GCC diagnostic push
#include <copasi/core/CCore.h>
#include <copasi/core/CEnumAnnotation.h>
#include <copasi/core/CRegisteredCommonName.h>
#pragma GCC diagnostic pop

CPSAPI_NAMESPACE_BEGIN

typedef std::vector< cpsapiData > cpsapiDataVector;

class cpsapiData
{
public:
  enum struct Type
  {
    Double,
    Int32,
    UnsignedInt32,
    SizeType,
    Bool,
    String,
    CommonName,
    Object,
    Data,
    __SIZE
  };

  typedef std::unique_ptr< void, cpsapiFactory::free_unique_t > DataPointer;

  // static const CEnumAnnotation< std::string, Type > Name;

  cpsapiData();

  cpsapiData(const C_FLOAT64 & value);

  cpsapiData(const C_INT32 & value);

  cpsapiData(const unsigned C_INT32 & value);

  cpsapiData(const size_t & value);

  cpsapiData(const bool & value);

  cpsapiData(const std::string & value);

  cpsapiData(const char * value);

  cpsapiData(const CCommonName & value);

  cpsapiData(const cpsapiDataVector & value);

  template < class Object > cpsapiData(const Object & value);

  cpsapiData(const Type & type, void * pValue);

  cpsapiData(const cpsapiData & src);

  cpsapiData & operator = (const cpsapiData & rhs);
  
  ~cpsapiData();

  C_FLOAT64 toDouble() const;

  C_INT32 toInt32() const;

  unsigned C_INT32 toUnsignedInt32() const;

  size_t toSizeType() const;

  bool toBool() const;

  std::string toString() const;

  CCommonName toCommonName() const;

  const cpsapiDataVector & toData() const;

  template < class Object > Object toObject() const;

  const Type & getType() const;

private:
  DataPointer copyData() const;

  template < class CType > void assign(const CType * pValue, const Type & type);

  template < class CType > static void free_unique(void *);

  Type mType;

  DataPointer mpData;
};

template < class Object > 
cpsapiData::cpsapiData(const Object & value)
  : mType(Type::Object)
  , mpData(cpsapiFactory::make_unique< Object >(value))
{}

template <> inline
void cpsapiData::assign(const cpsapiObject * pValue, const cpsapiData::Type & type)
{
  mType = type;
  mpData = cpsapiFactory::make_unique< cpsapiObject >(*pValue);
}

template < class CType > 
void cpsapiData::assign(const CType * pValue, const cpsapiData::Type & type)
{
  if (mType == type)
    *static_cast< CType * >(mpData.get()) = *pValue;
  else
    {
      mType = type;
      mpData = std::unique_ptr< CType, cpsapiFactory::free_unique_t >(new CType(*pValue), &cpsapiFactory::free_unique< Type >);
    }
}

template <>
inline cpsapiObject cpsapiData::toObject() const
{
  if (mType == Type::Object)
    return *static_cast< cpsapiObject * >(mpData.get());

  return cpsapiObject(nullptr, cpsapiObject::Type::Object);
}

template < class Object > 
Object cpsapiData::toObject() const
{
  if (mType == Type::Object
      && dynamic_cast< Object * >(static_cast< cpsapiObject * >(mpData.get())) != nullptr)
    return *static_cast< Object * >(mpData.get());

  return Object(nullptr);
}


CPSAPI_NAMESPACE_END
