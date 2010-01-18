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
* Description:  Handles the updating of dialer buttons
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

#include "cphonedialercontroller.h"
#include "cphonebubblewrapper.h"
#include "phoneui.hrh"
#include "phonerssbase.h"
#include "tphonecmdparaminteger.h"
#include "cphonemenucontroller.h"
#include "cphonemainresourceresolver.h"
#include "tphonecmdparamcallstatedata.h"
#include "tphonecmdparamboolean.h"
#include "mphonedialercontroller.h"

_LIT ( KPhoneMifFileName, "phoneui.mif" );
_LIT( KDialerToolbarButtonEmptyStr, "" );

const TInt KNumberOfButtons = 6;
struct SPhoneDialerToolbarButton
    {
    TInt iIconIndex;
    TInt iMaskIndex;
    TInt iCommandId;    
    };

const SPhoneDialerToolbarButton bArray[KNumberOfButtons] =
        {  
            { EMbmPhoneuiQgn_indi_dialer_call_send,
              EMbmPhoneuiQgn_indi_dialer_call_send_mask,
              EPhoneDialerCmdLog
            },
            { EMbmPhoneuiQgn_indi_dialer_call_send,
              EMbmPhoneuiQgn_indi_dialer_call_send_mask,
              EPhoneNumberAcqCmdSendCommand
            },        
            
            { EMbmPhoneuiQgn_indi_button_send_dtmf,
              EMbmPhoneuiQgn_indi_button_send_dtmf_mask,
              EPhoneCmdDtmfOk
            },               
            { EMbmPhoneuiQgn_indi_dialer_contacts,
              EMbmPhoneuiQgn_indi_dialer_contacts_mask,
              EPhoneDialerCmdContacts
            },
            { EMbmPhoneuiQgn_indi_dialer_add_contacts,
              EMbmPhoneuiQgn_indi_dialer_add_contacts_mask,
              EPhoneNumberAcqCmdAddToContacts
            },            
            { EMbmPhoneuiQgn_indi_dialer_clear,
              EMbmPhoneuiQgn_indi_dialer_clear_mask,
              EPhoneDialerCmdClear
            }
        }; 

const TInt KDialerToolbarButtonCount = 6; 
const TInt KDialerToolbarButtonKeyRepeatDelay = 300;
const TInt KDialerToolbarKeyRepeatInterval = 75;

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CPhoneDialerController::CPhoneDialerController( 
                                        CPhoneBubbleWrapper& aBubbleWrapper,
                                        CCoeEnv& aCoeEnv,
                                        CPhoneMenuController& aMenuController ):
    iBubbleWrapper ( aBubbleWrapper ),
    iCoeEnv ( aCoeEnv ),
    iMenuController( aMenuController ),
    iUpdateToolbar( EFalse )
    {
    }

// ---------------------------------------------------------------------------
// Second phase constructor
// ---------------------------------------------------------------------------
//
void CPhoneDialerController::ConstructL()
    {
   
    iBubbleWrapper.SetNumberEntryObserver( *this );
   
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

        for( TInt i = 0; i < KDialerToolbarButtonCount; i++ )
            {
            TAknsItemID skinId = SkinId( bArray[i].iIconIndex );
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
       
        // Hide 'Send dtmf' icon
        iToolbar->HideItem( EPhoneCmdDtmfOk, ETrue, EFalse );
        TRAP_IGNORE(iToolbar->DisableToolbarL( ETrue ));
        }
    }
    
// ---------------------------------------------------------------------------
// CreateButtonL
// ---------------------------------------------------------------------------
//
CAknButton* CPhoneDialerController::CreateButtonLC( 
    TInt aNormalIconId,
    TInt aNormalMaskId,
    const TDesC& aTooltipText,
    const TAknsItemID& aSkinIconId,
    TInt aCommand )
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
        KDialerToolbarButtonEmptyStr(),
        aTooltipText, // help
        0, // flags
        0, // state flags
        aSkinIconId );
                               
    if( aCommand == EPhoneDialerCmdClear )
        {
        button->SetButtonFlags( KAknButtonKeyRepeat );
        // delay 300 ms and repeat 75 ms interval   
        button->SetKeyRepeatInterval( KDialerToolbarButtonKeyRepeatDelay,
                                      KDialerToolbarKeyRepeatInterval );
        }
                                     
    button->SetFocusing( EFalse );
    button->SetBackground( iToolbar );

    return button;
    }
    
    
// ---------------------------------------------------------------------------
// Static constructor
// ---------------------------------------------------------------------------
//
CPhoneDialerController* CPhoneDialerController::NewL( 
                                    CPhoneBubbleWrapper& aBubbleWrapper,
                                    CCoeEnv& aCoeEnv,
                                    CPhoneMenuController& aMenuController  )
    {
    CPhoneDialerController* self =  new (ELeave) CPhoneDialerController( 
                                        aBubbleWrapper
                                       , aCoeEnv
                                       , aMenuController );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CPhoneDialerController::~CPhoneDialerController()
    {
    delete iToolbar;
    }

// ---------------------------------------------------------------------------
// NumberEntryStateChanged
// ---------------------------------------------------------------------------
//
void CPhoneDialerController::NumberEntryStateChanged( TBool aHidePromptText )
    {
    if( aHidePromptText != iPreviousHidePromptTextStatus )
        {
        iPreviousHidePromptTextStatus = aHidePromptText; 
        iUpdateToolbar = ETrue;
        }
    
    HandleNumberEntryChanged();
    
    }

// ---------------------------------------------------------------------------
// HandleNumberEntryChanged
// ---------------------------------------------------------------------------
//
void CPhoneDialerController::HandleNumberEntryChanged()
    {
    if( iUpdateToolbar )
        {
        UpdateToolbar();
        iUpdateToolbar = EFalse;        
        }
    
    iBubbleWrapper.HandleNumberEntryChanged();
    
    } 

// ---------------------------------------------------------------------------
// HandleNumberEntryChanged
// ---------------------------------------------------------------------------
//
void CPhoneDialerController::SetNumberEntryChangedCallBack( 
        TPhoneCommandParam* aCommandParam )
    {
    iBubbleWrapper.SetNumberEntryChangedCallBack( aCommandParam );
    
    }
     
// ---------------------------------------------------------------------------
// CreateButtonSetNumberEntryEmpty
// ---------------------------------------------------------------------------
//
void CPhoneDialerController::CreateButtonSetNumberEntryEmpty()
    {
    
    if( iToolbar )
        {
        if( iDtmfDialerVisible )
            {
            // Dim 'Send dtmf' 
            iToolbar->SetItemDimmed( EPhoneCmdDtmfOk, ETrue, EFalse );
            }
        else if ( iCustomizedDialer )
            {
            iCustomizedDialer->HandleNumberEntryIsEmpty( ETrue );
            }
        else
            {         
            // Show 'Logs ' and 'Contacts' items
            iToolbar->HideItem( EPhoneDialerCmdLog, EFalse,EFalse );
            iToolbar->HideItem( EPhoneDialerCmdContacts, EFalse, EFalse );
            
            if ( iRestrictedDialer )
                {
                // Dim 'Logs' 
                iToolbar->SetItemDimmed( EPhoneDialerCmdLog, ETrue
                                       , EFalse );
                // Dim 'Contacts' 
                iToolbar->SetItemDimmed( EPhoneDialerCmdContacts, ETrue
                                       , EFalse );
                }
                            
            // Hide 'Send' and 'Add to contacts'        
            iToolbar->HideItem( EPhoneNumberAcqCmdSendCommand, ETrue
                                , EFalse );
            iToolbar->HideItem( EPhoneNumberAcqCmdAddToContacts, ETrue
                                , EFalse );                
            }        
        }
    }

// ---------------------------------------------------------------------------
// CreateButtonSetNumberEntryNotEmpty
// ---------------------------------------------------------------------------
//
void CPhoneDialerController::CreateButtonSetNumberEntryNotEmpty()
    {
    if( iToolbar )
        {
        if( iDtmfDialerVisible )
            {
            // Show 'Send dtmf' 
            iToolbar->SetItemDimmed( EPhoneCmdDtmfOk, EFalse, EFalse );
            }           
        else if ( iCustomizedDialer )
            {
            iCustomizedDialer->HandleNumberEntryIsEmpty( EFalse );
            }
        else
            {
            // Show 'Send' and 'Add to contacts'
            iToolbar->HideItem( EPhoneNumberAcqCmdAddToContacts, EFalse
                              , EFalse );      
            iToolbar->HideItem( EPhoneNumberAcqCmdSendCommand, EFalse
                              , EFalse );  
                    
            if ( iRestrictedDialer )
                {
                // Dim 'Send' 
                iToolbar->SetItemDimmed( EPhoneNumberAcqCmdSendCommand, ETrue
                                       , EFalse );
                // Dim 'Add to Contacts' 
                iToolbar->SetItemDimmed( EPhoneNumberAcqCmdAddToContacts, ETrue
                                       , EFalse );
                }  
                           
            // Hide 'Logs ' and 'Contacts' 
            iToolbar->HideItem( EPhoneDialerCmdLog, ETrue,EFalse );
            iToolbar->HideItem( EPhoneDialerCmdContacts, ETrue, EFalse ); 
            
            TPhoneCmdParamCallStateData callStateData;
            callStateData.SetCallState( EPEStateConnected );
            iBubbleWrapper.GetCallIdByState( &callStateData );
            TInt connected = callStateData.CallId();    
             
            //If there is a active emergency call, Send item must be dimmed.  
            if ( connected == KEmergencyCallId )
                {
                //Dim Send
                iToolbar->SetItemDimmed( EPhoneNumberAcqCmdSendCommand, ETrue
                                        ,EFalse );                
                }
            else
                {
                //UnDim
                iToolbar->SetItemDimmed( EPhoneNumberAcqCmdSendCommand, EFalse
                                        ,EFalse );                     
                }
            }    
        }
    }

// ---------------------------------------------------------------------------
// SetDtmfDialerVisibleL
// ---------------------------------------------------------------------------
//
void CPhoneDialerController::SetDtmfDialerVisibleL( TBool aVisible )
    {
    iDtmfDialerVisible = aVisible;
    
    if( iDtmfDialerVisible )
        {
        // Load prompt resource text to DTMF dialer.
        TInt resourceId = CPhoneMainResourceResolver::Instance()->
                    ResolveResourceID( EPhoneDtmfDialerNumberEntryPromptText ); 
                    
        HBufC* loadedText = StringLoader::LoadL( resourceId
                                               , &iCoeEnv );

        iBubbleWrapper.SetNumberEntryPromptText( loadedText->Des() );
        
        delete loadedText;
        }
    else        
        { 
        // Clear prompt text.    
        iBubbleWrapper.SetNumberEntryPromptText( KNullDesC );  
                 
        // Hide
        iToolbar->HideItem( EPhoneCmdDtmfOk, ETrue, EFalse );  
           
        // Show       
        iToolbar->HideItem( EPhoneNumberAcqCmdSendCommand, EFalse, EFalse );
        iToolbar->HideItem( EPhoneDialerCmdLog, EFalse, EFalse );            
        iToolbar->HideItem( EPhoneNumberAcqCmdAddToContacts, EFalse, EFalse );    
        // Dimming 
        iToolbar->SetItemDimmed( EPhoneDialerCmdContacts, EFalse, EFalse );                
        }
    }
    
// ---------------------------------------------------------------------------
// SetRestrictedDialerL
// ---------------------------------------------------------------------------
//
void CPhoneDialerController::SetRestrictedDialer( TBool aRestricted )
    {
    iRestrictedDialer = aRestricted;
    }
// ---------------------------------------------------------------------------
// UpdateToolbar
// ---------------------------------------------------------------------------
//
void CPhoneDialerController::UpdateToolbar()
    {
    
    TPhoneCmdParamInteger numberEntryCountParam;
    iBubbleWrapper.GetNumberEntryCount( 
                            (TPhoneCommandParam*) &numberEntryCountParam );
    TInt neLength( numberEntryCountParam.Integer() );
    
    if ( neLength )
        {
        CreateButtonSetNumberEntryNotEmpty();
        iToolbar->SetItemDimmed( EPhoneDialerCmdClear, EFalse, EFalse );  
        iToolbar->DrawDeferred();     
        
        //Update also options menu via controller
        iMenuController.SetNumberEntryEmptyFlag( EFalse );        
        }
        
    else
        {
        CreateButtonSetNumberEntryEmpty();
        iToolbar->SetItemDimmed( EPhoneDialerCmdClear, ETrue, EFalse );
        iToolbar->DrawDeferred();
        
        //Update also options menu via controller
        iMenuController.SetNumberEntryEmptyFlag( ETrue );                  
        } 

    }   

// ---------------------------------------------------------------------------
// OfferToolbarEventL
// ---------------------------------------------------------------------------
//
void CPhoneDialerController::OfferToolbarEventL( TInt aCommand )
    {
    switch ( aCommand )
        {
        case EPhoneDialerCmdClear:
            {
            // Send key event.
            TKeyEvent keyEvent;
            keyEvent.iModifiers = 0;
            keyEvent.iRepeats = 0;
            keyEvent.iCode = EKeyBackspace;
            keyEvent.iScanCode = EStdKeyBackspace;
            iCoeEnv.SimulateKeyEventL( keyEvent, EEventKey );
            }
            break;   
        default:
            {
            // Forward pressed toolbar button to the framework 
            // so that we can handle it in active state            
            static_cast<MEikCommandObserver*>( CEikonEnv::Static()->EikAppUi() )
               ->ProcessCommandL( aCommand );            
            }
            break;    
        }
    } 
   
// ---------------------------------------------------------------------------
// DynInitToolbarL
// ---------------------------------------------------------------------------
//
void CPhoneDialerController::DynInitToolbarL( TInt aResourceId, 
                                              CAknToolbar* /*aToolbar*/ )
    { 
    if ( aResourceId == R_PHONEUI_TOOLBAR )
        {
        if ( !iDtmfDialerVisible )
            {
            UpdateToolbar();
            }
        else
            {
            // initialize toolbar for dtmf dialer
            //
            // Show
            iToolbar->HideItem( EPhoneCmdDtmfOk, EFalse, EFalse );
            iToolbar->HideItem( EPhoneDialerCmdContacts, EFalse, EFalse );     
            
            // Hide
            iToolbar->HideItem( EPhoneNumberAcqCmdSendCommand, ETrue, EFalse );
            iToolbar->HideItem( EPhoneDialerCmdLog, ETrue, EFalse );            
            iToolbar->HideItem( EPhoneNumberAcqCmdAddToContacts, ETrue
                              , EFalse );     
            
            // Dimming
            iToolbar->SetItemDimmed( EPhoneDialerCmdContacts, ETrue, EFalse ); 
            iToolbar->SetItemDimmed( EPhoneCmdDtmfOk, ETrue, EFalse );                   
            }
        }     
    }
// ---------------------------------------------------------------------------
// IsDTMFDialerVisible
// ---------------------------------------------------------------------------
//
TBool CPhoneDialerController::IsDTMFDialerVisible() const
    {
    return iDtmfDialerVisible;
    }


// ---------------------------------------------------------------------------
// GetTooltipText
//
// Tooltip texts for toolbar buttons
// ---------------------------------------------------------------------------
//
void  CPhoneDialerController::GetTooltipTextL( TInt aCommandId, HBufC*& aText ) 
    {
    TInt resourceId = 0;
    switch( aCommandId )
        {
        case EPhoneDialerCmdLog:
            resourceId = R_PHONEUI_DIALER_TOOLTIP_RECENT_CALLS;
            break;              
                  
        case EPhoneNumberAcqCmdSendCommand:
            resourceId = R_PHONEUI_DIALER_TOOLTIP_CALL;
            break;      
                          
        case EPhoneCmdDtmfOk:
            resourceId = R_PHONEUI_DIALER_TOOLTIP_SEND;
            break;   
                             
        case EPhoneDialerCmdContacts:
            resourceId = R_PHONEUI_DIALER_TOOLTIP_CONTACTS;
            break;   
                             
        case EPhoneNumberAcqCmdAddToContacts:
            resourceId = R_PHONEUI_DIALER_TOOLTIP_ADD_TO_CONTACTS;
            break;   
                             
        case EPhoneDialerCmdClear:
            resourceId = R_PHONEUI_DIALER_TOOLTIP_CLEAR;
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
// SetServiceCodeFlagL
//
// Change call button tooltip text between 'Send' or 'Call'
// 
// ---------------------------------------------------------------------------
//
void CPhoneDialerController::SetServiceCodeFlagL( 
    TPhoneCommandParam* aCommandParam )
    {
    
    // Check is the given parameter valid
    if ( aCommandParam->ParamId() == TPhoneCommandParam::EPhoneParamIdBoolean )
        {   
        TInt resourceId = 0;
        TPhoneCmdParamBoolean* serviceCodeFlag = 
            static_cast<TPhoneCmdParamBoolean*>( aCommandParam );
        // Sanity check
        if( serviceCodeFlag->Boolean() == iServiceCodeFlag )
            {
            return;
            }
        // Get the toolbar button control
        CAknButton* button = static_cast<CAknButton*>
        ( iToolbar->ControlOrNull( EPhoneNumberAcqCmdSendCommand) );
        
        CAknButtonState* state = button->State();                              
        if ( serviceCodeFlag->Boolean() )
            {
            iServiceCodeFlag = ETrue;
            resourceId = R_PHONEUI_DIALER_TOOLTIP_SEND;
            }
        else
            {
            iServiceCodeFlag = EFalse;
            resourceId = R_PHONEUI_DIALER_TOOLTIP_CALL;
            }  
                
        if( resourceId )
            {    
            // Change text              
            HBufC* tooltipText = StringLoader::LoadL( resourceId,
                                             CCoeEnv::Static() );
            CleanupStack::PushL( tooltipText );                                         
            state->SetHelpTextL( *tooltipText );
            CleanupStack::PopAndDestroy( tooltipText );   
            }
        }
    }

// ---------------------------------------------------------------------------
// ShowToolbar
//
// ---------------------------------------------------------------------------
//
void CPhoneDialerController::ShowToolbar()
    {
    if ( iToolbar )
        {
        if ( iToolbar->IsToolbarDisabled() )
            {
            TRAP_IGNORE(iToolbar->DisableToolbarL( EFalse ));
            }
        iToolbar->HideItemsAndDrawOnlyBackground( EFalse );
        iToolbar->SetToolbarVisibility( ETrue, EFalse );
        }
    }

// ---------------------------------------------------------------------------
// HideToolbar
//
// ---------------------------------------------------------------------------
//
void CPhoneDialerController::HideToolbar()
    {
    if ( iToolbar )
        {
        if( iToolbar->IsShown() )
            {
            iToolbar->HideItemsAndDrawOnlyBackground( ETrue );
            }
        else if ( iToolbar->IsToolbarDisabled() )
            {
            TRAP_IGNORE(iToolbar->DisableToolbarL( EFalse ));
            iToolbar->HideItemsAndDrawOnlyBackground( ETrue );
            iToolbar->SetToolbarVisibility( ETrue, EFalse );
            }
        }        
    }

// ---------------------------------------------------------------------------
// ShowCustomizedDialerL
//
// Sets custom dialer buttons visible
// 
// ---------------------------------------------------------------------------
//
void CPhoneDialerController::ShowCustomizedDialerL( 
    MPhoneDialerController* aCustomController )
    {  
    if ( aCustomController )
        {
        // Hide all buttons except clear button 
        for ( TInt i = 0; i < KNumberOfButtons; i++ )
            {
            if ( bArray[ i ].iCommandId != EPhoneDialerCmdClear )
                {
                iToolbar->HideItem( bArray[ i ].iCommandId, ETrue, EFalse );
                }
            }
 
        aCustomController->InitializeL( *iToolbar );
        iBubbleWrapper.SetNumberEntryPromptText( 
            aCustomController->NumberEntryPromptTextL() );
        aCustomController->ShowButtons( ETrue );
        }
    iCustomizedDialer = aCustomController;
    }

// ---------------------------------------------------------------------------
// HideCustomizedDialerL
//
// Hides custom dialer buttons 
// 
// ---------------------------------------------------------------------------
//
void CPhoneDialerController::HideCustomizedDialer()
    {  
    iCustomizedDialer->ShowButtons( EFalse );
    iBubbleWrapper.SetNumberEntryPromptText( KNullDesC );              
    iCustomizedDialer = NULL;
    }

// ---------------------------------------------------------------------------
// IsCustomizedDialerVisible
// ---------------------------------------------------------------------------
//
TBool CPhoneDialerController::IsCustomizedDialerVisible() const
    {
    return ( NULL != iCustomizedDialer );
    }

// ---------------------------------------------------------------------------
// CustomizedCbaResourceId
// ---------------------------------------------------------------------------
//
TInt CPhoneDialerController::CustomizedCbaResourceId() const
    {
    TInt resourceId = KErrNotFound;
    if ( iCustomizedDialer )
        {
        resourceId = iCustomizedDialer->CbaResourceId();
        }
    return resourceId;
    }

// ---------------------------------------------------------------------------
// CustomizedMenuResourceId
// ---------------------------------------------------------------------------
//
TInt CPhoneDialerController::CustomizedMenuResourceId() const
    {
    TInt resourceId = KErrNotFound;
    if ( iCustomizedDialer )
        {
        resourceId = iCustomizedDialer->MenuResourceId();  
        }
    return resourceId;
    }

// END
