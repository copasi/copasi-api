// BEGIN: Copyright 
// Copyright (C) 2019 - 2020 by Pedro Mendes, Rector and Visitors of the 
// University of Virginia, University of Heidelberg, and University 
// of Connecticut School of Medicine. 
// All rights reserved. 
// END: Copyright 

// BEGIN: License 
// Licensed under the Apache License, Version 2.0 (the "License"); 
// you may not use this file except in compliance with the License. 
// You may obtain a copy of the License at 
//  
//   http://www.apache.org/licenses/LICENSE-2.0 
// END: License 

#ifndef CPSAPI_DataObject
#define CPSAPI_DataObject

// TODO Make proper reference for libCopasi
#include "copasi/core/CDataObject.h"


// API use by setData
// virtual bool applyData(const CData & data, CUndoData::CChangeSet & changes);
bool setData();


//CDataObject(const CDataObject & src,
//            const CDataContainer * pParent = NULL);

// API (for reporting and expressions)
//virtual CCommonName getCN() const;

// API implement ostream
//virtual void print(std::ostream * ostream) const;

//virtual std::string getObjectDisplayName() const;

// API
//virtual const CValidity & getValidity() const;

//bool setObjectName(const std::string & name);

//const std::string & getObjectName() const;

//virtual const std::string getUnits() const;




#endif // CPSAPI_DataObject
