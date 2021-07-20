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

#include "catch.hpp"
#include <limits>

#include "cpsapi/core/cpsapiRoot.h"
#include "cpsapi/core/cpsapiDataModel.h"
#include "cpsapi/core/cpsapiValue.h"
#include "cpsapi/model/cpsapiModel.h"
#include "cpsapi/model/cpsapiCompartment.h"
#include "cpsapi/model/cpsapiSpecies.h"
#include "cpsapi/model/cpsapiGlobalQuantity.h"

using namespace std;
CPSAPI_NAMESPACE_USE

TEST_CASE("Edit model", "[cpsapi]")
{
  REQUIRE(cpsapiObject::AllSupportedProperties< cpsapiObject >().size() == 1);
  REQUIRE(cpsapiObject::AllSupportedProperties< cpsapiContainer >().size() == 1);
  REQUIRE(cpsapiObject::AllSupportedProperties< cpsapiDataModel >().size() == 1);
  REQUIRE(cpsapiObject::AllSupportedProperties< cpsapiModelEntity >().size() == 7);
  REQUIRE(cpsapiObject::AllSupportedProperties< cpsapiCompartment >().size() == 9);
  REQUIRE(cpsapiObject::AllSupportedProperties< cpsapiSpecies >().size() == 8);
  REQUIRE(cpsapiObject::AllSupportedProperties< cpsapiSpecies >().size() == 8);
  REQUIRE(cpsapiObject::AllSupportedProperties< cpsapiModel >().size() == 10);

  cpsapiModel Model = cpsapi::addDataModel("test_model").model();
  REQUIRE(Model);
  
  cpsapiCompartment Compartment = Model.addCompartment("test_compartment");
  REQUIRE(Compartment);
  
  cpsapiCompartment Copy(Compartment);
  REQUIRE(Copy);

  cpsapiCompartment Assignment;
  REQUIRE_FALSE(Assignment);

  Assignment = Compartment;
  REQUIRE(Assignment);
  REQUIRE(Assignment == Compartment);

  cpsapiValue Value(static_cast< CCompartment * >(Compartment.getObject())->getInitialValueReference());
  REQUIRE(Value.valid());
  REQUIRE(Value.setValue(5.0));
  REQUIRE(Compartment.get(cpsapiCompartment::Property::INITIAL_VALUE).toDouble() == 5.0);
  REQUIRE_FALSE(Value.setValue("wrong type"));
  
  REQUIRE(Model.compartment());
  REQUIRE_FALSE(Model.compartment("other"));
  REQUIRE(Model.getCompartments().size() == 1);

  cpsapiSpecies Species = Compartment.addSpecies("test_species");

  double ParticleNumber = Species.get(cpsapiSpecies::Property::INITIAL_VALUE, CCore::Framework::ParticleNumbers).toDouble();

  cpsapi::beginTransaction();
  
  Compartment.set(cpsapiCompartment::Property::INITIAL_VALUE, 1e-10, CCore::Framework::Concentration);
  REQUIRE(ParticleNumber == Species.get(cpsapiSpecies::Property::INITIAL_VALUE, CCore::Framework::ParticleNumbers).toDouble());
  
  cpsapi::endTransaction();

  REQUIRE_FALSE(ParticleNumber == Species.get(cpsapiSpecies::Property::INITIAL_VALUE, CCore::Framework::ParticleNumbers).toDouble());

  REQUIRE_FALSE(Model.addSpecies("test_species"));
  REQUIRE_FALSE(Copy.addSpecies("test_species"));
  REQUIRE(Copy.getSpecies().size() == 1);

  std::ostringstream Equation;
  Equation << 3 << "*" << Model.species().displayName() << "->";
  REQUIRE("3*test_species->" == Equation.str());

  REQUIRE_FALSE(Model.globalQuantity());
  REQUIRE(Model.addGlobalQuantity("test_global_quantity"));
  REQUIRE_FALSE(Model.addGlobalQuantity("test_global_quantity"));
  REQUIRE(Model.globalQuantity());
  REQUIRE(Model.getGlobalQuantities().size() == 1);
  REQUIRE(Model.deleteGlobalQuantity());
  REQUIRE(Model.getGlobalQuantities().size() == 0);
  
  REQUIRE(Model.deleteCompartment());
  REQUIRE(Model.getCompartments().size() == 0);
  REQUIRE_FALSE(Species);
  REQUIRE_FALSE(Compartment);
  REQUIRE_FALSE(Copy);
  REQUIRE_FALSE(Assignment);

  REQUIRE(cpsapi::deleteDataModel("test_model"));
}
