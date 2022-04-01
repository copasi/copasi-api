// BEGIN: Copyright 
// Copyright (C) 2022 by Pedro Mendes, Rector and Visitors of the 
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

#include "cpsapi/core/cpsapiContainer.h"
#include "cpsapi/core/cpsapiVector.h"


#pragma GCC diagnostic push
#include <copasi/model/CReaction.h>
#pragma GCC diagnostic pop

class CReaction;

CPSAPI_NAMESPACE_BEGIN

/**
 * @brief 
 * 
 */
class cpsapiKineticLawVariable : public cpsapiObject
{
public:
  /**
   * A fake CDataObject representing the the variables of the kinetic law  
   */
  class KineticLawVariable : public CDataObject
  {
    /**
     * Only the class cpsapiReaction is allowed to create KineticLawVariable 
     */
    friend class cpsapiReaction;

  public:
    /**
     * Required for objects to be insterted into CDataVector 
     * 
     * @param const CData & data 
     * @param CUndoObjectInterface * pParent 
     * @return KineticLawVariable * pData 
     */
    static KineticLawVariable * fromData(const CData & data, CUndoObjectInterface * pParent);

  private:
    /**
     * Default constructor (not implemented)
     */
    KineticLawVariable();

    /**
     * Construct a new Fake Data object
     * 
     * @param CReaction * pReaction 
     * @param const std::string & name 
     */
    KineticLawVariable(CReaction * pReaction, const std::string & name = "");

  public:
    /**
     * Copy constructor
     * 
     * @param const KineticLawVariable & src, 
     * @param CDataContainer * pParent (default: nullptr)
     */
    KineticLawVariable(const KineticLawVariable & src, CDataContainer * pParent = nullptr);

    /**
     * Destructor
     */
    virtual ~KineticLawVariable(); 

    /**
     * @brief 
     * 
     */
    void updateMappedObject();

    cpsapiObject * mpMappedObject;
  };

  /**
   * Enumeration of the exposed properties
   */ 
  enum class Property
  {
    NAME = cpsapiProperty::Type::NAME,
    OBJECT_UNIQUE_NAME = cpsapiProperty::Type::OBJECT_UNIQUE_NAME,
    CN = cpsapiProperty::Type::CN,
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
   * Enumeration of the exposed references
   */
  enum class Reference
  {
    NAME = cpsapiReference::Type::NAME,
    OBJECT_UNIQUE_NAME = cpsapiReference::Type::OBJECT_UNIQUE_NAME,
    VALUE = cpsapiReference::Type::PARAMETER_VALUE
  };

  /**
   * Static set of supported references
   */
  static const References SupportedReferences;

  /**
   * Static set of hidden references
   */
  static const References HiddenReferences;

  /**
   * The class
   */
  typedef cpsapiKineticLawVariable self;

  /**
   * The base class
   */
  typedef cpsapiObject base;

  /**
   * The wrapped COPASI class
   */
  typedef KineticLawVariable wrapped;

  /**
   * Specific constructor
   * @param wrapped * pWrapped
   */
  cpsapiKineticLawVariable(wrapped * pWrapped = nullptr);

  /**
   * Destructor
   */
  virtual ~cpsapiKineticLawVariable();

  /**
   * Check whether the object is valid 
   * @return bool valid
   */
  virtual bool isValid() const override;

  /**
   * Accept the given visitor
   * 
   * @tparam Visitor 
   * @param Visitor & visitor 
   */
  template < typename Visitor > void accept(Visitor & visitor);

  /**
   * Set a property of the object to the provided value under the given framework.
   * The value must match the underlying value of the property.
   * The default framework is unspecified
   * @param const Property & property
   * @param const cpsapiData & value
   * @param const CCore::Framework & framework (default: CCore::Framework::__SIZE)
   * @return bool success
   */
  bool setProperty(const Property & property, const cpsapiData & value, const CCore::Framework & framework = CCore::Framework::__SIZE);

  /**
   * Set a property of the object to the provided value under the given framework.
   * The value must match the underlying value of the property.
   * The default framework is unspecified
   * @param const Property & property
   * @param const CCore::Framework & framework (default: CCore::Framework::__SIZE)
   * @return cpsapiData property
   */
  cpsapiData getProperty(const Property & property, const CCore::Framework & framework = CCore::Framework::__SIZE) const;

  /**
   * Retrieve a property of the object to the provided value under the given framework.
   * The default framework is unspecified
   * @param const Reference & reference
   * @param const CCore::Framework & framework (default: CCore::Framework::__SIZE)
   * @return CCommonName
   */
CCommonName getDataCN(const Reference & reference, const CCore::Framework & framework = CCore::Framework::__SIZE) const;

protected:
  /**
   * Set the property
   * 
   * @param const cpsapiProperty::Type & property 
   * @param const cpsapiData & value 
   * @param const CCore::Framework &framework 
   * @return bool success
   */
  virtual bool setProperty(const cpsapiProperty::Type & property, const cpsapiData & value, const CCore::Framework & framework) override;

  /**
   * Retreive the property
   * 
   * @param const cpsapiProperty::Type & property 
   * @param const CCore::Framework &framework 
   * @return cspapiData
   */
  virtual cpsapiData getProperty(const cpsapiProperty::Type & property, const CCore::Framework & framework) const override;

  /**
   * Retreive the data reference
   * 
   * @param const cpsapiReference::Type & reference 
   * @param const CCore::Framework &framework 
   * @return CCommonName
   */
  virtual CCommonName getDataCN(const cpsapiReference::Type & reference, const CCore::Framework & framework) const override;
};

template< class Visitor >
void cpsapiKineticLawVariable::accept(Visitor & visitor)
{
  if (isValid())
    {
      cpsapiVisitor::acceptIfVisitable(visitor, this); 
      base::accept(visitor);
    }
}

template <>
inline size_t cpsapiVector< cpsapiKineticLawVariable >::index(const std::string & name) const
{
  if (operator bool())
    {
      wrapped * pWrapped = WRAPPED;
      wrapped::const_iterator itParameter = pWrapped->begin();
      wrapped::const_iterator endParameter = pWrapped->end();
      
      for (size_t Index = 0; itParameter != endParameter; ++itParameter, ++Index)
        {
          if (itParameter->getObjectName() == name)
            return Index;
        }
    }

  return C_INVALID_INDEX;
}

CPSAPI_NAMESPACE_END


