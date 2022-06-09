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

%ignore cpsapiObject::cpsapiObject;
%ignore cpsapiObject::~cpsapiObject;
%ignore cpsapiObject::SupportedProperties;
%ignore cpsapiObject::HiddenProperties;
%ignore cpsapiObject::SupportedReferences;
%ignore cpsapiObject::HiddenReferences;
%ignore cpsapiObject::operator->;
%ignore cpsapiObject::operator*;

%include <cpsapi/core/cpsapiObject.h>

%extend cpsapiObject {
  CDataObject * dataObject() {
    return $self->operator*();
  }
}
