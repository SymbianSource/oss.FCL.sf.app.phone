/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  Implementation of single item fetch interface.
*
*/


#ifndef CPHCNTSINGLEITEMFETCHIMPL_H
#define CPHCNTSINGLEITEMFETCHIMPL_H

// INCLUDES
#include    "CPhCntSingleItemFetch.h"

// FORWARD DECLARATIONS
class CPbkSingleItemFetch;
class CPbkSingleItemFetchDlg;
class CPbkFieldIdArray;
class MPhoneCntPbkOwner;
class CPhCntSingleCallFetchDlg;
class CPbkContactItem;
class TPbkContactItemField;

// CLASS DECLARATION

/**
*  It is implementation of single item fetch interface.
*
*  @lib PhoneCntFinder
*  @since 1.0
*/
NONSHARABLE_CLASS( CPhCntSingleItemFetchImpl )
    : public CPhCntSingleItemFetch
    {
    public:  // Constructors and destructor      
                
        /**
        * Two-phased constructor.
        *
        * @param aPbkOwner phonebook owner interface.
        * @return new instance of the class.
        */
        static CPhCntSingleItemFetchImpl* NewL(
            MPhoneCntPbkOwner& aPbkOwner );
        
        /**
        * Destructor.
        */
        ~CPhCntSingleItemFetchImpl();

    public: // Functions from base classes
        
        /**
        * From CPhCntSingleItemFetch
        */
        TInt FetchLD( TFetchParams& aParams );
        
    private:

        /**
        * C++ constructor.
        */
        CPhCntSingleItemFetchImpl(
            MPhoneCntPbkOwner& aPbkOwner );

        /**
        * Symbian OS constructor.
        */
        void ConstructL();

        /**
        * Fetch DTMF via single item fetch.
        *
        * @param aParams it contains parameters.
        */
        TInt FetchDtmfLD( TFetchParams& aParams );

        /**
        * Fetch new phonenumber field via single item fetch.
        *
        * @param aParams it contains parameters.
        */
        TInt FetchNewCallLD( TFetchParams& aParams );

        /**
        * Generates array of field id for DTMF fetch.
        */
        CPbkFieldIdArray* GenerateFieldIdArrayLC();

        /**
        * Pushes cleanup item to stack that resets some pointers.
        */
        void CleanupResetPtrPushL();

        /**
        * Resets iFetch & iDestroyed pointers.
        */
        static void ResetPointers( TAny* aAny );

        /**
        * Extracts result.
        *
        * @param aParams result will be stored there.
        * @param aItem contact item.
        * @param aField selected field.
        * @return error code.
        */
        static TInt ExtractResult( 
            TFetchParams& aParams,
            const CPbkContactItem* aItem,
            const TPbkContactItemField* aField );

    private:    // Data

        // Ref to pbk owner.
        MPhoneCntPbkOwner& iPbkOwner;
        
        // Pointer to boolean flag, will be updated to ETrue upon destruction.
        TBool* iDestroyed;

        // Owned: single item fetch
        CPbkSingleItemFetchDlg* iFetch;        

        // Owned: single entry fetch
        CPhCntSingleCallFetchDlg* iEntryFetch;

        // Type of fetch
        CPhCntSingleItemFetch::TFetchType iType;

    };

#endif      // CPHCNTSINGLEITEMFETCHIMPL_H
            
// End of File
