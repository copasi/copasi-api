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

#include <copasi/CopasiDataModel/CDataModel.h>
#include <copasi/core/CDataVector.h>

#include "cpsapi/core/cpsapiDataModel.h"
#include "cpsapi/model/cpsapiCompartment.h"

CPSAPI_NAMESPACE_USE

cpsapiDataModel::cpsapiDataModel(CDataModel * pDataModel)
  : base(pDataModel)
  , mDefaultModel(nullptr)
  , mpDefaultTask(nullptr)
  , mpDefaultReportDefinition(nullptr)
  , mpDefaultPlotSpecification(nullptr)
{}

cpsapiDataModel::cpsapiDataModel(const cpsapiDataModel & src)
  : base(src)
  , mDefaultModel(src.mDefaultModel)
  , mpDefaultTask(src.mpDefaultTask)
  , mpDefaultReportDefinition(src.mpDefaultReportDefinition)
  , mpDefaultPlotSpecification(src.mpDefaultPlotSpecification)
{}

// virtual 
cpsapiDataModel::~cpsapiDataModel()
{}

// virtual 
void cpsapiDataModel::accept(cpsapiVisitor & visitor)
{
  if (!mpObject)
    return;

  visitor.visit(this, cpsapiVisitor::TypeId::cpsapiDataModel);
  base::accept(visitor);
}

bool cpsapiDataModel::loadFromFile(const std::string & fileName)
{
  bool success = false;

  if (mpObject)
    {
      success = static_cast< CDataModel * >(*mpObject)->loadFromFile(fileName);
      mDefaultModel = cpsapiModel(static_cast< CDataModel * >(*mpObject)->getModel());
    }

  return success;
}

bool cpsapiDataModel::loadFromString(const std::string & content, const std::string & referenceDir)
{
  bool success = false;

  if (mpObject)
    {
      success = static_cast< CDataModel * >(*mpObject)->loadFromString(content, referenceDir);
      mDefaultModel = cpsapiModel(static_cast< CDataModel * >(*mpObject)->getModel());
    }

  return success;
}

cpsapiModel & cpsapiDataModel::model()
{
  if (!mDefaultModel && mpObject)
    mDefaultModel = static_cast< CDataModel * >(*mpObject)->getModel();

  return mDefaultModel;
}

void cpsapiDataModel::beginTransaction()
{
  model().beginTransaction();
}

void cpsapiDataModel::endTransaction()
{
  model().endTransaction();
}

bool cpsapiDataModel::addCompartment(const std::string & name)
{
  return model().addCompartment(name);
}

bool cpsapiDataModel::deleteCompartment(const std::string & name)
{
  return model().deleteCompartment(name);
}

cpsapiCompartment cpsapiDataModel::compartment(const std::string & name)
{
  return model().compartment(name);
}

std::vector< cpsapiCompartment > cpsapiDataModel::getCompartments()
{
  return model().getCompartments();
}

