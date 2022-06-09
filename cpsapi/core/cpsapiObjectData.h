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

#include <map>
#include <memory>
#include <string>

#include <copasi/core/CEnumAnnotation.h>

class CDataObject;

#include "cpsapi/cpsapiConfig.h"

CPSAPI_NAMESPACE_BEGIN

class cpsapiObject;

/**
 * The cpsapiObject class is the base class for all COPASI CDataObjects exposed in the cpsapi.
 */
class cpsapiObjectData
{
friend class cpsapiObject;

public:
  enum class Type
  {
    Object,
    Value,
    Container,
    Vector,
    ModelEntity,
    Model,
    Compartment,
    Species,
    GlobalQuantity,
    Reaction,
    ReactionParameter,
    Event,
    EventAssignment,
    DataModel,
    Parameter,
    Group,
    Method,
    Problem,
    Task,
    __SIZE
  };

  static const CEnumAnnotation< std::string, Type > TypeName;

  typedef std::shared_ptr< cpsapiObjectData > Pointer;

  typedef std::map< const CDataObject *, Pointer > Map;

  static Map Manager;

  static void release();

  static void erase(const CDataObject * pObject);

  static void deleted(const CDataObject * pObject);

  cpsapiObjectData(const cpsapiObjectData &) = default;

  cpsapiObjectData(CDataObject * pObject, const Type & type);

  virtual ~cpsapiObjectData() {}

  /**
   * Assert that mpData points of the proper class 
   * 
   * @tparam @tparam class CType cpsapiObject or a derived class
   * @param CDataObject * pObject 
   */
  template < class CType >
  static void assertDataType(Pointer & baseData);

protected:
  CDataObject * mpObject = nullptr;

  Type mType = Type::Object;

private:
  // We make clear Base class needs to be inherited
  cpsapiObjectData() = default;
  cpsapiObjectData(cpsapiObjectData &&) = default;
};

template <>
inline void cpsapiObjectData::assertDataType< cpsapiObject > (Pointer & data )
{
  if (data->mpObject != nullptr)
    {
      Map::iterator found = Manager.insert(std::make_pair(data->mpObject, data)).first;
      data = found->second;
    }
}

template < class CType >
void cpsapiObjectData::assertDataType(Pointer & data)
{
  if (data->mpObject != nullptr)
    {
      Map::iterator found = Manager.insert(std::make_pair(data->mpObject, nullptr)).first;

      if (!std::dynamic_pointer_cast< typename CType::Data >(found->second))
        found->second = std::make_shared< typename CType::Data >(*std::static_pointer_cast< typename CType::base::Data >(data));

      data = found->second;
    }
  else if (!std::dynamic_pointer_cast< typename CType::Data >(data))
    {
      data = std::make_shared< typename CType::Data >(*std::static_pointer_cast< typename CType::base::Data >(data));
    }
}

CPSAPI_NAMESPACE_END
