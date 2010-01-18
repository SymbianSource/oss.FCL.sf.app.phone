/*
* Copyright (c) 2005-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Implementation of CPhoneMenuController class.
*
*/


// INCLUDE FILES
#include "cphonemenucontroller.h"
#include "tphonecommandparam.h"
#include "tphonecmdparaminteger.h"
#include "tphonecmdparamdynmenu.h"
#include "tphonecmdparamboolean.h"
#include "phoneconstants.h"
#include "phonelogger.h"
#include "tphonecmdparamincallindicatordata.h"
#include "cphonepubsubproxy.h"
#include "cphonecenrepproxy.h"


#include <eikenv.h>
#include <eikappui.h>
#include <eikapp.h>
#include <eikmenub.h> 
#include <eikspane.h> 
#include <featmgr.h>
#include <ctsydomainpskeys.h>
#include "phoneui.hrh"
#include <phoneappvoipcommands.hrh>
#include <phoneui.rsg>
#include <telephonyvariant.hrh>

#include <hwrmdomainpskeys.h>

#include <AiwServiceHandler.h>

// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CPhoneMenuController::CPhoneMenuController
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPhoneMenuController::CPhoneMenuController( CEikonEnv& aEikEnv ): 
    iMenuResourceId( 0 ),
    iOkMenuResourceId( 0 ),
    iEikEnv( aEikEnv )
    {
    }

// -----------------------------------------------------------------------------
// CPhoneMenuController::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPhoneMenuController::ConstructL()
    {
    iMenu = iEikEnv.AppUiFactory()->MenuBar();
        
    if ( FeatureManager::FeatureSupported( KFeatureIdMultimediaSharing ) )
        {
        __PHONELOG( EBasic, EPhoneUIView, "CPhoneViewController::CPhoneMenuController -> Going to instantiate CAiwServiceHandler");
        //If multimedia sharing is supported, initialise instance to AIW handler
        //and attach menu interest.        
        iServiceHandler = CAiwServiceHandler::NewL();  
        
         // Attach menu service interests to AIW framework.
        iServiceHandler->AttachMenuL( R_PHONEUI_CALLHANDLING_INCALL_OPTIONS_MENU,
            R_PHONEUI_AIWSHARE_INTEREST );
        __PHONELOG( EBasic, EPhoneUIView, "CPhoneViewController::CPhoneMenuController -> AIW Menu attached");       
        }
    
    CPhonePubSubProxy::Instance()->NotifyChangeL(
                                         KPSUidHWRM, KHWRMGripStatus, this );
    }

// -----------------------------------------------------------------------------
// CPhoneMenuController::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPhoneMenuController* CPhoneMenuController::NewL( CEikonEnv& aEikEnv )
    {   
    CPhoneMenuController* self = new( ELeave ) CPhoneMenuController( aEikEnv );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    
    return self;
    }

// Destructor
CPhoneMenuController::~CPhoneMenuController()
    {
    delete iServiceHandler;      
    }

// ---------------------------------------------------------
// CPhoneMenuController::CloseMenuBar()
// ---------------------------------------------------------
//
void CPhoneMenuController::CloseMenuBar()
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneMenuController::CloseMenuBar()" );
    iMenu->StopDisplayingMenuBar();
    }

// ---------------------------------------------------------
// CPhoneMenuController::OpenMenuBar()
// ---------------------------------------------------------
//
void CPhoneMenuController::OpenMenuBarL()
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneMenuController::OpenMenuBarL()" );
    iMenu->SetMenuTitleResourceId( iMenuResourceId );
    iMenu->TryDisplayMenuBarL();
    }

// ---------------------------------------------------------
// CPhoneMenuController::SetMenubarL
// Set the Menubar to the specified setting
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPhoneMenuController::OpenMenuBarL( TPhoneCommandParam* aCommandParam )
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneMenuController::OpenMenuBarL()" );
    // Check is the given parameter valid
    if ( aCommandParam->ParamId() == TPhoneCommandParam::EPhoneParamIdInteger )
        {
        TPhoneCmdParamInteger* menuResource = 
            static_cast<TPhoneCmdParamInteger*>( aCommandParam );

        iMenuResourceId = menuResource->Integer();
        ShowTaskSwapper( iMenuResourceId );

        iMenu->SetMenuTitleResourceId( iMenuResourceId );
        iMenu->TryDisplayMenuBarL();
        }
    }

// -----------------------------------------------------------------------------
// CPhoneMenuController::IsSwivelClosed
// This function checks from PubSubProxy that is
// Swivel closed or not in product.
// 
// -----------------------------------------------------------------------------
//
TBool CPhoneMenuController::IsSwivelClosed() const
    {
    if( CPhoneCenRepProxy::Instance()->
            IsTelephonyFeatureSupported( KTelephonyLVFlagSwivelInDevice ))
        {
        return CPhonePubSubProxy::Instance()->Value(
                                         KPSUidHWRM, KHWRMGripStatus ) == EPSHWRMGripClosed;        
        }
    else
        {
        return EFalse;
        }

    } 

// ---------------------------------------------------------
// CPhoneMenuController::DynInitMenuBar
// ---------------------------------------------------------
//
void CPhoneMenuController::DynInitMenuBar( 
    TPhoneCommandParam* /*aCommandParam*/ )
    {
    }

// ---------------------------------------------------------
// CPhoneMenuController::DynInitMenuPaneL
// ---------------------------------------------------------
//
void CPhoneMenuController::DynInitMenuPaneL( TPhoneCommandParam* aCommandParam )
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneMenuController::DynInitMenuPaneL()" );
    TPhoneCmdParamDynMenu* menu = 
        static_cast<TPhoneCmdParamDynMenu*>( aCommandParam );

    TInt resourceId = menu->ResourceId();
    
    __PHONELOG1( 
        EBasic, 
        EPhonePhoneapp, 
        "CPhoneMenuController::DynInitMenuPaneL() - ResourceId = %d)", 
        resourceId );
 
    CEikMenuPane*  menuPane= 
        reinterpret_cast<CEikMenuPane*>( menu->DynMenu() );
 
    RArray<TInt> menuItemArray;
    CleanupClosePushL( menuItemArray );

    TInt menuItemCount = menuPane->NumberOfItemsInPane();
    __PHONELOG1( 
        EBasic, 
        EPhonePhoneapp, 
        "CPhoneMenuController::DynInitMenuPaneL() - MenuItemCount = %d)", 
        menuItemCount );
 
    // Menu items to array
    for ( TInt i( 0 ); i < menuItemCount; i++ )
        {
        menuItemArray.Append( menuPane->MenuItemCommandId( i ) );
        }
    TBool onScreenDialer = FeatureManager::FeatureSupported( 
                                    KFeatureIdOnScreenDialer );  
                                           
    // Menu items to be deleted 
    for ( TInt i( 0 ); i < menuItemArray.Count(); i++ )
        {
        switch( menuItemArray[ i ] )
            {
            case EPhoneInCallCmdDialer:
                if ( !onScreenDialer )
                    {
                    menuPane->DeleteMenuItem( menuItemArray[ i ] );    
                    }
                break;
            case EPhoneInCallCmdLockKeypad:
                if ( onScreenDialer || iHoldFlag )
                    {
                    menuPane->DeleteMenuItem( menuItemArray[ i ] );     
                    }
                break;
            case EPhoneInCallCmdLockScreen:
                if ( !onScreenDialer || iHoldFlag )
                    {
                    menuPane->DeleteMenuItem( menuItemArray[ i ] );     
                    }
                break;
            case EPhoneNumberAcqCmdCall:
                if ( onScreenDialer && iNumberEntryEmpty )
                    {
                    menuPane->DeleteMenuItem( menuItemArray[ i ] ); 
                    }
                else if ( iServiceCodeFlag )
                    {
                    menuPane->DeleteMenuItem( menuItemArray[ i ] );
                    }
                break;
                
            case EPhoneNumberAcqCmdVideoCall:
                if ( iServiceCodeFlag && 
                     FeatureManager::FeatureSupported( 
                         KFeatureIdVideocallMenuVisibility ))
                    {
                    menuPane->DeleteMenuItem( menuItemArray[ i ] );
                    }
                else if( !( FeatureManager::FeatureSupported( 
                        KFeatureIdCsVideoTelephony ) ) )
                    {
                    menuPane->DeleteMenuItem( menuItemArray[ i ] );
                    }
                break;
 
            case EPhoneNumberAcqCmdSendMessage:            
                if ( iNumberEntryEmpty )
                    {
                    menuPane->DeleteMenuItem( menuItemArray[ i ] ); 
                    }
                break;                    
                                
            case EPhoneNumberAcqCmdSendCommand:
                if ( !iServiceCodeFlag || iNumberEntryEmpty )
                    {
                    menuPane->DeleteMenuItem( menuItemArray[ i ] );
                    }
                break;
                
            case EPhoneNumberAcqCmdToggleNeAlphaMode:
                if ( iAlphaMode )
                    {
                    menuPane->DeleteMenuItem( menuItemArray[ i ] );
                    }
                break;
                         
            case EPhoneNumberAcqCmdToggleNeNumericMode:
                if ( !iAlphaMode )
                    {
                    menuPane->DeleteMenuItem( menuItemArray[ i ] );
                    }
                break;
                
                
            case EPhoneInCallCmdGoToIdle:
                if ( iNEVisible )
                    {
                    menuPane->DeleteMenuItem( menuItemArray[ i ] );    
                    }
                break;
                
            case EPhoneInCallCmdMute:
                if ( iMuteFlag )
                    {
                    menuPane->DeleteMenuItem( menuItemArray[ i ] );
                    }
                break;
                
            case EPhoneInCallCmdUnmute:
                if ( !iMuteFlag )
                    {
                    menuPane->DeleteMenuItem( menuItemArray[ i ] );
                    }
                break;
                
            case EPhoneInCallCmdHold: // fall through
            case EPhoneInCallCmdConferenceHold:
                if ( iHoldFlag && !iConferenceAndSingleFlag )
                    {
                    menuPane->DeleteMenuItem( menuItemArray[ i ] );
                    }
                break;
                
            case EPhoneInCallCmdDtmfListQuery:
                if ( ( iHoldFlag && !iAllowDtmfOptionsFlag ) || iHideVideoCallDTMFVisibility )
                    {
                    menuPane->DeleteMenuItem( menuItemArray[ i ] );
                    }
                break;
                
            case EPhoneInCallCmdSwitchToVideo:
                if( !iPhoneNumberAvailableInPhoneEngine  ||
                    !( FeatureManager::FeatureSupported( 
                            KFeatureIdCsVideoTelephony ) ) )
                    {
                    menuPane->DeleteMenuItem( menuItemArray[ i ] );                    
                    }
                 break;
            case EPhoneInCallCmdSwitchToVoice:
                if( !iPhoneNumberAvailableInPhoneEngine )
                    {
                    menuPane->DeleteMenuItem( menuItemArray[ i ] );                    
                    }
                break;
                
            case EPhoneCallComingCmdSoftReject:
                if ( !iSoftRejectFlag )
                    {
                    menuPane->DeleteMenuItem( menuItemArray[ i ] );
                    }
                break;
                
            case EPhoneCallComingCmdAnswerHandPortableVideo:
                if ( iWiredAccAvailableFlag || IsSwivelClosed() )
                    {
                    menuPane->DeleteMenuItem( menuItemArray[ i ] );        
                    }
                break;   
                  
            case EPhoneCallComingCmdAnswerHandPortable:
                if ( !iBTAccAvailableFlag )
                    {
                    menuPane->DeleteMenuItem( menuItemArray[ i ] );
                    }
                 break;
                 
            case EPhoneInCallCmdConferenceUnhold: // fall through
            case EPhoneInCallCmdUnhold:
                if ( !iHoldFlag )
                    {
                    menuPane->DeleteMenuItem( menuItemArray[ i ] );
                    }
                break;
                
            case EPhoneInCallCmdCreateConference:
                // Remove conference submenu item if:
                // 1. Specific flag is set (calls are different type) OR
                // 2. We are in Conference And Single state where the conference call is on hold and there
                //    are maximum amount of participants in conference call (can't add single to the conference) OR
                // 3. Conference call is on hold and we are in some other conference state than Conference And Single
                if ( iRemoveConfrenceOptionsFlag || ( iHoldFlag && iConferenceAndSingleFlag && iMaxConfMembersFlag ) ||
                     ( iHoldFlag && !iConferenceAndSingleFlag ) )
                    {
                    menuPane->DeleteMenuItem( menuItemArray[ i ] );
                    }
                break;
                                
            case EPhoneInCallCmdJoin:
                 if( iMaxConfMembersFlag || iRemoveConfrenceOptionsFlag )
                    {
                    menuPane->DeleteMenuItem( menuItemArray[ i ] );
                    }
                break;
                
            case EPhoneInCallCmdReplace:
                if ( !iReplace )
                    {
                    menuPane->DeleteMenuItem( menuItemArray[ i ] );
                    }
                break;
                
            case EPhoneInCallCmdActivateIhf:
                if ( iWiredAccFlag || iIhfFlag )
                    {
                    menuPane->DeleteMenuItem( menuItemArray[ i ] );
                    }
                break;
                
            case EPhoneInCallCmdDeactivateIhf:
                if ( iWiredAccFlag || !iIhfFlag || IsSwivelClosed() )
                    {
                    menuPane->DeleteMenuItem( menuItemArray[ i ] );
                    }
                break;
                
            case EPhoneInCallCmdBtHandsfree:
                if ( iBTAccTmpUnFlag && !iWiredAccFlag  )
                    {
                    // None
                    }
                else
                    {
                    menuPane->DeleteMenuItem( menuItemArray[i] );
                    }    
                break;

            case EPhoneInCallCmdHandset:
                if ( iBTAccFlag && !iWiredAccFlag )
                    {
                    if ( IsSwivelClosed() )
                        {
                        menuPane->DeleteMenuItem( menuItemArray[i] );
                        }
                    }
                 else
                    {
                    menuPane->DeleteMenuItem( menuItemArray[i] );
                    }   
                break; 

            case EPhoneDialerCallHandling:
                {
                // no active call
                if ( onScreenDialer && !iCallInProgress )
                    {
                    menuPane->DeleteMenuItem( menuItemArray[i] );                    
                    }                
                }
                break;
            case EPhoneDialerCmdLog:
                {
                if ( onScreenDialer && !iNumberEntryEmpty  )
                    {
                    menuPane->DeleteMenuItem( menuItemArray[i] );                    
                    }                
                }
                break;
            case EPhoneDialerCmdContacts:
                {
                if ( onScreenDialer && !iNumberEntryEmpty   )
                    {
                    menuPane->DeleteMenuItem( menuItemArray[i] );                    
                    }                
                }
                break;

            case EPhoneNumberAcqCmdAddToContacts:
                {
                if ( onScreenDialer && iNumberEntryEmpty   )
                    {
                    menuPane->DeleteMenuItem( menuItemArray[i] );                    
                    }                
                }
                break;
            case EPhoneDialerCmdSpeedDial:
                {
                if ( onScreenDialer && !iNumberEntryEmpty )
                    {
                    menuPane->DeleteMenuItem( menuItemArray[i] );                         
                    }
                }
                break;
            case EPhoneCmdDtmfOk:
                {
                if ( onScreenDialer && iNumberEntryEmpty )
                    {
                    menuPane->DeleteMenuItem( menuItemArray[i] );                     
                    }
                }
                break;
           case EPhoneInCallCmdPrivate:
                {
                if ( FeatureManager::FeatureSupported( KFeatureIdTouchCallHandling ) &&
                    iHoldFlag && 
                    iConferenceParticipantsListOpenFlag )
                    {
                    menuPane->DeleteMenuItem( menuItemArray[i] );                      
                    }
                }
                break;
            case EPhoneDialerCmdTouchInput:
                {
                if ( !FeatureManager::FeatureSupported( KFeatureIdCommonVoip ) )
                    {
                    menuPane->DeleteMenuItem( menuItemArray[i] );      
                    }
                }
                break;
            case EPhoneVideoSharing:
                {
                if( !FeatureManager::FeatureSupported( KFeatureIdMultimediaSharing ) )
                    {
                    menuPane->DeleteMenuItem( menuItemArray[i] );      
                    }
                }
                break;
                
            default:
                break;                                                                                                
             
            }
        }
    
    CleanupStack::PopAndDestroy(); // CleanupClosePushL
    
    if ( FeatureManager::FeatureSupported( KFeatureIdMultimediaSharing ) )
        {
        if( iServiceHandler->HandleSubmenuL( *menuPane ) )
            {
            // AIW handled this
            __PHONELOG( EBasic, EPhoneUIView, "CPhoneViewController::CPhoneMenuController -> AIW handled this sub menu");
            return;                
            }
            
        if( iServiceHandler->IsAiwMenu( resourceId ) )
            {
            __PHONELOG( EBasic, EPhoneUIView, "CPhoneViewController::CPhoneMenuController -> Resource is in AIW menu");
            iServiceHandler->InitializeMenuPaneL( 
                *menuPane, 
                resourceId, 
                EPhoneVideoSharing, 
                iServiceHandler->InParamListL() );                
            __PHONELOG( EBasic, EPhoneUIView, "CPhoneViewController::CPhoneMenuController -> AIW menu initialized");
            }        
        }
    }

// ---------------------------------------------------------
// CPhoneMenuController::AddMenuPaneL
// ---------------------------------------------------------
//
void CPhoneMenuController::AddMenuPaneL(
    TInt /*aPaneResourceId*/,
    CEikMenuPane* /*aMenuPane*/,
    TBool /*aAddSeparator*/ )
    { 
    }

// ---------------------------------------------------------
// CPhoneMenuController::SetMuteFlag
// ---------------------------------------------------------
//
void CPhoneMenuController::SetMuteFlag( TPhoneCommandParam* aCommandParam )
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneMenuController::SetMuteFlag()" );
    // Check is the given parameter valid
    if ( aCommandParam->ParamId() == TPhoneCommandParam::EPhoneParamIdBoolean )
        {
        TPhoneCmdParamBoolean* muteFlag = 
            static_cast<TPhoneCmdParamBoolean*>( aCommandParam );

        iMuteFlag = muteFlag->Boolean();
        }
    }

// ---------------------------------------------------------
// CPhoneMenuController::SetIhfFlag
// ---------------------------------------------------------
//
void CPhoneMenuController::SetIhfFlag( TPhoneCommandParam* aCommandParam )
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneMenuController::SetIhfFlag()" );
    // Check is the given parameter valid
    if ( aCommandParam->ParamId() == TPhoneCommandParam::EPhoneParamIdBoolean )
        {
        TPhoneCmdParamBoolean* ihfFlag = 
            static_cast<TPhoneCmdParamBoolean*>( aCommandParam );

        iIhfFlag = ihfFlag->Boolean();
        }
    }

// ---------------------------------------------------------
// CPhoneMenuController::SetHoldFlag
// ---------------------------------------------------------
//
void CPhoneMenuController::SetHoldFlag( TPhoneCommandParam* aCommandParam )
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneMenuController::SetHoldFlag()" );
    // Check is the given parameter valid
    if ( aCommandParam->ParamId() == TPhoneCommandParam::EPhoneParamIdBoolean )
        {
        TPhoneCmdParamBoolean* holdFlag = 
            static_cast<TPhoneCmdParamBoolean*>( aCommandParam );

        iHoldFlag = holdFlag->Boolean();
        }
    }
    
// ---------------------------------------------------------
// CPhoneMenuController::GetHoldFlag
// ---------------------------------------------------------
//
void CPhoneMenuController::GetHoldFlag( 
    TPhoneCommandParam* aCommandParam )
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneMenuController::GetHoldFlag()" );
    if ( aCommandParam->ParamId() == TPhoneCommandParam::EPhoneParamIdBoolean )
        {
        TPhoneCmdParamBoolean* booleanValue = 
            static_cast<TPhoneCmdParamBoolean*>( aCommandParam );
        booleanValue->SetBoolean( iHoldFlag );
        }
    }

// ---------------------------------------------------------
// CPhoneMenuController::SetConferenceAndSingleFlag
// ---------------------------------------------------------
//
void CPhoneMenuController::SetConferenceAndSingleFlag( TPhoneCommandParam* aCommandParam )
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneMenuController::SetConferenceAndSingleFlag()" );
    // Check is the given parameter valid
    if ( aCommandParam->ParamId() == TPhoneCommandParam::EPhoneParamIdBoolean )
        {
        TPhoneCmdParamBoolean* conferenceAndSingleFlag = 
            static_cast<TPhoneCmdParamBoolean*>( aCommandParam );

        iConferenceAndSingleFlag = conferenceAndSingleFlag->Boolean();
        }
    }
    
// ---------------------------------------------------------
// CPhoneMenuController::GetConferenceAndSingleFlag
// ---------------------------------------------------------
//
void CPhoneMenuController::GetConferenceAndSingleFlag( 
    TPhoneCommandParam* aCommandParam )
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneMenuController::GetConferenceAndSingleFlag()" );
    if ( aCommandParam->ParamId() == TPhoneCommandParam::EPhoneParamIdBoolean )
        {
        TPhoneCmdParamBoolean* booleanValue = 
            static_cast<TPhoneCmdParamBoolean*>( aCommandParam );
        booleanValue->SetBoolean( iConferenceAndSingleFlag );
        }
    }

// ---------------------------------------------------------
// CPhoneMenuController::SetSoftRejectFlag
// ---------------------------------------------------------
//
void CPhoneMenuController::SetSoftRejectFlag(
    TPhoneCommandParam* aCommandParam )
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneMenuController::SetSoftRejectFlag()" );
    // Check is the given parameter valid
    if ( aCommandParam->ParamId() == TPhoneCommandParam::EPhoneParamIdBoolean )
        {
        TPhoneCmdParamBoolean* softRejectFlag = 
            static_cast<TPhoneCmdParamBoolean*>( aCommandParam );

        iSoftRejectFlag = softRejectFlag->Boolean();
        }
    }
    
// ---------------------------------------------------------
// CPhoneMenuController::GetSoftRejectFlag
// ---------------------------------------------------------
//
void CPhoneMenuController::GetSoftRejectFlag(
    TPhoneCommandParam* aCommandParam )
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneMenuController::GetSoftRejectFlag()" );
    // Check is the given parameter valid
    if ( aCommandParam->ParamId() == TPhoneCommandParam::EPhoneParamIdBoolean )
        {
        TPhoneCmdParamBoolean* softRejectFlag = 
            static_cast<TPhoneCmdParamBoolean*>( aCommandParam );

        softRejectFlag->SetBoolean( iSoftRejectFlag );
        }
    }
    
// ---------------------------------------------------------
// CPhoneMenuController::SetServiceCodeFlag
// ---------------------------------------------------------
//
void CPhoneMenuController::SetServiceCodeFlag( 
    TPhoneCommandParam* aCommandParam )
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneMenuController::SetServiceCodeFlag()" );
    // Check is the given parameter valid
    if ( aCommandParam->ParamId() == TPhoneCommandParam::EPhoneParamIdBoolean )
        {
        TPhoneCmdParamBoolean* serviceCodeFlag = 
            static_cast<TPhoneCmdParamBoolean*>( aCommandParam );

        iServiceCodeFlag = serviceCodeFlag->Boolean();
        }
    }
    
// ---------------------------------------------------------
// CPhoneMenuController::SetWiredAccFlag
// ---------------------------------------------------------
//
void CPhoneMenuController::SetWiredAccFlag( TPhoneCommandParam* aCommandParam )
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneMenuController::SetWiredAccFlag()" );
    // Check is the given parameter valid
    if ( aCommandParam->ParamId() == TPhoneCommandParam::EPhoneParamIdBoolean )
        {
        TPhoneCmdParamBoolean* accFlag = 
            static_cast<TPhoneCmdParamBoolean*>( aCommandParam );

        iWiredAccFlag = accFlag->Boolean();
        }
    }

// ---------------------------------------------------------
// CPhoneMenuController::SetWiredAccAvailableFlag
// ---------------------------------------------------------
//
void CPhoneMenuController::SetWiredAccAvailableFlag( TPhoneCommandParam* aCommandParam )
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneMenuController::SetWiredAccAvailableFlag()" );
    // Check is the given parameter valid
    if ( aCommandParam->ParamId() == TPhoneCommandParam::EPhoneParamIdBoolean )
        {
        TPhoneCmdParamBoolean* accFlag = 
            static_cast<TPhoneCmdParamBoolean*>( aCommandParam );

        iWiredAccAvailableFlag = accFlag->Boolean();
        
        }
    }

// ---------------------------------------------------------
// CPhoneMenuController::SetMaxConfMembersFlag
// ---------------------------------------------------------
//
void CPhoneMenuController::SetMaxConfMembersFlag( TBool aIsMaxConfMembers )
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneMenuController::SetMaxConfMembersFlag()" );
    iMaxConfMembersFlag = aIsMaxConfMembers;
    }

// ---------------------------------------------------------
// CPhoneMenuController::SetBTAccFlag
// ---------------------------------------------------------
//
void CPhoneMenuController::SetBTAccFlag( TPhoneCommandParam* aCommandParam )
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneMenuController::SetBTAccFlag()" );
    // Check is the given parameter valid
    if ( aCommandParam->ParamId() == TPhoneCommandParam::EPhoneParamIdBoolean )
        {
        TPhoneCmdParamBoolean* accFlag = 
            static_cast<TPhoneCmdParamBoolean*>( aCommandParam );

        iBTAccFlag = accFlag->Boolean();
        iBTAccTmpUnFlag = iBTAccAvailableFlag && !iBTAccFlag;
        }
    }

// ---------------------------------------------------------
// CPhoneMenuController::SetBTAccAvailableFlag
// ---------------------------------------------------------
//
void CPhoneMenuController::SetBTAccAvailableFlag( TPhoneCommandParam* aCommandParam )
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneMenuController::SetBTAccAvailableFlag()" );
    // Check is the given parameter valid
    if ( aCommandParam->ParamId() == TPhoneCommandParam::EPhoneParamIdBoolean )
        {
        TPhoneCmdParamBoolean* accFlag = 
            static_cast<TPhoneCmdParamBoolean*>( aCommandParam );

        iBTAccAvailableFlag = accFlag->Boolean();
        iBTAccTmpUnFlag = iBTAccAvailableFlag && !iBTAccFlag;
        }
    }

// ---------------------------------------------------------
// CPhoneMenuController::SetNEVisibleFlag
// ---------------------------------------------------------
//
void CPhoneMenuController::SetNEVisibleFlag( TBool aVisible )
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneMenuController::SetNEVisibleFlag()" );
    iNEVisible = aVisible;
    }

// ---------------------------------------------------------
// CPhoneMenuController::SetReplaceFlag
// ---------------------------------------------------------
//
void CPhoneMenuController::SetReplaceFlag( TBool aReplace )
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneMenuController::SetReplaceFlag()" );
    iReplace = aReplace;
    }

// ---------------------------------------------------------
// CPhoneMenuController::SetPhoneNumberAvailableInPhoneEngine
// ---------------------------------------------------------
//
void CPhoneMenuController::SetPhoneNumberAvailableInPhoneEngine( 
    TPhoneCommandParam* aCommandParam )
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneMenuController::SetPhoneNumberAvailableInPhoneEngine()" );
    if ( aCommandParam->ParamId() == TPhoneCommandParam::EPhoneParamIdBoolean )
        {
        TPhoneCmdParamBoolean* phoneNumberAvailable = 
            static_cast<TPhoneCmdParamBoolean*>( aCommandParam );

        iPhoneNumberAvailableInPhoneEngine = phoneNumberAvailable->Boolean();
        }
    }

// -----------------------------------------------------------------------------
// CPhoneMenuController::ShowTaskSwapper
// 
// Decide if TaskSwapper is to be shown in menu.
// -----------------------------------------------------------------------------
//
void CPhoneMenuController::ShowTaskSwapper( TInt aResourceId ) const
    {
    if ( aResourceId == R_PHONEUI_NUMBERACQ_OK_MENUBAR ||
         aResourceId == R_PHONEUI_INCOMINGVIDEOCALL_MENUBAR || 
         aResourceId == R_PHONEUI_INCOMINGCALL_MENUBAR ||
         aResourceId == R_PHONEUI_INCOMINGCALL_MENUBAR_WITH_NUMBERENTRY ||
         aResourceId == R_PHONEUI_INCOMINGVIDEOCALL_MENUBAR_WITH_NUMBERENTRY ||
         aResourceId == R_PHONEUI_WAITING_LOCK_MENUBAR ||
         aResourceId == R_PHONEUI_TWOSINGLES_WAITING_LOCK_MENUBAR  )
        {
        iMenu->SetMenuType( CEikMenuBar::EMenuOptionsNoTaskSwapper );
        }
    else
        {
        iMenu->SetMenuType( CEikMenuBar::EMenuOptions );
        }
    }    

// ---------------------------------------------------------
// CPhoneMenuController::SendAiwCommandL
// ---------------------------------------------------------
//
void CPhoneMenuController::SendAiwCommandL( TPhoneCommandParam* aCommandParam )
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneMenuController::SendAiwCommandL()" );
    if( aCommandParam->ParamId() == TPhoneCommandParam::EPhoneParamIdInteger )
        {
        TPhoneCmdParamInteger* command =
            static_cast<TPhoneCmdParamInteger*>( aCommandParam );
        CAiwGenericParamList& in = iServiceHandler->InParamListL();
        iServiceHandler->ExecuteMenuCmdL( command->Integer(),
                                          in,
                                          iServiceHandler->OutParamListL() );                
        }
    }


// ---------------------------------------------------------------------------
// CPhoneMenuController::SetContextMenu
// ---------------------------------------------------------------------------
//
void CPhoneMenuController::SetContextMenu( TPhoneCommandParam* aCommandParam )
    {
    __LOGMETHODSTARTEND( EPhoneUIView, 
                         "CPhoneMenuController::SetContextMenu" );
    if( aCommandParam->ParamId() == TPhoneCommandParam::EPhoneParamIdInteger )
        {
        TPhoneCmdParamInteger* command =
               static_cast<TPhoneCmdParamInteger*>( aCommandParam );
        iMenu->SetContextMenuTitleResourceId( command->Integer() );
        
        }
    
    }

// ---------------------------------------------------------
// CPhoneMenuController::SetRemoveConfrenceOptionsFlag
// ---------------------------------------------------------
//
void CPhoneMenuController::SetRemoveConfrenceOptionsFlag( TBool aRemoveConfrenceOptions )
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneMenuController::SetRemoveConfrenceOptionsFlag()" );
    iRemoveConfrenceOptionsFlag = aRemoveConfrenceOptions;
    }
    
// ---------------------------------------------------------
// CPhoneMenuController::SetNumberEntryEmptyFlag
// ---------------------------------------------------------
//    
void CPhoneMenuController::SetNumberEntryEmptyFlag( TBool aNumberEntryEmpty )
    {
    iNumberEntryEmpty = aNumberEntryEmpty;
    }

// ---------------------------------------------------------
// CPhoneMenuController::SetCallInProgressFlag
// ---------------------------------------------------------
// 
void CPhoneMenuController::SetCallInProgressFlag( TPhoneCommandParam* aCommandParam )
    {
    TPhoneCmdParamIncallIndicatorData* param =
        static_cast<TPhoneCmdParamIncallIndicatorData*>( aCommandParam );

    TInt callState = param->CallState();    
    switch ( callState )
        {
        case EPSCTsyCallStateAlerting:
        case EPSCTsyCallStateRinging:
        case EPSCTsyCallStateDialling:
        case EPSCTsyCallStateAnswering:
        case EPSCTsyCallStateConnected:
        case EPSCTsyCallStateHold: 
            {
            //call in progress
            iCallInProgress = ETrue;
            break;
            }                 
        default:
            {
            iCallInProgress = EFalse;
             //call not in progress
            break;
            } 
        }
    }
    
// ---------------------------------------------------------
// CPhoneMenuController::IsMenuBarVisible()
// ---------------------------------------------------------
//
TBool CPhoneMenuController::IsMenuBarVisible()
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneMenuController::IsMenuBarVisible()" );
    return iMenu->IsDisplayed();
    }

// ---------------------------------------------------------
// CPhoneMenuController::SetConferenceParticipantsListVisibleFlag()
// ---------------------------------------------------------
//
void CPhoneMenuController::SetConferenceParticipantsListVisibleFlag( 
                                        TPhoneCommandParam* aCommandParam )
    {
    if ( aCommandParam->ParamId() == TPhoneCommandParam::EPhoneParamIdBoolean )
        {
        TPhoneCmdParamBoolean* participantsListOpenFlag = 
            static_cast<TPhoneCmdParamBoolean*>( aCommandParam );  
        iConferenceParticipantsListOpenFlag = participantsListOpenFlag->Boolean();
        }    
    }

//-------------------------------------------------------------------------------
// CPhoneMenuController::GetConferenceParticipantsListVisibleFlag
//-------------------------------------------------------------------------------
void CPhoneMenuController::GetConferenceParticipantsListVisibleFlag(
        TPhoneCommandParam* aCommandParam )
    {
    if ( aCommandParam->ParamId() == TPhoneCommandParam::EPhoneParamIdBoolean )
        {
        TPhoneCmdParamBoolean* participantsListOpenFlag = 
            static_cast<TPhoneCmdParamBoolean*>( aCommandParam );  
        participantsListOpenFlag->SetBoolean( iConferenceParticipantsListOpenFlag );
        } 
    }

// ---------------------------------------------------------
// CPhoneMenuController::SetAlphaModeFlag
// ---------------------------------------------------------
//
void CPhoneMenuController::SetAlphaModeFlag( TBool aMode )
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneMenuController::SetAlphaModeFlag()" );
    iAlphaMode = aMode;
    }

// ---------------------------------------------------------
// CPhoneMenuController::SetAllowDtmfOptionsFlag
// ---------------------------------------------------------
//
void CPhoneMenuController::SetAllowDtmfOptionsFlag( TBool aMode )
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneMenuController::SetAllowDtmfOptionsFlag()" );
    iAllowDtmfOptionsFlag = aMode;
    }

// ---------------------------------------------------------
// CPhoneMenuController::SetHideVideoCallDTMFVisibility
// ---------------------------------------------------------
//
void CPhoneMenuController::SetHideVideoCallDTMFVisibilityFlag( TBool aHideVideoCallDTMFVisibility )
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneMenuController::SetHideVideocallDTMFVisibility()" );
    iHideVideoCallDTMFVisibility = aHideVideoCallDTMFVisibility;
    }

// ---------------------------------------------------------
// CPhoneMenuController::SetVoipEnabledFlag
// ---------------------------------------------------------
//
void CPhoneMenuController::SetVoipEnabledFlag(  
                                        TPhoneCommandParam* aCommandParam )
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneMenuController::SetVoipEnabledFlag()" );
    
    if ( aCommandParam->ParamId() == TPhoneCommandParam::EPhoneParamIdBoolean )
        {
        TPhoneCmdParamBoolean* voipEnabled = 
            static_cast<TPhoneCmdParamBoolean*>( aCommandParam );  
        iVoipEnabledFlag = voipEnabled->Boolean();
        }
    }

// ---------------------------------------------------------
// CPhoneMenuController::HandlePropertyChangedL
// ---------------------------------------------------------
//
void CPhoneMenuController::HandlePropertyChangedL( 
        const TUid& aCategory,
        const TUint aKey,
        const TInt aValue )
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneMenuController::HandlePropertyChangedL()" );
    __PHONELOG1( EBasic, EPhoneUIView, 
            "CPhoneMenuController::HandlePropertyChangedL - aCategory= %d", aCategory  );      
    __PHONELOG1( EBasic, EPhoneUIView, 
            "CPhoneMenuController::HandlePropertyChangedL - aKey= %d", aKey  );  
    __PHONELOG1( EBasic, EPhoneUIView, 
            "CPhoneMenuController::HandlePropertyChangedL - aValue= %d", aValue  );
    if ( aCategory == KPSUidHWRM && 
         aKey == KHWRMGripStatus && 
         aValue != EPSHWRMGripStatusUninitialized && 
         iMenu->IsDisplayed()  )
        {
        CloseMenuBar();
        }
    }

     
// End of File
