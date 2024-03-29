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

#include "cpsapi/core/cpsapiContainer.h"

#pragma GCC diagnostic push
#include <copasi/core/CDataVector.h>
#pragma GCC diagnostic pop

CPSAPI_NAMESPACE_BEGIN

template < class Object > class cpsapiVector : public cpsapiContainer
{
public:
  /**
   * Enumeration of the exposed properties
   */ 
  enum class Property {};

  /**
   * Static set of supported properties
   */
  static const Properties SupportedProperties;

  /**
   * The class
   */
  typedef cpsapiVector< Object > self;

  /**
   * The base class
   */
  typedef cpsapiContainer base;

  /**
   * The wrapped COPASI class
   */
  typedef CDataVector< typename Object::wrapped > wrapped;

  /**
   * We need to keep the cpsapiObjects around since the iterator returns references or pointers. 
   */ 
  typedef std::map< typename Object::wrapped *, Object > ObjectMap;

  class Data : public base::Data
  {
  public:
    Data(const base::Data & data)
      : base::Data(data)
      , mMap()
    {}

    virtual ~Data() {}

    ObjectMap mMap;
  };


  class iterator: public wrapped::iterator
  {
  public:
    iterator()
      : wrapped::iterator()
      , mpMap(nullptr)
    {}
    
    iterator(const typename wrapped::iterator & src, ObjectMap & map)
      : wrapped::iterator(src)
      , mpMap(&map)
    {}

    iterator(ObjectMap & map)
      : wrapped::iterator()
      , mpMap(&map)
    {}

    ~iterator() {}

    Object & operator*() const
    {
      typename Object::wrapped & Wrapped = wrapped::iterator::operator*();
      typename ObjectMap::iterator found = mpMap->find(&Wrapped);

      if (found == mpMap->end())
        found = mpMap->insert(std::make_pair(&Wrapped, Object(&Wrapped))).first;

      return found->second;
    }

    Object * operator->() const
    {
      typename Object::wrapped & Wrapped = wrapped::iterator::operator*();
      typename ObjectMap::iterator found = mpMap->find(&Wrapped);

      if (found == mpMap->end())
        found = mpMap->insert(std::make_pair(&Wrapped, Object(&Wrapped))).first;

      return &found->second;
    }

    operator Object * () const
    {
      typename Object::wrapped & Wrapped = wrapped::iterator::operator*();
      typename ObjectMap::iterator found = mpMap->find(&Wrapped);

      if (found == mpMap->end())
        found = mpMap->insert(std::make_pair(&Wrapped, Object(&Wrapped))).first;

      return &found->second;
    }

    iterator & operator++()
    {
      wrapped::iterator::operator++();
      return *this;
    }

    iterator operator++(int)
    {
      return iterator(wrapped::iterator::operator++());
    }

    iterator & operator--()
    {
      wrapped::iterator::operator--();
      return *this;
    }

    iterator operator--(int)
    {
      return iterator(wrapped::iterator::operator--());
    }

    iterator & operator+=(const typename wrapped::iterator::difference_type & n)
    {
      wrapped::iterator::operator+=(n);
      return *this;
    }

    iterator operator+(const typename wrapped::iterator::difference_type & n) const
    {
      return iterator(wrapped::iterator::operator+(n), *mpMap);
    }

    iterator & operator-=(const typename wrapped::iterator::difference_type & n)
    {
      wrapped::iterator::operator-=(n);
      return *this;
    }

    iterator operator-(const typename wrapped::iterator::difference_type & n) const
    {
      return iterator(wrapped::iterator::operator-(n), *mpMap);
    }

  protected:
    ObjectMap * mpMap;
  };

  /**
   * Specific constructor
   * @param wrapped * pWrapped (default: nullptr)
   * @param const Type & type (default: Type::cpsapiVector)
  */
  cpsapiVector(wrapped * pWrapped = nullptr, const cpsapiObjectData::Type & type = cpsapiObjectData::Type::Vector);

  /**
   * Destructor
   */
  virtual ~cpsapiVector();

  /**
   * Accept the given visitor
   * 
   * @tparam Visitor 
   * @param Visitor & visitor 
   */
  template < typename Visitor > void accept(Visitor & visitor);

  size_t size() const;

  iterator begin();

  iterator end();

  Object & operator[](const size_t & index);

  Object & operator[](const std::string & name);

  size_t index(const std::string & name) const;
};

template < class Object > 
const typename cpsapiVector< Object >::Properties cpsapiVector< Object >::SupportedProperties = {};

template < class Object > 
cpsapiVector< Object >::cpsapiVector(wrapped * pWrapped, const cpsapiObjectData::Type & type)
  : base(pWrapped, type)
{
  cpsapiObjectData::assertDataType< cpsapiVector< Object > >(mpData);
}

template < class Object > 
cpsapiVector< Object >::~cpsapiVector()
{}

template < class Object > 
template < class Visitor >
void cpsapiVector< Object >::accept(Visitor & visitor)
{
  if (isValid())
    {
      cpsapiVisitor::acceptIfVisitable(visitor, this); 
      base::accept(visitor);
    }
}

template < class Object > 
size_t cpsapiVector< Object >::size() const
{
  if (!isValid())
    return 0;

  return WRAPPED->size();
}

template < class Object > 
typename cpsapiVector< Object >::iterator cpsapiVector< Object >::begin()
{
  if (!isValid())
    return iterator(DATA->mMap);

  return iterator(WRAPPED->begin(), DATA->mMap);
}

template < class Object > 
typename cpsapiVector< Object >::iterator cpsapiVector< Object >::end()
{
  if (!isValid())
    return iterator(DATA->mMap);

  return iterator(WRAPPED->end(), DATA->mMap);
}

template < class Object > 
Object & cpsapiVector< Object >::operator[](const size_t & index)
{
  static Object NotFound(nullptr);

  if (index < size())
    return *(begin() += index);

  return NotFound;
}

template < class Object > 
Object & cpsapiVector< Object >::operator[](const std::string & name)
{
  static Object NotFound(nullptr);

  if (operator bool())
    return operator[](index(name));

  return NotFound;
}

template < class Object > 
size_t cpsapiVector< Object >::index(const std::string & name) const
{
  if (dynamic_cast< CDataVectorN < typename Object::wrapped > * >(getObject()) != nullptr)
    return static_cast< CDataVectorN < typename Object::wrapped > * >(getObject())->getIndex(name);

  return C_INVALID_INDEX;
}

CPSAPI_NAMESPACE_END