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

#include "cpsapi/core/cpsapiContainer.h"

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

CPSAPI_NAMESPACE_BEGIN

class cpsapiModel;
class cpsapiCompartment;

class cpsapiDataModel : public cpsapiContainer
{
public:
  typedef cpsapiContainer base;

  cpsapiDataModel(CDataModel * pDataModel = nullptr);

  cpsapiDataModel(const cpsapiDataModel & src);

  virtual ~cpsapiDataModel();

  virtual void accept(cpsapiVisitor & visitor) override;

  bool load(const std::string & src);

  cpsapiModel model();

  void beginTransaction();

  void endTransaction();

  bool addCompartment(const std::string & name);

  bool deleteCompartment(const std::string & name);

  cpsapiCompartment compartment(const std::string & name = "");

  std::vector< cpsapiCompartment > getCompartments();

private:
  CCopasiTask * mpDefaultTask;
  CReportDefinition * mpDefaultReportDefinition;
  CPlotSpecification * mpDefaultPlotSpecification;

};

CPSAPI_NAMESPACE_END