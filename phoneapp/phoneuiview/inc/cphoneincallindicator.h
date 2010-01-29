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
* Description:  It manages incall indicator.
*
*/


#ifndef CPHONEINCALLINDICATOR_H
#define CPHONEINCALLINDICATOR_H

// INCLUDES
#include <e32base.h>

// FORWARD DECLARATIONS
class CAknIncallBubble;
class TPhoneCommandParam;

// CLASS DECLARATION

/**
*  It manages incall indicator (small bubble over the status pane).
*
*  @since 1.0
*/
class CPhoneIncallIndicator : public CBase
    {
    public:  // Constructors and destructor      
        
        /**
        * Two-phased constructor.
        * @return new instance.
        */
        static CPhoneIncallIndicator* NewL();
        
        /**
        * Destructor.
        */
        ~CPhoneIncallIndicator();

    public: // New functions

        /**
        * Update indicator using the specified data
        * @param aCommandParam command parameter
        */
        void Update( TPhoneCommandParam* aCommandParam );

        /**
        * Handle an audio mute change
        * @param aCommandParam command parameter
        */
        void HandleMuteChange( TPhoneCommandParam* aCommandParam );

        /**
        * Handle a ciphering change
        * @param aCommandParam command parameter
        */
        void HandleCipheringChange( TPhoneCommandParam* aCommandParam );
        
        
        /**
        * return is emergency call active
        */
        TBool IsEmergencyCall();

    private:

        /**
        * C++ constructor.
        */
        CPhoneIncallIndicator();

        /**
        * Symbian OS constructor.
        */
        void ConstructL();

        /**
        * Update indicator.
        */
        void Update();

        /**
        * Sets state.
        *
        * @param aState new state.
        * @param aSync do synchronously.
        */
        void SetState( TInt aState, TBool aSync );

        /**
        * Updates using recovery system.
        */
        void UpdateWithRecovery();

        /**
        * Updates incall indicator.
        */
        static TInt DoRecoverUpdateL( TAny* aAny );

        /**
        * Updates incall indicator.
        */
        static TInt DoUpdate( TAny* aAny );

    private:    // Data

        // Recovery id for updating incall indicator.
        TInt iUpdateRecoveryId;

        // Current call state.
        TInt iCallState;

        // Incall mode.
        TInt iMode;

        // Current state.
        TInt iState;

        // Owned incall bubble instance.
        CAknIncallBubble* iBubble;

        // Indicates if phone is muted.
        TBool iMuted;

        // Indicates if ciphering is off.
        TBool iCipheringOff;

        // Indicates if call is in emergency mode.
        TBool iEmergency;

        // Owned callback.
        CIdle* iUpdater;
        
        // Indicates if line2 is in use.
        TBool iLine2;
        
        // Indicated if bubble needs to be visible or not
        TBool iVisible;
        
        // Indicates if ciphering indicator is allowed.
        TBool iCipheringIndicatorAllowed; 
    };

#endif      // CPHONEINCALLINDICATOR_H
            
// End of File
