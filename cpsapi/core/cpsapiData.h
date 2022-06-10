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
#include <variant>

#pragma GCC diagnostic push
#include <copasi/core/CCore.h>
#include <copasi/core/CEnumAnnotation.h>
#include <copasi/core/CRegisteredCommonName.h>
#pragma GCC diagnostic pop

CPSAPI_NAMESPACE_BEGIN

class cpsapiData : protected std::variant< C_FLOAT64, C_INT32, C_UINT32, size_t, bool, std::string, CRegisteredCommonName, std::shared_ptr< cpsapiObject >, std::vector< cpsapiData > >
{
  typedef std::variant< C_FLOAT64, C_INT32, C_UINT32, size_t, bool, std::string, CRegisteredCommonName, std::shared_ptr< cpsapiObject >, std::vector< cpsapiData > > base;

public:
  enum class Type
  {
    Double = 0,
    Int32,
    UnsignedInt32,
    SizeType,
    Bool,
    String,
    CommonName,
    Object,
    Vector,
    __SIZE
  };

  typedef std::vector< cpsapiData > Vector;

  // static const CEnumAnnotation< std::string, Type > Name;

  cpsapiData();

  cpsapiData(const C_FLOAT64 & value);

  cpsapiData(const C_INT32 & value);

  cpsapiData(const C_UINT32 & value);

  cpsapiData(const size_t & value);

  cpsapiData(const bool & value);

  cpsapiData(const std::string & value);

  cpsapiData(const char * value);

  cpsapiData(const CCommonName & value);

  cpsapiData(const Vector & value);

  template < class Object > cpsapiData(const Object & value);

  cpsapiData(const Type & type, void * pValue);

  cpsapiData(const cpsapiData & src);

  cpsapiData & operator = (const cpsapiData & rhs);
  
  ~cpsapiData();

  C_FLOAT64 toDouble() const;

  C_INT32 toInt32() const;

  C_UINT32 toUnsignedInt32() const;

  size_t toSizeType() const;

  bool toBool() const;

  std::string toString() const;

  CCommonName toCommonName() const;

  const Vector & toData() const;

  template < class Object > Object toObject() const;

  Type getType() const;
};

template < class Object > 
cpsapiData::cpsapiData(const Object & value)
  : base(std::make_shared< Object >(value))
{}

template <>
inline cpsapiObject cpsapiData::toObject() const
{
  if (std::holds_alternative< std::shared_ptr< cpsapiObject > >(*this))
    return *std::get< std::shared_ptr< cpsapiObject > >(*this);

  return cpsapiObject(nullptr, cpsapiObjectData::Type::Object);
}

template < class Object > 
Object cpsapiData::toObject() const
{
  if (std::holds_alternative< std::shared_ptr< cpsapiObject > >(*this)
      && std::dynamic_pointer_cast< Object >(std::get< std::shared_ptr< cpsapiObject > >(*this)) != nullptr)
    return *std::static_pointer_cast< Object >(std::get< std::shared_ptr< cpsapiObject > >(*this));

  return Object(nullptr);
}


CPSAPI_NAMESPACE_END
