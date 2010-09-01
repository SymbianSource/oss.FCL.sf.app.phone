/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Controller for DTMF mode of Dialer
*
*/

#include <StringLoader.h>
#include <phoneui.rsg>
#include <phoneui.mbg>
#include <AknsUtils.h>
#include <data_caging_path_literals.hrh>
#include "phoneappcommands.hrh"
#include "cphonedtmfdialercontroller.h"
#include "cphonemainresourceresolver.h"
#include "phonerssbase.h"

_LIT ( KPhoneMifFileName, "phoneui.mif" );

const CPhoneDialerController::TLocalButtonData KSendDtmfButtonData =
    {
    EPhoneCmdDtmfOk,
    EAknsMinorGenericQgnIndiButtonSendDtmf,
    EMbmPhoneuiQgn_indi_button_send_dtmf,
    EMbmPhoneuiQgn_indi_button_send_dtmf_mask,
    R_PHONEUI_DIALER_TOOLTIP_SEND
    };

// ---------------------------------------------------------------------------
// CPhoneDtmfDialerController
// ---------------------------------------------------------------------------
//
CPhoneDtmfDialerController::CPhoneDtmfDialerController( 
        CPhoneBubbleWrapper* aBubbleWrapper,
        CCoeEnv& aCoeEnv ) : 
    CPhoneDialerController( aBubbleWrapper, aCoeEnv )
    {
    // No implementation required
    }

// ---------------------------------------------------------------------------
// ~CPhoneDtmfDialerController
// ---------------------------------------------------------------------------
//
CPhoneDtmfDialerController::~CPhoneDtmfDialerController()
    {
    delete iPromptText;
    }

// ---------------------------------------------------------------------------
// NewLC
// ---------------------------------------------------------------------------
//
CPhoneDtmfDialerController* CPhoneDtmfDialerController::NewLC( 
        CPhoneBubbleWrapper* aBubbleWrapper,
        CCoeEnv& aCoeEnv )
    {
    CPhoneDtmfDialerController* self =
        new (ELeave) CPhoneDtmfDialerController( aBubbleWrapper, aCoeEnv );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// NewL
// ---------------------------------------------------------------------------
//
CPhoneDtmfDialerController* CPhoneDtmfDialerController::NewL( 
        CPhoneBubbleWrapper* aBubbleWrapper,
        CCoeEnv& aCoeEnv )
    {
    CPhoneDtmfDialerController* self = 
        CPhoneDtmfDialerController::NewLC( aBubbleWrapper, aCoeEnv );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// ConstructL
// ---------------------------------------------------------------------------
//
void CPhoneDtmfDialerController::ConstructL()
    {
    CPhoneDialerController::ConstructL();
    }

// ---------------------------------------------------------------------------
// NumberEntryPromptTextL
// ---------------------------------------------------------------------------
//
const TDesC& CPhoneDtmfDialerController::NumberEntryPromptTextL()
    {
    if ( !iPromptText )
        {
        // Load prompt text when it's needed for the first time.
        // It can't be loaded in constructor as resource resolver
        // is not yet properly set up at that time.
        TInt resourceId = 
            CPhoneMainResourceResolver::Instance()->
                   ResolveResourceID( EPhoneDtmfDialerNumberEntryPromptText ); 
        iPromptText = StringLoader::LoadL( resourceId, &iCoeEnv );
        }
    if ( iPromptText )
        {
        return *iPromptText;
        }
    return KNullDesC;
    }

// ---------------------------------------------------------------------------
// GetButtonDataL
// ---------------------------------------------------------------------------
//
TInt CPhoneDtmfDialerController::GetButtonData( TButtonIndex aIndex, RPointerArray<CButtonData>& aData ) const
    {
    TInt err = KErrNone;
    
    if ( aIndex == ECallButton )
        {
        TFileName mifPath( KDriveZ );
        mifPath.Append( KDC_APP_BITMAP_DIR );
        mifPath.Append( KPhoneMifFileName );
        MAknsSkinInstance* skin = AknsUtils::SkinInstance();
        CButtonData* btnData = NULL;
        TRAP( err, btnData = CreateButtonDataL( KSendDtmfButtonData, skin, mifPath ) );
        if ( !err )
            {
            err = aData.Append( btnData );
            if ( err )
                {
                // append failed, delete created data to avoid memory leak
                delete btnData;
                }
            }
        }
    else if ( aIndex == EClearButton )
        {
        // let base class handle backspace
        err = CPhoneDialerController::GetButtonData( aIndex, aData );
        }
    else
        {
        // leave Phonebook button empty
        }
    return err;
    }

// ---------------------------------------------------------------------------
// ButtonState
// ---------------------------------------------------------------------------
//
TInt CPhoneDtmfDialerController::ButtonState( TButtonIndex aIndex ) const
    {
    TInt state = KErrNotFound;
    // Call button has one only one state, phonebook button has no states.
    // Clear button is handled by the base class.
    if ( aIndex == ECallButton )
        {
        state = 0;
        }
    else if ( aIndex == EClearButton )
        {
        state = CPhoneDialerController::ButtonState( aIndex );
        }
    return state;
    }

// ---------------------------------------------------------------------------
// GetButtonDimmed
// ---------------------------------------------------------------------------
//
TBool CPhoneDtmfDialerController::ButtonDimmed( TButtonIndex aIndex ) const
    {
    TBool dimmed = ETrue;
    // Call button is dimmed while number entry is empty. Phonebook button
    // is empty and always dimmed. Clear button is handled by the base class.
    if ( aIndex == ECallButton )
        {
        dimmed = !iNumberAvailable;
        }
    else if ( aIndex == EClearButton )
        {
        dimmed = CPhoneDialerController::ButtonDimmed( EClearButton );
        }
    return dimmed;
    }

// ---------------------------------------------------------------------------
// EasyDialingAllowed
// ---------------------------------------------------------------------------
//
TBool CPhoneDtmfDialerController::EasyDialingAllowed() const
    {
    return EFalse;
    }

// end of file
