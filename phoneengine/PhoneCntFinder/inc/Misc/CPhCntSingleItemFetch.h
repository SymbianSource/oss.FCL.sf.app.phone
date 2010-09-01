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
* Description:  Interface to make single item fetches.
*
*/


#ifndef CPHCNTSINGLEITEMFETCH_H
#define CPHCNTSINGLEITEMFETCH_H

// INCLUDES
#include    <e32base.h>
#include    <cntdef.h>
#include <mphcntmatch.h>

class MVPbkContactLink;
class CPhCntContactId;

class CPhCntSelection : public CBase
    {
public:
        
    /**
     * @since s60 v3.2
     * @return Number selected by user.
     */
    virtual const TDesC& Number() const = 0;
    
    /**
     * @since s60 v3.2
     * @return Contact identifier. Identifies the contact
     * where user selected the number.
     */
    virtual const CPhCntContactId& ContactId() const = 0;
    
    };
        
// CLASS DECLARATION

/**
*  It provides interface to make phone app single item fetches.
*
*  @lib PhoneCntFinder
*  @since 1.0
*/
class CPhCntSingleItemFetch
    : public CBase
    {
    public: // New functions       

        /**
        * It enumerates different types of fetches.
        *
        * EFetchDtmf          - fetch from dtmf
        * EFetchNewCall       - fetch from new call query
        * EFetchNewPSCall     - fetch from new PS call query
        */
        enum TFetchType
            {
            EFetchDtmf,
            EFetchNewCall,
            EFetchNewPSCall
            };

		/**
		* Phone number filtering for the FetchPhoneNumberLD implementation
		* 
		* ECallPhoneNumber		- CS
        * ECallVoip,          	- PS
        * ECallVideoNumber,		- Video 
		*/
		enum TCallType
            {
            ECallPhoneNumber, 
            ECallVoip, 
            ECallVideoNumber 
            };

        /**
        * It defines structure for result.
        */
        struct TFetchParams
            {
            public: // In

                // Fetch type.
                TFetchType iType;

                // Resource for softkeys.
                TInt iCbaResource;

            public:

                // Buffer for string.
                TDes* iString;

                MPhCntMatch::TNumberType iNumberType;
                
                CPhCntContactId* iContactId;
            };

        /**
        * Perform fetch.
        *
        * Destroyes itself always after fetch. In successful case, target 
        * descriptor will contain result. Otherwise it must be considered 
        * undefined.
        *
        * You can also cancel request by destroying the instance.
        *
        * @param aParams it contains parameters.
        * @return error code,
        *       KErrNone iff fetch succeeded,
        *       KErrCancel iff fetch was cancelled,
        *       KErrGeneral iff result was invalid,
        *       KErrOverflow iff result was too long
        *
        * If method returns error, then contents of aResult are undefined.
        */
        virtual TInt FetchLD( 
            TFetchParams& aParams ) = 0;

		/**
		* Performs a fetch.
		*
		* A contact link is used for fetching and as a search result 
		* a phone number is returned.
		*
		* Destroyes itself always after fetch. In successful case, target 
        * descriptor will contain result. Otherwise it must be considered 
        * undefined.
        *
        * You can also cancel request by destroying the instance.
        *
		* @param aContactLink Contact link.
		* @param aCallType Number type.
		* @param aNumber Found phone number. Needs to be NULL before the call.
		* @param aFieldLink Field link to user selected number. Needs to be NULL, before the call.
		* @return error code,
        *       KErrNone iff fetch succeeded,
        *       KErrCancel iff fetch was cancelled,
        *       KErrGeneral iff result was invalid,
        *       KErrOverflow iff result was too long        
        *
        * If method returns error, then contents of aResult are undefined.
		*/
		virtual TInt FetchPhoneNumberLD( 
			const TDesC8& aContactLink,		    
			const CPhCntSingleItemFetch::TCallType aCallType,
		    HBufC*& aNumber,
		    HBufC8*& aFieldLink ) = 0;
 
        /**
         * Shows dialog for user to select a phone number for other
         * purposes than making a call. The left softkey is 'Select'.
         *
         * Request can be canceled by deleting instance of this class.         
         *
         * @leave System wide error code
         *
         * @since S60 3.2
		 * @returns The selection made by user. Ownership is transferred to 
		 * caller. Note that the selection may be NULL, if user canceled the 
		 * the dialog.
		 */
        virtual CPhCntSelection* SelectPhoneNumberLD() = 0;
         
    };

#endif      // CPHCNTSINGLEITEMFETCH_H
            
// End of File
