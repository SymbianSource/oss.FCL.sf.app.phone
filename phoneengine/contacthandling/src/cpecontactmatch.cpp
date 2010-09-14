/*
* Copyright (c) 2002-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This module contains the implementation of CPEContactMatch class
*
*/


// INCLUDE FILES
#include "cpecontacthandling.h"
#include "cpecontactmatch.h"
#include <cphonecntfactory.h>
#include <cphcntmatcher.h>
#include <cphcntspeeddialmonitor.h>
#include <cphcntthumbnailloader.h>
#include <mpeclientinformation.h>
#include <mpedatastore.h>
#include <mphcntmatch.h>
#include <mphcntthumbnailloaderobserver.h>
#include <pepanic.pan>
#include <pevirtualengine.h>
#include <talogger.h>
#include <telmatchingextension.h>
#include <spsettings.h>
#include <spdefinitions.h>
#include <spproperty.h>
#include <spentry.h>
#include "cpecontactextensionwrapper.h"

// CONSTANTS

// Space - used as a separator between concatenated first and last name
_LIT( KPESpace," ");


// Contact group array granularity
const TInt KPEContactGroupArrayGranularity = 5;
// Minimum value of meanigful digits
const TInt KMinValueOfMeaningfulDigits = 4;
// Maximum value of meanigful digits
const TInt KMaxValueOfMeaningfulDigits = 20;


// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------------------------
// CPEContactMatch::NewL
// ---------------------------------------------------------------------------
//
CPEContactMatch* CPEContactMatch::NewL(
        CPEContactHandling& aOwner,
        MPEDataStore& aDataStore
        )
    {
    CPEContactMatch* self = 
        new ( ELeave ) CPEContactMatch( aOwner, aDataStore );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CPEContactMatch::~CPEContactMatch()
    {
    delete iSpeedDialCommand;
    if ( iContactThumbnailLoader && iThumbnailId )
        {
        iContactThumbnailLoader->Cancel( iThumbnailId );
        }
    delete iThumbnailImage;
    delete iContactThumbnailLoader;
    delete iContactMatcher;
    delete iContactFactory;
    
    iPlugins.ResetAndDestroy();
    REComSession::FinalClose();
    iLibrary.Close();
    }

// -----------------------------------------------------------------------------
// CPEContactMatch::CreateSingleItemFetchL
// -----------------------------------------------------------------------------
//
CPhCntSingleItemFetch* CPEContactMatch::CreateSingleItemFetchL()
    {
    return iContactFactory->CreateSingleItemFetchL();
    }

// -----------------------------------------------------------------------------
// CPEContactMatch::CreateContactIdL
// -----------------------------------------------------------------------------
//
CPhCntContactId* CPEContactMatch::CreateContactIdL( const TDesC8& aContactId )
    {  
    return iContactFactory->CreateContactIdL( aContactId );
    }

// -----------------------------------------------------------------------------
// CPEContactMatch::CreateContactIdL
// -----------------------------------------------------------------------------
//
CPhCntContactId* CPEContactMatch::CreateContactIdL(
    TContactItemId aContactId )
    {  
    return iContactFactory->CreateContactIdL( aContactId );
    }

// -----------------------------------------------------------------------------
// CPEContactMatch::CPEContactMatch
// -----------------------------------------------------------------------------
//
CPEContactMatch::CPEContactMatch(
        CPEContactHandling& aOwner,
        MPEDataStore& aDataStore )
        : iOwner( aOwner ), iDataStore( aDataStore )
    {
    }

// -----------------------------------------------------------------------------
// CPEContactMatch::ConstructL
// -----------------------------------------------------------------------------
//
void CPEContactMatch::ConstructL()
    {
    //Mandatory services for providing contact match functionality
    TEFLOGSTRING( KTAOBJECT, "CNT CPEContactMatch::ConstructL > RLibrary::Load" );
    iContactFactory = CreateCntFactoryL();
    TEFLOGSTRING( KTAOBJECT, "CNT CPEContactMatch::ConstructL > CPhCntFactory::CreateContactMatcherL" );
    iContactMatcher = iContactFactory->CreateContactMatcherL();

    //Secondary services that may be enabled on demand
    ConstructThumbnailLoaderL();
    ConstructSpeedDialCommandL();
    }

// -----------------------------------------------------------------------------
// CPEContactMatch::ConstructThumbnailLoaderL
// -----------------------------------------------------------------------------
//
void CPEContactMatch::ConstructThumbnailLoaderL()
    {
    if ( !iContactThumbnailLoader )
        {
        TEFLOGSTRING( KTAOBJECT, "CNT CPEContactMatch::ConstructL > CPhCntFactory::CreateThumbnailLoaderL" );
        iContactThumbnailLoader = iContactFactory->CreateThumbnailLoaderL( *this );
        }
    else
        {
        TEFLOGSTRING( KTAERROR,
            "CNT CPECONTACTMATCH::CONSTRUCTTHUMBNAILLOADERL ! ALREADY EXISTS" );
        }
    }

// -----------------------------------------------------------------------------
// CPEContactMatch::ConstructSpeedDialCommandL
// -----------------------------------------------------------------------------
//
void CPEContactMatch::ConstructSpeedDialCommandL()
    {
    if ( !iSpeedDialCommand )
        {
        TEFLOGSTRING( KTAOBJECT, "CNT CPEContactMatch::ConstructL > CPhCntFactory::CreateSpeedDialMonitorL" );
        iSpeedDialCommand = iContactFactory->CreateSpeedDialMonitorL();
        }
    else
        {
        TEFLOGSTRING( KTAERROR,
            "CNT CPECONTACTMATCH::CONSTRUCTSPEEDDIALCOMMANDL ! ALREADY EXISTS" );
        }
    }

// -----------------------------------------------------------------------------
// CPEContactMatch::MatchWithContactIdL
// Synchronous method for finding contact data specified 
// with aContactId.
// -----------------------------------------------------------------------------
//
void CPEContactMatch::MatchWithContactIdL(
        const TInt aCallId
        )
    {
    TEFLOGSTRING( KTAINT, "CNT CPEContactMatch::MatchWithContactIdL" );
    MPhCntMatch* match( NULL );
    CPhCntContactId* contactId( NULL );
    contactId = CreateContactIdL( iDataStore.CallClientInformation( aCallId ).ContactLink() );
    CleanupStack::PushL( contactId );
    if ( iDataStore.CallType( aCallId ) == EPECallTypeVoIP )
        {
        if ( contactId && contactId->IsValid() )
            {
            TEFLOGSTRING( KTAINT, "CNT CPEContactMatch::MatchWithContactIdL > CPhCntMatcher::MatchVoipNumber" );
            // Get contact based on contact id.
            iContactMatcher->MatchVoipNumber( match, 
                                              iDataStore.RemotePhoneNumber( aCallId ),
                                              *contactId );
            }
        else
            {
            User::Leave( KErrArgument );
            }
        }
    else
        {
        if ( contactId && contactId->IsValid() )
            {
            TEFLOGSTRING( KTAINT, "CNT CPEContactMatch::MatchWithContactIdL > CPhCntMatcher::MatchNumber" );
            iContactMatcher->MatchNumber( match, 
                                          iDataStore.RemotePhoneNumber( aCallId ),
                                          *contactId );
            }
        else
            {
            User::Leave( KErrArgument );
            }
        }

    if ( match )
        {
        TEFLOGSTRING( KTAINT, "CNT CPEContactMatch::MatchWithContactIdL, Contact match found" );
        CopyContactFieldsDataL( *match, aCallId );

        if ( match->HasThumbnailImage() )
            {
            TEFLOGSTRING( KTAINT, "CNT CPEContactMatch::MatchWithContactIdL, Thumbnail image found" );
            // Start loading thumbnail for this contact
            // Error code may be ignored since this is not a critical function
            iDataStore.SetHasCallerThumbnail( ETrue, aCallId );
            LoadThumbnail( iDataStore.ContactId( aCallId ), aCallId );
            }
        match->Release();
        match = NULL;
        }
    CleanupStack::PopAndDestroy( contactId ); //contactId 
    }

// -----------------------------------------------------------------------------
// CPEContactMatch::MatchWithNumberL
// Synchronous method for finding contact data specified with phone number.
// -----------------------------------------------------------------------------
//
void CPEContactMatch::MatchWithNumberL(
        const TInt aCallId
        )
    {
    MPhCntMatch* match( NULL );
    RBuf remoteParty;
    remoteParty.CleanupClosePushL();

    if ( iDataStore.CallType( aCallId ) == EPECallTypeVoIP )
        {
        TEFLOGSTRING2( KTAINT, "CNT CPEContactMatch::MatchWithNumberL > CPhCntMatcher::MatchVoipNumber, call id: %d", aCallId );
        MatchWithVoipAdressL( aCallId, match, remoteParty );
        
        }
    else
        {
        TPEPhoneNumber remoteNumber = iDataStore.RemotePhoneNumber( aCallId );
        
        TEFLOGSTRING3( KTAINT, "CNT CPEContactMatch::MatchWithNumberL > CPhCntMatcher::MatchNumber, number: %S, call id: %d",
            &remoteNumber, aCallId );
        iContactMatcher->MatchNumber( match, remoteNumber );
        }

    if ( match )
        {
        TEFLOGSTRING( KTAINT, "CNT CPEContactMatch::MatchWithNumberL, Contact match found" );
        CopyContactFieldsDataL( *match, aCallId );

        if ( match->HasThumbnailImage() )
            {
            TEFLOGSTRING( KTAINT, "CNT CPEContactMatch::MatchWithNumberL, Thumbnail image found" );
            // Start loading thumbnail for this contact
            // Error code may be ignored since this is not a critical function
            iDataStore.SetHasCallerThumbnail( ETrue, aCallId );
            LoadThumbnail( iDataStore.ContactId( aCallId ), aCallId );
            }
        match->Release();
        match = NULL;
        }
    else if ( 0 < remoteParty.Length() )
        {
        TEFLOGSTRING( KTAINT, "CNT CPEContactMatch::MatchWithNumberL, Set plugins remote name" );
        // Match not found, set plugins remote party name.
        // Make sure that length of the remote name is not too long.
        iDataStore.SetRemoteName( remoteParty.Left( 
                                    Min( KCntMaxTextFieldLength, remoteParty.Length() ) ), 
                                  aCallId );
        }

    CleanupStack::PopAndDestroy( &remoteParty );
    }

// ---------------------------------------------------------
// CPEContactMatch::LoadThumbnail
// ---------------------------------------------------------
//
TInt CPEContactMatch::LoadThumbnail(
        const CPhCntContactId& aContactId,
        const TInt aCallId )
    {
    // use call id for identifying which thumbnail is loaded
    // Increment it because zero is invalid value for the id
    TInt thumbnailId = aCallId + 1;
    iThumbnailId =
        reinterpret_cast< CPhCntThumbnailLoader::TThumbnailId > ( thumbnailId );

    TEFLOGSTRING( KTAINT, "CNT CPEContactMatch::LoadThumbnail > CPhCntThumbnailLoader::Load" );
    return iContactThumbnailLoader->Load( iThumbnailId, aContactId );
    }

// ---------------------------------------------------------
// CPEContactMatch::MpctloThumbnailLoaded
// ---------------------------------------------------------
//
void CPEContactMatch::MpctloThumbnailLoaded(
        CPhCntThumbnailLoader::TThumbnailId aId,
        TInt aResult, 
        CFbsBitmap* aBitmap )
    {
    // Operation complete
    // Reset id to indicate that no thumbnail loading is in progress
    iThumbnailId = KPhCntThumbnailNullId;

    if ( aResult == KErrNone )
        {
        iThumbnailImage = aBitmap;

        // Decrement call id, see LoadThumbnail() for reason
        TInt thumbnailId = ( reinterpret_cast< TInt >( aId ) ) - 1;

        iOwner.SendMessage( MEngineMonitor::EPEMessageThumbnailLoadingCompleted, thumbnailId );
        }
    else
        {
        TEFLOGSTRING( KTAERROR,
            "CNT CPECONTACTMATCH::MPCTLOTHUMBNAILLOADED ! ERROR LOADING THUMBNAIL" );
        iOwner.SendErrorMessage( aResult );
        }
    }

// ---------------------------------------------------------
// CPEContactMatch::ContactThumbnail
// Returns contact thumbnail image, NULL if not found
// ---------------------------------------------------------
//
CFbsBitmap* CPEContactMatch::ContactThumbnail()
    {
    if ( !iThumbnailImage )
        {
        return NULL;
        }
    CFbsBitmap* returnedThumbnailImage( iThumbnailImage );
    iThumbnailImage = NULL; // Ownership is passed to caller

    return returnedThumbnailImage;
    }

// -----------------------------------------------------------------------------
// CPEContactMatch::CopyContactFieldsDataL
// Copies contact information to remote info struct
// -----------------------------------------------------------------------------
//
void CPEContactMatch::CopyContactFieldsDataL(
        const MPhCntMatch& aMatch,
        const TInt aCallId
        )
    {
    TEFLOGSTRING( KTAINT, "CNT CPEContactMatch::CopyContactFieldsDataL: start" );

    // Set ringing tone's filename
    iDataStore.SetPersonalRingingTone( aMatch.PersonalRingingTone(), aCallId );

    // Set name
    // The following logic determines if one of the languages that require
    // swapping of first/last name order is selected and performs the operation.
    HBufC* lastname( NULL );
    HBufC* firstname( NULL );

    if ( aMatch.LastName().Length() > 0 )
        {
        TEFLOGSTRING( KTAINT, "CNT CPEContactMatch::CopyContactFieldsDataL, alloc last name" );
        lastname = aMatch.LastName().Alloc();
        }
    if ( aMatch.FirstName().Length() > 0 )
        {
        TEFLOGSTRING( KTAINT, "CNT CPEContactMatch::CopyContactFieldsDataL, alloc first name" );
        firstname = aMatch.FirstName().Alloc();
        }

    if ( ( lastname ) && ( firstname ) )
        {
        //Swap name order, if required by the current language setting
        const TLanguage language = User::Language();
    
        for ( TInt index = 0; index < KPENumberOfSwappedLanguages; index++ )
            {
            if ( KPESwappedLanguages[index] == language )
                {
                HBufC* tempfirstname( firstname );
                firstname = lastname;
                lastname = tempfirstname;
                break;
                }
            }
        }

    TPEContactName finalName;
    // Parse name by concatenating lastname and firstname
    if ( firstname )
        {
        finalName = *firstname;
        if ( finalName.Length() < KCntMaxTextFieldLength && lastname )
            {
            TEFLOGSTRING( KTAINT, "CNT CPEContactMatch::CopyContactFieldsDataL, append with space" );
            finalName.Append( KPESpace );
            }
        delete firstname;
        firstname = NULL;
        }
    if ( lastname )
        {
        if ( finalName.Length() < KCntMaxTextFieldLength )
            {
            // Check how long last name can be added to the iName string. 
            TInt lastNameMaxLength = KCntMaxTextFieldLength - finalName.Length();
            if ( lastname->Length() > lastNameMaxLength )
                {
                TEFLOGSTRING( KTAINT, "CNT CPEContactMatch::CopyContactFieldsDataL, append with part of last name" );
                // Cut extra chars from the string.
                finalName.Append( lastname->Left( lastNameMaxLength ) );          
                }
            else 
                {
                TEFLOGSTRING( KTAINT, "CNT CPEContactMatch::CopyContactFieldsDataL, append with last name" );
                finalName.Append( *lastname );
                }
            }
        delete lastname;
        lastname = NULL;
        }
        
    // Replace paragraph separator with space character
    const TText ps(0x2029);

    while ( finalName.Locate( ps ) != KErrNotFound )
        {
        finalName[ finalName.Locate( ps )] = ' ';
        }

    iDataStore.SetRemoteName( finalName, aCallId );

    iDataStore.SetRemoteTextToSpeechText( aMatch.TextToSpeechTextL(), aCallId );

    if ( aMatch.Number().Length() > 0 )
        {
        iDataStore.SetRemotePhoneNumber( aMatch.Number(), aCallId );    
        }   

    TInt phcntfinderNumberType = aMatch.NumberType();
            
    switch ( phcntfinderNumberType )
        {
        case MPhCntMatch::EMobileNumber:
            iDataStore.SetRemotePhoneNumberType( EPEMobileNumber, aCallId );
            break;
        case MPhCntMatch::EStandardNumber:
            iDataStore.SetRemotePhoneNumberType( EPETelephoneNumber, aCallId );
            break;
        case MPhCntMatch::EFaxNumber:
            iDataStore.SetRemotePhoneNumberType( EPEFaxNumber, aCallId );
            break;
        case MPhCntMatch::EPagerNumber:
            iDataStore.SetRemotePhoneNumberType( EPEPager, aCallId );
            break;
        case MPhCntMatch::EVoipNumber:
            iDataStore.SetRemotePhoneNumberType( EPEVoipNumber, aCallId );
            break;
        case MPhCntMatch::EVideoNumber:
            iDataStore.SetRemotePhoneNumberType( EPEVideoNumber, aCallId );
            break;        
        case MPhCntMatch::EAssistantNumber:
            iDataStore.SetRemotePhoneNumberType( EPEAssistantNumber, aCallId );
            break;        
        case MPhCntMatch::ECarNumber:
            iDataStore.SetRemotePhoneNumberType( EPECarNumber, aCallId );
            break;        
        case MPhCntMatch::EHomeNumber:
            iDataStore.SetRemotePhoneNumberType( EPEHomeNumber, aCallId );
            break;        
        case MPhCntMatch::EWorkNumber:
            iDataStore.SetRemotePhoneNumberType( EPEWorkNumber, aCallId );
            break;        
        case MPhCntMatch::ENone: // fall through
        default:
            iDataStore.SetRemotePhoneNumberType( EPEUnknownNumber, aCallId );
            break;
        }

    // Replace paragraph separator with space character
    TPEContactName companyName;
    companyName = aMatch.CompanyName();
    while ( companyName.Locate( ps ) != KErrNotFound )
        {
        companyName[ companyName.Locate( ps )] = ' ';
        }
    // Set Company name
    iDataStore.SetRemoteCompanyName( companyName, aCallId );

    // Set Call text
    iDataStore.SetCallerText( aMatch.CallText(), aCallId );

    // Set Call image
    iDataStore.SetCallerImage( aMatch.CallImage(), aCallId );
    
    // Set Contact ID
    iDataStore.SetContactId( aMatch.ContactId()->CloneL(), aCallId );

    CArrayFixFlat< TContactItemId >* groupList
        = new ( ELeave ) CArrayFixFlat< TContactItemId >
        ( KPEContactGroupArrayGranularity );
 
    TArray<TContactItemId> alertFor = iDataStore.AlertForGroup();
    const TInt groupLength = alertFor.Count();
    for ( TInt i = 0; i < groupLength; i++ ) 
        {
        groupList->AppendL( alertFor[ i ] );
        }
   
    if ( aMatch.BelongsToGroups( *groupList ) )
        {
        // Convert array type here to ease further handling
        TArray<TContactItemId> list = groupList->Array();
        // Set group information
        iDataStore.SetContactGroups( list, aCallId );
        }
    delete groupList;
    groupList = NULL;
    
    // Set dtmf array
    iDataStore.SetPredefinedDtmfStrings( aMatch.AllDtmfNumbers(), aCallId );
    TEFLOGSTRING( KTAINT, "CNT CPEContactMatch::CopyContactFieldsDataL: complete" );  
    }

// -----------------------------------------------------------------------------
// CPEContactMatch::GetSpeedDialLocationL
// Get's phone number from given location.
// -----------------------------------------------------------------------------
//
void CPEContactMatch::GetSpeedDialLocationL( 
    TInt aLocationIndex, 
    TPEPhoneNumber& aNumber )
    {
    iSpeedDialCommand->GetSpeedDialFieldL( aLocationIndex, aNumber);
    }

// -----------------------------------------------------------
// CPEContactMatch::ResolveImplementationUidL
// -----------------------------------------------------------
//
TUid CPEContactMatch::ResolveServicesImplementationUidL( TUint32 aServiceId )
    {
    TEFLOGSTRING2( KTAINT, "CNT CPEContactMatch::ResolveService > aServiceId: %d", aServiceId );   
    TUid implUid = {0};
    
    CSPEntry* entry = CSPEntry::NewLC();
    CSPSettings* spSettings = CSPSettings::NewLC(); 
    
    User::LeaveIfError(
        spSettings->FindEntryL( aServiceId, *entry ) ); 

    CleanupStack::PopAndDestroy( spSettings ); 

    const CSPProperty* property = NULL;
    User::LeaveIfError( entry->GetProperty( property, EPropertyCLIPluginId ) );
    
    if ( property ) 
        {
        TInt temp(0);
        User::LeaveIfError( property->GetValue( temp ) );
        implUid.iUid = temp;
        }
    else
        {
        User::Leave( KErrNotFound );
        }

    CleanupStack::PopAndDestroy( entry );
    return implUid;
    }

// -----------------------------------------------------------
// CPEContactMatch::MatchWithVoipAdressL
// -----------------------------------------------------------
//
void CPEContactMatch::MatchWithVoipAdressL( 
    const TInt aCallId, 
    MPhCntMatch*& aMatch, 
    RBuf& aRemotePartyName )
    {
    TEFLOGSTRING2( KTAINT, 
        "CNT CPEContactMatch::MatchWithVoipAdressL, CALL ID: %d", aCallId )
    
    aRemotePartyName.Close();
    TRAPD( result, MatchWithVoipAddressUsingExtensionL( 
        aCallId, aMatch, aRemotePartyName ) );
    if ( KErrNone != result )
        {
        TEFLOGSTRING2( KTAINT, 
            "CNT CPEContactMatch::MatchWithVoipAdressL > PLUGIN ERR: %d", result )
            
        // Some error happened while using extension plugin.
        // -> Default matching is used.
            
        // Try to find store uris that should be opened before contact match.    
        CDesCArray* storeUris = NULL; 
        TRAP_IGNORE( storeUris = StoreUrisForServiceL( 
                iDataStore.ServiceId( aCallId )) );
        
        TPEPhoneNumber remoteNumber = iDataStore.RemotePhoneNumber( aCallId );
        iContactMatcher->MatchVoipNumber( 
            aMatch, remoteNumber, ETrue, storeUris, remoteNumber.Length() );
        
        delete storeUris;
        }
    }

// -----------------------------------------------------------
// CPEContactMatch::StoreUrisForServiceL
// -----------------------------------------------------------
//
CDesCArray* CPEContactMatch::StoreUrisForServiceL( TUint aServiceId )
    {
    CDesCArray* storeUris = new ( ELeave ) CDesC16ArrayFlat( 1 );
    CleanupStack::PushL( storeUris );
    
    // Get contact store id from Service Table
    // KSPMaxDesLength from spdefinitions.h
    HBufC* cntStoreId = HBufC::NewLC( KSPMaxDesLength );
    CSPSettings* spSettings = CSPSettings::NewLC(); 
    CSPProperty* property = CSPProperty::NewLC();
    User::LeaveIfError( spSettings->FindPropertyL( 
            TServiceId( aServiceId ), 
            EPropertyContactStoreId, 
            *property ) );                        
          
    TPtr storeUri( cntStoreId->Des() );
    User::LeaveIfError( property->GetValue( storeUri ) );   

    storeUris->AppendL( storeUri );

    CleanupStack::PopAndDestroy( property );
    CleanupStack::PopAndDestroy( spSettings );     
    CleanupStack::PopAndDestroy( cntStoreId );
    CleanupStack::Pop( storeUris );

    return storeUris;
    }

// -----------------------------------------------------------
// CPEContactMatch::MatchWithVoipAddressUsingExtensionL
// -----------------------------------------------------------
//
void CPEContactMatch::MatchWithVoipAddressUsingExtensionL( TInt aCallId, 
        MPhCntMatch*& aMatch, RBuf& aRemotePartyName )
    {
    // Create and initialize extension plugin.
    TUint32 serviceId = iDataStore.ServiceId( aCallId );
    CTelMatchingExtension& plugin = MatchingExtensionL( serviceId );
    TPEPhoneNumber remoteNumber = iDataStore.RemotePhoneNumber( aCallId );
    plugin.InitializeL( serviceId, remoteNumber );
    
    // Get service specific parameters from the plugin.
    User::LeaveIfError( plugin.GetRemotePartyName( aRemotePartyName ) );
    
    RBuf parsedAddress;
    CleanupClosePushL( parsedAddress );
    TInt digitsformatching( 0 );
    TBool allowUserNameMatch( EFalse );
    User::LeaveIfError( plugin.GetAddressForMatching( 
            parsedAddress, digitsformatching, allowUserNameMatch ) );
			
    CDesCArray* array = new ( ELeave ) CDesC16ArrayFlat( 1 );
    CleanupStack::PushL( array );
    User::LeaveIfError( plugin.GetContactStoreUris( *array ) );
    
    TEFLOGSTRING2( KTAINT, "CNT Extension Plugin > remoteParty: %S", &aRemotePartyName )
    TEFLOGSTRING3( KTAINT, "CNT Extension Plugin > parsedAddress: %S , digits: %d",
        &parsedAddress, digitsformatching )
    TEFLOGSTRING2( KTAINT, "CNT Extension Plugin > array count: %d", array->MdcaCount() )
    
    // Digits for matching. Supported range is between 4-20.
    // If not valid value use whole address length for matching.
    if ( digitsformatching != 0 && 
         ( digitsformatching < KMinValueOfMeaningfulDigits ||
           digitsformatching > KMaxValueOfMeaningfulDigits ) )
        {
        digitsformatching = parsedAddress.Length();
        }
		
    // Use service specific parameters for matching.
    TEFLOGSTRING3( KTAINT, "CNT MatchWithVoipAddressUsingExtensionL >\
        CPhCntMatcher::MatchVoipNumber, NUMBER: %S, CALL ID: %d",
        &remoteNumber, aCallId )
    iContactMatcher->MatchVoipNumber( aMatch, 
        parsedAddress.Left( Min( parsedAddress.Length(), KPEPhoneNumberMaxLength ) ), 
        allowUserNameMatch, array, digitsformatching );                                       
    
    CleanupStack::PopAndDestroy( array );
    CleanupStack::PopAndDestroy( &parsedAddress );
    }


// -----------------------------------------------------------
// CPEContactMatch::MatchingExtensionL
// -----------------------------------------------------------
//
CTelMatchingExtension& CPEContactMatch::MatchingExtensionL( TUint aServiceId )
    {
    TUid pluginUid = ResolveServicesImplementationUidL( aServiceId );
    
    CPEContactExtensionWrapper* wrapper = NULL;
    TInt pluginInd = 
        iPlugins.Find( pluginUid, CPEContactExtensionWrapper::MatchByUid );
    if ( KErrNotFound == pluginInd )
        {
        wrapper = CPEContactExtensionWrapper::NewLC( pluginUid );
        iPlugins.AppendL( wrapper );
        CleanupStack::Pop( wrapper );
        }
    else
        {
        wrapper = iPlugins[pluginInd];
        }
    
    __ASSERT_ALWAYS( NULL != wrapper, User::Leave( KErrNotFound ) );
    return *wrapper;
    }

//  End of File  
