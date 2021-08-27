#pragma once

#include <map>
#include <typeindex>
#include <memory>

class CDataObject;

CPSAPI_NAMESPACE_BEGIN

class cpsapiFactory
{
private:
  struct create
  {
    virtual ~create()
    {}

    virtual cpsapiObject * operator()(CDataObject *) = 0;
  };

  template < class Target, class Source >
  struct createTemplate : public create
  {
    virtual ~createTemplate()
    {}

    virtual cpsapiObject * operator()(CDataObject * pDataObject)
    {
      return new Target(static_cast< Source * >(pDataObject));
    }
  };

public:
  typedef void (* free_unique_t)(void *);

  template < class Type > static void free_unique(void *);

  struct TypeInfo
  {
    std::type_index cpsapiClass;
    cpsapiObject::Type cpsapiType;
    std::shared_ptr< create > cpsapiCreate;
    std::type_index copasiClass;
    std::string copasiString;

    TypeInfo(const std::type_index & cpsapiClass = std::type_index(typeid(cpsapiObject)),
             cpsapiObject::Type cpsapiType = cpsapiObject::Type::__SIZE,
             std::shared_ptr< create > cpsapiCreate = nullptr,
             const std::type_index & copasiClass = std::type_index(typeid(CDataObject)),
             const std::string copasiString = "unknown");

    TypeInfo & operator=(const TypeInfo & rhs);
  };

private:
  typedef std::map< std::type_index, TypeInfo > CopasiMap;

  static CopasiMap copasiMap;

  typedef std::multimap< cpsapiObject::Type, TypeInfo > CpsapiMap;

  typedef std::pair< cpsapiFactory::CpsapiMap::const_iterator, cpsapiFactory::CpsapiMap::const_iterator > Range;

  static CpsapiMap cpsapiMap;

  static void insert(const TypeInfo & typeInfo);

public:
  static void init();

  static const TypeInfo & info(const std::type_index & typeIndex);

  static const TypeInfo & info(CDataObject * pObject);

  static const TypeInfo & info(const cpsapiObject & object);
  
  static Range info(const cpsapiObject::Type & type);
  
  template < class CType > 
  static std::shared_ptr< CType > make_shared(CDataObject * pFrom);

  template < class CType > 
  static std::shared_ptr< CType > make_shared(const cpsapiObject & from);

  template < class CType > 
  static std::unique_ptr< CType, free_unique_t > make_unique(CDataObject * pFrom);

  template < class CType > 
  static std::unique_ptr< CType, free_unique_t > make_unique(const cpsapiObject & from);

private:
  static cpsapiObject * make(CDataObject * pObject, const TypeInfo * pTypeInfo = nullptr);

};

template <> inline
std::shared_ptr< cpsapiObject > cpsapiFactory::make_shared(CDataObject * pFrom)
{
  return std::shared_ptr< cpsapiObject >(make(pFrom));
}

template <> inline
std::shared_ptr< cpsapiObject > cpsapiFactory::make_shared(const cpsapiObject & from)
{
  return std::shared_ptr< cpsapiObject >(make(from.getObject(), from ? nullptr : &info(from)));
}

template < class CType > inline
std::shared_ptr< CType > cpsapiFactory::make_shared(CDataObject * pFrom)
{
  return std::shared_ptr< CType >(dynamic_cast< CType * >(make(pFrom, pFrom != nullptr ? nullptr : &info(std::type_index(typeid(CType))))));
}

template < class CType > inline
std::shared_ptr< CType > cpsapiFactory::make_shared(const cpsapiObject & from)
{
  return std::shared_ptr< CType >(dynamic_cast< CType * >(make(from.getObject(), from ? nullptr : &info(std::type_index(typeid(CType))))));
}

template <> inline
std::unique_ptr< cpsapiObject, cpsapiFactory::free_unique_t > cpsapiFactory::make_unique(CDataObject * pFrom)
{
  return std::unique_ptr< cpsapiObject, cpsapiFactory::free_unique_t >(make(pFrom), &free_unique< cpsapiObject >);
}

template <> inline
std::unique_ptr< cpsapiObject, cpsapiFactory::free_unique_t >cpsapiFactory::make_unique(const cpsapiObject & from)
{
  return std::unique_ptr< cpsapiObject, cpsapiFactory::free_unique_t >(make(from.getObject(), from ? nullptr : &info(from)), &free_unique< cpsapiObject >);
}

template < class CType > 
std::unique_ptr< CType, cpsapiFactory::free_unique_t > cpsapiFactory::make_unique(CDataObject * pFrom)
{
  return std::unique_ptr< CType, cpsapiFactory::free_unique_t >(dynamic_cast< CType * >(make(pFrom, pFrom != nullptr ? nullptr : &info(std::type_index(typeid(CType))))), &free_unique< CType >);
}

template < class CType > 
std::unique_ptr< CType, cpsapiFactory::free_unique_t > cpsapiFactory::make_unique(const cpsapiObject & from)
{
  return std::unique_ptr< CType, cpsapiFactory::free_unique_t >(dynamic_cast< CType * >(make(from.getObject(), from ? nullptr : &info(std::type_index(typeid(CType))))), &free_unique< CType >);
}

template <> inline
void cpsapiFactory::free_unique< void >(void * pointer)
{}

template < class Type >
void cpsapiFactory::free_unique(void * pointer)
{
  delete static_cast< Type * >(pointer);
}


CPSAPI_NAMESPACE_END