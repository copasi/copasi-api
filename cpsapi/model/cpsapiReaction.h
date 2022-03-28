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

#include "cpsapi/model/cpsapiKineticLawVariable.h"
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
    NAME = cpsapiProperty::Type::NAME,
    UNIQUE_NAME = cpsapiProperty::Type::UNIQUE_NAME,
    CN = cpsapiProperty::Type::CN
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
    UNIQUE_NAME = cpsapiReference::Type::UNIQUE_NAME,
    FLUX = cpsapiReference::Type::FLUX,
    PARTICLE_FLUX = cpsapiReference::Type::PARTICLE_FLUX,
    INITIAL_FLUX = cpsapiReference::Type::INITIAL_FLUX,
    INITIAL_PARTICLE_FLUX = cpsapiReference::Type::INITIAL_PARTICLE_FLUX,
    NOISE = cpsapiReference::Type::NOISE,
    PROPENSITY = cpsapiReference::Type::PROPENSITY
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
  typedef cpsapiReaction self;

  /**
   * The base class
   */
  typedef cpsapiContainer base;

  /**
   * The wrapped COPASI class
   */
  typedef CReaction wrapped;

  typedef std::map< std::string, cpsapiKineticLawVariable > VariableManager;
  typedef CDataVector< cpsapiKineticLawVariable::KineticLawVariable > VariableVector;

  class Data : public base::Data
  {
  public:
    Data(const base::Data & data)
      : base::Data(data)
      , mVariableManager()
      , mpVector(nullptr)
      , mDefaultVariable()
    {}

    virtual ~Data() 
    {
      if (mpVector != nullptr)
        {
          deleted(mpVector);
          delete mpVector;
          mpVector = nullptr;
        }
    }

    VariableManager mVariableManager;
    VariableVector  * mpVector;
    cpsapiKineticLawVariable mDefaultVariable;
  };

  /**
   * Specific constructor
   * @param wrapped * pWrapped
   */
  cpsapiReaction(wrapped * pWrapped = nullptr);

  /**
   * Destructor
   */
  virtual ~cpsapiReaction();

  /**
   * Accept the given visitor
   * 
   * @tparam Visitor 
   * @param Visitor & visitor 
   */
  template < typename Visitor > void accept(Visitor & visitor);

  /**
   * Retrieve the kinetic law variable with the givent name
   * @param const std::string & name 
   * @return cpsapiKineticLawVariable variable
   */
  cpsapiKineticLawVariable variable(const std::string & name = "");

  /**
   * Retrieve a vector of the kinetic law variables 
   * @return cpsapiVector< cpsapiKineticLawVariable > variables
   */
  cpsapiVector< cpsapiKineticLawVariable > variables();

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
   * Retreive the property
   * 
   * @param const cpsapiProperty::Type & property 
   * @param const CCore::Framework &framework 
   * @return cpsapiData property
   */
  virtual cpsapiData getProperty(const cpsapiProperty::Type & property, const CCore::Framework & framework) const override;

  /**
   * Retreive the data reference
   * 
   * @param const cpsapiReference::Type & reference 
   * @param const CCore::Framework &framework 
   * @return CCommonName
   */
  virtual CCommonName getDataCN(const cpsapiReference::Type & reference, const CCore::Framework & framework) const override;

private:
  /**
   * Assert that the variable manager contains a variable with the given 
   * @param const std::string & name 
   * @return cpsapiKineticLawVariable::KineticLawVariable* 
   */
  cpsapiKineticLawVariable::KineticLawVariable * assertVariable(const std::string & name);
};

template< class Visitor >
void cpsapiReaction::accept(Visitor & visitor)
{
  if (isValid())
    {
      cpsapiVisitor::acceptIfVisitable(visitor, this); 
      base::accept(visitor);
    }
}

CPSAPI_NAMESPACE_END