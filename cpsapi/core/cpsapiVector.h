#pragma once

#include "cpsapi/core/cpsapiContainer.h"

#include <copasi/core/CDataVector.h>

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
    iterator() = delete;

    iterator(const iterator & src)
      : wrapped::iterator(src)
      , mMap(src.mMap)
    {}

    iterator(const typename wrapped::iterator & src, ObjectMap & map)
      : wrapped::iterator(src)
      , mMap(map)
    {}

    iterator(ObjectMap & map)
      : wrapped::iterator()
      , mMap(map)
    {}

    ~iterator() {}

    Object & operator*() const
    {
      typename Object::wrapped & Wrapped = wrapped::iterator::operator*();
      typename ObjectMap::iterator found = mMap.find(&Wrapped);

      if (found == mMap.end())
        found = mMap.insert(std::make_pair(&Wrapped, Object(&Wrapped))).first;

      return found->second;
    }

    Object * operator->() const
    {
      typename Object::wrapped & Wrapped = wrapped::iterator::operator*();
      typename ObjectMap::iterator found = mMap.find(&Wrapped);

      if (found == mMap.end())
        found = mMap.insert(std::make_pair(&Wrapped, Object(&Wrapped))).first;

      return &found->second;
    }

    operator Object * () const
    {
      typename Object::wrapped & Wrapped = wrapped::iterator::operator*();
      typename ObjectMap::iterator found = mMap.find(&Wrapped);

      if (found == mMap.end())
        found = mMap.insert(std::make_pair(&Wrapped, Object(&Wrapped))).first;

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
      return iterator(wrapped::iterator::operator+(n));
    }

    iterator & operator-=(const typename wrapped::iterator::difference_type & n)
    {
      wrapped::iterator::operator-=(n);
      return *this;
    }

    iterator operator-(const typename wrapped::iterator::difference_type & n) const
    {
      return iterator(wrapped::iterator::operator-(n));
    }

  protected:
    ObjectMap & mMap;
  };

  /**
   * Specific constructor
   * @param wrapped * pWrapped (default: nullptr)
   * @param const Type & type (default: Type::cpsapiVector)
  */
  cpsapiVector(wrapped * pWrapped = nullptr, const Type & type = Type::Vector);

  /**
   * Copy constructor
   * @param const cpsapiContainer & src
   */
  cpsapiVector(const cpsapiVector & src);

  /**
   * Destructor
   */
  virtual ~cpsapiVector();

  /**
   * Accept a visitor
   * @param cpsapiVisitor & visitor
   */
  virtual void accept(cpsapiVisitor & visitor) override;

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
cpsapiVector< Object >::cpsapiVector(wrapped * pWrapped, const cpsapiObject::Type & type)
  : base(pWrapped, type)
{
  assertData(Data(*std::static_pointer_cast< base::Data >(mpData)));
}

template < class Object > 
cpsapiVector< Object >::cpsapiVector(const cpsapiVector & src)
  : base(src)
{}

template < class Object > 
cpsapiVector< Object >::~cpsapiVector()
{}

template < class Object > 
void cpsapiVector< Object >::accept(cpsapiVisitor & visitor)
{
  if (!operator bool())
    return;

  visitor.visit(this, Type::Vector);
  base::accept(visitor);
}

template < class Object > 
size_t cpsapiVector< Object >::size() const
{
  if (!operator bool())
    return 0;

  return WRAPPED->size();
}

template < class Object > 
typename cpsapiVector< Object >::iterator cpsapiVector< Object >::begin()
{
  if (!operator bool())
    return iterator(DATA->mMap);

  return iterator(WRAPPED->begin(), DATA->mMap);
}

template < class Object > 
typename cpsapiVector< Object >::iterator cpsapiVector< Object >::end()
{
  if (!operator bool())
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
};

template < class Object > 
size_t cpsapiVector< Object >::index(const std::string & name) const
{
  if (static_cast< CDataVectorN < typename Object::wrapped > * >(getObject()) != nullptr)
    return static_cast< CDataVectorN < typename Object::wrapped > * >(getObject())->getIndex(name);

  return C_INVALID_INDEX;
}

CPSAPI_NAMESPACE_END