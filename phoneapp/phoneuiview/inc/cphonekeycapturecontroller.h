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
* Description:  PhoneUI Key Capture Controller
*
*/

/*
*/

#ifndef __CPHONEKEYCAPTURECONTROLLER_H
#define __CPHONEKEYCAPTURECONTROLLER_H


//  INCLUDES
#include <e32base.h>
#include <e32keys.h>

// FORWARD DECLARATIONS
class TPhoneCommandParam;

class CPhoneKeyCaptureController : public CBase
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CPhoneKeyCaptureController* NewL( CEikonEnv& aEikEnv );

        /**
        * Destructor.
        */
        virtual ~CPhoneKeyCaptureController();   

    public: 

        /**
        * Set key to be captured 
        * @param aCommandParam a command param
        */
        void StartCapturingKey( TPhoneCommandParam* aCommandParam );

        /**
        * Set key not to be captured 
        * @param aCommandParam a command param
        */
        void StopCapturingKey( TPhoneCommandParam* aCommandParam );

    private:  // Constructors
        
        /**
        * C++ default constructor.
        */
        CPhoneKeyCaptureController( CEikonEnv& aEikEnv );

    private:    // Data types

        // enumerated type for captured key events
        enum TPhoneKeyCaptureType
            {
            EPhoneKeyCaptureEvent,
            EPhoneKeyCaptureUpAndDownEvents,
            EPhoneKeyCaptureAllEvents
            };

        // key capture data structure
        class TPhoneKeyCapture
            {
            public:
                TStdScanCode            iKey;
                TKeyCode                iKeyCode;
                TPhoneKeyCaptureType    iType;
                TInt32                  iHandle;
                TInt32                  iHandleForUpAndDown;
            };

    private:  // New functions

        /**
        * May be used to ask whether a key has been set to be captured
        * using StartCapturingKey()
        * @param aKey is the iScanCode of the key
        * @return ETrue if the key is currently captured via this mechanism
        */
        TBool IsKeyCaptured( TStdScanCode aKey ) const;

        /**
        * Set key not to be captured 
        * @param aKeyCapture is the key not to be captured
        */
        void StopKeyCapture( TPhoneKeyCapture aKeyCapture );

    private:  // Data

        /**
        * Array of keycodes currently captured which includes the window
        * server handles for the captured keys.
        */
        RArray<TPhoneKeyCapture> iCapturedKeys;
        
        CEikonEnv& iEikEnv;

    };

#endif      // __CPHONEKEYCAPTURECONTROLLER_H   
            
// End of File
