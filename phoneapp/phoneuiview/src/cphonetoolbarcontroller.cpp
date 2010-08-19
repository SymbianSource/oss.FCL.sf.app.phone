/*
* Copyright (c) 2007 - 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Handles the updating of toolbar buttons
*
*/


#include <eikappui.h>
#include <eikenv.h>
#include <eikapp.h>
#include <eikcolib.h>
#include <gulicon.h>
#include <akntoolbar.h>
#include <aknbutton.h>
#include <StringLoader.h>
#include <phoneui.rsg>
#include <pevirtualengine.h>
#include <data_caging_path_literals.hrh>
#include <phoneappcommands.hrh>
#include <phoneui.mbg>

#include "tphonecmdparamincallindicatordata.h"
#include "cphonetoolbarcontroller.h"
#include "phoneui.hrh"
#include "phonerssbase.h"
#include "tphonecmdparaminteger.h"
#include "tphonecmdparamboolean.h"
#include "phonelogger.h"


_LIT ( KPhoneMifFileName, "phoneui.mif" );
_LIT( KToolbarButtonEmptyStr, "" );

const TInt KNumberOfButtons = 6;
struct SPhoneToolbarButton
    {
    TInt iIconIndex;
    TInt iMaskIndex;
    TInt iCommandId;
    };

const SPhoneToolbarButton bArray[KNumberOfButtons] =
        { 
            {  EMbmPhoneuiQgn_indi_dialer_contacts,
               EMbmPhoneuiQgn_indi_dialer_contacts_mask,
               EPhoneInCallCmdActivatEPhonebook
            },
            {  EMbmPhoneuiQgn_indi_tb_microphone_mute,
	           EMbmPhoneuiQgn_indi_tb_microphone_mute_mask,
	           EPhoneInCallCmdMute
	        }, 
	        {  EMbmPhoneuiQgn_indi_tb_microphone_unmute,
	           EMbmPhoneuiQgn_indi_tb_microphone_unmute_mask,
	           EPhoneInCallCmdUnmute
	        },
	        {  EMbmPhoneuiQgn_indi_tb_ihf_on,
	           EMbmPhoneuiQgn_indi_tb_ihf_on_mask,
	           EPhoneInCallCmdActivateIhf
	        },
	        {  EMbmPhoneuiQgn_indi_tb_handset,
	           EMbmPhoneuiQgn_indi_tb_handset_mask,
	           EPhoneInCallCmdHandset
	        },
	        {  EMbmPhoneuiQgn_indi_tb_bthf,
               EMbmPhoneuiQgn_indi_tb_bthf,
               EPhoneInCallCmdBtHandsfree
           }
        }; 


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CPhoneToolbarController::CPhoneToolbarController( CCoeEnv& aCoeEnv ):
    iCoeEnv ( aCoeEnv )   
    {
    }

// ---------------------------------------------------------------------------
// Second phase constructor
// ---------------------------------------------------------------------------
//
void CPhoneToolbarController::ConstructL()
    {
    if ( !iToolbar )
        {
        iToolbar = CAknToolbar::NewL( R_PHONEUI_TOOLBAR );
        }
  
    CEikAppUiFactory* appUiFactory =
        static_cast<CEikAppUiFactory*>( CEikonEnv::Static()->AppUiFactory() );  
    appUiFactory->SetViewFixedToolbar( iToolbar );
    
    if( iToolbar )
        {
        iToolbar->SetToolbarObserver( 
                        static_cast<MAknToolbarObserver*>( this ));  
        iToolbar->SetWithSliding( EFalse );
        iToolbar->SetCloseOnAllCommands( EFalse );
        iToolbar->SetFocusing( EFalse );
        
        CAknButton* button( NULL );

        for( TInt i = 0; i < KNumberOfButtons; i++ )
            {
            TAknsItemID skinId = GetSkinIdL( bArray[i].iCommandId );
            // Load tooltip text.
            HBufC* tooltipText;
            GetTooltipTextL ( bArray[i].iCommandId, tooltipText );  
            CleanupStack::PushL( tooltipText );      
         
            button = CreateButtonLC(
                bArray[i].iIconIndex, 
                bArray[i].iMaskIndex, 
                *tooltipText, 
                skinId,
                bArray[i].iCommandId );
            // Takes ownership of the button                    
            iToolbar->AddItemL( button, EAknCtButton, bArray[i].iCommandId,0 );
           
            CleanupStack::Pop( button );
            CleanupStack::PopAndDestroy( tooltipText );
            }
        }
    }
    
// ---------------------------------------------------------------------------
// CreateButtonL
// ---------------------------------------------------------------------------
//
CAknButton* CPhoneToolbarController::CreateButtonLC( 
    TInt aNormalIconId,
    TInt aNormalMaskId,
    const TDesC& aTooltipText,
    const TAknsItemID& aSkinIconId,
    TInt /*aCommand*/ )
    {
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
        KToolbarButtonEmptyStr(),
        aTooltipText, // help
        0, // flags
        0, // state flags
        aSkinIconId );
                                     
    button->SetFocusing( EFalse );
    button->SetBackground( iToolbar );

    return button;
    }
    
// ---------------------------------------------------------------------------
// Static constructor
// ---------------------------------------------------------------------------
//
CPhoneToolbarController* CPhoneToolbarController::NewL( CCoeEnv& aCoeEnv )
    {
    CPhoneToolbarController* self =  new (ELeave) CPhoneToolbarController( aCoeEnv);
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CPhoneToolbarController::~CPhoneToolbarController()
    {
    delete iToolbar;
    }

// ---------------------------------------------------------------------------
// UpdateToolbar
// ---------------------------------------------------------------------------
//
void CPhoneToolbarController::UpdateToolbar()
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneToolbarController::UpdateToolbar()" );
    // Show 'Mute'/'Unmute' icon
    iToolbar->HideItem( EPhoneInCallCmdMute, iMuteFlag, EFalse );
    iToolbar->HideItem( EPhoneInCallCmdUnmute, !iMuteFlag, ETrue );   

    // Show 'Handset'/'Ihf' icon
     if ( iWiredAccFlag )
         {    
         __PHONELOG( EBasic, EPhoneUIView, "CPhoneToolbarController::UpdateToolbar(), wired ");
         iToolbar->HideItem( EPhoneInCallCmdHandset, ETrue, EFalse );
         iToolbar->HideItem( EPhoneInCallCmdBtHandsfree, ETrue, EFalse );
         iToolbar->HideItem( EPhoneInCallCmdActivateIhf, EFalse, ETrue );
         }
     else if ( iBTAccAvailableFlag && !iBTAccFlag )
         {    
         __PHONELOG( EBasic, EPhoneUIView, "CPhoneToolbarController::UpdateToolbar(), BT ");
         iToolbar->HideItem( EPhoneInCallCmdActivateIhf, ETrue, EFalse );
         iToolbar->HideItem( EPhoneInCallCmdHandset, ETrue, EFalse );
         iToolbar->HideItem( EPhoneInCallCmdBtHandsfree, EFalse, ETrue );
         }
     else
         {
         __PHONELOG( EBasic, EPhoneUIView, "CPhoneToolbarController::UpdateToolbar(), else ");
         iToolbar->HideItem( EPhoneInCallCmdHandset, !iIhfFlag, EFalse );
         iToolbar->HideItem( EPhoneInCallCmdActivateIhf, iIhfFlag, EFalse );
         iToolbar->HideItem( EPhoneInCallCmdBtHandsfree, ETrue, ETrue );
         }
    }   

// ---------------------------------------------------------------------------
// OfferToolbarEventL
// ---------------------------------------------------------------------------
//
void CPhoneToolbarController::OfferToolbarEventL( TInt aCommand )
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneToolbarController::OfferToolbarEventL()" );
    // Forward pressed toolbar button to the framework 
    // so that we can handle it in active state            
    static_cast<MEikCommandObserver*>( CEikonEnv::Static()->EikAppUi() )
       ->ProcessCommandL( aCommand );            
    } 
   
// ---------------------------------------------------------------------------
// DynInitToolbarL
// ---------------------------------------------------------------------------
//
void CPhoneToolbarController::DynInitToolbarL( TInt aResourceId, 
                                              CAknToolbar* /*aToolbar*/ )
    { 
    if ( aResourceId == R_PHONEUI_TOOLBAR )
        {
        UpdateToolbar();
        }     
    }

// ---------------------------------------------------------------------------
// GetTooltipText
//
// Tooltip texts for toolbar buttons
// ---------------------------------------------------------------------------
//
void  CPhoneToolbarController::GetTooltipTextL( TInt aCommandId, HBufC*& aText ) 
    {
    TInt resourceId = 0;
    switch( aCommandId )
        {                     
        case EPhoneInCallCmdActivatEPhonebook:
            resourceId = R_PHONEUI_TOOLTIP_CONTACTS;
            break; 
            
        case EPhoneInCallCmdMute:
            resourceId = R_PHONEUI_TOOLTIP_MUTE;
            break;
            
        case EPhoneInCallCmdActivateIhf:
            resourceId = R_PHONEUI_TOOLTIP_LOUDSPEAKER;
            break;
            
        case EPhoneInCallCmdUnmute:
            resourceId = R_PHONEUI_TOOLTIP_UNMUTE;
            break;
            
        case EPhoneInCallCmdHandset:
            resourceId = R_PHONEUI_TOOLTIP_HANDSET;
            break;
            
        case EPhoneInCallCmdBtHandsfree:
            resourceId = R_PHONEUI_TOOLTIP_BT_HANDSFREE;
            break;
            
        default:
            aText = KNullDesC().Alloc();
            break;
        }
        
    if( resourceId )
        {
        aText = StringLoader::LoadL( resourceId, &iCoeEnv );
        }
        
    }

// ---------------------------------------------------------------------------
TAknsItemID CPhoneToolbarController::GetSkinIdL( TInt aCommandId ) 
    {
    TAknsItemID skinId = KAknsIIDNone;
    switch( aCommandId )
        {                       
        case EPhoneInCallCmdActivatEPhonebook:
            skinId = KAknsIIDQgnIndiDialerContacts;
            break; 
        case EPhoneInCallCmdMute:
            skinId = KAknsIIDQgnIndiButtonMuteMic;
            break;
        case EPhoneInCallCmdActivateIhf:
            skinId = KAknsIIDQgnIndiButtonLoudspeaker;
            break;
        case EPhoneInCallCmdUnmute:
            skinId = KAknsIIDQgnIndiButtonUnmuteMic;
            break;
        case EPhoneInCallCmdHandset:
            skinId = KAknsIIDQgnIndiButtonHandset;
            break;
        case EPhoneInCallCmdBtHandsfree:
            skinId = KAknsIIDQgnIndiButtonBluetooth;
            break;
        default:
            skinId = KAknsIIDNone;
            break;
        }
    return skinId;        
    }

// ---------------------------------------------------------------------------
// ShowToolbar
//
// ---------------------------------------------------------------------------
//
void CPhoneToolbarController::ShowToolbar()
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneToolbarController::ShowToolbar()" );

    if ( iToolbar )
        {
        if ( iToolbar->IsToolbarDisabled() )
            {
            TRAP_IGNORE(iToolbar->DisableToolbarL( EFalse ));
            }
        UpdateToolbar();
        iToolbar->SetToolbarVisibility( ETrue, EFalse );
        }
    }

// ---------------------------------------------------------------------------
// HideToolbar
//
// ---------------------------------------------------------------------------
//
void CPhoneToolbarController::HideToolbar()
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneToolbarController::HideToolbar()" );
    if ( iToolbar )
        {
        if ( iToolbar->IsToolbarDisabled() )
            {
            TRAP_IGNORE(iToolbar->DisableToolbarL( EFalse ));
            }
        iToolbar->SetToolbarVisibility( EFalse, EFalse );
        }
    }

// ---------------------------------------------------------
// CPhoneToolbarController::SetMuteFlag
// ---------------------------------------------------------
//
void CPhoneToolbarController::SetMuteFlag( TPhoneCommandParam* aCommandParam )
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneToolbarController::SetMuteFlag()" );
    // Check is the given parameter valid
    if ( aCommandParam->ParamId() == TPhoneCommandParam::EPhoneParamIdBoolean )
        {
        TPhoneCmdParamBoolean* muteFlag = 
            static_cast<TPhoneCmdParamBoolean*>( aCommandParam );

        iMuteFlag = muteFlag->Boolean();
        UpdateToolbar();
        }
    }

// ---------------------------------------------------------
// CPhoneToolbarController::SetIhfFlag
// ---------------------------------------------------------
//
void CPhoneToolbarController::SetIhfFlag( TPhoneCommandParam* aCommandParam )
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneToolbarController::SetIhfFlag()" );
    // Check is the given parameter valid
    if ( aCommandParam->ParamId() == TPhoneCommandParam::EPhoneParamIdBoolean )
        {
        TPhoneCmdParamBoolean* ihfFlag = 
            static_cast<TPhoneCmdParamBoolean*>( aCommandParam );

        iIhfFlag = ihfFlag->Boolean();
        UpdateToolbar();
        }
    }

// ---------------------------------------------------------
// CPhoneMenuController::SetWiredAccFlag
// ---------------------------------------------------------
//
void CPhoneToolbarController::SetWiredAccFlag( TPhoneCommandParam* aCommandParam )
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneMenuController::SetWiredAccAvailableFlag()" );
    // Check is the given parameter valid
    if ( aCommandParam->ParamId() == TPhoneCommandParam::EPhoneParamIdBoolean )
        {
        TPhoneCmdParamBoolean* accFlag = 
            static_cast<TPhoneCmdParamBoolean*>( aCommandParam );

        iWiredAccFlag = accFlag->Boolean();        
        if ( iCallInProgress ) 
            {
            if ( iWiredAccFlag )
                {
                // Dim toolbar button 
                iToolbar->SetItemDimmed( EPhoneInCallCmdActivateIhf, ETrue, ETrue );
                }
            else if ( !iDimActivateIhf )
                {
                // Don't undim ActivateIhf button, if it was intended to be dimmed all the time
                iToolbar->SetItemDimmed( EPhoneInCallCmdActivateIhf, EFalse, ETrue );
                }
            
            UpdateToolbar(); 
            }
        }
    }
// ---------------------------------------------------------
// CPhoneToolbarController::SetBTAccFlag
// ---------------------------------------------------------
//
void CPhoneToolbarController::SetBTAccFlag( TPhoneCommandParam* aCommandParam )
    {
    if ( aCommandParam->ParamId() == TPhoneCommandParam::EPhoneParamIdBoolean )
        {
        TPhoneCmdParamBoolean* booleanParam = 
            static_cast<TPhoneCmdParamBoolean*>( aCommandParam );
        iBTAccFlag = booleanParam->Boolean();
        UpdateToolbar(); 
        }     
    }

// ---------------------------------------------------------
// CPhoneToolbarController::SetBTAccAvailableFlag
// ---------------------------------------------------------
//
void CPhoneToolbarController::SetBTAccAvailableFlag( TPhoneCommandParam* aCommandParam )
    {
    // Check is the given parameter valid
    if ( aCommandParam->ParamId() == TPhoneCommandParam::EPhoneParamIdBoolean )
        {
        TPhoneCmdParamBoolean* accFlag = 
            static_cast<TPhoneCmdParamBoolean*>( aCommandParam );

        iBTAccAvailableFlag = accFlag->Boolean();
        UpdateToolbar();
        }
    }


// ---------------------------------------------------------
// CPhoneToolbarController::SetCallInProgressFlag
// ---------------------------------------------------------
// 
void CPhoneToolbarController::SetCallInProgressFlag( TPhoneCommandParam* aCommandParam )
    {
    TPhoneCmdParamIncallIndicatorData* param =
        static_cast<TPhoneCmdParamIncallIndicatorData*>( aCommandParam );
    
    TInt callState = param->CallState();  

    __PHONELOG1( EBasic, EPhoneUIView,
      "CPhoneToolbarController::SetCallInProgressFlag - Current CallState = (%d)",
      callState );
    
    switch ( callState )
        {
        case EPSCTsyCallStateUninitialized:
        case EPSCTsyCallStateNone:
            {
            // No active call in progress.
            iCallInProgress = EFalse;
            break;
            }                 
        default:
            {
            // Active call in progress.
            iCallInProgress = ETrue;
            break;
            } 
        }
    }

// ---------------------------------------------------------
// CPhoneToolbarController::DimToolbar
// ---------------------------------------------------------
//
void CPhoneToolbarController::DimToolbar( const TBool aDimmed )
    {
 //   __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneToolbarController::DimToolbar()" );

    // Dim/Undim all toolbar buttons 
    iToolbar->SetItemDimmed( EPhoneInCallCmdMute, aDimmed, ETrue );
    iToolbar->SetItemDimmed( EPhoneInCallCmdUnmute, aDimmed, ETrue );
    iToolbar->SetItemDimmed( EPhoneInCallCmdActivatEPhonebook, aDimmed, ETrue );
    iToolbar->SetItemDimmed( EPhoneInCallCmdHandset, aDimmed, ETrue );  
    iToolbar->SetItemDimmed( EPhoneInCallCmdBtHandsfree, aDimmed, ETrue );
    // Don't dim/undim iHF when complete toolbar dimming is changed. 
    if ( !iWiredAccFlag )
        {
        iToolbar->SetItemDimmed( EPhoneInCallCmdActivateIhf, aDimmed, ETrue );
        }
    iDimActivateIhf = aDimmed;
    }

// ---------------------------------------------------------
// CPhoneToolbarController::SetToolbarButtonDimmed
// ---------------------------------------------------------
//
void CPhoneToolbarController::SetToolbarButtonDimmed( const TInt aCommandId, const TBool aDimmed )
    {
 // __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneToolbarController::SetToolbarButtonDimmed()" );

    if ( aCommandId == EPhoneInCallCmdActivateIhf ) 
        {
        if ( aDimmed )
            {
            // Dim toolbar button 
            iToolbar->SetItemDimmed( aCommandId, aDimmed, ETrue );
            }
        else
            {
            // Don't undim IHF if wired acc is connected. 
            if ( !iWiredAccFlag )
                {
                iToolbar->SetItemDimmed( aCommandId, aDimmed, ETrue );
                }
            }
        iDimActivateIhf = aDimmed;
        }
    else
        {
        // Dim/undim toolbar button 
        iToolbar->SetItemDimmed( aCommandId, aDimmed, ETrue );
        }
    }

// END

