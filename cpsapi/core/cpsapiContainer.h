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
   * The class
   */
  typedef cpsapiContainer self;

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
   * @param const Type & type (default: Type::Container)
  */
  cpsapiContainer(wrapped * pWrapped = nullptr, const cpsapiObjectData::Type & type = cpsapiObjectData::Type::Container);

public:
  /**
   * Destructor
   */
  virtual ~cpsapiContainer();

  /**
   * Accept the given visitor
   * 
   * @tparam Visitor 
   * @param Visitor & visitor 
   */
  template < typename Visitor >
  void accept(Visitor & visitor);
};

template < class Visitor >
void cpsapiContainer::accept(Visitor & visitor)
{
  if (isValid())
    {
      cpsapiVisitor::acceptIfVisitable(visitor, this);
      base::accept(visitor);

      wrapped::objectMap & Objects = WRAPPED->getObjects();

      for (CDataObject * pDataObject : Objects)
        {
          cpsapiFactory::accept< Visitor >(visitor, pDataObject);
        }
    }
}

CPSAPI_NAMESPACE_END