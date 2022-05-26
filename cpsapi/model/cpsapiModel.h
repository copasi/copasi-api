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

#include "cpsapi/model/cpsapiModelEntity.h"
#include "cpsapi/model/cpsapiCompartment.h"
#include "cpsapi/model/cpsapiSpecies.h"
#include "cpsapi/model/cpsapiGlobalQuantity.h"
#include "cpsapi/model/cpsapiReaction.h"
#include "cpsapi/model/cpsapiEvent.h"

#pragma GCC diagnostic push
#include <copasi/model/CModel.h> 
#pragma GCC diagnostic pop

class CCompartment;
class CMetab;
class CReaction;
class CModelValue;
class CEvent;

CPSAPI_NAMESPACE_BEGIN


class cpsapiModel: public cpsapiModelEntity
{
public:
  /**
   * Enumeration of the exposed properties
   */ 
  enum class Property
  {
    INITIAL_VALUE = cpsapiProperty::INITIAL_VALUE,
    NAME = cpsapiProperty::NAME,
    OBJECT_UNIQUE_NAME = cpsapiProperty::OBJECT_UNIQUE_NAME,
    CN = cpsapiProperty::CN,
    UNIT = cpsapiProperty::UNIT,
    VOLUME_UNIT = cpsapiProperty::VOLUME_UNIT,
    AREA_UNIT = cpsapiProperty::AREA_UNIT,
    LENGTH_UNIT = cpsapiProperty::LENGTH_UNIT,
    TIME_UNIT = cpsapiProperty::TIME_UNIT,
    QUANTITY_UNIT = cpsapiProperty::QUANTITY_UNIT,
    MODEL_TYPE = cpsapiProperty::MODEL_TYPE,
    AVOGADRO_NUMBER = cpsapiProperty::AVOGADRO_NUMBER
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
   * The class
   */
  typedef cpsapiModel self;

  /**
   * The base class
   */
  typedef cpsapiModelEntity base;

  /**
   * The wrapped COPASI class
   */
  typedef CModel wrapped;

  class Data : public base::Data
  {
  public:
    Data(const base::Data & data)
      : base::Data(data)
      , mDefaultCompartment()
      , mDefaultReaction()
      , mDefaultGlobalQuantity()
      , mDefaultEvent()
    {}

    virtual ~Data() {}

    cpsapiCompartment mDefaultCompartment;
    cpsapiReaction mDefaultReaction;
    cpsapiGlobalQuantity mDefaultGlobalQuantity;
    cpsapiEvent mDefaultEvent;
  };

  /**
   * Specific constructor
   * @param wrapped * pWrapped
   */
  cpsapiModel(wrapped * pWrapped = nullptr);

  virtual ~cpsapiModel();

  /**
   * Accept the given visitor
   * 
   * @tparam Visitor 
   * @param Visitor & visitor 
   */
  template < typename Visitor > void accept(Visitor & visitor);

  void beginTransaction() const;

  void endTransaction() const;

  bool synchronize(std::set< const CDataObject * > & changedObjects, const CCore::Framework & framework);

  bool compile();

  cpsapiCompartment addCompartment(const std::string & name);

  bool deleteCompartment(const std::string & name = "");

  cpsapiCompartment compartment(const std::string & name = "");

  cpsapiVector< cpsapiCompartment > getCompartments() const;
  
  cpsapiSpecies addSpecies(const std::string & name, const std::string & compartment = "");

  bool deleteSpecies(const std::string & name = "", const std::string & compartment = "");

  cpsapiSpecies species(const std::string & name = "", const std::string & compartment = "");

  cpsapiVector< cpsapiSpecies > getSpecies() const;
  
  cpsapiGlobalQuantity addGlobalQuantity(const std::string & name);

  bool deleteGlobalQuantity(const std::string & name = "");

  cpsapiGlobalQuantity globalQuantity(const std::string & name = "");

  cpsapiVector< cpsapiGlobalQuantity > getGlobalQuantities() const;

  cpsapiReaction addReaction(const std::string & name);

  bool deleteReaction(const std::string & name = "");

  cpsapiReaction reaction(const std::string & name = "");

  cpsapiVector< cpsapiReaction > getReactions() const;
  
  cpsapiEvent addEvent(const std::string & name);

  bool deleteEvent(const std::string & name = "");

  cpsapiEvent event(const std::string & name = "");

  cpsapiVector< cpsapiEvent > getEvents() const;
  
  cpsapiEventAssignment addEventAssignment(const std::string & name, const std::string & event = "");

  bool deleteEventAssignment(const std::string & name = "", const std::string & event = "");

  cpsapiEventAssignment eventAssignment(const std::string & name = "", const std::string & event = "");

  cpsapiVector< cpsapiEventAssignment > getEventAssignments(const std::string & event = "") const;
  
  void deleteAllDependents(CDataContainer * pContainer);

  bool setProperty(const Property & property, const cpsapiData & value, const CCore::Framework & framework = CCore::Framework::__SIZE);

  cpsapiData getProperty(const Property & property, const CCore::Framework & framework = CCore::Framework::__SIZE) const;

protected:
  virtual bool setProperty(const cpsapiProperty::Type & property, const cpsapiData & value, const CCore::Framework & framework) override;

  virtual cpsapiData getProperty(const cpsapiProperty::Type & property, const CCore::Framework & framework) const override;

private:
  cpsapiCompartment __compartment(const std::string & name) const;

  void updateDefaultCompartment(const cpsapiCompartment & compartment);

  cpsapiSpecies __species(const std::string & name, const std::string & compartment) const;

  cpsapiGlobalQuantity __globalQuantity(const std::string & name) const;

  void updateDefaultGlobalQuantity(const cpsapiGlobalQuantity & globalQuantity);
  
  cpsapiReaction __reaction(const std::string & name) const;

  void updateDefaultReaction(const cpsapiReaction & reaction);
  
  /**
   * Retrieve the event with the given name 
   * Upper case name due __event being a Visual C keyword
   * @param const std::string & name 
   * @return cpsapiEvent 
   */
  cpsapiEvent __EVENT(const std::string & name) const;

  void updateDefaultEvent(const cpsapiEvent & event);
  
  void deleteDependents(const CDataObject::DataObjectSet & set);
};

template< class Visitor >
void cpsapiModel::accept(Visitor & visitor)
{
  if (isValid())
    {
      cpsapiVisitor::acceptIfVisitable(visitor, this); 
      base::accept(visitor);
    }
}

CPSAPI_NAMESPACE_END