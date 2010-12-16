/*
* Copyright (c) 2008-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This file contains the implementation of CPEServiceHandling class
*
*/


#include <mpephonemodelinternal.h>
#include <mpedatastore.h>
#include <pevirtualengine.h>
#include <e32debug.h>
#include <talogger.h>

#include "cpeservicehandling.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CPEServiceHandling::CPEServiceHandling( MPEPhoneModelInternal& aModel )
    : iModel( aModel )
    {
    TEFLOGSTRING( KTAOBJECT, "PE CPEServiceHandling::CPEServiceHandling" );
    }

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
void CPEServiceHandling::ConstructL()
    {
    TEFLOGSTRING( KTAOBJECT, "PE CPEServiceHandling::ConstructL" );
    }

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
EXPORT_C CPEServiceHandling* CPEServiceHandling::NewL( MPEPhoneModelInternal& aModel )
    {
    CPEServiceHandling* self = CPEServiceHandling::NewLC(aModel);
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
EXPORT_C CPEServiceHandling* CPEServiceHandling::NewLC( MPEPhoneModelInternal& aModel )
    {
    CPEServiceHandling* self = new( ELeave ) CPEServiceHandling( aModel );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
EXPORT_C CPEServiceHandling::~CPEServiceHandling()
    {
    TEFLOGSTRING( KTAOBJECT, "PE CPEServiceHandling::~CPEServiceHandling" );
    delete iCchClient;
    }

// ---------------------------------------------------------------------------
// CPEServiceHandling::EnableService
// ---------------------------------------------------------------------------
//
void CPEServiceHandling::EnableServiceL( TInt aServiceId )
	{
    TEFLOGSTRING( KTAREQIN, "PE CPEServiceHandling::EnableServiceL" );
	
    if ( !iCchClient )
        {
        iCchClient = CCch::NewL();
        }

	CCchService* service = iCchClient->GetService( aServiceId );
	
	TInt error( KErrNotFound );
	if( service )
		{
		iCurrentServiceId = aServiceId;
		
		TCchServiceStatus serviceStatus;
 		error = service->GetStatus( ECCHVoIPSub, serviceStatus );
		TCCHSubserviceState state = serviceStatus.State();
	
		if( error == KErrNone )
		    {
		    if ( serviceStatus.Error() == KErrNone )
		        {
		        error = EnableServiceIfNeeded( state, *service );
		        }
		    else
		        {
		        TEFLOGSTRING2( KTAERROR,
		                "PE CPEServiceHandling::EnableServiceL, error: %d"
		                , serviceStatus.Error() );
		        SendErrorMessage( serviceStatus.Error());
		        }
    		}
		}
	
	if ( error != KErrNone )
	    {
	    iCurrentServiceId = KErrNotFound;
	    if ( error == KErrNotFound )
	         {
	         iModel.SendMessage( MEngineMonitor::EPEMessageNoService );
	         }
	     else
	         {
	         TEFLOGSTRING2( KTAERROR,
	                  "PE CPEServiceHandling::EnableServiceL, Error: %d"
	                  , error );
	         SendErrorMessage( error );
             service->RemoveObserver( *this );	
	         }
	    }
	
	TEFLOGSTRING2( KTAINT, 
            "PE CPEServiceHandling::EnableServiceL, error: %d", error );		
	}

// ---------------------------------------------------------------------------
// CPEServiceHandling::EnableServiceIfNeeded
// ---------------------------------------------------------------------------
//
TInt CPEServiceHandling::EnableServiceIfNeeded( 
        const TCCHSubserviceState& aState, 
        CCchService& aService )
    {
    TEFLOGSTRING( KTAINT, "PE CPEServiceHandling::EnableServiceIfNeeded" );
    TInt error = KErrNone;
    
    TEFLOGSTRING2( KTAINT, 
            "PE CPEServiceHandling::EnableServiceIfNeeded, aState: %d", aState );
    switch ( aState )
        {
        case ECCHEnabled:
            {
            iCurrentServiceId = KErrNotFound;
            iModel.SendMessage( MEngineMonitor::EPEMessageServiceEnabled );
            }
            break;
        case ECCHUninitialized:
        case ECCHDisabled:
        case ECCHConnecting:
            {
            // Temporary solution, it will be fixed as soon as possible. 
            // Message have to send before enable is called. Reason is so that progress bar 
            //(global note) doesn't hide Networks's "Connection Needed" global note.
            iModel.SendMessage( MEngineMonitor::EPEMessageServiceEnabling );
            aService.AddObserver( *this );
            error = aService.Enable( ECCHUnknown );  
            }
            break;
        case ECCHDisconnecting:
            {
            iCurrentServiceId = KErrNotFound;
            error = KErrNotFound;
            }
            break;
        default:
            break;
        }
    TEFLOGSTRING2( KTAINT, 
            "PE CPEServiceHandling::EnableServiceIfNeeded, error: %d", error );
    return error;
    }
    
// ---------------------------------------------------------------------------
// CPEServiceHandling::ServiceStatusChanged
// ---------------------------------------------------------------------------
//
void CPEServiceHandling::ServiceStatusChanged(
    TInt aServiceId,
	const TCCHSubserviceType aType,
	const TCchServiceStatus& aServiceStatus )
	{
	TEFLOGSTRING( KTAINT, "PE CPEServiceHandling::ServiceStatusChanged <" );
	
	if( aServiceId == iCurrentServiceId && aType == ECCHVoIPSub )
	    {
    	TEFLOGSTRING3( KTAINT,
    	        "PE CPEServiceHandling::ServiceStatusChanged, state: %d, error: %d"
    	        , aServiceStatus.State()
    	        , aServiceStatus.Error() );

       	CCchService* service = iCchClient->GetService( aServiceId );
       	if( service )
       	    {
    	    if( aServiceStatus.Error() != KErrNone )
    	        {
    	        TEFLOGSTRING( KTAERROR, 
    	                "PE CPEServiceHandling::ServiceStatusChanged, error" );
    	        SendErrorMessage( aServiceStatus.Error());
    	        CancelServiceEnabling();
    	        }
    	    else 
    	        {
    	        if( aServiceStatus.State() == ECCHEnabled )
    	            {
                    TEFLOGSTRING( KTAINT, 
                             "PE CPEServiceHandling::ServiceStatusChanged, enabled" );
                    iModel.SendMessage( MEngineMonitor::EPEMessageServiceEnabled );
                    // Let's set current service if to not found to avoid
                    // disabling service after this since it has now been enabled
                    iCurrentServiceId = KErrNotFound;
    	            }

    	        // Notify UI, that service is disabled.
    	        else if( aServiceStatus.State() == ECCHDisabled ) 
                     {
                     TEFLOGSTRING( KTAERROR, 
                              "PE CPEServiceHandling::ServiceStatusChanged, disabled" );
                     iCurrentServiceId = KErrNotFound;
                     iModel.SendMessage( MEngineMonitor::EPEMessageServiceDisabled );
                     }
    	        }
    	    
    	    // don't remove observer, if state are connecting or disconnecting
    	    if ( aServiceStatus.State() != ECCHConnecting && 
    	         aServiceStatus.State() != ECCHDisconnecting )
    	        {
    	        service->RemoveObserver( *this );
    	        }
       	    }//if( service )
       	else
       	    {
            TEFLOGSTRING( KTAERROR, 
                    "PE CPEServiceHandling::ServiceStatusChanged, no service" );
       	    }
	    }
	TEFLOGSTRING( KTAINT, "PE CPEServiceHandling::ServiceStatusChanged >" );
	}
	
// ---------------------------------------------------------------------------
// CPEServiceHandling::CancelServiceEnabling
// ---------------------------------------------------------------------------
//
void CPEServiceHandling::CancelServiceEnabling() const
    {
    TEFLOGSTRING( KTAREQIN, "PE CPEServiceHandling::CancelServiceEnabling" );
    
    if ( iCchClient && iCurrentServiceId != KErrNotFound )
        {
        CCchService* service = iCchClient->GetService( iCurrentServiceId );
        
        if ( service )
            {
            TCchServiceStatus serviceStatus;
            TInt error = service->GetStatus( ECCHVoIPSub, serviceStatus );
            TCCHSubserviceState state = serviceStatus.State();
            if ( error == KErrNone )
                {
                TEFLOGSTRING( KTAREQOUT,
                  "PE CPEServiceHandling::CancelServiceEnabling, CCchService->Disable" );
                service->Disable( ECCHUnknown );
                }
            }
        }
    }
		
// -----------------------------------------------------------------------------
// CPEServiceHandling::SendErrorMessage
// -----------------------------------------------------------------------------
//
void CPEServiceHandling::SendErrorMessage(
        TInt aErrorCode )
    {
    TEFLOGSTRING2( KTAINT, 
        "PE CPEServiceHandling::SendErrorMessage, aErrorCode: %d", 
        aErrorCode );

    iModel.DataStore()->SetErrorCode( aErrorCode );
    iModel.SendMessage( MEngineMonitor::EPEMessageServiceHandlingError );
    }

// -----------------------------------------------------------------------------
// CPEServiceHandling::DisableService
// -----------------------------------------------------------------------------
//
void CPEServiceHandling::DisableService() const
    {
    TEFLOGSTRING( KTAREQIN, "PE CPEServiceHandling::DisableService" );

    CCchService* service = iCchClient->GetService( iCurrentServiceId );
    
    if ( service )
        {
        TEFLOGSTRING( KTAREQOUT,
          "PE CPEServiceHandling::DisableService, CCchService->Disable" );
        service->Disable( ECCHUnknown );
        }
    }

//  End of File
