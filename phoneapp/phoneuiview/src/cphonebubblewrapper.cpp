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
* Description: Implementation of CPhoneBubbleWrapper class.
*
*/


// INCLUDE FILES
#include <AknPhoneNumberEditor.h>
#include <featmgr.h>
#include <StringLoader.h>
#include <telephonyvariant.hrh>
#include <eikspane.h>
#include <layoutmetadata.cdl.h>
#include <bautils.h>
#include <bmtouchpaneinterface.h>
#include <AknUtils.h>
#include <spsettings.h>
#include <spproperty.h>

#include "cphonebubblewrapper.h"
#include "cphonebubblemapping.h"
#include "tphonecommandparam.h"
#include "cphonecallheadertextanimation.h"
#include "cphonemainresourceresolver.h"
#include "tphonecmdparamboolean.h"
#include "tphonecmdparaminteger.h"
#include "tphonecmdparamstring.h"
#include "tphonecmdparamkeyevent.h"
#include "tphonecmdparamdynmenu.h"
#include "tphonecmdparamcallheaderdata.h"
#include "tphonecmdparamemergencycallheaderdata.h"
#include "tphonecmdparamcallstatedata.h"
#include "tphonecmdparamincallindicatordata.h"
#include "cphonestatuspane.h"
#include "phoneconstants.h"
#include "phonerssbase.h"
#include "cphonecenrepproxy.h"
#include "phonelogger.h"
#include "mphonevideoplayerobserver.h"
#include "mphoneviewcommandhandle.h"
#include "phoneui.pan"
#include "mnumberentry.h"
#include "mphonenumberentry.h"
#include "cphonenumberentry.h"
#include "phonebubbleextensionmanager.h"

#include "mphonecustomization.h"

// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CPhoneBubbleWrapper::CPhoneBubbleWrapper
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPhoneBubbleWrapper::CPhoneBubbleWrapper( ) :
    iNumberEntryVisible( EFalse ),
    iConference( EFalse ),
    iCipheringIndicatorAllowed( ETrue ),
    iFlags ( 0 ),
    iVideoCallAnimationHeader( NULL ),
    iCallImageTextSupported ( EFalse ),
    iVideoPlayerObserver ( NULL ),
    iDialerNumberEntry ( NULL ),
    iUseDialer ( EFalse ),
    iIsNumberEntryModeNumeric( ETrue )
    {
    }

// -----------------------------------------------------------------------------
// CPhoneBubbleWrapper::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPhoneBubbleWrapper::ConstructL(
    CCoeControl* aControl,
    const TRect& aRect )
    {
    TPoint position ( 0, 0 );

    // Construct bubble manager
    iBubbleManager = CBubbleManager::NewL(
        *aControl,
        aRect,
        ETrue  );
    iBubbleManager->SetPosition( position );

    CCoeControl* bubbleNe = iBubbleManager->GetNumberEntry()->Parent();
    //cannot set bubblewrapper as an observer straight to CAknPhoneNumberEditor
    //because BMBubbleOutlookNE will not receive control events then.
    bubbleNe->SetObserver( this );
    
    // Construct extension manager
    iBubbleExtensionManager = CPhoneBubbleExtensionManager::NewL(
           *iBubbleManager );

    iPhoneNumberEntry = CPhoneNumberEntry::NewL( *iBubbleManager );

    // Construct bubble mapping
    iMapping = CPhoneBubbleMapping::NewL( KMaxNumberOfCallerBubbles );

    if( FeatureManager::FeatureSupported( KFeatureIdCallImagetext ) )
	    {
	    iCallTheme = CPhoneCallThemeMonitor::NewL();
	    iCallImageTextSupported = ETrue;
	    iBubbleManager->PreloadCallThemeImage(
	            iCallTheme->CallImageThemeSettings() == 2  );
	    iCallTheme->SetCallThemeSettingsObserver( this );
	    }

    if ( FeatureManager::FeatureSupported( KFeatureIdOnScreenDialer ) )
        {
        iUseDialer = ETrue;
        }
    }

// -----------------------------------------------------------------------------
// CPhoneBubbleWrapper::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPhoneBubbleWrapper* CPhoneBubbleWrapper::NewL(
    CCoeControl* aControl,
    const TRect& aRect )
    {
    CPhoneBubbleWrapper* self = new( ELeave ) CPhoneBubbleWrapper( );

    CleanupStack::PushL( self );
    self->ConstructL( aControl, aRect );
    CleanupStack::Pop( self );

    return self;
    }

// Destructor
CPhoneBubbleWrapper::~CPhoneBubbleWrapper()
    {
    delete iBubbleExtensionManager;
    delete iPhoneNumberEntry;
    delete iMapping;
    delete iBubbleManager;
    delete iVideoCallAnimationHeader;
    if( iParticipants )
	    {
	    iParticipants->Reset();
    	delete iParticipants;
	    }
	delete iCallTheme;

	iVideoPlayerObserver = NULL;
	iDialerNumberEntry = NULL;
	
    }

// -----------------------------------------------------------------------------
// CPhoneBubbleWrapper.::RemoveCallHeaderL
// -----------------------------------------------------------------------------
//
void CPhoneBubbleWrapper::RemoveCallHeaderL( TInt aCallId )
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneBubbleWrapper::RemoveCallHeader");
    CBubbleManager::TBubbleId bubble;

    if ( iMapping->FindBubbleByCall( aCallId, bubble ) )
        {
        iBubbleManager->StartChanges();

		if( aCallId == KVideoTelephonyCallId1 ||
			aCallId == KVideoTelephonyCallId2 )
    		{
    		CPhoneCallHeaderTextAnimation::InstanceL()->RemoveAnimatingVideoCallHeader();
    		delete iVideoCallAnimationHeader;
    		iVideoCallAnimationHeader = NULL;
    		}

        // remove the bubble
		iBubbleExtensionManager->StopCall( bubble );
        iBubbleManager->RemoveCallHeader( bubble );
        iBubbleManager->EndChanges();
        iMapping->RemoveFromMapping( aCallId );

        __PHONELOG1(
            EBasic,
            EPhoneUIView,
            "CPhoneBubbleWrapper::RemoveCallHeader ShownHeaderCount = %d",
            iBubbleManager->ShownHeaderCount() );

		if (!FeatureManager::FeatureSupported( KFeatureIdTouchCallHandling ) &&
		    iConference && ( iBubbleManager->ShownHeaderCount() == 1 ) )
            {
            SetConferenceExpand( ETrue );
            }
        }
    }

// -----------------------------------------------------------------------------
// CPhoneBubbleWrapper.::CreateCallHeaderL
// -----------------------------------------------------------------------------
//
CBubbleManager::TBubbleId CPhoneBubbleWrapper::CreateCallHeaderL(
    TInt aCallId,
    TPhoneCommandParam *aCommandParam )
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneBubbleWrapper::CreateCallHeader");
    CBubbleManager::TBubbleId bubble;

    if ( iMapping->FindBubbleByCall( aCallId, bubble ) )
        {
        // Invalid - don't create a bubble
        return bubble;
        }

    TPhoneCmdParamCallHeaderData* callHeaderParam =
        static_cast<TPhoneCmdParamCallHeaderData*>( aCommandParam );

    iCipheringIndicatorAllowed = callHeaderParam->CipheringIndicatorAllowed();

    iBubbleManager->StartChanges();
    // Create call header
    bubble = iBubbleManager->CreateCallHeader();
    iMapping->AddToMappingL( bubble, aCallId, CBubbleManager::ENone, NULL );

    if ( iConference )
	    {
	    SetConferenceExpand( EFalse );
	    }

    // Set call state
    PrepareCallStateL(
        aCallId,
        callHeaderParam->CallState(),
        callHeaderParam->LabelText(),
        callHeaderParam->ShortLabelText() );

    // Set call flag ( EDivert... )    
    iBubbleManager->SetCallFlags( 
        bubble, 
        callHeaderParam->CallFlag() );

	 __PHONELOG1(
	    EBasic,
	    EPhoneUIView,
	    "CPhoneBubbleWrapper::CreateCallHeader callHeaderParam->CLIText = %S",
	    &callHeaderParam->CLIText() );

    // Set CLI text
    iBubbleManager->SetCLI(
        bubble,
        callHeaderParam->CLIText(),
        callHeaderParam->CLITextClippingDirection() );
    
    // Set CNAP text if applicable
    iBubbleManager->SetCNAP( 
            bubble, 
            callHeaderParam->CNAPText(), 
            callHeaderParam->CNAPTextClippingDirection() );
    
    // Set CLI type to be used in conference call participant list
    CBubbleManager::TBubbleParticipantListCLI partipantCli =
        ( callHeaderParam->ParticipantCLI() ==
          TPhoneCmdParamCallHeaderData::EPhoneParticipantCNAPText ) ?
        CBubbleManager::EParticipantListCNAP :
        CBubbleManager::EParticipantListCLIText;
    iBubbleManager->SetParticipantListCLI( bubble, partipantCli );

    // Set ciphering indicator
    iBubbleManager->SetCallFlag(
        bubble,
        CBubbleManager::ENoCiphering,
        iCipheringIndicatorAllowed &&
        !callHeaderParam->Ciphering() );

	 __PHONELOG1(
	    EBasic,
	    EPhoneUIView,
	    "CPhoneBubbleWrapper::CreateCallHeader Line2() = %d",
	    callHeaderParam->Line2() );

    // Set line2 indicator
    iBubbleManager->SetCallFlag(
        bubble,
        CBubbleManager::ELine2,
        callHeaderParam->Line2() );

    // Set number type indicator
    SetNumberTypeIndicator( bubble, static_cast<TPEPhoneNumberIdType>(
        callHeaderParam->NumberType() ) );

    if ( !iCallImageTextSupported )
		{
        if( callHeaderParam->Thumbnail() )
            {
            // Thumbnail
            iBubbleManager->SetThumbnail( bubble, callHeaderParam->Thumbnail(), NULL );
            }
		}
    else  // call object display
        {
        if ( callHeaderParam->CallerText().Length() )
            {
            __PHONELOG(
                EBasic,
                EPhoneUIView,
                "CPhoneBubbleWrapper::CreateCallHeaderL()-> Set 1.Call text");
            // 1. Call text
            iBubbleManager->SetCallObjectText( bubble, callHeaderParam->CallerText() );
            }
        else if( callHeaderParam->Picture().Length() )
            {
            __PHONELOG(
                EBasic,
                EPhoneUIView,
                "CPhoneBubbleWrapper::CreateCallHeaderL()-> Set 2. Call image");
            // 2. Call image
            iBubbleManager->SetCallObjectImage( bubble, callHeaderParam->Picture() );
            }
        else if( callHeaderParam->HasThumbnail() )
            {
            __PHONELOG(
                EBasic,
                EPhoneUIView,
                "CPhoneBubbleWrapper::CreateCallHeaderL()-> Set 3. Thumbnail");
            // 3. Thumbnail
            if ( callHeaderParam->Thumbnail() )
                {
                iBubbleManager->SetCallObjectImage( bubble, callHeaderParam->Thumbnail(), NULL );
                }
            }
        else if ( iCallTheme->CallImageThemeSettings() == 2 )
            {
            __PHONELOG(
                EBasic,
                EPhoneUIView,
                "CPhoneBubbleWrapper::CreateCallHeaderL()-> Set 4. Theme image");
            // 4. Theme image
            iBubbleManager->SetCallObjectFromTheme( bubble );
            }
        else if ( iCallTheme->CallImageThemeSettings() == 1 &&
                  BaflUtils::FileExists( CCoeEnv::Static()->FsSession(),
                                         iCallTheme->CallImageThemeFilePath() ) )
            {
            __PHONELOG(
                EBasic,
                EPhoneUIView,
                "CPhoneBubbleWrapper::CreateCallHeaderL()-> Set 5. User defined theme image");
            // 5. User defined theme image
            iBubbleManager->SetCallObjectImage(
                bubble,
                iCallTheme->CallImageThemeFilePath() );
            }
        else
            {
            __PHONELOG(
                EBasic,
                EPhoneUIView,
                "CPhoneBubbleWrapper::CreateCallHeaderL()-> Do nothing ");
            // do nothing
            }
        }

    iBubbleExtensionManager->StartCallL( bubble, callHeaderParam );
    iBubbleManager->EndChanges();

    return bubble;
    }

// -----------------------------------------------------------------------------
// CPhoneBubbleWrapper.::StartMOVideoCallAnimationHeaderL
// -----------------------------------------------------------------------------
//
void CPhoneBubbleWrapper::StartMOVideoCallAnimationHeaderL(
	CBubbleManager::TBubbleId aBubbleId,
	const TDesC& aStateLabelText,
    const TDesC& aStateShortLabelText)
	{
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneBubbleWrapper::StartMOVideoCallAnimationHeaderL");

	// We have MO video call - start animating
	iVideoCallAnimationHeader = CPhoneCallHeaderTextAnimation::InstanceL();
	iVideoCallAnimationHeader->StartAnimatingVideoCallHeaderL(
		aBubbleId,
		aStateLabelText,
		aStateShortLabelText,
		*iBubbleManager );

	}

// -----------------------------------------------------------------------------
// CPhoneBubbleWrapper.::CreateEmergencyCallHeaderL
// -----------------------------------------------------------------------------
//
CBubbleManager::TBubbleId CPhoneBubbleWrapper::CreateEmergencyCallHeaderL(
    TInt aCallId,
    TPhoneCommandParam *aCommandParam )
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneBubbleWrapper::CreateEmergencyCallHeader");
    TPhoneCmdParamEmergencyCallHeaderData* emergencyHeaderParam =
        static_cast<TPhoneCmdParamEmergencyCallHeaderData*>(
        aCommandParam );

    iBubbleManager->StartChanges();

    // Create the bubble if necessary
    CBubbleManager::TBubbleId bubble;
    if ( !iMapping->FindBubbleByCall( aCallId, bubble ) )
        {
        bubble = iBubbleManager->CreateCallHeader();
        iMapping->AddToMappingL( bubble, aCallId, CBubbleManager::EActive, NULL );
        }

    iBubbleManager->SetState( bubble, CBubbleManager::EActive );

    // Set the call header title
    iBubbleManager->SetCLI(
        bubble,
        emergencyHeaderParam->HeaderText(),
        CBubbleManager::ERight );

    // Set ciphering indicator
    iBubbleManager->SetCallFlag(
        bubble,
        CBubbleManager::ENoCiphering,
        iCipheringIndicatorAllowed &&
        !emergencyHeaderParam->Ciphering() );

    // Set the call label as the CNAP text
    SetCNAP( bubble, 
            emergencyHeaderParam->LabelText(), 
            CBubbleManager::ELeft );

    // Set own phone number as the label
    iBubbleManager->SetLabel(
        bubble,
        emergencyHeaderParam->OwnPhoneNumberText(),
        CBubbleManager::ELeft );
    iBubbleManager->EndChanges();

    return bubble;
    }

// -----------------------------------------------------------------------------
// CPhoneBubbleWrapper.::UpdateCallHeaderDisplay
// -----------------------------------------------------------------------------
//
void CPhoneBubbleWrapper::UpdateCallHeaderDisplay(
    TInt aCallId,
    TPhoneCommandParam* aCommandParam )
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneBubbleWrapper::UpdateCallHeaderDisplay");
    CBubbleManager::TBubbleId bubble;

    if ( iMapping->FindBubbleByCall( aCallId, bubble ) )
        {
        TPhoneCmdParamCallHeaderData* callHeaderParam =
            static_cast<TPhoneCmdParamCallHeaderData*>( aCommandParam );

        iBubbleManager->StartChanges();

        SetCallHeaderParameters( bubble, callHeaderParam );

		if( iVideoCallAnimationHeader )
			{
			if( callHeaderParam->CallState() != EPEStateDialing )
				{
				iVideoCallAnimationHeader->RemoveAnimatingVideoCallHeader();
				delete iVideoCallAnimationHeader;
    		    iVideoCallAnimationHeader = NULL;
				}
			}
        // End changes
        iBubbleManager->EndChanges();
        }
    }

// -----------------------------------------------------------------------------
// CPhoneBubbleWrapper.::UpdateCallHeaderAndLabel
// -----------------------------------------------------------------------------
//
void CPhoneBubbleWrapper::UpdateCallHeaderAndLabel(
    TInt aCallId,
    TPhoneCommandParam* aCommandParam )
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneBubbleWrapper::UpdateCallHeaderAndLabel");
        CBubbleManager::TBubbleId bubble;

    if ( iMapping->FindBubbleByCall( aCallId, bubble ) )
        {
        TPhoneCmdParamCallHeaderData* callHeaderParam =
            static_cast<TPhoneCmdParamCallHeaderData*>( aCommandParam );

        iBubbleManager->StartChanges();

        SetCallHeaderParameters( bubble, callHeaderParam );

        // Set the call label associated with the state
        iBubbleManager->SetLabel(
            bubble,
            callHeaderParam->LabelText(),
            CBubbleManager::ERight );

        if( iVideoCallAnimationHeader )
            {
            if( callHeaderParam->CallState() != EPEStateDialing )
                {
                iVideoCallAnimationHeader->RemoveAnimatingVideoCallHeader();
                delete iVideoCallAnimationHeader;
                iVideoCallAnimationHeader = NULL;
                }
            }

        // End changes
        iBubbleManager->EndChanges();
        }
    }

// -----------------------------------------------------------------------------
// CPhoneBubbleWrapper.::SetCallHeaderParameters
// -----------------------------------------------------------------------------
//
void CPhoneBubbleWrapper::SetCallHeaderParameters(
        TInt aBubble,
        TPhoneCmdParamCallHeaderData* aCallHeaderParam )
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneBubbleWrapper::SetCallHeaderParameters");
    __PHONELOG1(
        EBasic,
        EPhoneUIView,
        "CPhoneBubbleWrapper::SetCallHeaderParameters callHeaderParam->CLIText = %S",
        &aCallHeaderParam->CLIText() );

    // Set CLI text
    iBubbleManager->SetCLI(
        aBubble,
        aCallHeaderParam->CLIText(),
        aCallHeaderParam->CLITextClippingDirection() );

    // Set CNAP text if applicable
    iBubbleManager->SetCNAP(
            aBubble,
            aCallHeaderParam->CNAPText(),
            aCallHeaderParam->CNAPTextClippingDirection() );

    iBubbleManager->SetCallFlags( aBubble, aCallHeaderParam->CallFlag() );

    // Set ciphering indicator
    iBubbleManager->SetCallFlag(
        aBubble,
        CBubbleManager::ENoCiphering, 
        aCallHeaderParam->CipheringIndicatorAllowed() && 
        !aCallHeaderParam->Ciphering() );

    // Set line2 indicator
    iBubbleManager->SetCallFlag(
        aBubble,
        CBubbleManager::ELine2,
        aCallHeaderParam->Line2() );

    if ( !iCallImageTextSupported )
        {
        if( aCallHeaderParam->Thumbnail() )
            {
            __PHONELOG(
                EBasic,
                EPhoneUIView,
                "CPhoneBubbleWrapper::SetCallHeaderParameters()-> Set Thumbnail" );
            // Thumbnail
            iBubbleManager->SetThumbnail( aBubble, aCallHeaderParam->Thumbnail(), NULL );
            }
        }
    else  // call object display
        {
        if ( aCallHeaderParam->CallerText().Length() )
            {
            __PHONELOG(
                EBasic,
                EPhoneUIView,
                "CPhoneBubbleWrapper::SetCallHeaderParameters()-> Set 1.Call text");
            // 1. Call text
            iBubbleManager->SetCallObjectText( aBubble, aCallHeaderParam->CallerText() );
            }
        else if( aCallHeaderParam->Picture().Length() )
            {
            __PHONELOG(
                EBasic,
                EPhoneUIView,
                "CPhoneBubbleWrapper::SetCallHeaderParameters()-> Set 2.Call image");
            // 2. Call image
            iBubbleManager->SetCallObjectImage( aBubble, aCallHeaderParam->Picture() );
            }
        else if( aCallHeaderParam->HasThumbnail() )
            {
            // 3. Thumbnail
            __PHONELOG(
                EBasic,
                EPhoneUIView,
                "CPhoneBubbleWrapper::SetCallHeaderParameters()-> Set 3.Thumbnail");
            if ( aCallHeaderParam->Thumbnail() )
                {
                iBubbleManager->SetCallObjectImage( aBubble, aCallHeaderParam->Thumbnail(), NULL );
                }
            }
        else
            {
            // do nothing
            __PHONELOG(
                EBasic,
                EPhoneUIView,
                "CPhoneBubbleWrapper::SetCallHeaderParameters()-> Do nothing");
            }
        }
    }

// -----------------------------------------------------------------------------
// CPhoneBubbleWrapper.::SetNumberEntryContent
// -----------------------------------------------------------------------------
//
void CPhoneBubbleWrapper::SetNumberEntryContent( const TDesC& aContent )
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneBubbleWrapper::SetNumberEntryContent");
    iPhoneNumberEntry->SetNumberEntryContent( aContent );
    }

// -----------------------------------------------------------------------------
// CPhoneBubbleWrapper.::GetNumberEntryContent
// -----------------------------------------------------------------------------
//
void CPhoneBubbleWrapper::GetNumberEntryContent(
    TPhoneCommandParam* aCommandParam )
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneBubbleWrapper::GetNumberEntryContent");

    GetLocalizedNumberEntryContent( aCommandParam );

    TPhoneCmdParamString* entryContent =
        static_cast<TPhoneCmdParamString*>( aCommandParam );

    __PHONELOG1( EBasic, EPhoneUIView,
        "CPhoneBubbleWrapper::GetNumberEntryContent(%S)",
        entryContent->String() );

    AknTextUtils::ConvertDigitsTo( *entryContent->String(), EDigitTypeWestern );

    __PHONELOG1( EBasic, EPhoneUIView,
        "CPhoneBubbleWrapper::GetNumberEntryContent(%S)",
        entryContent->String() );
    }

// -----------------------------------------------------------------------------
// CPhoneBubbleWrapper.::GetNumberEntryContent
// -----------------------------------------------------------------------------
//
void CPhoneBubbleWrapper::GetLocalizedNumberEntryContent(
    TPhoneCommandParam* aCommandParam )
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneBubbleWrapper::GetLocalizedNumberEntryContent");
    TPhoneCmdParamString* entryContent =
        static_cast<TPhoneCmdParamString*>( aCommandParam );

    // Get the initial number from number entry window
    if ( iUseDialer )
        {
        iDialerNumberEntry->GetTextFromNumberEntry( *entryContent->String() );
        }
    else
        {
        iBubbleManager->GetTextFromNumberEntry( *entryContent->String() );
        }
    }

// -----------------------------------------------------------------------------
// CPhoneBubbleWrapper.::CreateNumberEntry
// -----------------------------------------------------------------------------
//
void CPhoneBubbleWrapper::CreateNumberEntry()
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneBubbleWrapper::CreateNumberEntry");

    iIsNumberEntryModeNumeric = ETrue;
    if ( iUseDialer )
        {
        iDialerNumberEntry->CreateNumberEntry();
        }
    else
        {
        iBubbleManager->ResetEditorToDefaultValues();
        if( iPhoneCustomization && 
            iPhoneCustomization->AllowAlphaNumericMode() )
            {    
            iBubbleManager->ChangeEditorMode( ETrue );  
            }
        TInt flags = iFlags;
        flags &= ~EFlagNumberEntryExists;

        flags |= EFlagNumberEntryExists;
        flags |= EFlagNumberEntryVisible;

        HandleFlagsUpdated( flags );
        }
    }
// -----------------------------------------------------------------------------
// CPhoneBubbleWrapper.::SetNumberEntryObserver
// -----------------------------------------------------------------------------
//
void CPhoneBubbleWrapper::SetNumberEntryObserver( MNumberEntryObserver& aObserver )
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneBubbleWrapper::SetNumberEntryObserver");

    if ( iUseDialer )
        {
        iDialerNumberEntry->SetNumberEntryObserver( aObserver );
        }

    }
// -----------------------------------------------------------------------------
// CPhoneBubbleWrapper.::SetNumberEntryPromptText
// -----------------------------------------------------------------------------
//
void CPhoneBubbleWrapper::SetNumberEntryPromptText( const TDesC& aPromptText )
    {
    iPhoneNumberEntry->SetNumberEntryPromptText( aPromptText );
    }

// -----------------------------------------------------------------------------
// CPhoneBubbleWrapper.::SetNumberEntryVisible
// -----------------------------------------------------------------------------
//
void CPhoneBubbleWrapper::SetNumberEntryVisible(
    TPhoneCommandParam* aCommandParam )
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneBubbleWrapper::SetNumberEntryVisible");
    // Check is the given parameter valid
    if ( aCommandParam->ParamId() == TPhoneCommandParam::EPhoneParamIdBoolean )
        {
        TPhoneCmdParamBoolean* visible =
            static_cast<TPhoneCmdParamBoolean*>( aCommandParam );

        if ( iUseDialer )
            {
            iDialerNumberEntry->SetNumberEntryVisible( visible->Boolean() );
            }
        else
            {
            TInt flags = iFlags;
            flags &= ~EFlagNumberEntryVisible;
            if ( visible->Boolean() )
                {
                flags |= EFlagNumberEntryVisible;
                }
            HandleFlagsUpdated( flags );
            }
        }
    }

// -----------------------------------------------------------------------------
// CPhoneBubbleWrapper.::RemoveNumberEntry
// -----------------------------------------------------------------------------
//
void CPhoneBubbleWrapper::RemoveNumberEntry()
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneBubbleWrapper::RemoveNumberEntry");

    if ( iUseDialer )
        {
        iDialerNumberEntry->RemoveNumberEntry( );
        }
    else
        {
        TInt flags = iFlags;
        flags &= ~EFlagNumberEntryExists;

        HandleFlagsUpdated( flags );
        }
    }

// -----------------------------------------------------------------------------
// CPhoneBubbleWrapper::GetNumberEntryCount
// -----------------------------------------------------------------------------
//
void CPhoneBubbleWrapper::GetNumberEntryCount(
    TPhoneCommandParam* aCommandParam )
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneBubbleWrapper::CountNumberEntryCharacters");
    TPhoneCmdParamInteger* integerParam = static_cast<TPhoneCmdParamInteger*>(
        aCommandParam );

    // Return the number of characters in the number entry
    integerParam->SetInteger( CountNumberEntryCharacters() );
    }

// ---------------------------------------------------------------------------
// CPhoneBubbleWrapper::CountNumberEntryCharacters
// ---------------------------------------------------------------------------
//
TInt CPhoneBubbleWrapper::CountNumberEntryCharacters()
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneBubbleWrapper::CountNumberEntryCharacters");

    return iPhoneNumberEntry->CountNumberEntryCharacters();
    }

// ---------------------------------------------------------------------------
// CPhoneBubbleWrapper::HandleNumberEntryChanged
// ---------------------------------------------------------------------------
//
void CPhoneBubbleWrapper::HandleNumberEntryChanged()
    {
    iPhoneNumberEntry->HandleNumberEntryChanged();

    }

// ---------------------------------------------------------------------------
// CPhoneBubbleWrapper::HandleNumberEntryCommand
// ---------------------------------------------------------------------------
//
void CPhoneBubbleWrapper::SetNumberEntryChangedCallBack( 
        TPhoneCommandParam* aCommandParam )
    {
    iPhoneNumberEntry->SetNumberEntryChangedCallBack( aCommandParam );

    }


// ---------------------------------------------------------------------------
// CPhoneBubbleWrapper::HandleControlEventL
//  
//  This must be non leaving function, because others use it too.
// ---------------------------------------------------------------------------
//
void CPhoneBubbleWrapper::HandleControlEventL( CCoeControl* aControl, 
                                                     TCoeEvent aEventType)
    {
    if ( aControl == iBubbleManager->GetNumberEntry()->Parent() 
         && aEventType == EEventStateChanged )
        {
        HandleNumberEntryChanged();
                
        }
    }

// ---------------------------------------------------------------------------
// CPhoneBubbleWrapper::IsNumberEntryUsed
// ---------------------------------------------------------------------------
//
TBool CPhoneBubbleWrapper::IsNumberEntryUsed()
    {
    return iPhoneNumberEntry->IsNumberEntryUsed();
    }

// ---------------------------------------------------------------------------
// CPhoneBubbleWrapper::IsNumberEntryVisible
// ---------------------------------------------------------------------------
//
TBool CPhoneBubbleWrapper::IsNumberEntryVisible()
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneBubbleWrapper::IsNumberEntryVisible");
    __PHONELOG1(
            EBasic,
            EPhoneUIView,
            "CPhoneBubbleWrapper::IsNumberEntryVisible() Visible = %d",
            iNumberEntryVisible );

    if ( iUseDialer )
        {
        return iDialerNumberEntry->IsNumberEntryUsed();
        }
    else
        {
        return iBubbleManager->IsNumberEntryUsed() && iNumberEntryVisible;
        }
    }

// ---------------------------------------------------------------------------
// CPhoneBubbleWrapper::HandleKeyEventL
// ---------------------------------------------------------------------------
//
TKeyResponse CPhoneBubbleWrapper::HandleKeyEventL(
    TPhoneCommandParam* aCommandParam )
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneBubbleWrapper::HandleKeyEventL");
    TKeyResponse response = EKeyWasNotConsumed;

    CCoeControl* control(NULL);
    if ( iUseDialer )
        {
        // TODO: We should try to clarify the dialer control hierarchy.
        // Currently, we have three controls, each of which are somewhere
        // referred as "number entry": CDialer, CDialerNumberEntry, and CAknPhoneNumberEditor.
        // Each of these are present on the code line below.
        control = iDialerNumberEntry->GetNumberEntry()->Parent();
        }
    else
        {
        control = iBubbleManager->GetNumberEntry();
        if ( iPhoneCustomization && control )
            {
            iBubbleManager->GetEditorMode();
            }
        }

    if ( control )
        {
        // Check if the given parameter is valid
        if ( aCommandParam->ParamId() ==
            TPhoneCommandParam::EPhoneParamIdKeyEvent )
            {
            TPhoneCmdParamKeyEvent* keyEventParam =
                static_cast<TPhoneCmdParamKeyEvent*>( aCommandParam );

            response = control->OfferKeyEventL(
                keyEventParam->KeyEvent(), keyEventParam->EventCode() );
            }
        }

    return response;
    }

// ---------------------------------------------------------------------------
// CPhoneBubbleWrapper::SetPhoneMuted
// Show or clear the "Mute" Icon.
// ---------------------------------------------------------------------------
//
void CPhoneBubbleWrapper::SetPhoneMuted( TPhoneCommandParam* aCommandParam )
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneBubbleWrapper::SetPhoneMuted");
    // Check if the given parameter is valid
    if ( aCommandParam->ParamId() == TPhoneCommandParam::EPhoneParamIdBoolean )
        {
        TPhoneCmdParamBoolean* booleanParam =
            static_cast<TPhoneCmdParamBoolean*>( aCommandParam );

        iBubbleManager->StartChanges();
        iBubbleManager->SetPhoneMuted( booleanParam->Boolean() );
        iBubbleManager->EndChanges();
        }
    }

// ---------------------------------------------------------------------------
// CPhoneBubbleWrapper::PrepareCallStateL
// Prepare call bubble state.
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CPhoneBubbleWrapper::PrepareCallStateL(
    TInt aCallId,
    TPEState aState,
    const TDesC& aStateLabelText,
    const TDesC& aStateShortLabelText )
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneBubbleWrapper::PrepareCallState");

    if ( aCallId == KConferenceCallId && !iConference )
        {
        return;
        }

    // JMK: moved these two lines temporarily here
    iBubbleManager->StartChanges();
    CBubbleManager::TBubbleId bubble;

    if ( !iMapping->FindBubbleByCall( aCallId, bubble ) )
        {
        bubble = iBubbleManager->CreateCallHeader();
        iMapping->AddToMappingL( bubble, aCallId, CBubbleManager::ENone, NULL );
        }

    // Set the call label associated with the state
    iBubbleManager->SetLabel(
        bubble,
        aStateLabelText,
        CBubbleManager::ERight );


    if( aState == EPEStateDialing &&
        ( aCallId == KVideoTelephonyCallId1 ||
	    aCallId == KVideoTelephonyCallId2 ) )
		{
        StartMOVideoCallAnimationHeaderL( bubble,
                                          aStateLabelText,
                                          aStateShortLabelText );
		}

    // Stop video call header animation if no EPEStateDialing state
	if( iVideoCallAnimationHeader && ( aState != EPEStateDialing ))
    	{
    	iVideoCallAnimationHeader->RemoveAnimatingVideoCallHeader();
    	}

    CBubbleManager::TPhoneCallState callState = GetBubbleState( aState );

    // Set state
    iBubbleManager->SetState( bubble, callState );
    iMapping->SetCallStateByCall( aCallId, callState );

    // inform extension plugins
    iBubbleExtensionManager->UpdateCallState( bubble, aState );

    iBubbleManager->EndChanges();
    }

// ---------------------------------------------------------------------------
// CPhoneBubbleWrapper::UpdateCallDuration
// ---------------------------------------------------------------------------
//
void CPhoneBubbleWrapper::UpdateCallDuration(
    TInt aCallId,
    const TDesC& aDurationText )
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneBubbleWrapper::UpdateCallDuration");
    CBubbleManager::TBubbleId bubble;

    if ( iMapping->FindBubbleByCall( aCallId, bubble ) )
        {
        iBubbleManager->UpdateCallTime( bubble, aDurationText );
        }
    }

// ---------------------------------------------------------------------------
// CPhoneBubbleWrapper::CreateBubbleL
// ---------------------------------------------------------------------------
//
CBubbleManager::TBubbleId CPhoneBubbleWrapper::CreateBubbleL( TInt aCallId )
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneBubbleWrapper::CreateBubble");
    CBubbleManager::TBubbleId bubble;

    if ( !iMapping->FindBubbleByCall( aCallId, bubble ) )
        {
        bubble = iBubbleManager->CreateCallHeader();
        iMapping->AddToMappingL( bubble, aCallId, CBubbleManager::ENone, NULL );
        }

    return bubble;
    }

// ---------------------------------------------------------------------------
// CPhoneBubbleWrapper::SetCLI
// ---------------------------------------------------------------------------
//
void CPhoneBubbleWrapper::SetCLI(
    CBubbleManager::TBubbleId aId,
    const TDesC& aText,
    CBubbleManager::TPhoneClippingDirection aDirection )
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneBubbleWrapper::SetCLI");

    // Start bubble manager changes
    iBubbleManager->StartChanges();

    iBubbleManager->SetCLI(
        aId,
        aText,
        aDirection );

    // End bubble manager changes
    iBubbleManager->EndChanges();
    }

// ---------------------------------------------------------------------------
// CPhoneBubbleWrapper::SetCnap
// needed because of __SERIES60_PHONE_CNAP
// ---------------------------------------------------------------------------
//
void CPhoneBubbleWrapper::SetCNAP(
    CBubbleManager::TBubbleId aId,
    const TDesC& aCnapText, 
    CBubbleManager::TPhoneClippingDirection aDirection )
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneBubbleWrapper::SetCNAP");

    iBubbleManager->SetCNAP(
        aId, 
        aCnapText, 
        aDirection );
    }

// ---------------------------------------------------------------------------
// CPhoneBubbleWrapper::SetNumberTypeIndicator
// ---------------------------------------------------------------------------
//
void CPhoneBubbleWrapper::SetNumberTypeIndicator(
    CBubbleManager::TBubbleId aBubbleId,
    TPEPhoneNumberIdType aNumberType )
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneBubbleWrapper::SetNumberTypeIndicator");
    CBubbleManager::TPhoneNumberType phoneNumberType =
        CBubbleManager::ENotSet;

    // map the number type to the bubble's number types
    switch( aNumberType )
        {
        case EPEFaxNumber:
            phoneNumberType = CBubbleManager::EFaxNumber;
            break;

        case EPEMobileNumber:
            phoneNumberType = CBubbleManager::EMobile;
            break;

        case EPEPager:
            phoneNumberType = CBubbleManager::EPager;
            break;

        case EPETelephoneNumber:
            phoneNumberType = CBubbleManager::EPhone;
            break;

        case EPEAssistantNumber:
            phoneNumberType = CBubbleManager::EAssistant;
            break;

        case EPECarNumber:
            phoneNumberType = CBubbleManager::ECar;
            break;

        default:
            break;
        }

    // signal the bubble manager to update the call type icon
    iBubbleManager->SetNumberType( aBubbleId, phoneNumberType );
    }

// ---------------------------------------------------------------------------
// CPhoneBubbleWrapper::CreateConferenceL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CPhoneBubbleWrapper::CreateConferenceL(
    TInt aCallId,
    TPhoneCommandParam *aCommandParam )
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneBubbleWrapper::CreateConference");

    TPhoneCmdParamCallHeaderData* callHeaderParam =
            static_cast<TPhoneCmdParamCallHeaderData*>( aCommandParam );

	if( !iParticipants )
		{
		// Create participants array if doesn't exist
	    iParticipants = new ( ELeave ) CParticipantArray(
	        KConferenceMaxParticipants );
	    iParticipants->SetReserveL( KConferenceMaxParticipants );
		}

    // find out call ids for initial conference members
    TInt firstCallId( KErrNotFound );
    TInt secondCallId( KErrNotFound );
    TInt callCount = iMapping->ItemCount();

    // Find bubbles to conference
    for ( TInt index = 0; index < callCount; index++ )
        {
        TInt callId = iMapping->CallIdAt( index );
        CBubbleManager::TPhoneCallState bubbleState;
        bubbleState = iMapping->FindCallStateByCall( callId );


        if ( firstCallId == KErrNotFound )
            {
            TInt callId = iMapping->CallIdAt( index );
            if( bubbleState == CBubbleManager::EActive ||
                bubbleState == CBubbleManager::EOnHold )
                {
                firstCallId = iMapping->CallIdAt( index );
                }
            }
        else if ( secondCallId == KErrNotFound )
            {
            if( bubbleState == CBubbleManager::EActive ||
                bubbleState == CBubbleManager::EOnHold )

                {
                secondCallId = iMapping->CallIdAt( index );
                }
            }
        }

    if ( firstCallId != KErrNotFound && secondCallId != KErrNotFound )
        {
        // get bubbles which have to connect to conference
        CBubbleManager::TBubbleId first = CreateBubbleL( firstCallId );
        CBubbleManager::TBubbleId second = CreateBubbleL( secondCallId );

        iBubbleManager->StartChanges();
        CBubbleManager::TBubbleId conference =
            iBubbleManager->CreateConference( first, second );
        iBubbleManager->SetState( conference, CBubbleManager::EActive );
        iBubbleManager->SetLabel( conference, KNullDesC );
        iBubbleManager->SetCallTime( conference, KNullDesC );
        // Set CLI text
        iBubbleManager->SetCLI(
                conference, callHeaderParam->CLIText(), CBubbleManager::ERight );

        // Set ciphering indicator
        iBubbleManager->SetCallFlag(
            conference,
            CBubbleManager::ENoCiphering,
            callHeaderParam->CipheringIndicatorAllowed() && 
            !callHeaderParam->Ciphering() );

        // add calls to participant list - conference call itself is not a participant
        iParticipants->AppendL(firstCallId);
        iParticipants->AppendL(secondCallId);

        if ( FeatureManager::FeatureSupported( KFeatureIdTouchCallHandling ) ||
             callCount > KNumberOfActiveCalls )
            {
            SetConferenceExpand( EFalse );
            }
        else
            {
            SetConferenceExpand( ETrue );
            }

        // add conference to mapping
        iMapping->AddToMappingL(conference, aCallId, CBubbleManager::EActive, NULL);

        TInt count = iParticipants->Count();

        for ( TInt index = 0; index < count; index++ )
            {
            CBubbleManager::TBubbleId bubble =
                CreateBubbleL( iParticipants->At( index ) );
            iBubbleManager->SetState( bubble, CBubbleManager::EActive );
            iBubbleManager->SetLabel( bubble, KNullDesC );
            iBubbleManager->SetCallTime( bubble, KNullDesC );
            }

    	// If image/text is enabled and theme image is set, use it
      	if( iCallImageTextSupported )
    	  	{
            if ( iCallTheme->CallImageThemeSettings() == 2 )
                {
                // 4. Theme image
                iBubbleManager->SetCallObjectFromTheme( conference );
                }
            else if ( iCallTheme->CallImageThemeSettings() == 1 )
                {
                // 5. User defined theme image
                iBubbleManager->SetCallObjectImage(
                    conference,
                    iCallTheme->CallImageThemeFilePath() );
                }
    	  	}
      	
      	// Read and save internally the simplified conference call header 
      	// setting for the service where conference is to be created.
      	SetSimplifiedConferenceCallHeaderStatus( 
      	    IsSimplifiedConferenceCallHeaderEnabledL( 
      	        callHeaderParam->ServiceId() ) );
      	
        iBubbleManager->EndChanges();
        iConference = ETrue;
        }
    else
        {
        __PHONELOG(
            EBasic,
            EPhoneUIView,
            "CPhoneBubbleWrapper::CreateConference -> No valid call bubbles -> Can't Create conference " );
        }
    }

// ---------------------------------------------------------
// CPhoneBubbleWrapper::RemoveConferenceBubbleL
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPhoneBubbleWrapper::RemoveConferenceBubbleL()
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneBubbleWrapper::RemoveConferenceBubbleL");
    iBubbleManager->SetState( CreateBubbleL( KConferenceCallId ), CBubbleManager::EDisconnected );
    
    SetConferenceExpand( EFalse );
    SetConferenceHighlight( EFalse );
    SetSimplifiedConferenceCallHeaderStatus( EFalse );
    iBubbleManager->RemoveConference();
    
    // remove conference from mapping
    iMapping->RemoveFromMapping( KConferenceCallId );
    }

// ---------------------------------------------------------
// CPhoneBubbleWrapper::RemoveConferenceL
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPhoneBubbleWrapper::RemoveConferenceL()
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneBubbleWrapper::RemoveConference");

	if ( iConference )
		{
		iBubbleManager->StartChanges();

	    // Set conference bubble state and remove bubble
	    RemoveConferenceBubbleL();

	    TInt count = iParticipants->Count();

	    // empty participants list
	    iParticipants->Delete(0, count);

	    iBubbleManager->EndChanges();

	    iConference = EFalse;
		}
    }
// ---------------------------------------------------------
// CPhoneBubbleWrapper::AddToConferenceL
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPhoneBubbleWrapper::AddToConferenceL( TInt aCallId )
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneBubbleWrapper::AddToConference");

    iBubbleManager->StartChanges();

    CBubbleManager::TBubbleId bubble = CreateBubbleL( aCallId );

    iBubbleManager->AddRowToConference( bubble );

    iBubbleManager->SetState( bubble, CBubbleManager::EActive );
    iBubbleManager->SetLabel( bubble, KNullDesC );
    iBubbleManager->SetCallTime( bubble, KNullDesC );

    if ( !FeatureManager::FeatureSupported( KFeatureIdTouchCallHandling ) &&
         iMapping->FindCallIdByCallState( CBubbleManager::EWaiting ) < 0 &&
         iMapping->FindCallIdByCallState( CBubbleManager::EOutgoing ) < 0 &&
         iMapping->FindCallIdByCallState( CBubbleManager::EDisconnected ) < 0 )
        {
        SetConferenceExpand( ETrue );
        }

    iParticipants->AppendL( aCallId );

    iBubbleManager->EndChanges();
    }

// ---------------------------------------------------------
// CPhoneBubbleWrapper::RemoveFromConferenceL
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPhoneBubbleWrapper::RemoveFromConferenceL( TInt aCallId )
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneBubbleWrapper::RemoveFromConference");
    TInt pos = FindParticipantPosition( aCallId );

    if ( pos != KErrNotFound )
        {
        iBubbleManager->StartChanges();
        TInt count = iParticipants->Count();
        CBubbleManager::TBubbleId bubble = CreateBubbleL( aCallId );
	    iBubbleManager->RemoveRowFromConference( bubble );

        // If there are enough participants for conference, then
        // simply remove it from conference.
        if ( count > KPhoneConferenceInitialSize )
            {
            iParticipants->Delete( pos );
            }
        else
        // Otherwise we also have to clear the conference.
            {
            RemoveConferenceL();
            }

	    iBubbleManager->RemoveCallHeader( bubble );
	    iMapping->RemoveFromMapping( aCallId );
	    iBubbleManager->EndChanges();
        }
    }

// ---------------------------------------------------------
// CPhoneBubbleWrapper::FindParticipantPosition
// ---------------------------------------------------------
//
TInt CPhoneBubbleWrapper::FindParticipantPosition(
    const TInt aCallId )
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneBubbleWrapper::FindParticipantPosition");
    TInt result = KErrNotFound;

    TInt index;
    TInt count = iParticipants->Count();

    for ( index = 0; index < count; index++ )
        {
        if ( iParticipants->At( index ) == aCallId )
            {
            result = index;
            break;
            }
        }

    return result;
    }

// -----------------------------------------------------------------------------
// CPhoneBubbleWrapper::SetConferenceExpand
// -----------------------------------------------------------------------------
//
void CPhoneBubbleWrapper::SetConferenceExpand( TBool aStatus )
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneBubbleWrapper::SetConferenceExpand");
    TInt flags = iFlags;

    flags &= ~EFlagConferenceExpanded;

    if ( aStatus )
        {
        flags |= EFlagConferenceExpanded;
        }

    HandleFlagsUpdated( flags );
    }


// -----------------------------------------------------------------------------
// CPhoneBubbleWrapper::SetConferenceHighlight
// -----------------------------------------------------------------------------
//
void CPhoneBubbleWrapper::SetConferenceHighlight(TBool aStatus)
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneBubbleWrapper::SetConferenceHighlight");
    TInt flags = iFlags;
    flags &= ~EFlagConferenceReqExpanded;

    if ( aStatus )
        {
        flags |= EFlagConferenceReqExpanded;
        }

    HandleFlagsUpdated( flags );
    }

// ---------------------------------------------------------
// CPhoneBubbleWrapper::HandleFlagsUpdated
// ---------------------------------------------------------
//
void CPhoneBubbleWrapper::HandleFlagsUpdated(
    TInt aNewFlags )
    {
    // If no changes, then just return.
    if ( iFlags == aNewFlags )
        {
        return;
        }

    TInt old = iFlags;
    iFlags = aNewFlags;

    if ( !( iFlags & EFlagNumberEntryExists ) )
        {
        // Number entry can't be visible if it doesn't exist.
        iFlags &= ~EFlagNumberEntryVisible;
        iNumberEntryVisible = EFalse;
        }

    iBubbleManager->StartChanges();

    // Check if number entry has been removed.
    if ( old & ~iFlags & EFlagNumberEntryExists )
        {
        iBubbleManager->RemoveNumberEntry();
        }

    // If conference is required to expanded, then it must be
    // kept so.
    if ( iFlags & EFlagConferenceReqExpanded )
        {
        // If there has been change, then enable it.
        if ( !( old & EFlagConferenceReqExpanded) )
            {
            iBubbleManager->SetExpandedConferenceCallHeader( ETrue );
            iBubbleManager->SetSelectionInConference( 1 );
            iBubbleManager->SetNumberEntryVisible( EFalse );
            iNumberEntryVisible = EFalse;
            }

        // We do not have to anything else.
        iBubbleManager->EndChanges();
        return;
        }

    // Now we can consider that there is no highlight.
    iBubbleManager->SetSelectionInConference( 0 );

    // Number entry exists now.
    if ( iFlags & EFlagNumberEntryExists )
        {
        // If there has been change, create number entry.
        if ( !( old & EFlagNumberEntryExists ) )
            {
            iBubbleManager->CreateNumberEntry();
            iNumberEntryVisible = ETrue;
            }

        // If number entry exists, then make it visible.
        iBubbleManager->SetNumberEntryVisible( iFlags & EFlagNumberEntryVisible );
        iNumberEntryVisible = ( iFlags & EFlagNumberEntryVisible );

        // Shrink conference
        iBubbleManager->SetExpandedConferenceCallHeader( EFalse );
        }

    // If conference expanded flag, then expand/shrink conference.
    if ( iFlags == EFlagConferenceExpanded )
        {
        iBubbleManager->SetExpandedConferenceCallHeader( ETrue );
        }
    else if ( !( iFlags & EFlagConferenceExpanded ) )
        {
        iBubbleManager->SetExpandedConferenceCallHeader( EFalse );
        }
    
    // Simplified conference call header setting overrides 
    // EFlagConferenceExpanded setting.
    if ( iFlags & EFlagSimplifiedConferenceCallHeader )
        {
        iBubbleManager->SetExpandedConferenceCallHeader( EFalse );
        }
    
    iBubbleManager->EndChanges();
    }

// ---------------------------------------------------------
// CPhoneBubbleWrapper::SelectedConfMemberCallIdL
// ---------------------------------------------------------
//
TBool CPhoneBubbleWrapper::SelectedConfMemberCallIdL(
    TPhoneCommandParam* aCommandParam )
    {
    CBubbleManager::TBubbleId selected =
        iBubbleManager->SelectionIdInConference();
    TInt callId;

    TBool ok = iMapping->FindCallByBubble( selected, callId );

    TPhoneCmdParamInteger* locationContent =
        static_cast<TPhoneCmdParamInteger*>( aCommandParam );

    locationContent->SetInteger( callId );
    return ok;
    }

// ---------------------------------------------------------
// CPhoneBubbleWrapper::OpenConferenceList
// ---------------------------------------------------------
//
void CPhoneBubbleWrapper::OpenConferenceList(
    TPhoneCommandParam* aCommandParam )
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneBubbleWrapper::OpenConferenceList");
    // Check if the given parameter is valid
    if ( aCommandParam->ParamId() == TPhoneCommandParam::EPhoneParamIdBoolean )
        {
        TPhoneCmdParamBoolean* booleanParam =
            static_cast<TPhoneCmdParamBoolean*>( aCommandParam );
        SetConferenceHighlight( booleanParam->Boolean() );
        }
    }

// -----------------------------------------------------------------------------
// CPhoneBubbleWrapper::MoveHighlightInConference
// -----------------------------------------------------------------------------
//
void CPhoneBubbleWrapper::MoveHighlightInConference(
    TPhoneCommandParam* aCommandParam )
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneBubbleWrapper::MoveHighlightInConference");
    // Check Is selection list Highlight
    if ( iFlags & EFlagConferenceReqExpanded )
        {
        if ( aCommandParam->ParamId() ==
            TPhoneCommandParam::EPhoneParamIdKeyEvent )
            {
            TPhoneCmdParamKeyEvent* keyEventParam =
                static_cast<TPhoneCmdParamKeyEvent*>( aCommandParam );

            if ( keyEventParam->KeyEvent().iCode == EKeyUpArrow )
                {
                iBubbleManager->MoveHighlightOneUpInConference();
                }
            else if ( keyEventParam->KeyEvent().iCode == EKeyDownArrow  )
                {
                iBubbleManager->MoveHighlightOneDownInConference();
                }
            }
        }
    }

// -----------------------------------------------------------------------------
// CPhoneBubbleWrapper::GetCallState
// -----------------------------------------------------------------------------
//
TInt CPhoneBubbleWrapper::GetCallState(
    TPhoneCommandParam* aCommandParam )
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneBubbleWrapper::GetCallState");
    TPEState callState = EPEStateUnknown;

    CBubbleManager::TPhoneCallState bubbleState = CBubbleManager::ENone;
    if ( aCommandParam->ParamId() ==
        TPhoneCommandParam::EPhoneParamIdCallStateData )
        {
        TPhoneCmdParamCallStateData* stateParam =
            static_cast<TPhoneCmdParamCallStateData*>( aCommandParam );
        bubbleState = iMapping->FindCallStateByCall( stateParam->CallId() );
        callState = ConvertCallState(  bubbleState );
        stateParam->SetCallState( callState );
        }
    return bubbleState;
    }

// -----------------------------------------------------------------------------
// CPhoneBubbleWrapper::GetCallIdByState
// -----------------------------------------------------------------------------
//
TInt CPhoneBubbleWrapper::GetCallIdByState(
    TPhoneCommandParam* aCommandParam )
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneBubbleWrapper::GetCallIdByState");
    TInt callId = KErrNotFound;

    if ( aCommandParam->ParamId() ==
        TPhoneCommandParam::EPhoneParamIdCallStateData )
        {
        TPhoneCmdParamCallStateData* stateParam =
            static_cast<TPhoneCmdParamCallStateData*>( aCommandParam );
        CBubbleManager::TPhoneCallState callState =
            GetBubbleState( stateParam->CallState() );

         __PHONELOG1(
            EBasic,
            EPhoneUIView,
            "CPhoneBubbleWrapper::GetCallIdByState iConference = %d",
            iConference );

        if ( iConference )
            {
            if ( iMapping->FindCallStateByCall( KConferenceCallId ) == callState )
                {
                callId = KConferenceCallId;
                }
            else
                {
                callId = iMapping->FindCallIdByCallState( callState );

                // check if this call conference member
                if ( FindParticipantPosition( callId ) != KErrNotFound )
                    {
                    callId = KConferenceCallId;
                    }

                // Check if not found Incoming that is there waiting
                if ( ( callId == KErrNotFound ) &&
                    ( callState == CBubbleManager::EIncoming ) )
                    {
                    callId = iMapping->FindCallIdByCallState( CBubbleManager::EWaiting );
                    }
                }
            }
        else
            {
            callId = iMapping->FindCallIdByCallState( callState );
            // Check if not found Incoming that is there waiting
            if ( ( callId == KErrNotFound ) &&
                ( callState == CBubbleManager::EIncoming ) )
                {
                callId = iMapping->FindCallIdByCallState(
                    CBubbleManager::EWaiting );
                }
            }

        stateParam->SetCallId( callId );
        }
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CPhoneBubbleWrapper::GetBubbleState
// -----------------------------------------------------------------------------
//
CBubbleManager::TPhoneCallState CPhoneBubbleWrapper::GetBubbleState(
    TPEState aState )
    {
    CBubbleManager::TPhoneCallState callState = CBubbleManager::ENone;
    switch ( aState )
        {
        case EPEStateIdle:
            callState = CBubbleManager::EDisconnected;
            break;

        case EPEStateDialing:
            callState = CBubbleManager::EOutgoing;
            break;

        case EPEStateRinging:
            if ( iMapping->ItemCount() > 1 )
                {
                callState = CBubbleManager::EWaiting;
                }
            else
                {
                callState = CBubbleManager::EIncoming;
                }
            break;

        case EPEStateConnecting:
            callState = CBubbleManager::EAlerting;
            break;

        case EPEStateConnected:
            callState = CBubbleManager::EActive;
            break;

        case EPEStateHeld:
            callState = CBubbleManager::EOnHold;
            break;

        case EPEStateDisconnecting:
            callState = CBubbleManager::EDisconnected;
            break;

        case EPEStateConferenceIdle:
            callState = CBubbleManager::EDisconnected;
            break;

        // GSM specific
        case EPEStateConnectedConference:
            callState = CBubbleManager::EActive;
            break;

        case EPEStateHeldConference:
            callState = CBubbleManager::EOnHold;
            break;

        case EPEStateUnknown:
            break;

        default:
            break;
        }
    return callState;
    }

// -----------------------------------------------------------------------------
// CPhoneBubbleWrapper::ConvertCallState
// -----------------------------------------------------------------------------
//
TPEState CPhoneBubbleWrapper::ConvertCallState(
    CBubbleManager::TPhoneCallState aBubbleState )
    {
    TPEState callState = EPEStateUnknown;
    switch ( aBubbleState )
        {
        case CBubbleManager::EOutgoing:
            callState = EPEStateDialing;
            break;
        case CBubbleManager::EWaiting:
        case CBubbleManager::EIncoming:
            callState = EPEStateRinging;
            break;
        case CBubbleManager::EAlerting:
            callState = EPEStateConnecting;
            break;
        case CBubbleManager::EActive:
            callState = EPEStateConnected;
            break;
        case CBubbleManager::EOnHold:
            callState = EPEStateHeld;
            break;
        case CBubbleManager::EDisconnected:
            callState = EPEStateDisconnecting;
            break;
        default:
            break;
        }
    return callState;
    }

// -----------------------------------------------------------------------------
// CPhoneBubbleWrapper::ConferenceMemberToPrivateL
// -----------------------------------------------------------------------------
//
void CPhoneBubbleWrapper::ConferenceMemberToPrivateL(
        TInt aCallId )
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneBubbleWrapper::ConferenceMemberToPrivate");
    TInt pos = FindParticipantPosition( aCallId );

    if ( pos != KErrNotFound )
        {
        TInt count = iParticipants->Count();

        // If there are enough participants for conference, then
        // simply remove it from conference.
        if ( count > KPhoneConferenceInitialSize )
            {
            iParticipants->Delete( pos );
            SetConferenceHighlight( EFalse );
            SetConferenceExpand( EFalse );
            iBubbleManager->StartChanges();
            CBubbleManager::TBubbleId bubble = CreateBubbleL( aCallId );
            iBubbleManager->RemoveRowFromConference( bubble );
            iBubbleManager->EndChanges();
            }
        else
        // Otherwise we also have to clear the conference.
            {
            RemoveConferenceL();
            }
        }
    }

// -----------------------------------------------------------------------------
// CPhoneBubbleWrapper::GetActiveCallsCount
// -----------------------------------------------------------------------------
//
void CPhoneBubbleWrapper::GetActiveCallsCount( TPhoneCommandParam* aCommandParam )
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneBubbleWrapper::GetActiveCallsCount");

    if ( aCommandParam->ParamId() ==
        TPhoneCommandParam::EPhoneParamIdInteger )
        {
        TPhoneCmdParamInteger* intParam =
            static_cast<TPhoneCmdParamInteger*>( aCommandParam );

        intParam->SetInteger( CountNumberOfActiveCalls() );
        }
    }

// -----------------------------------------------------------------------------
// CPhoneBubbleWrapper::CountNumberOfActiveCalls
// -----------------------------------------------------------------------------
//
TInt CPhoneBubbleWrapper::CountNumberOfActiveCalls() const
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneBubbleWrapper::CountNumberOfActiveCalls");
    TInt callCount = iMapping->ItemCount();
    TInt activeCallCount = 0;

    if ( iConference )
        {
        if ( iMapping->FindCallIdByCallState(
            CBubbleManager::EActive ) != KErrNotFound )
            {
            activeCallCount++;
            }
        if ( iMapping->FindCallIdByCallState(
            CBubbleManager::EOnHold ) != KErrNotFound )
            {
            activeCallCount++;
            }
        if ( iMapping->FindCallIdByCallState(
            CBubbleManager::EDisconnected ) != KErrNotFound )
            {
            activeCallCount++;
            }
        }
    else
        {
        CBubbleManager::TPhoneCallState bubbleState = CBubbleManager::ENone;
        TInt callId ( KErrNotFound );

        for ( TInt index = ( callCount - 1 ); index >= 0; index-- )
            {
            callId = iMapping->CallIdAt( index );
            bubbleState = iMapping->FindCallStateByCall( callId );

            if ( bubbleState == CBubbleManager::EActive ||
                 bubbleState == CBubbleManager::EOnHold ||
                 bubbleState == CBubbleManager::EDisconnected )
                {
                activeCallCount++;
                }
            }
        }
    return activeCallCount;
    }

// -----------------------------------------------------------------------------
// CPhoneBubbleWrapper::GetIsConference
// -----------------------------------------------------------------------------
//
void CPhoneBubbleWrapper::GetIsConference( TPhoneCommandParam* aCommandParam )
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneBubbleWrapper::GetIsConference");

    if( aCommandParam->ParamId() == TPhoneCommandParam::EPhoneParamIdBoolean )
        {
        TPhoneCmdParamBoolean* booleanValue =
            static_cast<TPhoneCmdParamBoolean*>( aCommandParam );
        booleanValue->SetBoolean( iConference );
        }
    }

// -----------------------------------------------------------------------------
// CPhoneBubbleWrapper::CallExistsInConference
// -----------------------------------------------------------------------------
//
void CPhoneBubbleWrapper::CallExistsInConference(
    TInt aCallId,
    TPhoneCommandParam* aCommandParam )
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneBubbleWrapper::CallExistsInConference");

    if( aCommandParam->ParamId() == TPhoneCommandParam::EPhoneParamIdBoolean )
        {
        TPhoneCmdParamBoolean*  conferenceDataValue =
            static_cast<TPhoneCmdParamBoolean*>(
            aCommandParam );
        if ( FindParticipantPosition( aCallId ) == KErrNotFound )
            {
            conferenceDataValue->SetBoolean( EFalse );
            }
        else
            {
            conferenceDataValue->SetBoolean( ETrue );
            }
        }
    }
// -----------------------------------------------------------------------------
// CPhoneBubbleWrapper::RemoveAllCalls
// -----------------------------------------------------------------------------
//
void CPhoneBubbleWrapper::RemoveAllCallHeadersL()
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "RemoveAllCallHeaders::RemoveAllCallHeadersL");
    if ( iConference )
        {
        RemoveConferenceL();
        }
    TInt callCount = iMapping->ItemCount();

    // Find bubbles to conference
    // Delete in reverse order or otherwise mapping gets corrupted in between
    for ( TInt index = ( callCount - 1 ); index >= 0; index-- )
        {
        RemoveCallHeaderL( iMapping->CallIdAt( index ));
        }
    }

// -----------------------------------------------------------------------------
// CPhoneBubbleWrapper::AddToConferenceL
// -----------------------------------------------------------------------------
//
void CPhoneBubbleWrapper::AddToConferenceL()
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneBubbleWrapper::AddToConference");
    if ( iConference && !IsMaxMembersInConf() )
        {
        TInt callCount = iMapping->ItemCount();
        TInt index = callCount-1;
        TInt callId;

        while ( index >= 0 )
            {
            callId = iMapping->CallIdAt( index );
            // Check that call not conference
            if ( callId != KConferenceCallId &&
                 iMapping->FindCallStateByCall( callId ) !=
                        CBubbleManager::EWaiting )
                {
                if ( FindParticipantPosition( callId )
                    == KErrNotFound )
                    {
                    AddToConferenceL( callId );
                    return;
                    }
                }
            index--;
            }
        }
    }

// -----------------------------------------------------------------------------
// CPhoneBubbleWrapper::UpdateCipheringIndicator
// -----------------------------------------------------------------------------
//
void CPhoneBubbleWrapper::UpdateCipheringIndicator(
    TInt aCallId, TPhoneCommandParam* aCommandParam )
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneBubbleWrapper::SetCiphering");
    
    TPhoneCmdParamCallHeaderData* callHeaderParam = 
        static_cast<TPhoneCmdParamCallHeaderData*>( aCommandParam );
        
    iCipheringIndicatorAllowed = callHeaderParam->CipheringIndicatorAllowed();
    
    TInt callCount( iMapping->ItemCount() );
    if( callCount > 0 )
        {
        iBubbleManager->StartChanges();

        for( TInt i = 0; i < callCount; i++ )
            {
// Update ciphering to active bubble
            CBubbleManager::TBubbleId bubble = iMapping->BubbleAt( i );
            if ( aCallId == iMapping->CallIdAt( i ) )
                {
                iBubbleManager->SetCallFlag( 
                    bubble, 
                    CBubbleManager::ENoCiphering,
                    iCipheringIndicatorAllowed && 
                    !callHeaderParam->Ciphering() ); 
                }
            }
            
        iBubbleManager->EndChanges();
        }
    }

// -----------------------------------------------------------------------------
// CPhoneBubbleWrapper::IsMaxMembersInConf
// -----------------------------------------------------------------------------
//
TBool CPhoneBubbleWrapper::IsMaxMembersInConf() const
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneBubbleWrapper::IsMaxMembersInConf");
    return ( iParticipants->Count() == KConferenceMaxParticipants );
    }


// -----------------------------------------------------------------------------
// CPhoneBubbleWrapper::PlayVideoRingTone
// -----------------------------------------------------------------------------
//
void CPhoneBubbleWrapper::PlayVideoRingTone(
    const TDesC& aFileName,
    TPlayMode aPlayMode,
    TInt aVolumeLevel,
    TBool aArbitraryScaling,
    MPhoneVideoPlayerObserver* aObserver )
    {
    // store the observer
    iVideoPlayerObserver = aObserver;

    // convert the play type
    CBubbleManager::TBubbleVideoPlayMode bmPlayMode;
    switch ( aPlayMode )
        {
        case EPlayOnce:
            bmPlayMode = CBubbleManager::EPlayOnce;
            break;
        case EPlayAscending:
            bmPlayMode = CBubbleManager::EPlayAscending;
            break;
        case EPlayInLoop:
        default:
            bmPlayMode = CBubbleManager::EPlayInLoop;
            break;
        }

    iBubbleManager->StartChanges();
    iBubbleManager->SetVideoRingTone( aFileName,
                                     bmPlayMode,
                                     aVolumeLevel,
                                     aArbitraryScaling,
                                     this );
    iBubbleManager->EndChanges();
    }

// -----------------------------------------------------------------------------
// CPhoneBubbleWrapper::StopVideoRingTone
// -----------------------------------------------------------------------------
//
void CPhoneBubbleWrapper::StopVideoRingTone()
    {
    iBubbleManager->StopVideoRingTone();
    }

// -----------------------------------------------------------------------------
// CPhoneBubbleWrapper::MuteVideoRingTone
// -----------------------------------------------------------------------------
//
void CPhoneBubbleWrapper::MuteVideoRingTone()
    {
    iBubbleManager->MuteVideoRingTone();
    }

// -----------------------------------------------------------------------------
// CPhoneBubbleWrapper::CancelVideoRingTone
// -----------------------------------------------------------------------------
//
void CPhoneBubbleWrapper::CancelVideoRingTone()
    {
    iBubbleManager->StartChanges();
    iBubbleManager->CancelVideoRingTone();
    iBubbleManager->EndChanges();
    }

// -----------------------------------------------------------------------------
// CPhoneBubbleWrapper::HandleBubbleVideoPlaybackEvent
// -----------------------------------------------------------------------------
//
void CPhoneBubbleWrapper::HandleBubbleVideoPlaybackEvent(
    TBubbleVideoPlaybackEvent aEvent )
    {

    if ( iVideoPlayerObserver )
        {
        switch ( aEvent )
            {
            case EPlayerInitComplete:
                iVideoPlayerObserver->HandleVideoPlayerInitComplete();
                break;
            case EPlayingComplete:
                iVideoPlayerObserver->HandleVideoPlayerPlayingComplete();
                break;
            default:
                break;
            }
        }
    }

// -----------------------------------------------------------------------------
// CPhoneBubbleManager::HandleBubbleVideoPlaybackError
// -----------------------------------------------------------------------------
//
void CPhoneBubbleWrapper::HandleBubbleVideoPlaybackError(
    TBubbleVideoPlaybackError aErrorType, TInt aErrorCode )
    {

    if ( iVideoPlayerObserver )
        {
        switch ( aErrorType )
            {
            case EPlayerInitFailure:
                iVideoPlayerObserver->HandleVideoPlayerError(
                    MPhoneVideoPlayerObserver::EVideoPlayerInitializingFailure,
                    aErrorCode );
                break;
            case EPlaybackFailure:
                iVideoPlayerObserver->HandleVideoPlayerError(
                    MPhoneVideoPlayerObserver::EVideoPlayerPlayingFailure,
                    aErrorCode );
                break;
            default:
                break;
            }
        }
    }

// -----------------------------------------------------------------------------
// CPhoneBubbleManager::HandleCommandL
// -----------------------------------------------------------------------------
//
void CPhoneBubbleWrapper::HandleCommandL( TInt /*aCommand*/ )
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneBubbleWrapper::HandleCommandL");
    }

// -----------------------------------------------------------------------------
// CPhoneBubbleWrapper::ToggleNumberEntryAlphaNumericMode
// -----------------------------------------------------------------------------
//
void CPhoneBubbleWrapper::ToggleNumberEntryAlphaNumericMode(
    TPhoneCommandParam* aCommandParam )
    {
    __LOGMETHODSTARTEND( EPhoneUIView,
        "CPhoneBubbleWrapper::ToggleNumberEntryAlphaNumericMode()" );
    if( aCommandParam->ParamId() == TPhoneCommandParam::EPhoneParamIdBoolean )
        {
        TPhoneCmdParamBoolean*  modeAlpha =
            static_cast<TPhoneCmdParamBoolean*>(
            aCommandParam );

        if ( iIsNumberEntryModeNumeric )
	        {
	        iIsNumberEntryModeNumeric = EFalse;
	        if ( IsNumberEntryNumericMode() )
		        {
		        if ( !iUseDialer )
		            {
		            if ( CountNumberEntryCharacters() == 1 )
	                    {
	                    // Number entry contents is cleared as specified in 
	                    // In-Call Operations, 4.2 Option menu functions.
	                    iBubbleManager->SetTextToNumberEntry( KNullDesC() );
	                    }
		            iBubbleManager->ChangeEditorMode( EFalse );
		            }
		        }
	        }
        else
	        {
	        iIsNumberEntryModeNumeric = ETrue;
	        if ( !IsNumberEntryNumericMode() )
		        {
	             if ( !iUseDialer )
	                 {
	                 iBubbleManager->ChangeEditorMode( EFalse );
	                 }
		        }		
	        }
        modeAlpha->SetBoolean( !iIsNumberEntryModeNumeric );
        }
    }
        
// -----------------------------------------------------------------------------
// CPhoneBubbleWrapper::OpenVKBL
// -----------------------------------------------------------------------------
//
void CPhoneBubbleWrapper::OpenVkbL()
    {
    if ( iUseDialer )
        {
        iDialerNumberEntry->OpenVkbL();
        }
    }

// -----------------------------------------------------------------------------
// CPhoneBubbleWrapper::GetNumberEntryCursorPosition
// -----------------------------------------------------------------------------
//
void CPhoneBubbleWrapper::GetNumberEntryCursorPosition(
    TPhoneCommandParam* aCommandParam )
    {
    __LOGMETHODSTARTEND( EPhoneUIView,
        "CPhoneBubbleWrapper::GetNumberEntryCursorPosition()" );
    if ( aCommandParam->ParamId() ==
        TPhoneCommandParam::EPhoneParamIdInteger )
        {
        TPhoneCmdParamInteger* intParam =
            static_cast<TPhoneCmdParamInteger*>( aCommandParam );

        CAknPhoneNumberEditor* control(NULL); 
            
        if ( iUseDialer )
            {
            control = static_cast<CAknPhoneNumberEditor*>
                ( iDialerNumberEntry->GetNumberEntry() );
            }
        else
            {
            control = static_cast<CAknPhoneNumberEditor*>
                ( iBubbleManager->GetNumberEntry() );
            }
        intParam->SetInteger( control->CursorPos() );
        }
    }

// -----------------------------------------------------------------------------
// CPhoneBubbleWrapper::SetNumberEntryCursorPositionL
// -----------------------------------------------------------------------------
//
void CPhoneBubbleWrapper::SetNumberEntryCursorPositionL(
    TPhoneCommandParam* aCommandParam )
    {
    __LOGMETHODSTARTEND( EPhoneUIView,
        "CPhoneBubbleWrapper::SetNumberEntryCursorPositionL() ");
    if ( aCommandParam->ParamId() ==
        TPhoneCommandParam::EPhoneParamIdInteger )
        {
        TPhoneCmdParamInteger* intParam =
            static_cast<TPhoneCmdParamInteger*>( aCommandParam );
        CAknPhoneNumberEditor* control(NULL);
            
        if ( iUseDialer )
            {    
            control = static_cast<CAknPhoneNumberEditor*>
                ( iDialerNumberEntry->GetNumberEntry() );
            }
        else
            {
            control = static_cast<CAknPhoneNumberEditor*>
                ( iBubbleManager->GetNumberEntry() );
            }
        control->SetCursorPosL( intParam->Integer(), EFalse );
        }
    }

// -----------------------------------------------------------------------------
// CPhoneBubbleWrapper::IsNumberEntryNumericMode
// -----------------------------------------------------------------------------
//
TBool CPhoneBubbleWrapper::IsNumberEntryNumericMode() const
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneBubbleWrapper::IsNumberEntryNumericMode()" );
    if ( iUseDialer )
        { 
        return iDialerNumberEntry->GetEditorMode() == EAknEditorNumericInputMode;
        }
    else
        {
        return iBubbleManager->GetEditorMode() == EAknEditorNumericInputMode;
        }
    }

// -----------------------------------------------------------------------------
// CPhoneBubbleWrapper::IsNENumericMode()
// -----------------------------------------------------------------------------
//
TBool CPhoneBubbleWrapper::IsNENumericMode() const
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneBubbleWrapper::IsNENumericMode()" );
    
    if ( iUseDialer )
        {
        return EAknEditorNumericInputMode == 
            iDialerNumberEntry->GetEditorMode();
        }
    else
        {
        return iIsNumberEntryModeNumeric;    
        }
    }

// -----------------------------------------------------------------------------
// CPhoneBubbleWrapper::StartChanges
// -----------------------------------------------------------------------------
//
void CPhoneBubbleWrapper::StartChanges()
    {
    iBubbleManager->StartChanges();
    }

// -----------------------------------------------------------------------------
// CPhoneBubbleWrapper::EndChanges
// -----------------------------------------------------------------------------
//
void CPhoneBubbleWrapper::EndChanges()
    {
    iBubbleManager->EndChanges();
    }

// -----------------------------------------------------------------------------
// CPhoneBubbleManager::SetTouchPaneVisible
// -----------------------------------------------------------------------------
//
void CPhoneBubbleWrapper::SetTouchPaneVisible( TBool aVisible )
    {
    iBubbleManager->StartChanges();
    iBubbleManager->SetTouchPaneVisible( aVisible );
    iBubbleManager->EndChanges();
    }

// -----------------------------------------------------------------------------
// CPhoneBubbleWrapper::TouchPane
// -----------------------------------------------------------------------------
//
MBubbleTouchPaneInterface& CPhoneBubbleWrapper::TouchPane() const
    {
    return *iBubbleManager->TouchPane();
    }

// -----------------------------------------------------------------------------
// CPhoneBubbleWrapper::BubbleManager
// -----------------------------------------------------------------------------
//
CBubbleManager& CPhoneBubbleWrapper::BubbleManager()
    {
    return *iBubbleManager;
    }

// -----------------------------------------------------------------------------
// CPhoneBubbleWrapper::BubbleManager
// -----------------------------------------------------------------------------
//
CCoeControl* CPhoneBubbleWrapper::Control()
    {
    return iBubbleManager;
    }

// -----------------------------------------------------------------------------
// CPhoneBubbleWrapper::IsConferenceInExpandedMode
// -----------------------------------------------------------------------------
//
TBool CPhoneBubbleWrapper::IsConferenceInExpandedMode() const
    {
    return iFlags & EFlagConferenceReqExpanded;
    }

// -----------------------------------------------------------------------------
// CPhoneBubbleWrapper::LoadPlugins
// Loads the bubble extension plugins.
// -----------------------------------------------------------------------------
//
void CPhoneBubbleWrapper::LoadPlugins()
    {
    TRAP_IGNORE( iBubbleExtensionManager->InitializeL() );
    }

// -----------------------------------------------------------------------------
// CPhoneBubbleWrapper::SetPhoneCustomization
// -----------------------------------------------------------------------------
//
void CPhoneBubbleWrapper::SetPhoneCustomization( 
        MPhoneCustomization* aCustomization )
    {
    iPhoneCustomization = aCustomization;
    }

// -----------------------------------------------------------------------------
// CPhoneBubbleWrapper::SetNumberEntry
// -----------------------------------------------------------------------------
//
void CPhoneBubbleWrapper::SetNumberEntry( MNumberEntry* aNumberEntry )
    {
    iDialerNumberEntry = aNumberEntry;
    iPhoneNumberEntry->SetNumberEntry( aNumberEntry );
    }

// -----------------------------------------------------------------------------
// CPhoneBubbleWrapper::CallThemeSettingsChanged
// -----------------------------------------------------------------------------
//
void CPhoneBubbleWrapper::CallThemeSettingsChanged()
    {
    iBubbleManager->PreloadCallThemeImage(
            iCallTheme->CallImageThemeSettings() == 2 );
    }

// -----------------------------------------------------------------------------
// CPhoneBubbleWrapper::KeypadAudioEnabled
// -----------------------------------------------------------------------------
//
void CPhoneBubbleWrapper::KeypadAudioEnabled()
    {
    // Tactile sound feedback must be disabled when keypad volume is 1...5
    // otherwise DTMF tone and tactile tone is played simultaneously.
    iPhoneNumberEntry->EnableTactileFeedback( EFalse );
    }

// -----------------------------------------------------------------------------
// CPhoneBubbleWrapper::KeypadAudioDisabled
// -----------------------------------------------------------------------------
//
void CPhoneBubbleWrapper::KeypadAudioDisabled()
    {
    // Tactile sound feedback can be enabled when keypad volume is 0
    iPhoneNumberEntry->EnableTactileFeedback( ETrue );
    }


// -----------------------------------------------------------------------------
// CPhoneBubbleWrapper::IsSimplifiedConferenceCallHeaderEnabledL
// -----------------------------------------------------------------------------
//
TBool CPhoneBubbleWrapper::IsSimplifiedConferenceCallHeaderEnabledL( 
        TUint32 aServiceId ) const
    {
    __LOGMETHODSTARTEND( EPhoneUIView, 
        "CPhoneBubbleWrapper::IsSimplifiedConferenceCallHeaderEnabledL" );
    
    TBool isSimplifiedBubble = EFalse;
    
    CSPSettings* settings = CSPSettings::NewLC();
    CSPProperty* property = CSPProperty::NewLC();
    
    TInt serviceMask = 0;
    TInt result = settings->FindPropertyL( 
        aServiceId, EPropertyServiceAttributeMask, *property );
    if ( KErrNone == result ) 
        {
        result = property->GetValue( serviceMask );
        if ( KErrNone == result ) 
            {
            isSimplifiedBubble =
                ( serviceMask & static_cast<TInt>( ESimplifiedConferenceCallBubble ) );
            }
        }
    
    CleanupStack::PopAndDestroy( property );
    CleanupStack::PopAndDestroy( settings );
    
    return isSimplifiedBubble; 
    }


// -----------------------------------------------------------------------------
// CPhoneBubbleWrapper::SetSimplifiedConferenceCallHeaderStatus
// -----------------------------------------------------------------------------
//
void CPhoneBubbleWrapper::SetSimplifiedConferenceCallHeaderStatus( 
        TBool aOnOff )
    {
    __LOGMETHODSTARTEND( EPhoneUIView, 
        "CPhoneBubbleWrapper::SetSimplifiedConferenceCallHeaderStatus" );
    
    TInt flags = iFlags;
    flags &= ~EFlagSimplifiedConferenceCallHeader;
    
    if ( aOnOff )
        {
        flags |= EFlagSimplifiedConferenceCallHeader;
        }
    
    HandleFlagsUpdated( flags );
    }

//  End of File  
