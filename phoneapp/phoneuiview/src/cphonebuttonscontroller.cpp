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
* Description:  Handles the updating of touch buttons
*
*/


#include <eikappui.h>
#include <eikenv.h>
#include <pevirtualengine.h>
#include <AknsSkinInstance.h>
#include <AknsUtils.h>
#include <AknsConstants.h>
#include <data_caging_path_literals.hrh>
#include <phoneui.mbg>
#include "cphonebuttonscontroller.h"
#include "phoneui.hrh"
#include "tphonecommandparam.h"
#include "tphonecmdparamboolean.h"
#include "tphonecmdparaminteger.h"
#include "tphonecmdparamcallheaderdata.h"
#include "cphonemainresourceresolver.h"
#include "phonerssbase.h"

_LIT( KPhoneUiScalableIconFile, "Z:phoneui.mif" );

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CPhoneButtonsController::CPhoneButtonsController(
    MBubbleTouchPaneInterface& aTouchPane ) : 
    iTouchPane( aTouchPane )
    {
    iTouchPane.SetIconProvider( this );
    }

// ---------------------------------------------------------------------------
// Second phase constructor
// ---------------------------------------------------------------------------
//
void CPhoneButtonsController::ConstructL()
    {
    }

// ---------------------------------------------------------------------------
// Static constructor
// ---------------------------------------------------------------------------
//
CPhoneButtonsController* CPhoneButtonsController::NewL(
    MBubbleTouchPaneInterface& aTouchPane )
    {
    CPhoneButtonsController* self = 
        new (ELeave) CPhoneButtonsController( aTouchPane );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CPhoneButtonsController::~CPhoneButtonsController()
    {
    }

// ---------------------------------------------------------------------------
// SetButtonSet
// ---------------------------------------------------------------------------
//
void CPhoneButtonsController::SetButtonSet(
    TPhoneCommandParam* aCommandParam )
    {
    TPhoneCmdParamInteger* cbaResource = 
        static_cast<TPhoneCmdParamInteger*>( aCommandParam );
    TInt resourceId = cbaResource->Integer();
    
    iTouchPane.SetButtonSet( resourceId );
    ReplaceAudioButton();
    SetButtonDimming();
    }


// ---------------------------------------------------------------------------
//  SetMuteFlag
// ---------------------------------------------------------------------------
//
void CPhoneButtonsController::SetMuteFlag( 
    TPhoneCommandParam* aCommandParam )
    {
    if ( aCommandParam->ParamId() == TPhoneCommandParam::EPhoneParamIdBoolean )
        {
        TPhoneCmdParamBoolean* booleanParam = 
            static_cast<TPhoneCmdParamBoolean*>( aCommandParam );
            
        if ( booleanParam->Boolean() )
            {
            iFlags |= EButtonsPhoneMuted;     
            }
        else
            {
            iFlags &= ~EButtonsPhoneMuted;    
            }            
        
        if ( iFlags & EButtonsPhoneMuted )
            {
            iTouchPane.SetButtonState( EPhoneInCallCmdUnmute );    
            }
        else
            {
            iTouchPane.SetButtonState( EPhoneInCallCmdMute );        
            }            
        
        }
    }
    
// ---------------------------------------------------------------------------
// SetHoldFlag
// ---------------------------------------------------------------------------
//
void CPhoneButtonsController::SetHoldFlag( 
    TPhoneCommandParam* aCommandParam )
    {
    if ( aCommandParam->ParamId() == 
        TPhoneCommandParam::EPhoneParamIdBoolean )
        { 
        TPhoneCmdParamBoolean* booleanParam = 
            static_cast<TPhoneCmdParamBoolean*>( aCommandParam );
            
        if ( booleanParam->Boolean() )
            {
            iFlags |= EButtonsCallOnHold;     
            }
        else
            {
            iFlags &= ~EButtonsCallOnHold;    
            }            
        
        if ( iFlags & EButtonsCallOnHold )
            {
            iTouchPane.SetButtonState( EPhoneInCallCmdUnhold );
            }
        else
            {
            iTouchPane.SetButtonState( EPhoneInCallCmdHold );
            }            
        }
    }

// ---------------------------------------------------------------------------
// SetIhfFlag
// ---------------------------------------------------------------------------
//
void CPhoneButtonsController::SetIhfFlag(
    TPhoneCommandParam* aCommandParam )
    {
    if ( aCommandParam->ParamId() == TPhoneCommandParam::EPhoneParamIdBoolean )
        {
        TPhoneCmdParamBoolean* booleanParam = 
            static_cast<TPhoneCmdParamBoolean*>( aCommandParam );
            
        if ( booleanParam->Boolean() )
            {
            iFlags |= EButtonsIhfActive;     
            }
        else
            {
            iFlags &= ~EButtonsIhfActive;    
            }            
         }    
    }
 
// ---------------------------------------------------------
// CPhoneButtonsController::SetBTAccFlag
// ---------------------------------------------------------
//
void CPhoneButtonsController::SetBTAccFlag( TPhoneCommandParam* aCommandParam )
    {
    if ( aCommandParam->ParamId() == TPhoneCommandParam::EPhoneParamIdBoolean )
        {
        TPhoneCmdParamBoolean* booleanParam = 
            static_cast<TPhoneCmdParamBoolean*>( aCommandParam );
            
        if ( booleanParam->Boolean() )
            {
            iFlags |= EButtonsBTAcc;     
            }
        else
            {
            iFlags &= ~EButtonsBTAcc;    
            }            
        // Temporary unavailability
        if ( ( iFlags & EButtonsBTAccAvail ) && !( iFlags & EButtonsBTAcc ) )
            {
            iFlags |= EButtonsBTAccTmpUn;     
            }
        else
            {
            iFlags &= ~EButtonsBTAccTmpUn;  
            }      
         }     
    }
 
 // ---------------------------------------------------------
// CPhoneButtonsController::SetWiredAccFlag
// ---------------------------------------------------------
//
void CPhoneButtonsController::SetWiredAccFlag( TPhoneCommandParam* aCommandParam )
    {
    if ( aCommandParam->ParamId() == TPhoneCommandParam::EPhoneParamIdBoolean )
        {
        TPhoneCmdParamBoolean* booleanParam = 
            static_cast<TPhoneCmdParamBoolean*>( aCommandParam );
            
        if ( booleanParam->Boolean() )
            {
            iFlags |= EButtonsWiredAcc;     
            }
        else
            {
            iFlags &= ~EButtonsWiredAcc;    
            }            
        }     
    }

// ---------------------------------------------------------
// CPhoneButtonsController::SetBTAccAvailableFlag
// ---------------------------------------------------------
//
void CPhoneButtonsController::SetBTAccAvailableFlag( TPhoneCommandParam* aCommandParam )
    {
    if ( aCommandParam->ParamId() == TPhoneCommandParam::EPhoneParamIdBoolean )
        {
        TPhoneCmdParamBoolean* booleanParam = 
            static_cast<TPhoneCmdParamBoolean*>( aCommandParam );
           
        if ( booleanParam->Boolean() )
            {
            iFlags |= EButtonsBTAccAvail;
            }
        else
            {
            iFlags &= ~EButtonsBTAccAvail;    
            }
        
        // Temporary unavailability        
        if ( ( iFlags & EButtonsBTAccAvail ) && !( iFlags & EButtonsBTAcc ) )
            {
            iFlags |= EButtonsBTAccTmpUn;     
            }
        else
            {
            iFlags &= ~EButtonsBTAccTmpUn;  
            }            
        }
    }
// ---------------------------------------------------------------------------
//  SetVideoCallFlag
// ---------------------------------------------------------------------------
//
void CPhoneButtonsController::SetVideoCallFlag( 
    TPhoneCommandParam* aCommandParam )
    {
    if ( aCommandParam->ParamId() == TPhoneCommandParam::EPhoneParamIdBoolean )
        {
        TPhoneCmdParamBoolean* booleanParam = 
            static_cast<TPhoneCmdParamBoolean*>( aCommandParam );
            
        if ( booleanParam->Boolean() )
            {
            iFlags |= EButtonsVideoCallOngoing;     
            }
        else
            {
            iFlags &= ~EButtonsVideoCallOngoing;    
            }            
        
        if ( iFlags & EButtonsVideoCallOngoing )
            {
            iTouchPane.SetButtonState( EPhoneInCallCmdSwitchToVoice );
            iTouchPane.SetButtonDimmed( EPhoneInCallCmdHold, ETrue );    
            }
        else
            {
            iTouchPane.SetButtonState( EPhoneInCallCmdSwitchToVideo );
            iTouchPane.SetButtonDimmed( EPhoneInCallCmdHold, EFalse );
            }            
        }
    }
        

// ---------------------------------------------------------
// CPhoneButtonsController::SetInCallJoinEnabled
// ---------------------------------------------------------
//
void CPhoneButtonsController::SetInCallJoinEnabled( TPhoneCommandParam* aCommandParam )
    {
    if ( aCommandParam->ParamId() == TPhoneCommandParam::EPhoneParamIdBoolean )
        {
        TPhoneCmdParamBoolean* booleanParam = 
            static_cast<TPhoneCmdParamBoolean*>( aCommandParam );
         if ( booleanParam->Boolean() )
            {
            iFlags |= EButtonsDimJoin;     
            }
        else
            {
            iFlags &= ~EButtonsDimJoin;    
            }            
        }
    SetButtonDimming();     
    }

// ---------------------------------------------------------
// SetButtonEnabled
// ---------------------------------------------------------
// 
void CPhoneButtonsController::SetButtonEnabled( 
    TPhoneCommandParam* aCommandParam )
    {
    if ( aCommandParam->ParamId() == 
        TPhoneCommandParam::EPhoneParamIdInteger )
        { 
        TInt commandId =
            static_cast<TPhoneCmdParamInteger*>( aCommandParam )->Integer();
            
        switch ( commandId )
            {
            case EPhoneCallComingCmdAnswer:
                iFlags &= ~EButtonsDimAnswer;    
                break;
            case EPhoneInCallCmdPrivate:
                iFlags &= ~EButtonsDimPrivate;
                break;
            case EPhoneCallComingCmdSilent:
                iFlags &= ~EButtonsDimSilence;
                break;
            case EPhoneInCallCmdCreateConference:
                iFlags &= ~EButtonsDimCreateConference;
                break;
            default:
                break;                
            }

        SetButtonDimming(); 
        }    
    }

// ---------------------------------------------------------
// SetButtonDisabled
// ---------------------------------------------------------
//    
void CPhoneButtonsController::SetButtonDisabled(
    TPhoneCommandParam* aCommandParam )
    {
    if ( aCommandParam->ParamId() == 
        TPhoneCommandParam::EPhoneParamIdInteger )
        { 
        TInt commandId =
            static_cast<TPhoneCmdParamInteger*>( aCommandParam )->Integer();
            
        switch ( commandId )
            {
            case EPhoneCallComingCmdAnswer:
                iFlags |= EButtonsDimAnswer;    
                break;
            case EPhoneInCallCmdPrivate:
                iFlags |= EButtonsDimPrivate;
                break;                
            case EPhoneCallComingCmdSilent:
                iFlags |= EButtonsDimSilence;
                break;
           case EPhoneInCallCmdCreateConference:
                iFlags |= EButtonsDimCreateConference;
                break;
            default:
                break;                
            }

        SetButtonDimming(); 
        }
    }
 
// ---------------------------------------------------------------------------
// SetToggleButtons
// ---------------------------------------------------------------------------
//
void CPhoneButtonsController::SetToggleButtons()
    {
    TInt buttonCount = iTouchPane.NumberOfButtonsInPane();
    
    for ( TInt i=0; i < buttonCount ; i++ )
        {
        switch ( iTouchPane.ButtonCommandId( i ) )
            {
            case EPhoneInCallCmdMute:
                {
                if ( iFlags & EButtonsPhoneMuted )    
                    {
                    iTouchPane.SetButtonState( EPhoneInCallCmdUnmute );           
                    }
                }
                break;
                
            case EPhoneInCallCmdUnmute:
                {
                if ( !( iFlags & EButtonsPhoneMuted ) )   
                    {
                    iTouchPane.SetButtonState( EPhoneInCallCmdMute );           
                    }
                }
                break;
                
            case EPhoneInCallCmdHold:
                {
                if ( iFlags & EButtonsCallOnHold )    
                    {
                    iTouchPane.SetButtonState( EPhoneInCallCmdUnhold );           
                    }    
                }
                break;
                
            case EPhoneInCallCmdUnhold:
                {
                if ( !( iFlags & EButtonsCallOnHold ) )
                    {
                    iTouchPane.SetButtonState( EPhoneInCallCmdHold );           
                    }    
                }
                break;
                
            case EPhoneInCallCmdActivateIhf:
                {
                if ( iFlags & EButtonsIhfActive )
                    {
                    iTouchPane.SetButtonState( EPhoneInCallCmdDeactivateIhf );    
                    }
                }
                break;
                
            case EPhoneInCallCmdDeactivateIhf:
                {
                if ( !( iFlags & EButtonsIhfActive ) )
                    {
                    iTouchPane.SetButtonState( EPhoneInCallCmdActivateIhf );    
                    }
                }
                break;
                
            case EPhoneInCallCmdHandset:
                {
                if ( iFlags & EButtonsBTAccTmpUn )
                    {
                    iTouchPane.SetButtonState( EPhoneInCallCmdBtHandsfree );    
                    }
                }
                break;
            
            case EPhoneInCallCmdBtHandsfree:
                {
                if ( iFlags & EButtonsBTAcc )
                    {
                    iTouchPane.SetButtonState( EPhoneInCallCmdHandset );    
                    }
                }
                break;
                
            case EPhoneInCallCmdSwitchToVideo:
                {
                if ( iFlags & EButtonsVideoCallOngoing )    
                    {
                    iTouchPane.SetButtonState( EPhoneInCallCmdSwitchToVoice );           
                    }
                }
                break;    
                            
            case EPhoneInCallCmdSwitchToVoice:
                {
                if ( !( iFlags & EButtonsVideoCallOngoing ) )   
                    {
                    iTouchPane.SetButtonState( EPhoneInCallCmdSwitchToVideo );           
                    }
                }
                break;                         
                                                            
            default:
                break;                                                                                                
            }
        }
    }

// ---------------------------------------------------------------------------
// ReplaceAudioButton
// ---------------------------------------------------------------------------
//
void CPhoneButtonsController::ReplaceAudioButton()
    {
  
    TInt buttonCount = iTouchPane.NumberOfButtonsInPane();
    
    for ( TInt i=0; i < buttonCount; i++ )    
        {
        TInt commandId = iTouchPane.ButtonCommandId( i );
        
        if( commandId == EPhoneInCallCmdHandset ||
            commandId == EPhoneInCallCmdBtHandsfree )
            {
            // wired connect
            if ( iFlags & EButtonsWiredAcc )
                {
                // Bt disconnect when wired
                if ( !( iFlags & EButtonsBTAccTmpUn ) )
                    {
                    iTouchPane.ReplaceButton( i, 
                    CPhoneMainResourceResolver::Instance()->ResolveResourceID( EPhoneAudioDefButton ) );
                    commandId = iTouchPane.ButtonCommandId( i );
                    }
                iTouchPane.SetButtonDimmed( commandId, ETrue);                    
                }
            // BTA connect or temporarary unavailable
            else if ( iFlags & EButtonsBTAcc || iFlags & EButtonsBTAccTmpUn )
                {
                iTouchPane.SetButtonDimmed( commandId, EFalse );
                }
            else  
                {
                iTouchPane.ReplaceButton( i, 
                    CPhoneMainResourceResolver::Instance()->ResolveResourceID( EPhoneAudioDefButton ) );
                }
            }
         else if ( commandId == EPhoneInCallCmdActivateIhf ||
                   commandId == EPhoneInCallCmdDeactivateIhf )
            {
            // wired connect
            if ( iFlags & EButtonsWiredAcc )
                {
                // Bt connect when wired
                if ( iFlags & EButtonsBTAccTmpUn )
                    {
                    iTouchPane.ReplaceButton( i, 
                    CPhoneMainResourceResolver::Instance()->ResolveResourceID( EPhoneAudioBTAButton ) );
                    commandId = iTouchPane.ButtonCommandId( i );
                    }
                iTouchPane.SetButtonDimmed( commandId, ETrue);                    
                }
            // BTA connect or temporarary unavailable
            else if ( iFlags & EButtonsBTAcc || iFlags & EButtonsBTAccTmpUn )
                {
                iTouchPane.ReplaceButton( i, 
                    CPhoneMainResourceResolver::Instance()->ResolveResourceID( EPhoneAudioBTAButton ) );
                }
            else
                {
                iTouchPane.SetButtonDimmed( commandId, EFalse ); 
                }
            
            }
        }
    SetToggleButtons();        
    }

// ---------------------------------------------------------------------------
// SetButtonDimming
// ---------------------------------------------------------------------------
//
void CPhoneButtonsController::SetButtonDimming()
    {
    TInt buttonCount = iTouchPane.NumberOfButtonsInPane();
    
    for ( TInt i=0; i < buttonCount; i++ )    
        {
        TInt commandId = iTouchPane.ButtonCommandId( i );
    
        switch ( commandId )
            {
            case EPhoneCallComingCmdAnswer:
                {
                if ( iFlags & EButtonsDimAnswer )
                    {
                    iTouchPane.SetButtonDimmed( commandId, ETrue );
                    }
                else
                    {
                    iTouchPane.SetButtonDimmed( commandId, EFalse ); 
                    }    
                break;    
                }                
            case EPhoneInCallCmdPrivate:
                {
                if ( iFlags & EButtonsDimPrivate )
                    {
                    iTouchPane.SetButtonDimmed( commandId, ETrue );
                    }
                else
                    {
                    iTouchPane.SetButtonDimmed( commandId, EFalse ); 
                    }
                break;    
                }
            case EPhoneInCallCmdJoin:
                {
                if ( iFlags & EButtonsDimJoin )
                    {
                    iTouchPane.SetButtonDimmed( commandId, ETrue );
                    }
                else
                    {
                    iTouchPane.SetButtonDimmed( commandId, EFalse ); 
                    }
                break;    
                }
            case EPhoneInCallCmdHold:
                {
                if ( iFlags & EButtonsVideoCallOngoing )
                    {
                    iTouchPane.SetButtonDimmed( commandId, ETrue );
                    }
                else
                    {
                    iTouchPane.SetButtonDimmed( commandId, EFalse ); 
                    }
                break;    
                }
            case EPhoneCallComingCmdSilent:
                {
                if ( iFlags & EButtonsDimSilence )
                    {
                    iTouchPane.SetButtonDimmed( commandId, ETrue );
                    }
                else
                    {
                    iTouchPane.SetButtonDimmed( commandId, EFalse ); 
                    }
                break;    
                }                
            case EPhoneInCallCmdCreateConference:
                {
                if ( iFlags & EButtonsDimCreateConference )
                    {
                    iTouchPane.SetButtonDimmed( commandId, ETrue );
                    }
                else
                    {
                    iTouchPane.SetButtonDimmed( commandId, EFalse ); 
                    }
                break;  
                }
            default:
                break;                    
            } // switch
        } // for

    if ( iCustomization )
        {
        // Customizes touch pane button.
        iCustomization->CustomizeTouchPaneButtons( iTouchPane );
        }
    }

// -----------------------------------------------------------
// CPhoneButtonsController::SetCustomization
// -----------------------------------------------------------
//
void CPhoneButtonsController::SetCustomization(
        MPhoneButtonCustomization* aCustomization )
    {
    iCustomization = aCustomization;
    }
  
// ---------------------------------------------------------------------------
// GetIconForCommandL
// ---------------------------------------------------------------------------
//
CGulIcon* CPhoneButtonsController::GetIconForCommandL( TInt aCommandId )
    {
    TAknsItemID skinId( KAknsIIDNone );
    TInt fileIndex( 0 );
    TInt maskIndex(0);
    switch ( aCommandId )
        {
        case EPhoneInCallCmdMute:
            {
            skinId =KAknsIIDQgnIndiButtonMuteMic; 
            fileIndex = EMbmPhoneuiQgn_indi_button_mute_mic;
            maskIndex = EMbmPhoneuiQgn_indi_button_mute_mic_mask;                     
            break;
            }
        case EPhoneInCallCmdUnmute:
            {
            skinId = KAknsIIDQgnIndiButtonUnmuteMic; 
            fileIndex = EMbmPhoneuiQgn_indi_button_unmute_mic;
            maskIndex = EMbmPhoneuiQgn_indi_button_unmute_mic_mask;     
            break;     
            }
        case EPhoneInCallCmdEndAllCalls:
            {
            skinId = KAknsIIDQgnIndiButtonEndAllCalls; 
            fileIndex = EMbmPhoneuiQgn_indi_button_end_all_calls; 
            maskIndex =  EMbmPhoneuiQgn_indi_button_end_all_calls_mask;   
            break;           
            }
        case EPhoneInCallCmdHold:
            {
            skinId = KAknsIIDQgnIndiButtonHold; 
            fileIndex = EMbmPhoneuiQgn_indi_button_hold; 
            maskIndex = EMbmPhoneuiQgn_indi_button_hold_mask;               
            break;            
            }
        case EPhoneInCallCmdUnhold:
            {
            skinId = KAknsIIDQgnIndiButtonUnhold; 
            fileIndex = EMbmPhoneuiQgn_indi_button_unhold; 
            maskIndex = EMbmPhoneuiQgn_indi_button_unhold_mask;                 
            break;          
            }
        case EPhoneInCallCmdActivateIhf:
            {
            skinId = KAknsIIDQgnIndiButtonLoudspeaker;
            fileIndex = EMbmPhoneuiQgn_indi_button_loudspeaker;
            maskIndex = EMbmPhoneuiQgn_indi_button_loudspeaker_mask;                
            break;          
            }
        case EPhoneInCallCmdDeactivateIhf:
            {
            skinId = KAknsIIDQgnIndiButtonHandset;   
            fileIndex = EMbmPhoneuiQgn_indi_button_handset;
            maskIndex = EMbmPhoneuiQgn_indi_button_handset_mask;                
            break;           
            }
        case EPhoneInCallCmdHandset:
            {
            skinId = KAknsIIDQgnIndiButtonHandset;   
            fileIndex = EMbmPhoneuiQgn_indi_button_handset;
            maskIndex = EMbmPhoneuiQgn_indi_button_handset_mask;                 
            break;
            }
        case EPhoneInCallCmdBtHandsfree:
            {
            skinId = KAknsIIDQgnIndiButtonBluetooth;    
            fileIndex = EMbmPhoneuiQgn_indi_button_bluetooth;
            maskIndex = EMbmPhoneuiQgn_indi_button_bluetooth_mask;                 
            break;         
            }
        case EPhoneInCallCmdSwap:
            {
            skinId = KAknsIIDQgnIndiButtonSwap;  
            fileIndex = EMbmPhoneuiQgn_indi_button_swap;
            maskIndex = EMbmPhoneuiQgn_indi_button_swap_mask;                
            break;
            }
        case EPhoneInCallCmdGoToIdle:
            {
            skinId = KAknsIIDQgnIndiButtonGoIdle; 
            fileIndex = EMbmPhoneuiQgn_indi_button_go_idle;
            maskIndex = EMbmPhoneuiQgn_indi_button_go_idle_mask;                
            break;           
            }
        case EPhoneInCallCmdCreateConference:
            {
            skinId = KAknsIIDQgnIndiButtonConference; 
            fileIndex = EMbmPhoneuiQgn_indi_button_conference;
            maskIndex = EMbmPhoneuiQgn_indi_button_conference_mask;                
            break;             
            }
        case EPhoneInCallCmdParticipants:
            {
            skinId = KAknsIIDQgnIndiButtonParticipants; 
            fileIndex = EMbmPhoneuiQgn_indi_button_participants; 
            maskIndex = EMbmPhoneuiQgn_indi_button_participants_mask;                
            break;
            }
        case EPhoneInCallCmdDrop:
            {
            skinId = KAknsIIDQgnIndiButtonDrop; 
            fileIndex = EMbmPhoneuiQgn_indi_button_drop;
            maskIndex = EMbmPhoneuiQgn_indi_button_drop_mask;                 
            break;
            }
        case EPhoneInCallCmdPrivate:
            {
            skinId = KAknsIIDQgnIndiButtonPrivate; 
            fileIndex = EMbmPhoneuiQgn_indi_button_private; 
            maskIndex = EMbmPhoneuiQgn_indi_button_private_mask;                 
            break;
            }
        case EPhoneInCallCmdJoin:
            {
            skinId = KAknsIIDQgnIndiButtonJoin;  
            fileIndex = EMbmPhoneuiQgn_indi_button_join;
            maskIndex = EMbmPhoneuiQgn_indi_button_join_mask;                 
            break;          
            }
        case EPhoneInCallCmdEndThisOutgoingCall: 
            {
            skinId = KAknsIIDQgnIndiButtonEndActiveCall;
            fileIndex = EMbmPhoneuiQgn_indi_button_end_active_call;
            maskIndex = EMbmPhoneuiQgn_indi_button_end_active_call_mask;
            break;
            }
        case EPhoneCmdEnd:
            {
            skinId = KAknsIIDQgnIndiButtonEndCall;
            fileIndex = EMbmPhoneuiQgn_indi_button_end_active_call;
            maskIndex = EMbmPhoneuiQgn_indi_button_end_active_call_mask;
            break;
            }
        case EPhoneCallComingCmdAnswer:
            {
            skinId = KAknsIIDQgnIndiButtonAnswer;
            fileIndex = EMbmPhoneuiQgn_indi_button_answer;
            maskIndex = EMbmPhoneuiQgn_indi_button_answer_mask;
            break;
            }
        case EPhoneCallComingCmdReject:
            {
            skinId = KAknsIIDQgnIndiButtonReject;
            fileIndex = EMbmPhoneuiQgn_indi_button_reject;
            maskIndex = EMbmPhoneuiQgn_indi_button_reject_mask;
            break;
            }
        case EPhoneInCallCmdSwitchToVideo:
            {
            skinId = KAknsIIDQgnIndiButtonVoiceToVideo;
            fileIndex = EMbmPhoneuiQgn_indi_button_voice_to_video;
            maskIndex = EMbmPhoneuiQgn_indi_button_voice_to_video_mask;
            break;
            }            
        case EPhoneInCallCmdSwitchToVoice:
            {
            skinId = KAknsIIDQgnIndiButtonVideoToVoice;
            fileIndex = EMbmPhoneuiQgn_indi_button_video_to_voice;
            maskIndex = EMbmPhoneuiQgn_indi_button_video_to_voice_mask;
            break;
            }              
        case EPhoneCallComingCmdSilent:
            {
            skinId = KAknsIIDQgnIndiButtonSilence;
            fileIndex = EMbmPhoneuiQgn_indi_button_silence;
            maskIndex = EMbmPhoneuiQgn_indi_button_silence_mask;
            break;
            }  

        default:
            return NULL; 
        } // switch
        
    CGulIcon* icon = NULL;
    if ( skinId != KAknsIIDNone )
        {
        TParse* fp = new( ELeave ) TParse();
        CleanupStack::PushL( fp );
        TInt err = fp->Set(KPhoneUiScalableIconFile, &KDC_APP_BITMAP_DIR, NULL); 

        if (err != KErrNone)
            {
            User::Leave(err);
            }

        TBuf<KMaxFileName> fileName = fp->FullName();
        CleanupStack::PopAndDestroy();  //fp
            
        MAknsSkinInstance* skin = AknsUtils::SkinInstance();
        
        icon = AknsUtils::CreateGulIconL( skin, 
                                          skinId, 
                                          fileName, 
                                          fileIndex,
                                          maskIndex );
        }

    return icon;  
    }
    

// ---------------------------------------------------------------------------
// GetButtonForCommandL
// ---------------------------------------------------------------------------
//
TAknsItemID CPhoneButtonsController::GetButtonForCommandL( TInt aCommandId )
    {
    TAknsItemID frameId( KAknsIIDNone );
    switch ( aCommandId )
        {
        case EPhoneInCallCmdEndAllCalls:
            {
            frameId = KAknsIIDQsnFrButtonNormalReject; 
            break;           
            }
        case EPhoneInCallCmdHold:
            {
            frameId = KAknsIIDQsnFrButtonNormalHold; 
            break;            
            }
        case EPhoneInCallCmdUnhold:
            {
            frameId = KAknsIIDQsnFrButtonNormalHold; 
            break;          
            }
        case EPhoneInCallCmdEndThisOutgoingCall: 
            {
            frameId = KAknsIIDQsnFrButtonNormalReject;
            break;
            }
        case EPhoneCmdEnd:
            {
            frameId = KAknsIIDQsnFrButtonNormalReject;
            break;
            }
        case EPhoneCallComingCmdAnswer:
            {
            frameId = KAknsIIDQsnFrButtonNormalAnswer;
            break;
            }
        case EPhoneCallComingCmdReject:
            {
            frameId = KAknsIIDQsnFrButtonNormalReject;
            break;
            }

        default:
            frameId= KAknsIIDQsnFrButtonNormal;
            break; 
        } // switch
        
    return frameId;  
    }


// END
