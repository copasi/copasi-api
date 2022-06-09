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
%include "cpsapi_rtype.swg"
%include "typemaps.i"
%typemap("rtype") size_t, size_t *, size_t &, const size_t&      "integer";
%typemap("rtype") bool, bool *, bool &, const bool&      "logical";
#endif 

%include std_string.i
%include std_vector.i
%include std_map.i

%{
  
#include <cpsapi/git-commit.h>
#include <cpsapi/cpsapiConfig.h>
#include <cpsapi/core/cpsapiObjectData.h>
#include <cpsapi/core/cpsapiGroup.h>
#include <cpsapi/core/cpsapiProperty.h>
#include <cpsapi/core/cpsapiValue.h>
#include <cpsapi/core/cpsapiObject.h>
#include <cpsapi/core/cpsapiParameter.h>
#include <cpsapi/core/cpsapiDataModel.h>
#include <cpsapi/core/cpsapiContainer.h>
#include <cpsapi/core/cpsapiRoot.h>
#include <cpsapi/core/cpsapiFactory.h>
#include <cpsapi/core/cpsapiVector.h>
#include <cpsapi/core/cpsapiDataCollector.h>
#include <cpsapi/core/cpsapiVisitor.h>
#include <cpsapi/core/cpsapiData.h>
#include <cpsapi/model/cpsapiGlobalQuantity.h>
#include <cpsapi/model/cpsapiCompartment.h>
#include <cpsapi/model/cpsapiModel.h>
#include <cpsapi/model/cpsapiModelEntity.h>
#include <cpsapi/model/cpsapiReaction.h>
#include <cpsapi/model/cpsapiSpecies.h>
#include <cpsapi/model/cpsapiTransaction.h>
#include <cpsapi/model/cpsapiKineticLawVariable.h>
#include <cpsapi/model/cpsapiEvent.h>
#include <cpsapi/model/cpsapiEventAssignment.h>
#include <cpsapi/task/cpsapiTask.h>
#include <cpsapi/task/cpsapiProblem.h>
#include <cpsapi/task/cpsapiMethod.h>

CPSAPI_NAMESPACE_USE

%}

%include <cpsapi/cpsapiConfig.h>

%include cpsapiObjectData.i
%include cpsapiProperty.i
%include cpsapiObject.i
%include cpsapiValue.i
%include cpsapiContainer.i
%include cpsapiVector.i


/*
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
%include cpsapiParameter.i
%include cpsapiProblem.i
%include cpsapiReaction.i
%include cpsapiRoot.i
%include cpsapiSpecies.i
%include cpsapiTask.i
%include cpsapiTransaction.i
%include cpsapiValue.i
%include cpsapiVisitor.i
*/