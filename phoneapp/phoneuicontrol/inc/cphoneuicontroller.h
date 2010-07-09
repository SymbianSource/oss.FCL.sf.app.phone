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
*     Phone UI controller class.
*
*/


#ifndef CPHONEUICONTROLLER_H
#define CPHONEUICONTROLLER_H

//  INCLUDES
#include <e32base.h>
#include <e32std.h>

#include "mphonekeyeventhandler.h"
#include "mphonemenuandcbaevents.h"
#include "mphonesystemeventhandler.h"
#include "mphonestatemachine.h"

// FORWARD DECLARATIONS
class CPhoneEngineHandler;
class MPhoneKeyEventHandler;
class CPhoneSystemEventHandler;
class MPhoneViewCommandHandle;
class MPhoneStateMachine;
class CPhoneStateHandle;
class CPEPhoneModel;
class CPhoneRemoteControlHandler;


class CPhoneUIController : 
    public CBase, 
    public MEngineMonitor,
    public MPhoneKeyEventHandler,
    public MPhoneSystemEventHandler,
    public MPhoneMenuAndCbaEvents
    {
    public:

        /**
        * Two-phased constructor.
        */
        IMPORT_C static CPhoneUIController* NewL( 
            MPhoneViewCommandHandle* aViewCommandHandle );
        
        /**
        * Destructor.
        */
        IMPORT_C virtual ~CPhoneUIController();

        /**
        * HandleMessage
        * Creates a trap for leaves from message handling functions and 
        * calls DoHandleMessage to handle the message.
        * @param aMessage: a message Id from phone application engine
        * @param aCallId: a Id of the call
        * @return 
        */
        IMPORT_C void HandleMessage( 
            const TInt aMessage, 
            const TInt aCallId );
            
        /**
        * HandleError
        * Implements error handling framework
        * @param aErrorInfo: the error info
        */
        IMPORT_C void HandleError( const TPEErrorInfo& aErrorInfo );

        IMPORT_C TKeyResponse HandleKeyEventL(
            const TKeyEvent& aKeyEvent,
            TEventCode aEventCode );

        /**
        * From CEikAppUi. For Idle indicator
        */
        IMPORT_C void HandleSystemEventL(
            const TWsEvent& aEvent );

        /**
        * From CAknAppUi, indicates when app ui is on the foreground.
        * @param aForeground It is true if app is on the foreground.
        */
        IMPORT_C void HandleForegroundEventL( TBool aForeground );

        /**
        * Indicates when the Phone app is in the foreground.
        */
        IMPORT_C virtual void HandlePhoneForegroundEventL();

        /**
        * Indicates when the Phone app is losing focus.
        */
        IMPORT_C virtual void HandlePhoneFocusLostEventL();
        
        /**
        * Indicates when the Idle app is in the foreground.
        */
        IMPORT_C virtual void HandleIdleForegroundEventL();

        /**
        * Handle environment changes.
        * @param aChanges environment changes which may be reported by
        *  a change notifier through the RChangeNotifier interface.
        */
        IMPORT_C void HandleEnvironmentChangeL( const TInt aChanges );

        /**
        * Handles startup of the phone application 
        */
        IMPORT_C void HandlePhoneStartupL();

        /**
        * Handles commands.
        * @param aCommand It is the code of the command to be handled.
        * @returns boolean value was the command handled by the state 
        *          (ETrue) or not (EFalse)
        */
        IMPORT_C TBool HandleCommandL( TInt aCommand );

        /**
        * Process command.
        * @param aCommand It is the code of the command to be handled.
        * @returns boolean value was the command handled by the state 
        *          (ETrue) or not (EFalse)
        */
        IMPORT_C TBool ProcessCommandL( TInt aCommand );

        /**
        * Handles keylock events
        * @param aCommand It is the code of the command to be handled.
        */
        IMPORT_C void HandleKeyLockEnabled( TBool aKeylockEnabled );
        
    private:
        
        /**
        * C++ default constructor.
        */
        CPhoneUIController();

        /**
        * By default EPOC constructor is private.
        */
        void ConstructL( MPhoneViewCommandHandle* aViewCommandHandle );
        
        /**
        * Creates correct protocol DLL depending of the variation.
        */
        void CreateProtocolDllL( MPhoneViewCommandHandle* aViewCommandHandle );
        /**
        * CallBack for Phone Number Editor
        */
        static TInt HandlePhoneNumberEditorCallBack( TAny* aAny );
        
        /**
        * Handles the CallBack for Phone Number Editor
        */
        void DoHandlePhoneNumberEditorCallBack();
        
    private:    // Data

        /**
        * CPEPhoneModel
        */
        MPEPhoneModel*  iPhoneEngine;

        /**
        * MPEEngineInfo
        */
        MPEEngineInfo*  iEngineInfo;

        /**
        * iEngineHandler : 
        */
        CPhoneEngineHandler* iEngineHandler;

        /**
        * iKeyEventHandler : 
        */
        MPhoneKeyEventHandler* iKeyEventHandler;

        /**
        * iSystemEventHandler : 
        */
        CPhoneSystemEventHandler* iSystemEventHandler;
        
        /**
        * iRemoteControlHandler : 
        */
        CPhoneRemoteControlHandler* iRemoteControlHandler;

        /**
        * MPhoneStateMachine
        */
        CPhoneStateHandle* iStateHandle;

        /**
        * MPhoneStateMachine
        */
        MPhoneStateMachine* iStateMachine;  // NOT OWNED
        
    };

#endif      // CPHONEUICONTROLLER_H   
            
// End of File
