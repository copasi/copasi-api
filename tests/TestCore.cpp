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
  auto fileName = getTestFile("/test-data/brusselator.cps");
  auto* dm = CRootContainer::addDatamodel();
  REQUIRE(dm != NULL);
    
  REQUIRE(dm->loadModel(fileName, NULL) == true);

  auto * model = dm->getModel();
  REQUIRE(model != NULL);
  REQUIRE(model->getNumMetabs() == 6);
}
