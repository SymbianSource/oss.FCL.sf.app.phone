/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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

#include <akntoolbar.h>
#include <AknsItemID.h>
#include <StringLoader.h>


// <-- QT PHONE START -->
//#include <phoneui.mbg>
#include "phoneresourceids.h"
//#include <phoneui.rsg>
//#include <phoneuivoip.rsg>
// <-- QT PHONE END -->

#include "phoneui.pan"
#include "phonelogger.h"
// <-- QT PHONE START -->
//#include "cphonebubblewrapper.h" // For KDC_APP_BITMAP_DIR
#include <data_caging_path_literals.hrh>
// <-- QT PHONE END -->

#include "cphonetransferdialercontroller.h"
#include "cphonemainresourceresolver.h"
#include "phonerssvoip.h"
#include "phoneappcommands.hrh"
#include "phoneappvoipcommands.hrh"


// CONSTANTS
_LIT ( KPhoneMifFileName, "phoneui.mif" );

// Number of buttons 
const TInt KButtonCount = 2;

// <-- QT PHONE START -->
/*
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
        
 */
// <-- QT PHONE END -->
// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//  
CPhoneTransferDialerController::CPhoneTransferDialerController()
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
// CPhoneTransferDialerController::Initialize
// -----------------------------------------------------------
//
void CPhoneTransferDialerController::InitializeL( CAknToolbar& aToolbar )
    {  
    __LOGMETHODSTARTEND( PhoneUIVoIPExtension, 
        "CPhoneTransferDialerController::InitializeL()"); 
    
    if ( !iIsInitialized )
        {    
        __PHONELOG1( 
                EAll, 
                PhoneUIVoIPExtension, 
                "Toolbar control count:=%d", 
                aToolbar.CountComponentControls());
        iToolbar = &aToolbar;
        // Create transfer specific buttons and add them to the 
        // toolbar starting from index 0
 // <-- QT PHONE START --> 
        //CAknButton* button( NULL );
  
 /*
        for( TInt i = 0; i < KButtonCount; i++ )
            {
           
            TAknsItemID skinId = SkinId( bArray[i].iIconIndex );
            // Load tooltip text.
            HBufC* tooltipText = GetTooltipTextL( bArray[i].iCommandId );  
            CleanupStack::PushL( tooltipText );
            button = CreateButtonLC(
                    bArray[i].iIconIndex, 
                    bArray[i].iMaskIndex, 
                    *tooltipText, 
                    skinId );
            // Toolbar takes ownership of the button                    
            iToolbar->AddItemL( button, EAknCtButton, bArray[i].iCommandId, 0, i );
            CleanupStack::Pop( button );
            CleanupStack::PopAndDestroy( tooltipText );
            } 
            */
 // <-- QT PHONE END -->        
        iIsInitialized = ETrue;
        }
   
    iNumberEntryIsEmpty = ETrue;
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
    if ( iNumberEntryIsEmpty )
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
            ResolveResourceID( EPhoneVoIPTransferAddress ) );
        }  
    
    return *iNumberEntryPromptText;
    }

// -----------------------------------------------------------
// CPhoneTransferDialerController::HandleNumberEntryIsEmpty
// -----------------------------------------------------------
//
void CPhoneTransferDialerController::HandleNumberEntryIsEmpty( TBool aIsEmpty )
    {
    __LOGMETHODSTARTEND( PhoneUIVoIPExtension, 
        "CPhoneTransferDialerController::HandleNumberEntryIsEmpty()");
    __ASSERT_DEBUG( iToolbar, Panic( EPhoneCtrlInvariant ) );
    iToolbar->SetItemDimmed( EPhoneCmdTransferDialerOk, aIsEmpty, EFalse );
    iToolbar->SetItemDimmed( EPhoneCmdTransferDialerSearch, !aIsEmpty, EFalse );
    iToolbar->SetItemDimmed( EPhoneDialerCmdClear, aIsEmpty, EFalse );
    iNumberEntryIsEmpty = aIsEmpty;
    }

// -----------------------------------------------------------
// CPhoneTransferDialerController::ShowButtons
// -----------------------------------------------------------
//
void CPhoneTransferDialerController::ShowButtons( TBool aShow )
    {
    __LOGMETHODSTARTEND( PhoneUIVoIPExtension, 
        "CPhoneTransferDialerController::ShowButtons()");
    __ASSERT_DEBUG( iToolbar, Panic( EPhoneCtrlInvariant ) );
    for( TInt i = 0; i < KButtonCount; i++ )
        {
// <-- QT PHONE START -->   
        //iToolbar->HideItem( bArray[ i ].iCommandId, !aShow, EFalse );
        }
    if ( aShow )
        {
        iToolbar->SetItemDimmed( EPhoneCmdTransferDialerOk, ETrue, EFalse ); 
        iToolbar->SetItemDimmed( EPhoneCmdTransferDialerSearch, EFalse, EFalse ); 
        iToolbar->SetItemDimmed( EPhoneDialerCmdClear, ETrue, EFalse ); 
        }
    
    //iToolbar->DrawDeferred();
// <-- QT PHONE END --> 
    }

// ---------------------------------------------------------------------------
// PhoneTransferDialerController::CreateButtonLC
// ---------------------------------------------------------------------------
//
CAknButton* CPhoneTransferDialerController::CreateButtonLC( 
    TInt aNormalIconId,
    TInt aNormalMaskId,
    const TDesC& aTooltipText,
    const TAknsItemID& aSkinIconId ) const
    {   
    __LOGMETHODSTARTEND( PhoneUIVoIPExtension, 
        "CPhoneTransferDialerController::CreateButtonLC()");
    TFileName mifPath( KDriveZ );
    mifPath.Append( KDC_APP_BITMAP_DIR );
    mifPath.Append( KPhoneMifFileName );

    CAknButton* button = CAknButton::NewLC( 
        mifPath,
        aNormalIconId,
        aNormalMaskId,
        -1, -1, // dimmed
        -1, -1, // pressed
        -1, -1, // hover
        KNullDesC,
        aTooltipText, // help
        0, // flags
        0, // state flags
        aSkinIconId );
                                                                 
    button->SetFocusing( EFalse );
    button->SetBackground( iToolbar );

    return button;
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
           tooltip = KNullDesC().Alloc();
           break;
       }
           
    if ( resourceId )
        {
        tooltip = StringLoader::LoadL( resourceId, CCoeEnv::Static() );
        }
    return tooltip;
    }

// ---------------------------------------------------------------------------
// CPhoneTransferDialerController::SkinId
// ---------------------------------------------------------------------------
//    
TAknsItemID CPhoneTransferDialerController::SkinId( TInt /*aIconIndex*/ ) const
    {
    TAknsItemID skinId;
 // <-- QT PHONE START -->
 /*
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
  */
  skinId = KAknsIIDNone; 
// <-- QT PHONE END -->     
    return skinId;  
    }
