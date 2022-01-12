#pragma once

#include <copasi/output/COutputHandler.h>

#include "cpsapi/core/cpsapiObject.h"

CPSAPI_NAMESPACE_BEGIN

class cpsapiDataCollector : public COutputInterface
{
public:
  /**
   * Destructor
   */
  virtual ~cpsapiDataCollector();

  /**
   * compile the object list from name vector
   * @param CObjectInterface::ContainerList listOfContainer
   * @return bool success
   */
  virtual bool compile(CObjectInterface::ContainerList listOfContainer) override;
  ;

  /**
   * Perform an output event for the current activity
   * @param const Activity & activity
   */
  virtual void output(const Activity & activity) override;

  /**
   * Introduce an additional separator into the output
   * @param const Activity & activity
   */
  virtual void separate(const Activity & activity) override;

  /**
   * Finish the output
   */
  virtual void finish() override;

  void clear();

  void addObjectBefore(const cpsapiObject & object);
  void addObjectDuring(const cpsapiObject & object);
  void addObjectAfter(const cpsapiObject & object);

  const std::vector< CRegisteredCommonName > & getObjectsBefore() const;
  const std::vector< CRegisteredCommonName > & getObjectsDuring() const;
  const std::vector< CRegisteredCommonName > & getObjectsAfter() const;

  const cpsapiData & getDataBefore() const;
  const cpsapiData & getDataDuring() const;
  const cpsapiData & getDataAfter() const;

private:
  static void collect(cpsapiData & data, const std::vector< std::pair< cpsapiVariant::Type, void * > > & values);

  bool generateValues(const CObjectInterface::ContainerList & listOfContainer,
                      const std::vector< CRegisteredCommonName > & cns,
                      std::vector< std::pair< cpsapiVariant::Type, void * > > & values);

  cpsapiData mDataBefore;
  cpsapiData mDataDuring;
  cpsapiData mDataAfter;

  std::vector< CRegisteredCommonName > mCNsBefore;
  std::vector< CRegisteredCommonName > mCNsDuring;
  std::vector< CRegisteredCommonName > mCNsAfter;

  std::vector< std::pair< cpsapiVariant::Type, void * > > mValuesBefore;
  std::vector< std::pair< cpsapiVariant::Type, void * > > mValuesDuring;
  std::vector< std::pair< cpsapiVariant::Type, void * > > mValuesAfter;
};

CPSAPI_NAMESPACE_END
