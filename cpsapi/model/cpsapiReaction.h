#pragma once

#include "cpsapi/core/cpsapiContainer.h"

class CReaction;

CPSAPI_NAMESPACE_BEGIN

class cpsapiReaction: public cpsapiContainer
{
public:
  typedef cpsapiContainer base;

  enum class Property
  {
    CHEMICAL_EQUATION = cpsapiProperty::Type::CHEMICAL_EQUATION,
    KINETIC_LAW = cpsapiProperty::Type::KINETIC_LAW,
    KINETIC_LAW_UNIT_TYPE = cpsapiProperty::Type::KINETIC_LAW_UNIT_TYPE,
    KINETIC_LAW_VARIABLE_MAPPING = cpsapiProperty::Type::KINETIC_LAW_VARIABLE_MAPPING,
    LOCAL_REACTION_PARAMETERS = cpsapiProperty::Type::LOCAL_REACTION_PARAMETERS,
    SCALING_COMPARTMENT = cpsapiProperty::Type::SCALING_COMPARTMENT,
    ADD_NOISE = cpsapiProperty::Type::ADD_NOISE,
    NOISE_EXPRESSION = cpsapiProperty::Type::NOISE_EXPRESSION,
    OBJECT_NAME = cpsapiProperty::Type::OBJECT_NAME,
    DISPLAY_NAME = cpsapiProperty::Type::DISPLAY_NAME,
    CN = cpsapiProperty::Type::CN
  };

  static const Properties SupportedProperties;

  cpsapiReaction() = delete;

  cpsapiReaction(CReaction * pReaction);

  cpsapiReaction(const cpsapiReaction & src);

  virtual ~cpsapiReaction();

  virtual void accept(cpsapiVisitor & visitor) override;

  bool setProperty(const Property & property, const CDataValue & value, const CCore::Framework & framework = CCore::Framework::__SIZE);

  CDataValue getProperty(const Property & property, const CCore::Framework & framework = CCore::Framework::__SIZE) const;

protected:
  virtual bool setProperty(const cpsapiProperty::Type & property, const CDataValue & value, const CCore::Framework & framework) override;

  virtual CDataValue getProperty(const cpsapiProperty::Type & property, const CCore::Framework & framework) const override;
};

CPSAPI_NAMESPACE_END