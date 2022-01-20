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

#pragma once

#include "cpsapi/core/cpsapiObject.h"

#pragma GCC diagnostic push
#include <copasi/core/CDataContainer.h>
#pragma GCC diagnostic pop

CPSAPI_NAMESPACE_BEGIN

class cpsapiContainer : public cpsapiObject
{
public:
  /**
   * The base class
   */
  typedef cpsapiObject base;

  /**
   * The wrapped COPASI class
   */
  typedef CDataContainer wrapped;

protected:
  /**
   * Specific constructor
   * @param wrapped * pWrapped (default: nullptr)
   * @param const Type & type (default: Type::cpsapiContainer)
  */
  cpsapiContainer(wrapped * pWrapped = nullptr, const Type & type = Type::Container);

public:
  /**
   * Copy constructor
   * @param const cpsapiContainer & src
   */
  cpsapiContainer(const cpsapiContainer & src);

  /**
   * Destructor
   */
  virtual ~cpsapiContainer();

  /**
   * Accept a visitor
   * @param cpsapiVisitor & visitor
   */
  virtual void accept(cpsapiVisitor & visitor) override;
};

CPSAPI_NAMESPACE_END