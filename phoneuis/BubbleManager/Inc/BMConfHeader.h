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
* Description: 
*     Concrete class which holds info about conference call
*
*/


#ifndef CONFHEADER_H
#define CONFHEADER_H

// INCLUDES
#include  "BMBubbleHeader.h"

// FORWARD DECLARATIONS
class CBubbleCallHeader;

// CLASS DECLARATION

/**
*  CBubbleConfHeader class
*
*  Concrete class which holds info about conference call
*
*  @lib bubblemanager
*  @since 1.0
*/

class CBubbleConfHeader : public CBubbleHeader
    {    
    public: // Constructors and destructor
        
        /**
        * two phase constructor
        * @param aBubbleId Id of the call header
        * @return New instance
        */
        static CBubbleConfHeader* NewL( 
            const CBubbleManager::TBubbleId& aBubbleId);
        
        /**
        * Destructor.
        */
        virtual ~CBubbleConfHeader();
        
    public: // Member functions
              
        /**
        * Add call to conference call.
        * @param aCall Added call.
        */
        void AddRow( CBubbleCallHeader& aCall );

        /**
        * Remove call from conferece.
        * @param aCallId Bubble id of removed call.
        */
        void RemoveRow( const CBubbleManager::TBubbleId& aCallId );

        /**
        * Get headers in conference call.
        * @param aCalls Reference where item are put to.
        */
        void GetRows( CArrayPtrFlat<CBubbleCallHeader>& aCalls ) const;

        /**
        * Get amount of calls in conference call.
        * @return Amount of calls in conf.
        */
        TUint8 RowCount() const;

        /**
        * Set highlight row.
        * @param aRow The row number 1-n. KBubbleNoHighlightRow no highlight.
        */
        void SetHighlight( const CBubbleManager::TRowNumber& aRow );

        /**
        * Set highlight header.
        * @param aBubbleId Id of highlighted item.
        */
        void SetHighlightId( const CBubbleManager::TBubbleId& aBubbleId );

        /**
        * Get bubble id of highlighted item.
        * @return Id of highlighted item. KBubbleInvalidId if no highlight.
        */
        CBubbleManager::TBubbleId HighlightId() const;

        /**
        * Move highlight one item upwards. If already at upmost, no change.
        */
        void MoveHighlightOneUp();

        /**
        * Move highlight one item downwards. If already at lowest, no change.
        */
        void MoveHighlightOneDown();

        /**
        * Get the highlight row number.
        * @return The row number 1-n. KBubbleNoHighlightRow no highlight.
        */
        CBubbleManager::TRowNumber Highlight() const;

        /**
        * Set conference call expanded.
        * @param aIsExpanded ETrue expands the conf call.
        */
        void SetIsExpanded( TBool aIsExpanded );

        /**
        * Is conference expanded.
        * @return ETrue if expanded.
        */
        TBool IsExpanded( ) const;

    public: // Functions form base classes

        /**
        * From CBubbleHeader.
        */
        virtual void Reset();

        /**
        * From CBubbleHeader.
        */
        virtual void SetIsUsed( const TBool& aIsUsed );

        /**
        * From CBubbleHeader.
        */
        virtual TBool IsUsed() const;

        /**
        * From CBubbleHeader. 
        */
        virtual TBool IsConference() const;
        
    protected:

        /**
        * Default constructor.
        */
        CBubbleConfHeader();
        
    private: // private functions

        /**
        * 2nd phase constructor.
        */
        void ConstructL( const CBubbleManager::TBubbleId& aBubbleId );
        
    private: //data
        // Is header used?
        TBool iIsUsed;
        // Is conference expanded?
        TBool iIsExpanded;
        //1-5 , KBubbleNoHighlightRow no highlight
        CBubbleManager::TRowNumber iSelection;
        // Current calls in conference call
        CArrayPtrFlat<CBubbleCallHeader>* iCalls;

    private:
        friend class CT_CBubbleConfHeader;
    };

#endif //CONFHEADER_H

// End of File
