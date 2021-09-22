#include "cpsapi/model/cpsapiReactionParameter.h"
#include "cpsapi/model/cpsapiTransaction.h"
#include "cpsapi/model/cpsapiModel.h"

#include <copasi/model/CModel.h>
#include <copasi/model/CReaction.h>

CPSAPI_NAMESPACE_USE

// static
const cpsapiReactionParameter::Properties cpsapiReactionParameter::SupportedProperties{
  cpsapiProperty::Type::OBJECT_NAME,
  cpsapiProperty::Type::PARAMETER_ROLE,
  cpsapiProperty::Type::PARAMETER_VALUE,
  cpsapiProperty::Type::PARAMETER_MAPPING};

cpsapiReactionParameter::cpsapiReactionParameter(wrapped * pWrapped)
  : base(pWrapped, Type::ReactionParameter)
{
  assertData(Data(*std::static_pointer_cast< base::Data >(mpData)));
}

cpsapiReactionParameter::cpsapiReactionParameter(const cpsapiReactionParameter & src)
  : base(src)
{}

// virtual
cpsapiReactionParameter::~cpsapiReactionParameter()
{}

// virtual
void cpsapiReactionParameter::accept(cpsapiVisitor & visitor)
{
  if (!operator bool()
      || static_cast< FakeData * >(getObject())->mpReaction == nullptr
      || static_cast< FakeData * >(getObject())->mName.empty())
    return;

  visitor.visit(this, Type::ReactionParameter);
  base::accept(visitor);
}

bool cpsapiReactionParameter::setProperty(const cpsapiReactionParameter::Property & property, const cpsapiVariant & value, const CCore::Framework & framework)
{
  return setProperty(static_cast< const cpsapiProperty::Type >(property), value, framework);
}

cpsapiVariant cpsapiReactionParameter::getProperty(const cpsapiReactionParameter::Property & property, const CCore::Framework & framework) const
{
  return getProperty(static_cast< const cpsapiProperty::Type >(property), framework);
}

// virtual
bool cpsapiReactionParameter::setProperty(const cpsapiProperty::Type & property, const cpsapiVariant & value, const CCore::Framework & framework)
{
  if (!operator bool()
      || static_cast< FakeData * >(getObject())->mpReaction == nullptr
      || static_cast< FakeData * >(getObject())->mName.empty())
    return false;

  if (!isValidProperty< cpsapiReactionParameter >(property))
    return base::setProperty(property, value, CCore::Framework::__SIZE);

  CCore::Framework Framework(framework);

  const std::string & Name = static_cast< FakeData * >(getObject())->mName;
  wrapped * pWrapped = static_cast< wrapped * >(getObject());
  CReaction * pChangedObject = pWrapped->mpReaction;

  const CFunctionParameter * pParameter;
  size_t Index = pChangedObject->getFunctionParameters().findParameterByName(Name, &pParameter);

  if (Index == C_INVALID_INDEX)
    return false;

  CDataObject * pObject = nullptr;
  if (pChangedObject->getParameterObjects()[Index].size() == 1)
    pObject = const_cast< CDataObject * >(pChangedObject->getParameterObjects()[Index][0]);

  bool success = cpsapiTransaction::endStructureChange(static_cast< CModel * >(pChangedObject->getObjectAncestor("Model")));

  switch (property)
    {
    case cpsapiProperty::Type::OBJECT_NAME:
      success = false; // The name of areaction parameter must not be changed
      break;

    case cpsapiProperty::Type::PARAMETER_ROLE:
      success = false; // The role of a reaction parameter must not be changed
      break;

    case cpsapiProperty::Type::PARAMETER_VALUE:
      if (dynamic_cast< CCopasiParameter * >(pObject))
        success &= static_cast< CCopasiParameter * >(pObject)->setValue(value.toDouble());
      break;

    case cpsapiProperty::Type::PARAMETER_MAPPING:
      switch (pParameter->getUsage())
        {
        case CFunctionParameter::Role::SUBSTRATE:
        case CFunctionParameter::Role::PRODUCT:
        case CFunctionParameter::Role::MODIFIER:
          if (value.toObject< cpsapiSpecies >())
            success &= pChangedObject->setParameterObject(Index, value.toObject< cpsapiSpecies >().getObject());
          else
            success = false;

          break;

        case CFunctionParameter::Role::PARAMETER:
          if (value.toObject< cpsapiGlobalQuantity >())
            success &= pChangedObject->setParameterObject(Index, value.toObject< cpsapiGlobalQuantity >().getObject());
          else
            success = false;
            
          break;

        case CFunctionParameter::Role::VOLUME:
          if (value.toObject< cpsapiCompartment >())
            success &= pChangedObject->setParameterObject(Index, value.toObject< cpsapiCompartment >().getObject());
          else
            success = false;

          break;

        case CFunctionParameter::Role::TIME:
          if (value.toObject< cpsapiModel >())
            success &= pChangedObject->setParameterObject(Index, value.toObject< cpsapiModel >().getObject());
          else
            success = false;

          break;

        default:
          success = false;
          break;
        }
      break;
    }

  return success && cpsapiTransaction::synchronize(pChangedObject, Framework);
}

// virtual
cpsapiVariant cpsapiReactionParameter::getProperty(const cpsapiProperty::Type & property, const CCore::Framework & framework) const
{
  if (!operator bool()
      || static_cast< FakeData * >(getObject())->mpReaction == nullptr
      || static_cast< FakeData * >(getObject())->mName.empty())
    return cpsapiVariant();

  if (!isValidProperty< cpsapiReactionParameter >(property))
    return base::getProperty(property, CCore::Framework::__SIZE);

  const std::string & Name = static_cast< FakeData * >(getObject())->mName;
  CReaction * pReaction = static_cast< FakeData * >(getObject())->mpReaction;

  const CFunctionParameter * pParameter;
  size_t Index = pReaction->getFunctionParameters().findParameterByName(Name, &pParameter);

  if (Index == C_INVALID_INDEX)
    return cpsapiVariant();

  CDataObject * pObject = nullptr;
  if (pReaction->getParameterObjects()[Index].size() == 1)
    pObject = const_cast< CDataObject * >(pReaction->getParameterObjects()[Index][0]);

  switch (property)
    {
    case cpsapiProperty::Type::OBJECT_NAME:
      return Name;
      break;

    case cpsapiProperty::Type::PARAMETER_ROLE:
      if (pParameter != nullptr)
        return CFunctionParameter::RoleNameXML[pParameter->getUsage()];
      break;

    case cpsapiProperty::Type::PARAMETER_VALUE:
      if (dynamic_cast< CCopasiParameter * >(pObject))
        return static_cast< CCopasiParameter * >(pObject)->getValue< C_FLOAT64 >();

      break;

    case cpsapiProperty::Type::PARAMETER_MAPPING:
      switch (pParameter->getUsage())
        {
        case CFunctionParameter::Role::SUBSTRATE:
        case CFunctionParameter::Role::PRODUCT:
        case CFunctionParameter::Role::MODIFIER:
          return cpsapiSpecies(dynamic_cast< CMetab * >(pObject));
          break;

        case CFunctionParameter::Role::PARAMETER:
          if (dynamic_cast< CModelValue * >(pObject) != nullptr)
            return cpsapiGlobalQuantity(static_cast< CModelValue * >(pObject));

          break;

        case CFunctionParameter::Role::VOLUME:
          return cpsapiCompartment(dynamic_cast< CCompartment * >(pObject));
          break;

        case CFunctionParameter::Role::TIME:
          return cpsapiModel(dynamic_cast< CModel * >(pObject));
          break;

        default:
          break;
        }
      break;
    }

  return cpsapiVariant();
}