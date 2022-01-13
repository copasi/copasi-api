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

  const cpsapiDataVector & getDataBefore() const;
  const cpsapiDataVector & getDataDuring() const;
  const cpsapiDataVector & getDataAfter() const;

private:
  static void collect(cpsapiDataVector & data, const std::vector< std::pair< cpsapiData::Type, void * > > & values);

  bool generateValues(const CObjectInterface::ContainerList & listOfContainer,
                      const std::vector< CRegisteredCommonName > & cns,
                      std::vector< std::pair< cpsapiData::Type, void * > > & values);

  cpsapiDataVector mDataBefore;
  cpsapiDataVector mDataDuring;
  cpsapiDataVector mDataAfter;

  std::vector< CRegisteredCommonName > mCNsBefore;
  std::vector< CRegisteredCommonName > mCNsDuring;
  std::vector< CRegisteredCommonName > mCNsAfter;

  std::vector< std::pair< cpsapiData::Type, void * > > mValuesBefore;
  std::vector< std::pair< cpsapiData::Type, void * > > mValuesDuring;
  std::vector< std::pair< cpsapiData::Type, void * > > mValuesAfter;
};

CPSAPI_NAMESPACE_END
