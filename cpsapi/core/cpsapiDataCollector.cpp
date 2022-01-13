#include "cpsapi/core/cpsapiDataCollector.h"
#include "cpsapi/core/cpsapiValue.h"

CPSAPI_NAMESPACE_USE

// static
void cpsapiDataCollector::collect(cpsapiData & data, const std::vector< std::pair< cpsapiVariant::Type, void * > > & values)
{
  data.push_back(cpsapiData());
  cpsapiData * pCollected = const_cast< cpsapiData * >(&data[data.size() - 1].toData());

  for (const std::pair< cpsapiVariant::Type, void * > & value: values)
    pCollected->push_back(cpsapiVariant(value.first, value.second));
}

bool cpsapiDataCollector::generateValues(const CObjectInterface::ContainerList & listOfContainer,
                                         const std::vector< CRegisteredCommonName > & cns,
                                         std::vector< std::pair< cpsapiVariant::Type, void * > > & values)
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
      cpsapiVariant::Type Type = cpsapiVariant::Type::Double;

      // This will be false for CMathObject
      if (pObjectInterface == pObjectInterface->getDataObject())
        {
          switch (cpsapiFactory::getDataType(pObjectInterface))
            {
            case CDataValue::Type::DOUBLE:
              Type = cpsapiVariant::Type::Double;
              break;

            case CDataValue::Type::INT:
              Type = cpsapiVariant::Type::Int32;
              break;

            case CDataValue::Type::BOOL:
              Type = cpsapiVariant::Type::Bool;
              break;

            case CDataValue::Type::STRING:
              Type = cpsapiVariant::Type::String;
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
      mDataBefore.push_back(cpsapiVariant());
      break;

    case DURING:
      mDataDuring.push_back(cpsapiVariant());
      break;

    case AFTER:
      mDataAfter.push_back(cpsapiVariant());
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

const cpsapiData & cpsapiDataCollector::getDataBefore() const
{
  return mDataBefore;
}

const cpsapiData & cpsapiDataCollector::getDataDuring() const
{
  return mDataDuring;
}

const cpsapiData & cpsapiDataCollector::getDataAfter() const
{
  return mDataAfter;
}
