// BEGIN: Copyright 
// Copyright (C) 2021 by Pedro Mendes, Rector and Visitors of the 
// University of Virginia, University of Heidelberg, and University 
// of Connecticut School of Medicine. 
// All rights reserved 
// END: Copyright 

// BEGIN: License 
// Licensed under the Apache License, Version 2.0 (the "License"); 
// you may not use this file except in compliance with the License. 
// You may obtain a copy of the License at 
//   http://www.apache.org/licenses/LICENSE-2.0 
// END: License 

#pragma once

#include <set>
#include <copasi/undo/CData.h>
#include <copasi/core/CDataVector.h>

#include "cpsapi/core/cpsapiTransaction.h"
#include "cpsapi/core/cpsapiVisitor.h"
#include "cpsapi/core/cpsapiPointer.h"

class CDataObject;

CPSAPI_NAMESPACE_BEGIN

class cpsapiObject 
{
public:
  enum class Property
  {
    OBJECT_NAME = CData::Property::OBJECT_NAME
  };

  cpsapiObject(CDataObject * pObject = nullptr);

  cpsapiObject(const cpsapiObject & src);

  virtual ~cpsapiObject();

  virtual void accept(cpsapiVisitor& v);

  CDataObject * getObject();

  const CDataObject * getObject() const;

  bool set(const Property & property, const CDataValue & value, const CCore::Framework & framework = CCore::Framework::__SIZE);

  bool set(const std::string & property, const CDataValue & value, const std::string & framework = "");

  bool isValidProperty(const std::string & property) const;

  bool isValidProperty(const CData::Property & property) const;

  operator bool() const;

protected:
  typedef std::set< CData::Property > Properties;

  virtual bool set(const CData::Property & property, const CDataValue & value, const CCore::Framework & framework);

  cpsapiPointer mObject;
  Properties * mpSupportedProperties;

private:
  static Properties SupportedProperties;
};

template < typename Target, class SourceVector > std::vector< Target > convertVector(SourceVector  &src)
{
  std::vector< Target > Result(src.size());

  typename std::vector< Target >::iterator it = Result.begin();
  typename std::vector< Target >::iterator end = Result.end();
  typename SourceVector::iterator itSrc = src.begin();

  for (; it != end; ++it, ++itSrc)
    *it = &*itSrc;

  return Result;
}


CPSAPI_NAMESPACE_END
