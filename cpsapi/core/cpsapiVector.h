#pragma once

#include "cpsapi/core/cpsapiContainer.h"

#include <copasi/core/CDataVector.h>

CPSAPI_NAMESPACE_BEGIN

template < class Object > class cpsapiVector : public cpsapiContainer
{
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

public:
  class iterator: public wrapped::iterator
  {
  public:
    iterator()
      : wrapped::iterator()
      , mObject(nullptr)
    {}

    iterator(const iterator & src)
      : wrapped::iterator(src)
      , mObject(src.mObject)
    {}

    iterator(const typename wrapped::iterator & src)
      : wrapped::iterator(src)
      , mObject(nullptr)
    {}

    ~iterator() {}

    Object & operator*() const
    {
      mObject = Object(&wrapped::iterator::operator*());
      return mObject;
    }

    Object * operator->() const
    {
      mObject = Object(&wrapped::iterator::operator*());
      return &mObject;
    }

    operator Object * () const
    {
      mObject = Object(&wrapped::iterator::operator*());
      return &mObject;
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

    private:
      mutable Object mObject;
  };

  /**
   * Specific constructor
   * @param wrapped * pWrapped (default: nullptr)
   * @param const Type & type (default: Type::cpsapiVector)
  */
  cpsapiVector(wrapped * pWrapped = nullptr, const Type & type = Type::cpsapiVector);

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
};

template < class Object > 
const typename cpsapiVector< Object >::Properties cpsapiVector< Object >::SupportedProperties = {};

template < class Object > 
cpsapiVector< Object >::cpsapiVector(wrapped * pWrapped, const cpsapiObject::Type & type)
  : base(pWrapped, type)
{}

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

  visitor.visit(this, Type::cpsapiVector);
  base::accept(visitor);
}

template < class Object > 
size_t cpsapiVector< Object >::size() const
{
  return static_cast< wrapped * >(getObject())->size();
}

template < class Object > 
typename cpsapiVector< Object >::iterator cpsapiVector< Object >::begin()
{
  return iterator(static_cast< wrapped * >(getObject())->begin());
}

template < class Object > 
typename cpsapiVector< Object >::iterator cpsapiVector< Object >::end()
{
  return iterator(static_cast< wrapped * >(getObject())->end());
}

template < class Object > 
Object & cpsapiVector< Object >::operator[](const size_t & index)
{
  static Object NotFound(nullptr);

  if (index < size())
    return Object(&static_cast< wrapped * >(getObject())->operator[](index));

  return NotFound;
}

template < class Object > 
Object & cpsapiVector< Object >::operator[](const std::string & name)
{
  return operator[](static_cast< wrapped * >(getObject())->getIndex(name));
};

CPSAPI_NAMESPACE_END