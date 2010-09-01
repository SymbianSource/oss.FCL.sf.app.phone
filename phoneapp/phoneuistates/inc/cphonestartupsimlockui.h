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
* Description: 
*     GSM-specific single call state implementation.
*
*/
#ifndef CPHONESTARTUPSIMLOCKUI_H
#define CPHONESTARTUPSIMLOCKUI_H

// INCLUDES
#include "cphonestatestartup.h"

// FORWARD DECLARATIONS

// CLASS DECLARATION
/**
 *  GSM-specific start up state implementation.
 */
class CPhoneStartupSimlockUi : public CPhoneStateStartup
    {
public:

    /**
     * Destructor.
     */
    virtual ~CPhoneStartupSimlockUi();

    /**
     * Creates the Start up state class
     * @param aStateMachine: a state machine
     * @param aViewCommandHandle: handle to the PhoneUIView
     * @param aPEReady: true if Phone Engine already started
     * @return an instance of class CPhoneEmergency
     */
    static CPhoneStartupSimlockUi* NewL( MPhoneStateMachine* aStateMachine,
            MPhoneViewCommandHandle* aViewCommandHandle, TBool aPEReady );

     /**
     * A message handling function for Phone Engine messages
     * @param aMessage Message from Phone Engine
     * @param aCallId Call id the message concerns
     */
     IMPORT_C void HandlePhoneEngineMessageL(
         const TInt aMessage, 
         TInt aCallId );       


    // From MPhonePubSubObserver
    /**
     * This function is called when there is property value change.
     * @param aCategory Category of the property
     * @param aKey Property key that is changed
     * @param aValue New property value
     */
    IMPORT_C virtual void HandlePropertyChangedL( const TUid& aCategory,
            const TUint aKey, const TInt aValue );       

private:

    /**
     * By default EPOC constructor is private.
     */
    CPhoneStartupSimlockUi( MPhoneStateMachine* aStateMachine,
            MPhoneViewCommandHandle* aViewCommandHandle, TBool aPEReady );

    /**
     * ConstructL()
     */
    void ConstructL();

    /**
     * SimLock Observer.
     * Determines  if the SimLock mode should be entered
     */
    void EnterSimLockModeL();

    /**
     * SimLock Observer.
     * Takes care of the SimLock Exit Scenario
     */
    void ExitSimLockModeL();

    /**
     * Handles startup of the phone application 
     */
    IMPORT_C virtual void HandlePhoneStartupL();


private:
       
    //To check if Simlock is active
    TBool iSimLockActive;
    
    // Ownded: idle object to create note.
    CIdle* iCreateNote;

    };

#endif // CPHONESTARTUPSIMLOCK_H
// End of File
