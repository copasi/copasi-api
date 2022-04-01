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

#include "cpsapi/core/cpsapiContainer.h"
#include "cpsapi/task/cpsapiMethod.h"
#include "cpsapi/task/cpsapiProblem.h"

CPSAPI_NAMESPACE_BEGIN

class cpsapiTask: public cpsapiContainer
{
public:
  /**
   * Enumeration of the exposed properties
   */ 
  enum class Property
  {
    VALUE = cpsapiProperty::Type::VALUE,
    NAME = cpsapiProperty::Type::NAME,
    OBJECT_UNIQUE_NAME = cpsapiProperty::Type::OBJECT_UNIQUE_NAME,
    CN = cpsapiProperty::Type::CN,
    TYPE = cpsapiProperty::Type::TASK_TYPE
  };

  /**
   * Static set of supported properties
   */
  static const Properties SupportedProperties;

  /**
   * The class
   */
  typedef cpsapiTask self;

  /**
   * The base class
   */
  typedef cpsapiContainer base;

  /**
   * The wrapped COPASI class
   */
  typedef CCopasiTask wrapped;

 class Data : public base::Data
  {
  public:
    Data(const base::Data & data)
      : base::Data(data)
      , mMethod()
      , mProblem()
    {}

    virtual ~Data() {}

    cpsapiMethod mMethod;
    cpsapiProblem mProblem;
  };


  /**
   * Specific constructor
   * @param wrapped * pWrapped (default: nullptr)
  */
  cpsapiTask(wrapped * pWrapped = nullptr);

  /**
   * Destructor
   */
  virtual ~cpsapiTask();

  /**
   * Accept the given visitor
   * 
   * @tparam Visitor 
   * @param Visitor & visitor 
   */
  template < typename Visitor > void accept(Visitor & visitor);

  cpsapiMethod createMethod(const CTaskEnum::Method & type) const;

  bool setMethod(const cpsapiMethod & method);
  
  cpsapiMethod method() const;

  cpsapiProblem problem() const;
  
  bool setProperty(const Property & property, const cpsapiData & value, const CCore::Framework & framework = CCore::Framework::__SIZE);

  cpsapiData getProperty(const Property & property, const CCore::Framework & framework = CCore::Framework::__SIZE) const;

protected:
  virtual bool setProperty(const cpsapiProperty::Type & property, const cpsapiData & value, const CCore::Framework & framework) override;

  virtual cpsapiData getProperty(const cpsapiProperty::Type & property, const CCore::Framework & framework) const override;
};

template< class Visitor >
void cpsapiTask::accept(Visitor & visitor)
{
  if (isValid())
    {
      cpsapiVisitor::acceptIfVisitable(visitor, this); 
      base::accept(visitor);
    }
}

CPSAPI_NAMESPACE_END