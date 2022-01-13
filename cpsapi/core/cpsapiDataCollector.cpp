// BEGIN: Copyright 
// Copyright (C) 2022 by Pedro Mendes, Rector and Visitors of the 
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

#include "cpsapi/core/cpsapiDataCollector.h"
#include "cpsapi/core/cpsapiValue.h"

CPSAPI_NAMESPACE_USE

// static
void cpsapiDataCollector::collect(cpsapiDataVector & data, const std::vector< std::pair< cpsapiData::Type, void * > > & values)
{
  data.push_back(cpsapiDataVector());
  
  cpsapiDataVector * pCollected = const_cast< cpsapiDataVector *>(&data.back().toData());

  for (const std::pair< cpsapiData::Type, void * > & value: values)
    pCollected->push_back(cpsapiData(value.first, value.second));
}

bool cpsapiDataCollector::generateValues(const CObjectInterface::ContainerList & listOfContainer,
                                         const std::vector< CRegisteredCommonName > & cns,
                                         std::vector< std::pair< cpsapiData::Type, void * > > & values)
{
  bool success = true;
  values.clear();

  for (const CRegisteredCommonName & CN : cns)
    {
      CObjectInterface * pObjectInterface = CObjectInterface::GetObjectFromCN(listOfContainer, CN);

      if (pObjectInterface == NULL)
        {
          CCopasiMessage(CCopasiMessage::WARNING, "cpsapiDataCollector: Object not found '%s'.", CN.c_str());
          success = false;
          continue;
        }

      // CMathObject is double
      cpsapiData::Type Type = cpsapiData::Type::Double;

      // This will be false for CMathObject
      if (pObjectInterface == pObjectInterface->getDataObject())
        {
          switch (cpsapiFactory::getDataType(pObjectInterface))
            {
            case CDataValue::Type::DOUBLE:
              Type = cpsapiData::Type::Double;
              break;

            case CDataValue::Type::INT:
              Type = cpsapiData::Type::Int32;
              break;

            case CDataValue::Type::BOOL:
              Type = cpsapiData::Type::Bool;
              break;

            case CDataValue::Type::STRING:
              Type = cpsapiData::Type::String;
              break;

            default:
              CCopasiMessage(CCopasiMessage::WARNING, "cpsapiDataCollector: Unsupported value type for '%s'.", CN.c_str());
              success = false;
              continue;
              break;
            }
        }

      mObjects.insert(pObjectInterface);
      values.push_back(std::make_pair(Type, pObjectInterface->getValuePointer()));
    }

  return success;
}

// virtual 
cpsapiDataCollector::~cpsapiDataCollector()
{}

// virtual
bool cpsapiDataCollector::compile(CObjectInterface::ContainerList listOfContainer)
{
  bool success = true;
  mObjects.clear();
  clear();

  success &= generateValues(listOfContainer, mCNsBefore, mValuesBefore);
  success &= generateValues(listOfContainer, mCNsDuring, mValuesDuring);
  success &= generateValues(listOfContainer, mCNsAfter, mValuesAfter);

  return success;
}

// virtual 
void cpsapiDataCollector::output(const Activity & activity)
{
  switch (activity)
  {
    case BEFORE:
      collect(mDataBefore, mValuesBefore);
      break;

    case DURING:
      collect(mDataDuring, mValuesDuring);
      break;

    case AFTER:
      collect(mDataAfter, mValuesAfter);
      break;

    case MONITORING:
      break;
  }
}

// virtual 
void cpsapiDataCollector::separate(const Activity & activity)
{
  switch (activity)
  {
    case BEFORE:
      mDataBefore.push_back(cpsapiData());
      break;

    case DURING:
      mDataDuring.push_back(cpsapiData());
      break;

    case AFTER:
      mDataAfter.push_back(cpsapiData());
      break;

    case MONITORING:
      break;
  }

}

// virtual 
void cpsapiDataCollector::finish()
{}

void cpsapiDataCollector::clear()
{
  mDataBefore.clear();
  mDataDuring.clear();
  mDataAfter.clear();
}

void cpsapiDataCollector::addObjectBefore(const cpsapiObject & object)
{
  if (object)
    mCNsBefore.push_back(object->getCN());
}

void cpsapiDataCollector::addObjectDuring(const cpsapiObject & object)
{
  if (object)
    mCNsDuring.push_back(object->getCN());
}

void cpsapiDataCollector::addObjectAfter(const cpsapiObject & object)
{
  if (object)
    mCNsAfter.push_back(object->getCN());
}

const std::vector< CRegisteredCommonName > & cpsapiDataCollector::getObjectsBefore() const
{
  return mCNsBefore;
}

const std::vector< CRegisteredCommonName > & cpsapiDataCollector::getObjectsDuring() const
{
  return mCNsDuring;
}

const std::vector< CRegisteredCommonName > & cpsapiDataCollector::getObjectsAfter() const
{
  return mCNsAfter;
}

const cpsapiDataVector & cpsapiDataCollector::getDataBefore() const
{
  return mDataBefore;
}

const cpsapiDataVector & cpsapiDataCollector::getDataDuring() const
{
  return mDataDuring;
}

const cpsapiDataVector & cpsapiDataCollector::getDataAfter() const
{
  return mDataAfter;
}
