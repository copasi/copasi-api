// BEGIN: Copyright 
// Copyright (C) 2021 by Pedro Mendes, Rector and Visitors of the 
// University of Virginia, University of Heidelberg, and University 
// of Connecticut School of Medicine. 
// All rights reserved 
// END: Copyright 

// BEGIN: License 
// Licensed under the Apache License, Version 2.0 (the "License"); 
// you may not use this file except in compliance with the License. 
// You may obtain a copy of the License at 
//   http://www.apache.org/licenses/LICENSE-2.0 
// END: License 

#pragma once

#include <map>

#include "cpsapi/core/cpsapiContainer.h"
#include "cpsapi/model/cpsapiModel.h"
#include "cpsapi/model/cpsapiCompartment.h"
#include "cpsapi/model/cpsapiSpecies.h"

class CRootContainer;
class CDataModel;
class CModel;
class CCompartment;
class CMetab;
class CReaction;
class CModelValue;
class CEvent;
class CCopasiTask;
class CReportDefinition;
class CPlotSpecification;
class CFunction;
class CUnitDefinition;

CPSAPI_NAMESPACE_BEGIN

class cpsapiRoot: public cpsapiContainer
{
private:
  typedef cpsapiContainer base;

public:
  cpsapiRoot() = delete;

  cpsapiRoot(const cpsapiRoot & src) = delete;

  virtual ~cpsapiRoot();

  static void init();

  static void release();

  static cpsapiModel addModel(const std::string & name = "");

  static bool deleteModel(const std::string & name);

  static CDataModel * dataModel(const std::string & name = "");

  static const std::set< std::string > listModels();

  static bool loadModel(const std::string & src);

  static cpsapiModel model(const std::string & name = "");

  static void beginTransaction(const std::string & name = "");

  static void endTransaction(const std::string & name = "");

  static bool addCompartment(const std::string & name, const std::string & modelName = "");

  static bool deleteCompartment(const std::string & name, const std::string & modelName = "");

  static cpsapiCompartment compartment(const std::string & name = "", const std::string & modelName = "");

  static std::vector< cpsapiCompartment > getCompartments(const std::string & modelName = "");
  
private:
  static void changeDataModel(CDataModel * pDataModel);
  static void assertDefaultDataModel();

  static std::map< std::string, CDataModel * > DataModels;
  static CDataModel * pDefaultDataModel;
  static CModel * pDefaultModel;
  static CCopasiTask * pDefaultTask;
  static CReportDefinition * pDefaultReportDefinition;
  static CPlotSpecification * pDefaultPlotSpecification;
  static CFunction * pDefaultFunction;
  static CUnitDefinition * pDefaultUnitDefinition;
};

CPSAPI_NAMESPACE_END