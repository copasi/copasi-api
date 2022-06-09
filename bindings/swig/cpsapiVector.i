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

%ignore cpsapiVector::cpsapiValue(wrapped *);
%ignore cpsapiVector::SupportedProperties;
%ignore cpsapiVector::HiddenProperties;
%ignore cpsapiVector::SupportedReferences;
%ignore cpsapiVector::HiddenReferences;

%ignore cpsapiVector::iterator::operator++;
%ignore cpsapiVector::iterator::operator--;
%ignore cpsapiVector::operator[];

%include <cpsapi/core/cpsapiVector.h>

%template(cpsapiVectorCompartment) cpsapiVector< cpsapiCompartment >;
%extend cpsapiVector< cpsapiCompartment > {
  cpsapiCompartment & __getitem__(unsigned int i)
    {
      return $self->operator[](i);
    }
}
typedef cpsapiVector< cpsapiCompartment > cpsapiVectorCompartment;

%template(cpsapiVectorSpecies) cpsapiVector< cpsapiSpecies >;
%extend cpsapiVector< cpsapiSpecies > {
  cpsapiSpecies & __getitem__(unsigned int i)
    {
      return $self->operator[](i);
    }
}
typedef cpsapiVector< cpsapiSpecies > cpsapiVectorSpecies;

%template(cpsapiVectorGlobalQuantity) cpsapiVector< cpsapiGlobalQuantity >;
%extend cpsapiVector< cpsapiGlobalQuantity > {
  cpsapiGlobalQuantity & __getitem__(unsigned int i)
    {
      return $self->operator[](i);
    }
}
typedef cpsapiVector< cpsapiGlobalQuantity > cpsapiVectorGlobalQuantity;

%template(cpsapiVectorReaction) cpsapiVector< cpsapiReaction >;
%extend cpsapiVector< cpsapiReaction > {
  cpsapiReaction & __getitem__(unsigned int i)
    {
      return $self->operator[](i);
    }
}
typedef cpsapiVector< cpsapiReaction > cpsapiVectorReaction;

%template(cpsapiVectorEvent) cpsapiVector< cpsapiEvent >;
%extend cpsapiVector< cpsapiEvent > {
  cpsapiEvent & __getitem__(unsigned int i)
    {
      return $self->operator[](i);
    }
}
typedef cpsapiVector< cpsapiEvent > cpsapiVectorEvent;

%template(cpsapiVectorEventAssignment) cpsapiVector< cpsapiEventAssignment >;
%extend cpsapiVector< cpsapiEventAssignment > {
  cpsapiEventAssignment & __getitem__(unsigned int i)
    {
      return $self->operator[](i);
    }
}
typedef cpsapiVector< cpsapiEventAssignment > cpsapiVectorEventAssignment;

%template(cpsapiVectorDataModel) cpsapiVector< cpsapiDataModel >;
%extend cpsapiVector< cpsapiDataModel > {
  cpsapiDataModel & __getitem__(unsigned int i)
    {
      return $self->operator[](i);
    }
}
typedef cpsapiVector< cpsapiDataModel > cpsapiVectorDataModel;

%template(cpsapiVectorTask) cpsapiVector< cpsapiTask >;
%extend cpsapiVector< cpsapiTask > {
  cpsapiTask & __getitem__(unsigned int i)
    {
      return $self->operator[](i);
    }
}
typedef cpsapiVector< cpsapiTask > cpsapiVectorTask;

%template(cpsapiVectorKineticLawVariable) cpsapiVector< cpsapiKineticLawVariable >;
%extend cpsapiVector< cpsapiKineticLawVariable > {
  cpsapiKineticLawVariable & __getitem__(unsigned int i)
    {
      return $self->operator[](i);
    }
}
typedef cpsapiVector< cpsapiKineticLawVariable > cpsapiVectorKineticLawVariable;