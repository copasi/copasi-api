
#ifndef CPSAPI_NAMESPACE_H
#define CPSAPI_NAMESPACE_H 1

#if defined(__cplusplus) && defined(CPSAPI_USE_CPP_NAMESPACE) && !defined(SWIG)
  /* C++ namespace of CPSAPI */
  #define CPSAPI_NAMESPACE            libsbml
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

