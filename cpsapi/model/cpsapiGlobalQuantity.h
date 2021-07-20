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

#include "cpsapi/model/cpsapiModelEntity.h"

class CModelValue;

CPSAPI_NAMESPACE_BEGIN
class cpsapiGlobalQuantity : public cpsapiModelEntity
{
public:
  typedef cpsapiModelEntity base;

  enum class Property
  {
    EXPRESSION = cpsapiProperty::Type::EXPRESSION,
    INITIAL_EXPRESSION = cpsapiProperty::Type::INITIAL_EXPRESSION,
    INITIAL_VALUE = cpsapiProperty::Type::INITIAL_VALUE,
    SIMULATION_TYPE = cpsapiProperty::Type::SIMULATION_TYPE,
    ADD_NOISE = cpsapiProperty::Type::ADD_NOISE,
    NOISE_EXPRESSION = cpsapiProperty::Type::NOISE_EXPRESSION,
    OBJECT_NAME = cpsapiProperty::Type::OBJECT_NAME,
    UNIT = cpsapiProperty::Type::UNIT
  };

  static const Properties SupportedProperties;

  cpsapiGlobalQuantity(CModelValue * pModelValue = nullptr);

  cpsapiGlobalQuantity(const cpsapiGlobalQuantity & src);

  virtual ~cpsapiGlobalQuantity();

  virtual void accept(cpsapiVisitor & visitor) override;

  bool set(const Property & property, const CDataValue & value, const CCore::Framework & framework = CCore::Framework::__SIZE);

  CDataValue get(const Property & property, const CCore::Framework & framework = CCore::Framework::__SIZE) const;

  /**
   * Return the properties supported 
   * @return const Properties & supportedProperties
   */
  static const Properties & supportedProperties();

protected:
  virtual bool set(const cpsapiProperty::Type & property, const CDataValue & value, const CCore::Framework & framework) override;

  virtual CDataValue get(const cpsapiProperty::Type & property, const CCore::Framework & framework) const override;
};

CPSAPI_NAMESPACE_END