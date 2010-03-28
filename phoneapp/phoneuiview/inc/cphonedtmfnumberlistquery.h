/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Definition of CPhoneDtmfNumberListQuery class.
*
*/


#ifndef CPHONEDTMFNUMBERLISTQUERY_H
#define CPHONEDTMFNUMBERLISTQUERY_H

#include    <AknQueryDialog.h>

    
// CLASS DECLARATION
class MPhoneQueryObserver; 
    
/**
*   DTMF number list query.
*
*   Shows DTMF list query. 
*   By default list is formatted with 'Find' and 'Enter manually' items.
*   In addition predefined DTMF of the contact can be seen in 
*   list if available.
*
*   Otherwise base class can handle it.
*
*   @since 5.0
*/
class CPhoneDtmfNumberListQuery : public CAknListQueryDialog
    {
    public:  // Constructors and destructor      
               
         /**
        * Creates new instance of CPhoneDtmfNumberListQuery
        * @param aCommandObserver callback observer
        * @param aQueryObserver  query observer
        * @param aSelectedDtmfNumber selected predefined dtmf number 
        * @return CPhoneDtmfNumberListQuery* object  
        */
        static CPhoneDtmfNumberListQuery* 
            NewL( MEikCommandObserver& aCommandObserver,
                  MPhoneQueryObserver& aQueryObserver,
                  HBufC*& aSelectedDtmfNumber );
        
        /**
        * Destructor.
        */
        virtual ~CPhoneDtmfNumberListQuery();
    
    public:
        
        /**
        * Appends given string to DTMF array 
        * @param aString -string to be added 
        */    
        void AppendStringToArrayL( TDesC& aString );
        
    protected: // Functions from base classes
            
        /**
        * From base class, 
        * check if it is ok to exit.
        */
        TBool OkToExitL( TInt aButtonId );
            
        /**
        * From CEikDialog, initialization after layout.
        */
        void PostLayoutDynInitL();
        

        TKeyResponse OfferKeyEventL( const TKeyEvent& aKeyEvent,
                                          TEventCode aType );
    
    private:
    
        /**
        * C++ constructor.
        * @param aCommandObserver callback observer
        * @param aQueryObserver  query observer
        * @param aSelectedDtmfNumber selected predefined dtmf number 
        */
        CPhoneDtmfNumberListQuery( MEikCommandObserver& aCommandObserver,
                                   MPhoneQueryObserver& aQueryObserver,
                                   HBufC*& aSelectedDtmfNumber  );
        
        
        /**
        * Symbian constructor
        */
        void ConstructL();

    private:    // Data

        MEikCommandObserver& iCommandObserver;
                
        MPhoneQueryObserver& iQueryObserver;
        
        HBufC*& iSelectedDtmfNumber;
   
        // Index of selected item.
        TInt iIndex;
        
        // Contains predefined DTMF(s), 'Find' and 'Enter manually' items
        // Owned.
        CDesCArrayFlat iArray;
        
        
    };

#endif      // CPHONEDTMFNUMBERLISTQUERY_H
            
// End of File
