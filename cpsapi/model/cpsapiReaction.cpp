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
    cpsapiProperty::Type::KINETIC_LAW_EXPRESSION,
    cpsapiProperty::Type::KINETIC_LAW_UNIT_TYPE,
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

cpsapiReactionParameter::FakeData * cpsapiReaction::assertParameter(const std::string & name)
{
  ParameterManager::iterator found = DATA->mManager.find(name);

  if (found == DATA->mManager.end())
    found = DATA->mManager.insert(std::make_pair(name, new cpsapiReactionParameter::FakeData(static_cast< CReaction * >(getObject()), name))).first;

  return found->second;
}

cpsapiReactionParameter cpsapiReaction::parameter(const std::string & name)
{
  if (!operator bool())
    return nullptr;

  if (!name.empty() &&
      name != DATA->mDefaultParameter.getProperty(cpsapiReactionParameter::Property::NAME).toString())
    DATA->mDefaultParameter = cpsapiReactionParameter(assertParameter(name));
    
  return DATA->mDefaultParameter;
}

cpsapiVector< cpsapiReactionParameter > cpsapiReaction::parameters()
{
  if (DATA->mpVector == nullptr)
    DATA->mpVector = new ParameterVector();
  else
    DATA->mpVector->clear();

  if (operator bool())
    {
      wrapped * pWrapped = WRAPPED;

      for (const CFunctionParameter & Parameter : pWrapped->getFunctionParameters())
        DATA->mpVector->add(assertParameter(Parameter.getObjectName()), false);
    }

  return cpsapiVector< cpsapiReactionParameter >(DATA->mpVector);
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

  wrapped * pWrapped = WRAPPED;
  CDataObject * pChangedObject = pWrapped;
  bool success = cpsapiTransaction::endStructureChange(static_cast< CModel * >(pWrapped->getObjectAncestor("Model")));

  CReactionInterface Reaction;
  Reaction.init(*pWrapped);
  bool isValid = Reaction.isValid();

  switch (property)
    {
    case cpsapiProperty::Type::CHEMICAL_EQUATION:
      Reaction.setChemEqString(value.toString(), Reaction.getFunctionName());
      success &= Reaction.createMetabolites();
      break;

    case cpsapiProperty::Type::KINETIC_LAW:
      Reaction.setFunctionAndDoMapping(value.toString());
      break;

    case cpsapiProperty::Type::KINETIC_LAW_EXPRESSION:
      {
        CFunction * pFunction = pWrapped->createFunctionFromExpression(value.toString());

        if (pFunction != nullptr)
          Reaction.setFunctionAndDoMapping(pFunction->getObjectName());
        else
          success = false;
      }
      break;

    case cpsapiProperty::Type::KINETIC_LAW_UNIT_TYPE:
      Reaction.setKineticLawUnitType(CReaction::KineticLawUnitTypeName.toEnum(value.toString()));
      break;

    case cpsapiProperty::Type::SCALING_COMPARTMENT:
      Reaction.setScalingCompartment(value.toString());
      break;

    case cpsapiProperty::Type::ADD_NOISE:
      Reaction.setHasNoise(value.toBool());
      break;

    case cpsapiProperty::Type::NOISE_EXPRESSION:
      Reaction.setNoiseExpression(value.toString());
      break;

    default:
      break;
    }

  isValid = (!isValid || Reaction.isValid());
  Reaction.writeBackToReaction();

  if (DATA->mpVector != nullptr)
    parameters();
    
  return success && isValid && cpsapiTransaction::synchronize(pChangedObject, Framework);
}

// virtual
cpsapiVariant cpsapiReaction::getProperty(const cpsapiProperty::Type & property, const CCore::Framework & framework) const
{
  if (!operator bool())
    return cpsapiVariant();

  if (!isValidProperty<cpsapiReaction>(property))
    return base::getProperty(property, CCore::Framework::__SIZE);

  wrapped * pWrapped = WRAPPED;
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

    case cpsapiProperty::Type::KINETIC_LAW_EXPRESSION:
      return Reaction.getFunction().getInfix();
      break;

    case cpsapiProperty::Type::KINETIC_LAW_UNIT_TYPE:
      return CReaction::KineticLawUnitTypeName[Reaction.getKineticLawUnitType()];
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