/*
* Copyright (c) 2008, 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Implementation of CPhoneTransferDialerController class.
*
*/


// INCLUDES
#include <e32base.h>

#include <AknsItemID.h>
#include <AknsUtils.h>
#include <StringLoader.h>

#include <phoneui.mbg>
#include <phoneui.rsg>
#include <phoneuivoip.rsg>
#include <data_caging_path_literals.hrh> // For KDC_APP_BITMAP_DIR

#include "phoneui.pan"
#include "phonelogger.h"
#include "cphonetransferdialercontroller.h"
#include "cphonemainresourceresolver.h"
#include "phonerssvoip.h"
#include "phoneappcommands.hrh"
#include "phoneappvoipcommands.hrh"


// CONSTANTS
_LIT ( KPhoneMifFileName, "phoneui.mif" );

// Number of buttons 
const TInt KButtonCount = 2;

class TPhoneDialerToolbarButton 
    {
public:
    TInt iIconIndex;
    TInt iMaskIndex;
    TInt iCommandId;    
    };

const TPhoneDialerToolbarButton bArray[KButtonCount] = 
        {  
            { EMbmPhoneuiQgn_indi_button_send_dtmf,
              EMbmPhoneuiQgn_indi_button_send_dtmf_mask,
              EPhoneCmdTransferDialerOk
            },
            { EMbmPhoneuiQgn_indi_dialer_contacts,
              EMbmPhoneuiQgn_indi_dialer_contacts_mask,
              EPhoneCmdTransferDialerSearch
            }                 
        };  

// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//  
CPhoneTransferDialerController::CPhoneTransferDialerController()
    : CPhoneDialerController( NULL, *CCoeEnv::Static() )
    {
    }

// -----------------------------------------------------------
// CPhoneTransferDialerController::~CPhoneTransferDialerController()
// Destructor
// -----------------------------------------------------------
//
CPhoneTransferDialerController::~CPhoneTransferDialerController()
    {
    __LOGMETHODSTARTEND( PhoneUIVoIPExtension, 
        "CPhoneTransferDialerController::~CPhoneTransferDialerController()");
    delete iNumberEntryPromptText;
    }

// -----------------------------------------------------------
// CPhoneTransferDialerController::NewL()
// Constructor
// -----------------------------------------------------------
//
CPhoneTransferDialerController* CPhoneTransferDialerController::NewL()
    {
    CPhoneTransferDialerController* self = new (ELeave) 
        CPhoneTransferDialerController();
  
    return self;
    }

// -----------------------------------------------------------
// CPhoneTransferDialerController::CbaResourceId
// -----------------------------------------------------------
//
TInt CPhoneTransferDialerController::CbaResourceId() const
    {
    return EPhoneVoIPTransferDialerSoftkeys;
    }

// -----------------------------------------------------------
// CPhoneTransferDialerController::MenuResourceId
// -----------------------------------------------------------
//
TInt CPhoneTransferDialerController::MenuResourceId() const
    {
    if ( !iNumberAvailable )
        {
        return EPhoneVoIPTransferDialerNumberEntryEmptyMenubar;
        }
    else
        {
        return EPhoneVoIPTransferDialerNumberEntryNotEmptyMenubar;
        }
    }

// -----------------------------------------------------------
// CPhoneTransferDialerController::NumberEntryPromptTextL
// -----------------------------------------------------------
//
const TDesC& CPhoneTransferDialerController::NumberEntryPromptTextL()
    {
    __LOGMETHODSTARTEND( PhoneUIVoIPExtension, 
        "CPhoneTransferDialerController::NumberEntryPromptTextL()");
    if ( !iNumberEntryPromptText )
        {
        iNumberEntryPromptText = StringLoader::LoadL( 
            CPhoneMainResourceResolver::Instance()->
            ResolveResourceID( EPhoneVoIPTransferAddress ), &iCoeEnv );
        }  
    
    return *iNumberEntryPromptText;
    }

// ---------------------------------------------------------------------------
// GetButtonData
// ---------------------------------------------------------------------------
//
TInt CPhoneTransferDialerController::GetButtonData( TButtonIndex aIndex, RPointerArray<CButtonData>& aData ) const
    {
    TInt err = KErrNone;
    
    TInt arrayIdx = KErrNotFound;
    if ( aIndex == ECallButton )
        {
        arrayIdx = 0;
        }
    else if ( aIndex == EPhonebookButton )
        {
        arrayIdx = 1;
        }
    
    if ( arrayIdx >= 0 )
        {
        const TPhoneDialerToolbarButton& arrItem = bArray[ arrayIdx ];
        CButtonData* btn = NULL;
        TRAP( err, btn = CreateButtonDataL( arrItem.iCommandId, 
                                            arrItem.iIconIndex, 
                                            arrItem.iMaskIndex ) );
        if ( !err )
            {
            err = aData.Append( btn );
            if ( err )
                {
                delete btn;
                }
            }
        }
    else
        {
        // Trust base class on other buttons (i.e. the clear button)
        err = CPhoneDialerController::GetButtonData( aIndex, aData );
        }
    
    return err;
    }

// ---------------------------------------------------------------------------
// ButtonState
// ---------------------------------------------------------------------------
//
TInt CPhoneTransferDialerController::ButtonState( TButtonIndex aIndex ) const
    {
    // Out button have only one state. Trust base class on clear button
    if ( aIndex == ECallButton || aIndex == EPhonebookButton )
        {
        return 0;
        }
    else
        {
        return CPhoneDialerController::ButtonState( aIndex );
        }
    }

// ---------------------------------------------------------------------------
// ButtonDimmed
// ---------------------------------------------------------------------------
//
TBool CPhoneTransferDialerController::ButtonDimmed( TButtonIndex aIndex ) const
    {
    TBool dimmed = EFalse;
    
    // Ok is dimmed when there's no number. Search is dimmed when there is
    // a number. Trust base class on clear button.
    if ( aIndex == ECallButton )
        {
        dimmed = !iNumberAvailable;
        }
    else if ( aIndex == EPhonebookButton )
        {
        dimmed = iNumberAvailable;
        }
    else
        {
        dimmed = CPhoneDialerController::ButtonDimmed( aIndex );
        }
    return dimmed;
    }

// ---------------------------------------------------------------------------
// EasyDialingAllowed
// ---------------------------------------------------------------------------
//
TBool CPhoneTransferDialerController::EasyDialingAllowed() const
    {
    return EFalse;
    }

// ---------------------------------------------------------------------------
// PhoneTransferDialerController::CreateButtonDataL
// ---------------------------------------------------------------------------
//
MPhoneDialerController::CButtonData* CPhoneTransferDialerController::CreateButtonDataL(
        TInt aCommandId,
        TInt aNormalIconId,
        TInt aNormalMaskId ) const
    {
    __LOGMETHODSTARTEND( PhoneUIVoIPExtension, 
            "CPhoneTransferDialerController::CreateButtonDataL()");
    
    // Load tooltip text.
    HBufC* tooltipText = GetTooltipTextL( aCommandId );
    CleanupStack::PushL( tooltipText );
    
    // Load icon
    TFileName mifPath( KDriveZ );
    mifPath.Append( KDC_APP_BITMAP_DIR );
    mifPath.Append( KPhoneMifFileName );
    
    TAknsItemID skinId = SkinId( aNormalIconId );
    MAknsSkinInstance* skin = AknsUtils::SkinInstance();
    CGulIcon* icon = AknsUtils::CreateGulIconL( skin, skinId, mifPath, 
                aNormalIconId, aNormalMaskId );
    CleanupStack::PushL( icon );
    
    CButtonData* btnData = new (ELeave) CButtonData( aCommandId, icon, tooltipText );
    CleanupStack::Pop( icon );
    CleanupStack::Pop( tooltipText );
    
    return btnData;
    }

// ---------------------------------------------------------------------------
// CPhoneTransferDialerController::GetTooltipText
//
// Tooltip texts for toolbar buttons
// ---------------------------------------------------------------------------
//
HBufC* CPhoneTransferDialerController::GetTooltipTextL( TInt aCommandId ) const
    {
    __LOGMETHODSTARTEND( PhoneUIVoIPExtension, 
        "CPhoneTransferDialerController::GetTooltipTextL()");
    TInt resourceId = 0;
    HBufC* tooltip = NULL;
    switch( aCommandId )
        {                            
        case EPhoneCmdTransferDialerOk:
            resourceId = R_VOIP_DIALER_TOOLTIP_ACCEPT;
            break;    
        case EPhoneCmdTransferDialerSearch:
            resourceId = R_VOIP_DIALER_TOOLTIP_SEARCH;
            break;                      
       default:
           break;
       }
           
    if ( resourceId )
        {
        tooltip = StringLoader::LoadL( resourceId, &iCoeEnv );
        }
    else
        {
        tooltip = KNullDesC().AllocL();
        }
    return tooltip;
    }

// ---------------------------------------------------------------------------
// CPhoneTransferDialerController::SkinId
// ---------------------------------------------------------------------------
//    
TAknsItemID CPhoneTransferDialerController::SkinId( TInt aIconIndex ) const
    {
    TAknsItemID skinId;
    
    switch ( aIconIndex )
        {
        case EMbmPhoneuiQgn_indi_button_send_dtmf:
            skinId = KAknsIIDQgnIndiButtonSendDtmf;
            break;
        case EMbmPhoneuiQgn_indi_dialer_contacts:
            skinId = KAknsIIDQgnIndiDialerContacts;
            break;                     
        default:
            skinId = KAknsIIDNone;        
            break;            
        }
    
    return skinId;  
    }
