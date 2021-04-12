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

#include <set>
#include <algorithm>
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

  CDataValue get(const Property & property, const CCore::Framework & framework = CCore::Framework::__SIZE) const;

  CDataValue get(const std::string & property, const std::string & framework = "") const;

  bool isValidProperty(const std::string & property) const;

  bool isValidProperty(const CData::Property & property) const;

  operator bool() const;

protected:
  typedef std::set< CData::Property > Properties;

  virtual bool set(const CData::Property & property, const CDataValue & value, const CCore::Framework & framework);

  virtual CDataValue get(const CData::Property & property, const CCore::Framework & framework) const;

  cpsapiPointer mpObject;
  Properties * mpSupportedProperties;

private:
  static Properties SupportedProperties;
};

template < typename Target, class SourceVector > std::vector< Target > convertVector(SourceVector  &src)
{
  std::vector< Target > Result(src.size());
  typename SourceVector::iterator itSrc = src.begin();

  std::for_each(Result.begin(), Result.end(), [&itSrc](Target & target){
    target = &*itSrc++;
  });

  return Result;
}


CPSAPI_NAMESPACE_END
