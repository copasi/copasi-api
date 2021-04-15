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

#pragma once

#include "cpsapi/core/cpsapiObject.h"

#include <copasi/core/CDataObjectReference.h>

#include <limits>

CPSAPI_NAMESPACE_BEGIN

class cpsapiValue : public cpsapiObject
{
private:
  typedef cpsapiObject base;

public:
  cpsapiValue() = delete;

  cpsapiValue(CDataObject * pValue);

  cpsapiValue(const cpsapiValue & src);

  virtual ~cpsapiValue();

  virtual void accept(cpsapiVisitor & visitor) override;

  bool setValue(const CDataValue & value);

  CDataValue getValue() const;

  operator CDataValue() const;

  bool valid() const;
  
  CDataValue::Type getType() const;
};

CPSAPI_NAMESPACE_END