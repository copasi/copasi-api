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

%module(directors="1") cpsapi

#pragma SWIG nowarn=314,402,813,833,302,325,362,503;

#if SWIGR
%include "rtype.swg"
%include "copasi_rtype.swg"
%include "typemaps.i"
%typemap("rtype") size_t, size_t *, size_t &, const size_t&      "integer";
%typemap("rtype") bool, bool *, bool &, const bool&      "logical";
#endif 

%include cpsapiCompartment.i
%include cpsapiContainer.i
%include cpsapiDataCollector.i
%include cpsapiData.i
%include cpsapiDataModel.i
%include cpsapiEventAssignment.i
%include cpsapiEvent.i
%include cpsapiFactory.i
%include cpsapiGlobalQuantity.i
%include cpsapiGroup.i
%include cpsapiKineticLawVariable.i
%include cpsapiMethod.i
%include cpsapiModelEntity.i
%include cpsapiModel.i
%include cpsapiObjectData.i
%include cpsapiObject.i
%include cpsapiParameter.i
%include cpsapiProblem.i
%include cpsapiProperty.i
%include cpsapiReaction.i
%include cpsapiRoot.i
%include cpsapiSpecies.i
%include cpsapiTask.i
%include cpsapiTransaction.i
%include cpsapiValue.i
%include cpsapiVector.i
%include cpsapiVisitor.i
