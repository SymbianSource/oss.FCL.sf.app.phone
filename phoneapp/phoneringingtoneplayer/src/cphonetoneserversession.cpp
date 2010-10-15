/*
* Copyright (c) 2010 - 2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Phone ringing tone server session.
*
*/


// INCLUDE FILES
#include <e32base.h>
#include <utf.h>

#include "phoneconstants.h"
#include "mphonetoneserver.h"
#include "cphonetoneserver.h"
#include "phoneringingtonetrace.h"
#include "cphonetoneserversession.h"
#include "cphonetoneserverplaybackcontroller.h"



// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhoneToneServerSession::CPhoneToneServerSession
// -----------------------------------------------------------------------------
//
CPhoneToneServerSession::CPhoneToneServerSession( const MPhoneToneServer& aServer )
    : iServer( aServer )
    {
    PHONERINGINGTONE_PRINT("CPhoneToneServerSession::CPhoneToneServerSession <");    
    PHONERINGINGTONE_PRINT("CPhoneToneServerSession::CPhoneToneServerSession >");
    }


// -----------------------------------------------------------------------------
// CPhoneToneServerSession::~CPhoneToneServerSession
// -----------------------------------------------------------------------------
//
CPhoneToneServerSession::~CPhoneToneServerSession()
    {
    PHONERINGINGTONE_PRINT("CPhoneToneServerSession::~CPhoneToneServerSession <");
    delete iController;
    iServer.RunDown();
    PHONERINGINGTONE_PRINT("CPhoneToneServerSession::~CPhoneToneServerSession >");    
    }

// -----------------------------------------------------------------------------
// CPhoneToneServerSession::NewL
// -----------------------------------------------------------------------------
//
CPhoneToneServerSession* CPhoneToneServerSession::NewL( const MPhoneToneServer& aServer )
    {
    PHONERINGINGTONE_PRINT("CPhoneToneServerSession::NewL <");
    CPhoneToneServerSession* self = new( ELeave )CPhoneToneServerSession( aServer );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    PHONERINGINGTONE_PRINT("CPhoneToneServerSession::NewL >");
    return self;
    }

// -----------------------------------------------------------------------------
// CPhoneToneServerSession::ConstructL
// -----------------------------------------------------------------------------
//
void CPhoneToneServerSession::ConstructL()
    {        
    PHONERINGINGTONE_PRINT("CPhoneToneServerSession::ConstructL <");
    iController = CPhoneToneServerPlaybackController::NewL();
    PHONERINGINGTONE_PRINT("CPhoneToneServerSession::ConstructL >");
    }    

// -----------------------------------------------------------------------------
// CPhoneToneServerSession::ServiceL
// -----------------------------------------------------------------------------
//
void CPhoneToneServerSession::ServiceL( const RMessage2& aMessage )
    {
    PHONERINGINGTONE_PRINT("CPhoneToneServerSession::ServiceL <");
    TInt volume = aMessage.Int1();
    TProfileRingingType ringingType = ( TProfileRingingType )aMessage.Int2();
    
    switch ( aMessage.Function() )
        {
        
        //S^4 onwards supported types are 
        //EProfileRingingTypeRinging and EProfileRingingTypeSilent
        case EPlayAudioRingTone:
            {
            if ( EProfileRingingTypeSilent == ringingType )
                {
                PHONERINGINGTONE_PRINT("CPhoneToneServerSession::ServiceL - EProfileRingingTypeSilent");
                iController->PlaySilentTone();
                }
            else
                {
                TFileName filename;
                if ( aMessage.GetDesLength( 0 ) < filename.MaxLength() )
                    {
                    // Ignore error. 
                    // Handling is done in audio playing sequence.
                    aMessage.Read( 0, filename );
                    }
                PHONERINGINGTONE_PRINT("CPhoneToneServerSession::ServiceL - playNormalTone");
                iController->PlayNormalToneL( filename, volume, ringingType );
                }
            }
            break;
            
        case EStopPlaying:
            {
            iController->StopTone();
            }
            break;
            
        default:
            break;
        }

    aMessage.Complete( KErrNone );
    PHONERINGINGTONE_PRINT("CPhoneToneServerSession::ServiceL >");    
    }

// End of file
