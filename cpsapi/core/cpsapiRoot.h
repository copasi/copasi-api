// BEGIN: Copyright 
// Copyright (C) 2021 by Pedro Mendes, Rector and Visitors of the 
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

class cpsapiDataModel;
class cpsapiModel;

class cpsapi: public cpsapiContainer
{
public:
  typedef cpsapiContainer base;

  cpsapi() = delete;

  cpsapi(const cpsapi & src) = delete;

  virtual ~cpsapi();

  static void init();

  static void release();

  static cpsapiDataModel & addDataModel(const std::string & name = "");

  static bool deleteDataModel(const std::string & name = "");

  static cpsapiDataModel & dataModel(const std::string & name = "");

  static std::vector< cpsapiDataModel > getDataModels();
  
  static const std::set< std::string > listModelNames();

  static bool loadFromFile(const std::string & fileName, const std::string & modelName = "");

  static bool loadFromString(const std::string & content, const std::string & referenceDir = "", const std::string & modelName = "");

  static cpsapiModel & model(const std::string & name = "");

  static void beginTransaction(const std::string & name = "");

  static void endTransaction(const std::string & name = "");

  static bool addCompartment(const std::string & name, const std::string & modelName = "");

  static bool deleteCompartment(const std::string & name, const std::string & modelName = "");

  static cpsapiCompartment compartment(const std::string & name = "", const std::string & modelName = "");

  static std::vector< cpsapiCompartment > getCompartments(const std::string & modelName = "");
  
private:
  static std::map< std::string, CDataModel * > DataModels;
  static cpsapiDataModel DefaultDataModel;
  static CFunction * pDefaultFunction;
  static CUnitDefinition * pDefaultUnitDefinition;
};

CPSAPI_NAMESPACE_END