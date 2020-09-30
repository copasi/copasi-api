// BEGIN: Copyright 
// Copyright (C) 2019 - 2020 by Pedro Mendes, Rector and Visitors of the 
// University of Virginia, University of Heidelberg, and University 
// of Connecticut School of Medicine. 
// All rights reserved. 
// END: Copyright 

// BEGIN: License 
// Licensed under the Apache License, Version 2.0 (the "License"); 
// you may not use this file except in compliance with the License. 
// You may obtain a copy of the License at 
//  
//   http://www.apache.org/licenses/LICENSE-2.0 
// END: License 

#ifndef CPSAPI_NAMESPACE_H
#define CPSAPI_NAMESPACE_H 1

#if defined(__cplusplus) && defined(CPSAPI_USE_CPP_NAMESPACE) && !defined(SWIG)
  /* C++ namespace of CPSAPI */
  #define CPSAPI_NAMESPACE            cpsapi
  #define CPSAPI_NAMESPACE_BEGIN      namespace CPSAPI_NAMESPACE {
  #define CPSAPI_NAMESPACE_END        }
  #define CPSAPI_NAMESPACE_USE        using namespace CPSAPI_NAMESPACE;
  #define CPSAPI_NAMESPACE_QUALIFIER  CPSAPI_NAMESPACE::

  namespace CPSAPI_NAMESPACE {}
#else
  #define CPSAPI_NAMESPACE 
  #define CPSAPI_NAMESPACE_BEGIN
  #define CPSAPI_NAMESPACE_END  
  #define CPSAPI_NAMESPACE_USE 
  #define CPSAPI_NAMESPACE_QUALIFIER 
#endif


#endif  /* CPSAPI_NAMESPACE_H */

