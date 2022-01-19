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

#include "catch.hpp"
#include <limits>

#include <iostream>
#include <string>
#include <sstream>

#include <cpsapi/cpsapiConfig.h>
#include <cpsapi/core/cpsapiRoot.h>
#include <cpsapi/core/cpsapiDataModel.h>
#include <cpsapi/model/cpsapiModel.h>
#include <cpsapi/model/cpsapiSpecies.h>

#include <copasi/CopasiTypes.h>

extern std::string getTestFile(const std::string & fileName);

using namespace std;
CPSAPI_NAMESPACE_USE


TEST_CASE("load COPASI file and access via cpsapi", "[cpsapi]")
{
  std::string fileName = getTestFile("test-data/brusselator.cps");

  REQUIRE(cpsapi::addDataModel("brusselator") == true);
  REQUIRE(cpsapi::loadFromFile(fileName) == true);
  REQUIRE(cpsapi::model() == true);
  REQUIRE(cpsapi::model().getDataCN (cpsapiObject::Reference::OBJECT_NAME) == "CN=Root,Model=The Brusselator,Reference=Name");
  REQUIRE(cpsapi::model().getSpecies().size() == 6);

  REQUIRE(cpsapi::deleteDataModel());
}
