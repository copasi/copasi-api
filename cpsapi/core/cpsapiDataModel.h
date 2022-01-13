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
#include "cpsapi/model/cpsapiModel.h"
#include "cpsapi/task/cpsapiTask.h"

#include <memory>

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

class cpsapiDataModel : public cpsapiContainer
{
public:
  /**
   * Enumeration of the exposed properties
   */ 
  enum class Property {};

  /**
   * Static set of supported properties
   */
  // static const Properties SupportedProperties;

  /**
   * The base class
   */
  typedef cpsapiContainer base;

  /**
   * The wrapped COPASI class
   */
  typedef CDataModel wrapped;

  class Data : public base::Data
  {
  public:
    Data(const base::Data & data)
      : base::Data(data)
      , mModel()
      , mDefaultTask()
      , mpDefaultReportDefinition(nullptr)
      , mpDefaultPlotSpecification(nullptr)
    {}

    virtual ~Data() {}

    cpsapiModel mModel;
    cpsapiTask  mDefaultTask;
    CReportDefinition * mpDefaultReportDefinition;
    CPlotSpecification * mpDefaultPlotSpecification;
  };

  /**
   * Specific constructor
   * @param wrapped * pDataModel (default: nullptr)
   */
  cpsapiDataModel(wrapped * pDataModel = nullptr);

  /**
   * Copy constructor
   * @param const cpsapiContainer & src
   */
  cpsapiDataModel(const cpsapiDataModel & src);

  /**
   * Destructor
   */
  virtual ~cpsapiDataModel();

  /**
   * Accept a visitor
   * @param cpsapiVisitor & visitor
   */
  virtual void accept(cpsapiVisitor & visitor) override;

  /**
   * Load a model from a file (any supported format)
   * @param const std::string & fileName
   * @return bool success
   */
  bool loadFromFile(const std::string & fileName);

  /**
   * Load a model from a file (any supported format)
   * @param const std::string & fileName
   * @return bool success
   */
  bool loadFromString(const std::string & content, const std::string & referenceDir = "");

  cpsapiModel & model();

  void beginTransaction();

  void endTransaction();

  bool addCompartment(const std::string & name);

  bool deleteCompartment(const std::string & name);

  cpsapiCompartment compartment(const std::string & name = "");

  cpsapiVector< cpsapiCompartment > getCompartments();

  bool addSpecies(const std::string & name, const std::string & compartmentName = "");

  bool deleteSpecies(const std::string & name, const std::string & compartmentName = "");

  cpsapiSpecies species(const std::string & name = "", const std::string & compartmentName = "");

  cpsapiVector< cpsapiSpecies > getSpecies();

  bool addGlobalQuantity(const std::string & name);

  bool deleteGlobalQuantity(const std::string & name);

  cpsapiGlobalQuantity globalQuantity(const std::string & name = "");

  cpsapiVector< cpsapiGlobalQuantity > getGlobalQuantities();

  cpsapiVector< cpsapiTask > getTasks();

  cpsapiTask task(const std::string & name = "");

  cpsapiMethod method();

  cpsapiProblem problem();

private:
  cpsapiTask __task(const std::string & name) const;
};

CPSAPI_NAMESPACE_END