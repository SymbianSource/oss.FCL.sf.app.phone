/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
*     Singleton class for implementing video call header text animation.
*
*/


#ifndef CPHONECALLHEADERTEXTANIMATION_H
#define CPHONECALLHEADERTEXTANIMATION_H

// INCLUDES
#include <coemain.h>
#include "mphonetimer.h"
#include "bmbubblemanager.h"

// CONSTANTS
const TInt KPhoneCallHeaderAnimationTextLength = 125;

// FORWARD DECLARATIONS
class CPhoneTimer;

// CLASS DECLARATION

class CPhoneCallHeaderTextAnimation : public CCoeStatic, public MPhoneTimer
    {
    public:  

        /**    
         * Returns an instance of this class. When called for the first
         * time, a new instance is created and returned.  After that,
         * calling InstanceL returns the same instance that was created
         * earlier.
         *   
         * @return A pointer to a CPhoneCallHeaderTextAnimation object    
         */    
        static CPhoneCallHeaderTextAnimation* InstanceL();   
        
        /**
        * Destructor.
        */
        virtual ~CPhoneCallHeaderTextAnimation();
        
        /**
        * Stops and destroys video call text animation bubble.
        */
        void RemoveAnimatingVideoCallHeader();
 
		/**
		* Begins animating video call header
		* @param aBubbleId - bubble which will affect on this
		* @param aNormalText - normal length text
		* @param aShortText - short version of the text
		* @param aBubbleManager - reference to bubble manager for manipulating
		*						  the bubble
		*/
		void StartAnimatingVideoCallHeaderL(
			const CBubbleManager::TBubbleId aBubbleId,    
    		const TDesC& aNormalText,
    		const TDesC& aShortText,
    		CBubbleManager& aBubbleManager );
    	
    	/**
    	* Sets bubble texts
    	* @param aNormalText - normal length text
    	* @param aShortText - short version of the text
    	*/	
		void SetBubbleTexts( const TDesC& aNormalText, 
			const TDesC& aShortText );
        
    // From base class MPhoneTimer
        /**
        * From MPhoneTimer
        * This function is called after on timeout.
        */
        void HandleTimeOutL();
        

    private:

        /**    
         * Default constructor is private because we are using the
         * singleton design pattern.
         */    
        CPhoneCallHeaderTextAnimation();  
        
        /**
        * By default EPOC constructor is private.
        */
        void ConstructL();
                        
        
    private:
    
        /**
        * Animation timer
        */
        CPhoneTimer* iTimer;
        
        /**
        * Number of dots displayed in the bubble
        */
        TInt iDotNum;
          
        /*
        * Pointer to bubble manager
        */
        CBubbleManager* iBubbleManager;

        // Bubble Id for the bubble of active call
        CBubbleManager::TBubbleId iActiveBubble;

        // Normal text for video call header
        TBuf<KPhoneCallHeaderAnimationTextLength> iNormalText;

        // Short text for video call header
        TBuf<KPhoneCallHeaderAnimationTextLength> iShortText;

    };
    
#endif // CPHONECALLHEADERTEXTANIMATION_H

// End of File
