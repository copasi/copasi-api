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
  enum struct Variant
  {
    cpsapiDouble,
    cpsapiInt32,
    cpsapiUnsignedInt32,
    cpsapiSizeType,
    cpsapiBool,
    cpsapiString,
    cpsapiCommonName,
    cpsapiObject,
    cpsapiVector,
    __SIZE
  };

  static const CEnumAnnotation< std::string, Variant > Name;

  cpsapiVariant() = delete;

  cpsapiVariant(const C_FLOAT64 & value);

  cpsapiVariant(const C_INT32 & value);

  cpsapiVariant(const unsigned C_INT32 & value);

  cpsapiVariant(const size_t & value);

  cpsapiVariant(const bool & value);

  cpsapiVariant(const std::string & value);

  cpsapiVariant(const CRegisteredCommonName & value);

  template < class Object > cpsapiVariant(const Object & value);

  template < class Object > cpsapiVariant(const std::vector< Object > & value);

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

  template < class Object > std::vector< Object > toVector() const;

private:
  std::unique_ptr< void, cpsapiFactory::free_unique_t > copyData() const;

  template < class Type > void assign(const Type * pValue, const Variant & variant);

  template < class Type > static void free_unique(void *);

  Variant mVariant;

  std::unique_ptr< void, cpsapiFactory::free_unique_t > mpData;
};

template < class Object > 
cpsapiVariant::cpsapiVariant(const Object & value)
  : mVariant(Variant::cpsapiObject)
  , mpData(cpsapiFactory::make_unique(value))
{}

template < class Object > 
cpsapiVariant::cpsapiVariant(const std::vector< Object > & value)
  : mVariant(Variant::cpsapiDouble)
  , mpData(cpsapiFactory::make_unique((value)))
{}

template < class Type > 
void cpsapiVariant::assign(const Type * pValue, const cpsapiVariant::Variant & variant)
{
  if (mVariant == variant)
    *static_cast< Type * >(mpData.get()) = *pValue;
  else
    {
      mVariant = variant;
      mpData = std::unique_ptr< Type, cpsapiFactory::free_unique_t >(new Type(*pValue), &cpsapiFactory::free_unique< Type >);
    }
}

CPSAPI_NAMESPACE_END
