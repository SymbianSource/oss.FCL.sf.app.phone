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


//  INCLUDES

#include <e32base.h>
#include <PbkDataSaveAppUi.hrh>

#include "MPhCntPhonebookServices.h"
#include "CPhoneCntSaveAddToName.h"
#include "MPhoneCntPbkOwner.h"

// ============================ MEMBER FUNCTIONS ===============================

// ----------------------------------------------------------------------------
//  CPhoneCntSaveAddToName::NewL
// ----------------------------------------------------------------------------
//
CPhoneCntSaveAddToName* CPhoneCntSaveAddToName::NewL( 
       MPhCntPhonebookServices& aServices )
    {
    CPhoneCntSaveAddToName* self = 
        new(ELeave) CPhoneCntSaveAddToName( aServices );
    return self;
    }

// ----------------------------------------------------------------------------
// CPhoneCntSaveAddToName::~CPhoneCntSaveAddToName
// ----------------------------------------------------------------------------
//
EXPORT_C CPhoneCntSaveAddToName::~CPhoneCntSaveAddToName()
    {
    iServices.CancelRequest();
    }

// ----------------------------------------------------------------------------
// CPhoneCntSaveAddToName::HandlePbkOperationL
// ----------------------------------------------------------------------------
//
EXPORT_C void CPhoneCntSaveAddToName::HandlePbkOperationL( 
        const TCntPhoneNumber& aTelNumber,
        TInt aCommand )
    {
    TInt err = KErrNone;
    switch( aCommand ) 
        {
        case EPbkCmdDataSaveCreateNew:
            TRAP( err, iServices.CreateNewContactL( aTelNumber ) );
            break;
        
        case EPbkCmdDataSaveAddToExisting:
            TRAP( err,iServices.UpdateExistingContactL( aTelNumber ) ); 
            break;
        
        default:
            User::Leave( KErrArgument );
            break;
        }
    // Dont leave with KErrCancel, because it is valid functionality to
    // cancel service.
    if( err != KErrCancel ) 
        {
        User::LeaveIfError( err );
        }
    }
    
// ----------------------------------------------------------------------------
// CPhoneCntSaveAddToName::HandlePbkOperationL
// ----------------------------------------------------------------------------
//
EXPORT_C void CPhoneCntSaveAddToName::HandlePbkOperationL( 
        const TCntPhoneNumber& aTelNumber,
        TInt aCommand,
        TCntNumberType /*aNumberType*/ )
    {
    HandlePbkOperationL( aTelNumber, aCommand );
    }

// ----------------------------------------------------------------------------
// CPhoneCntSaveAddToName::CPhoneCntSaveAddToName
// ----------------------------------------------------------------------------
//
CPhoneCntSaveAddToName::CPhoneCntSaveAddToName( 
        MPhCntPhonebookServices& aServices  )
    : iServices( aServices )  
    {
    }

