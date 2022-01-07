#pragma once

#include "cpsapi/core/cpsapiContainer.h"
#include "cpsapi/core/cpsapiVector.h"
#include "copasi/model/CReaction.h"

class CReaction;

CPSAPI_NAMESPACE_BEGIN

class cpsapiReactionParameter : public cpsapiObject
{
public:
  class FakeData : public CDataObject
  {
  public:
    static FakeData * fromData(const CData & data, CUndoObjectInterface * pParent)
    {
      return nullptr;
    }

    FakeData(CReaction * pReaction, const std::string & name = "")
      : CDataObject()
      , mpReaction(pReaction)
      , mName(name)
    {}

    FakeData(const FakeData & src, CDataContainer * pParent = nullptr)
      : CDataObject(src)
      , mpReaction(src.mpReaction)
      , mName(src.mName)
    {}

    virtual ~FakeData() {} 

    CReaction * mpReaction;
    std::string mName;
  };

  /**
   * Enumeration of the exposed properties
   */ 
  enum class Property
  {
    NAME = cpsapiProperty::Type::OBJECT_NAME,
    ROLE = cpsapiProperty::Type::PARAMETER_ROLE,
    VALUE = cpsapiProperty::Type::PARAMETER_VALUE,
    MAPPING = cpsapiProperty::Type::PARAMETER_MAPPING
  };

  /**
   * Static set of supported properties
   */
  static const Properties SupportedProperties;

  /**
   * Static set of hidden properties
   */
  static const Properties HiddenProperties;

  /**
   * The base class
   */
  typedef cpsapiObject base;

  /**
   * The wrapped COPASI class
   */
  typedef FakeData wrapped;

  /**
   * Specific constructor
   * @param wrapped * pWrapped
   */
  cpsapiReactionParameter(wrapped * pWrapped = nullptr);

  /**
   * Copy constructor
   * @param const cpsapiReactionParameter & src
   */
  cpsapiReactionParameter(const cpsapiReactionParameter & src);

  virtual ~cpsapiReactionParameter();

  virtual void accept(cpsapiVisitor & visitor) override;

  bool setProperty(const Property & property, const cpsapiVariant & value, const CCore::Framework & framework = CCore::Framework::__SIZE);

  cpsapiVariant getProperty(const Property & property, const CCore::Framework & framework = CCore::Framework::__SIZE) const;

protected:
  virtual bool setProperty(const cpsapiProperty::Type & property, const cpsapiVariant & value, const CCore::Framework & framework) override;

  virtual cpsapiVariant getProperty(const cpsapiProperty::Type & property, const CCore::Framework & framework) const override;

};

template <>
inline size_t cpsapiVector< cpsapiReactionParameter >::index(const std::string & name) const
{
  if (operator bool())
    {
      wrapped * pWrapped = WRAPPED;
      wrapped::const_iterator itParameter = pWrapped->begin();
      wrapped::const_iterator endParameter = pWrapped->end();
      
      for (size_t Index = 0; itParameter != endParameter; ++itParameter, ++Index)
        {
          if (itParameter->mName == name)
            return Index;
        }
    }

  return C_INVALID_INDEX;
};

CPSAPI_NAMESPACE_END

