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


#ifndef TPHCNTVOIPMATCHARRAY_H
#define TPHCNTVOIPMATCHARRAY_H

#include <e32std.h>

class CPhCntContact;
class TPhCntSipURI;

/**
 *  Array of voip matches
 *
 *  Offers way to figure out correct match from many matches.
 *
 *  @lib PhoneCntFinder
 *  @since S60 v3.1
 */
class TPhCntVoipMatchArray 
    {
public:
    TPhCntVoipMatchArray();
    
    ~TPhCntVoipMatchArray();
    
    /**
     * Adds contact, takes ownership if the contact.
     *
     * @since S60 v3.1
     * @param aVoipMatch Voip match.
     */
    void AppendL( CPhCntContact* aVoipMatch );

    /**
     * Finds first matched contact, which number
     * is matches aSipNumber. If no such match is found, null is
     * returned. Note that the found match is not in this
     * array after this call. Ownership of match is given
     * to client.
     *
     * @since S60 v3.1
     * @return Contact, which voip number is aSipURI
     */
    CPhCntContact* FindFullMatch( const TPhCntSipURI& aSipURI );
    
    /**
     * Finds full or just username match from contacts added to this array.
     * If no full matches are found then possible username match is given.
     * Note that the found match is not in this array after this call. 
     * Ownership of match is given to client.
     * 
     * @since S60 v3.1
     * @param aSipUri Sip URI that the match is compared against.
     * @return Found match. Null is returned if no match is found.
     */
    CPhCntContact* FindFullOrUsernameMatch( 
        const TPhCntSipURI& aSipUri,
        const TInt aCharsForMatch = 0 );
    
    /**
     * Releases the matches. Needs to be called before this class goes 
     * out of scope.
     * @since S60 
     */
    void ReleaseMatches();
    
private:
    
    /**
     * Removes contact from array of iMatches. If aMatch does not
     * exist in iMatches then nothing is done.
     * 
     * @since S60 v3.1
     * @param aContact Contact to be removed from iMatches.
     */
    void RemoveMatchFromArray( const CPhCntContact* const aContact );    

    /**
     * Match type.
     */
    enum TMatchType 
        {
        ENone,
        EFullMatch,
        EUserNameMatch,
        };
         
    /**
     * Determines if contact has aSipURI voip number.
     *
     * @since S60 v3.2
     * @param aContact Contact, which is checked.
     * @param aSipURI SIP URI
     * @return ENone Contact does not have aSipURI voip number.
     *         EFullMatch Contact has aSipUri voip number.
     *         EUserNameMatch Contact has voip number which user name part
     *         equals to aSupURI's user name part.
     */
    TMatchType HasVoipNumber( 
        CPhCntContact& aContact,
        const TPhCntSipURI& aSipURI,
        const TInt aCharsForMatch = 0 ) const;
    
    /**
     * Finds contact, which voip number is aSipUri or username part
     * are same.
     * 
     * @since S60 v3.2
     * @param aSipURI Voip number of wanted contact.
     * @param aContact Contact found or null.
     * @return ENone - No contact found.
     *         EFullMatch Contact found with full match.
     *         EUserNameMatch contact found that has same user name part
     *         than aSipURI.
     */
    TMatchType FindFullOrUsernameMatch( 
        const TPhCntSipURI& aSipUri,
        CPhCntContact*& aFoundContact,
        const TInt aCharsForMatch = 0 );
      
    
private: // data

    /**
     * Array of contact matches.
     * Own.
     */
    RPointerArray<CPhCntContact> iMatches;

    };

#endif // TPHCNTVOIPMATCHARRAY_H
