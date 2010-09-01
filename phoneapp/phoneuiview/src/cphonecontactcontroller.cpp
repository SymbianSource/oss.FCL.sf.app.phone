/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Contact Controller
*
*/



// INCLUDE FILES
#include    "cphonecontactcontroller.h"
#include    <cphonecntsaveaddtoname.h>
#include    <cphcntsingleitemfetch.h>
#include    <cphonecntfactory.h>
#include    <eikenv.h>
#include    <aknappui.h>
#include	<cphcntrfshandler.h>
#include    "phonelogger.h"


// CONSTANTS

// Library containing contact matcher.
_LIT( KPhoneCntFinderLibrary, "phonecntfinder.dll" );

// Ordinal position of function to create instance of CPhoneCntFactory.
const TInt KPhoneCntFinderOrdinal = 1;

// ================= MEMBER FUNCTIONS =======================

CPhoneContactController* CPhoneContactController::NewL()
    {
    CPhoneContactController* self = 
        new ( ELeave ) CPhoneContactController();

    return self;
    }
        
CPhoneContactController::~CPhoneContactController()
    {
    delete iFactory;
    iLibrary.Close();
    }

// -----------------------------------------------------------------------------
// CPhoneContactController::ContinueConstructL
// -----------------------------------------------------------------------------
//
TInt CPhoneContactController::ContinueConstructL( TInt aSteps )
    {
    __PHONELOG( EBasic, EPhoneUIView,
                "CPhoneViewController::ContinueConstructL()" );
    if ( aSteps == KConstructAll )
        {
        aSteps = ConstructionSteps();
        }

    // Perform as many steps as required..
    for ( ; aSteps > 0; aSteps-- )
        {
        DoStepL();

        // If we do not need any steps, stop.
        if ( !ConstructionSteps() )
            {
            break;
            }
        }

    return ConstructionSteps();
    }

// -----------------------------------------------------------------------------
// CPhoneContactController::ConstructionSteps
// -----------------------------------------------------------------------------
//
TInt CPhoneContactController::ConstructionSteps() const
    {
    return TInt( EStepDone - iCurrentStep );
    }

// ---------------------------------------------------------------------------
// CPhoneContactController::CreateSingleItemFetchL
// ---------------------------------------------------------------------------
//
CPhCntSingleItemFetch* CPhoneContactController::CreateSingleItemFetchL()
    {
    ConstructCreateFactoryL();

    return iFactory->CreateSingleItemFetchL();
    }

// -----------------------------------------------------------------------------
// CPhoneContactController::CreateSaveAddToNameL
// -----------------------------------------------------------------------------
//
CPhoneCntSaveAddToName* CPhoneContactController::CreateSaveAddToNameL()
    {
    ConstructCreateFactoryL();

    return iFactory->CreateSaveAddToNameL();
    }


CPhoneContactController::CPhoneContactController()
    {
    }

// -----------------------------------------------------------------------------
// CPhoneContactController::DoStepL
// -----------------------------------------------------------------------------
//
void CPhoneContactController::DoStepL()
    {
    // If all steps have been completed, then there is 
    // nothing to do.
    if ( iCurrentStep == EStepDone )
        {
        return;
        }

    // Perform one step; if it doesn't leave, then we have done it.
    switch ( iCurrentStep )
        {
        case EStepLoadLibrary:
            ConstructLoadLibraryL();
            break;

        case EStepCreateFactory:
            ConstructCreateFactoryL();
            break;

        case EStepDone:
        default:
            return;
        }

    // Next step
    iCurrentStep = NextStep( iCurrentStep );
    }

// -----------------------------------------------------------------------------
// CPhoneContactController::NextStep
// -----------------------------------------------------------------------------
//
CPhoneContactController::TStep 
    CPhoneContactController::NextStep( TStep aStep )
    {
    TStep result = EStepDone;

    switch ( aStep )
        {
        case EStepLoadLibrary:
            result = EStepCreateFactory;
            break;

        case EStepCreateFactory:
            result = EStepDone;
            break;

        case EStepDone:
            result = EStepDone;
            break;

        default:
            break;
        }

    return result;
    }

// -----------------------------------------------------------------------------
// CPhoneContactController::ConstructLoadLibraryL
// -----------------------------------------------------------------------------
//
void CPhoneContactController::ConstructLoadLibraryL()
    {
    if ( !iLibrary.Handle() )
        {
        User::LeaveIfError( iLibrary.Load( KPhoneCntFinderLibrary ) );
        }
    }

// -----------------------------------------------------------------------------
// CPhoneContactController::ConstructCreateFactoryL
// -----------------------------------------------------------------------------
//
void CPhoneContactController::ConstructCreateFactoryL()
    {
    __PHONELOG( EBasic, EPhoneUIView,
                    "CPhoneContactController::ConstructCreateFactoryL()" );
    if ( !iFactory )
        {
        ConstructLoadLibraryL();

        TInt res = iLibrary.Lookup( KPhoneCntFinderOrdinal )();
        iFactory = (CPhCntFactory*)res;
        }
    }

// -----------------------------------------------------------------------------
// CPhoneContactController::CreatePhoneBookServiceL
// -----------------------------------------------------------------------------
//
void CPhoneContactController::CreatePhoneBookServiceL()
    {
    __PHONELOG( EBasic, EPhoneUIView,
                    "CPhoneContactController::CreatePhoneBookServiceL()" );
    if ( iFactory )
        {
        iFactory->CreatePhonebookServicesL();
        }
    }

// -----------------------------------------------------------------------------
// CPhoneContactController::CreateRfsHandlerL
// -----------------------------------------------------------------------------
//
CPhCntRfsHandler* CPhoneContactController::CreateRfsHandlerL()
    {
    ConstructCreateFactoryL();

    return iFactory->CreateRfsHandlerL();
    }

// -----------------------------------------------------------------------------
// CPhoneContactController::CreateSpeedDialMonitorL
// -----------------------------------------------------------------------------
//
CPhCntSpeedDialMonitor* CPhoneContactController::CreateSpeedDialMonitorL()
    {
    ConstructCreateFactoryL();

    return iFactory->CreateSpeedDialMonitorL();
    }    
//  End of File  
