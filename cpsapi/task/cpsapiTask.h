#pragma once

#include "cpsapi/core/cpsapiContainer.h"
#include "cpsapi/task/cpsapiMethod.h"
#include "cpsapi/task/cpsapiProblem.h"

CPSAPI_NAMESPACE_BEGIN

class cpsapiTask: public cpsapiContainer
{
public:
  /**
   * Enumeration of the exposed properties
   */ 
  enum class Property
  {
    VALUE = cpsapiProperty::Type::VALUE,
    OBJECT_NAME = cpsapiProperty::Type::OBJECT_NAME,
    DISPLAY_NAME = cpsapiProperty::Type::DISPLAY_NAME,
    CN = cpsapiProperty::Type::CN
  };

  /**
   * Static set of supported properties
   */
  static const Properties SupportedProperties;

  /**
   * The base class
   */
  typedef cpsapiContainer base;

  /**
   * The wrapped COPASI class
   */
  typedef CCopasiTask wrapped;

 class Data : public base::Data
  {
  public:
    Data(const base::Data & data)
      : base::Data(data)
      , mMethod()
      , mProblem()
    {}

    virtual ~Data() {}

    cpsapiMethod mMethod;
    cpsapiProblem mProblem;
  };


  /**
   * Specific constructor
   * @param wrapped * pWrapped (default: nullptr)
  */
  cpsapiTask(wrapped * pWrapped = nullptr);

  /**
   * Copy constructor
   * @param const cpsapiTask & src
   */
  cpsapiTask(const cpsapiTask & src);

  /**
   * Destructor
   */
  virtual ~cpsapiTask();

  /**
   * Accept a visitor
   * @param cpsapiVisitor & visitor
   */
  virtual void accept(cpsapiVisitor & visitor) override;

  cpsapiMethod createMethod(const CTaskEnum::Method & type) const;

  bool setMethod(const cpsapiMethod & method);
  
  cpsapiMethod method() const;

  cpsapiProblem problem() const;
  
  bool setProperty(const Property & property, const cpsapiVariant & value, const CCore::Framework & framework = CCore::Framework::__SIZE);

  cpsapiVariant getProperty(const Property & property, const CCore::Framework & framework = CCore::Framework::__SIZE) const;

protected:
  virtual bool setProperty(const cpsapiProperty::Type & property, const cpsapiVariant & value, const CCore::Framework & framework) override;

  virtual cpsapiVariant getProperty(const cpsapiProperty::Type & property, const CCore::Framework & framework) const override;
};

CPSAPI_NAMESPACE_END