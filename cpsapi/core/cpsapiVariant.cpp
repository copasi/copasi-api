#include "cpsapi/core/cpsapiObject.h"
#include "cpsapi/core/cpsapiVector.h"
#include "cpsapi/model/cpsapiModel.h"

CPSAPI_NAMESPACE_USE

cpsapiVariant::cpsapiVariant()
  : mType(Type::__SIZE)
  , mpData(nullptr, &cpsapiFactory::free_unique< void >)
{}

cpsapiVariant::cpsapiVariant(const C_FLOAT64 & value)
  : mType(Type::Double)
  , mpData(new C_FLOAT64(value), &cpsapiFactory::free_unique< C_FLOAT64 >)
{}

cpsapiVariant::cpsapiVariant(const C_INT32 & value)
  : mType(Type::Int32)
  , mpData(new C_INT32(value), &cpsapiFactory::free_unique< C_INT32 >)
{}

cpsapiVariant::cpsapiVariant(const unsigned C_INT32 & value)
  : mType(Type::UnsignedInt32)
  , mpData(new unsigned C_INT32(value), &cpsapiFactory::free_unique< unsigned C_INT32 >)
{}

cpsapiVariant::cpsapiVariant(const size_t & value)
  : mType(Type::SizeType)
  , mpData(new size_t(value), &cpsapiFactory::free_unique< size_t >)
{}

cpsapiVariant::cpsapiVariant(const bool & value)
  : mType(Type::Bool)
  , mpData(new bool(value), &cpsapiFactory::free_unique< bool >)
{}

cpsapiVariant::cpsapiVariant(const std::string & value)
  : mType(Type::String)
  , mpData(new std::string(value), &cpsapiFactory::free_unique< std::string >)
{}

cpsapiVariant::cpsapiVariant(const char * value)
  : mType(Type::String)
  , mpData(new std::string(value), &cpsapiFactory::free_unique< std::string >)
{}

cpsapiVariant::cpsapiVariant(const CRegisteredCommonName & value)
  : mType(Type::CommonName)
  , mpData(new CRegisteredCommonName(value), &cpsapiFactory::free_unique< CRegisteredCommonName >)
{}

cpsapiVariant::cpsapiVariant(const cpsapiVariant & src)
  : mType(src.mType)
  , mpData(src.copyData())
{}

cpsapiVariant::~cpsapiVariant()
{}

C_FLOAT64 cpsapiVariant::toDouble() const
{
  switch (mType)
  {
    case Type::Double:
      return *static_cast< const C_FLOAT64 * >(mpData.get());
      break;

    case Type::Int32:
      return *static_cast< const C_INT32 * >(mpData.get());
      break;
      
    case Type::UnsignedInt32:
      return *static_cast< const unsigned C_INT32 * >(mpData.get());
      break;
      
    case Type::Bool:
      return *static_cast< const bool * >(mpData.get());
      break;
      
    case Type::SizeType:
      return *static_cast< const size_t * >(mpData.get());
      break;
      
    default:
      break;
  }

  return std::numeric_limits< C_FLOAT64 >::quiet_NaN();
}

C_INT32 cpsapiVariant::toInt32() const
{
  switch (mType)
  {
    case Type::Double:
      return *static_cast< const C_FLOAT64 * >(mpData.get());
      break;

    case Type::Int32:
      return *static_cast< const C_INT32 * >(mpData.get());
      break;
      
    case Type::UnsignedInt32:
      return *static_cast< const unsigned C_INT32 * >(mpData.get());
      break;
      
    case Type::Bool:
      return *static_cast< const bool * >(mpData.get());
      break;

    case Type::SizeType:
      return *static_cast< const size_t * >(mpData.get());
      break;
      
    default:
      break;
  }

  return 0;
}

unsigned C_INT32 cpsapiVariant::toUnsignedInt32() const
{
  switch (mType)
  {
    case Type::Double:
      return *static_cast< const C_FLOAT64 * >(mpData.get());
      break;

    case Type::Int32:
      return *static_cast< const C_INT32 * >(mpData.get());
      break;
      
    case Type::UnsignedInt32:
      return *static_cast< const unsigned C_INT32 * >(mpData.get());
      break;
      
    case Type::Bool:
      return *static_cast< const bool * >(mpData.get());
      break;
      
    case Type::SizeType:
      return *static_cast< const size_t * >(mpData.get());
      break;
      
    default:
      break;
  }

  return 0;
}

size_t cpsapiVariant::toSizeType() const
{
  switch (mType)
  {
    case Type::Double:
      return *static_cast< const C_FLOAT64 * >(mpData.get());
      break;

    case Type::Int32:
      return *static_cast< const C_INT32 * >(mpData.get());
      break;
      
    case Type::UnsignedInt32:
      return *static_cast< const unsigned C_INT32 * >(mpData.get());
      break;
      
    case Type::Bool:
      return *static_cast< const bool * >(mpData.get());
      break;
      
    case Type::SizeType:
      return *static_cast< const size_t * >(mpData.get());
      break;
      
    default:
      break;
  }

  return 0;
}

bool cpsapiVariant::toBool() const
{
  switch (mType)
  {
    case Type::Double:
      return *static_cast< const C_FLOAT64 * >(mpData.get());
      break;

    case Type::Int32:
      return *static_cast< const C_INT32 * >(mpData.get());
      break;
      
    case Type::UnsignedInt32:
      return *static_cast< const unsigned C_INT32 * >(mpData.get());
      break;
      
    case Type::Bool:
      return *static_cast< const bool * >(mpData.get());
      break;
      
    case Type::SizeType:
      return *static_cast< const size_t * >(mpData.get());
      break;
      
    default:
      break;
  }

  return 0;
}
std::string cpsapiVariant::toString() const
{
  switch (mType)
  {
    case Type::String:
      return *static_cast< const std::string * >(mpData.get());
      break;

    case Type::CommonName:
      return *static_cast< const CCommonName * >(mpData.get());
      break;
      
    default:
      break;
  }

  return std::string();
}

CRegisteredCommonName cpsapiVariant::toCommonName() const
{
  switch (mType)
  {
    case Type::String:
      return *static_cast< const std::string * >(mpData.get());
      break;

    case Type::CommonName:
      return *static_cast< const CCommonName * >(mpData.get());
      break;
      
    default:
      break;
  }

  return std::string();
}

const cpsapiVariant::Type & cpsapiVariant::getType() const
{
  return mType;
}

cpsapiVariant::DataPointer cpsapiVariant::copyData() const
{
  switch (mType)
    {
    case Type::Double:
      return DataPointer(new C_FLOAT64(*static_cast< C_FLOAT64 * >(mpData.get())), &cpsapiFactory::free_unique< C_FLOAT64 >);
      break;

    case Type::Int32:
      return DataPointer(new C_INT32(*static_cast< C_INT32 * >(mpData.get())), &cpsapiFactory::free_unique< C_INT32 >);
      break;

    case Type::UnsignedInt32:
      return DataPointer(new unsigned C_INT32(*static_cast< unsigned C_INT32 * >(mpData.get())), &cpsapiFactory::free_unique< unsigned C_INT32 >);
      break;

    case Type::SizeType:
      return DataPointer(new size_t(*static_cast< size_t * >(mpData.get())), &cpsapiFactory::free_unique< size_t >);
      break;

    case Type::Bool:
      return DataPointer(new bool(*static_cast< bool * >(mpData.get())), &cpsapiFactory::free_unique< bool >);
      break;

    case Type::String:
      return DataPointer(new std::string(*static_cast< std::string * >(mpData.get())), &cpsapiFactory::free_unique< std::string >);
      break;

    case Type::CommonName:
      return DataPointer(new CRegisteredCommonName(*static_cast< CRegisteredCommonName * >(mpData.get())), &cpsapiFactory::free_unique< CRegisteredCommonName >);
      break;

    case Type::Object:
      return cpsapiFactory::make_unique< cpsapiObject >(*static_cast< const cpsapiObject * >(mpData.get()));
      break;

    case Type::__SIZE:
      break;
    }

}
