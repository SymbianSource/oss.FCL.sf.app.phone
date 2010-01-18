/*
* Copyright (c) 2003 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Implementation of CCoUtlImplementation.
*
*/



// INCLUDE FILES
#include    "CCoUtlImplementation.h"
#include    "CCoUtlActive.h"
#include    <coemain.h>
#include    <data_caging_path_literals.hrh>

// CONSTANTS
// Path to ConnectUtil engine resource file.
_LIT( KCoUtlResourceFile, "z:connectutilrsc.rsc" );
 
// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CCoUtlImplementation::CCoUtlImplementation
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CCoUtlImplementation::CCoUtlImplementation()
    : iResourceLoader( *CCoeEnv::Static() )
    {
    }

// -----------------------------------------------------------------------------
// CCoUtlImplementation::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CCoUtlImplementation::ConstructL()
    {
    TParse* fp = new (ELeave) TParse();
    fp->Set( KCoUtlResourceFile, &KDC_RESOURCE_FILES_DIR, NULL ); 
    TFileName fileName( fp->FullName() );
    delete fp;
    User::LeaveIfError( iResourceLoader.Open( fileName ) );

    iActive = new (ELeave) CCoUtlActive;
    }

// -----------------------------------------------------------------------------
// CCoUtlImplementation::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCoUtlImplementation* CCoUtlImplementation::NewL()
    {
    if ( !CCoeEnv::Static() )
        {
        // CONE must exist - use only from applications
        User::Leave( KErrNotSupported ); 
        }

    CCoUtlImplementation* self = new( ELeave ) CCoUtlImplementation;
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }

// -----------------------------------------------------------------------------
// CCoUtlImplementation::~CCoUtlImplementation
// Destructor.
// -----------------------------------------------------------------------------
//
CCoUtlImplementation::~CCoUtlImplementation()
    {
    delete iActive;
    iResourceLoader.Close();
    }

// -----------------------------------------------------------------------------
// CCoUtlImplementation::CurrentState
// -----------------------------------------------------------------------------
//
CCoUtlInterface::TState CCoUtlImplementation::CurrentState()
    {
    CCoUtlInterface::TState result = CCoUtlInterface::EIdle;

    switch ( iActive->CurrentState() )
        {
        case CCoUtlActive::EStarted:
        case CCoUtlActive::ECheckIfAttached:
        case CCoUtlActive::ECheckIfNetworkModeIII:
        case CCoUtlActive::EGetConnectionCount:
        case CCoUtlActive::EGoThroughConnections:
        case CCoUtlActive::EGoThroughConnectionsGetBearer:
        case CCoUtlActive::EGoThroughConnectionsGetStatus:
        case CCoUtlActive::ECheckConnectionCount:
        case CCoUtlActive::EGetNameThenConfirmTermination:
        case CCoUtlActive::EConfirmAllConnectionsTermination:
            result = CCoUtlInterface::EConfirm;
            break;

        case CCoUtlActive::ECheckDetachRequired:
        case CCoUtlActive::EStopConnectionsAndDetach:
        case CCoUtlActive::EStopConnectionsAndCheckDetachRequired:
        case CCoUtlActive::EDetach:
            result = CCoUtlInterface::ETerminate;
            break;
 
        case CCoUtlActive::EIdle:
        default:
            break;
        }

    return result;
    }

// -----------------------------------------------------------------------------
// CCoUtlImplementation::Terminate
// -----------------------------------------------------------------------------
//
void CCoUtlImplementation::Terminate( TRequestStatus& aStatus )
    {
    if ( !iActive->IsActive() )
    	{
    	iActive->Start( aStatus );
    	}
    }

// -----------------------------------------------------------------------------
// CCoUtlImplementation::Cancel
// -----------------------------------------------------------------------------
//
void CCoUtlImplementation::Cancel()
    {
    iActive->Cancel();
    }

//  End of File  
