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

#include "cpsapi/cpsapiConfig.h"

#include <cstddef>
#include <utility>

CPSAPI_NAMESPACE_BEGIN

struct cpsapiVisitor
{
private:
  template < typename visitor, typename visited, typename Z = decltype(std::declval< visitor >().visit(std::declval< visited * >())) >
  static void doVisit(visitor & v, visited * pV, int)
  {
    v.visit(pV);
  }

  template < typename visitor, typename visited >
  static void doVisit(visitor, visited *, ...)
  {}

public:
  template < typename Visitor, typename Visited >
  static void acceptIfVisitable(Visitor & visitor, Visited * pVisited)
  {
    doVisit(visitor, pVisited, 0);
  }

  template< class Visited > 
  static void visit(Visited * pObject) = delete;
};

CPSAPI_NAMESPACE_END