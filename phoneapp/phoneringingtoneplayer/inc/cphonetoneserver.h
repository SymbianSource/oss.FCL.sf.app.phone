/*
* Copyright (c) 2010-2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Phone ringing tone server definitions.
*
*/


#ifndef PHONETONESERVER_H
#define PHONETONESERVER_H

// INCLUDES
#include <f32file.h>
#include <Profile.hrh>
#include <e32base.h>
    
#include "mphonetoneserver.h"

// CONSTANTS
const TInt KRingingToneSrvVersionMajor = 1;
const TInt KRingingToneSrvVersionMinor = 0;
const TInt KRingingToneSrvVersionBuild = 0;
_LIT( KRingingToneServerName, "PhoneRingingToneSrv" );

enum TRingingToneSrvOpcodes
    {
    EPlayAudioRingTone,
    EStopPlaying
    };

// CLASS DECLARATION
NONSHARABLE_CLASS( CPhoneToneServer ) : public CServer2, public MPhoneToneServer
    {
public:

    static CPhoneToneServer* NewLC();

    ~CPhoneToneServer();

    
    /**
    * Thread entry function.
    *
    * @param aPtr
    * @return new instance.
    */    

    static TInt ThreadEntryFunc( TAny* aPtr );

public: 
    
    /**
    * Derived from CServer2, creates a new session.
    * @param aVersion It is the version of the client api.
    * @param aMessage Connect message from Client.
    * @see CServer2
    * @return Returns a new session.
    */          
    CSession2* NewSessionL( const TVersion& aVersion,  
                            const RMessage2& aMessage ) const;
    
    /**
    * Runs server down.
    * @see MPhoneToneServer
    */    
    virtual void RunDown() const;

private:

    CPhoneToneServer();

    void ConstructL();

private:

    /**
    * Runs server.
    */
    static void RunServerL();

private:
    
    /*
     * Server should be unique connectable to avoid IPC fuzzing. 
     * However, NewSessionL is a const function,
     * so we have to use mutable flag to define connection existence.
     */
    mutable TBool iHasConnection;
};

#endif   //  PHONETONESERVER_H

// End of file
