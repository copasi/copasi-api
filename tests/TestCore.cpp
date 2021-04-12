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

#include <iostream>
#include <string>
#include <sstream>

#include <cpsapi/cpsapiConfig.h>

#include <copasi/CopasiTypes.h>

extern std::string getTestFile(const std::string & fileName);

using namespace std;
CPSAPI_NAMESPACE_USE


TEST_CASE("load copasi file and access via regular COPASI api", "[copasi]")
{
  auto fileName = getTestFile("test-data/brusselator.cps");
  auto* dm = CRootContainer::addDatamodel();
  REQUIRE(dm != NULL);
    
  REQUIRE(dm->loadModel(fileName, NULL) == true);

  auto * model = dm->getModel();
  REQUIRE(model != NULL);
  REQUIRE(model->getNumMetabs() == 6);
}
