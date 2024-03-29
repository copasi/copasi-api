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

#include "cpsapi/core/cpsapiRoot.h"
#include "cpsapi/core/cpsapiDataModel.h"
#include "cpsapi/core/cpsapiValue.h"
#include "cpsapi/core/cpsapiParameter.h"
#include "cpsapi/core/cpsapiGroup.h"
#include "cpsapi/model/cpsapiModel.h"
#include "cpsapi/model/cpsapiCompartment.h"
#include "cpsapi/model/cpsapiSpecies.h"
#include "cpsapi/model/cpsapiGlobalQuantity.h"

using namespace std;
CPSAPI_NAMESPACE_USE

TEST_CASE("Edit model", "[cpsapi]")
{
  REQUIRE(cpsapiObject::supportedProperties< cpsapiObject >().size() == 3);
  REQUIRE(cpsapiObject::supportedProperties< cpsapiContainer >().size() == 3);
  REQUIRE(cpsapiObject::supportedProperties< cpsapiValue >().size() == 4);
  REQUIRE(cpsapiObject::supportedProperties< cpsapiDataModel >().size() == 3);
  REQUIRE(cpsapiObject::supportedProperties< cpsapiModelEntity >().size() == 9);
  REQUIRE(cpsapiObject::supportedProperties< cpsapiCompartment >().size() == 11);
  REQUIRE(cpsapiObject::supportedProperties< cpsapiValue >().size() == 4);
  REQUIRE(cpsapiObject::supportedProperties< cpsapiSpecies >().size() == 10);
  REQUIRE(cpsapiObject::supportedProperties< cpsapiModel >().size() == 12);
  REQUIRE(cpsapiObject::supportedProperties< cpsapiParameter >().size() == 4);
  REQUIRE(cpsapiObject::supportedProperties< cpsapiGroup >().size() == 3);

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

  cpsapiValue Value(static_cast< CCompartment * >(*Compartment)->getInitialValueReference());
  REQUIRE(Value.valid());
  REQUIRE(Value.setProperty(cpsapiValue::Property::VALUE, 5.0));
  REQUIRE(Compartment.getProperty(cpsapiCompartment::Property::INITIAL_VALUE).toDouble() == 5.0);
  REQUIRE_FALSE(Value.setProperty(cpsapiValue::Property::VALUE, "wrong type"));
  
  REQUIRE(Model.compartment());
  REQUIRE_FALSE(Model.compartment("other"));
  REQUIRE(Model.getCompartments().size() == 1);

  cpsapiSpecies Species = Compartment.addSpecies("test_species");
  double ParticleNumber = Species.getProperty(cpsapiSpecies::Property::INITIAL_VALUE, CCore::Framework::ParticleNumbers).toDouble();

  cpsapiSpecies Species2 = Compartment.addSpecies("test_species_2");
  REQUIRE(ParticleNumber == Species2.getProperty(cpsapiSpecies::Property::INITIAL_VALUE, CCore::Framework::ParticleNumbers).toDouble());

  cpsapi::beginTransaction();
  
  Compartment.setProperty(cpsapiCompartment::Property::INITIAL_VALUE, 1e-10, CCore::Framework::Concentration);
  REQUIRE(ParticleNumber == Species.getProperty(cpsapiSpecies::Property::INITIAL_VALUE, CCore::Framework::ParticleNumbers).toDouble());
  
  cpsapi::endTransaction();

  REQUIRE_FALSE(ParticleNumber == Species.getProperty(cpsapiSpecies::Property::INITIAL_VALUE, CCore::Framework::ParticleNumbers).toDouble());

  REQUIRE_FALSE(Model.addSpecies("test_species"));
  REQUIRE_FALSE(Copy.addSpecies("test_species"));
  REQUIRE(Copy.getSpecies().size() == 2);

  cpsapiVector< cpsapiSpecies > SpeciesVector = Compartment.getSpecies();
  REQUIRE(SpeciesVector);
  REQUIRE(SpeciesVector.size() == 2);

  cpsapiVector< cpsapiSpecies >::iterator it = SpeciesVector.begin();
  cpsapiVector< cpsapiSpecies >::iterator end = SpeciesVector.end();

  REQUIRE(cpsapiSpecies(*it));

  for (cpsapiSpecies & species : SpeciesVector)
    REQUIRE_FALSE(ParticleNumber == species.getProperty(cpsapiSpecies::Property::INITIAL_VALUE, CCore::Framework::ParticleNumbers).toDouble());
    
  std::ostringstream Equation;
  Equation << 3 << "*" << Model.species().getProperty(cpsapiSpecies::Property::OBJECT_UNIQUE_NAME).toString() << "->";
  REQUIRE("3*test_species_2->" == Equation.str());

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
