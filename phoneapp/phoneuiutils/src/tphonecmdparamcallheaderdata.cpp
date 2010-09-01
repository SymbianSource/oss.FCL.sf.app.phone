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
* Description: Implementation of TPhoneCmdParamCallHeaderData class.
*
*/


// INCLUDE FILES
#include    <fbs.h>
#include "tphonecmdparamcallheaderdata.h"


// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// TPhoneCmdParamCallHeaderData::TPhoneCmdParamCallHeaderData
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
EXPORT_C TPhoneCmdParamCallHeaderData::TPhoneCmdParamCallHeaderData() :
    TPhoneCommandParam(),
    iLabelText( KNullDesC ),
    iShortLabelText( KNullDesC ),
    iCallState( 0 ),
    iNumberType( 0 ),
    iCiphering( EFalse ),
    iCLIText( KNullDesC ),
    iCNAPText( KNullDesC ),
    iPicture( KNullDesC ),
    iCallFlag( CBubbleManager::ENormal ),
    iLine2 ( EFalse ),
    iThumbnail( NULL ),
    iCallerText( KNullDesC ),
    iParticipantListCLI( EPhoneParticipantCLIText ),
    iHasThumbnail( EFalse ),
    iCipheringIndicatorAllowed( ETrue ),
    iContactLink( KNullDesC8 ),
    iRemotePhoneNumber( KNullDesC )
    {
    iParamId = EPhoneParamIdCallHeaderData;
    }

// ---------------------------------------------------------
// TPhoneCmdParamCallHeaderData::SetLabelText
// Sets the call header label text
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void TPhoneCmdParamCallHeaderData::SetLabelText( 
   const TDesC& aLabelText )
	{
	iLabelText = aLabelText;
	}

// ---------------------------------------------------------
// TPhoneCmdParamCallHeaderData::SetShortLabelText
// Sets the call header short label text
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void TPhoneCmdParamCallHeaderData::SetShortLabelText( 
   const TDesC& aShortLabelText )
    {
	iShortLabelText = aShortLabelText;
    }

// ---------------------------------------------------------
// TPhoneCmdParamCallHeaderData::SetCallState
// Sets the call header call state
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void TPhoneCmdParamCallHeaderData::SetCallState( TInt aCallState )
   {
   iCallState = aCallState;
   }

// ---------------------------------------------------------
// TPhoneCmdParamCallHeaderData::SetCallType
// Sets the call header call type
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void TPhoneCmdParamCallHeaderData::SetCallType( TInt aCallType )
   {
   iCallType = aCallType;
   }

// ---------------------------------------------------------
// TPhoneCmdParamCallHeaderData::SetNumberType
// Sets the call header number type 
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void TPhoneCmdParamCallHeaderData::SetNumberType( TInt aNumberType )
   {
   iNumberType = aNumberType;
   }

// ---------------------------------------------------------
// TPhoneCmdParamCallHeaderData::SetCiphering
// Sets the call header ciphering status
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void TPhoneCmdParamCallHeaderData::SetCiphering( 
   TBool aCiphering )
   {
   iCiphering = aCiphering;
   }

// ---------------------------------------------------------
// TPhoneCmdParamCallHeaderData::SetCLIText
// Sets the call header CLI text
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void TPhoneCmdParamCallHeaderData::SetCLIText( 
   const TDesC& aCLIText,
   CBubbleManager::TPhoneClippingDirection aDirection )
   {
   iCLIText = aCLIText;
   iCLITextClippingDirection = aDirection; 
   }

// ---------------------------------------------------------
// TPhoneCmdParamCallHeaderData::SetCNAPText
// Sets the call header CNAP text
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void TPhoneCmdParamCallHeaderData::SetCNAPText( 
   const TDesC& aCNAPText,
   CBubbleManager::TPhoneClippingDirection aDirection )
   {
   iCNAPText = aCNAPText;
   iCNAPTextClippingDirection = aDirection; 
   }

// ---------------------------------------------------------
// TPhoneCmdParamCallHeaderData::SetPicture
// Sets the call header picture
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void TPhoneCmdParamCallHeaderData::SetPicture( 
   const TDesC& aPicture )
   {
   iPicture.Set( aPicture );
   }

// ---------------------------------------------------------
// TPhoneCmdParamCallHeaderData::SetCallFlag
// Sets the call flag
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void TPhoneCmdParamCallHeaderData::SetCallFlag(
    const CBubbleManager::TPhoneCallTypeFlags aCallFlag )
    {
    iCallFlag = aCallFlag;        
    }

// ---------------------------------------------------------
// TPhoneCmdParamCallHeaderData::AddCallFlag
// Adds the call flag
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void TPhoneCmdParamCallHeaderData::AddCallFlag(
    const CBubbleManager::TPhoneCallTypeFlags aCallFlag )
    {
    iCallFlag |= aCallFlag;
    }

// ---------------------------------------------------------
// TPhoneCmdParamCallHeaderData::SetLine2
// ---------------------------------------------------------
//	
EXPORT_C void TPhoneCmdParamCallHeaderData::SetLine2(
    TBool aLine2 )
   {
   iLine2 = aLine2;
   }
    
// ---------------------------------------------------------
// TPhoneCmdParamCallHeaderData::LabelText
// Returns the call header label text
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C const TDesC& TPhoneCmdParamCallHeaderData::LabelText() const
   {
   return iLabelText;
   }

// ---------------------------------------------------------
// TPhoneCmdParamCallHeaderData::ShortLabelText
// Returns the call header short label text
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C const TDesC& TPhoneCmdParamCallHeaderData::ShortLabelText() const
   {
   return iShortLabelText;
   }

// ---------------------------------------------------------
// TPhoneCmdParamCallHeaderData::CallState
// Returns the call header call state
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TInt TPhoneCmdParamCallHeaderData::CallState() const
   {
   return iCallState;
   }

// ---------------------------------------------------------
// TPhoneCmdParamCallHeaderData::CallType
// Returns the call header call type
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TInt TPhoneCmdParamCallHeaderData::CallType() const
   {
   return iCallType;
   }

// ---------------------------------------------------------
// TPhoneCmdParamCallHeaderData::NumberType
// Returns the call header number type
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TInt TPhoneCmdParamCallHeaderData::NumberType() const
   {
   return iNumberType;
   }

// ---------------------------------------------------------
// TPhoneCmdParamCallHeaderData::Ciphering
// Returns the call header ciphering
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TBool TPhoneCmdParamCallHeaderData::Ciphering() const
   {
   return iCiphering;
   }

// ---------------------------------------------------------
// TPhoneCmdParamCallHeaderData::CLIText
// Returns the call header CLI text
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C const TDesC& TPhoneCmdParamCallHeaderData::CLIText() const
   {
   return iCLIText;
   }

// ---------------------------------------------------------
// TPhoneCmdParamCallHeaderData::CLITextClippingDirection
// Returns the clipping direction of the CLI text.
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C CBubbleManager::TPhoneClippingDirection 
TPhoneCmdParamCallHeaderData::CLITextClippingDirection() const
    {
    return iCLITextClippingDirection;
    }

// ---------------------------------------------------------
// TPhoneCmdParamCallHeaderData::CNAPTextClippingDirection
// Returns the clipping direction of the CNAP text.
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C CBubbleManager::TPhoneClippingDirection 
TPhoneCmdParamCallHeaderData::CNAPTextClippingDirection() const
    {
    return iCNAPTextClippingDirection;
    }

// ---------------------------------------------------------
// TPhoneCmdParamCallHeaderData::CNAPText
// Returns the call header CNAP text
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C const TDesC& TPhoneCmdParamCallHeaderData::CNAPText() const
   {
   return iCNAPText;
   }

// ---------------------------------------------------------
// TPhoneCmdParamCallHeaderData::Picture
// Returns the call header picture
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C const TDesC& TPhoneCmdParamCallHeaderData::Picture() const
   {
   return iPicture;
   }

// ---------------------------------------------------------
// TPhoneCmdParamCallHeaderData::CallFlag
// Returns the call flag
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TUint32
    TPhoneCmdParamCallHeaderData::CallFlag() const
    {
    return iCallFlag;        
    }

// ---------------------------------------------------------
// TPhoneCmdParamCallHeaderData::Line2
// Returns the call header line2
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TBool TPhoneCmdParamCallHeaderData::Line2() const
   {
   return iLine2;
   }
   
// ---------------------------------------------------------
// TPhoneCmdParamCallHeaderData::Thumbnail
// Returns the pointer to thumbnail image
// ---------------------------------------------------------
//
EXPORT_C CFbsBitmap* TPhoneCmdParamCallHeaderData::Thumbnail() const
   {
   return iThumbnail;
   }

// ---------------------------------------------------------
// TPhoneCmdParamCallHeaderData::SetThumbnail
// Sets the pointer to thumbnail image
// ---------------------------------------------------------
//
EXPORT_C void TPhoneCmdParamCallHeaderData::SetThumbnail( 
   CFbsBitmap* aThumbnail )
   {
   iThumbnail = aThumbnail;
   }

// ---------------------------------------------------------
// TPhoneCmdParamCallHeaderData::CallerText
// Returns the Caller text
// ---------------------------------------------------------
//
EXPORT_C const TDesC& TPhoneCmdParamCallHeaderData::CallerText() const
   {
   return iCallerText;
   }

// ---------------------------------------------------------
// TPhoneCmdParamCallHeaderData::CallerText
// Sets the Caller text
// ---------------------------------------------------------
//
EXPORT_C void TPhoneCmdParamCallHeaderData::SetCallerText( 
   const TDesC& aCallerText )
   {
   iCallerText.Set( aCallerText );
   }
   
// ---------------------------------------------------------
// TPhoneCmdParamCallHeaderData::SetParticipantCLI
// Sets the Caller text
// ---------------------------------------------------------
//
EXPORT_C void TPhoneCmdParamCallHeaderData::SetParticipantListCLI(
    TPhoneParticipantListCLI aCLIType )
    {
    iParticipantListCLI = aCLIType;
    }

// ---------------------------------------------------------
// TPhoneCmdParamCallHeaderData::ParticipantCLI
// Sets the Caller text
// ---------------------------------------------------------
//
EXPORT_C TPhoneCmdParamCallHeaderData::TPhoneParticipantListCLI 
    TPhoneCmdParamCallHeaderData::ParticipantCLI() const
    {
    return iParticipantListCLI;    
    }       

// ---------------------------------------------------------
// TPhoneCmdParamCallHeaderData::HasThumbnail
// ---------------------------------------------------------
//
EXPORT_C TBool TPhoneCmdParamCallHeaderData::HasThumbnail() const
    {
    return iHasThumbnail;
    }    
// ---------------------------------------------------------
// TPhoneCmdParamCallHeaderData::SetHasThumbnail
// ---------------------------------------------------------
//
EXPORT_C void TPhoneCmdParamCallHeaderData::SetHasThumbnail( TBool aStatus )
    {
    iHasThumbnail = aStatus;
    }
   
// ---------------------------------------------------------
// TPhoneCmdParamCallHeaderData::SetCipheringIndicatorAllowed
// Sets the ciphering indicator allowed status
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void TPhoneCmdParamCallHeaderData::SetCipheringIndicatorAllowed( 
    TBool aAllowed )
    {
    iCipheringIndicatorAllowed = aAllowed;
    }

// ---------------------------------------------------------
// TPhoneCmdParamCallHeaderData::CipheringIndicatorAllowed
// Returns the ciphering indicator allowed status
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TBool TPhoneCmdParamCallHeaderData::CipheringIndicatorAllowed() const    
    {
    return iCipheringIndicatorAllowed;
    }

// ---------------------------------------------------------
// TPhoneCmdParamCallHeaderData::SetServiceId
// ---------------------------------------------------------
//
EXPORT_C void TPhoneCmdParamCallHeaderData::SetServiceId( TUint32 aServiceId )
    {
    iServiceId = aServiceId;
    }

// ---------------------------------------------------------
// TPhoneCmdParamCallHeaderData::ServiceId
// ---------------------------------------------------------
//
EXPORT_C TUint32 TPhoneCmdParamCallHeaderData::ServiceId() const
    {
    return iServiceId;
    }

// ---------------------------------------------------------
// TPhoneCmdParamCallHeaderData::SetContactLink
// ---------------------------------------------------------
//
EXPORT_C void TPhoneCmdParamCallHeaderData::SetContactLink( 
    const TDesC8& aContactLink )
    {
    iContactLink.Set( aContactLink );
    }

// ---------------------------------------------------------
// TPhoneCmdParamCallHeaderData::ContactLink
// ---------------------------------------------------------
//
EXPORT_C const TDesC8& TPhoneCmdParamCallHeaderData::ContactLink() const
    {
    return iContactLink;
    }

// ---------------------------------------------------------
// TPhoneCmdParamCallHeaderData::SetRemotePhoneNumber
// ---------------------------------------------------------
//
EXPORT_C void TPhoneCmdParamCallHeaderData::SetRemotePhoneNumber( 
    const TDesC& aPhoneNumber )
    {
    iRemotePhoneNumber.Set( aPhoneNumber );
    }

// ---------------------------------------------------------
// TPhoneCmdParamCallHeaderData::RemotePhoneNumber
// ---------------------------------------------------------
//
EXPORT_C const TDesC& TPhoneCmdParamCallHeaderData::RemotePhoneNumber() const
    {
    return iRemotePhoneNumber;
    }

//  End of File
