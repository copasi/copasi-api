// BEGIN: Copyright 
// Copyright (C) 2022 by Pedro Mendes, Rector and Visitors of the 
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

#include "cpsapi/model/cpsapiCompartment.h"
#include "cpsapi/core/cpsapiVisitor.h"
#include "cpsapi/core/cpsapiContainer.h"
#include "cpsapi/core/cpsapiDataModel.h"
#include "cpsapi/core/cpsapiValue.h"

CPSAPI_NAMESPACE_BEGIN

// static
template < class Visitor >
void cpsapiFactory::accept(Visitor & visitor , CDataObject * pDataObject)
{
  const cpsapiFactory::PartInterface & Part = cpsapiFactory::info(pDataObject);
  std::unique_ptr< cpsapiObject > Object(Part.create(pDataObject));

  if (Object)
    switch (Part.type)
      {
      case PartType::vectorCompartment:
        Part.accept(pDataObject, VisitorImplementation< cpsapiVector< cpsapiCompartment >, Visitor >(visitor));
        break;

      case PartType::vectorSpecies:
        Part.accept(pDataObject, VisitorImplementation< cpsapiVector< cpsapiSpecies >, Visitor >(visitor));
        break;

      case PartType::vectorGlobalQuantity:
        Part.accept(pDataObject, VisitorImplementation< cpsapiVector< cpsapiGlobalQuantity >, Visitor >(visitor));
        break;

      case PartType::vectorReaction:
        Part.accept(pDataObject, VisitorImplementation< cpsapiVector< cpsapiReaction >, Visitor >(visitor));
        break;

      case PartType::vectorDataModel:
        Part.accept(pDataObject, VisitorImplementation< cpsapiVector< cpsapiDataModel >, Visitor >(visitor));
        break;

      case PartType::vectorTask:
        Part.accept(pDataObject, VisitorImplementation< cpsapiVector< cpsapiTask >, Visitor >(visitor));
        break;

      case PartType::value:
        Part.accept(pDataObject, VisitorImplementation< cpsapiValue, Visitor >(visitor));
        break;

      case PartType::model:
        Part.accept(pDataObject, VisitorImplementation< cpsapiModel, Visitor >(visitor));
        break;

      case PartType::compartment:
        Part.accept(pDataObject, VisitorImplementation< cpsapiCompartment, Visitor >(visitor));
        break;

      case PartType::species:
        Part.accept(pDataObject, VisitorImplementation< cpsapiSpecies, Visitor >(visitor));
        break;

      case PartType::globalQuantity:
        Part.accept(pDataObject, VisitorImplementation< cpsapiGlobalQuantity, Visitor >(visitor));
        break;

      case PartType::reaction:
        Part.accept(pDataObject, VisitorImplementation< cpsapiReaction, Visitor >(visitor));
        break;

      case PartType::dataModel:
        Part.accept(pDataObject, VisitorImplementation< cpsapiDataModel, Visitor >(visitor));
        break;

      case PartType::parameter:
        Part.accept(pDataObject, VisitorImplementation< cpsapiParameter, Visitor >(visitor));
        break;

      case PartType::group:
        Part.accept(pDataObject, VisitorImplementation< cpsapiGroup, Visitor >(visitor));
        break;

      case PartType::method:
        Part.accept(pDataObject, VisitorImplementation< cpsapiMethod, Visitor >(visitor));
        break;

      case PartType::problem:
        Part.accept(pDataObject, VisitorImplementation< cpsapiProblem, Visitor >(visitor));
        break;

      case PartType::task:
        Part.accept(pDataObject, VisitorImplementation< cpsapiTask, Visitor >(visitor));
        break;

      case PartType::__SIZE:
        break;
      }
}

CPSAPI_NAMESPACE_END