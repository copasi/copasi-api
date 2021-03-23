// BEGIN: Copyright 
// Copyright (C) 2021 Rector and Visitors of the University of Virginia 
// All rights reserved 
// END: Copyright 

// BEGIN: License 
// Licensed under the Apache License, Version 2.0 (the "License"); 
// you may not use this file except in compliance with the License. 
// You may obtain a copy of the License at 
//   http://www.apache.org/licenses/LICENSE-2.0 
// END: License 

#pragma once

#include <copasi/core/CDataObject.h>

#include "cpsapi/cpsapiNamespace.h"

CPSAPI_NAMESPACE_BEGIN

class cpsapiObject 
{
public:
  typedef std::set< CData::Property > ValidProperties;

  enum Property
  {
    OBJECT_NAME = CData::Property::OBJECT_NAME
  };

  cpsapiObject() = delete;

  cpsapiObject(CDataObject * pObject);

  cpsapiObject(const cpsapiObject & src);

  virtual ~cpsapiObject();

  CDataObject & getObject();

  const CDataObject & getObject() const;

  bool set(const Property & property, const CDataValue & value);

  bool set(const std::string & property, const CDataValue & value);

  bool isValidAttribute(const std::string & property) const;

protected:
  virtual bool set(const CData::Property & property, const CDataValue & value);

  virtual bool isValidAttribute(const CData::Property & property) const;

private:
  static ValidProperties __ValidProperties;

  CDataObject * mpObject;
};

CPSAPI_NAMESPACE_END
