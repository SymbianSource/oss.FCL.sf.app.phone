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
* Description:  Constructs text to speech descriptor.
*
*/


#include "tphcnttxttospeech.h"
#include "mphcntcontactfields.h"

// Separator between lastname/firstname for TTS text.
_LIT( KPhCntTtsNamesDelimeter, " " );

// Number of languages that use lastname-firstname order
const TInt KPhCntTtsNumberOfSwappedLanguages = 7;

// Languages which use lastname-firstname order
const TLanguage KPhCntTtsSwappedLanguages[KPhCntTtsNumberOfSwappedLanguages] = 
    {
    ELangHungarian,       // 17
    ELangTaiwanChinese,   // 29
    ELangHongKongChinese, // 30
    ELangPrcChinese,      // 31
    ELangJapanese,		  // 32
    ELangKorean,          // 65
    ELangVietnamese       // 96
    };

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
TPhCntTxtToSpeech::TPhCntTxtToSpeech(
    const TDesC& aFirstName,
    const TDesC& aSecondName,
    const TDesC& aLastName,
    const TDesC& aCompanyName,
    const TDesC& aFirstNamePronunciation,
    const TDesC& aLastNamePronunciation,
    const TDesC& aCompanyNamePronunciation,
    TLanguage aCurrentLanguage ) :
    iFirstName( aFirstName ),
    iSecondName( aSecondName ),
    iLastName( aLastName ),
    iCompanyName( aCompanyName ),
    iFirstNamePronunciation( aFirstNamePronunciation ),
    iLastNamePronunciation( aLastNamePronunciation ),
    iCompanyNamePronunciation( aCompanyNamePronunciation ),
    iCurrentLanguage( aCurrentLanguage )
    {
    }

// ---------------------------------------------------------------------------
// Constructs text to speech descriptor.
// ---------------------------------------------------------------------------
//
void TPhCntTxtToSpeech::GetTextToSpeechL( HBufC*& aTxtToSpeech ) const
    {
    aTxtToSpeech = AllocAndTrimL( iSecondName );
    
    // Try contact name (first & last name).
    if ( !aTxtToSpeech )
        {
        HBufC* trimmedFirstName = GetTrimmedNameLC( 
            iFirstName, iFirstNamePronunciation );
            
        HBufC* trimmedLastName = GetTrimmedNameLC( 
            iLastName, iLastNamePronunciation );

        // Determine order.
        const TBool swapNameOrder = SwapNameOrder();

        HBufC* firstComponent = trimmedFirstName;
        HBufC* lastComponent = trimmedLastName;
        if ( swapNameOrder )
            {
            firstComponent = trimmedLastName;
            lastComponent = trimmedFirstName;
            }

        // Calculate length.
        const TInt totalLength( 
            CalculateTxtToSpeechLength( firstComponent, lastComponent ) );
        
        // Compose.
        if ( totalLength )
            {
            aTxtToSpeech = HBufC::NewL( totalLength );
            TPtr ptr = aTxtToSpeech->Des();

            if ( firstComponent )
                {
                ptr.Append( *firstComponent );
                if ( lastComponent )
                    {
                    ptr.Append( KPhCntTtsNamesDelimeter );
                    }
                }
            if ( lastComponent )
                {
                ptr.Append( *lastComponent );
                }
            }

        CleanupStack::PopAndDestroy( trimmedLastName );
        CleanupStack::PopAndDestroy( trimmedFirstName );
        }

    // Try company name.
    if ( !aTxtToSpeech )
        {
        aTxtToSpeech = AllocAndTrimL( iCompanyNamePronunciation );
        if( !aTxtToSpeech ) 
            {
            aTxtToSpeech = AllocAndTrimL( iCompanyName );
            }
        }
    }

// ---------------------------------------------------------------------------
// Copies the text and trims it.
// ---------------------------------------------------------------------------
//    
HBufC* TPhCntTxtToSpeech::AllocAndTrimL( const TDesC& aText ) const
    {
    HBufC* result = NULL;

    if( aText.Length() > 0 )
        {
        result = aText.AllocL();
        TPtr ptr = result->Des();
        ptr.Trim();
        if ( !ptr.Length() )
            {
            delete result;
            result = NULL;
            }
        }
        
    return result;
    }
    
// -----------------------------------------------------------------------------
// Swaps name order depending on current language.
// -----------------------------------------------------------------------------
//
TBool TPhCntTxtToSpeech::SwapNameOrder() const
    {
    TBool result = EFalse;
    for ( TInt index = 0; index < KPhCntTtsNumberOfSwappedLanguages; index++ )
        {
        if ( KPhCntTtsSwappedLanguages[ index ] == iCurrentLanguage )
            {
            result = ETrue;
            index = KPhCntTtsNumberOfSwappedLanguages;
            }
        }

    return result;
    }
    
// -----------------------------------------------------------------------------
// Takes suitable name, copies and trims it.
// -----------------------------------------------------------------------------
//  
HBufC* TPhCntTxtToSpeech::GetTrimmedNameLC( 
    const TDesC& aWritenName, 
    const TDesC& aNamePronunciation ) const
    {
    HBufC* trimmedName = AllocAndTrimL( aNamePronunciation );
    if ( !trimmedName )
        {
        trimmedName = AllocAndTrimL( aWritenName );
        }
    CleanupStack::PushL( trimmedName );
    return trimmedName;
    }

// -----------------------------------------------------------------------------
// Calculates the length of text to speech string.
// -----------------------------------------------------------------------------
//
TInt TPhCntTxtToSpeech::CalculateTxtToSpeechLength( 
    const HBufC* const aFirstComponent, 
    const HBufC* const aLastComponent ) const
    {
    TInt totalLength = 0;
    if( aFirstComponent && aLastComponent ) 
        {
        totalLength += KPhCntTtsNamesDelimeter().Length();
        }
    
    if( aFirstComponent )
        {
        totalLength += aFirstComponent->Length();
        }
        
    if( aLastComponent )
        {
        totalLength += aLastComponent->Length();
        }
        
    return totalLength;
    }

//  End of File 
