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
#include "cpsapi/core/cpsapiValue.h"
#include "cpsapi/model/cpsapiModel.h"
#include "cpsapi/model/cpsapiCompartment.h"
#include "cpsapi/model/cpsapiSpecies.h"

using namespace std;
CPSAPI_NAMESPACE_USE

TEST_CASE("Edit model", "[cpsapi]")
{
  cpsapiModel Model = cpsapiRoot::addModel("test_model");
  REQUIRE(Model);

  cpsapiCompartment Compartment = Model.addCompartment("test_compartment");
  REQUIRE(Compartment);
  
  cpsapiCompartment Copy(Compartment);
  REQUIRE(Copy);

  cpsapiCompartment Assignement;
  REQUIRE(!Assignement);

  Assignement = Compartment;
  REQUIRE(Assignement);

  cpsapiValue Value(static_cast< CCompartment * >(Compartment.getObject())->getInitialValueReference());
  REQUIRE(Value);
  REQUIRE(Value.setValue(5.0));
  REQUIRE(!Value.setValue("wrong type"));

  REQUIRE(Model.compartment());
  REQUIRE(!Model.compartment("other"));
  REQUIRE(Model.getCompartments().size() == 1);

  cpsapiSpecies Species = Compartment.addSpecies("test_species");
  REQUIRE(!Model.addSpecies("test_species"));
  REQUIRE(!Copy.addSpecies("test_species"));
  REQUIRE(Copy.getSpecies().size() == 1);

  REQUIRE(Model.deleteCompartment());
  REQUIRE(Model.getCompartments().size() == 0);
  REQUIRE(!Species);
  REQUIRE(!Compartment);
  REQUIRE(!Copy);
  REQUIRE(!Assignement);

  REQUIRE(cpsapiRoot::deleteModel("test_model"));
}
