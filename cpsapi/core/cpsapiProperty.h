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

#include <string>

#pragma GCC diagnostic push
#include <copasi/core/CEnumAnnotation.h>
#pragma GCC diagnostic pop

#include "cpsapi/cpsapiConfig.h"

CPSAPI_NAMESPACE_BEGIN

class cpsapiProperty
{
public:
  enum Type
  {
    EXPRESSION = 0,
    VALUE,
    INTENSIVE_VALUE,
    RATE,
    INTENSIVE_RATE,
    INITIAL_EXPRESSION,
    INITIAL_VALUE,
    INITIAL_INTENSIVE_VALUE,
    INITIAL_RATE,
    INITIAL_INTENSIVE_RATE,
    SIMULATION_TYPE,
    SPATIAL_DIMENSION,
    ADD_NOISE,
    NOISE_EXPRESSION,
    CHEMICAL_EQUATION,
    KINETIC_LAW,
    KINETIC_LAW_EXPRESSION,
    KINETIC_LAW_UNIT_TYPE,
    KINETIC_LAW_VARIABLE_MAPPING,
    LOCAL_REACTION_PARAMETERS,
    SCALING_COMPARTMENT,
    FLUX,
    PARTICLE_FLUX,
    INITIAL_FLUX,
    INITIAL_PARTICLE_FLUX,
    OBJECT_UUID,
    NAME,
    OBJECT_PARENT_CN,
    OBJECT_TYPE,
    OBJECT_FLAG,
    OBJECT_HASH,
    OBJECT_INDEX,
    OBJECT_REFERENCES,
    OBJECT_REFERENCE,
    OBJECT_REFERENCE_CN,
    OBJECT_REFERENCE_INDEX,
    OBJECT_POINTER,
    OBJECT_UNIQUE_NAME,
    CN,
    EVALUATION_TREE_TYPE,
    TASK_TYPE,
    TASK_SCHEDULED,
    TASK_UPDATE_MODEL,
    TASK_REPORT,
    TASK_REPORT_TARGET,
    TASK_REPORT_APPEND,
    TASK_REPORT_CONFIRM_OVERWRITE,
    PROBLEM,
    METHOD,
    METHOD_TYPE,
    PLOT_TYPE,
    PLOT_ITEM_TYPE,
    PARAMETER,
    PARAMETER_TYPE,
    PARAMETER_ROLE,
    PARAMETER_USED,
    PARAMETER_VALUE,
    PARAMETER_MAPPING,
    UNIT,
    VOLUME_UNIT,
    AREA_UNIT,
    LENGTH_UNIT,
    TIME_UNIT,
    QUANTITY_UNIT,
    MODEL_TYPE,
    AVOGADRO_NUMBER,
    DIMENSIONALITY,
    ARRAY_ELEMENT_INDEX,
    REPORT_SEPARATOR,
    REPORT_IS_TABLE,
    REPORT_SHOW_TITLE,
    REPORT_PRECISION,
    NOTES,
    MIRIAM_RDF_XML,
    MIRIAM_PREDICATE,
    MIRIAM_RESOURCE,
    MIRIAM_DESCRIPTION,
    MIRIAM_ID,
    DATE,
    GIVEN_NAME,
    FAMILY_NAME,
    EMAIL,
    ORGANIZATION,
    FRAMEWORK,
    DELAY_ASSIGNMENT,
    FIRE_AT_INITIALTIME,
    PERSISTENT_TRIGGER,
    TRIGGER_EXPRESSION,
    DELAY_EXPRESSION,
    PRIORITY_EXPRESSION,
    ASSIGNMENTS,
    VECTOR_CONTENT,
    UNIT_SYMBOL,
    UNIT_EXPRESSION,
    NOISE,
    PROPENSITY,
    __SIZE
  };

  static const CEnumAnnotation< std::string, Type > Name;
};

typedef cpsapiProperty cpsapiReference;

CPSAPI_NAMESPACE_END