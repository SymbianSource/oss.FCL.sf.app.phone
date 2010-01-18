/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Contains call data.
*
*/


#include "phonebubbleextensiondata.h"
#include "tphonecmdparamcallheaderdata.h"
#include "pevirtualengine.h"

// --------------------------------------------------------------------------
// Two-phased constructor.
// --------------------------------------------------------------------------
//
CPhoneBubbleExtensionData* CPhoneBubbleExtensionData::NewL(
    TInt aBubbleId,
    TPhoneCmdParamCallHeaderData* aParams, 
    TBool aFirst )
    {
    CPhoneBubbleExtensionData* self =
        CPhoneBubbleExtensionData::NewLC( aBubbleId, aParams, aFirst );
    CleanupStack::Pop( self );
    return self;
    }

// --------------------------------------------------------------------------
// Two-phased constructor.
// --------------------------------------------------------------------------
//
CPhoneBubbleExtensionData* CPhoneBubbleExtensionData::NewLC(
    TInt aBubbleId,
    TPhoneCmdParamCallHeaderData* aParams, 
    TBool aFirst )
    {
    CPhoneBubbleExtensionData* self =
        new( ELeave ) CPhoneBubbleExtensionData( aBubbleId, aParams, aFirst );
    CleanupStack::PushL( self );
    self->ConstructL( aParams );
    return self;
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CPhoneBubbleExtensionData::~CPhoneBubbleExtensionData()
    {
    delete iContactLink;
    delete iRemotePhoneNumber;
    }

// ---------------------------------------------------------------------------
// Sets the call state
// ---------------------------------------------------------------------------
//
void CPhoneBubbleExtensionData::SetState( TInt aState, TBool aFirst )
    {
    switch ( aState )
        {
        case EPEStateDialing:
            iState = MTelBubbleExtensionData::EOutgoing;
            break;

        case EPEStateRinging:
            if ( aFirst )
                {
                iState = MTelBubbleExtensionData::EIncoming;
                }
            else
                {
                iState = MTelBubbleExtensionData::EWaiting;
                }
            break;

        case EPEStateConnecting:
            iState = MTelBubbleExtensionData::EAlerting;
            break;

        case EPEStateHeld: // fall-through
        case EPEStateHeldConference:
            iState = MTelBubbleExtensionData::EOnHold;
            break;

        case EPEStateDisconnecting: // fall-through
        case EPEStateConferenceIdle: // fall-through
        case EPEStateIdle:
            iState = MTelBubbleExtensionData::EDisconnected;
            break;

        case EPEStateConnected: // fall-through
        case EPEStateConnectedConference:
            iState = MTelBubbleExtensionData::EActive;
            break;

        case EPEStateUnknown: // fall-through
        default:
            break;
        }
    
    }

// ---------------------------------------------------------------------------
// From class MTelBubbleExtensionData.
// Bubble ID.
// ---------------------------------------------------------------------------
//
inline TInt CPhoneBubbleExtensionData::BubbleId() const
    {
    return iBubbleId;
    }

// --------------------------------------------------------------------------
// From class MTelBubbleExtensionData.
// Call state information.
// --------------------------------------------------------------------------
//
inline CPhoneBubbleExtensionData::TCallState
    CPhoneBubbleExtensionData::State() const
    {
    return iState;
    }

// ---------------------------------------------------------------------------
// From class MTelBubbleExtensionData.
// Call type information
// ---------------------------------------------------------------------------
//
inline CPhoneBubbleExtensionData::TCallType
    CPhoneBubbleExtensionData::Type() const
    {
    return iType;
    }

// ---------------------------------------------------------------------------
// From class MTelBubbleExtensionData.
// Call service id
// ---------------------------------------------------------------------------
//
inline TUint32 CPhoneBubbleExtensionData::ServiceId() const
    {
    return iService;
    }

// ---------------------------------------------------------------------------
// From class MTelBubbleExtensionData.
// Contact Link related to this call.
// ---------------------------------------------------------------------------
//
inline const TDesC8& CPhoneBubbleExtensionData::ContactLink() const
    {
    return *iContactLink;
    }

// ---------------------------------------------------------------------------
// From class MTelBubbleExtensionData.
// Remote phone number related to this call.
// ---------------------------------------------------------------------------
//
const TDesC& CPhoneBubbleExtensionData::RemotePhoneNumber() const
    {
    return *iRemotePhoneNumber;
    }

// ---------------------------------------------------------------------------
// First phase constructor
// ---------------------------------------------------------------------------
//
CPhoneBubbleExtensionData::CPhoneBubbleExtensionData( 
    TInt aBubbleId,
    TPhoneCmdParamCallHeaderData* aParams, 
    TBool aFirst )
    : iBubbleId(aBubbleId), iState(ENone), iType(EUninitialized), iService(0)
    {
    // call state
    SetState( aParams->CallState(), aFirst );

    // call type
    switch ( aParams->CallType() )
        {
        case EPECallTypeCSVoice:
            iType = MTelBubbleExtensionData::ECsVoice;
            break;

        case EPECallTypeVideo:
            iType = MTelBubbleExtensionData::ECsVideo;
            break;
        
        case EPECallTypeVoIP:
            iType = MTelBubbleExtensionData::EPsVoice;
            break;
        
        case EPECallTypeUninitialized:
        default:
            iType = MTelBubbleExtensionData::EUninitialized;
            break;
        }
    
    // service ID
    iService = aParams->ServiceId();
    }


// ---------------------------------------------------------------------------
// Second phase constructor
// ---------------------------------------------------------------------------
//
void CPhoneBubbleExtensionData::ConstructL( 
    TPhoneCmdParamCallHeaderData* aParams )
    {
    // Copy contact link to local buffer
    iContactLink = HBufC8::NewL( aParams->ContactLink().Length() );
    *iContactLink = aParams->ContactLink();
    
    // Copy phone number to local buffer
    iRemotePhoneNumber = aParams->RemotePhoneNumber().AllocL();
    }


