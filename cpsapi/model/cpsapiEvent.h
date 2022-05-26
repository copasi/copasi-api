// BEGIN: Copyright 
// Copyright (C) 2021 - 2022 by Pedro Mendes, Rector and Visitors of the 
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

#include "cpsapi/core/cpsapiVector.h"
#include "cpsapi/model/cpsapiEventAssignment.h"

class CEvent;

CPSAPI_NAMESPACE_BEGIN

class cpsapiEvent : public cpsapiContainer
{
public:
  /**
   * Enumeration of the exposed properties
   */
  enum class Property
  {
    DELAY_ASSIGNMENT = cpsapiProperty::DELAY_ASSIGNMENT,
    FIRE_AT_INITIALTIME = cpsapiProperty::FIRE_AT_INITIALTIME,
    PERSISTENT_TRIGGER = cpsapiProperty::PERSISTENT_TRIGGER,
    TRIGGER_EXPRESSION = cpsapiProperty::TRIGGER_EXPRESSION,
    DELAY_EXPRESSION = cpsapiProperty::DELAY_EXPRESSION,
    PRIORITY_EXPRESSION = cpsapiProperty::PRIORITY_EXPRESSION,
    NAME = cpsapiProperty::NAME,
    OBJECT_UNIQUE_NAME = cpsapiProperty::OBJECT_UNIQUE_NAME,
    CN = cpsapiProperty::CN
  };

  /**
   * Static set of supported properties
   */
  static const Properties SupportedProperties;

  /**
   * Static set of hidden properties
   */
  static const Properties HiddenProperties;

  /**
   * Enumeration of the exposed references
   */
  enum class Reference
  {
    NAME = cpsapiReference::Type::NAME,
    OBJECT_UNIQUE_NAME = cpsapiReference::Type::OBJECT_UNIQUE_NAME
  };

  /**
   * Static set of supported references
   */
  static const References SupportedReferences;

  /**
   * Static set of hidden references
   */
  static const References HiddenReferences;

  /**
   * The class
   */
  typedef cpsapiEvent self;

  /**
   * The base class
   */
  typedef cpsapiContainer base;

  /**
   * The wrapped COPASI class
   */
  typedef CEvent wrapped;

  class Data : public base::Data
  {
  public:
    Data(const base::Data & data)
      : base::Data(data)
      , mDefaultEventAssignment()
    {}

    virtual ~Data() {}

    cpsapiEventAssignment mDefaultEventAssignment;
  };

  /**
   * Specific constructor
   * @param wrapped * pWrapped (default: nullptr)
   * @param const Type & type (default: Type::cpsapiEvent)
   */
  cpsapiEvent(wrapped * pWrapped = nullptr, const cpsapiObjectData::Type & type = cpsapiObjectData::Type::Parameter);

  /**
   * Destructor
   */
  virtual ~cpsapiEvent();

  /**
   * Accept the given visitor
   * 
   * @tparam Visitor 
   * @param Visitor & visitor 
   */
  template < typename Visitor >
  void accept(Visitor & visitor);

  /**
   * Add a an assignment to the event
   * 
   * @param const std::string & name 
   * @return cpsapiEventAssignment 
   */
  cpsapiEventAssignment addEventAssignment(const std::string & name);

  /**
   * Delete the Assignment with the given name 
   * 
   * @param const std::string & name = (default: current assignment) 
   * @return bool success 
   */
  bool deleteEventAssignment(const std::string & name = "");

  /**
   * Retrieve an assignment
   * 
   * @param const std::string & name = (default: current assignment) 
   * @return cpsapiEventAssignment 
   */
  cpsapiEventAssignment eventAssignment(const std::string & name = "");

  /**
   * Retrieve the vector af assignments
   * 
   * @return cpsapiVector< cpsapiEventAssignment > 
   */
  cpsapiVector< cpsapiEventAssignment > getEventAssignments() const;
  
  /**
   * Set a property of the object to the provided value under the given framework.
   * The value must match the underlying value of the property.
   * The default framework is unspecified
   * @param const Property & property
   * @param const cpsapiData & value
   * @param const CCore::Framework & framework (default: CCore::Framework::__SIZE)
   * @return bool success
   */
  bool setProperty(const Property & property, const cpsapiData & value, const CCore::Framework & framework = CCore::Framework::__SIZE);

  /**
   * Set a property of the object to the provided value under the given framework.
   * The value must match the underlying value of the property.
   * The default framework is unspecified
   * @param const Property & property
   * @param const CCore::Framework & framework (default: CCore::Framework::__SIZE)
   * @return cpsapiData property
   */
  cpsapiData getProperty(const Property & property, const CCore::Framework & framework = CCore::Framework::__SIZE) const;

  /**
   * Retrieve a property of the object to the provided value under the given framework.
   * The default framework is unspecified
   * @param const Reference & reference
   * @param const CCore::Framework & framework (default: CCore::Framework::__SIZE)
   * @return CCommonName
   */
  CCommonName getDataCN(const Reference & reference, const CCore::Framework & framework = CCore::Framework::__SIZE) const;

protected:
  /**
   * Set the property
   * 
   * @param const cpsapiProperty::Type & property 
   * @param const cpsapiData & value 
   * @param const CCore::Framework &framework 
   * @return bool success
   */
  virtual bool setProperty(const cpsapiProperty::Type & property, const cpsapiData & value, const CCore::Framework & framework) override;

  /**
   * Retrieve the property
   * 
   * @param const cpsapiProperty::Type & property 
   * @param const CCore::Framework &framework 
   * @return cpsapiData property
   */
  virtual cpsapiData getProperty(const cpsapiProperty::Type & property, const CCore::Framework & framework) const override;

  /**
   * Retrieve the data reference
   * 
   * @param const cpsapiReference::Type & reference 
   * @param const CCore::Framework &framework 
   * @return CCommonName
   */
  virtual CCommonName getDataCN(const cpsapiReference::Type & reference, const CCore::Framework & framework) const override;

private:
  /**
   * Resolve and retrieve the event assignment by name
   * @param const std::string & name
   * @return cpsapiEventAssignment 
   */
  cpsapiEventAssignment __eventAssignment(const std::string & name) const;

  /**
   * Update the default event assignment
   * @param assignment 
   */
  void updateDefaultEventAssignment(const cpsapiEventAssignment & assignment);
};

template < class Visitor >
void cpsapiEvent::accept(Visitor & visitor)
{
  if (isValid())
    {
      cpsapiVisitor::acceptIfVisitable(visitor, this);
      base::accept(visitor);
    }
}

CPSAPI_NAMESPACE_END