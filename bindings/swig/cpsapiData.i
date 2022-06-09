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

%include <cpsapi/core/cpsapiData.h>

%extend cpsapiData {
  cpsapiValue toValue() 
  {
    return $self->toObject< cpsapiValue >();
  } 

  cpsapiModel toModel()
  {
    return $self->toObject< cpsapiModel >();
  } 

  cpsapiCompartment toCompartment()
  {
    return $self->toObject< cpsapiCompartment >();
  } 

  cpsapiSpecies toSpecies()
  {
    return $self->toObject< cpsapiSpecies >();
  } 

  cpsapiGlobalQuantity toGlobalQuantity()
  {
    return $self->toObject< cpsapiGlobalQuantity >();
  } 

  cpsapiReaction toReaction()
  {
    return $self->toObject< cpsapiReaction >();
  } 

  cpsapiKineticLawVariable toKineticLawVariable()
  {
    return $self->toObject< cpsapiKineticLawVariable >();
  } 

  cpsapiEvent toEvent()
  {
    return $self->toObject< cpsapiEvent >();
  } 

  cpsapiEventAssignment toEventAssignment()
  {
    return $self->toObject< cpsapiEventAssignment >();
  } 

  cpsapiDataModel toDataModel()
  {
    return $self->toObject< cpsapiDataModel >();
  } 

  cpsapiParameter toParameter()
  {
    return $self->toObject< cpsapiParameter >();
  } 

  cpsapiGroup toGroup()
  {
    return $self->toObject< cpsapiGroup >();
  } 

  cpsapiMethod toMethod()
  {
    return $self->toObject< cpsapiMethod >();
  } 

  cpsapiProblem toProblem()
  {
    return $self->toObject< cpsapiProblem >();
  } 

  cpsapiTask toTask()
  {
    return $self->toObject< cpsapiTask >();
  } 
}
