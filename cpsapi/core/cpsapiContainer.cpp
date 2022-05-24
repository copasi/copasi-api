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

#include "cpsapi/core/cpsapiContainer.h"

#pragma GCC diagnostic push
#include <copasi/core/CDataContainer.h>
#include <copasi/core/CDataObjectReference.h>
#include <copasi/model/CModel.h>
#pragma GCC diagnostic pop

#include <algorithm>
#include <typeindex>

CPSAPI_NAMESPACE_USE

cpsapiContainer::cpsapiContainer(wrapped * pWrapped, const cpsapiObjectData::Type & type)
  : base(pWrapped, type)
{}

// virtual
cpsapiContainer::~cpsapiContainer()
{}

