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
