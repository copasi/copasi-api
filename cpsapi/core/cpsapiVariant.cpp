#include "cpsapi/core/cpsapiObject.h"
#include "cpsapi/core/cpsapiVector.h"
#include "cpsapi/model/cpsapiModel.h"

CPSAPI_NAMESPACE_USE

cpsapiVariant::cpsapiVariant(const C_FLOAT64 & value)
  : mVariant(Variant::cpsapiDouble)
  , mpData(new C_FLOAT64(value), &cpsapiFactory::free_unique< C_FLOAT64 >)
{}

cpsapiVariant::cpsapiVariant(const C_INT32 & value)
  : mVariant(Variant::cpsapiInt32)
  , mpData(new C_INT32(value), &cpsapiFactory::free_unique< C_INT32 >)
{}

cpsapiVariant::cpsapiVariant(const unsigned C_INT32 & value)
  : mVariant(Variant::cpsapiUnsignedInt32)
  , mpData(new unsigned C_INT32(value), &cpsapiFactory::free_unique< unsigned C_INT32 >)
{}

cpsapiVariant::cpsapiVariant(const size_t & value)
  : mVariant(Variant::cpsapiSizeType)
  , mpData(new size_t(value), &cpsapiFactory::free_unique< size_t >)
{}

cpsapiVariant::cpsapiVariant(const bool & value)
  : mVariant(Variant::cpsapiBool)
  , mpData(new bool(value), &cpsapiFactory::free_unique< bool >)
{}

cpsapiVariant::cpsapiVariant(const std::string & value)
  : mVariant(Variant::cpsapiString)
  , mpData(new std::string(value), &cpsapiFactory::free_unique< std::string >)
{}

cpsapiVariant::cpsapiVariant(const CRegisteredCommonName & value)
  : mVariant(Variant::cpsapiCommonName)
  , mpData(new CRegisteredCommonName(value), &cpsapiFactory::free_unique< CRegisteredCommonName >)
{}

cpsapiVariant::cpsapiVariant(const cpsapiVariant & src)
  : mVariant(Variant::__SIZE)
  , mpData(src.copyData())
{
  switch (src.mVariant)
    {
      case Variant::cpsapiDouble:
        assign(static_cast< C_FLOAT64 * >(src.mpData.get()), src.mVariant);
        break;

      case Variant::cpsapiInt32:
        assign(static_cast< C_INT32 * >(src.mpData.get()), src.mVariant);
        break;
        
      case Variant::cpsapiUnsignedInt32:
        assign(static_cast< unsigned C_INT32 * >(src.mpData.get()), src.mVariant);
        break;
        
      case Variant::cpsapiSizeType:
        assign(static_cast< size_t * >(src.mpData.get()), src.mVariant);
        break;
        
      case Variant::cpsapiBool:
        assign(static_cast< bool * >(src.mpData.get()), src.mVariant);
        break;
        
      case Variant::cpsapiString:
        assign(static_cast< std::string * >(src.mpData.get()), src.mVariant);
        break;
        
      case Variant::cpsapiCommonName:
        assign(static_cast< CRegisteredCommonName * >(src.mpData.get()), src.mVariant);
        break;
        
      case Variant::cpsapiObject:
         mpData = cpsapiFactory::make_unique< cpsapiObject >(*static_cast< cpsapiObject * >(src.mpData.get()));
        break;
        
      case Variant::cpsapiVector:
        mpData = cpsapiFactory::make_unique< cpsapiVector< cpsapiContainer > >(*static_cast< cpsapiVector< cpsapiContainer > * >(src.mpData.get()));
        break;
        
      case Variant::__SIZE:
        break;
    }
}

cpsapiVariant::~cpsapiVariant()
{}

 