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


#ifndef TPHCNTTXTTOSPEECH_H
#define TPHCNTTXTTOSPEECH_H

#include <e32std.h>

/**
 *  Constructs text to speech descriptor.
 *  Converts data from contact to text to speech descriptor.
 *
 *  @lib PhoneCntFinder
 *  @since S60 v3.1
 */
class TPhCntTxtToSpeech 
    {
public:

    /**
     * Constructor.
     *
     * @since S60 v3.1
     * @param aFirstName First name from contact.
     * @param aSecondName aSecondName from contact.
     * @param aLastName Last name from contact.
     * @param aCompanyName Company name from contact.
     * @param aFirstNamePronunciation First name pronunciation from contact.
     * @param aLastNamePronunciation Last name pronunciation from contact.
     * @param aCompanyNamePronunciation Company name pronunciation from contact.
     * @param aCurrentLanguage Current language in use.
     */
    TPhCntTxtToSpeech( 
        const TDesC& aFirstName,
        const TDesC& aSecondName,
        const TDesC& aLastName,
        const TDesC& aCompanyName,
        const TDesC& aFirstNamePronunciation,
        const TDesC& aLastNamePronunciation,
        const TDesC& aCompanyNamePronunciation,
        TLanguage aCurrentLanguage
    );

    /**
     * Constructs txt to speech string from contact field data, 
     * given in constructor.
     *
     * @since S60 v3.1
     * @param aTxtToSpeech Creates instance of txt to speech. May be null.
     */
     void GetTextToSpeechL( HBufC*& aTxtToSpeech ) const;

private:

    HBufC* AllocAndTrimL( const TDesC& aText ) const;
    
    TBool SwapNameOrder() const;
    
    HBufC* GetTrimmedNameLC( 
        const TDesC& aWritenName, 
        const TDesC& aNamePronunciation ) const;
        
    TInt CalculateTxtToSpeechLength( const HBufC* const aComponent, const HBufC* const aComponent2 ) const;

private: // data

    /** 
     * Contact field data.
     * Not own.
     */
    const TDesC& iFirstName;
    const TDesC& iSecondName;
    const TDesC& iLastName;
    const TDesC& iCompanyName;
    const TDesC& iFirstNamePronunciation;
    const TDesC& iLastNamePronunciation;
    const TDesC& iCompanyNamePronunciation;
    TLanguage iCurrentLanguage;
    };

#endif // TPHCNTTXTTOSPEECH_H
