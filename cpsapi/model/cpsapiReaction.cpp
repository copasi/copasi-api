#include "cpsapi/model/cpsapiReaction.h"
#include "cpsapi/model/cpsapiTransaction.h"

#include <copasi/model/CReaction.h>
#include <copasi/model/CReactionInterface.h>
#include <copasi/model/CModel.h>

CPSAPI_NAMESPACE_USE

// static
const cpsapiReaction::Properties cpsapiReaction::SupportedProperties =
  {
    cpsapiProperty::Type::CHEMICAL_EQUATION,
    cpsapiProperty::Type::KINETIC_LAW,
    cpsapiProperty::Type::KINETIC_LAW_UNIT_TYPE,
    cpsapiProperty::Type::KINETIC_LAW_VARIABLE_MAPPING,
    cpsapiProperty::Type::LOCAL_REACTION_PARAMETERS,
    cpsapiProperty::Type::SCALING_COMPARTMENT,
    cpsapiProperty::Type::ADD_NOISE,
    cpsapiProperty::Type::NOISE_EXPRESSION
  };

cpsapiReaction::cpsapiReaction(cpsapiReaction::wrapped * pWrapped)
  : base(pWrapped, Type::Reaction)
{
  assertData(Data(*std::static_pointer_cast< base::Data >(mpData)));
}

cpsapiReaction::cpsapiReaction(const cpsapiReaction & src)
  : base(src)
{}

// virtual
cpsapiReaction::~cpsapiReaction()
{}

// virtual 
void cpsapiReaction::accept(cpsapiVisitor & visitor)
{
  if (!operator bool())
    return;

  visitor.visit(this, Type::Reaction);
  base::accept(visitor);
}

cpsapiReactionParameter cpsapiReaction::parameter(const std::string & name)
{
  if (!operator bool())
    return nullptr;

  if (!name.empty() &&
      name != DATA(mpData)->mDefaultParameter.getProperty(cpsapiReactionParameter::Property::NAME).toString())
    {
      ParameterManager::iterator found = DATA(mpData)->mManager.insert(std::make_pair(name, new cpsapiReactionParameter::FakeData(static_cast< CReaction * >(getObject()), name))).first;
      DATA(mpData)->mDefaultParameter = cpsapiReactionParameter(found->second);
    }
    
  return DATA(mpData)->mDefaultParameter;
}

cpsapiVector< cpsapiReactionParameter > cpsapiReaction::parameters()
{
  DATA(mpData)->mpVector->clear();

  if (operator bool())
    {
      wrapped * pWrapped = static_cast< wrapped * >(getObject());

      for (const CFunctionParameter & Parameter : pWrapped->getFunctionParameters())
        {
          ParameterManager::iterator found = DATA(mpData)->mManager.insert(std::make_pair(Parameter.getObjectName(), new cpsapiReactionParameter::FakeData(static_cast< CReaction * >(getObject()), Parameter.getObjectName()))).first;
          DATA(mpData)->mpVector->add(found->second, false);
        }
    }

  return cpsapiVector< cpsapiReactionParameter >(DATA(mpData)->mpVector);
}

bool cpsapiReaction::setProperty(const cpsapiReaction::Property & property, const cpsapiVariant & value, const CCore::Framework & framework)
{
  return setProperty(static_cast< const cpsapiProperty::Type >(property), value, framework);
}

cpsapiVariant cpsapiReaction::getProperty(const cpsapiReaction::Property & property, const CCore::Framework & framework) const
{
  return getProperty(static_cast< const cpsapiProperty::Type >(property), framework);
}

// virtual
bool cpsapiReaction::setProperty(const cpsapiProperty::Type & property, const cpsapiVariant & value, const CCore::Framework & framework)
{
  if (!operator bool())
    return false;

  if (!isValidProperty< cpsapiReaction >(property))
    return base::setProperty(property, value, CCore::Framework::__SIZE);

  CCore::Framework Framework(framework);

  wrapped * pWrapped = static_cast< wrapped * >(getObject());
  CDataObject * pChangedObject = pWrapped;
  bool success = cpsapiTransaction::endStructureChange(static_cast< CModel * >(pWrapped->getObjectAncestor("Model")));

  switch (property)
    {
    case cpsapiProperty::Type::CHEMICAL_EQUATION:
      break;

    case cpsapiProperty::Type::KINETIC_LAW:
      break;

    case cpsapiProperty::Type::KINETIC_LAW_UNIT_TYPE:
      break;

    case cpsapiProperty::Type::KINETIC_LAW_VARIABLE_MAPPING:
      break;

    case cpsapiProperty::Type::LOCAL_REACTION_PARAMETERS:
      break;

    case cpsapiProperty::Type::SCALING_COMPARTMENT:
      break;

    case cpsapiProperty::Type::ADD_NOISE:
      break;

    case cpsapiProperty::Type::NOISE_EXPRESSION:
      break;

    default:
      break;
    }

  return success && cpsapiTransaction::synchronize(pChangedObject, Framework);
}

// virtual
cpsapiVariant cpsapiReaction::getProperty(const cpsapiProperty::Type & property, const CCore::Framework & framework) const
{
  if (!operator bool())
    return cpsapiVariant();

  if (!isValidProperty<cpsapiReaction>(property))
    return base::getProperty(property, CCore::Framework::__SIZE);

  wrapped * pWrapped = static_cast< wrapped * >(getObject());
  CReactionInterface Reaction;
  Reaction.init(*pWrapped);

  switch (property)
    {
    case cpsapiProperty::Type::CHEMICAL_EQUATION:
      return Reaction.getChemEqString();
      break;

    case cpsapiProperty::Type::KINETIC_LAW:
      return Reaction.getFunctionName();
      break;

    case cpsapiProperty::Type::KINETIC_LAW_UNIT_TYPE:
      return CReaction::KineticLawUnitTypeName[Reaction.getKineticLawUnitType()];
      break;

    case cpsapiProperty::Type::KINETIC_LAW_VARIABLE_MAPPING:
      break;

    case cpsapiProperty::Type::LOCAL_REACTION_PARAMETERS:
      break;

    case cpsapiProperty::Type::SCALING_COMPARTMENT:
      return Reaction.getScalingCompartment();
      break;

    case cpsapiProperty::Type::ADD_NOISE:
      return Reaction.hasNoise();
      break;

    case cpsapiProperty::Type::NOISE_EXPRESSION:
      return Reaction.getNoiseExpression();
      break;

    default:
      break;
    }

  return cpsapiVariant();
}
