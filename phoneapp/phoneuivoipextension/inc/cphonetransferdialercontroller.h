/*
* Copyright (c) 2008, 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
*     Unattended transfer dialer customization
*     controller
*
*/


#ifndef CPHONETRANSFERDIALERCONTROLLER_H_
#define CPHONETRANSFERDIALERCONTROLLER_H_

// INCLUDES
#include <aknbutton.h>
#include "cphonedialercontroller.h"

// FORWARD DECLARATIONS
class CAknToolbar; 

/**
*  VoIP unattended transfer dialer customization
*/
NONSHARABLE_CLASS( CPhoneTransferDialerController ): public CPhoneDialerController
    {
    public:      

        static CPhoneTransferDialerController* NewL();
        
        /**
        * Destructor.
        */
        virtual ~CPhoneTransferDialerController();

    public: // From MDialerController
    
        /**
        * Returns Cba resource id
        * @return Resource Id of the softkeys
        * @since S60 v5.1
        */
        TInt CbaResourceId() const;
        
        /**
        * Returns Menu resource id
        * @return Id of the menu resource
        * @since S60 v5.1
        */
        TInt MenuResourceId() const;
        
        /**
        * Returns number entry prompt text
        * @return Reference to text to be shown on number entry
        * @since S60 v5.1
        */
        const TDesC& NumberEntryPromptTextL();
        
        /**
         * @see MPhoneDialerController
         */
        TInt GetButtonData( TButtonIndex aIndex, RPointerArray<CButtonData>& aData ) const;
        
        /**
         * @see MPhoneDialerController
         */
        TInt ButtonState( TButtonIndex aIndex ) const;
        
        /**
         * @see MPhoneDialerController
         */
        TBool ButtonDimmed( TButtonIndex aIndex ) const;
        
        /**
         * @see MPhoneDialerController
         */
        TBool EasyDialingAllowed() const;
        
    private:
    
        /**
         * Returns tool tip for specific command
         * @param aCommandId Command id which the tooltip text 
         * is needed
         * @return Pointer to descriptor containing tooltip text
         */
        HBufC* GetTooltipTextL( TInt aCommandId ) const; 
        
        TAknsItemID SkinId( TInt aIconIndex ) const;

        CButtonData* CreateButtonDataL(
                TInt aCommandId,
                TInt aNormalIconId,
                TInt aNormalMaskId ) const;
    protected:

        /**
        * By default EPOC constructor is private.
        */
        CPhoneTransferDialerController();
            
    private: // Data
    
        // Number entry prompt text ("Address:")
        HBufC* iNumberEntryPromptText;
      
    };

#endif /*CPHONETRANSFERDIALERCONTROLLER_H_*/
