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

#include "cpsapi/model/cpsapiEvent.h"
#include "cpsapi/model/cpsapiModel.h"
#include "cpsapi/model/cpsapiTransaction.h"

#pragma GCC diagnostic push
#include <copasi/model/CEvent.h>
#include <copasi/model/CModel.h>
#pragma GCC diagnostic pop

CPSAPI_NAMESPACE_USE

// static
const cpsapiEvent::Properties cpsapiEvent::SupportedProperties =
  {
    cpsapiProperty::DELAY_ASSIGNMENT,
    cpsapiProperty::FIRE_AT_INITIALTIME,
    cpsapiProperty::PERSISTENT_TRIGGER,
    cpsapiProperty::TRIGGER_EXPRESSION,
    cpsapiProperty::DELAY_EXPRESSION,
    cpsapiProperty::PRIORITY_EXPRESSION
  };

// static
const cpsapiEvent::Properties cpsapiEvent::HiddenProperties = {};

// static
const cpsapiEvent::References cpsapiEvent::SupportedReferences = {};

// static
const cpsapiEvent::References cpsapiEvent::HiddenReferences = {};

cpsapiEvent::cpsapiEvent(wrapped * pWrapped, const cpsapiObjectData::Type & type)
  : base(pWrapped, type)
{}

// virtual
cpsapiEvent::~cpsapiEvent()
{}

cpsapiEventAssignment cpsapiEvent::addAssignment(const std::string & name)
{
  if (!isValid())
    return nullptr;
  
  CDataVectorN< CEventAssignment > & Assignments = WRAPPED->getAssignments();
  
  if (!Assignments.add(CEventAssignment(name)))
    return nullptr;

  cpsapiEventAssignment Assignment = __assignment(name);

  if (!Assignment)
    return nullptr;

  if (*DATA->mDefaultAssignment != *Assignment)
    updateDefaultAssignment(Assignment);

  return DATA->mDefaultAssignment;
}

bool cpsapiEvent::deleteAssignment(const std::string & name)
{
  CEventAssignment * pAssignment = static_cast< CEventAssignment * >(*__assignment(name));

  if (pAssignment == nullptr)
    return false;
  
  if (*DATA->mDefaultAssignment == pAssignment)
    updateDefaultAssignment(nullptr);
  
  CModel * pModel = static_cast< CModel * >(WRAPPED->getObjectAncestor("Model"));
  cpsapiTransaction::beginStructureChange(pModel);

  cpsapiModel(pModel).deleteAllDependents(pAssignment);
  cpsapiObjectData::deleted(pAssignment);
  pdelete(pAssignment);

  return true;
}

cpsapiEventAssignment cpsapiEvent::assignment(const std::string & name)
{
  cpsapiEventAssignment Assignment(__assignment(name));

  if (!Assignment)
    return Assignment;

  if (*DATA->mDefaultAssignment != *Assignment)
    updateDefaultAssignment(Assignment);

  return DATA->mDefaultAssignment;
}

cpsapiVector< cpsapiEventAssignment > cpsapiEvent::getAssignments() const
{
  if (!isValid())
    return cpsapiVector< cpsapiEventAssignment >();

  return cpsapiVector< cpsapiEventAssignment >(&WRAPPED->getAssignments());
}

cpsapiEventAssignment cpsapiEvent::__assignment(const std::string & name) const
{ 
  if (!isValid())
    return nullptr;
    
  if (name.empty())
    return DATA->mDefaultAssignment;

  size_t Index = WRAPPED->getAssignments().getIndex(name);

  if (Index == C_INVALID_INDEX)
    return nullptr;

  return  &WRAPPED->getAssignments()[Index];
}

void cpsapiEvent::updateDefaultAssignment(const cpsapiEventAssignment & assignment)
{
  DATA->mDefaultAssignment = assignment;
}

bool cpsapiEvent::setProperty(const cpsapiEvent::Property & property, const cpsapiData & value, const CCore::Framework & framework)
{
  return setProperty(static_cast< const cpsapiProperty::Type >(property), value, framework);
}

cpsapiData cpsapiEvent::getProperty(const Property & property, const CCore::Framework & framework) const
{
  return getProperty(static_cast< const cpsapiProperty::Type >(property), framework);
}

// virtual
bool cpsapiEvent::setProperty(const cpsapiProperty::Type & property, const cpsapiData & value, const CCore::Framework & framework)
{
  if (!isValid()
      || isHiddenProperty< cpsapiEvent >(property))
    return false;

  if (!isImplementedProperty< cpsapiEvent >(property))
    return base::setProperty(property, value, framework);

  wrapped * pWrapped = WRAPPED;
  CDataObject * pChangedObject = pWrapped;
  bool success = false;

  switch (property)
    {
    case cpsapiProperty::DELAY_ASSIGNMENT:
      if (value.getType() == cpsapiData::Type::Bool)
        {
          pWrapped->setDelayAssignment(value.toBool());
          success = true;
        }
      break;

    case cpsapiProperty::FIRE_AT_INITIALTIME:
      if (value.getType() == cpsapiData::Type::Bool)
        {
          pWrapped->setFireAtInitialTime(value.toBool());
          success = true;
        }
      break;

    case cpsapiProperty::PERSISTENT_TRIGGER:
      if (value.getType() == cpsapiData::Type::Bool)
        {
          pWrapped->setPersistentTrigger(value.toBool());
          success = true;
        }
      break;

    case cpsapiProperty::TRIGGER_EXPRESSION:
      if (value.getType() == cpsapiData::Type::String)
        success = pWrapped->setTriggerExpression(value.toString());
      break;

    case cpsapiProperty::DELAY_EXPRESSION:
      if (value.getType() == cpsapiData::Type::String)
        success = pWrapped->setDelayExpression(value.toString());
      break;

    case cpsapiProperty::PRIORITY_EXPRESSION:
      if (value.getType() == cpsapiData::Type::String)
        success = pWrapped->setPriorityExpression(value.toString());
      break;

    default:
      break;
    }

  return success && cpsapiTransaction::synchronize(pChangedObject, framework);
}

// virtual
cpsapiData cpsapiEvent::getProperty(const cpsapiProperty::Type & property, const CCore::Framework & framework) const
{
  if (!isValid()
      || isHiddenProperty< cpsapiEvent >(property))
    return cpsapiData();

  if (!isImplementedProperty< cpsapiEvent >(property))
    return base::getProperty(property, framework);

  wrapped * pWrapped = WRAPPED;

  switch (property)
    {
    case cpsapiProperty::DELAY_ASSIGNMENT:
      return pWrapped->getDelayAssignment();
      break;

    case cpsapiProperty::FIRE_AT_INITIALTIME:
      return pWrapped->getFireAtInitialTime();
      break;

    case cpsapiProperty::PERSISTENT_TRIGGER:
      return pWrapped->getPersistentTrigger();
      break;

    case cpsapiProperty::TRIGGER_EXPRESSION:
      return pWrapped->getTriggerExpression();
      break;

    case cpsapiProperty::DELAY_EXPRESSION:
      return pWrapped->getDelayExpression();
      break;

    case cpsapiProperty::PRIORITY_EXPRESSION:
      return pWrapped->getPriorityExpression();
      break;

    default:
      break;
    }

  return cpsapiData();
}

CCommonName cpsapiEvent::getDataCN(const cpsapiEvent::Reference & reference, const CCore::Framework & framework) const
{
  return getDataCN(static_cast< cpsapiReference::Type >(reference), framework);
}

// virtual
CCommonName cpsapiEvent::getDataCN(const cpsapiReference::Type & reference, const CCore::Framework & framework) const
{
  if (!isValid()
      || isHiddenReference< cpsapiEvent >(reference))
    return Invalid;

  if (!isImplementedReference< cpsapiEvent >(reference))
    return base::getDataCN(reference, framework);

  switch (reference)
    {
    default:
      break;
    }

  return Invalid;
}
