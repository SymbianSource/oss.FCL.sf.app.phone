/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This class extracts phone number for comparsion.
*
*/


//  INCLUDES
#include "CPhoneRawMatchNumberExtractor.h"
#include "CPhoneCntNumberParser.h"
#include <cntdb.h>

#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <cntphonenumparser.h>
#endif

// CONSTANTS

// DEPRECATED
const TUint KPhCntExtractorExtraChar = '1'; // number makes no difference.

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CCntRawPhoneNumberExtractor::CCntRawPhoneNumberExtractor
// C++ constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
EXPORT_C CCntRawPhoneNumberExtractor::CCntRawPhoneNumberExtractor() 
: iKPhoneNumberExtraChar( KPhCntExtractorExtraChar ) 
    {
    }

// Destructor
CCntRawPhoneNumberExtractor::~CCntRawPhoneNumberExtractor()
    {
    delete iNumberParser;
    }

// -----------------------------------------------------------------------------
// CCntRawPhoneNumberExtractor::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CCntRawPhoneNumberExtractor::ConstructL()
    {
    iNumberParser = CCntNumberParser::NewL();
    iParser = iNumberParser->CntPhoneNumberParser();
    }


// -----------------------------------------------------------------------------
// CCntRawPhoneNumberExtractor::Release
// 
//  Releases used memory
// -----------------------------------------------------------------------------
//
void CCntRawPhoneNumberExtractor::Release()
    {
    delete this;
    }


// -----------------------------------------------------------------------------
// CCntRawPhoneNumberExtractor::ExtractRawNumber
// 
//  DEPRECATED. 
// -----------------------------------------------------------------------------
//
TUint CCntRawPhoneNumberExtractor::ExtractRawNumber(
    const TDesC& /*aTextualNumber*/)
    {
    __ASSERT_DEBUG( EFalse , User::Invariant() );
    return 0;
    }


// -----------------------------------------------------------------------------
// CCntRawPhoneNumberExtractor::ExtractRawNumber
// 
//  Does the actual extracting
// -----------------------------------------------------------------------------
//
TUint CCntRawPhoneNumberExtractor::ExtractRawNumber(
    const TDesC& aTextualNumber, 
    TDes& aRawNumber)
    {
    aRawNumber.Zero();
    iParser->ExtractRawNumber( aTextualNumber , iMatchBuffer );
    if ( iMatchBuffer.Length() == 0 )
        {
        return 0;
        }

    if ( iMatchBuffer.Length() > aRawNumber.MaxLength() )
        {
        aRawNumber = iMatchBuffer.Right( aRawNumber.MaxLength() );
        }
    else
        {
        aRawNumber = iMatchBuffer;
        }
    return 0;
    }

// -----------------------------------------------------------------------------
// CCntRawPhoneNumberExtractor::ExtraChar() const
// 
//  DEPRECATED.
// -----------------------------------------------------------------------------
//
TChar CCntRawPhoneNumberExtractor::ExtraChar() const
    {
    __ASSERT_DEBUG( EFalse , User::Invariant() );
    return iKPhoneNumberExtraChar;
    }   
    
// End of file
