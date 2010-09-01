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
*     Concrete class which helds all the info about single call
*
*/


#ifndef CALLHEADER_H
#define CALLHEADER_H

// INCLUDES
#include  "BMBubbleHeader.h"

// CLASS DECLARATION

/**
*  CBubbleCallHeader class
*
*  Concrete class which helds all the info about single call.
*
*  @lib bubblemanager
*  @since 1.0
*/

class CBubbleCallHeader : public CBubbleHeader
    {
    
    public: // Constructors and destructor
        
        /**
        * two phase constructor
        * @param aBubbleId Id of the call header.
        * @return New instance.
        */
        static CBubbleCallHeader* NewL( 
            const CBubbleManager::TBubbleId& aBubbleId );
        
        /**
        * Destructor.
        */
        virtual ~CBubbleCallHeader();
        
    public: // Functions from base classes

        /**
        * From CBubbleHeader.
        */
        virtual TBool IsUsed() const;

        /**
        * From CBubbleHeader
        */
        virtual void SetIsUsed( const TBool& aIsUsed );

        /**
        * From CBubbleHeader
        */
        virtual TBool IsConference() const;

        /**
        * From CBubbleHeader
        */
        virtual void Reset();
        
    private: // private functions

        /**
        * Default constructor.
        */
        CBubbleCallHeader();

        /**
        * Symbian OS 2nd phase constructor.
        */
        void ConstructL( const CBubbleManager::TBubbleId& aBubbleId );
        
    private: //data
        // Is header in use?
        TBool iIsUsed;

    private:
        friend class CT_CBubbleCallHeader;
               
    };

#endif //CALLHEADER_H

// End of File
