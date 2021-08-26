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
#include "cpsapi/model/cpsapiSpecies.h"
#include "cpsapi/model/cpsapiGlobalQuantity.h"

CPSAPI_NAMESPACE_USE

cpsapiDataModel::cpsapiDataModel(wrapped * pWrapped)
  : base(pWrapped, Type::cpsapiDataModel)
  , mpModel()
  , mpDefaultTask(nullptr)
  , mpDefaultReportDefinition(nullptr)
  , mpDefaultPlotSpecification(nullptr)
{
  for (cpsapiObject * pReference : references())
    if (this != pReference
        && pReference->getType() == Type::cpsapiDataModel)
      {
        mpModel = static_cast< cpsapiDataModel * >(pReference)->mpModel;
        break;
      }

  if (!mpModel)
    {
      if (operator bool())
        mpModel = cpsapiFactory::make_shared< cpsapiModel >(static_cast< wrapped * >(getObject())->getModel());
      else
        mpModel = cpsapiFactory::make_shared< cpsapiModel >(nullptr);
    }
}

cpsapiDataModel::cpsapiDataModel(const cpsapiDataModel & src)
  : base(src)
  , mpModel(src.mpModel)
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
  if (!operator bool())
    return;

  visitor.visit(this, Type::cpsapiDataModel);
  base::accept(visitor);
}

bool cpsapiDataModel::loadFromFile(const std::string & fileName)
{
  bool success = false;

  if (operator bool())
    {
      success = static_cast< wrapped * >(getObject())->loadFromFile(fileName);
      *mpModel = cpsapiModel(static_cast< wrapped * >(getObject())->getModel());
    }

  return success;
}

bool cpsapiDataModel::loadFromString(const std::string & content, const std::string & referenceDir)
{
  bool success = false;

  if (operator bool())
    {
      success = static_cast< wrapped * >(getObject())->loadFromString(content, referenceDir);
      *mpModel = cpsapiModel(static_cast< wrapped * >(getObject())->getModel());
    }

  return success;
}

cpsapiModel & cpsapiDataModel::model()
{
  if (operator bool()
      && !*mpModel)
    *mpModel = static_cast< wrapped * >(getObject())->getModel();

  return *mpModel;
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

bool cpsapiDataModel::addSpecies(const std::string & name, const std::string & compartmentName)
{
  return model().addSpecies(name, compartmentName);
}

bool cpsapiDataModel::deleteSpecies(const std::string & name, const std::string & compartmentName)
{
  return model().deleteSpecies(name, compartmentName);
}

cpsapiSpecies cpsapiDataModel::species(const std::string & name, const std::string & compartmentName)
{
  return model().species(name, compartmentName);
}

std::vector< cpsapiSpecies > cpsapiDataModel::getSpecies()
{
  return model().getSpecies();
}

bool cpsapiDataModel::addGlobalQuantity(const std::string & name)
{
  return model().addGlobalQuantity(name);
}

bool cpsapiDataModel::deleteGlobalQuantity(const std::string & name)
{
  return model().deleteGlobalQuantity(name);
}

cpsapiGlobalQuantity cpsapiDataModel::globalQuantity(const std::string & name)
{
  return model().globalQuantity(name);
}

std::vector< cpsapiGlobalQuantity > cpsapiDataModel::getGlobalQuantities()
{
  return model().getGlobalQuantities();
}

