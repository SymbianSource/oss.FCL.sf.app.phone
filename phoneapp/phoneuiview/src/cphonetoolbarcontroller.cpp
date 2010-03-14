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

#include "cphonetoolbarcontroller.h"
#include "phoneui.hrh"
#include "phonerssbase.h"
#include "tphonecmdparaminteger.h"
#include "tphonecmdparamboolean.h"
#include "phonelogger.h"


_LIT ( KPhoneMifFileName, "phoneui.mif" );
_LIT( KToolbarButtonEmptyStr, "" );

const TInt KNumberOfButtons = 5;
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
        TRAP_IGNORE(iToolbar->DisableToolbarL( ETrue ));
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
    iToolbar->HideItem( EPhoneInCallCmdActivateIhf, iIhfFlag, EFalse );
    iToolbar->HideItem( EPhoneInCallCmdHandset, !iIhfFlag, ETrue );
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
    iToolbar->SetItemDimmed( EPhoneInCallCmdActivateIhf, aDimmed, ETrue );
    iToolbar->SetItemDimmed( EPhoneInCallCmdHandset, aDimmed, ETrue );        
    }

// ---------------------------------------------------------
// CPhoneToolbarController::SetToolbarButtonDimmed
// ---------------------------------------------------------
//
void CPhoneToolbarController::SetToolbarButtonDimmed( const TInt aCommandId, const TBool aDimmed )
    {
 // __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneToolbarController::SetToolbarButtonDimmed()" );

    // Dim toolbar button 
    iToolbar->SetItemDimmed( aCommandId, aDimmed, ETrue );
    }

// END

