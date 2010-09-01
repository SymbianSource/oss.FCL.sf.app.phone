/*
* Copyright (c) 2002 - 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Container for Voice mailbox application.
*
*/

// INCLUDE FILES
#include <aknlists.h>
#include <AknDef.h>
#include <AknsUtils.h>
#include <AknsConstants.h>
#include <AknsUtils.h>
#include <AknsBasicBackgroundControlContext.h>
#include <badesca.h>
#include <aknkeys.h>
#include <gulicon.h>
#include <featmgr.h>
#include <StringLoader.h>
#include <data_caging_path_literals.hrh>

#include <PbkFields.hrh>
#include <PhCltUtils.h>
#include <AiwServiceHandler.h> //For Application InterWorking
#include <NumberGrouping.h>
#include <NumberGroupingCRKeys.h>
#include <settingsinternalcrkeys.h>
#include <NetworkHandlingDomainPSKeys.h>
#include <centralrepository.h>
#include <AiwDialDataTypes.h>
#include <aiwdialdataext.h>

#include <spsettings.h>
#include <vmnumber.h>
#include <vm.rsg>
#include <vm.mbg>
#include "vm.hrh"

#include "VoiceMailboxAppPrivateCRKeys.h"
#include "voicemailboxdomaincrkeys.h"
#include "VMBLogger.h"
#include "VmContainer.h"
#include "VmApp.h"
#include "VmAppUi.h"

// CONSTANTS

_LIT( KVmNoIconPrefix, "0\t");       // no icon
_LIT( KVmSimIconPrefix, "1\t");      // CS icon, number is stored in sim
_LIT( KVmwCSIconPrefix, "2\t");      // CS icon, number is stored in phone memory
_LIT( KVmwVideoIconPrefix, "3\t");   // default icon to Video
_LIT( KVmwVoIPIconPrefix,  "4\t");   // default icon to VoIP

_LIT( KVmSuffix, "\t");
//for the mbm file
_LIT( KVmMbmDrive, "Z:");
_LIT( KVmLibMbmFile, "vm.mbm");

const TInt KVmClearIcon = 0x02;
const TInt KVmMailboxNumber = 0x04;
const TInt KVmMessageDetails = 0x08;
const TInt KVmPrefixLength = 3;
const TInt KVmIconArraySize = 2;
const TInt KFormattedCharLength = 3;

// ================= MEMBER FUNCTIONS ==========================================

// -----------------------------------------------------------------------------
// CVmContainer::CVmContainer
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//

CVmContainer::CVmContainer()
    : iArray( 2 ),
      iVmUiOpts( 0 )
    {
    }


// -----------------------------------------------------------------------------
// CVmContainer::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
/*****************************************************
*      Series 60 Customer / ETel
*      Series 60  ETel API
*****************************************************/
void CVmContainer::ConstructL( const TRect& aRect,
                                           CAiwServiceHandler* aServiceHandler )
    {
    VMBLOGSTRING( "VMBX: CVmContainer::ConstructL: =>" );
    CreateWindowL();
    SetBlank();

    iServiceProviderSettings = CSPSettings::NewL();

    //Create repository and notify handlers.
    iSession = CRepository::NewL( KCRUidVoiceMailbox );
    iNotifyHandler1 = CCenRepNotifyHandler::NewL( *this, *iSession,
                          CCenRepNotifyHandler::EIntKey, KVmMessageCounter );
    iNotifyHandler2 = CCenRepNotifyHandler::NewL( *this, *iSession,
                          CCenRepNotifyHandler::EIntKey, KVmSummaryInfoIsSet );
    iNotifyHandler3 = CCenRepNotifyHandler::NewL( *this, *iSession,
                          CCenRepNotifyHandler::EIntKey, KVideoMbxSupport );
    iNotifyHandler1->StartListeningL();
    iNotifyHandler2->StartListeningL();
    iNotifyHandler3->StartListeningL();

    FeatureManager::InitializeLibL();
    iFeatureManagerInitialized = ETrue;

    TInt result( KErrNone );
    if ( FeatureManager::FeatureSupported( KFeatureIdProtocolCdma ) )
        {
        result = iSession->Get( KVmUIOpts, iVmUiOpts );

        if ( result != KErrNone )
            {
            if ( result == KErrNotFound )
                {
                iVmUiOpts = 0;
                }
            else
                {
                VMBLOGSTRING2( "VMBX: CVmContainer::ConstructL: CenRep error: %d", result );
                User::Leave( result );
                }
            }
        }

    result = iSession->Get( KVideoMbxSupport, iIsVideoSupported );
    if ( result != KErrNone )
        {
        iIsVideoSupported = EFalse;
        }

    // connect to phoneclient
    iServiceHandler  = aServiceHandler;

    // open vmbx number interface
    User::LeaveIfError( iVmbxNumber.Open() );

    // get the vmbx number of the active ALS line
    TInt ret( iVmbxNumber.GetVmbxNumber( iNumber ) );

    if ( ret != KErrNone && ret != KErrNotFound )
        {
        User::Leave( ret );
        }

    if ( IsVideoSupported() )
        {
        // get the video mailbox number of the active ALS line
        ret = iVmbxNumber.GetVideoMbxNumber( iVideoNumber );
        if ( ret != KErrNone && ret != KErrNotFound )
             {
             User::Leave( ret );
             }
        }

    TInt pngErr( KErrNone );
    TInt pngSupported( 0 );
    CRepository* repository = CRepository::NewL( KCRUidNumberGrouping );
    pngErr = repository->Get( KNumberGrouping, pngSupported );
    delete repository;

    if ( KErrNone == pngErr && pngSupported != 0 )
        {
        iNumberGrouping = CPNGNumberGrouping::NewL( KVmbxMaxNumberLength );
        }

    // construct the listbox object
    iListBox = new (ELeave) CAknDoubleGraphicStyleListBox;
    iListBox->SetContainerWindowL( *this );

    if ( FeatureManager::FeatureSupported( KFeatureIdCommonVoip ) )
        {
        iListBox->ConstructL( this, EAknListBoxMultiselectionList );
        }
    else
        {
        iListBox->ConstructL( this, EAknListBoxSelectionList );
        }

    // load the two localised strings for the listbox
    if (  FeatureManager::FeatureSupported( KFeatureIdProtocolCdma ) )
        {
        iLine1 = StringLoader::LoadL( R_MAILBOX_NUMBER );
        }
    else
        {
        iLine1 = iEikonEnv->AllocReadResourceL( R_CALL_VOICEMAIL );
        }
    iLine2 = iEikonEnv->AllocReadResourceL( R_NUMBER_PANE_EMPTY );

    if ( FeatureManager::FeatureSupported( KFeatureIdCsVideoTelephony ) )
        {
        iVideoLine1 = iEikonEnv->AllocReadResourceL( R_CALL_VIDEOMAIL );
        iVideoLine2 = iEikonEnv->AllocReadResourceL( R_NUMBER_PANE_EMPTY );
        }

    if ( FeatureManager::FeatureSupported( KFeatureIdCommonVoip ) )
        {
        iLine3 = iEikonEnv->AllocReadResourceL( R_INT_CALL_VOICEMAIL );
        iLine4 = iEikonEnv->AllocReadResourceL( R_ADDRESS_PANE_EMPTY );
        }

    FillIndArray();

    // Set the text in the listbox
    // First create space for the items
    for ( TInt i = 0; i < iIndArray.Count(); i++ )
        {
        iArray.AppendL( KNullDesC );
        }

    //construct the list icon array, empty icon and sim icon
    ConstructIconsForListboxL();
    //set the icon array to the list box
    iListBox->ItemDrawer()->FormattedCellData()->SetIconArrayL( iListIcons );
    iListBox->SetListBoxObserver( this );

    // New voice messages
    UpdateNewVoiceMessagesInListBoxL();

    // Voice mailbox number
    UpdateVmbxNumberInListBoxL();

    // Details
    UpdateLastMessageDetailsInListBoxL();

    iListBox->Model()->SetItemTextArray( &iArray );
    iListBox->Model()->SetOwnershipType( ELbmDoesNotOwnItemArray );
    iListBox->ItemDrawer()->FormattedCellData()->EnableMarqueeL( ETrue );
    iListBox->CreateScrollBarFrameL( ETrue );
    iListBox->ScrollBarFrame()->SetScrollBarVisibilityL(
                        CEikScrollBarFrame::EOff, CEikScrollBarFrame::EAuto );

    TRect rect( 0, 0, 0, 0 );
    // Temporary rect is passed. Correct rect is set in SizeChanged.
    iSkinContext = CAknsBasicBackgroundControlContext::NewL(
        KAknsIIDQsnBgAreaMainCalc, rect, EFalse );

    SetRect( aRect );
    ActivateL();
    DrawNow();

    // request notify on ALS line changes
    iVmbxNumber.NotifyAlsLineChangeL( this );

    // request notify on vmbx number changes
    iVmbxNumber.NotifyVmbxNumberChangeL( this, ETrue );

    // request notify on Service changes
    iVmbxNumber.NotifyServiceChange( this );

    UpdateData( EFalse );
    iIsCallOngoing = EFalse;

    VMBLOGSTRING( "VMBX: CVmContainer::ConstructL: <=" );
    }


// -----------------------------------------------------------------------------
// CVmContainer::~CVmContainer
// Destructor
// -----------------------------------------------------------------------------
//
CVmContainer::~CVmContainer()
    {
    VMBLOGSTRING( "VMBX: CVmContainer::~CVmContainer: =>" );

    delete iServiceProviderSettings;
    
    delete iListBox;

    iArray.Reset();
    
    delete iLine1;
    delete iLine2;
    delete iVideoLine1;
    delete iVideoLine2;
    delete iLine3;
    delete iLine4;
    
   
    iVmbxNumber.NotifyServiceChangeCancel();

    iVmbxNumber.Close();
    if ( iNotifyHandler1 )
        {
        iNotifyHandler1->StopListening();
        delete iNotifyHandler1;
        }

    if ( iNotifyHandler2 )
        {
        iNotifyHandler2->StopListening();
        delete iNotifyHandler2;
        }
    
    if ( iNotifyHandler3 )
        {
        iNotifyHandler3->StopListening();
        delete iNotifyHandler3;
        }

    delete iSession;
    delete iNumberGrouping;

    iIndArray.Close();

    if ( iFeatureManagerInitialized )
        {
        FeatureManager::UnInitializeLib();
        }

    delete iSkinContext;

    VMBLOGSTRING( "VMBX: CVmContainer::~CVmContainer: <=" );
    }

// -----------------------------------------------------------------------------
// CVmContainer::SizeChanged
// Called by framework when the view size is changed
// -----------------------------------------------------------------------------
//
void CVmContainer::SizeChanged()
    {
    if ( iSkinContext )
        {
        iSkinContext->SetRect( TRect( TPoint( 0, 0 ), Rect().Size() ) );
        }
    AknLayoutUtils::LayoutControl( iListBox, Rect(),
                                        AKN_LAYOUT_WINDOW_list_gen_pane( 0 ) );

    }

// -----------------------------------------------------------------------------
// CVmContainer::CountComponentControls
// Gets a count of the component controls
// -----------------------------------------------------------------------------
//
TInt CVmContainer::CountComponentControls() const
    {
    if( FeatureManager::FeatureSupported( KFeatureIdCommonVoip ) )
        {
        return 2; // the listbox is the only control
        }
    else
        {
        return 1; // the listbox is the only control
        }
    }

// -----------------------------------------------------------------------------
// CVmContainer::ComponentControl
// Gets an indexed component of a compound control
// -----------------------------------------------------------------------------
//
CCoeControl* CVmContainer::ComponentControl( TInt /*aIndex*/ ) const
    {
    return iListBox;
    }

// -----------------------------------------------------------------------------
// CVmContainer::AllowedToChangeTheNumber
// Check wether is possible to edit the voice mailbox number.
// -----------------------------------------------------------------------------
//
TBool CVmContainer::AllowedToChangeTheNumber()
    {
    VMBLOGSTRING( "VMBX: CVmContainer::AllowedToChangeTheNumber: =>" );
    VMBLOGSTRING( "VMBX: CVmContainer::AllowedToChangeTheNumber: <=" );
    return iVmbxNumber.AllowedToChangeTheNumber();
    }


// -----------------------------------------------------------------------------
// CVmContainer::OfferKeyEventL
// Handles key events
// -----------------------------------------------------------------------------
//
TKeyResponse CVmContainer::OfferKeyEventL( const TKeyEvent& aKeyEvent,
                                                             TEventCode aType )
    {
    VMBLOGSTRING( "VMBX: CVmContainer::OfferKeyEventL: =>" );
    // For CDMA:
    if ( FeatureManager::FeatureSupported( KFeatureIdProtocolCdma ) )
        {
        if ( aType == EEventKey )
            {
            TInt currentItem = CurrentItem();
            if ( currentItem != KErrNotFound )
                {
                if ( aKeyEvent.iCode == EKeyPhoneSend )
                    {
                    if ( currentItem == EVmbxVoiceMessageAmountInd
                                 || currentItem == EVmbxVoiceMailboxNumberInd )
                        {
                        CallVoiceMailBoxL();
                        VMBLOGSTRING( "VMBX: CVmContainer::OfferKeyEventL: Send key" );
                        return EKeyWasConsumed;
                        }
                    }

                if ( aKeyEvent.iCode == EKeyOK )
                    {
                    if ( currentItem == EVmbxVoiceMessageAmountInd )
                        {
                        CallVoiceMailBoxL();
                        VMBLOGSTRING( "VMBX: CVmContainer::OfferKeyEventL: OK key" );
                        return EKeyWasConsumed;
                        }
                    else if ( currentItem == EVmbxVoiceMailboxNumberInd )
                        {
                        if ( AllowedToChangeTheNumber() )
                            {
                            iAvkonAppUi->HandleCommandL( EvmCmdChangeNumber );
                            }
                        else
                            {
                            CallVoiceMailBoxL();
                            }
                        VMBLOGSTRING( "VMBX: CVmContainer::OfferKeyEventL: OK key" );
                        return EKeyWasConsumed;
                        }
                    else if ( currentItem == EVmbxIntCallVoiceMailboxNumberInd &&
                              FeatureManager::FeatureSupported( KFeatureIdCommonVoip ) )
                        {
                        VMBLOGSTRING( "VMBX: CVmContainer::OfferKeyEventL: OK key" );
                        return EKeyWasConsumed;
                        }
                    else if ( currentItem == EVmbxDetailsOfLastMessageInd )
                        {
                        iAvkonAppUi->HandleCommandL( EvmCmdShowLastMessageDetails );
                        VMBLOGSTRING( "VMBX: CVmContainer::OfferKeyEventL: OK key" );
                        return EKeyWasConsumed;
                        }
                    }
                }
            }

        return iListBox->OfferKeyEventL( aKeyEvent, aType );
        }

    // General variant:
    if ( aType == EEventKey )
        {
        TInt currentItem = CurrentItem();
        // pressing selection/SEND/enter key initiates a voice call
        if ( ( aKeyEvent.iCode == EKeyOK
               || aKeyEvent.iCode == EKeyPhoneSend
               || aKeyEvent.iCode == EKeyEnter )
            && ( currentItem == EVmbxVoiceMessageAmountInd
                 || currentItem == EVmbxVoiceMailboxNumberInd ) )
            {
            CallVoiceMailBoxL();
            }
        else if ( ( aKeyEvent.iCode == EKeyOK
                    || aKeyEvent.iCode == EKeyPhoneSend
                    || aKeyEvent.iCode == EKeyEnter )
                && ( FeatureManager::FeatureSupported( KFeatureIdCommonVoip )
                && currentItem != EVmbxVoiceMessageAmountInd
                && currentItem != EVmbxVoiceMailboxNumberInd
                && currentItem != EVmbxVideoMailboxNumberInd) )
            {
            CallIntVoiceMailBoxL();
            }

        else if ( IsVideoSupported()
                  && ( aKeyEvent.iCode == EKeyOK
                      || aKeyEvent.iCode == EKeyPhoneSend
                      || aKeyEvent.iCode == EKeyEnter )
                  && currentItem == EVmbxVideoMailboxNumberInd )
            {
            CallVideoMailBoxL();
            }

        if ( ( aKeyEvent.iCode == EKeyUpArrow || aKeyEvent.iCode == EKeyDownArrow ) &&
             ( IsVideoSupported() ||
               FeatureManager::FeatureSupported( KFeatureIdCommonVoip ) ) )
            {
            TKeyResponse response( iListBox->OfferKeyEventL( aKeyEvent,
                                                                    aType ) );
            iListBox->SetCurrentItemIndex( iListBox->CurrentItemIndex() );
            ( static_cast <CVmAppUi*>iAvkonAppUi ) ->DrawCbaButtonsL();
            VMBLOGSTRING2( "VMBX: CVmContainer::OfferKeyEventL: \
            response = %I", response );
            return response;
            }
        }
    VMBLOGSTRING( "VMBX: CVmContainer::OfferKeyEventL: <=" );
    return EKeyWasConsumed;
    }

// -----------------------------------------------------------------------------
// CVmContainer::HandleListBoxEventL
// Handles listbox clicks.
// -----------------------------------------------------------------------------
//
void CVmContainer::HandleListBoxEventL(
    CEikListBox* /*aListBox*/,
    TListBoxEvent aEventType)
    {
    VMBLOGSTRING( "VMBX: HandleListBoxEventL: =>" );
    VMBLOGSTRING2( "VMBX: HandleListBoxEventL: aEventType=%d", aEventType );
    switch ( aEventType )
        {
        case EEventItemSingleClicked:
            {
            // at begining,if Querying number, 
            // the key envent should not be response
            if( ELauncherQueryingNumber != 
                    ( static_cast <CVmAppUi*>iAvkonAppUi )->GetAppUiState() )
                {
                TKeyEvent aKeyEvent;
                aKeyEvent.iCode = EKeyOK;
                VMBLOGSTRING( "VMBX: HandleListBoxEventL: OfferKeyEventL" );
                OfferKeyEventL( aKeyEvent, EEventKey );
                }
            VMBLOGSTRING( "VMBX: HandleListBoxEventL: EEventItemSingleClicked" );
            break;
            }
        default:
            VMBLOGSTRING( "VMBX: HandleListBoxEventL: default" );
            break;
        }

    VMBLOGSTRING( "VMBX: HandleListBoxEventL: <=" );
    }

// -----------------------------------------------------------------------------
// CVmContainer::HandleResourceChange
// Notifier for changing orientation
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CVmContainer::HandleResourceChange( TInt aType )
    {
    VMBLOGSTRING( "VMBX: HandleResourceChange: =>" );
    TRect mainPaneRect ;
    if ( aType == KEikDynamicLayoutVariantSwitch )
        {
        VMBLOGSTRING( "VMBX: HandleResourceChange: DynamicLayout" );
        iListBox->HandleResourceChange( aType );
        AknLayoutUtils::LayoutMetricsRect( AknLayoutUtils::EMainPane,
                                                                 mainPaneRect );
        TRAP_IGNORE( CEikonEnv::Static()->AppUiFactory()->StatusPane()->
                            SwitchLayoutL( R_AVKON_STATUS_PANE_LAYOUT_USUAL ) );
        SetRect( mainPaneRect );
        DrawNow();
        }
    CCoeControl::HandleResourceChange( aType );
    VMBLOGSTRING( "VMBX: HandleResourceChange: <=" );
    }

// -----------------------------------------------------------------------------
// CVmContainer::Draw
// Redraw the window owned by this container
// -----------------------------------------------------------------------------
//
void CVmContainer::Draw( const TRect& aRect ) const
    {
    VMBLOGSTRING( "VMBX: Draw: =>" );
    CWindowGc& gc = SystemGc();
    gc.Clear( aRect );

    // Drawing skin
    if ( iSkinContext )
        {
        MAknsSkinInstance* skin = AknsUtils::SkinInstance();
        AknsDrawUtils::Background( skin, iSkinContext, this, gc, aRect );
        }
    VMBLOGSTRING( "VMBX: Draw: <=" );
    }
// -----------------------------------------------------------------------------
// CVmContainer::QueryNumberL
// Queries for a new voice mailbox number. If the query
// is approved, the listbox is updated.
// -----------------------------------------------------------------------------
//
void CVmContainer::QueryNumberL( TVmbxQueryType aQueryType )
    {
    VMBLOGSTRING( "VMBX: CVmContainer::QueryNumberL: =>" );
    TBool ret( EFalse );
    TRAPD( queryErr, 
        ret = iVmbxNumber.QueryNumberL( aQueryType, iNumber, ETrue ) );

    if ( KErrNone == queryErr )
        {
        if ( ret )
            {
            // if the query was approved, update vmbx number
            UpdateVmbxNumberInListBoxL();
            (static_cast <CVmAppUi*>iAvkonAppUi) ->DrawCbaButtonsL();
            iListBox->DrawNow();
            VMBLOGSTRING( "VMBX: CVmContainer::QueryNumberL: \
            update vmbx number listbox" );
            }
        }
    else
        {
        VMBLOGSTRING2( "VMBX: CVmContainer::QueryNumberL: \
        queryErr = %d", queryErr );
        
        if ( KErrAccessDenied == queryErr )
            {
            VMBLOGSTRING( "VMBX: CVmContainer::QueryNumberL: \
            not allow to change number" );
            }
        else
            {
            User::Leave( queryErr );
            }
        }
    VMBLOGSTRING( "VMBX: CVmContainer::QueryNumberL: <=" );
    }

// -----------------------------------------------------------------------------
// CVmContainer::UpdateNewVoiceMessagesInListBoxL
// Updates the new voice message to the listbox
// -----------------------------------------------------------------------------
//
void CVmContainer::UpdateNewVoiceMessagesInListBoxL()
    {
    VMBLOGSTRING( "VMBX: CVmContainer::UpdateNewVoiceMessagesInListBoxL: =>" );
    if ( !FeatureManager::FeatureSupported( KFeatureIdProtocolCdma ) )
        {
        VMBLOGSTRING( "VMBX: CVmContainer::UpdateNewVoiceMessagesInListBoxL:\
        Cdma not supported" );
        return;
        }

    // First line
    HBufC* firstLineText = StringLoader::LoadLC( R_VOICE_MESSAGES );

    // Second line
    HBufC* secondLineText = NULL;
    TInt voiceMessageAmout = 0;

    TInt err = iSession->Get( KVmMessageCounter, voiceMessageAmout );
    if ( err != KErrNotFound )
        {
        VMBLOGSTRING2( "VMBX: CVmContainer::UpdateNewVoiceMessagesInListBoxL:\
        CenRep error: %d", err );
        User::LeaveIfError( err );
        }

    if ( voiceMessageAmout == 0 )
        {
        secondLineText = StringLoader::LoadLC( R_NO_NEW );
        }
    else if ( voiceMessageAmout == 1 )
        {
        secondLineText = StringLoader::LoadLC( R_NEW_MESSAGES );
        }
    else
        {
        secondLineText = StringLoader::LoadLC( R_N_NEW, voiceMessageAmout );
        }

    // Create combination:
    HBufC* comboText = HBufC::NewLC( firstLineText->Length() +
                                    secondLineText->Length() +
                                    KFormattedCharLength ); // 3 special chars

    TPtr comboPtr = comboText->Des();
    comboPtr.Append( KVmNoIconPrefix );
    comboPtr.Append( *firstLineText );
    comboPtr.Append( KVmSuffix );
    comboPtr.Append( *secondLineText );

    TInt ind = iIndArray.Find( EVmbxVoiceMessageAmountInd );
    if ( ind != KErrNotFound )
        {
        iArray.Delete( ind );
        iArray.InsertL( ind, comboPtr );
        }

    CleanupStack::PopAndDestroy( comboText );
    CleanupStack::PopAndDestroy( secondLineText );
    CleanupStack::PopAndDestroy( firstLineText );
    VMBLOGSTRING( "VMBX: CVmContainer::UpdateNewVoiceMessagesInListBoxL: <=" );
    }

// -----------------------------------------------------------------------------
// CVmContainer::UpdateVmbxNumberInListBoxL
// Updates the vmbx number in the listbox shown on the app screen.
// -----------------------------------------------------------------------------
//
void CVmContainer::UpdateVmbxNumberInListBoxL()
    {
    VMBLOGSTRING( "VMBX: CVmContainer::UpdateVmbxNumberInListBoxL: =>" );
    if ( FeatureManager::FeatureSupported( KFeatureIdProtocolCdma ) &&
        !( iVmUiOpts & KVmMailboxNumber ) )
        {
        VMBLOGSTRING( "VMBX: CVmContainer::UpdateNewVoiceMessagesInListBoxL: \
        Cdma supported or/and no vmbxnumber support" );
        return;
        }

    // allocate buffer for icon, title, vmxb number and suffix
    HBufC* buf = HBufC::NewLC( KVmNoIconPrefix().Length() +
                               iLine1->Length() +
                               KVmSuffix().Length() +
                               KVmbxMaxNumberLength );
        
    TPtr bufPtr = buf->Des();

    // listbox formatting
    if ( iVmbxNumber.IsSimMemory() && iNumber.Length() )
        {
        VMBLOGSTRING( "UpdateNewVoiceMessagesInListBoxL::Show SIM icon" );
        bufPtr.Append( KVmSimIconPrefix );
        }
    else
        {
        VMBLOGSTRING( "UpdateNewVoiceMessagesInListBoxL::Show CS icon, number stored to phone" );
        bufPtr.Append( KVmwCSIconPrefix );
        }

    bufPtr.Append( *iLine1 );  // "Voice Mailbox"-text (localised)
    bufPtr.Append( KVmSuffix );

    if ( !iNumber.Length() )
        {
        bufPtr.Append( *iLine2 );       // "No number"-text (localised)
        }
    else
        {
        // Convert the phone number to accommodate the current locale
        TBuf<KVmbxMaxNumberLength> locNumber;

        if ( iNumberGrouping )
            {
            iNumberGrouping->Set( iNumber );
            locNumber.Copy( iNumberGrouping->FormattedNumber() );
            }
        else
            {
            locNumber.Copy( iNumber );
            }

        AknTextUtils::LanguageSpecificNumberConversion( locNumber );

        bufPtr.Append( locNumber );
        }

    TInt ind = iIndArray.Find( EVmbxVoiceMailboxNumberInd );
    if ( ind  != KErrNotFound )
        {
        iArray.Delete( ind );
        iArray.InsertL( ind, bufPtr );
        }
    CleanupStack::PopAndDestroy( buf );

    // ========================================================================
    // Video Mailbox specific part starts from here
    // ========================================================================

    if ( IsVideoSupported() )
        {
        TInt videoline2MaxLength( iVideoLine2->Length() );

        if ( KVmbxMaxNumberLength > videoline2MaxLength )
            {
            videoline2MaxLength = KVmbxMaxNumberLength;
            }

        // allocate buffer for 2 listbox lines and 3 formatter chars
        HBufC* videoBuf = HBufC::NewLC( iVideoLine1->Length() +
                                            videoline2MaxLength +
                                            KVmNoIconPrefix().Length() +
                                            KVmSuffix().Length() );

        TPtr videoBufPtr = videoBuf->Des();

        // listbox formatting
        videoBufPtr.Append( KVmwVideoIconPrefix );
        videoBufPtr.Append( *iVideoLine1 );  // "Video Mailbox"-text (localised)
        videoBufPtr.Append( KVmSuffix );

        if ( !iVideoNumber.Length() )
            {
            videoBufPtr.Append( *iVideoLine2 );  // "No number"-text (localised)
            }
        else
            {
            // Convert the phone number to accommodate the current locale
            TBuf<KVmbxMaxNumberLength> videoLocNumber;

            if ( iNumberGrouping )
                {
                iNumberGrouping->Set( iVideoNumber );
                videoLocNumber.Copy( iNumberGrouping->FormattedNumber() );
                }
            else
                {
                videoLocNumber.Copy( iVideoNumber );
                }

            AknTextUtils::LanguageSpecificNumberConversion( videoLocNumber );

            videoBufPtr.Append( videoLocNumber );
            }

        TInt videoInd = iIndArray.Find( EVmbxVideoMailboxNumberInd );
        if ( videoInd != KErrNotFound )
            {
            iArray.Delete( videoInd );
            iArray.InsertL( videoInd, videoBufPtr );
            }
        CleanupStack::PopAndDestroy( videoBuf );
        }
    
    // ========================================================================
    // IP Mailbox specific part starts from here
    // ========================================================================
    
    if ( IsIpVoiceMailboxServices() )
        {
        // Get all Service Ids
        RArray<TUint> profileIds;
        CleanupClosePushL( profileIds );
        iVmbxNumber.GetServiceIds( profileIds );

        TBuf<KVmbxMaxAddressLength> vmbxAddress;

        for ( TInt i( 0 ); i < profileIds.Count(); i++ )
            {
            // Get mailbox name
            TVmbxServiceInfo serviceInfo;
            serviceInfo.iServiceId = profileIds[ i ];
            iVmbxNumber.GetServiceInfo( serviceInfo );
            HBufC* name = HBufC::NewLC( serviceInfo.iName.Length() );
            name->Des().Copy( serviceInfo.iName );

            HBufC* mailbox = StringLoader::LoadLC(
                R_VOIP_LIST_MAILBOX_NAME, name->Des( ) );

            HBufC* buf2 = HBufC::NewLC( mailbox->Length()
                                        + KVmbxMaxNumberLength
                                        + KVmNoIconPrefix().Length()
                                        + KVmSuffix().Length() * 3 );
            TPtr bufPtr2 = buf2->Des();

            // Brand icon here, if brand icon is not available
            // use default VoIP icon
            TInt iconId( KErrNotFound );
            TRAPD( error, AddBrandIconL( profileIds[ i ], iconId ) );

            if ( KErrNone == error && KErrNotFound != iconId )
                {
                bufPtr2.AppendNum( iconId );
                bufPtr2.Append( KVmSuffix );
                }
            else
                {
                bufPtr2.Append( KVmwVoIPIconPrefix );
                }

            // Add Mailbox text
            bufPtr2.Append( mailbox->Des() );
            bufPtr2.Append( KVmSuffix );

            // Write address
            vmbxAddress.Copy( serviceInfo.iAddress );

            if ( !vmbxAddress.Length() )
                {
                bufPtr2.Append( *iLine4 );  // "No number"-text (localised)
                }
            else
                {
                bufPtr2.Append( vmbxAddress );
                }

            // this corrects SIM icon to the right place
            bufPtr2.Append( KVmSuffix );

            ind = iIndArray.Find( EVmbxIntCallVoiceMailboxNumberInd );
            if ( KErrNotFound != ind )
                {
                VMBLOGSTRING2( "VMBX: CVmContainer::UpdateVmbxNumberInListBoxL: %S", buf2 );
                iArray.Delete( ind + i );
                iArray.InsertL( ind + i, bufPtr2 );
                }

            CleanupStack::PopAndDestroy( buf2 );
            CleanupStack::PopAndDestroy( mailbox );
            CleanupStack::PopAndDestroy( name );
            }
        CleanupStack::PopAndDestroy( &profileIds );
        }
    VMBLOGSTRING( "VMBX: CVmContainer::UpdateVmbxNumberInListBoxL: <=" );
    }

// -----------------------------------------------------------------------------
// CVmContainer::UpdateLastMessageDetailsInListBoxL
// Updates the last message details to the listbox
// -----------------------------------------------------------------------------
//
void CVmContainer::UpdateLastMessageDetailsInListBoxL()
    {
    VMBLOGSTRING( "VMBX: CVmContainer::UpdateLastMessageDetailsInListBoxL: =>" );
    if ( !FeatureManager::FeatureSupported( KFeatureIdProtocolCdma ) ||
         !( iVmUiOpts & KVmMessageDetails ) )
        {
        VMBLOGSTRING( "VMBX: CVmContainer::UpdateNewVoiceMessagesInListBoxL: \
         Cdma supported or/and no vmbxdetails support" );
        return;
        }

    // Determine the position of this item in the list (if any)
    TInt ind = iIndArray.Find( EVmbxDetailsOfLastMessageInd );

    // if the last message is not set, hide the item:
    // 0 = not set
    // 1 = set.
    TInt messageSet = 0;

    iSession->Get( KVmSummaryInfoIsSet, messageSet ); // ignore error

    if ( messageSet == 0 )
        {
        if ( ind != KErrNotFound )
            {
            // remove the item from the list
            iIndArray.Remove( ind );
            iArray.Delete( ind );
            }
        return;
        }

    HBufC* resourceText = StringLoader::LoadLC( R_DETAILS_OF_LAST );

    // Three special chars
    HBufC* detailsText = HBufC::NewLC( KVmPrefixLength +
                                                    resourceText->Length() );
    TPtr detailsPtr = detailsText->Des();
    detailsPtr.Append( KVmNoIconPrefix );
    detailsPtr.Append( *resourceText );
    detailsPtr.Append( KVmSuffix );
    detailsPtr.Append( KNullDesC ); // the second line is empty

    if ( ind == KErrNotFound )
        {
        // Add the item back to the list
        iIndArray.Append( EVmbxDetailsOfLastMessageInd );
        iArray.AppendL( detailsPtr );
        }

    CleanupStack::PopAndDestroy( detailsText );
    CleanupStack::PopAndDestroy( resourceText );
    VMBLOGSTRING( "VMBX: CVmContainer::\
    UpdateLastMessageDetailsInListBoxL: <=" );
    }

// -----------------------------------------------------------------------------
// CVmContainer::CallVoiceMailBoxL
// Calls the vmbx number using PhoneClient.
// -----------------------------------------------------------------------------
//
/*****************************************************
*      Series 60 Customer / ETel
*      Series 60  ETel API
*****************************************************/
void CVmContainer::CallVoiceMailBoxL()
    {
    VMBLOGSTRING( "VMBX: CVmContainer::CallVoiceMailBoxL: =>" );

    if ( !iIsCallOngoing )
        {
        AiwCallRequestL( KVmCmdAiwVoiceCall, CAiwDialData::EAIWVoice );
        }

    VMBLOGSTRING( "VMBX: CVmContainer::CallVoiceMailBoxL: <=" );
    }


// -----------------------------------------------------------------------------
// CVmContainer::CallVoiceMailBoxL
// Calls the vmbx number using PhoneClient.
// -----------------------------------------------------------------------------
//
/*****************************************************
*      Series 60 Customer / ETel
*      Series 60  ETel API
*****************************************************/
void CVmContainer::CallIntVoiceMailBoxL()
    {
    VMBLOGSTRING( "VMBX: CVmContainer::CallVoiceMailBoxL: =>" );

    if ( !iIsCallOngoing )
        {
        AiwCallRequestL( KVmCmdAiwVoipCall, CAiwDialData::EAIWVoiP );
        }

    VMBLOGSTRING( "VMBX: CVmContainer::CallVoiceMailBoxL: <=" );
    }


// -----------------------------------------------------------------------------
// CVmContainer::HandleNotifyL
// When user has completed an action to a vmbx number
// -----------------------------------------------------------------------------
//
void CVmContainer::HandleNotifyL( TInt /*aLine*/, const TDesC& aValue )
    {
    VMBLOGSTRING( "VMBX: CVmContainer::HandleNotifyL: =>" );
    iNumber.Copy( aValue );
    UpdateVmbxNumberInListBoxL();
    (static_cast <CVmAppUi*>iAvkonAppUi) ->DrawCbaButtonsL();
    iListBox->DrawNow();
    VMBLOGSTRING( "VMBX: CVmContainer::HandleNotifyL: <=" );
    }

// -----------------------------------------------------------------------------
// CVmContainer::HandleAlsChangeL
// ALS line changed
// -----------------------------------------------------------------------------
//
void CVmContainer::HandleAlsChangeL( TVmbxAlsLine /*aLine*/ )
    {
    VMBLOGSTRING( "VMBX: CVmContainer::HandleAlsChangeL: =>" );
    VMBLOGSTRING( "VMBX: CVmContainer::HandleAlsChangeL: <=" );
    }

// -----------------------------------------------------------------------------
// CVmContainer::UpdateVmbxNumberL
// Gets the vmbx number from SIM/SharedData and updates it in the display.
// -----------------------------------------------------------------------------
//
void CVmContainer::UpdateVmbxNumberL()
    {
    VMBLOGSTRING( "VMBX: CVmContainer::UpdateVmbxNumberL: =>" );
    TInt ret ( iVmbxNumber.GetVmbxNumber( iNumber ) );

    if ( ret == KErrNotFound )
        {
        iNumber.Zero();
        }
    else if ( ret != KErrNone )
        {
        User::LeaveIfError( ret );
        }

    UpdateVmbxNumberInListBoxL();
    iListBox->DrawNow();
    (static_cast <CVmAppUi*>iAvkonAppUi) ->DrawCbaButtonsL();
    VMBLOGSTRING( "VMBX: CVmContainer::UpdateVmbxNumberL: <=" );
    }

// -----------------------------------------------------------------------------
// CVmContainer::HandleNotifyInt
// Integer value keys has pressed
// -----------------------------------------------------------------------------
//
void CVmContainer::HandleNotifyInt( TUint32 aKey, TInt aValue )
    {
    VMBLOGSTRING( "VMBX: CVmContainer::HandleNotifyInt: =>" );
    if ( aKey == KVmMessageCounter)
        {
        TRAPD( err1, UpdateNewVoiceMessagesInListBoxL() );
        if ( err1 != KErrNone )
            {
            _LIT(KVmbxPanicType1,"UpdateNewVoiceMessagesInListBoxL");
            User::Panic( KVmbxPanicType1, err1 );
            }
        iListBox->DrawNow();
        }
    else if ( aKey == KVmSummaryInfoIsSet )
        {
        TInt currentItem = CurrentItem();
        if ( currentItem == EVmbxDetailsOfLastMessageInd )
            {
            TInt newInd = iIndArray.Find( EVmbxVoiceMailboxNumberInd );
            if ( newInd != KErrNotFound )
                {
                iListBox->SetCurrentItemIndex( newInd );
                }
            }
        TRAPD( err2, UpdateLastMessageDetailsInListBoxL() );
        if ( err2 != KErrNone )
            {
            _LIT(KVmbxPanicType2,"UpdateLastMessageDetailsInListBoxL");
            User::Panic( KVmbxPanicType2, err2 );
            }
        TRAPD( err3, iListBox->HandleItemAdditionL() );
        if ( err3 != KErrNone )
            {
            _LIT(KVmbxPanicType3,"iListBox->HandleItemAdditionL");
            User::Panic( KVmbxPanicType3, err3 );
            }
        }
    else if ( aKey == KVideoMbxSupport )
        {
        iIsVideoSupported = aValue;
        UpdateData( ETrue );
        TRAP_IGNORE( UpdateListboxL() );
        }
    VMBLOGSTRING( "VMBX: CVmContainer::HandleNotifyInt: <=" );
    }

// -----------------------------------------------------------------------------
// CVmContainer::ResetVoicemailMessageCounterL
// Resets to zero the voicemail message counter in SD
// -----------------------------------------------------------------------------
//
void CVmContainer::ResetVoicemailMessageCounterL()
    {
    VMBLOGSTRING( "VMBX: CVmContainer::ResetVoicemailMessageCounterL: =>" );

    TInt err = iSession->Set( KVmMessageCounter, 0 );
    if ( err != KErrNotFound )
        {
        VMBLOGSTRING2( "VMBX: CVmContainer::ResetVoicemailMessageCounterL: \
        CenRep error: %d", err );
        User::LeaveIfError( err );
        }

    UpdateNewVoiceMessagesInListBoxL();
    iListBox->DrawNow();
    VMBLOGSTRING( "VMBX: CVmContainer::ResetVoicemailMessageCounterL: <=" );
    }

// -----------------------------------------------------------------------------
// CVmContainer::IsClearIcon
// Checks if clear icon is set
// -----------------------------------------------------------------------------
//
TBool CVmContainer::IsClearIcon()
    {
    VMBLOGSTRING( "VMBX: CVmContainer::IsClearIcon: =>" );
    VMBLOGSTRING( "VMBX: CVmContainer::IsClearIcon: <=" );
    return iVmUiOpts & KVmClearIcon;
    }

// -----------------------------------------------------------------------------
// CVmContainer::FillIndArray
// Fill array with indications
// -----------------------------------------------------------------------------
//
void CVmContainer::FillIndArray()
    {
    VMBLOGSTRING( "VMBX: CVmContainer::FillIndArray: =>" );
    if ( FeatureManager::FeatureSupported( KFeatureIdProtocolCdma ) )
        {
        //NewVoiceMessages
        iIndArray.Append( EVmbxVoiceMessageAmountInd );

        if ( iVmUiOpts & KVmMailboxNumber )
            {
            //VmbxNumber
            iIndArray.Append( EVmbxVoiceMailboxNumberInd );
            }

         // Details (EVmbxDetailsOfLastMessageInd) are added dynamically in
         // UpdateLastMessageDetailsInListBoxL, always to the end of the list
         }
    else
        {
        //VmbxNumber
        iIndArray.Append( EVmbxVoiceMailboxNumberInd );
        }

    if ( IsVideoSupported() )
        {
        iIndArray.Append( EVmbxVideoMailboxNumberInd );
        }

    if ( FeatureManager::FeatureSupported( KFeatureIdCommonVoip ) )
        {
        RArray<TUint> profileIds;
        iVmbxNumber.GetServiceIds( profileIds );

        for ( TInt i( 0 ); i < profileIds.Count(); i++ )
            {
            VMBLOGSTRING( "VMBX: CVmContainer::FillIndArray \
                           Appending Voip mailbox service" );
            iIndArray.Append( EVmbxIntCallVoiceMailboxNumberInd );
            }

        profileIds.Close();
        }
    VMBLOGSTRING( "VMBX: CVmContainer::FillIndArray: <=" );
    }

// -----------------------------------------------------------------------------
// CVmContainer::CurrentItem
// Gets index  of the current item in listbox
// -----------------------------------------------------------------------------
//
TInt CVmContainer::CurrentItem()
    {
    VMBLOGSTRING( "VMBX: CVmContainer::CurrentItem: =>" );
    if ( iListBox->CurrentItemIndex() >= iIndArray.Count() )
        {
        VMBLOGSTRING( "VMBX: CVmContainer::CurrentItem: \
        listbox iten not found" );
        return KErrNotFound;
        }
    TInt listBoxCurrentIndex = iListBox->CurrentItemIndex();
    VMBLOGSTRING2( "VMBX: CVmContainer::CurrentItem: listBoxCurrentIndex=%d",
    listBoxCurrentIndex );
    TInt currentItemIndex = iIndArray[ listBoxCurrentIndex ];
    VMBLOGSTRING2( "VMBX: CVmContainer::CurrentItem: currentItemIndex=%d <=",
    currentItemIndex );
    return currentItemIndex;
    }

// -----------------------------------------------------------------------------
// CVmContainer::AskTypeL
// Ask Vmbx type.
// -----------------------------------------------------------------------------
//
TInt CVmContainer::AskTypeL()
    {
    VMBLOGSTRING( "VMBX: CVmContainer::AskTypeL: =>" );
    TInt type( EVmbx );
    
    if ( IsVideoSupported() )
        {
        type = iVmbxNumber.SelectTypeL( EVmbxDefine );
        }
    
    VMBLOGSTRING( "VMBX: CVmContainer::AskTypeL: <=" );
    return type;
    }

// -----------------------------------------------------------------------------
// CVmContainer::IsVoIPProfilesFound
// Check if VoIP profiles is found.
// -----------------------------------------------------------------------------
//
TBool CVmContainer::IsVoIPProfilesFound()
    {
    VMBLOGSTRING( "VMBX: CVmContainer::IsVoIPProfilesFound" );

    return iVmbxNumber.IsVoIPProfilesFound();
   }

// -----------------------------------------------------------------------------
// CVmContainer::QueryVideoNumberL
// Queries for a new video mailbox number. If the query
// is approved, the listbox is updated.
// -----------------------------------------------------------------------------
//
void CVmContainer::QueryVideoNumberL( TVmbxQueryType aQueryType )
    {
    if ( IsVideoSupported() )
        {
        TBool ret( EFalse );
        TRAPD( queryErr, 
            ret = iVmbxNumber.QueryVideoMbxNumberL( aQueryType, iVideoNumber ) );

        if ( KErrNone == queryErr )
            {
            if ( ret )
                {
                // if the query was approved, update vmbx number
                UpdateVmbxNumberInListBoxL();
                (static_cast <CVmAppUi*>iAvkonAppUi) ->DrawCbaButtonsL();
                iListBox->DrawNow();
                }
            }
         else
            {
            VMBLOGSTRING2( "VMBX: CVmContainer::QueryVideoMbxNumberL: \
            queryErr = %d", queryErr );
            
            if ( KErrAccessDenied == queryErr )
                {
                VMBLOGSTRING( "VMBX: CVmContainer::QueryVideoMbxNumberL: \
                not allow to change number" );
                }
            else
                {
                User::Leave( queryErr );
                }
            }
        }
    }

// -----------------------------------------------------------------------------
// CVmContainer::FocusChanged
// Indicate the requirements for control redrawing.
// -----------------------------------------------------------------------------
//
void CVmContainer::FocusChanged( TDrawNow /*aDrawNow*/ )
    {
    VMBLOGSTRING( "VMBX: CVmContainer::FocusChanged: =>" );
    TInt ret1( KErrNone );
    TInt ret2( KErrNone );

    // Wwhen returned from other application to vmbx.
    // Not necessary to update every time when focus changed.
    if ( iUpdateData )
        {
        // get numbers, possible changed from Idle or General settings
        ret1 = iVmbxNumber.GetVmbxNumber( iNumber );
        if ( IsVideoSupported() )
            {
            ret2 = iVmbxNumber.GetVideoMbxNumber( iVideoNumber );
            }
        }
    if ( iListBox )
        {
        if ( iUpdateData )
            {
            if ( ( ret1 == KErrNone || ret1 == KErrNotFound) &&
                 ( ret2 == KErrNone || ret2 == KErrNotFound) )
                {
                TRAP_IGNORE( UpdateVmbxNumberInListBoxL() );
                TRAP_IGNORE(
                    ( static_cast <CVmAppUi*>iAvkonAppUi ) ->DrawCbaButtonsL() );
                }
            UpdateData( EFalse );
            }
        iListBox->SetFocus( IsFocused() );
        }
    iIsCallOngoing = EFalse;
    VMBLOGSTRING( "VMBX: CVmContainer::FocusChanged: <=" );
    }

// -----------------------------------------------------------------------------
// CVmContainer::ConstructIconsForListboxL
// Constructs the icons that are needed to indicate where the vmbx number
// is stored
// -----------------------------------------------------------------------------
//
void CVmContainer::ConstructIconsForListboxL()
    {
    VMBLOGSTRING( "VMBX: CVmContainer::ConstructIconsForListboxL: =>" );
    iListIcons = new (ELeave) CAknIconArray( KVmIconArraySize );

    CFbsBitmap* bitmap;
    CFbsBitmap* mask;

    //Get the mbm file path
    TFileName mbmfile( KVmMbmDrive );
    mbmfile.Append( KDC_APP_BITMAP_DIR );
    mbmfile.Append( KVmLibMbmFile );

    MAknsSkinInstance* skin = AknsUtils::SkinInstance();

    //empty bitmap
    AknsUtils::CreateIconL( skin, KAknsIIDQgnPropEmpty, bitmap,mask, mbmfile,
            EMbmVmQgn_prop_empty, EMbmVmQgn_prop_empty_mask );

    CGulIcon* icon = CGulIcon::NewL( bitmap, mask );
    iListIcons->AppendL( icon );

    bitmap = NULL;
    mask = NULL;

    //sim icon
    AknIconUtils::CreateIconL( bitmap, mask, mbmfile,
                                EMbmVmQgn_prop_nrtyp_sim_contact,
                                EMbmVmQgn_prop_nrtyp_sim_contact_mask );

    CGulIcon* icon1 = CGulIcon::NewL( bitmap, mask );
    iListIcons->AppendL( icon1 );
    // Add default icons(CS, Video and VoIP) to the icon list
    iVmbxNumber.ConstructDefaultIconsL( iListIcons );
    VMBLOGSTRING2( "VMBX: CVmContainer::ConstructIconsForListboxL: count: %d", iListIcons->Count() );

    CTextListBoxModel* model = iListBox->Model();
    CDesCArray* items = static_cast<CDesCArray*>( model->ItemTextArray() );
    items->Reset();

    // Update listbox according new values..
    iListBox->SetCurrentItemIndex( 0 );
    iListBox->HandleItemAdditionL();
    VMBLOGSTRING( "VMBX: CVmContainer::ConstructIconsForListboxL: <=" );
    }

// -----------------------------------------------------------------------------
// CVmContainer::CallVideoMailBoxL
// Calls the vmbx number using PhoneClient
// -----------------------------------------------------------------------------
//
void CVmContainer::CallVideoMailBoxL()
    {
    VMBLOGSTRING( "VMBX: CVmContainer::CallVideoMailBoxL: =>" );

    if ( !iIsCallOngoing )
        {
        AiwCallRequestL( KVmCmdAiwVideoCall, CAiwDialData::EAIWForcedVideo );
        }

    VMBLOGSTRING( "VMBX: CVmContainer::CallVideoMailBoxL: <=" );
    }

// -----------------------------------------------------------------------------
// CVmContainer::AiwCallRequestL()
// Makes a call request to AiwProvider.
// -----------------------------------------------------------------------------
//
void CVmContainer::AiwCallRequestL( TInt aCommand, 
                                    CAiwDialData::TCallType aCallType )
    {
    VMBLOGSTRING( "VMBX: CVmContainer::AiwCallRequestL: =>" );
    TBuf<KVmbxMaxNumberLength> number;

    if ( aCallType == CAiwDialData::EAIWVoice )
        {
        VMBLOGSTRING( "VMBX: CVmContainer::AiwCallRequestL: Type: VoiceCall" );
        number.Copy( iNumber );

        if ( !iNumber.Length() )
            {
            QueryNumberL( EVmbxNotDefinedQuery );
            VMBLOGSTRING( "VMBX: CVmContainer::AiwCallRequestL: \
                No VoiceCall number: <=" );
            return;
            }
        VMBLOGSTRING( "VMBX: CVmContainer::AiwCallRequestL: \
            VoiceCall number copied" );
        number.Copy( iNumber );
        }

    if ( aCallType == CAiwDialData::EAIWForcedVideo )
        {
        if ( !iVideoNumber.Length() )
            {
            QueryVideoNumberL( EVmbxNotDefinedQuery );
            VMBLOGSTRING( "VMBX: CVmContainer::AiwCallRequestL: \
                No VideoCall number: <=" );
            return;
            }
        VMBLOGSTRING( "VMBX: CVmContainer::AiwCallRequestL: \
            VideoCall number copied" );
        number.Copy( iVideoNumber );
        }

    TVmbxServiceInfo serviceInfo;
    
    if ( aCallType == CAiwDialData::EAIWVoiP)
        {
        VMBLOGSTRING( "VMBX: CVmContainer::AiwCallRequestL: Type: VoipCall" );
        if ( !FeatureManager::FeatureSupported( KFeatureIdCsVideoTelephony ) )
            {
            // Internet call command id is the same as video call command id
            // when video call is not supported.
            aCommand = KVmCmdAiwVideoCall;
            }

        serviceInfo.iServiceId = ServiceId();

        TInt error( iVmbxNumber.GetServiceInfo( serviceInfo ) );

        if ( KErrNone == error )
            {
            number.Copy( serviceInfo.iAddress );
            }

        VMBLOGSTRING2( "VMBX: AiwCallRequestL: VoIP address error: %d", error );
        VMBLOGSTRING2( "VMBX: AiwCallRequestL: VoIP address: %S", &number );
        VMBLOGSTRING2( "VMBX: AiwCallRequestL: VoIP serviceId: %d", serviceInfo.iServiceId );
        }

    // Create a dial data object and push it into the cleanup stack.
    CAiwDialDataExt* dialData = CAiwDialDataExt::NewLC();
    
    VMBLOGSTRING( "VMBX: CVmContainer::AiwCallRequestL: \
        AIW dialData object created" );

    // Set up dial data parameters.
    dialData->SetPhoneNumberL( number );
    dialData->SetCallType( aCallType );
    dialData->SetServiceId( serviceInfo.iServiceId );

    // Set up CAiwGenericParamList object.
    CAiwGenericParamList& paramList = iServiceHandler->InParamListL();
    dialData->FillInParamListL( paramList );

    // Execute AIW menu service command.
    iIsCallOngoing = ETrue;
    VMBLOGSTRING( "VMBX: CVmContainer::AiwCallRequestL: Execute AIW menu \
        service command" );
    iServiceHandler->ExecuteMenuCmdL( aCommand,
                                      paramList,
                                      iServiceHandler->OutParamListL(),
                                      0,
                                      NULL );

    // Dial data is no longer needed and can be deleted.
    CleanupStack::PopAndDestroy( dialData );
    VMBLOGSTRING( "VMBX: CVmContainer::AiwCallRequestL: <=" );
    }


// -----------------------------------------------------------------------------
// CVmContainer::Number
// Query for a voice mailbox number.
// -----------------------------------------------------------------------------
//
const TDesC& CVmContainer::Number() const
    {
    return iNumber;
    }

// -----------------------------------------------------------------------------
// CVmContainer::VideoNumber
// Query for a video mailbox number.
// -----------------------------------------------------------------------------
//
const TDesC& CVmContainer::VideoNumber() const
    {
    return iVideoNumber;
    }

// -----------------------------------------------------------------------------
// CVmContainer::UpdateData
// LisBox will be updated with data possibly changed in other application
// -----------------------------------------------------------------------------
//
void CVmContainer::UpdateData( TBool aState )
    {
    iUpdateData = aState;
    }

// -----------------------------------------------------------------------------
// CVmContainer::ServiceId
//
// -----------------------------------------------------------------------------
//
TUint CVmContainer::ServiceId()
    {
    TUint serviceId( 0 );
    TInt decrease( 0 );

    RArray<TUint> profileIds;
    iVmbxNumber.GetServiceIds( profileIds );

    // Count how many items we have to decrease
    for ( TInt i( 0 ); i < iIndArray.Count(); i++ )
        {
        if ( EVmbxIntCallVoiceMailboxNumberInd != iIndArray[ i ] )
            {
            decrease++;
            }
        }

    serviceId = profileIds[ iListBox->CurrentItemIndex() - decrease ];
    profileIds.Close();

    VMBLOGSTRING2( "VMBX: CVmContainer::ServiceId() return %d", serviceId );    
    return serviceId;
    }


// -----------------------------------------------------------------------------
// CVmContainer::HandleServiceNotifyL
// VoIP profile has changed
// -----------------------------------------------------------------------------
//
void CVmContainer::HandleServiceNotifyL()
    {
    VMBLOGSTRING( "VMBX: CVmContainer::HandleServiceNotifyL: =>" );

    iServiceUpdated = ETrue;

    VMBLOGSTRING( "VMBX: CVmContainer::HandleServiceNotifyL: <=" );
    }

// -----------------------------------------------------------------------------
// CVmContainer::AddBrandIconL
//
// -----------------------------------------------------------------------------
//
void CVmContainer::AddBrandIconL( TUint aServiceId, TInt& aIconId )
    {
    // Set branding icon to context pane if available.
    TBuf8<KVmSettingsUiBrandingIdLength> brandId( KNullDesC8 );

    iVmbxNumber.BrandIdL( aServiceId, brandId );

    CFbsBitmap* brandedBitmap = NULL;
    CFbsBitmap* brandedBitmapMask = NULL;

    // Get branded bitmap
    TRAPD( err, iVmbxNumber.GetBrandedIconL(
        brandId, brandedBitmap, brandedBitmapMask ) );

    VMBLOGSTRING2( "VMBX: CVmContainer::AddBrandIconL: error: %d", err );

    if ( KErrNone == err )
        {
        // The count of list icons(before adding branding icon to the list)
        // must be the branded icon list id
        aIconId = iListIcons->Count();
        // Create new icon and add it to the icon list
        iListIcons->AppendL(
            CGulIcon::NewL( brandedBitmap, brandedBitmapMask ) );
        }
    else
        {
        delete brandedBitmap;
        delete brandedBitmapMask;
        }
    }

// -----------------------------------------------------------------------------
// CVmContainer::UpdateListboxL
//
// -----------------------------------------------------------------------------
//
void CVmContainer::UpdateListboxL()
    {
    VMBLOGSTRING( "VMBX: CVmContainer::UpdateListboxL: =>" );

    iIndArray.Reset();
    iArray.Reset();

    FillIndArray();

    // Set the text in the listbox
    // First create space for the items
    for ( TInt i = 0; i < iIndArray.Count(); i++ )
        {
        iArray.AppendL( KNullDesC );
        }

    // New voice messages
    UpdateNewVoiceMessagesInListBoxL();

    // Voice mailbox number
    UpdateVmbxNumberInListBoxL();

    // Details
    UpdateLastMessageDetailsInListBoxL();

    KErrNotFound == iListBox->CurrentItemIndex() ?
        iListBox->SetCurrentItemIndex( 0 ) :
            iListBox->SetCurrentItemIndex( iListBox->CurrentItemIndex() );

    ( static_cast <CVmAppUi*>iAvkonAppUi )->DrawCbaButtonsL();

    // Update listbox according new values..
    iListBox->HandleItemAdditionL();
    iListBox->DrawNow();

    // Reset updating value
    iServiceUpdated = EFalse;

    VMBLOGSTRING( "VMBX: CVmContainer::UpdateListboxL: <=" );
    }

// -----------------------------------------------------------------------------
// CVmContainer::IsServiceUpdated
//
// -----------------------------------------------------------------------------
//
TBool CVmContainer::IsServiceUpdated() const
    {
    return iServiceUpdated;
    }

TBool CVmContainer::IsIpVoiceMailboxServices()
    {
    return iVmbxNumber.IsIpVoiceMailboxServices();
    }

TBool CVmContainer::IsVideoSupported()
    {
    if ( iIsVideoSupported &&
         FeatureManager::FeatureSupported( KFeatureIdCsVideoTelephony ) )
        {
        return ETrue;
        }
    else
        {
        return EFalse;
        }
    }

// End of File

