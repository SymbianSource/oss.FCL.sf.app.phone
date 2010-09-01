/*
* Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  This module contains the implementation of CVmAppUi
*                class member functions.
*
*/


// INCLUDE FILES
#include "VoiceMailboxAppPrivateCRKeys.h"
#include "voicemailboxdomaincrkeys.h"
#include "VmAppUi.h"

#include "VmApp.h"

#include "VmContainer.h"

#include <e32property.h>
#include <PSVariables.h>
#include "VmQueryLauncher.h"
#include <vm.rsg>

#include "vm.hrh"

#include <eiklabel.h>
#include <aknlists.h>
#include <aknnotedialog.h>
#include <eikmenup.h>
#include <AknGlobalNote.h>
#include <AknQueryDialog.h>
#include <AknQueryValuePhone.h>
#include <avkon.mbg>

#include <AiwCommon.hrh>    //KAiwCmdCall
#include <AiwServiceHandler.h>      //For Application InterWorking
#include <csxhelp/smsvo.hlp.hrh>  // for help context
#include <hlplch.h>  // for HlpLauncher
#include <featmgr.h>  // Feature Manager

#include <aknnotewrappers.h>
#include <StringLoader.h>

#ifdef __BT_SAP
#include <BTSapDomainPSKeys.h>
#endif
#include <centralrepository.h>

#include "VmDetailsDialog.h"
#include <AknNotify.h>
#include <AknNotifyStd.h>
#include "VMBLogger.h"
#include <gsfwviewuids.h> // For General Settings application UID

const TInt KVmHelpArraySize = 1;
// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CVmAppUi::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CVmAppUi::ConstructL()
    {
    BaseConstructL( 
    EAknEnableSkin | EAknEnableMSK | EAknSingleClickCompatible );
    FeatureManager::InitializeLibL();
    iFeatureManagerInitialized = ETrue;
    iAppUiState = EIdle;

#ifdef __BT_SAP
    if ( FeatureManager::FeatureSupported( KFeatureIdBtSap ) )
        {
        TInt sapState( 0 );
        TInt error = RProperty::Get( KPSUidBluetoothSapConnectionState,
                                     KBTSapConnectionState,
                                     sapState );
        if (  error == KErrNone )
            {
            if ( sapState == EBTSapConnected )
                {
                HBufC* informationNoteTxt = StringLoader::LoadLC(
                                                       R_OFFLINE_NOT_POSSIBLE_SAP );
                TPtrC note = informationNoteTxt->Des();
                CAknGlobalNote* globalNote = CAknGlobalNote::NewLC();
                globalNote->SetGraphic( EMbmAvkonQgn_note_info,
                                                      EMbmAvkonQgn_note_info_mask );
                globalNote->ShowNoteL( EAknGlobalErrorNote, note );
                CleanupStack::PopAndDestroy( globalNote );
                Exit();
                }
            }
        }
#endif //__BT_SAP

    iFromGS = IsStartedFromGSL();

    iAppContainer = new (ELeave) CVmContainer();

    iServiceHandler = CAiwServiceHandler::NewL();
    iAppContainer->ConstructL( ClientRect(), iServiceHandler );
    AddToStackL( iAppContainer );
    iServiceHandler->AttachMenuL( R_VM_MENU, R_VMBX_APP_INTEREST );

    CEikMenuPane* temp =
                (CEikonEnv::Static())->AppUiFactory()->MenuBar()->MenuPane();

    _LIT(KNull, "");
    AddMenuItemL( temp, KNull, EVmCmdAiwPlaceholder );

    iServiceHandler->InitializeMenuPaneL( *temp, R_VM_MENU, EVmCmdAiwCmdLast,
                                         iServiceHandler->InParamListL() );

    // fetch some localised texts from resources
    iTextCallVoiceMail = iEikonEnv->AllocReadResourceL( R_MENU_CALL_VOICEMAIL );

    iTextDefineNumber = iEikonEnv->AllocReadResourceL( R_MENU_DEFINE_NUMBER );
    iTextChangeNumber = iEikonEnv->AllocReadResourceL( R_MENU_CHANGE_NUMBER );
    iTextClearIcon = StringLoader::LoadL( R_CLEAR_ICON );

    if ( FeatureManager::FeatureSupported( KFeatureIdCsVideoTelephony ) )
        {
        iTextCallVideoMail = iEikonEnv->AllocReadResourceL(
            R_MENU_CALL_VIDEOMAIL );
        iTextDefineVideoNumber = iEikonEnv->AllocReadResourceL(
            R_MENU_DEFINE_VIDEO_NUMBER );
        }

    if ( FeatureManager::FeatureSupported( KFeatureIdCommonVoip ) )
        {
        iTextCallIntVoiceMail =
                      iEikonEnv->AllocReadResourceL( R_MENU_CALL_INT_MAILBOX );
        }

    if ( FeatureManager::FeatureSupported( KFeatureIdProtocolCdma ) )
        {
        iTextShowDetails = StringLoader::LoadL( R_OPEN_DETAILS );
        }

    if ( FeatureManager::FeatureSupported( KFeatureIdHelp ) )
        {
        iTextHelp = iEikonEnv->AllocReadResourceL( R_MENU_HELP );
        }

    iTextExit = iEikonEnv->AllocReadResourceL( R_MENU_EXIT );

    if ( !IsIpVoiceMailboxServices() && 
        ( ( !iAppContainer->Number().Length() &&
        !iAppContainer->VideoNumber().Length() ) || 
        ( !iAppContainer->Number().Length() && 
        !iAppContainer->IsVideoSupported() ) ) )
        {
        // If the vmbx number is not defined, query it as soon as
        // the app has been fully constructed.
        iQueryLauncher = CVmQueryLauncher::NewL( *iAppContainer, *this );
        iAppUiState = ELauncherQueryingNumber;
        }

    DrawCbaButtonsL();
    }

// -----------------------------------------------------------------------------
// CVmAppUi::~CVmAppUi
// Destructor
// -----------------------------------------------------------------------------
//
CVmAppUi::~CVmAppUi()
    {
    if ( iServiceHandler )
        {
        delete iServiceHandler;
        }

    if ( iQueryLauncher )
        {
        iQueryLauncher->Cancel();
        delete iQueryLauncher;
        }

    if ( iAppContainer )
        {
        RemoveFromStack( iAppContainer );
        delete iAppContainer;
        }
    delete iTextCallVoiceMail;
    delete iTextDefineNumber;
    delete iTextDefineVideoNumber;
    delete iTextChangeNumber;
    delete iTextShowDetails;
    delete iTextClearIcon;
    delete iTextCallVideoMail;
    delete iTextCallIntVoiceMail;

    delete iTextHelp;
    delete iTextExit;

    if ( iFeatureManagerInitialized )
        {
        FeatureManager::UnInitializeLib();
        }
    }

// ------------------------------------------------------------------------------
// CVmAppUi::DynInitMenuPaneL(TInt aResourceId,CEikMenuPane* aMenuPane)
//  This function is called by the UIKON framework just before it displays
//  a menu pane. Menu items are created.
// ------------------------------------------------------------------------------
void CVmAppUi::DynInitMenuPaneL( TInt aResourceId, CEikMenuPane* aMenuPane )
    {
    VMBLOGSTRING( "VMBX: CVmAppUi::DynInitMenuPaneL: =>" );

    iServiceHandler->InitializeMenuPaneL( *aMenuPane,
                                            aResourceId,
                                            EVmCmdAiwCmdLast,
                                            iServiceHandler->InParamListL() );

    if ( aResourceId == R_VM_MENU )
        {
        aMenuPane->SetItemDimmed( KVmCmdAiwVoiceCall, ETrue );

        if ( FeatureManager::FeatureSupported( KFeatureIdCsVideoTelephony ) )
            {
            aMenuPane->SetItemDimmed( KVmCmdAiwVideoCall, ETrue );
            }

        if ( iAppContainer->IsVoIPProfilesFound() )
            {
            // Internet call command id is same as video call command id
            // when video call is not supported. There is no video call
            // option in menu when video call is not supported.
            TInt intCallMenuPosition =
                FeatureManager::FeatureSupported(
                KFeatureIdCsVideoTelephony ) ?
                KVmCmdAiwVoipCall :
                KVmCmdAiwVideoCall;

            aMenuPane->SetItemDimmed( intCallMenuPosition,ETrue );
            }

        TInt currentItem = iAppContainer->CurrentItem();
        VMBLOGSTRING2( "VMBX: CVmAppUi::DynInitMenuPaneL:currentItem=%d",
                currentItem );
        switch ( currentItem )
            {
            case EVmbxVoiceMailboxNumberInd:
                {
                if ( iAppContainer->Number().Length() )
                    {
                    AddMenuItemL( aMenuPane, *iTextCallVoiceMail,
                                  EvmCmdCallVoiceMail );
                    
                    if ( iAppContainer->AllowedToChangeTheNumber() )
                        {
                        AddMenuItemL( aMenuPane, *iTextChangeNumber,
                                      EvmCmdChangeNumber );
                        }
                    }
                else 
                    {
                    if ( iAppContainer->AllowedToChangeTheNumber() )
                        {
                        AddMenuItemL( aMenuPane, *iTextDefineNumber, 
                                      EvmCmdChangeNumber );
                        }
                    }
                break;
                }
                
            case EVmbxVideoMailboxNumberInd:
                {                    
                if ( iAppContainer->IsVideoSupported() )
                    {
                    if ( iAppContainer->VideoNumber().Length() )
                        {
                        AddMenuItemL( aMenuPane, *iTextCallVideoMail,
                                      EvmCmdCallVideoMail );
                        if ( iAppContainer->AllowedToChangeTheNumber() )
                            {
                            AddMenuItemL( aMenuPane, *iTextChangeNumber,
                                          EvmCmdChangeVideoNumber );
                            }
                        }
                    else
                        {
                        if ( iAppContainer->AllowedToChangeTheNumber() )
                            {
                            AddMenuItemL( aMenuPane, *iTextDefineVideoNumber,
                                          EvmCmdChangeVideoNumber );
                            }
                        }
                    }
                break;
                }
                    
            case EVmbxIntCallVoiceMailboxNumberInd:
                {
                if ( iAppContainer->IsIpVoiceMailboxServices() )
                    {
                    AddMenuItemL( aMenuPane, *iTextCallIntVoiceMail,
                                  EvmCmdCallIntVoiceMail );
                    }
                break;
                }
                
            // CDMA specific:

            case EVmbxVoiceMessageAmountInd:
                {
                if ( FeatureManager::FeatureSupported( KFeatureIdProtocolCdma ) )
                    {
                    if ( iAppContainer->Number().Length() )
                        {
                        AddMenuItemL( aMenuPane, *iTextCallVoiceMail,
                                      EvmCmdCallVoiceMail );
                        }
                    }
                break;
                }
            
            case EVmbxDetailsOfLastMessageInd:
                {
                if ( FeatureManager::FeatureSupported( KFeatureIdProtocolCdma ) )
                    {
                    AddMenuItemL( aMenuPane, *iTextShowDetails,
                                  EvmCmdShowLastMessageDetails );
                    }
                break;
                }


            default:
                {
                break;
                }
            }

        if ( iAppContainer->IsClearIcon() )
            {
            AddMenuItemL( aMenuPane, *iTextClearIcon, EvmCmdClearIcon );
            }

        // Check if help feature is available
        if ( FeatureManager::FeatureSupported( KFeatureIdHelp ) )
            {
            AddMenuItemL( aMenuPane, *iTextHelp, EAknCmdHelp );
            }

        AddMenuItemL( aMenuPane, *iTextExit, EAknCmdExit );
        }

    VMBLOGSTRING( "VMBX: CVmAppUi::DynInitMenuPaneL: <=" );
    }

// -----------------------------------------------------------------------------
// CVmAppUi::AddMenuItemL
// Adds an item in menu
// -----------------------------------------------------------------------------
//
void CVmAppUi::AddMenuItemL( CEikMenuPane* aMenuPane,
                                                 const TDesC& aText,
                                                             TInt aCommandId )
    {
    VMBLOGSTRING( "VMBX: CVmAppUi::AddMenuItemL=>" ); 
    CEikMenuPaneItem::SData data;
    data.iCascadeId = 0;
    data.iFlags = 0;
    data.iCommandId = aCommandId;
    data.iText = aText;
    aMenuPane->AddMenuItemL( data );
    if ( aText.Length() 
         && ( EAknCmdExit != aCommandId ) 
         && ( EAknCmdHelp != aCommandId ) )
        {
        SetItemSpecificL( aMenuPane, aCommandId );
        }
    VMBLOGSTRING2( "VMBX: CVmAppUi::AddMenuItemL:aCommandId=%d",aCommandId );
    VMBLOGSTRING( "VMBX: CVmAppUi::AddMenuItemL<=" ); 
    }

// -----------------------------------------------------------------------------
// CVmAppUi::HandleCommandL
// Handles menu commands and softkey-exit
// -----------------------------------------------------------------------------
//
void CVmAppUi::HandleCommandL( TInt aCommand )
    {
    VMBLOGSTRING( "VMBX: CVmAppUi::HandleCommandL: =>" );
    switch ( aCommand )
        {
        case EEikCmdExit:
        case EAknSoftkeyExit:
        case EAknSoftkeyBack:
            {
            Exit();
            break;
            }
        case EVmCbaMskDefVoMbx:
        case EvmCmdChangeNumber:
            {
            if ( !iAppContainer->AllowedToChangeTheNumber() )
                {
                User::Leave( KErrNotSupported );
                }
            if ( ELauncherQueryingNumber != iAppUiState )
                {
                iAppContainer->QueryNumberL( EVmbxChangeNumberQuery );
                DrawCbaButtonsL();
                }
            break;
            }

        case EVmCbaMskCallVoipMbx:
        case EvmCmdCallIntVoiceMail:
           {
            if ( FeatureManager::FeatureSupported( KFeatureIdCommonVoip ) )
                {
                iAppContainer->CallIntVoiceMailBoxL();
                }

            break;
            }

        case EAknCmdHelp:
            {
            if ( FeatureManager::FeatureSupported( KFeatureIdHelp ) )
                {
                HlpLauncher::LaunchHelpApplicationL( iEikonEnv->WsSession(),
                                                            AppHelpContextL() );
                }
            break;
            }

        case EVmCbaMskCallVoMbx:
        case EvmCmdCallVoiceMail:
            {
            iAppContainer->CallVoiceMailBoxL();
            break;
            }

        case EvmCmdShowLastMessageDetails:
            {
            CVmDetailsDialog* dlg = CVmDetailsDialog::NewL();
            dlg->ExecuteLD( R_VMBX_DETAIL_DIALOG );
            break;
            }

        case EvmCmdClearIcon:
            {
            // Reset counter to 0 in SD
            iAppContainer->ResetVoicemailMessageCounterL();

            // Show confirmation note:
            HBufC* noteText = StringLoader::LoadLC( R_VOICEMAIL_CLEARED );

            CAknConfirmationNote* note = new(ELeave) CAknConfirmationNote();
            note->ExecuteLD( *noteText );

            CleanupStack::PopAndDestroy( noteText );
            break;
            }

        case EVmCbaMskDefViMbx:
        case EvmCmdChangeVideoNumber:
            {
            if ( ELauncherQueryingNumber != iAppUiState )
                {
                iAppContainer->QueryVideoNumberL( EVmbxChangeNumberQuery );
                DrawCbaButtonsL();
                }
            break;
            }

        case EVmCbaMskCallViMbx:
        case EvmCmdCallVideoMail:
            {
            iAppContainer->CallVideoMailBoxL();
            break;
            }
        default:
            break;
        }
    VMBLOGSTRING( "VMBX: CVmAppUi::HandleCommandL: <=" );
    }

// -----------------------------------------------------------------------------
// CVmAppUi::HelpContextL
// Returns the help context
// -----------------------------------------------------------------------------
//
CArrayFix<TCoeHelpContext>* CVmAppUi::HelpContextL() const
    {
    CArrayFixFlat<TCoeHelpContext>* array =
                   new( ELeave ) CArrayFixFlat<TCoeHelpContext>( KVmHelpArraySize );

    // Cannot leave because the buffer for the appended item is allocated
    // in the constructor.
    array->AppendL( TCoeHelpContext( KUidvm, KSMSVO_HLP_MAIN_VIEW ) );

    return array;
    }

// -----------------------------------------------------------------------------
// CVmAppUi::DrawCbaButtonsL
// Loads the appropriate CBA buttons for MSK.
// -----------------------------------------------------------------------------
//
void CVmAppUi::DrawCbaButtonsL()
    {
    VMBLOGSTRING( "VMBX: CVmAppUi::DrawCbaButtonsL: =>" );

    CEikButtonGroupContainer* cba = Cba();
    TInt currentItem = iAppContainer->CurrentItem();
    TInt resourceId = 0;

    switch ( currentItem )
        {
        case EVmbxVoiceMailboxNumberInd:
            {
            if ( !iAppContainer->Number().Length())
                {
                if ( iFromGS )
                    {
                    if ( iAppContainer->AllowedToChangeTheNumber() )
                        {
                        resourceId = R_VMBX_GS_CBA_DEFINE_VO_NUMBER;
                        }
                    else
                        {
                        resourceId = R_VMBX_GS_CBA_NOTALLOW_DEFINE_VO_NUMBER;
                        }
                    }
                else
                    {
                    if ( iAppContainer->AllowedToChangeTheNumber() )
                        {
                        resourceId = R_VMBX_CBA_DEFINE_VO_NUMBER;
                        }
                    else
                        {
                        resourceId = R_VMBX_CBA_NOTALLOW_DEFINE_VO_NUMBER;
                        }
                    }
                }
            else
                {
                if(iFromGS)
                    {
                    resourceId = R_VMBX_GS_CBA_CALL_VO_MAILBOX;
                    }
                else
                    {
                    resourceId = R_VMBX_CBA_CALL_VO_MAILBOX;
                    }
                }
            break;
            }
        case EVmbxVideoMailboxNumberInd:
            {
            if ( !iAppContainer->VideoNumber().Length() )
                {
                if ( iFromGS )
                    {
                    if ( iAppContainer->AllowedToChangeTheNumber() )
                        {
                        resourceId = R_VMBX_GS_CBA_DEFINE_VI_NUMBER;
                        }
                    else
                        {
                        resourceId = R_VMBX_GS_CBA_NOTALLOW_DEFINE_VI_NUMBER;
                        }
                    }
                else
                    {
                    if ( iAppContainer->AllowedToChangeTheNumber() )
                        {
                        resourceId = R_VMBX_CBA_DEFINE_VI_NUMBER;
                        }
                    else
                        {
                        resourceId = R_VMBX_CBA_NOTALLOW_DEFINE_VI_NUMBER;
                        }
                    }
                }
            else
                {
                if ( iFromGS )
                    {
                    resourceId = R_VMBX_GS_CBA_CALL_VI_MAILBOX;
                    }
                else
                    {
                    resourceId = R_VMBX_CBA_CALL_VI_MAILBOX;
                    }
                }
            break;
            }
        case EVmbxIntCallVoiceMailboxNumberInd:
            {
            if ( FeatureManager::FeatureSupported( KFeatureIdCommonVoip ) )
                {
                if ( iFromGS )
                    {
                    resourceId = R_VMBX_GS_CALL_VOIP_MAILBOX;
                    }
                else
                    {
                    resourceId = R_VMBX_CBA_CALL_VOIP_MAILBOX;
                    }
                }
            break;
            }
       default:
            break;
        }

    cba->SetCommandSetL( resourceId );
    cba->DrawDeferred();
    VMBLOGSTRING( "VMBX: CVmAppUi::DrawCbaButtonsL: <=" );
    }

// -----------------------------------------------------------------------------
// CVmAppUi::HandleForegroundEventL
// Handles changes in keyboard focus when an application switches to or from
// the foreground.
// -----------------------------------------------------------------------------
//
void CVmAppUi::HandleForegroundEventL( TBool aForeground )
    {
    if ( aForeground ) // back to foreground
        {
        if ( iAppContainer->IsServiceUpdated() )
            {
            iAppContainer->UpdateListboxL();
            }

        // get numbers
        iAppContainer->UpdateVmbxNumberL();
        // change state so that FocusChanged will update ListBox fields
        iAppContainer->UpdateData( ETrue );
        }
    CAknAppUi::HandleForegroundEventL( aForeground );
    }

// -----------------------------------------------------------------------------
// CVmAppUi::IsStartedFromGSL
// Defines whether voice mail application is started from general
// settings or not.
// -----------------------------------------------------------------------------
//
TBool CVmAppUi::IsStartedFromGSL()
    {
    TBool result = EFalse;
    RWsSession ws;
    User::LeaveIfError( ws.Connect() );
    CleanupClosePushL( ws );

    // Find the task with uid
    TApaTaskList taskList( ws );
    TApaTask task = taskList.FindApp( KUidGS );

    if ( task.Exists() )
        {
        // read window group id of general settings
        TInt gsTaskId = task.WgId();

        // Find task from position 1 (background)
        TApaTask bgTask = taskList.FindByPos(1);

        // Read window group id of background task
        TInt bgTaskId = bgTask.WgId();

        if( gsTaskId == bgTaskId )
        	{
        	// General settings is straight below voice mail application
        	// => application is started from general settings
        	result = ETrue;
        	}
        else
        	{
        	result = EFalse;
        	}
        }
    else
    	{
    	// General settings task does not exist
    	result = EFalse;
    	}
    CleanupStack::PopAndDestroy( ); // ws

    return result;
    }

// -----------------------------------------------------------------------------
// CVmAppUi::NumberQueryComplete
//
//
// -----------------------------------------------------------------------------
void CVmAppUi::NumberQueryComplete()
    {
    iAppUiState = EIdle;
    }

// -----------------------------------------------------------------------------
// CVmAppUi::GetAppUiState
//
//
// -----------------------------------------------------------------------------
TVmbxAppUiState CVmAppUi::GetAppUiState()
    {
    return iAppUiState;
    }

// -----------------------------------------------------------------------------
// CVmAppUi::IsIpVoiceMailboxServices
//
//
// -----------------------------------------------------------------------------   
TBool CVmAppUi::IsIpVoiceMailboxServices()
    {
    return iAppContainer->IsIpVoiceMailboxServices();
    }

// -----------------------------------------------------------------------------
// CVmAppUi::SetItemSpecific
//
//
// -----------------------------------------------------------------------------
void CVmAppUi::SetItemSpecificL( CEikMenuPane* aMenuPane, TInt aCommandId )
    {
    VMBLOGSTRING2( "VMBX: CVmAppUi::SetItemSpecificL:aCommandId=%d =>",
    aCommandId );
    if ( aMenuPane )
        {
        aMenuPane->SetItemSpecific( aCommandId, ETrue );
        }
    else
        {
        User::Leave( KErrArgument );
        }
    VMBLOGSTRING( "VMBX: CVmAppUi::SetItemSpecificL: <=" );
    }

// End of File
