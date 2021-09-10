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

  struct _copy
  {
    virtual ~_copy()
    {}

    virtual cpsapiObject * operator()(const cpsapiObject &) = 0;
  };

  template < class CType >
  struct copyTemplate : public _copy
  {
    virtual ~copyTemplate()
    {}

    virtual cpsapiObject * operator()(const cpsapiObject & object)
    {
      return new CType(*static_cast< const CType *>(&object));
    }
  };


public:
  typedef void (*free_unique_t)(void *);

  template < class Type >
  static void free_unique(void *);

  struct TypeInfo
  {
    std::type_index cpsapiClass;
    std::shared_ptr< create > cpsapiCreate;
    std::shared_ptr< _copy > cpsapiCopy;
    std::type_index copasiClass;
    std::string copasiString;

    template < class Target, class Source >
    static void initProtected()
    {
      insert(TypeInfo(std::type_index(typeid(Target)),
                      nullptr,
                      nullptr,
                      std::type_index(typeid(Source)),
                      typeid(Source).name()));
    }

    template < class Target, class Source >
    static void init()
    {
      insert(TypeInfo(std::type_index(typeid(Target)),
                      std::make_shared< createTemplate< Target, Source > >(),
                      std::make_shared< copyTemplate< Target > >(),
                      std::type_index(typeid(Source)),
                      typeid(Source).name()));
    }

    TypeInfo(const std::type_index & cpsapiClass = std::type_index(typeid(cpsapiObject)),
             std::shared_ptr< create > cpsapiCreate = nullptr,
             std::shared_ptr< _copy > cpsapiCopy = nullptr,
             const std::type_index & copasiClass = std::type_index(typeid(CDataObject)),
             const std::string copasiString = "unknown");

    TypeInfo & operator=(const TypeInfo & rhs);
  };

private:
  typedef std::map< std::type_index, TypeInfo > CopasiMap;

  static CopasiMap copasiMap;

  static void insert(const TypeInfo & typeInfo);

public:
  static void init();

  static const TypeInfo & info(const std::type_index & typeIndex);

  static const TypeInfo & info(CDataObject * pObject);

  static const TypeInfo & info(const cpsapiObject & object);

  template < class CType >
  static std::shared_ptr< CType > make_shared(CDataObject * pFrom);

  template < class CType >
  static std::shared_ptr< CType > make_shared(const cpsapiObject & from);

  template < class CType >
  static std::unique_ptr< CType, free_unique_t > make_unique(CDataObject * pFrom);

  template < class CType >
  static std::unique_ptr< CType, free_unique_t > make_unique(const cpsapiObject & from);

  static cpsapiObject * copy(const cpsapiObject & object);

private:
  static cpsapiObject * make(CDataObject * pObject, const TypeInfo * pTypeInfo = nullptr);
};

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
inline void cpsapiFactory::free_unique< void >(void * pointer)
{}

template < class Type >
void cpsapiFactory::free_unique(void * pointer)
{
  delete static_cast< Type * >(pointer);
}

CPSAPI_NAMESPACE_END