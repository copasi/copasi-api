#pragma once

#include <string>
#include <memory>

#include <copasi/core/CCore.h>
#include <copasi/core/CEnumAnnotation.h>
#include <copasi/core/CRegisteredCommonName.h>

CPSAPI_NAMESPACE_BEGIN

class cpsapiVariant
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
    __SIZE
  };

  typedef std::unique_ptr< void, cpsapiFactory::free_unique_t > DataPointer;

  static const CEnumAnnotation< std::string, Type > Name;

  cpsapiVariant();

  cpsapiVariant(const C_FLOAT64 & value);

  cpsapiVariant(const C_INT32 & value);

  cpsapiVariant(const unsigned C_INT32 & value);

  cpsapiVariant(const size_t & value);

  cpsapiVariant(const bool & value);

  cpsapiVariant(const std::string & value);

  cpsapiVariant(const char * value);

  cpsapiVariant(const CRegisteredCommonName & value);

  template < class Object > cpsapiVariant(const Object & value);

  cpsapiVariant(const cpsapiVariant & src);

  ~cpsapiVariant();

  C_FLOAT64 toDouble() const;

  C_INT32 toInt32() const;

  unsigned C_INT32 toUnsignedInt32() const;

  size_t toSizeType() const;

  bool toBool() const;

  std::string toString() const;;

  CRegisteredCommonName toCommonName() const;

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
cpsapiVariant::cpsapiVariant(const Object & value)
  : mType(Type::Object)
  , mpData(cpsapiFactory::make_unique< Object >(value))
{}

template <> inline
void cpsapiVariant::assign(const cpsapiObject * pValue, const cpsapiVariant::Type & type)
{
  mType = type;
  mpData = cpsapiFactory::make_unique< cpsapiObject >(*pValue);
}

template < class CType > 
void cpsapiVariant::assign(const CType * pValue, const cpsapiVariant::Type & type)
{
  if (mType == type)
    *static_cast< CType * >(mpData.get()) = *pValue;
  else
    {
      mType = type;
      mpData = std::unique_ptr< CType, cpsapiFactory::free_unique_t >(new CType(*pValue), &cpsapiFactory::free_unique< Type >);
    }
}

template < class Object > 
Object cpsapiVariant::toObject() const
{
  if (mType == Type::Object
      && dynamic_cast< Object * >(static_cast< cpsapiObject * >(mpData.get())) != nullptr)
    return *static_cast< Object * >(mpData.get());

  return Object(nullptr);
}


CPSAPI_NAMESPACE_END
