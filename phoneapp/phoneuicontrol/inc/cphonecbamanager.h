/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Class takes care of CBA's.
*
*/


#ifndef CPHONECBAMANAGER_H
#define CPHONECBAMANAGER_H

//INCLUDES
#include <e32base.h>

//FORWARD
class MPhoneViewCommandHandle;
class MPhoneStateMachine;
class CPhoneState;

/**
 * Class takes care of CBA's.
 *
 * @lib phoneuicontrol
 * @since S60 v9.1
 */
NONSHARABLE_CLASS( CPhoneCbaManager ) :  public CBase
    {
    public:
        
        /**
        * Creates new instance of CPhoneCbaManager
        * @param aStateMachine: a state machine
        * @param aViewCommandHandle: handle to the PhoneUIView
        * @return CPhoneCbaManager* object  
        */
        static CPhoneCbaManager* NewL(
                    CPhoneState* aState,
                    MPhoneViewCommandHandle& aViewCommandHandle,
                    MPhoneStateMachine& aStateMachine );
        /**
        * Destructor.
        */
        virtual ~CPhoneCbaManager();
        
    public:

        /**
        * Update CBA
        */
        IMPORT_C void UpdateCbaL( TInt aResource );

        /**
        * Set CBA
        */
        IMPORT_C void SetCbaL( TInt aResource );
        
        /**
        * Update incoming CBAs
        */
        IMPORT_C void UpdateIncomingCbaL( TInt aCallId );
        
    public:
        
        /**
        * Update In Call CBA
        */ 
        void UpdateInCallCbaL();
        
        /**
        * Sets the ringtone silenced status
        * @param aSilenceStatus, new status
        */
        void SetRingtoneSilencedStatus( const TBool aSilenceStatus );
        
    private:
        
        /*
        * Updated Cba when Swivel state changes.
        */
        void UpdateCbaSwivelStateChangedL();
        
        /**
        * Check is alerting or active voice call
        */
        TBool IsVoiceCallAlertingOrActiveL();
        
        /**
        * Get silence cba resource when incomng call
        * @param aSoftRejectActivated Is soft reject activated or not
        * @return Resource id of CBA
        */
        TInt GetIncomingCallSilenceCBA( const TBool aSoftRejectActivated ); 
        
    private:
    
        /**
        * C++ default constructor.
        */
        CPhoneCbaManager(
                CPhoneState* aState,
                MPhoneViewCommandHandle& aViewCommandHandle,
                MPhoneStateMachine& aStateMachine );
        
    private:
        
        // Not owned:
        CPhoneState* iState;
        MPhoneViewCommandHandle& iViewCommandHandle;
        MPhoneStateMachine& iStateMachine;
        
        // RingtoneSilenced flag.
        // This is used to determine if the ringtone
        // for incoming call is silenced. The information
        // is used while updating softkeys in method
        // UpdateIncomingCbaL only
        TBool iRingtoneSilenced;
        
    };


#endif // CPHONECBAMANAGER_H
