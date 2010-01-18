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
* Description:  Add number to existing name and save the number.
*
*/


#ifndef CPHONECNTSAVEADDTONAME_H
#define CPHONECNTSAVEADDTONAME_H

// INCLUDES
#include <e32base.h>

// DATA TYPES
enum TCntNumberType
    {
    /**
    * If saved type is normal number.
    */
    ECntNormalNumber = 0,
    /**
    * If saved type is normal number it could be used in SIP Uri.
    */
    ECntVoiceInternetNumber,
    /**
    * If saved type is internet number.
    */
    ECntOnlyInternetNumber,
    /**
    * If saved type is internet number and could be used like
    * email address.
    */
    ECntInternetEmailField    
    };    

// KCntPhoneNumberMaxLength must be same as MPhEngCallHandling::TPhEngPhoneNumber  
const TInt KCntPhoneNumberMaxLength = 100;     
typedef TBuf<KCntPhoneNumberMaxLength> TCntPhoneNumber;

// FORWARD DECLARATIONS
class CPbkDataSaveAppUi;
class CPbkViewResourceLoader;
class MPhoneCntPbkOwner;
class MPhCntPhonebookServices;

// CLASS DECLARATION

/**
*  MPhoneCntSaveAddToName
*
*  Abstact class for saving new entry and adding data to old phonebook entry.
*
*  @lib PhoneCntFinder
*  @since 1.0
*/
NONSHARABLE_CLASS( MPhoneCntSaveAddToName )
    {
    public: // New functions   
    
        virtual ~MPhoneCntSaveAddToName()  {};
        
        /**
        * Handles phonebook operation
        * @param aTelNumber Telephone number to save
        * @param aCommand Given command (new, update existing)
        */
        virtual void HandlePbkOperationL( 
            const TCntPhoneNumber& aTelNumber,                                            
            TInt aCommand ) = 0;
            
        /**
        * Handles phonebook operation.
        * @since Series60 3.0
        * @param aTelNumber Telephone number to save
        * @param aCommand Given command (new, update existing)
        * @param aNumberType Type of saved telnumber (E.g If not numeric
        *                    could not save to the Phone number field)
        */       
        virtual void HandlePbkOperationL(
            const TCntPhoneNumber& aTelNumber,
            TInt aCommand,
            TCntNumberType aNumberType ) = 0;    
       };



/**
*  This class is used to add number to existing name and to save the number.
*
*  @lib PhoneCntFinder
*  @since 1.0
*/
class CPhoneCntSaveAddToName
    : public CBase, 
      public MPhoneCntSaveAddToName
    {
    public: // New functions       
        /**
        * Two-phased constructor.
        * @param aPbkOwner Phonebook owner.
        * @return new instance of the class.
        */
        static CPhoneCntSaveAddToName* NewL( MPhCntPhonebookServices& aServices );

        /**
        * Destructor.
        */
        IMPORT_C ~CPhoneCntSaveAddToName();

    public : // Functions from base classes

        /**
        * From MPhoneCntSaveAddToName
        */
        IMPORT_C void HandlePbkOperationL(
            const TCntPhoneNumber& aTelNumber,
            TInt aCommand );
        
        /**
        * From MPhoneCntSaveAddToName
        * @since 3.0
        */
        IMPORT_C void HandlePbkOperationL(
            const TCntPhoneNumber& aTelNumber,
            TInt aCommand,
            TCntNumberType aNumberType );    
    private:
        
        /**
        * C++ constructor.
        */
        CPhoneCntSaveAddToName( MPhCntPhonebookServices& aServices  );
        
    private://data
    
        /**
         * Service for creating and updating contacts.
         * Not own.
         */
        MPhCntPhonebookServices& iServices;

    };

#endif      // CPHONECNTSAVEADDTONAME_H
            
// End of File
