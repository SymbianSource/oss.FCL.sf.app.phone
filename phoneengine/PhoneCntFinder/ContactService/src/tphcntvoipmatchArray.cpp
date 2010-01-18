/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Array of voip matches.
*
*/


#include "tphcntvoipmatchArray.h"
#include "CPhCntContact.h"
#include "tphcntsipuri.h"
#include "tphcntcontactutil.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
TPhCntVoipMatchArray::TPhCntVoipMatchArray()
    {
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//    
TPhCntVoipMatchArray::~TPhCntVoipMatchArray()
    {
    iMatches.Reset();
    }

// ---------------------------------------------------------------------------
// Addes contact to array.
// ---------------------------------------------------------------------------
//
void TPhCntVoipMatchArray::AppendL( 
    CPhCntContact* aVoipMatch )
    {
    User::LeaveIfError( iMatches.Append( aVoipMatch ) );
    }

// ---------------------------------------------------------------------------
// Finds first match that has voip number.
// ---------------------------------------------------------------------------
//
CPhCntContact* TPhCntVoipMatchArray::FindFullMatch( 
    const TPhCntSipURI& aSipURI )
    {
    CPhCntContact* foundContact = NULL;
    CPhCntContact* contact = NULL;
    
    const TPhCntVoipMatchArray::TMatchType type = 
        FindFullOrUsernameMatch( aSipURI, contact );
        
    if( type == TPhCntVoipMatchArray::EFullMatch ) 
        {
        foundContact = contact;
        }
    RemoveMatchFromArray( foundContact );
    return foundContact;
    }

// ---------------------------------------------------------------------------
// Finds full or username match.
// ---------------------------------------------------------------------------
//    
CPhCntContact* TPhCntVoipMatchArray::FindFullOrUsernameMatch( 
    const TPhCntSipURI& aSipUri,
    const TInt aCharsForMatch )
    {
    CPhCntContact* contact = NULL;
    FindFullOrUsernameMatch( aSipUri, contact, aCharsForMatch );
    RemoveMatchFromArray( contact );
    return contact;
    }

// ---------------------------------------------------------------------------
// Destroys the items in array.
// ---------------------------------------------------------------------------
//   
void TPhCntVoipMatchArray::ReleaseMatches()
    {
    const TInt matchesCount( iMatches.Count() );
    for( TInt i = 0; i < matchesCount; i++ )
        {
        iMatches[i]->Release();
        }
    iMatches.Reset();
    }
 
// ---------------------------------------------------------------------------
// Removes match from array.
// ---------------------------------------------------------------------------
//   
void TPhCntVoipMatchArray::RemoveMatchFromArray( 
    const CPhCntContact* const aMatch )
    {
    const TInt index( iMatches.Find( aMatch ) );
    if( index >= 0 )
        {
        iMatches.Remove( index );    
        }
    iMatches.Compress();
    }

// ---------------------------------------------------------------------------
// Determines if contact has aSipURI voip number.
// ---------------------------------------------------------------------------
//    
TPhCntVoipMatchArray::TMatchType TPhCntVoipMatchArray::HasVoipNumber( 
    CPhCntContact& aContact,
    const TPhCntSipURI& aSipURI,
    const TInt aCharsForMatch ) const
    {
    const RArray<TPhCntNumber>& allNumbers = aContact.AllNumbers();
    const TInt numbers( allNumbers.Count() );
    TMatchType matchType = TPhCntVoipMatchArray::ENone;
    
    for( TInt i = 0; i < numbers; i++ ) 
        {
        TPhCntNumber number = allNumbers[i];
        if( number.Type() == CPhCntContact::EVoipNumber ||
            number.Type() == CPhCntContact::EMobileNumber || 
            number.Type() == CPhCntContact::EStandardNumber ||
            number.Type() == CPhCntContact::EPagerNumber ||
            number.Type() == CPhCntContact::EVideoNumber ||
            number.Type() == CPhCntContact::EAssistantNumber ||
            number.Type() == CPhCntContact::EFaxNumber ||
            number.Type() == CPhCntContact::ECarNumber
             ) 
            {
            // If uris are the same then we have a full match.
            TPhCntSipURI matchURI( number.Number(), aCharsForMatch );
			// full match if uris are the same (usernamepart requires
			// case sensitive match, domain is not sensitive)
            if ( matchURI.SipURI().CompareF( aSipURI.SipURI() ) 
                == KErrNone &&
                matchURI.UserNamePart().Compare( aSipURI.UserNamePart() )
                == KErrNone )
                {
                matchType = TPhCntVoipMatchArray::EFullMatch;
                aContact.SetMatchedVoipNumber( matchURI );
                break;
                }
            else if( matchURI.UserNamePart().Compare( aSipURI.UserNamePart() ) 
                == KErrNone ) 
                {
                matchType = TPhCntVoipMatchArray::EUserNameMatch;
                aContact.SetMatchedVoipNumber( matchURI );
                }
            else if( 0 != aCharsForMatch && matchURI.FixedUserNamePart().Compare( 
                     aSipURI.FixedUserNamePart() ) == KErrNone ) 
                {
                matchType = TPhCntVoipMatchArray::EUserNameMatch;
                aContact.SetMatchedVoipNumber( matchURI );
                }
            }
        }
    return matchType;
    }

// ---------------------------------------------------------------------------
// Finds contact that has aSipURI voip number
// ---------------------------------------------------------------------------
//    
TPhCntVoipMatchArray::TMatchType 
    TPhCntVoipMatchArray::FindFullOrUsernameMatch( 
        const TPhCntSipURI& aSipUri,
        CPhCntContact*& aFoundContact,
        const TInt aCharsForMatch )
    {
    CPhCntContact* userNameMatch = NULL;
    CPhCntContact* fullMatch = NULL;
    TBool duplicateMatch( EFalse );
    TMatchType matchType = TPhCntVoipMatchArray::ENone;
    // Go through all voip matches.
    const TInt matchesCount( iMatches.Count() );

    for( TInt i = 0; i < matchesCount; i++ )
        {
       
        CPhCntContact* possibleMatch = iMatches[i];

        matchType = HasVoipNumber( *possibleMatch, aSipUri, aCharsForMatch ) ;
        if( matchType == TPhCntVoipMatchArray::EFullMatch ) 
            {
            if ( fullMatch && 
                 !TPhCntContactUtil::AreFirstAndLastAndCompanyNameSame( 
                         *fullMatch, *possibleMatch ) )
                {
                // Duplicate match means no match found
                fullMatch = NULL;
                userNameMatch = NULL;
                break;
                }
            fullMatch = possibleMatch;
            }
        // Once username match is found it 
        // will be the match if fullmatch is not found.
        else if( matchType == TPhCntVoipMatchArray::EUserNameMatch &&
                 EFalse == duplicateMatch ) 
            {
            if ( userNameMatch )
                {
                // Duplicate username match.
                duplicateMatch = ETrue;
                }
            userNameMatch = possibleMatch;
            }
        }
    
    if( fullMatch )
        {
        aFoundContact = fullMatch;
        }
    else if ( duplicateMatch )
        {
        aFoundContact = NULL;
        }
    else
        {
        aFoundContact = userNameMatch;
        }
    return matchType;
    }

// End of File
