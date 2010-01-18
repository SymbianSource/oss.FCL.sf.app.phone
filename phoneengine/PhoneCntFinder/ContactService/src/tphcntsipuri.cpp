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
* Description:  SIP URI descriptor
*
*/


#include "tphcntsipuri.h"

const TInt KSipPrefixLength = 4;
_LIT( KPhCntSipUriAt, "@" );
_LIT( KPhCntSipPrefix, "sip:" );
_LIT( KPhCntSipUriSeparatorBegin, "<" );
_LIT( KPhCntSipUriSeparatorEnd, ">" );


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
TPhCntSipURI::TPhCntSipURI( 
    const TDesC& aSipURI,
    TInt aDigitsForMatching ) : 
    iSipURI( aSipURI ),
    iDigitsForMatching( aDigitsForMatching )
    {
    RemoveUnusedChars();
    const TInt sipUriAtPos( iSipURIStripped.Find( KPhCntSipUriAt ) );
    if( sipUriAtPos > 0 ) 
        {
        iUserNamePart.Set( iSipURIStripped.Left( sipUriAtPos ) );
    	}
    else
        {
        iUserNamePart.Set( iSipURIStripped );
        }

    if ( iDigitsForMatching )
        {
        // Match is done with username part if iDigitsForMatching is given.
        // E.g. If username part contains valid cs number (sip:0401234567@domain.com)
        // and iDigitsForMatching is 7 match is done with string 1234567. 
        TInt length( 0 );
                                
        if( sipUriAtPos > 0 )
            {
            length = iUserNamePart.Length() > aDigitsForMatching ? 
                        aDigitsForMatching :
                        iUserNamePart.Length();
                                
            iFixedUserNamePart.Set( iUserNamePart.Right( length ) );
            }
        else
            {
            length = iSipURIStripped.Length() > aDigitsForMatching ? 
                        aDigitsForMatching :
                        iSipURIStripped.Length();
            
            iFixedUserNamePart.Set( iSipURIStripped.Right( length ) );
            }
        }
    
    }
    
// ---------------------------------------------------------------------------
// Gives the username part of sip uri.
// ---------------------------------------------------------------------------
//
const TDesC& TPhCntSipURI::UserNamePart() const
    {
    return iUserNamePart;
    }

// ---------------------------------------------------------------------------
// Gives the whole sip uri.
// ---------------------------------------------------------------------------
//    
const TDesC& TPhCntSipURI::SipURI() const
    {
    return iSipURIStripped;
    }
    
// ---------------------------------------------------------------------------
// Gives the fixed username part of sip uri.
// ---------------------------------------------------------------------------
//    
const TDesC& TPhCntSipURI::FixedUserNamePart() const
    {
    return iFixedUserNamePart;
    }

// ---------------------------------------------------------------------------
// Removes possible < and > marks and sip: prefix from sip uri.
// ---------------------------------------------------------------------------
//    
void TPhCntSipURI::RemoveUnusedChars()
    {
    TPtrC sipuri( iSipURI );
    
    // Remove begin mark
    const TInt startMarkPos( sipuri.Find( KPhCntSipUriSeparatorBegin ) );
    if( startMarkPos >= 0 ) 
        {
        // Extract the part after "<" mark ie take the part from end of 
        // the sip uri to "<" mark.
        sipuri.Set( sipuri.Right( iSipURI.Length() - ( startMarkPos + 1 ) ) );
        }
    
    // Remove end mark
    const TInt endMarkPos( sipuri.Find( KPhCntSipUriSeparatorEnd ) );
    if( endMarkPos >= 0 )
        {
        sipuri.Set( sipuri.Left( endMarkPos ) );
        }
        
    // "sip:" is not used when comparing addresses. 
    if ( sipuri.Length() > KSipPrefixLength )
        {
        TBuf<KSipPrefixLength> possibleSipPrefix;
        possibleSipPrefix.Copy( sipuri.Left( KSipPrefixLength ) );
        possibleSipPrefix.LowerCase();
               
        if ( possibleSipPrefix.Compare( KPhCntSipPrefix ) == KErrNone )
            {
            sipuri.Set( sipuri.Right( sipuri.Length() - KSipPrefixLength ) );
            }
        }
    
    iSipURIStripped.Set( sipuri );
    }

// End of File
