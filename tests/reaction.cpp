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
#include "cpsapi/core/cpsapiParameter.h"
#include "cpsapi/core/cpsapiGroup.h"
#include "cpsapi/model/cpsapiModel.h"
#include "cpsapi/model/cpsapiCompartment.h"
#include "cpsapi/model/cpsapiSpecies.h"
#include "cpsapi/model/cpsapiGlobalQuantity.h"
#include "cpsapi/model/cpsapiReaction.h"

using namespace std;
CPSAPI_NAMESPACE_USE

TEST_CASE("edit reaction", "[cpsapi]")
{
  REQUIRE(cpsapiObject::AllSupportedProperties< cpsapiReaction >().size() == 10);
  REQUIRE(cpsapiObject::AllSupportedProperties< cpsapiReactionParameter >().size() == 4);
  
  cpsapiModel Model = cpsapi::addDataModel("test_model").model();
  REQUIRE(Model);

  cpsapiReaction Reaction = Model.addReaction("R1");
  REQUIRE(Reaction);

  REQUIRE(Reaction.setProperty(cpsapiReaction::Property::CHEMICAL_EQUATION, "2 * H2 + O2 = 2 * H2O"));
  REQUIRE(Reaction.getProperty(cpsapiReaction::Property::CHEMICAL_EQUATION).toString() == "2 * H2 + O2 = 2 * H2O");
  
  cpsapiVector< cpsapiReactionParameter > Parameters = Reaction.parameters();
  REQUIRE(Parameters.size() == 4);

  REQUIRE(Parameters[string("k1")].setProperty(cpsapiReactionParameter::Property::VALUE, 2.0));
  REQUIRE(Reaction.setProperty(cpsapiReaction::Property::KINETIC_LAW_EXPRESSION, "k1*H2^2*O2-k2*H2O^2"));
  REQUIRE(Parameters.size() == 5);
  REQUIRE(Reaction.parameter("k1").getProperty(cpsapiReactionParameter::Property::VALUE).toDouble() == 2.0);

  for (const cpsapiReactionParameter & parameter : Parameters)
    {
      cout << parameter.getProperty(cpsapiReactionParameter::Property::NAME).toString() << ",";
      cout << parameter.getProperty(cpsapiReactionParameter::Property::ROLE).toString() << ",";
      cout << parameter.getProperty(cpsapiReactionParameter::Property::VALUE).toDouble() << ",";
      cout << parameter.getProperty(cpsapiReactionParameter::Property::MAPPING).toObject< cpsapiObject >() << endl;
    }

  REQUIRE(cpsapi::deleteDataModel("test_model"));
}
