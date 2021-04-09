// BEGIN: Copyright 
// Copyright (C) 2021 by Pedro Mendes, Rector and Visitors of the 
// University of Virginia, University of Heidelberg, and University 
// of Connecticut School of Medicine. 
// All rights reserved 
// END: Copyright 

// BEGIN: License 
// Licensed under the Apache License, Version 2.0 (the "License"); 
// you may not use this file except in compliance with the License. 
// You may obtain a copy of the License at 
//   http://www.apache.org/licenses/LICENSE-2.0 
// END: License 

#include "catch.hpp"
#include <limits>

#include "cpsapi/core/cpsapiRoot.h"
#include "cpsapi/model/cpsapiModel.h"
#include "cpsapi/model/cpsapiCompartment.h"

using namespace std;
CPSAPI_NAMESPACE_USE

TEST_CASE("Edit model", "[cpsapi]")
{
  cpsapiModel Model = cpsapiRoot::addModel("test_model");
  REQUIRE(Model);

  cpsapiCompartment Compartment = Model.addCompartment("test_compartment");
  
  REQUIRE(Compartment);
  REQUIRE(Model.compartment());
  REQUIRE(!Model.compartment("other"));
  REQUIRE(Model.getCompartments().size() == 1);

  REQUIRE(Model.deleteCompartment());
  REQUIRE(Model.getCompartments().size() == 0);

  REQUIRE(cpsapiRoot::deleteModel("test_model"));
}
