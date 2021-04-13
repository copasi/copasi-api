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

using namespace std;
CPSAPI_NAMESPACE_USE

TEST_CASE("Edit model", "[cpsapi]")
{
  cpsapiModel Model = cpsapi::addDataModel("test_model").model();
  REQUIRE(Model);

  cpsapiCompartment Compartment = Model.addCompartment("test_compartment");
  REQUIRE(Compartment);
  
  cpsapiCompartment Copy(Compartment);
  REQUIRE(Copy);

  cpsapiCompartment Assignment;
  REQUIRE(!Assignment);

  Assignment = Compartment;
  REQUIRE(Assignment);

  cpsapiValue Value(static_cast< CCompartment * >(Compartment.getObject())->getInitialValueReference());
  // REQUIRE(Value); <-- does not work
  REQUIRE(Value.setValue(5.0));
  REQUIRE(Compartment.get(cpsapiCompartment::Property::INITIAL_VALUE).toDouble() == 5.0);
  REQUIRE(!Value.setValue("wrong type"));
  
  REQUIRE(Model.compartment());
  REQUIRE(!Model.compartment("other"));
  REQUIRE(Model.getCompartments().size() == 1);

  cpsapiSpecies Species = Compartment.addSpecies("test_species");

  double ParticleNumber = Species.get(cpsapiSpecies::Property::INITIAL_VALUE, CCore::Framework::ParticleNumbers).toDouble();

  cpsapi::beginTransaction();
  Compartment.set(cpsapiCompartment::Property::INITIAL_VALUE, 1e-10, CCore::Framework::Concentration);
  REQUIRE(ParticleNumber == Species.get(cpsapiSpecies::Property::INITIAL_VALUE, CCore::Framework::ParticleNumbers).toDouble());
  cpsapi::endTransaction();
  REQUIRE(ParticleNumber != Species.get(cpsapiSpecies::Property::INITIAL_VALUE, CCore::Framework::ParticleNumbers).toDouble());

  REQUIRE(!Model.addSpecies("test_species"));
  REQUIRE(!Copy.addSpecies("test_species"));
  REQUIRE(Copy.getSpecies().size() == 1);

  REQUIRE(Model.deleteCompartment());
  REQUIRE(Model.getCompartments().size() == 0);
  REQUIRE(!Species);
  REQUIRE(!Compartment);
  REQUIRE(!Copy);
  REQUIRE(!Assignment);

  REQUIRE(cpsapi::deleteDataModel("test_model"));
}
