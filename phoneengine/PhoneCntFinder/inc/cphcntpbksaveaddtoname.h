/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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


#ifndef CPHCNTVPBKSAVEADDTONAME_H
#define CPHCNTVPBKSAVEADDTONAME_H

#include <e32base.h>

#include "CPhoneCntSaveAddToName.h"

/**
 *  Add number to existing name and save the number.
 *
 *  @lib PhoneCntFinder
 *  @since S60 v3.1
 */
NONSHARABLE_CLASS( CPhCntPbkSaveAddToName )
    : public CBase, 
      public MPhoneCntSaveAddToName
    {
public: // New functions       
    /**
    * Two-phased constructor.
    * @param aPbkOwner Phonebook owner.
    * @return new instance of the class.
    */
    static CPhCntPbkSaveAddToName* NewL( MPhoneCntPbkOwner& aPbkOwner );

    /**
    * Destructor.
    */
    ~CPhCntPbkSaveAddToName();

public : // Functions from base classes

    /**
    * From MPhoneCntSaveAddToName
    */
    void HandlePbkOperationL(
        const TCntPhoneNumber& aTelNumber,
        TInt aCommand );
    
    /**
    * From MPhoneCntSaveAddToName
    * @since 3.0
    */
    void HandlePbkOperationL(
        const TCntPhoneNumber& aTelNumber,
        TInt aCommand,
        TCntNumberType aNumberType );
        
private:
    
    /**
    * C++ constructor.
    */
    CPhCntPbkSaveAddToName( MPhoneCntPbkOwner& aPbkOwner );

    /**
    * Symbian OS constructor.
    */
    void ConstructL();
    
private://data
    // Phoneboo owner
    MPhoneCntPbkOwner& iPbkOwner;

    // Phonebook saver
    CPbkDataSaveAppUi* iPbkDataSaveAppUi;
    };


#endif // CPHCNTVPBKSAVEADDTONAME_H
