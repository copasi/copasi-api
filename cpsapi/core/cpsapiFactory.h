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

#pragma once

#include <map>
#include <typeindex>
#include <memory>

#pragma GCC diagnostic push
#include <copasi/CopasiDataModel/CDataModel.h>
#include <copasi/core/CDataObjectReference.h>
#include <copasi/utilities/CCopasiMethod.h>
#include <copasi/utilities/CCopasiProblem.h>
#include <copasi/utilities/CCopasiTask.h>
#pragma GCC diagnostic pop

CPSAPI_NAMESPACE_BEGIN

class cpsapiFactory
{
public:
  typedef void (*free_unique_t)(void *);

  template < class Type >
  static void free_unique(void *);

  struct PartInterface
  {
    virtual ~PartInterface() {}

    virtual cpsapiObject * create(CDataObject * /* pDataObject */) const = 0;

    virtual cpsapiObject * copy(const cpsapiObject & /* src */) const = 0;

    virtual const std::type_info & cpsapiType() const = 0;

    virtual const std::type_info & copasiType() const = 0;
  };

  template < class cpsapi, class copasi >
  struct Part : public PartInterface
  {
    virtual ~Part() {}

    virtual cpsapiObject * create(CDataObject * pDataObject) const override;

    virtual cpsapiObject * copy(const cpsapiObject & src) const override;

    virtual const std::type_info & cpsapiType() const override;

    virtual const std::type_info & copasiType() const override;
  };


private:
  typedef std::map< std::type_index, std::shared_ptr< PartInterface > > CopasiMap;

  static CopasiMap copasiMap;

  template < class cpsapi, class copasi >
  static void insert();

public:
  static void init();

  static const PartInterface & info(const std::type_index & typeIndex);

  static const PartInterface & info(CDataObject * pObject);

  static const PartInterface & info(const cpsapiObject & object);

  template < class CType >
  static std::shared_ptr< CType > make_shared(CDataObject * pFrom);

  template < class CType >
  static std::shared_ptr< CType > make_shared(const cpsapiObject & from);

  template < class CType >
  static std::unique_ptr< CType, free_unique_t > make_unique(CDataObject * pFrom);

  template < class CType >
  static std::unique_ptr< CType, free_unique_t > make_unique(const cpsapiObject & from);

  template < class Visitor >
  static void accept(Visitor & visitor, CDataObject * pObject);

  static cpsapiObject * copy(const cpsapiObject & object);

  static cpsapiObject * create(CDataObject * pFrom);

  static CDataValue::Type getDataType(const CObjectInterface * pObject);

  template< class Function >
  static void callDerived(CDataObject * pObject);

private:
  static cpsapiObject * make(CDataObject * pObject, const PartInterface * pInfo = nullptr);
};

// virtual
template <>
inline cpsapiObject * cpsapiFactory::Part< void, void >::create(CDataObject * /* pDataObject */) const
{
  return nullptr;
}

template < class cpsapi, class copasi >
cpsapiObject * cpsapiFactory::Part< cpsapi, copasi >::create(CDataObject * pDataObject) const
{
  return new cpsapi(static_cast< copasi * >(pDataObject));
}

// virtual
template <>
inline cpsapiObject * cpsapiFactory::Part< void, void >::copy(const cpsapiObject & /* src */) const
{
  return nullptr;
}

// virtual
template < class cpsapi, class copasi >
cpsapiObject * cpsapiFactory::Part< cpsapi, copasi >::copy(const cpsapiObject & src) const
{
  return new cpsapi(*static_cast< const cpsapi * >(&src));
}

// virtual
template < class cpsapi, class copasi >
const std::type_info & cpsapiFactory::Part< cpsapi, copasi >::cpsapiType() const
{
  return typeid(cpsapi);
}

// virtual
template < class cpsapi, class copasi >
const std::type_info & cpsapiFactory::Part< cpsapi, copasi >::copasiType() const
{
  return typeid(copasi);
}

// static
template < class cpsapi, class copasi >
void cpsapiFactory::insert()
{
  std::shared_ptr< Part< cpsapi, copasi > > InfoPointer = std::make_shared< Part< cpsapi, copasi > >();

  copasiMap.insert(std::make_pair(std::type_index(typeid(cpsapi)), InfoPointer));
  copasiMap.insert(std::make_pair(std::type_index(typeid(copasi)), InfoPointer));
}

template <>
inline std::shared_ptr< cpsapiObject > cpsapiFactory::make_shared(CDataObject * pFrom)
{
  return std::shared_ptr< cpsapiObject >(make(pFrom));
}

template <>
inline std::shared_ptr< cpsapiObject > cpsapiFactory::make_shared(const cpsapiObject & from)
{
  return std::shared_ptr< cpsapiObject >(copy(from));
}

template < class CType >
inline std::shared_ptr< CType > cpsapiFactory::make_shared(CDataObject * pFrom)
{
  return std::shared_ptr< CType >(dynamic_cast< CType * >(make(pFrom, pFrom != nullptr ? nullptr : &info(std::type_index(typeid(CType))))));
}

template < class CType >
inline std::shared_ptr< CType > cpsapiFactory::make_shared(const cpsapiObject & from)
{
  return std::shared_ptr< CType >(dynamic_cast< CType * >(copy(from)));
}

template <>
inline std::unique_ptr< cpsapiObject, cpsapiFactory::free_unique_t > cpsapiFactory::make_unique(CDataObject * pFrom)
{
  return std::unique_ptr< cpsapiObject, cpsapiFactory::free_unique_t >(make(pFrom), &free_unique< cpsapiObject >);
}

template <>
inline std::unique_ptr< cpsapiObject, cpsapiFactory::free_unique_t > cpsapiFactory::make_unique(const cpsapiObject & from)
{
  return std::unique_ptr< cpsapiObject, cpsapiFactory::free_unique_t >(copy(from), &free_unique< cpsapiObject >);
}

template < class CType >
std::unique_ptr< CType, cpsapiFactory::free_unique_t > cpsapiFactory::make_unique(CDataObject * pFrom)
{
  return std::unique_ptr< CType, cpsapiFactory::free_unique_t >(dynamic_cast< CType * >(make(pFrom, pFrom != nullptr ? nullptr : &info(std::type_index(typeid(CType))))), &free_unique< CType >);
}

template < class CType >
std::unique_ptr< CType, cpsapiFactory::free_unique_t > cpsapiFactory::make_unique(const cpsapiObject & from)
{
  return std::unique_ptr< CType, cpsapiFactory::free_unique_t >(dynamic_cast< CType * >(copy(from)), &free_unique< CType >);
}

template <>
inline void cpsapiFactory::free_unique< void >(void * /* pointer */)
{}

template < class Type >
void cpsapiFactory::free_unique(void * pointer)
{
  delete static_cast< Type * >(pointer);
}


CPSAPI_NAMESPACE_END