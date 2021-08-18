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

#include "Visitor.h"

#include <cpsapi/core/cpsapiRoot.h>

#include <type_traits>

CPSAPI_NAMESPACE_USE

int main(int argc, char *argv[])
{

  cpsapi::init();

  cpsapi::addDataModel();

  cpsapi::model().setProperty(cpsapiModel::Property::OBJECT_NAME, "model");
  cpsapi::addCompartment("compartment");
  cpsapi::model().addSpecies("species");
  Visitor V;

  cpsapi::model().accept(V);

  cpsapi::release();
  
  return 0;
}

// virtual
void Visitor::visit(cpsapiObject * pObject, const cpsapiObject::Type & type)
{
  // Sanity check
  if (pObject == nullptr || !*pObject)
    return;

  switch (type)
    {
    case cpsapiObject::Type::cpsapiModel:
      std::cout << "visit cpsapiModel: " << static_cast< cpsapiModel * >(pObject)->getObject()->getCN() << std::endl;
      break;

    case cpsapiObject::Type::cpsapiCompartment:
      std::cout << "visit cpsapiCompartment: " << static_cast< cpsapiCompartment * >(pObject)->getObject()->getCN() << std::endl;
      break;

    case cpsapiObject::Type::cpsapiSpecies:
      std::cout << "visit cpsapiSpecies: " << static_cast< cpsapiSpecies * >(pObject)->getObject()->getCN() << std::endl;
      break;

    case cpsapiObject::Type::cpsapiContainer:
      std::cout << "visit cpsapiContainer: " << static_cast< cpsapiContainer * >(pObject)->getObject()->getCN() << std::endl;
      break;

    case cpsapiObject::Type::cpsapiValue:
      std::cout << "visit cpsapiValue: " << static_cast< cpsapiValue * >(pObject)->getObject()->getCN() << std::endl;
      break;

    default:
      std::cout << "visit unhandled (" << (std::underlying_type< cpsapiObject::Type >::type) type << "): " << static_cast< cpsapiObject * >(pObject)->getObject()->getCN() << std::endl;
      break;
    }
}
