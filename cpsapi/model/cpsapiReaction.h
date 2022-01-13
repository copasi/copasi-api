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

#include "cpsapi/model/cpsapiReactionParameter.h"
#include "cpsapi/core/cpsapiContainer.h"
#include "cpsapi/core/cpsapiVector.h"

class CReaction;

CPSAPI_NAMESPACE_BEGIN

class cpsapiReaction : public cpsapiContainer
{
public:
  /**
   * Enumeration of the exposed properties
   */ 
  enum class Property
  {
    CHEMICAL_EQUATION = cpsapiProperty::Type::CHEMICAL_EQUATION,
    KINETIC_LAW = cpsapiProperty::Type::KINETIC_LAW,
    KINETIC_LAW_EXPRESSION = cpsapiProperty::Type::KINETIC_LAW_EXPRESSION,
    KINETIC_LAW_UNIT_TYPE = cpsapiProperty::Type::KINETIC_LAW_UNIT_TYPE,
    SCALING_COMPARTMENT = cpsapiProperty::Type::SCALING_COMPARTMENT,
    ADD_NOISE = cpsapiProperty::Type::ADD_NOISE,
    NOISE_EXPRESSION = cpsapiProperty::Type::NOISE_EXPRESSION,
    OBJECT_NAME = cpsapiProperty::Type::OBJECT_NAME,
    DISPLAY_NAME = cpsapiProperty::Type::DISPLAY_NAME,
    CN = cpsapiProperty::Type::CN
  };

  /**
   * Static set of supported properties
   */
  static const Properties SupportedProperties;

  /**
   * The base class
   */
  typedef cpsapiContainer base;

  /**
   * The wrapped COPASI class
   */
  typedef CReaction wrapped;

  typedef std::map< std::string, cpsapiReactionParameter::FakeData * > ParameterManager;
  typedef CDataVector< cpsapiReactionParameter::FakeData > ParameterVector;
  class Data : public base::Data
  {
  public:
    Data(const base::Data & data)
      : base::Data(data)
      , mManager()
      , mpVector(nullptr)
      , mDefaultParameter()
    {}

    virtual ~Data() 
    {
      ParameterManager::iterator it = mManager.begin();
      ParameterManager::iterator end = mManager.end();
      
      for (; it != end; ++it)
        {
          deleted(it->second);
          delete it->second;
        }

      mManager.clear();

      if (mpVector != nullptr)
        {
          deleted(mpVector);
          delete mpVector;
          mpVector = nullptr;
        }
    }

    ParameterManager mManager;
    ParameterVector  * mpVector;
    cpsapiReactionParameter mDefaultParameter;
  };

  /**
   * Specific constructor
   * @param wrapped * pWrapped
   */
  cpsapiReaction(wrapped * pWrapped = nullptr);

  /**
   * Copy constructor
   * @param const cpsapiReaction & src
   */
  cpsapiReaction(const cpsapiReaction & src);

  virtual ~cpsapiReaction();

  virtual void accept(cpsapiVisitor & visitor) override;

  cpsapiReactionParameter parameter(const std::string & name = "");

  cpsapiVector< cpsapiReactionParameter > parameters();

  bool setProperty(const Property & property, const cpsapiData & value, const CCore::Framework & framework = CCore::Framework::__SIZE);

  cpsapiData getProperty(const Property & property, const CCore::Framework & framework = CCore::Framework::__SIZE) const;

protected:
  virtual bool setProperty(const cpsapiProperty::Type & property, const cpsapiData & value, const CCore::Framework & framework) override;

  virtual cpsapiData getProperty(const cpsapiProperty::Type & property, const CCore::Framework & framework) const override;

private:
  cpsapiReactionParameter::FakeData * assertParameter(const std::string & name);
};

CPSAPI_NAMESPACE_END