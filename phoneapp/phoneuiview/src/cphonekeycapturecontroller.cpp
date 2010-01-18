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
* Description: Implementation of CPhoneKeyCaptureController class.
*
*/


// INCLUDE FILES
#include <eikenv.h>
#include <w32std.h>
#include "cphonekeycapturecontroller.h"
#include "tphonecmdparamKeycapture.h"
#include "phoneconstants.h"
#include "phonelogger.h"

// ================= MEMBER FUNCTIONS =======================
// C++ default constructor can NOT contain any code, that
// might leave.
//
CPhoneKeyCaptureController::CPhoneKeyCaptureController( CEikonEnv& aEikEnv ): iEikEnv( aEikEnv )
    {
    }

// ---------------------------------------------------------
// CPhoneKeyCaptureController::NewL()
// Two-phased constructor
// (other items were commented in a header).
// ---------------------------------------------------------
CPhoneKeyCaptureController* CPhoneKeyCaptureController::NewL( CEikonEnv& aEikEnv )
    {
    CPhoneKeyCaptureController* self = 
        new( ELeave ) CPhoneKeyCaptureController( aEikEnv );
    
    return self;
    }

// ---------------------------------------------------------
// Destructor
// ---------------------------------------------------------
// 
CPhoneKeyCaptureController::~CPhoneKeyCaptureController()
    {
    // should we have any captured keys, stop capturing now:
    TInt capturedKeyCount = iCapturedKeys.Count();
    for( TInt i = 0; i < capturedKeyCount ; i++ )
        {
        StopKeyCapture( iCapturedKeys[i] );
        }

    iCapturedKeys.Close();
    }

// ---------------------------------------------------------
// CPhoneKeyCaptureController::StartCapturingKey
// Starts capturing a key
// (other items were commented in a header).
// ---------------------------------------------------------
//      
void CPhoneKeyCaptureController::StartCapturingKey( 
    TPhoneCommandParam* aCommandParam ) 
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneKeyCaptureController::StartCapturingKey");
    TPhoneCmdParamKeyCapture* keyCaptureParam = 
        static_cast<TPhoneCmdParamKeyCapture*>( aCommandParam );

    TInt32 handle = KErrNotFound;
    TPhoneKeyCapture keyCapture;
    
    RWindowGroup& windowGroup = iEikEnv.RootWin();

    if ( !IsKeyCaptured ( keyCaptureParam->Key() ) )
        {
        __PHONELOG1( 
            EBasic, 
            EPhoneUIView,
            "Capture key %d here ", 
            static_cast<TInt>( keyCaptureParam->Key() ) );

        keyCapture.iKey = keyCaptureParam->Key();
        keyCapture.iKeyCode = keyCaptureParam->KeyCode();
        keyCapture.iHandle = 0; // set as initial value
        keyCapture.iHandleForUpAndDown = 0; // set as initial value

        switch( keyCaptureParam->CaptureType() )
            {
            case EPhoneKeyEvent:
                keyCapture.iType = EPhoneKeyCaptureEvent;
                keyCapture.iHandle = windowGroup.CaptureKey( 
                    keyCapture.iKeyCode, 0, 0 );
                handle = keyCapture.iHandle;
                break;
            case EPhoneKeyUpAndDownEvents:
                keyCapture.iType = EPhoneKeyCaptureUpAndDownEvents;
                keyCapture.iHandleForUpAndDown = 
                    windowGroup.CaptureKeyUpAndDowns( 
                    keyCapture.iKey, 0, 0 );
                handle = keyCapture.iHandleForUpAndDown;
                break;
            default: // EPhoneKeyAllEvents
                {
                keyCapture.iType = EPhoneKeyCaptureAllEvents;
                keyCapture.iHandle = windowGroup.CaptureKey( 
                    keyCapture.iKeyCode, 0, 0 );
                if ( keyCapture.iHandle >= 0 )
                    {
                    keyCapture.iHandleForUpAndDown = 
                        windowGroup.CaptureKeyUpAndDowns( 
                        keyCapture.iKey, 0, 0 );

                    if ( keyCapture.iHandleForUpAndDown < 0 )
                        {
                        windowGroup.CancelCaptureKey( keyCapture.iHandle );
                        }

                    handle = keyCapture.iHandleForUpAndDown;
                    }   
                break;
                }
            } // switch

        if ( handle >= 0 )
            {
            if ( iCapturedKeys.Append( keyCapture ) != KErrNone )
                {
                StopKeyCapture( keyCapture );
                }
            }
        }   // if isKeyCaptured
    }

// ---------------------------------------------------------
// CPhoneKeyCaptureController::StopCapturingKey
// Stops capturing a key
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPhoneKeyCaptureController::StopCapturingKey( 
    TPhoneCommandParam* aCommandParam ) 
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneKeyCaptureController::StopCapturingKey");
    TPhoneCmdParamKeyCapture* keyCaptureParam = 
        static_cast<TPhoneCmdParamKeyCapture*>( aCommandParam );

    const TInt capturedKeyCount = iCapturedKeys.Count();

    __PHONELOG2( 
        EBasic, 
        EPhoneUIView,
        "UnCapture key %d here (%d keys left)",
        static_cast<TInt>( keyCaptureParam->Key() ), 
        static_cast<TInt>( capturedKeyCount ) );

    TBool foundKey = EFalse;

    for( TInt i = 0; ( i < capturedKeyCount ) && !foundKey ; i++ )
        {
        if( iCapturedKeys[i].iKey == keyCaptureParam->Key() )
            {
            foundKey = ETrue;
            StopKeyCapture( iCapturedKeys[i] );
            iCapturedKeys.Remove(i);
            }
        }
    }

// ---------------------------------------------------------
// CPhoneKeyCaptureController::IsKeyCaptured
// May be used to ask whether a key has been set to be captured
// (other items were commented in a header).
// ---------------------------------------------------------
//      
TBool CPhoneKeyCaptureController::IsKeyCaptured( TStdScanCode aKey ) const
    {
    const TInt capturedKeyCount = iCapturedKeys.Count();
    TBool isCaptured = EFalse;

    for( TInt i = 0 ; ( i < capturedKeyCount ) && !isCaptured; i++ )
        {
        isCaptured = iCapturedKeys[i].iKey == aKey; 
        }

    return isCaptured;
    }

// ---------------------------------------------------------
// CPhoneKeyCaptureController::StopKeyCapture
// Stops capturing a key
// (other items were commented in a header).
// ---------------------------------------------------------
//      
void CPhoneKeyCaptureController::StopKeyCapture( TPhoneKeyCapture aKeyCapture )
    {
    RWindowGroup& windowGroup = iEikEnv.RootWin();

    switch ( aKeyCapture.iType )
        {
        case EPhoneKeyCaptureEvent:
            windowGroup.CancelCaptureKey( aKeyCapture.iHandle );
            break;
        case EPhoneKeyCaptureUpAndDownEvents:
            windowGroup.CancelCaptureKeyUpAndDowns( 
                aKeyCapture.iHandleForUpAndDown );
            break;
        default: // EPhoneKeyCaptureAllEvents
            {
            windowGroup.CancelCaptureKey( aKeyCapture.iHandle );
            windowGroup.CancelCaptureKeyUpAndDowns( 
                aKeyCapture.iHandleForUpAndDown );
            break;
            }
        }
    }

// end of file
