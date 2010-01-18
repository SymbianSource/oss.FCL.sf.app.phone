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
*     Resource pool for CCoeControls.
*
*/


#ifndef CBUBBLERESOURCEMANAGER_H
#define CBUBBLERESOURCEMANAGER_H

//  INCLUDES
#include <e32std.h>
#include <e32base.h>

// FORWARD DECLARATIONS
class CEikImage;
class CEikLabel;
class CBubbleAnimationControl;
class CCoeControl;
class CBubbleManager;

// CLASS DECLARATION

/**
*  Resource pool for CCoeControls.
*
*  @lib bubblemanager
*  @since 1.0
*/
class CBubbleResourceManager :public CBase
    {
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        * @param aParentWindow Main container
        * @return New instance
        */
        static CBubbleResourceManager* NewL( CBubbleManager& aParentWindow );
        
        /**
        * Destructor.
        */
        virtual ~CBubbleResourceManager();

    public: // New functions
        
        /**
        * Reserves CEikLabel object. Object must be released after use.
        * @return CEikLabel object.
        */
        CEikLabel* ReserveEikLabel();

        /**
        * Releases CEikLabel object. 
        * @param aLabel Object to be release. Pointer will be set to NULL.
        */
        void ReleaseEikLabel( CEikLabel*& aLabel );

        /**
        * Reserves CEikImage object. Object must be released after use.
        * @return CEikImage object.
        */
        CEikImage* ReserveEikImage( TBool aIsBackgroundImage = EFalse );

        /**
        * Releases CEikImage object. 
        * @param aImage Object to be release. Pointer will be set to NULL.
        */
        void ReleaseEikImage( CEikImage*& aImage );

        /**
        * Activates all the controls.
        * @since 2.0
        */
        void ActivateL();

    private:

        // Enumeration for different resources
        enum TBubbleResource
            {
            EBMEikImage = 0,
            EBMEikLabel
            };

        /**
        * C++ default constructor.
        */
        CBubbleResourceManager( CBubbleManager& aParentWindow );

        /**
        * 2nd phase constructor
        */
        void ConstructL();

        /**
        * Reserves one resource object.
        */
        void ReserveResource( 
            CCoeControl*& aResource , 
            TBubbleResource aType );

        /**
        * Releases current resource.
        */
        void ReleaseResource( 
            CCoeControl*& aResource , 
            TBubbleResource aType );

    private:    // Data

        typedef CArrayPtrFlat<CCoeControl> CSingleResource;

        // array of max elements
        RArray<TUint> iMaxAmounts;       
        // array of resource arrays
        CArrayPtrFlat<CSingleResource>* iResources; 
        // array of info about usage
        RArray<TUint> iAvailabilities; 

        // Parent window for controls
        CBubbleManager& iParentWindow;     

    };

#endif      // CBUBBLERESOURCEMANAGER_H 
            
// End of File
