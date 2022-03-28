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

#include "Visitor.h"

#include <cpsapi/core/cpsapiRoot.h>
#include <cpsapi/core/cpsapiFactory.h>
#include <cpsapi/crtp/interface.h>

CPSAPI_NAMESPACE_USE

int main(int argc, char *argv[])
{
  cpsapi::init();

  cpsapi::addDataModel();

  cpsapi::model().setProperty(cpsapiModel::Property::NAME, "model");
  cpsapi::addCompartment("compartment");
  cpsapi::model().addSpecies("species");
  
  Visitor visitor;
  cpsapi::model().accept(visitor);

  cpsapi::release();

  return 0;
}

