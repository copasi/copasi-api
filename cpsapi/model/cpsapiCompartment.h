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
#include "cpsapi/model/cpsapiModelEntity.h"
#include "cpsapi/model/cpsapiSpecies.h"

#include <memory>

#pragma GCC diagnostic push
#include <copasi/model/CCompartment.h>
#pragma GCC diagnostic pop

class CMetab;

CPSAPI_NAMESPACE_BEGIN

class cpsapiCompartment: public cpsapiModelEntity
{
public:
  /**
   * Enumeration of the exposed properties
   */ 
  enum class Property
  {
    DIMENSIONALITY = cpsapiProperty::DIMENSIONALITY,
    EXPRESSION = cpsapiProperty::EXPRESSION,
    INITIAL_EXPRESSION = cpsapiProperty::INITIAL_EXPRESSION,
    INITIAL_VALUE = cpsapiProperty::INITIAL_VALUE,
    SIMULATION_TYPE = cpsapiProperty::SIMULATION_TYPE,
    ADD_NOISE = cpsapiProperty::ADD_NOISE,
    NOISE_EXPRESSION = cpsapiProperty::NOISE_EXPRESSION,
    NAME = cpsapiProperty::NAME,
    OBJECT_UNIQUE_NAME = cpsapiProperty::OBJECT_UNIQUE_NAME,
    CN = cpsapiProperty::CN,
    UNIT = cpsapiProperty::UNIT
  };

  /**
   * Static set of supported properties
   */
  static const Properties SupportedProperties;

  /**
   * The class
   */
  typedef cpsapiCompartment self;

  /**
   * The base class
   */
  typedef cpsapiModelEntity base;

  /**
   * The wrapped COPASI class
   */
  typedef CCompartment wrapped;

  class Data : public base::Data
  {
  public:
    Data(const base::Data & data)
      : base::Data(data)
      , mDefaultSpecies()
    {}

    virtual ~Data() {}


    cpsapiSpecies mDefaultSpecies;
  };

  /**
   * Specific constructor
   * @param wrapped * pWrapped
   */
  cpsapiCompartment(wrapped * pWrapped = nullptr);

  /**
   * @brief Destroy the cpsapi Compartment object
   */
  virtual ~cpsapiCompartment();

  /**
   * Accept the given visitor
   * 
   * @tparam Visitor 
   * @param Visitor & visitor 
   */
  template < typename Visitor > void accept(Visitor & visitor);

  cpsapiSpecies addSpecies(const std::string & name);

  bool deleteSpecies(const std::string & name = "");

  cpsapiSpecies species(const std::string & name = "");

  cpsapiVector< cpsapiSpecies > getSpecies() const;
  
  bool setProperty(const Property & property, const cpsapiData & value, const CCore::Framework & framework = CCore::Framework::__SIZE);

  cpsapiData getProperty(const Property & property, const CCore::Framework & framework = CCore::Framework::__SIZE) const;

protected:
  virtual bool setProperty(const cpsapiProperty::Type & property, const cpsapiData & value, const CCore::Framework & framework) override;

  virtual cpsapiData getProperty(const cpsapiProperty::Type & property, const CCore::Framework & framework) const override;

private:
  cpsapiSpecies __species(const std::string & name) const;

  void updateDefaultSpecies(const cpsapiSpecies & species);
};

template< class Visitor >
void cpsapiCompartment::accept(Visitor & visitor)
{
  if (isValid())
    {
      cpsapiVisitor::acceptIfVisitable(visitor, this); 
      base::accept(visitor);
    }
}

CPSAPI_NAMESPACE_END