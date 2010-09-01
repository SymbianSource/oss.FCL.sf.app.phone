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
* Description:  Mapping Call To Bubble
*
*/


#ifndef __CPHONEBUBBLEMAPPING_H
#define __CPHONEBUBBLEMAPPING_H

// INCLUDES
#include    <e32base.h>
#include    <bmbubblemanager.h>

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
* It defines a single item in mapping.
*/
class TMappingItem
    {
    public:
        TInt iCallId;
        CBubbleManager::TBubbleId iBubbleId;
        CBubbleManager::TPhoneCallState iCallState;
    
            // NOT OWNED
        CFbsBitmap* iThumbnail; // the thumbnail to be displayed in the 
                                // call bubble 
    };
   
/**
* It keeps up mapping from call objects to bubble ids.
*/
class CPhoneBubbleMapping 
    : public CBase
    {
    public:  // Constructors and destructor      
        
        /**
        * Two-phased constructor.
        *
        * @param aMaximumAmount It contains maximum amount of 
        *        calls at the same time.
        */
        static CPhoneBubbleMapping* NewL(
            TInt aMaximumAmount );
        
        /**
        * Destructor.
        */
        ~CPhoneBubbleMapping();

    public: // New functions

        /**
        * Find a bubble id by call id.
        *
        * @param aCallId call Id.
        * @param aId It will contain bubble id corresponding to the call.
        * @return ETrue iff succeeded.
        */
        TBool FindBubbleByCall( 
            TInt aCallId,
            CBubbleManager::TBubbleId& aId ) const;
        
        /**
        * Find call id by bubble id.
        *
        * @param aId bubble id.
        * @param aCallId It will contain call corresponding to the bubble.
        * @return ETrue iff succeeded.
        */
        TBool FindCallByBubble(
            CBubbleManager::TBubbleId aId,
            TInt& aCallId ) const;

        /**
        * Find thumbnail id by call id.
        *
        * @param aCallId call Id.
        * @return the thumbnail iff succeeded.
        */
        const CFbsBitmap* FindThumbnailByCall( 
            TInt aCallId ) const;
        
        /**
        * Add to mapping.
        *
        * Note that method panics if there already is an item with
        * the same call or allocation fails.
        *
        * @param aId It is the id of the call bubble.
        * @param aCallId It is call id corresponding to the bubble.
        * @param aThumbnail is the thumbnail attached to the bubble
        */
        void AddToMappingL( 
            CBubbleManager::TBubbleId aId, 
            TInt aCallId,
            CBubbleManager::TPhoneCallState aCallState,
            CFbsBitmap* aThumbnail
            );

        /**
        * Remove from mapping.
        *
        * @param aCallId The call id
        */
        void RemoveFromMapping( 
            TInt aCallId );
 
        /**
        * Returns total amount of items in mapping.
        *
        * @return amount of items in mapping.
        */
        TInt ItemCount() const;

        /**
        * Returns bubble at position.
        *
        * @param aIndex position.
        * @return id of the corresponding bubble.
        */
        CBubbleManager::TBubbleId BubbleAt( TInt aIndex ) const;
        
        /**
        * Returns call id at position.
        *
        * @param aIndex position.
        * @return corresponding call id.
        */
        TInt CallIdAt( TInt aIndex ) const;

        /**
        * Returns Thumbnail pointer at position.
        *
        * @param aIndex position.
        * @return corresponding thumbnail
        */
        const CFbsBitmap* ThumbnailAt( TInt aIndex ) const;

        /**
        * update Thumbnail pointer at position.  this function
        * will release the memory if the current status of the
        * thumbnail pointer is not NULL
        *
        * @param aCallId the call ID
        * @param corresponding thumbnail
        * @return none
        */
        void SetThumbnailByCall(
            TInt aCallId,
            CFbsBitmap* aThumbnail );
            
        /**
        * update CallState at position. 
        *
        * @param aCallId the call ID
        * @param corresponding callState
        * @return none
        */
        void SetCallState(
            TInt aCallId,
            CBubbleManager::TPhoneCallState aCallState );
            
        /**
        * Find callState by call id.
        *
        * @param aCallId call Id.
        * @return the callState if succeeded.
        */
        CBubbleManager::TPhoneCallState FindCallStateByCall( 
            TInt aCallId ) const;
            
       /**
        * Returns CallState at position.
        *
        * @param aIndex position.
        * @return callState
        */
        CBubbleManager::TPhoneCallState CallStateAt( TInt aIndex ) const;

        /**
        * update callState at position.
        *
        * @param aCallId the call ID
        * @param aCallState
        * @return none
        */
        void SetCallStateByCall(
            TInt aCallId,
            CBubbleManager::TPhoneCallState aCallState );
        
        /**
        * Find callId by callState.
        *
        * @param aCallState call state.
        * @return the callId if succeeded
        * and -1 if not found.
        */    
        TInt FindCallIdByCallState( 
            CBubbleManager::TPhoneCallState aCallState );

    private:
        /**
        * update Thumbnail pointer at position.
        * If user need to free the thumbnail, this call should
        * be used instead of user doing "delete thumbnail"
        * the user can call SetThumbnailAt( index , NULL ) to 
        * release and reset the pointer
        *
        * @param aIndex position.
        * @param corresponding thumbnail
        * @return none
        */
        void SetThumbnailAt( 
            TInt aIndex, 
            CFbsBitmap* aThumbnail);
            
        /**
        * update CallState at position.
        *
        * @param aIndex position.
        * @param callState
        * @return none
        */
        void SetCallStateAt( 
            TInt aIndex, 
            CBubbleManager::TPhoneCallState aCallState );

        /**
        * Find the index of the item by call ID
        *
        * @param aCallId call Id.
        * @param aIndex the index of the call item
        * @return ETrue iff succeeded.
        */
        TBool FindIndexByCall( 
            TInt aCallId,
            TInt& aIndex ) const;

    private:

        /**
        * C++ default constructor.
        */
        CPhoneBubbleMapping();

        /**
        * By default Symbian OS constructor is private.
        */
        void ConstructL( TInt aMaximumAmount );

        // By default, prohibit copy constructor
        CPhoneBubbleMapping( const CPhoneBubbleMapping& );
        // Prohibit assigment operator
        CPhoneBubbleMapping& operator= ( 
            const CPhoneBubbleMapping& );

    private:    // Data

        /**
        * It defines type for array of mapping items.
        */
        typedef CArrayFixFlat<TMappingItem> CMappingArray;

        /**
        * It is the mapping.
        */
        CMappingArray* iMapping;

        /**
        * It is the amount of reserved items in mapping.
        */
        TInt iMappingReserve;
    };

#endif      // __CPHONEBUBBLEMAPPING_H
            
// End of File
