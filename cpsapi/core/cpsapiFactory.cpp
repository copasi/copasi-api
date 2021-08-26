#include "cpsapi/core/cpsapiValue.h"
#include "cpsapi/core/cpsapiContainer.h"
#include "cpsapi/core/cpsapiVector.h"
#include "cpsapi/core/cpsapiDataModel.h"
#include "cpsapi/core/cpsapiParameter.h"
#include "cpsapi/core/cpsapiGroup.h"
#include "cpsapi/model/cpsapiModelEntity.h"
#include "cpsapi/model/cpsapiModel.h"

#include <copasi/CopasiDataModel/CDataModel.h>
#include <copasi/core/CDataObjectReference.h>

CPSAPI_NAMESPACE_USE

// static 
cpsapiFactory::CopasiMap cpsapiFactory::copasiMap;

// static 
cpsapiFactory::CpsapiMap cpsapiFactory::cpsapiMap;

// static 
void cpsapiFactory::insert(const TypeInfo & typeInfo)
{
  copasiMap.insert(std::make_pair(typeInfo.copasiClass, typeInfo));
  copasiMap.insert(std::make_pair(typeInfo.cpsapiClass, typeInfo));
  cpsapiMap.insert(std::make_pair(typeInfo.cpsapiType, typeInfo));
}

// static 
void cpsapiFactory::init()
{
  if (copasiMap.empty())
    {
      insert(TypeInfo(std::type_index(typeid(cpsapiObject)), cpsapiObject::Type::cpsapiObject, nullptr, std::type_index(typeid(CDataObject)), "CDataObject"));
      insert(TypeInfo(std::type_index(typeid(cpsapiContainer)), cpsapiObject::Type::cpsapiContainer, nullptr, std::type_index(typeid(CDataContainer)), "CDataContainer"));
      insert(TypeInfo(std::type_index(typeid(cpsapiModelEntity)), cpsapiObject::Type::cpsapiModelEntity, nullptr, std::type_index(typeid(CModelEntity)), "CModelEntity"));
      insert(TypeInfo(std::type_index(typeid(cpsapiVector< cpsapiCompartment >)), cpsapiObject::Type::cpsapiVector, new createTemplate< cpsapiVector< cpsapiCompartment >, CDataVectorNS < CCompartment > >(), std::type_index(typeid(CDataVectorNS < CCompartment >)), "CDataVectorNS < CCompartment >"));
      insert(TypeInfo(std::type_index(typeid(cpsapiVector< cpsapiSpecies >)), cpsapiObject::Type::cpsapiVector, new createTemplate< cpsapiVector< cpsapiSpecies >, CDataVectorNS < CMetab > >(), std::type_index(typeid(CDataVectorNS < CMetab >)), "CDataVectorNS < CMetab >"));
      insert(TypeInfo(std::type_index(typeid(cpsapiVector< cpsapiGlobalQuantity >)), cpsapiObject::Type::cpsapiVector, new createTemplate< cpsapiVector< cpsapiGlobalQuantity >, CDataVectorN < CModelValue > >(), std::type_index(typeid(CDataVectorN < CModelValue >)), "CDataVectorNS < CModelValue >"));
      insert(TypeInfo(std::type_index(typeid(cpsapiValue)), cpsapiObject::Type::cpsapiValue, new createTemplate< cpsapiValue, CDataObject >(), std::type_index(typeid(CDataObjectReference< C_FLOAT64 >)), "CDataObjectReference< C_FLOAT64 >"));
      insert(TypeInfo(std::type_index(typeid(cpsapiValue)), cpsapiObject::Type::cpsapiValue, new createTemplate< cpsapiValue, CDataObject >(), std::type_index(typeid(CDataObjectReference< C_INT32 >)), "CDataObjectReference< C_INT32 >"));
      insert(TypeInfo(std::type_index(typeid(cpsapiValue)), cpsapiObject::Type::cpsapiValue, new createTemplate< cpsapiValue, CDataObject >(), std::type_index(typeid(CDataObjectReference< unsigned C_INT32 >)), "CDataObjectReference< unsigned C_INT32 >"));
      insert(TypeInfo(std::type_index(typeid(cpsapiValue)), cpsapiObject::Type::cpsapiValue, new createTemplate< cpsapiValue, CDataObject >(), std::type_index(typeid(CDataObjectReference< size_t >)), "CDataObjectReference< size_t >"));
      insert(TypeInfo(std::type_index(typeid(cpsapiValue)), cpsapiObject::Type::cpsapiValue, new createTemplate< cpsapiValue, CDataObject >(), std::type_index(typeid(CDataObjectReference< std::string >)), "CDataObjectReference< std::string >"));
      insert(TypeInfo(std::type_index(typeid(cpsapiValue)), cpsapiObject::Type::cpsapiValue, new createTemplate< cpsapiValue, CDataObject >(), std::type_index(typeid(CDataObjectReference< CRegisteredCommonName >)), "CDataObjectReference< CRegisteredCommonName >"));
      insert(TypeInfo(std::type_index(typeid(cpsapiModel)), cpsapiObject::Type::cpsapiModel, new createTemplate< cpsapiModel, CModel >(), std::type_index(typeid(CModel)), "CModel"));
      insert(TypeInfo(std::type_index(typeid(cpsapiCompartment)), cpsapiObject::Type::cpsapiCompartment, new createTemplate< cpsapiCompartment, CCompartment >(), std::type_index(typeid(CCompartment)), "CCompartment"));
      insert(TypeInfo(std::type_index(typeid(cpsapiSpecies)), cpsapiObject::Type::cpsapiSpecies, new createTemplate< cpsapiSpecies, CMetab >(), std::type_index(typeid(CMetab)), "CMetab"));
      insert(TypeInfo(std::type_index(typeid(cpsapiGlobalQuantity)), cpsapiObject::Type::cpsapiGlobalQuantity, new createTemplate< cpsapiGlobalQuantity, CModelValue >(), std::type_index(typeid(CModelValue)), "CModelValue"));
      insert(TypeInfo(std::type_index(typeid(cpsapiDataModel)), cpsapiObject::Type::cpsapiDataModel, new createTemplate< cpsapiDataModel, CDataModel >(), std::type_index(typeid(CDataModel)), "CDataModel"));
      insert(TypeInfo(std::type_index(typeid(cpsapiParameter)), cpsapiObject::Type::cpsapiParameter, new createTemplate< cpsapiParameter, CCopasiParameter >(), std::type_index(typeid(CCopasiParameter)), "CCopasiParameter"));
      insert(TypeInfo(std::type_index(typeid(cpsapiGroup)), cpsapiObject::Type::cpsapiGroup, new createTemplate< cpsapiGroup, CCopasiParameterGroup >(), std::type_index(typeid(CCopasiParameterGroup)), "CCopasiParameterGroup"));
    }
}


cpsapiFactory::TypeInfo::TypeInfo(const std::type_index & cpsapiClass, cpsapiObject::Type cpsapiType, create * cpsapiCreate, const std::type_index & copasiClass, const std::string copasiString)
  : cpsapiClass(cpsapiClass)
  , cpsapiType(cpsapiType)
  , copasiClass(copasiClass)
  , copasiString(copasiString)
{
  this->cpsapiClass = cpsapiClass;
  this->cpsapiType = cpsapiType;
  this->cpsapiCreate = cpsapiCreate;
  this->copasiClass = copasiClass;
  this->copasiString = copasiString;
}

cpsapiFactory::TypeInfo & cpsapiFactory::TypeInfo::operator = (const cpsapiFactory::TypeInfo & rhs)
{
  if (this != &rhs)
    {
      cpsapiClass = rhs.cpsapiClass;
      cpsapiType = rhs.cpsapiType;
      copasiClass = rhs.copasiClass;
      copasiString = rhs.copasiString;
    }

  return *this;
}

// static 
const cpsapiFactory::TypeInfo & cpsapiFactory::info(const std::type_index & typeIndex)
{
  static const TypeInfo Unknown;

  CopasiMap::const_iterator found = copasiMap.find(typeIndex);

  if (found != copasiMap.end())
    return found->second;

  return Unknown;
}

// static 
const cpsapiFactory::TypeInfo & cpsapiFactory::info(CDataObject * pFrom)
{
  static const TypeInfo Unknown;

  if (pFrom != nullptr)
    return info(std::type_index(typeid(*pFrom)));

  return Unknown;
}

// static 
const cpsapiFactory::TypeInfo & cpsapiFactory::info(const cpsapiObject & from)
{
  static const TypeInfo Unknown;

  if (from)
    return info(std::type_index(typeid(*from.getObject())));

  return Unknown;
}

// static 
cpsapiFactory::Range cpsapiFactory::info(const cpsapiObject::Type & type)
{
  return cpsapiMap.equal_range(type);
}

// static
cpsapiObject * cpsapiFactory::make(CDataObject * pDataObject, const TypeInfo * pTypeInfo)
{
  cpsapiObject * pObject = nullptr;
  
  const TypeInfo * pInfo = pTypeInfo;

  if (pTypeInfo == nullptr)
    pInfo = &info(pDataObject);
  
  if (pInfo->cpsapiCreate != nullptr)
    pObject = (*pInfo->cpsapiCreate)(pDataObject);

  return pObject;
}
