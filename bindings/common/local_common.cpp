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
#include <cpsapi/templates/object.h>
