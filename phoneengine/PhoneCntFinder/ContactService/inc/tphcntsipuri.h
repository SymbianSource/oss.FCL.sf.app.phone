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


#ifndef TPHCNTSIPURI_H
#define TPHCNTSIPURI_H

#include <e32std.h>

/**
 *  SIP URI descriptor
 *
 *  @lib PhoneCntFinder
 *  @since S60 v3.1
 */
class TPhCntSipURI 
    {

public:

    /**
     * Constructor.
     *
     * @since S60 v3.1
     * @param aSipURI Sip URI.
     */
    TPhCntSipURI( 
        const TDesC& aSipURI,
        TInt aDigitsForMatching = 0 );

    /**
     * Gives the username part.
     *
     * @since S60 v3.1
     * @return Username part of SIP URI.
     */
    const TDesC& UserNamePart() const;

    /**
     * Gives the sip uri, where extra SIP chars has been removed.
     * Sip uri will be in format "username@domain"
     *
     * @since S60 v3.1
     * @return Sip uri
     */
    const TDesC& SipURI() const;
    
    /**
     * Gives the fixed username part.
     * User part is 
     *
     * @since S60 v3.1
     * @return Username part of SIP URI.
     */
    const TDesC& FixedUserNamePart() const;
    
protected:

    void RemoveUnusedChars();
     
private: // Data

    /** 
     * Descriptor containing sip uri.
     */
    const TDesC& iSipURI;
    
    /**
     * Sip uri, where not needed parts has been removed.
     */
    TPtrC iSipURIStripped;
    
    /**
     * Username part.
     */
    TPtrC iUserNamePart;
    
     /**
     * Fixed username part.
     */
    TPtrC iFixedUserNamePart;
    
     /**
     * Digits for matchig.
     */
    TInt iDigitsForMatching;
    
    };



#endif // TPHCNTSIPURI_H
