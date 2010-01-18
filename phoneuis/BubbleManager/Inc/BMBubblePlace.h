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
*     Handles single place on screen.
*     One place may contain several popup windows from LAF
*
*/
 

#ifndef BUBBLEPLACE_H
#define BUBBLEPLACE_H

// INCLUDES  
#include <coecntrl.h>
#include <bldvariant.hrh>

// FORWARD DECLARATIONS
class CBubbleManager;
class CBubbleImageManager;
class CBubbleOutlook;


// CLASS DECLARATION

/**
*  CBubblePlace container class
*
*  Handles single place on screen.
*
*  @lib bubblemanager
*  @since 1.0
*/
class CBubblePlace : public CCoeControl 
    {
    public:
        
        // Place enums
        enum TPhoneBubblePlace
            {
            EBottom = 0,
            EBottomRightActive, 
            EBottomRightHeld,
            EMiddleActive,
            EMiddleHeld,
            ETopLeft,
            ETopRight,
            EHide,
            E2InConf,
            E3InConf,
            E4InConf,
            E5InConf,
            EBottomText, // Call object text
            EBottomImage, // Call object image
            EVideo,
            EBottomCnap,
            EMiddleCnap,
            EVideoCnap,
            EBottomRightCnap,
            EBottomImageCnap, // 19
            EBottomRightActiveTouch,
            EBottomRightActiveCnapTouch,
            EMiddleHeldTouch // 22
            };
        
    public: // Constructors and destructor
        
        /**
        * Symbian OS 2nd phase constructor.
        * @param aBubblePlace Current place
        * @param aBubbleManager Main container
        */
        void ConstructL( const TPhoneBubblePlace& aBubblePlace,
                          CBubbleManager& aBubbleManager );
        
        /**
        * Destructor.
        */
        ~CBubblePlace();
        
    public: //member functions
        
        /**
        * Set current place in use.
        * @param aIsUsed ETrue if set in use.
        */
        void SetIsUsed( const TBool& aIsUsed );
        
        /**
        * Query is used?
        * @return is used.
        */
        TBool IsUsed( ) const;
        
        /**
        * Get current place.
        * @return The place enumeration.
        */
        TPhoneBubblePlace GetBubblePlace( ) const;
        
        /**
        * Get current outlook component. Abstraction!
        * @return BubbleOutlook, not always in use!
        */
        CBubbleOutlook& GetBubbleOutlook( ) const;
        
    private: // Functions from base classes
        
        /**
        * From CCoeControl
        */
        void SizeChanged();

        /**
        * From CCoeControl
        */
        void PositionChanged();

        /**
        * From CCoeControl
        */
        TInt CountComponentControls() const;
        
        /**
        * From CCoeControl
        */
        CCoeControl* ComponentControl( TInt aIndex ) const;
        
        
    private: //data
        
        // Current place
        TPhoneBubblePlace iBubblePlace;
        // Current outlook
        CBubbleOutlook* iBubbleOutlook;
        // Is place in use?
        TBool iIsUsed;

    private:

        friend class CT_CBubblePlace;
        
    };

#endif // BUBBLEPLACE_H

// End of File
