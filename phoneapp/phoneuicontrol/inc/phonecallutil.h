/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Call util class.
*
*/


#ifndef PHONECALLUTIL_H
#define PHONECALLUTIL_H

// INCLUDES
#include <e32base.h>
#include "mphoneviewcommandhandle.h"
#include "mphonestatemachine.h"

// CLASS DECLARATION
 
/**
*  Call util class.
*
*/
class PhoneCallUtil
    {
    public:
        
        /**
         * Dials multimedia call.
         */
        IMPORT_C static void DialMultimediaCall();
        
        /**
         * Dials voice call.
         */
        IMPORT_C static void DialVoiceCallL();
        
        /**
         * Disconnects call.
         */
        IMPORT_C static TBool DisconnectCallL();
        
        /**
         * Checks is call a video call.
         */
        IMPORT_C static TBool IsVideoCall( const TInt aCallId );
        
        /**
         * Returns call id by state (KErrNotFound if call
         * not exists).
         */
        IMPORT_C static TInt CallIdByState( const TPEState aCallState );

        /**
         * Checks if call exists by state and type (returns 
         * EFalse if call not exists).
         */
        IMPORT_C static TBool CheckIfCallExists( 
                const TPEState aCallState, const TPECallType aCallType );
        
        /**
         * Checks is there any connected calls.
         * List of connected call states: EPEStateConnected
         *                                EPEStateConnectedConference
         *                                EPEStateConnecting
         */
        IMPORT_C static TBool IsAnyConnectedCalls();
        
    private:
      
        /**
         * Returns view command handler.
         */
        inline static MPhoneViewCommandHandle* ViewCommandHandle();
        
        /**
         * Returns phone state handler.
         */
        inline static MPhoneStateMachine* StateMachine();
    };

#endif      // PHONECALLUTIL_H  
            
// End of File
