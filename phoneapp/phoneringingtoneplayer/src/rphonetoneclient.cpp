/*
* Copyright (c) 2002-2005,2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Client interface of the phone ringing tone server.
*
*/


// INCLUDE FILES
#include <e32base.h>
#include <e32std.h>
#include <e32svr.h>

#include "rphonetoneclient.h"
#include "cphonetoneserver.h"
#include "tphonecmdparamringtone.h"
#include "phoneringingtonetrace.h"

// LOCAL FUNCTION PROTOTYPES
static TInt StartServer();

// -----------------------------------------------------------------------------
// RPhoneToneClient::RPhoneToneClient
// -----------------------------------------------------------------------------
//
EXPORT_C RPhoneToneClient::RPhoneToneClient() : RSessionBase()
    {
    }

// -----------------------------------------------------------------------------
// RPhoneToneClient::~RPhoneToneClient
// -----------------------------------------------------------------------------
//
EXPORT_C RPhoneToneClient::~RPhoneToneClient()
    {
    Close();
    }

// -----------------------------------------------------------------------------
// RPhoneToneClient::Connect
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RPhoneToneClient::Connect()
    {
    PHONERINGINGTONE_PRINT("RPhoneToneClient::Connect <");  
    
    TBool reConnect( EFalse );
   
    TInt err = CreateSession( KRingingToneServerName, 
            TVersion( KRingingToneSrvVersionMajor,
                      KRingingToneSrvVersionMinor,
                      KRingingToneSrvVersionBuild ) ); 

    if ( KErrNotFound == err )
        {
         err = StartServer();

         if ( !err )
             {
             reConnect = ETrue;
             }
        }

    if ( reConnect )
        {
        err = CreateSession( KRingingToneServerName, 
           TVersion( KRingingToneSrvVersionMajor,
                     KRingingToneSrvVersionMinor,
                     KRingingToneSrvVersionBuild ) );
        }

    PHONERINGINGTONE_PRINTF("RPhoneToneClient::Connect - err(%d)", err);
    PHONERINGINGTONE_PRINT("RPhoneToneClient::Connect >");
    return err;

    }

// -----------------------------------------------------------------------------
// StartServer
// -----------------------------------------------------------------------------
//
TInt StartServer()
    {
    PHONERINGINGTONE_PRINT("RPhoneToneClient - StartServer <");
    // Form thread name
    TTime time;
    time.UniversalTime();

    TFullName threadName;
    threadName.Format( _L("PhoneRingingToneThread%Ld"), time.Int64() );

    // Create thread
    RThread thread;
    TInt err = thread.Create( threadName, 
            CPhoneToneServer::ThreadEntryFunc, 
            0x8000, 
            NULL, 
            NULL );
    
    PHONERINGINGTONE_PRINTF("RPhoneToneClient - StartServer - err(%d)", err);
    if ( err )
        {
        thread.Close();
        return err;
        }

    thread.SetPriority( EPriorityAbsoluteHigh );
    TRequestStatus status;
    thread.Rendezvous( status );

    thread.Resume();

    // Wait for thread start or death
    User::WaitForRequest( status );
    thread.Close();
    PHONERINGINGTONE_PRINT("RPhoneToneClient - StartServer >");
    return status.Int();
    }

// -----------------------------------------------------------------------------
// RPhoneToneClient::PlayRingingToneL
// -----------------------------------------------------------------------------
//
EXPORT_C void RPhoneToneClient::PlayRingingToneL( TPhoneCommandParam *aCommandParam )
    {
    PHONERINGINGTONE_PRINT("RPhoneToneClient::PlayRingingToneL <");
    
    if ( aCommandParam->ParamId() == TPhoneCommandParam::EPhoneParamIdRingTone )
        {
        TPhoneCmdParamRingTone* ringToneParam = 
            static_cast<TPhoneCmdParamRingTone*>( aCommandParam );
            
        TIpcArgs args( &( ringToneParam->RingTone() ), 
                ringToneParam->Volume(), 
                ringToneParam->RingingType() );
        User::LeaveIfError( SendReceive( EPlayAudioRingTone, args ) );
        }
    PHONERINGINGTONE_PRINT("RPhoneToneClient::PlayRingingToneL >");
    }

// -----------------------------------------------------------------------------
// RPhoneToneClient::StopPlaying
// -----------------------------------------------------------------------------
//
EXPORT_C void RPhoneToneClient::StopPlayingL()
    {
    PHONERINGINGTONE_PRINT("RPhoneToneClient::StopPlayingL <");    
    User::LeaveIfError( SendReceive( EStopPlaying ) );
    PHONERINGINGTONE_PRINT("RPhoneToneClient::StopPlayingL >"); 
    }

// End of file
