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

#include "cpsapi/core/cpsapiProperty.h"

CPSAPI_NAMESPACE_USE

// static
const CEnumAnnotation< std::string, cpsapiProperty::Type > cpsapiProperty::Name(
{
  "Expression", // EXPRESSION
  "Value", // VALUE
  "Intensive Value", // INTENSIVE_VALUE
  "Rate", // RATE
  "Intensive Rate", // INTENSIVE_RATE
  "Initial Expression", // INITIAL_EXPRESSION
  "Initial Value", // INITIAL_VALUE
  "Initial Intensive Value", // INITIAL_INTENSIVE_VALUE
  "Initial Rate", // INITIAL_RATE
  "Initial Intensive Rate", // INITIAL_INTENSIVE_RATE
  "Simulation Type", // SIMULATION_TYPE
  "Spatial Dimensions", // SPATIAL_DIMENSION
  "Add Noise", // ADD_NOISE
  "Noise Expression", // NOISE_EXPRESSION
  "Chemical Equation", // CHEMICAL_EQUATION
  "Kinetic Law", // KINETIC_LAW
  "Kinetic Expression", // KINETIC_LAW_EXPRESSION
  "Kinetic Law Unit Type", // KINETIC_LAW_UNIT_TYPE
  "Kinetic Law Variable Mapping", // KINETIC_LAW_VARIABLE_MAPPING
  "Local Reaction Parameters", // LOCAL_REACTION_PARAMETERS
  "Scaling Compartment", // SCALING_COMPARTMENT
  "Flux", // FLUX
  "Particle Flux", // PARTICLE_FLUX
  "Initial Flux", // INITIAL_FLUX
  "Initial Particle Flux", // INITIAL_PARTICLE_FLUX
  "Object UUID", // OBJECT_UUID
  "Object Name", // NAME
  "Object Parent CN", // OBJECT_PARENT_CN
  "Object Type", // OBJECT_TYPE
  "Object Flag", // OBJECT_FLAG
  "Object Hash", // OBJECT_HASH,
  "Object Index", // OBJECT_INDEX
  "Object References", // OBJECT_REFERENCES
  "Object Reference", // OBJECT_REFERENCE
  "Object Reference CN", // OBJECT_REFERENCE_CN
  "Object Reference Index", // OBJECT_REFERENCE_INDEX
  "Object Pointer", // OBJECT_POINTER
  "Display Name", // UNIQUE_NAME
  "CN", // CN
  "Evaluation Tree Type", // EVALUATION_TREE_TYPE
  "Task Type", // TASK_TYPE
  "Task Scheduled", // TASK_SCHEDULED
  "Task Update Model", // TASK_UPDATE_MODEL
  "Task Report", // TASK_REPORT
  "Task Report Target", // TASK_REPORT_TARGET
  "Task Report Append", // TASK_REPORT_APPEND
  "Task Report Confirm Overwrite", // TASK_REPORT_CONFIRM_OVERWRITE
  "Problem", // PROBLEM
  "Method", // METHOD
  "Method Type", // METHOD_TYPE
  "Plot Type", // PLOT_TYPE
  "Plot Item Type", // PLOT_ITEM_TYPE
  "Parameter", // PARAMETER
  "Parameter Type", // PARAMETER_TYPE
  "Parameter Role", // PARAMETER_ROLE
  "Parameter Used", //  PARAMETER_USED
  "Parameter Value", // PARAMETER_VALUE
  "Parameter Mapping", // PARAMETER_MAPPING
  "Unit", // UNIT
  "Volume Unit", // VOLUME_UNIT
  "Area Unit", // AREA_UNIT
  "Length Unit", // LENGTH_UNIT
  "Time Unit", // TIME_UNIT
  "Quantity Unit", // QUANTITY_UNIT
  "Model Type", // MODEL_TYPE
  "Avogadro's Number", // AVOGADRO_NUMBER
  "Dimensionality", // DIMENSIONALITY
  "Array Element Index", // ARRAY_ELEMENT_INDEX
  "Report Separator", // REPORT_SEPARATOR
  "Report is Table", // REPORT_IS_TABLE
  "Report show Title", // REPORT_SHOW_TITLE
  "Report Precision", // REPORT_PRECISION
  "Notes", // NOTES
  "MIRIAM RDF/XML", // MIRIAM_RDF_XML
  "MIRIAM Predicate", // MIRIAM_PREDICATE
  "MIRIAM Resource", // MIRIAM_RESOURCE
  "MIRIAM Description", // MIRIAM_DESCRIPTION
  "MIRIAM Id", // MIRIAM_ID
  "Date", // DATE
  "Given Name", // GIVEN_NAME
  "Family Name", // FAMILY_NAME
  "Email", // EMAIL
  "Organization", // ORGANIZATION
  "Framework", // FRAMEWORK
  "Delay Assignment", // DELAY_ASSIGNMENT
  "Fire at Initial Time", // FIRE_AT_INITIALTIME
  "Persistent Trigger", // PERSISTENT_TRIGGER
  "Trigger Expression", // TRIGGER_EXPRESSION
  "Delay Expression", // DELAY_EXPRESSION
  "Priority Expression", // PRIORITY_EXPRESSION
  "Assignments", // ASSIGNMENTS
  "Vector Content" // VECTOR_CONTENT
  "Unit symbol", // UNIT_SYMBOL
  "Unit expression", // UNIT_EXPRESSION
  "Noise",  // NOISE
  "Propensity", // PROPENSITY
 });

