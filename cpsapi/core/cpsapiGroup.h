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

#include "cpsapi/core/cpsapiParameter.h"

#include <copasi/utilities/CCopasiParameterGroup.h>

CPSAPI_NAMESPACE_BEGIN

class cpsapiGroup : public cpsapiParameter
{
public:
  /**
   * Enumeration of the exposed properties
   */ 
  enum class Property {};

  /**
   * Static set of supported properties
   */
  static const Properties SupportedProperties;

  /**
   * The base class
   */
  typedef cpsapiParameter base;

  /**
   * The wrapped COPASI class
   */
  typedef CCopasiParameterGroup wrapped;

  class Data : public base::Data
  {
  public:
    Data(const base::Data & data)
      : base::Data(data)
      , mDefaultParameter()
    {}

    virtual ~Data() {}

    cpsapiParameter mDefaultParameter;
  };

  /**
   * Specific constructor
   * @param wrapped * pGroup
   */
  cpsapiGroup(wrapped * pGroup);

  /**
   * Copy constructor
   * @param const cpsapiGroup & src
   */
  cpsapiGroup(const cpsapiGroup & src);

  virtual ~cpsapiGroup();

  virtual void accept(cpsapiVisitor & visitor) override;

  cpsapiParameter addParameter(const std::string & name, const CDataValue & value, CCopasiParameter::Type type = CCopasiParameter::Type::__SIZE);

  cpsapiGroup addGroup(const std::string & name);

  bool deleteParameter(const std::string & name = "");

  cpsapiParameter parameter(const std::string & name = "");

  std::vector< cpsapiParameter > getParameters() const;

protected:
  cpsapiParameter __parameter(const std::string & name) const;

  void updateDefaultParameter(const cpsapiParameter & parameter);
};

CPSAPI_NAMESPACE_END