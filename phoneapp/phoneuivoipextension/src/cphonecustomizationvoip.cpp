/*
* Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Implementation of CPhoneCustomizationVoip class.
*
*/


// INCLUDES
#include <eikmenup.h>
#include <spsettings.h>
#include <spentry.h>
#include <spproperty.h>
#include <mpeengineinfo.h>
#include <bmbubblemanager.h>
#include <phoneui.rsg>
#include <avkon.rsg>
#include <phoneuivoip.rsg>
#include <StringLoader.h>
#include <featmgr.h>
#include <centralrepository.h>
#include <easydialingcrkeys.h>

#include "cphonecustomizationvoip.h"
#include "tphonecmdparaminteger.h"
#include "tphonecmdparamboolean.h"
#include "tphonecmdparamstring.h"
#include "cphonemainresourceresolver.h"
#include "mphonestatemachine.h"
#include "mphonesecuritymodeobserver.h"
#include "cphonelogger.h"
#include "cphonecenrepproxy.h"
#include "phoneui.pan"
#include "mphoneviewcommandhandle.h"
#include "tphonecmdparamdynmenu.h"
#include "phoneappvoipcommands.hrh"
#include "phonerssvoip.h"
#include "tphonecmdparamcallstatedata.h"
#include "tphonecmdparamquery.h"
#include "tphonecmdparamcallheaderdata.h"
#include "cphoneextensionhandler.h"
#include "cphonestate.h"
#include "tphonecmdparamglobalnote.h"
#include "cphonestatemachinevoip.h"
#include "cphonevoiperrormessageshandler.h"
#include "cphoneviewcustomizationvoip.h"
#include "cphonestateutilsvoip.h"
#include "cphoneunattendedtransfercmdhandler.h"
#include "tphonecmdparampointer.h"
#include "cphonenewcallcmdhandler.h"
#include "cphonevcchandler.h"
#include "cphonecallforwardqueryhandler.h"
#include "cphonekeys.h"
#include "phoneui.hrh"

// CONSTANTS
const TInt KMaxLengthForSIPURIFirstLine = 15;
const TInt KMaxLengthForSIPURITwoLines = 30;
const TInt KMaxLengthForSIPURI = 256;

_LIT( KVoipAt, "@" );

// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//
CPhoneCustomizationVoip::CPhoneCustomizationVoip( 
    MPhoneStateMachine& aStateMachine,
    MPhoneViewCommandHandle& aViewCommandHandle ) : 
    iCoeEnv( *CCoeEnv::Static() ),
    iStateMachine( aStateMachine ),
    iViewCommandHandle( aViewCommandHandle )
    {
    }


// -----------------------------------------------------------
// CPhoneCustomizationVoip::~CPhoneCustomizationVoip()
// Destructor
// -----------------------------------------------------------
//
CPhoneCustomizationVoip::~CPhoneCustomizationVoip()
    {
    delete iCallForwardHandler;
    delete iExtensionHandler;
    delete iPhoneViewCustomization;
    delete iTransferCmdHandler;
    delete iNewCallCmdHandler;
	delete iVccHandler;
    }


// -----------------------------------------------------------
// CPhoneCustomizationVoip::ConstructL()
// Constructor
// -----------------------------------------------------------
//
void CPhoneCustomizationVoip::ConstructL()
    {
    // Set view customization
    iPhoneViewCustomization = CPhoneViewCustomizationVoip::NewL( *this );
    TPhoneCmdParamPointer pointerParam;
    pointerParam.SetPointer( 
        static_cast<MPhoneViewCustomization*>( iPhoneViewCustomization ) );
    iViewCommandHandle.ExecuteCommand( EPhoneViewSetViewCustomization, 
        &pointerParam );
    // Set touch button customization
    pointerParam.SetPointer( 
        static_cast<MPhoneButtonCustomization*>( this ) );
    iViewCommandHandle.ExecuteCommand( EPhoneViewSetButtonCustomization, 
        &pointerParam );
    
    // Set phone customization
    pointerParam.SetPointer( static_cast<MPhoneCustomization*>( this ) );
    iViewCommandHandle.ExecuteCommand( EPhoneViewSetPhoneCustomization, 
        &pointerParam );
    
    iExtensionHandler = CPhoneExtensionHandler::NewL();
    iTransferCmdHandler = CPhoneUnattendedTransferCmdHandler::NewL( 
        iStateMachine, iViewCommandHandle );
        
    iNewCallCmdHandler = CPhoneNewCallCmdHandler::NewL( 
        iStateMachine, iViewCommandHandle );
		
    if ( FeatureManager::FeatureSupported( KFeatureIdFfVoiceCallContinuity ) )
        {
        iVccHandler = CPhoneVccHandler::NewL( iStateMachine, iViewCommandHandle );
        }
    
    iCallForwardHandler = CPhoneCallForwardQueryHandler::NewL( 
        iStateMachine, iViewCommandHandle );
    }


// -----------------------------------------------------------
// CPhoneCustomizationVoip::NewL()
// Constructor
// -----------------------------------------------------------
//
CPhoneCustomizationVoip* CPhoneCustomizationVoip::NewL( 
    MPhoneStateMachine& aStateMachine,
    MPhoneViewCommandHandle& aViewCommandHandle )
    {
    CPhoneCustomizationVoip* self = new (ELeave) CPhoneCustomizationVoip( 
        aStateMachine,
        aViewCommandHandle );
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    
    return self;
    }


// -----------------------------------------------------------
// From class MPhoneCustomization.
// CPhoneCustomizationVoip::CustomizeMenuBarL
// -----------------------------------------------------------
// 
void CPhoneCustomizationVoip::CustomizeMenuBarL(
        TInt aResourceId, CEikMenuBar* aMenuBar )
    {
    __LOGMETHODSTARTEND( PhoneUIVoIPExtension, 
        "CPhoneCustomizationVoip::CustomizeMenuBarL" );
    
    if ( 0 == aResourceId || !aMenuBar )
        {
        __ASSERT_DEBUG( EFalse, Panic( EPhoneCtrlParameterNotInitialized ) );
        }
    }


// -----------------------------------------------------------
// From class MPhoneCustomization.
// CPhoneCustomizationVoip::CustomizeMenuPaneL
// -----------------------------------------------------------
//
void CPhoneCustomizationVoip::CustomizeMenuPaneL(
    TInt aResourceId, CEikMenuPane* aMenuPane)
    {
    __LOGMETHODSTARTEND( PhoneUIVoIPExtension, 
          "CPhoneCustomizationVoip::CustomizeMenuPaneL" );
    
    CEikMenuPane* menuPane = aMenuPane;
    RArray<TInt> menuItemArray;
    CleanupClosePushL( menuItemArray );

    // Menu items to array
    TInt menuItemCount = menuPane->NumberOfItemsInPane();
    for ( TInt i( 0 ); i < menuItemCount; i++ )
        {
        menuItemArray.Append( menuPane->MenuItemCommandId( i ) );
        }

    // Menu items to be deleted 
    for ( TInt i( 0 ); i < menuItemArray.Count(); i++ )
        {
        switch( menuItemArray[ i ] )
            {
            case EPhoneDialerCmdTouchInput:
                {
                if ( !VoIPSupportedL() || iStateMachine.SecurityMode()->IsSecurityMode() )
                    {
                    menuPane->DeleteMenuItem( menuItemArray[ i ] ); 
                    }
                break;
                }
            case EPhoneInCallCmdTransfer:
                {
                if ( ! IsFeatureSupported( EFeatureTransfer ) )
                    {
                    menuPane->DeleteMenuItem( menuItemArray[ i ] ); 
                    }
                break;
                }
            case EPhoneInCallCmdCreateConference:
                {
                if ( ! IsFeatureSupported( EFeatureConference ) )
                    {
                    menuPane->DeleteMenuItem( menuItemArray[ i ] ); 
                    }
                break;
                }
            case EPhoneInCallCmdNewCall:
                {
                if ( !FeatureManager::FeatureSupported( KFeatureIdOnScreenDialer ) &&
                     VoIPSupportedL() &&
                     R_PHONEUI_NEW_CALL_MENU_VOIP != aResourceId )
                    {
                        
                    menuPane->DeleteMenuItem( menuItemArray[ i ] ); 
                    
                    HBufC* string = StringLoader::LoadLC( 
                          CPhoneMainResourceResolver::Instance()->
                          ResolveResourceID( EPhoneVoIPNewCallMenuOption ) );
                          
                    CEikMenuPaneItem::SData data;
                    data.iCascadeId=R_PHONEUI_NEW_CALL_MENU_VOIP; //r_phoneui_new_call_menu_voip
                    data.iText = *string;
                    data.iFlags=0;
                    data.iCommandId = 0;             
                    
                    menuPane->InsertMenuItemL( data, i );
                    
                    CleanupStack::PopAndDestroy( string ); 
                    }
                break;
                }
            default: 
                break;
            }
        }

    CleanupStack::PopAndDestroy( &menuItemArray );    

    // Added HO menu items if needed
    AddHandoverMenuItemIfNeededL( aResourceId, *aMenuPane );
	
    if ( iViewCommandHandle.HandleCommandL( 
            EPhoneViewGetNumberEntryIsVisibleStatus ) != 
            EPhoneViewResponseSuccess )
        {
        // Handle call menu without dialer/number entry
        TRAP_IGNORE( ModifyInCallMenuL( aResourceId, *aMenuPane ) );
        }
    else
        {
        // Handle dialer/number entry options menu modification
        ModifyNumberAcquisitionMenuL( aResourceId, *aMenuPane );
        }
    }


// -----------------------------------------------------------
// From class MPhoneCustomization.
// CPhoneCustomizationVoip::ModifyCallHeaderTexts
// Formats and fits VoIP address and possible display name into two lines to 
// be shown on a call bubble. These two lines are placed on CLI and CNAP 
// fields of the TPhoneCmdParamCallHeaderData. In a call bubble CLI-field is 
// placed as a first line and CNAP-field is placed as a second line. 
// -----------------------------------------------------------
//
void CPhoneCustomizationVoip::ModifyCallHeaderTexts( TInt aCallId, 
        TPhoneCmdParamCallHeaderData* aCommandParam,
        const TDesC& /*aInCallNumberText*/ ) 
    {
    __LOGMETHODSTARTEND( PhoneUIVoIPExtension, 
        "CPhoneCustomizationVoip::ModifyCallHeaderTexts" );
    
    MPEEngineInfo& engineInfo = *iStateMachine.PhoneEngineInfo();
    
    if ( EPECallTypeVoIP == engineInfo.CallType( aCallId ) ) 
        {
        const TBool contactInfoAvailable = 
            engineInfo.RemoteName( aCallId ).Length() ||
            engineInfo.RemoteCompanyName( aCallId ).Length();
        
        if ( EPEStateRinging == aCommandParam->CallState() &&
            !contactInfoAvailable )
            {
            if ( KNullDesC() != engineInfo.RemotePartyName( aCallId ) )
                {
                // Telephone UI specification, 4.2.1 Call identification:
                // display name is used as the CLI if available.
                aCommandParam->SetCLIText( 
                    engineInfo.RemotePartyName( aCallId ), 
                    CBubbleManager::ERight );
                // use line reserved for CNAP to show SIP URI
                aCommandParam->SetCNAPText( 
                    engineInfo.RemotePhoneNumber( aCallId ),
                    CBubbleManager::ERight );
                }
            else if( KNullDesC() != engineInfo.RemotePhoneNumber( aCallId ) )
                {
                // Fit address to two rows if it doesnt fit to one
                TBuf<KMaxLengthForSIPURI> sipURI;
                TBuf<KMaxLengthForSIPURI> sipURIDomainPart;
                HandleURIFittingToCallBubble( 
                    aCommandParam->CLIText(),
                    sipURI,
                    sipURIDomainPart );
                
                aCommandParam->SetCLIText( sipURI, CBubbleManager::ERight );
                aCommandParam->SetCNAPText( 
                    sipURIDomainPart,
                    CBubbleManager::ERight );
                }
            else
                {
                // No address or display name specified
                // Do nothing
                }
            }
        else
            {
            // Change clipping direction
            aCommandParam->SetCLIText( 
                aCommandParam->CLIText(), 
                CBubbleManager::ERight );
            
            // Change clipping direction
            aCommandParam->SetCNAPText( 
                aCommandParam->CNAPText(), 
                CBubbleManager::ERight );
            }
        }
    }


// -----------------------------------------------------------
// From class MPhoneCustomization.
// CPhoneCustomizationVoip::HandleCommandL
// -----------------------------------------------------------
//
TBool CPhoneCustomizationVoip::HandleCommandL( TInt aCommand )
    {
    __LOGMETHODSTARTEND( PhoneUIVoIPExtension, 
         "CPhoneCustomizationVoip::HandleCommandL" );
    
    TBool handled( EFalse );
    switch( aCommand )
        {
        case EPhoneNumberAcqCmdInternetCall:
            {
            StateUtils().SelectServiceAndDialL();
            handled = ETrue;
            }
            break;
        case EPhoneInCallCmdNewInternetCall:
        case EPhoneCmdNewInternetCallOk:
        case EPhoneCmdNewInternetCallSearch:
        case EPhoneCmdNewInternetCallContactFetch: 
            {
            iNewCallCmdHandler->HandleCommandL( aCommand );
            handled = ETrue;
            }
            break;
        case EPhoneCmdUnattendedTransferCallBackCancel:
            if ( NeedToRestoreKeyLock() )
                 {
                 SetKeyLockEnabledL( ETrue );
                 SetNeedToRestoreKeyLock( EFalse );
                 }
            // intended fall-through
        case EPhoneInCallCmdUnattendedTransfer:
        case EPhoneCmdTransferDialerOk:
        case EPhoneCmdTransferDialerExit:
        case EPhoneCmdTransferDialerSearch:
        case EPhoneCmdTransferDialerContactFetch:
        case EPhoneCmdUnattendedTransferCallBackOk:
            // intended fall-through
            iTransferCmdHandler->HandleCommandL( aCommand );
            handled = ETrue;
            break;
        case EPhoneCmdCancelServiceEnabling:
            {
            iStateMachine.SendPhoneEngineMessage( 
                MPEPhoneModel::EPEMessageDisableService );
            handled = ETrue;
            }
            break;
        case EPhoneCmdServiceEnablingTimedOut:
            {
            iStateMachine.SendPhoneEngineMessage( 
                MPEPhoneModel::EPEMessageDisableService ); 
  
            TUint32 serviceId = 
                iStateMachine.PhoneEngineInfo()->ServiceIdCommand();
            static_cast<CPhoneStateMachineVoIP&>( iStateMachine ).
                VoipErrorMessageHandler().ShowErrorNoteWithServiceNameL( 
                    EPhoneVoIPServiceUnavailable, serviceId );
                    
            handled = ETrue;
            }
            break;
            
        case EPhoneCmdHandoverToGsm:
            iVccHandler->StartManualHandoverL( EPhoneVoIPWaitHandoverFromWlan );
            handled = ETrue;
            break;

        
        case EPhoneCmdHandoverToWlan:
            iVccHandler->StartManualHandoverL( EPhoneVoIPWaitHandoverFromGsm );
            handled = ETrue;
            break;
            
       case EPhoneCmdRejectUnattendedTransfer:
            if ( NeedToRestoreKeyLock() )
                {
                SetKeyLockEnabledL( ETrue );
                SetNeedToRestoreKeyLock( EFalse );
                } 
            handled = ETrue;
            break;
			
        default:
            {
            RArray<CTelMenuExtension::TCallInfo> array;
            CleanupClosePushL( array );
            GetActiveCallArrayL( array );
            
            // Let extensions handle custom commands
            TRAP_IGNORE( handled = iExtensionHandler->HandleCommandL( 
                    ServiceIdForActivestCallL( array ),
                    aCommand ) );

            CleanupStack::PopAndDestroy( &array );
            }
            break;
        }
    
    return handled;
    }


// -----------------------------------------------------------
// From class MPhoneCustomization.
// CPhoneCustomizationVoip::AllowAlphaNumericMode
// -----------------------------------------------------------
//
TBool CPhoneCustomizationVoip::AllowAlphaNumericMode()
    {
    __LOGMETHODSTARTEND( PhoneUIVoIPExtension, 
        "CPhoneCustomizationVoip::AllowAlphaNumericMode" );
    
    TBool alphaModeSupported( EFalse );
    TRAP_IGNORE( alphaModeSupported = VoIPSupportedL() );

    if ( !alphaModeSupported )
        {
        alphaModeSupported = EasyDialingEnabled();
        }
    
    return alphaModeSupported;
    }


// -----------------------------------------------------------
// From class MPhoneCustomization.
// CPhoneCustomizationVoip::CustomizeSoftKeys
// -----------------------------------------------------------
//
TInt CPhoneCustomizationVoip::CustomizeSoftKeys()
    {
    __LOGMETHODSTARTEND( PhoneUIVoIPExtension, 
        "CPhoneCustomizationVoip::CustomizeSoftKeys" );
	
    return CPhoneMainResourceResolver::Instance()->
        ResolveResourceID( EPhoneVoIPNumberAcqInternetSoftkey );
    }


// -----------------------------------------------------------
// From class MPhoneCustomization.
// CPhoneCustomizationVoip::CustomizeCallHeaderText
// -----------------------------------------------------------
//
TInt CPhoneCustomizationVoip::CustomizeCallHeaderText()
    {
    __LOGMETHODSTARTEND( PhoneUIVoIPExtension, 
        "CPhoneCustomizationVoip::CustomizeCallHeaderText" );
	
    return EPhoneVoIPPrivateAddress;
    }


// -----------------------------------------------------------
// From class MPhoneCustomization.
// CPhoneCustomizationVoip::CustomizeBusyNoteText
// -----------------------------------------------------------
//
TInt CPhoneCustomizationVoip::CustomizeBusyNoteText()
    {
    __LOGMETHODSTARTEND( PhoneUIVoIPExtension, 
        "CPhoneCustomizationVoip::CustomizeBusyNoteText" );
    
    TInt resource( EPhoneNumberBusy );
    
    const TPECallType callType =
        iStateMachine.PhoneEngineInfo()->CallTypeCommand();
    if ( callType == EPECallTypeVoIP )
        {
        resource = EPhoneVoIPNumberBusy;
        }
    
    return resource;
    }


// -----------------------------------------------------------
// From class MPhoneCustomization.
// CPhoneCustomizationVoip::HandleCallFromNumberEntryL
// -----------------------------------------------------------
//
TBool CPhoneCustomizationVoip::HandleCallFromNumberEntryL()
    {
    __LOGMETHODSTARTEND( PhoneUIVoIPExtension, 
        "CPhoneCustomizationVoip::HandleCallFromNumberEntryL" );
    
    TBool handled = EFalse;
    
    if ( StateUtils().IsVoipNumber() )
        {
        // Dial voip because invalid string for cs call in number entry/dialer.
        StateUtils().SelectServiceAndDialL();
        handled = ETrue;
        }
    
    return handled;
    }


// -----------------------------------------------------------
// From class MPhoneCustomization.
// CPhoneCustomizationVoip::HandlePhoneEngineMessageL
// -----------------------------------------------------------
//
TBool CPhoneCustomizationVoip::HandlePhoneEngineMessageL(
        const TInt aMessage, 
        TInt aCallId )
    {
    __LOGMETHODSTARTEND( PhoneUIVoIPExtension, 
        "CPhoneCustomizationVoip::HandlePhoneEngineMessageL" );
    
    TBool handled = ETrue;
    switch ( aMessage )
        {
        case MEngineMonitor::EPEMessageServiceEnabled:
        case MEngineMonitor::EPEMessageServiceDisabled:
            {
            iViewCommandHandle.ExecuteCommandL( 
                    EPhoneViewRemoveGlobalWaitNote );
            break;
            }
        case MEngineMonitor::EPEMessageServiceEnabling:
            {
            LaunchEnablingServiceNoteL();
            break; 
            }
        case MEngineMonitor::EPEMessageNoService:
            {
            SendGlobalErrorNoteL( EPhoneVoIPNoService );
            break; 
            }
        case MEngineMonitor::EPEMessageMovedPermanently:
            {
            iCallForwardHandler->
                ShowMovedPermanentlyQueryL( aCallId );
            break;
            }    
        case MEngineMonitor::EPEMessageMultipleChoices:
            {
            iCallForwardHandler->
                ShowMultipleChoicesQueryL( aCallId );
            break;
            }
        case MEngineMonitor::EPEMessageCallSecureStatusChanged:
            {
            HandleCallSecureStatusChangeL( aCallId );
            break;
            }
        case MEngineMonitor::EPEMessageTransferring:
            {
            iViewCommandHandle.ExecuteCommandL( EPhoneViewRemoveGlobalNote );
            break;
            }
        case MEngineMonitor::EPEMessageTransferDone:
            {
            iViewCommandHandle.ExecuteCommandL( EPhoneViewRemoveGlobalNote );
            SendGlobalInfoNoteL( EPhoneInCallTransferred );
            break; 
            }
        case MEngineMonitor::EPEMessageRemoteForwarding:
            {
            SendGlobalErrorNoteL( EPhoneRemoteForwarding );
            break; 
            }
        case MEngineMonitor::EPEMessageTransferCallBackRequest:
            {
            if ( iStateMachine.State()->IsKeyLockOn()  )
                {
                SetNeedToRestoreKeyLock( ETrue );
                SetKeyLockEnabledL( EFalse );      
                }
            iTransferCmdHandler->LaunchCallBackQueryL();
            break;
            }
		case MEngineMonitor::EPEMessageIdle:
            {
            TPhoneCmdParamInteger activeCallCount;
            iViewCommandHandle.ExecuteCommandL(
                    EPhoneViewGetCountOfActiveCalls, &activeCallCount );
            
            if ( ENoActiveCalls == activeCallCount.Integer() 
                 && NeedToRestoreKeyLock() )
                {
                SetKeyLockEnabledL( ETrue );
                SetNeedToRestoreKeyLock( EFalse );
                }
			break;
			}
        default:
            handled = EFalse;
            break;
        }
    return handled;
    }

// -----------------------------------------------------------
// From class MPhoneCustomization.
// CPhoneCustomizationVoip::HandleDialL
// -----------------------------------------------------------
//
void CPhoneCustomizationVoip::HandleDialL( const TDesC& aNumber )
    {
    __LOGMETHODSTARTEND( PhoneUIVoIPExtension, 
        "CPhoneCustomizationVoip::HandleDialL" );
        
    StateUtils().SelectServiceAndDialL( aNumber, NULL );
    }

// -----------------------------------------------------------
// From class MPhoneButtonCustomization.
// CPhoneCustomizationVoip::CustomizeTouchPaneButtons
// -----------------------------------------------------------
//
void CPhoneCustomizationVoip::CustomizeTouchPaneButtons(
        MBubbleTouchPaneInterface& aTouchPane )
    {
    __LOGMETHODSTARTEND( PhoneUIVoIPExtension, 
        "CPhoneCustomizationVoip::CustomizeTouchPaneButtons" );
    
    TRAPD( err, CustomizeTouchPaneUsingExtensionL( aTouchPane ) );
    
    if ( KErrNone != err )
        {
        // Extension plugin didn't handle touch pane dimming.
        TInt buttonCount = aTouchPane.NumberOfButtonsInPane();
            
        for ( TInt i=0; i < buttonCount; i++ )    
            {
            TInt commandId = aTouchPane.ButtonCommandId( i );
        
            switch ( commandId )
                {              
                case EPhoneInCallCmdCreateConference:
                case EPhoneInCallCmdJoin:
                    {
                    if ( !IsFeatureSupported( EFeatureConference ) )
                        {
                        // Conference not supported -> set item dimmed
                        aTouchPane.SetButtonDimmed( commandId, ETrue );
                        }               
                    break;  
                    }
                default:
                    break;                    
                } // switch
            } // for    
        }
    }


// -----------------------------------------------------------
// CPhoneCustomizationVoip::VoIPSupportedL
// -----------------------------------------------------------
//
TBool CPhoneCustomizationVoip::VoIPSupportedL()
    {
    __LOGMETHODSTARTEND( PhoneUIVoIPExtension, 
        "CPhoneCustomizationVoip::VoIPSupportedL" );
    
    TBool voipSupported( EFalse );
    
    CSPSettings* serviceProviderSettings = CSPSettings::NewL();
    voipSupported = serviceProviderSettings->IsFeatureSupported( 
        ESupportInternetCallFeature );            
    delete serviceProviderSettings;
    
    return voipSupported;   
    }

// -----------------------------------------------------------
// CPhoneCustomizationVoip::EasyDialingEnabled
// -----------------------------------------------------------
//
TBool CPhoneCustomizationVoip::EasyDialingEnabled()
    {
    __LOGMETHODSTARTEND( PhoneUIVoIPExtension,
            "CPhoneCustomizationVoip::EasyDialingEnabled" );

    TBool easyDialingEnabled( EFalse );
    if ( FeatureManager::FeatureSupported( KFeatureIdProductIncludesHomeScreenEasyDialing ) )
        {
        TInt easyDialingSetting;
        TInt err = CPhoneCenRepProxy::Instance()->GetInt(
                KCRUidEasyDialSettings,
                KEasyDialing,
                easyDialingSetting);
        if ( !err && easyDialingSetting )
            {
            easyDialingEnabled = ETrue;
            }
        }
    return easyDialingEnabled;
    }

// -----------------------------------------------------------
// CPhoneCustomizationVoip::ModifyInCallMenuL
// -----------------------------------------------------------
//
void CPhoneCustomizationVoip::ModifyInCallMenuL( TInt aResourceId, CEikMenuPane& aMenuPane )
    {
    __LOGMETHODSTARTEND( PhoneUIVoIPExtension, 
        "CPhoneCustomizationVoip::ModifyInCallMenuL" );
    
    if ( aResourceId != R_AVKON_MENUPANE_FEP_DEFAULT &&
        aResourceId != R_AVKON_MENUPANE_EDITTEXT_DEFAULT &&
        aResourceId != R_AVKON_MENUPANE_LANGUAGE_DEFAULT )
        {
        RArray<CTelMenuExtension::TCallInfo> array;
        CleanupClosePushL( array );
        
        GetActiveCallArrayL( array );

        // Use extensions for menu modifications 
        iExtensionHandler->ModifyInCallMenuL(
                ServiceIdForActivestCallL( array ),
                array,
                aResourceId,
                aMenuPane );
        
        __PHONELOG( EBasic, PhoneUIVoIPExtension,
            "CPhoneCustomizationVoip::ModifyInCallMenuL() plugin exists" );
        
        CleanupStack::PopAndDestroy( &array );
        }
    }


// -----------------------------------------------------------
// CPhoneCustomizationVoip::ModifyNumberAcquisitionMenuL
// This function is called when user opens menu from dialler/number entry.
// -----------------------------------------------------------
//
void CPhoneCustomizationVoip::ModifyNumberAcquisitionMenuL(
        TInt aResourceId, CEikMenuPane& aMenuPane )
    {
    __LOGMETHODSTARTEND( PhoneUIVoIPExtension, 
        "CPhoneCustomizationVoip::ModifyNumberAcquisitionMenuL");
    
    if ( aResourceId == R_PHONEUI_NUMBERACQ_OPTIONS_CALL_MENU &&
           VoIPSupportedL() )
        {
        InsertInternetCallMenuItemL( aResourceId, aMenuPane );
        }
    
    if ( ( aResourceId == R_PHONEUI_NUMBERACQ_OPTIONS_MENU ||
           aResourceId == R_PHONEUI_NUMBERACQ_OPTIONS_MENU_VOIP ||
           aResourceId == R_PHONEUI_NUMBERACQ_OPTIONS_MENU_VOIP_WITH_SERVICE_NUMBER ) &&
           VoIPSupportedL() )
        {
        InsertTextInputModeChangeMenuItemL( aMenuPane );
        }
    }


// -----------------------------------------------------------
// CPhoneCustomizationVoip::InsertInternetCallMenuItemL
// -----------------------------------------------------------
//
void CPhoneCustomizationVoip::InsertInternetCallMenuItemL( 
        TInt /* aResourceId */, CEikMenuPane& aMenuPane )
    {
    __LOGMETHODSTARTEND( PhoneUIVoIPExtension, 
        "CPhoneCustomizationVoip::InsertInternetCallMenuItemL" );

    // get VoIP services
    RIdArray voipServices;
    CleanupClosePushL( voipServices );
    GetVoIPServiceIdsL( voipServices );
    
    CEikMenuPaneItem* voipMenuItem = NULL;
    
    if ( 1 == voipServices.Count() )
        {
        voipMenuItem = CreateMenuPaneItemLC( 
            EPhoneNumberAcqCmdInternetCall,
            voipServices[0] );
        }
    else
        {
        voipMenuItem = CreateMenuPaneItemLC( 
            EPhoneNumberAcqCmdInternetCall,
            CPhoneMainResourceResolver::Instance()->
                ResolveResourceID( EPhoneVoIPCallTypeInternet ) );
        }

    HBufC* phoneNumber = PhoneNumberFromEntryLC();

    // If number entry is empty don't add internet call option 
    if ( phoneNumber->Length() )
        {
        aMenuPane.AddMenuItemL( voipMenuItem->iData );
        }
    
    CleanupStack::PopAndDestroy( 3, &voipServices );
    }


// -----------------------------------------------------------
// CPhoneCustomizationVoip::InsertTextInputModeChangeMenuItemL
// -----------------------------------------------------------
//
void CPhoneCustomizationVoip::InsertTextInputModeChangeMenuItemL( 
        CEikMenuPane& aMenuPane )
    {
    __LOGMETHODSTARTEND( PhoneUIVoIPExtension, 
        "CPhoneCustomizationVoip::InsertTextInputModeChangeMenuItemL" );
    
    TInt modeChangeOptionIndex = KErrNotFound;
    if ( aMenuPane.MenuItemExists( 
             EPhoneNumberAcqCmdToggleNeAlphaMode, modeChangeOptionIndex ) ||
         aMenuPane.MenuItemExists( 
             EPhoneNumberAcqCmdToggleNeNumericMode, modeChangeOptionIndex ) )
        {
        __PHONELOG( EBasic, PhoneUIVoIPExtension,
            "InsertTextInputModeChangeMenuItemL, MENU ITEM ALREADY EXISTS" );
        __ASSERT_DEBUG( EFalse, Panic( EPhoneCtrlInvariant ) );
        }
    
    CEikMenuPaneItem* menuItem = NULL;
    TBool numberEntryInNumericMode = iViewCommandHandle.HandleCommandL( 
        EPhoneViewIsNumberEntryNumericMode ) == EPhoneViewResponseSuccess;
    if ( numberEntryInNumericMode )
        {
        menuItem = CreateMenuPaneItemLC( 
            EPhoneNumberAcqCmdToggleNeAlphaMode,
            R_PHONEUI_NUMBERACQ_OPTIONS_MENU_ITEM_ALPHA_MODE );
        }
    else
        {
        menuItem = CreateMenuPaneItemLC( 
            EPhoneNumberAcqCmdToggleNeNumericMode,
            R_PHONEUI_NUMBERACQ_OPTIONS_MENU_ITEM_NUMERIC_MODE );
        }
    
    // alpha/numeric mode option is always second item in number entry menu
    const TInt KTextInputModeChangeMenuItemIndex = 1;
    aMenuPane.InsertMenuItemL( 
        menuItem->iData, KTextInputModeChangeMenuItemIndex );
    CleanupStack::PopAndDestroy( menuItem );
    }


// -----------------------------------------------------------
// CPhoneCustomizationVoip::GetActiveCallArrayL
// -----------------------------------------------------------
//
void CPhoneCustomizationVoip::GetActiveCallArrayL( 
        RArray<CTelMenuExtension::TCallInfo>& aArray )
    {
    __LOGMETHODSTARTEND( PhoneUIVoIPExtension, 
        "CPhoneCustomizationVoip::GetActiveCallArrayL");
    
    MPEEngineInfo* engineInfo = iStateMachine.PhoneEngine()->EngineInfo();
    
    for( TInt i = 0; i < KPEMaximumNumberOfCalls; i++ )
        {
        CTelMenuExtension::TCallInfo callInfo;
        callInfo.iCallId = i;
        callInfo.iServiceId = engineInfo->ServiceId( i );
        callInfo.iCallType = CTelMenuExtension::EUninitialized;

        switch( engineInfo->CallType( i ) )
            {
            case EPECallTypeUninitialized:
                callInfo.iCallType = CTelMenuExtension::EUninitialized;
                break;
            case EPECallTypeCSVoice:
                callInfo.iCallType = CTelMenuExtension::ECsVoice;
                break;
            case EPECallTypeVideo:
                callInfo.iCallType = CTelMenuExtension::ECsVideo;
                break;
            case EPECallTypeVoIP:
                callInfo.iCallType = CTelMenuExtension::EPsVoice;
                break;
            default:
                break;
            }
        
        callInfo.iCallState = CTelMenuExtension::ENone;
        
        // Convert call bubble indications to call states
        switch( engineInfo->CallState( i ) )
            {
            // Call states                        0 - 499
            case EPEStateDialing:
            case EPEStateEmergencyDialing:
            case EPEStateConnecting:
                callInfo.iCallState = CTelMenuExtension::EOutgoing;
                break;
            case EPEStateAnswering:
            case EPEStateRinging:
                callInfo.iCallState = CTelMenuExtension::EIncoming;
                break;

            case EPEStateConnectedConference:
            case EPEStateConnected:
                callInfo.iCallState = CTelMenuExtension::EActive;
                break;

            case EPEStateHeldConference:
            case EPEStateHeld:
                callInfo.iCallState = CTelMenuExtension::EOnHold;
                break;
            
            case EPEStateHangingUpConference:
            case EPEStateRejecting:
            case EPEStateHangingUp:
            case EPEStateDisconnecting:
                callInfo.iCallState = CTelMenuExtension::EDisconnecting;
                break;
                
            case EPEStateUnknown:
            case EPEStateIdle:
            case EPEStateConferenceIdle:
                break;
                
            // conference
            case EPEStateCreatingConference:
            case EPEStateSwappingConference:
            case EPEStateResumeHoldConference:
            // misc states.
            case EPEStateMaximumState:
            default:
                User::Leave( KErrNotReady );
                break;
            }

        if( callInfo.iCallState != CTelMenuExtension::ENone )
            {
            aArray.Append( callInfo );                                   
            }

        }
    }


// -----------------------------------------------------------
// CPhoneCustomizationVoip::ResolveMostActiveCallL
// -----------------------------------------------------------
//
TInt CPhoneCustomizationVoip::ResolveMostActiveCallL( 
        const RArray<CTelMenuExtension::TCallInfo>& aArray )
    {
    __LOGMETHODSTARTEND( PhoneUIVoIPExtension, 
        "CPhoneCustomizationVoip::ResolveMostActiveCallL" );
    
    TInt incomingCallId( KErrNotFound );
    TInt activeCallId( KErrNotFound );
    TInt onholdCallId( KErrNotFound );
    TInt disconnectingCallId( KErrNotFound );
    
    for( TInt i = 0; i < aArray.Count(); i++ )
        {
        if( aArray[i].iCallState == CTelMenuExtension::EOutgoing )
            {
            // Always most active call
            return aArray[i].iCallId;
            }
        else if( aArray[i].iCallState == CTelMenuExtension::EActive )
            {
            activeCallId = aArray[i].iCallId;
            }
        else if( aArray[i].iCallState == CTelMenuExtension::EIncoming )
            {
            incomingCallId = aArray[i].iCallId;
            }
        else if( aArray[i].iCallState == CTelMenuExtension::EOnHold )
            {
            onholdCallId = aArray[i].iCallId;
            }
        else if( aArray[i].iCallState == CTelMenuExtension::EDisconnecting )
            {
            disconnectingCallId = aArray[i].iCallId;
            }
        }
    
    if( activeCallId != KErrNotFound )
        {
        return activeCallId;
        }
    else if( incomingCallId != KErrNotFound )
        {
        return incomingCallId;
        }
    else if( onholdCallId != KErrNotFound )
        {
        return onholdCallId;
        }
    else if( disconnectingCallId != KErrNotFound )
        {
        return disconnectingCallId;
        }
    else
        {
        User::Leave( KErrNotFound );
        }
    
    return KErrNotFound;
    }


// -----------------------------------------------------------
// CPhoneCustomizationVoip::ServiceIdForActivestCallL
// (other items were commented in a header).
// -----------------------------------------------------------
//
TUint CPhoneCustomizationVoip::ServiceIdForActivestCallL(
        const RArray<CTelMenuExtension::TCallInfo>& aCallArray )
    {   
    __LOGMETHODSTARTEND( PhoneUIVoIPExtension, 
        "CPhoneCustomizationVoip::ServiceIdForActivestCallL" );
    
    const TInt KActiveCallId = ResolveMostActiveCallL( aCallArray );
     
    MPEEngineInfo* engineInfo = iStateMachine.PhoneEngine()->EngineInfo();
    
    // Return service id for activest call
    return engineInfo->ServiceId( KActiveCallId ); 
    }


// -----------------------------------------------------------
// CPhoneCustomizationVoip::IsFeatureSupported()
// (other items were commented in a header).
// -----------------------------------------------------------
//
TBool CPhoneCustomizationVoip::IsFeatureSupported( TFeature aFeature )
    { 
    __LOGMETHODSTARTEND( PhoneUIVoIPExtension, 
        "CPhoneCustomizationVoip::IsFeatureSupported" );
    
    TBool ret( EFalse );

    TRAP_IGNORE( ret = CheckFeatureSupportByCallTypeL( aFeature ) );
    
    __PHONELOG1( EBasic, PhoneUIVoIPExtension, 
        "CPhoneCustomizationVoip::IsFeatureSupported =%d", ret  );
    
    return ret;
    }


// -----------------------------------------------------------
// CPhoneCustomizationVoip::CheckFeatureSupportByCallTypeL()
// (other items were commented in a header).
// -----------------------------------------------------------
//
TBool CPhoneCustomizationVoip::CheckFeatureSupportByCallTypeL( 
        TFeature aFeature )
    { 
    __LOGMETHODSTARTEND( PhoneUIVoIPExtension, 
        "CPhoneCustomizationVoip::CheckFeatureSupportByCallTypeL" );
    
    if ( aFeature != EFeatureConference &&
         aFeature != EFeatureTransfer )
        {
        return ETrue;
        }
    
    RArray<CTelMenuExtension::TCallInfo> array;
    CleanupClosePushL( array );
    GetActiveCallArrayL( array );
    __PHONELOG1( EBasic, PhoneUIVoIPExtension, 
        "CPhoneCustomizationVoip::CheckFeatureSupportByCallTypeL CallCount=%d", 
        array.Count()  );
    
    TBool supported( ETrue );
    TInt firstCallType = CTelMenuExtension::EUninitialized;
    for( TInt i = 0; i < array.Count(); i++ )
        {
        if( array[i].iCallState == CTelMenuExtension::EOutgoing ||
            array[i].iCallState == CTelMenuExtension::EActive ||
            array[i].iCallState == CTelMenuExtension::EOnHold )
            {              
            if( aFeature == EFeatureTransfer &&
                array[i].iCallState == CTelMenuExtension::EOutgoing &&
                array[i].iCallType == CTelMenuExtension::EPsVoice  )
                {
                // If there is outgoing VoIP call then transfer is not supported
                supported = EFalse;
                break;
                }
            
            if( firstCallType == CTelMenuExtension::EUninitialized )
                {
                // Memorize first found call type
                firstCallType = array[i].iCallType;
                }
            else if( firstCallType != array[i].iCallType )
                {
                // Found different call type
                supported = EFalse;
                break;
                }
            }

        }
    
    CleanupStack::PopAndDestroy( &array );

    return supported;
    }


// -----------------------------------------------------------
// CPhoneCustomizationVoip::CustomizeTouchPaneUsingExtensionL
// -----------------------------------------------------------
//
void CPhoneCustomizationVoip::CustomizeTouchPaneUsingExtensionL(
        MBubbleTouchPaneInterface& aTouchPane )
    {
    __LOGMETHODSTARTEND( PhoneUIVoIPExtension, 
        "CPhoneCustomizationVoip::CustomizeTouchPaneUsingExtensionL" );
    
    RArray<CTelMenuExtension::TCallInfo> array;
    CleanupClosePushL( array );
    
    GetActiveCallArrayL( array );

    // Use extensions for touch pane buttons dimming 
    iExtensionHandler->CustomizeTouchPaneButtonsL(
            ServiceIdForActivestCallL( array ),
            array,
            aTouchPane );
    
    __PHONELOG( EBasic, PhoneUIVoIPExtension,
        "CPhoneCustomizationVoip::ModifyInCallMenuL() plugin exists" );
    
    CleanupStack::PopAndDestroy( &array );    
    }


// -----------------------------------------------------------
// CPhoneCustomizationVoip::LaunchEnablingServiceNoteL
// -----------------------------------------------------------
//
void CPhoneCustomizationVoip::LaunchEnablingServiceNoteL()
    {
    __LOGMETHODSTARTEND( PhoneUIVoIPExtension, 
        "CPhoneCustomizationVoip::LaunchEnablingServiceNoteL" )
    
    TPhoneCmdParamQuery queryParam;
    queryParam.SetQueryType( EPhoneGlobalWaitNote );
    queryParam.SetCommandParamId( 
        TPhoneCommandParam::EPhoneParamIdGlobalWaitNote );
    
    RBuf serviceName;
    CleanupClosePushL( serviceName );
    TUint32 serviceId = 
        iStateMachine.PhoneEngineInfo()->ServiceIdCommand(); 
    static_cast<CPhoneStateMachineVoIP&>( iStateMachine ).
        VoipErrorMessageHandler().GetServiceProviderNameL( 
        serviceName, serviceId );
    
    RBuf noteText;
    CleanupClosePushL( noteText );
    noteText.Assign( StringLoader::LoadL( 
    CPhoneMainResourceResolver::Instance()->ResolveResourceID( 
        EPhoneVoIPEnablingServiceWaitNoteText ), serviceName ) );
    
    queryParam.SetDataText( &noteText );  
    queryParam.SetDefaultCba( R_AVKON_SOFTKEYS_CANCEL );
    
    const TInt KEnableServiceTimeOutInSecs = 60;
    queryParam.SetTimeOut( KEnableServiceTimeOutInSecs );
    // configure custom command mappings for user responses
    queryParam.SetCbaCommandMapping( 
        EAknSoftkeyCancel, EPhoneCmdCancelServiceEnabling );
    queryParam.SetCbaCommandMapping( 
        KErrCancel, EPhoneCmdCancelServiceEnabling );
    queryParam.SetCustomCommandForTimeOut( EPhoneCmdServiceEnablingTimedOut ); 
    iViewCommandHandle.ExecuteCommandL( EPhoneViewShowQuery, &queryParam );
     
    CleanupStack::PopAndDestroy( &noteText );
    CleanupStack::PopAndDestroy( &serviceName ); 
    }


// ---------------------------------------------------------
//  CPhoneCustomizationVoip::SendGlobalErrorNoteL
// ---------------------------------------------------------
//
void CPhoneCustomizationVoip::SendGlobalErrorNoteL( TInt aResourceId )
    {
    __LOGMETHODSTARTEND( PhoneUIVoIPExtension, 
        "CPhoneCustomizationVoip::SendGlobalErrorNoteL" );
    
    __ASSERT_DEBUG( aResourceId, Panic( EPhoneCtrlParameterNotInitialized ) );
    
	CPhoneState* phoneState = 
        static_cast< CPhoneState* >( iStateMachine.State() ); 		
			
    phoneState->SendGlobalErrorNoteL( aResourceId );		
    }


// ---------------------------------------------------------
//  CPhoneCustomizationVoip::SendGlobalInfoNoteL
// ---------------------------------------------------------
//
void CPhoneCustomizationVoip::SendGlobalInfoNoteL( TInt aResourceId )
    {
    __LOGMETHODSTARTEND( PhoneUIVoIPExtension, 
        "CPhoneCustomizationVoip::SendGlobalInfoNoteL" );
    
    __ASSERT_DEBUG( aResourceId, Panic( EPhoneCtrlParameterNotInitialized ) );
    
    CPhoneState* phoneState = 
        static_cast< CPhoneState* >( iStateMachine.State() );       
    
    phoneState->SendGlobalInfoNoteL( aResourceId );
    }


// -----------------------------------------------------------
// CPhoneCustomizationVoip::HandleURIFittingToCallBubble
// -----------------------------------------------------------
//
void CPhoneCustomizationVoip::HandleURIFittingToCallBubble( 
    const TDesC& aCLIText, TDes& aSipURI, TDes& aSipURIDomainPart )
    {
    __LOGMETHODSTARTEND( PhoneUIVoIPExtension,
        "CPhoneCustomizationVoip::HandleURIFittingToCallBubble" );
    
    TInt cliLength = aCLIText.Length();
    if ( KMaxLengthForSIPURIFirstLine > cliLength )
        {
        aSipURI.Copy( aCLIText );
        aSipURIDomainPart.Copy( KNullDesC );
        }
    else if ( KMaxLengthForSIPURITwoLines > cliLength )
        {
        aSipURI.Copy( aCLIText.Mid( 0, KMaxLengthForSIPURIFirstLine ) );
        aSipURIDomainPart.Copy( aCLIText.Mid( KMaxLengthForSIPURIFirstLine,
            cliLength-KMaxLengthForSIPURIFirstLine ) );
        }
    else  
        {
        TInt atPosition = aCLIText.Find( KVoipAt );
        if ( atPosition == KErrNotFound )
            {
            atPosition = KMaxLengthForSIPURIFirstLine;
            }
        aSipURI.Copy( aCLIText.Mid( 0, atPosition ) );
        aSipURIDomainPart.Copy( aCLIText.Mid( atPosition,
            ( cliLength-atPosition ) ) );
        }      
    }


// -----------------------------------------------------------
// CPhoneCustomizationVoip::StateUtils
// -----------------------------------------------------------
//
CPhoneStateUtilsVoip& CPhoneCustomizationVoip::StateUtils()
    {
    return static_cast<CPhoneStateMachineVoIP&>( iStateMachine ).StateUtils();
    }


// -----------------------------------------------------------
// CPhoneCustomizationVoip::HandleSecureStatusChangeL
// -----------------------------------------------------------
//
void CPhoneCustomizationVoip::HandleCallSecureStatusChangeL( TInt aCallId )
    {
    __LOGMETHODSTARTEND( PhoneUIVoIPExtension, 
        "CPhoneCustomizationVoip::HandleCallSecureStatusChangeL" );
    
    if ( KErrNotFound < aCallId )
        {
        if ( ( EPECallTypeVoIP == 
            iStateMachine.PhoneEngineInfo()->CallType( aCallId ) ) &&
            !iStateMachine.PhoneEngineInfo()->IsSecureCall( aCallId ) ) 
            {
            TInt state = iStateMachine.PhoneEngineInfo()->CallState( 
                aCallId );
            switch ( state )
                {      
                case EPEStateConnected:
                    {
                    iViewCommandHandle.ExecuteCommand( 
                        EPhoneViewCipheringInfoChangePlayTone );
                    break;
                    }
                case EPEStateDialing:
                    {
                    SendGlobalInfoNoteL( 
                        EPhoneSSNotificationTextVoipSipsFailed );
                    break;
                    }
                default:
                    break;
                
                }
            }
        }
    }


// -----------------------------------------------------------
// CPhoneCustomizationVoip::CreateMenuPaneItemLC
// -----------------------------------------------------------
//
CEikMenuPaneItem* CPhoneCustomizationVoip::CreateMenuPaneItemLC( 
        TInt aCommandId, TInt aTextResourceId ) const
    {
    __LOGMETHODSTARTEND( PhoneUIVoIPExtension, 
        "CPhoneCustomizationVoip::CreateMenuPaneItemLC" );
    
    CEikMenuPaneItem::SData data;
    data.iCommandId = aCommandId;
    data.iCascadeId = 0;
    data.iFlags = 0;
    iCoeEnv.ReadResourceL( data.iText, aTextResourceId );    
    data.iExtraText = KNullDesC();
    
    CEikMenuPaneItem* menuItem = new ( ELeave ) CEikMenuPaneItem();
    CleanupStack::PushL( menuItem );
    menuItem->iData = data;
    
    return menuItem;
    }


// -----------------------------------------------------------
// CPhoneCustomizationVoip::CreateMenuPaneItemLC
// -----------------------------------------------------------
//
CEikMenuPaneItem* CPhoneCustomizationVoip::CreateMenuPaneItemLC(
    TInt aCommandId, TServiceId aServiceId ) const
    {
    __LOGMETHODSTARTEND( PhoneUIVoIPExtension, 
        "CPhoneCustomizationVoip::CreateMenuPaneItemLC" );

    CEikMenuPaneItem::SData data;
    data.iCommandId = aCommandId;
    data.iCascadeId = 0;
    data.iFlags = 0;
    data.iExtraText = KNullDesC();

    // insert service name
    TBuf<KMaxVoIPServiceName> serviceName;
    GetVoipServiceNameL( aServiceId, serviceName );
    
    HBufC* callText = StringLoader::LoadLC(
        CPhoneMainResourceResolver::Instance()
            ->ResolveResourceID( EPhoneVoIPNumberAcqXSPCall ),
        serviceName );
    
    data.iText.Append( *callText );
    
    CleanupStack::PopAndDestroy( callText );

    CEikMenuPaneItem* menuItem = new ( ELeave ) CEikMenuPaneItem();
    CleanupStack::PushL( menuItem );
    menuItem->iData = data;

    return menuItem;
    }

    
// -----------------------------------------------------------
// CPhoneCustomizationVoip::AddHandoverMenuItemIfNeededL
// -----------------------------------------------------------
//
void CPhoneCustomizationVoip::AddHandoverMenuItemIfNeededL( TInt aResourceId,
        CEikMenuPane& aMenuPane )
    {
    __LOGMETHODSTARTEND( PhoneUIVoIPExtension, 
        "CPhoneCustomizationVoip::AddHandoverMenuItemIfNeededL" );
    
    // Menu items to be added
    if ( iVccHandler && VoIPSupportedL() &&
        ( aResourceId == R_PHONEUI_CALLHANDLING_INCALL_OPTIONS_MENU ||
          aResourceId == R_PHONEUI_CALLWAITING_OPTIONS_MENU ||
          aResourceId == R_PHONEUI_ACTIVEANDHELDCALL_OPTIONS_MENU ||
          aResourceId == R_PHONEUI_ACTIVEANDHELDCONFCALL_OPTIONS_MENU ||
          aResourceId == R_PHONEUI_CALLACTIVEHELDANDWAITING_OPTIONS_MENU 
        ))
        {        
        RArray<CTelMenuExtension::TCallInfo> array;
        CleanupClosePushL( array );
        GetActiveCallArrayL( array );
        
        iVccHandler->AddHandoverMenuItemL( array, aResourceId, aMenuPane );
        CleanupStack::PopAndDestroy( &array );   
        }
    }


// -----------------------------------------------------------
// CPhoneCustomizationVoip::GetVoIPServiceIdsL
// -----------------------------------------------------------
//
void CPhoneCustomizationVoip::GetVoIPServiceIdsL( RIdArray& aVoipServiceIds ) const
    {
    aVoipServiceIds.Reset();

    // Fetch the settings api.
    CSPSettings* settingsApi = CSPSettings::NewLC();
        
    // read all service ids to array
    RIdArray idArray;
    CleanupClosePushL( idArray );
        
    if ( KErrNone == settingsApi->FindServiceIdsL( idArray ) )
        {
        // go throught all the services and check if any
        // of them supports internet call
        for ( TInt i = 0; idArray.Count() > i; i++)
            {
            // check if the service supports internet call
            CSPProperty* property = CSPProperty::NewLC();

            // get attribute mask of the service
            TRAPD( error, error = settingsApi->FindPropertyL( idArray[i],
                EPropertyServiceAttributeMask, *property ) );

            if ( KErrNone == error )
                {
                // read the value of mask property
                TInt mask = 0;
                error = property->GetValue( mask );
                if ( KErrNone == error )
                    {
                    if ( ( mask & ESupportsInternetCall )
                        && ( mask & EIsVisibleInCallMenu ) )
                        {
                        aVoipServiceIds.Append( idArray[i] );
                        }
                    }
                }
            CleanupStack::PopAndDestroy( property );
            }
        }

    CleanupStack::PopAndDestroy( 2, settingsApi );
    }


// -----------------------------------------------------------
// CPhoneCustomizationVoip::GetVoIPServiceIdsL
// -----------------------------------------------------------
//
void CPhoneCustomizationVoip::GetVoipServiceNameL( TServiceId aServiceId, TDes& aServiceName ) const
    {
    CSPSettings* settingsApi = CSPSettings::NewLC();
    CSPProperty* property = CSPProperty::NewLC();

    settingsApi->FindPropertyL( aServiceId, EServiceName, *property );
    property->GetValue( aServiceName );
    
    CleanupStack::PopAndDestroy( 2, settingsApi );
    }


// -----------------------------------------------------------
// CPhoneCustomizationVoip::PhoneNumberFromEntryLC
// -----------------------------------------------------------
//
HBufC* CPhoneCustomizationVoip::PhoneNumberFromEntryLC() const
    {
    HBufC* phoneNumber = HBufC::NewLC( KPhoneNumberEntryBufferSize );
    
    TPtr ptr( phoneNumber->Des() );
    TPhoneCmdParamString stringParam;
    stringParam.SetString( &ptr );
    iViewCommandHandle.ExecuteCommand(
        EPhoneViewGetNumberFromEntry,
        &stringParam );
    
    return phoneNumber;
    }

// ---------------------------------------------------------------------------
// CPhoneCustomizationVoip::SetKeyLockEnabledL
// ---------------------------------------------------------------------------
//
void CPhoneCustomizationVoip::SetKeyLockEnabledL( TBool aEnabled )
    {
    __LOGMETHODSTARTEND( PhoneUIVoIPExtension, 
        "CPhoneCustomizationVoip::CPhoneCustomizationVoip::SetKeyLockEnabledL" );
    if ( aEnabled )
        {
        iViewCommandHandle.ExecuteCommandL(
                                  EPhoneViewEnableKeyLock );
        }
    else
        {
        iViewCommandHandle.ExecuteCommandL(
                              EPhoneViewDisableKeyLockWithoutNote );
        }           
    }

// ---------------------------------------------------------------------------
// CPhoneCustomizationVoip::SetNeedToRestoreKeyLock
// ---------------------------------------------------------------------------
//
void CPhoneCustomizationVoip::SetNeedToRestoreKeyLock( TBool aRestore )
    {
    __LOGMETHODSTARTEND( PhoneUIVoIPExtension, 
          "CPhoneCustomizationVoip::CPhoneCustomizationVoip::SetNeedToRestoreKeyLock" );
    __PHONELOG1( EBasic, PhoneUIVoIPExtension, 
               "CPhoneCustomizationVoip::AwrNeedToRestoreKeyLock:%d", aRestore );
    iNeedToRestoreKeyLock = aRestore;
    }

// -----------------------------------------------------------
// CPhoneCustomizationVoip::NeedToRestoreKeyLock
// -----------------------------------------------------------
//
TBool CPhoneCustomizationVoip::NeedToRestoreKeyLock() const
    {
    __LOGMETHODSTARTEND( PhoneUIVoIPExtension, 
          "CPhoneCustomizationVoip::CPhoneCustomizationVoip::NeedToRestoreKeyLock" );
    __PHONELOG1( EBasic, PhoneUIVoIPExtension, 
            "CPhoneCustomizationVoip::NeedToRestoreKeyLock:%d", iNeedToRestoreKeyLock );
    return iNeedToRestoreKeyLock;
    }

// End of File
