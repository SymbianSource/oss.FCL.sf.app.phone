/*
* Copyright (c) 2007-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Observer for Phonebook AIW services.
*
*/


#include "tphcntaiwserviceobserver.h"
#include "mphcntservicerequestparam.h"
#include "mphcntserviceresult.h"

// Phonebook2 AIW service constants
const TInt KServiceTermiationAllowed = 0; 
const TInt KProcessTerminationAllowed = 1;

// Phone Process Uid.
const TInt KPhoneProcess = 0x100058B3;
const TBool KDoNotAllowProcessTermination = EFalse;
const TBool KAllowProcessTermination = ETrue;

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
TPhCntAiwServiceObserver::TPhCntAiwServiceObserver(
    MPhCntAiwServiceCompleteObserver& aObserver) : 
    iResult( NULL ),
    iParams( NULL ),
    iObserver( aObserver ),
    iAiwServiceShuttingDown( EFalse )
    {
    }
   
// ---------------------------------------------------------------------------
// Sets params and result
// ---------------------------------------------------------------------------
// 
void TPhCntAiwServiceObserver::SetRequestAndResult( 
    MPhCntServiceRequestParam* aParams,
    MPhCntServiceResult* aResult )
    {
    iParams = aParams;
    iResult = aResult;
    }

// ---------------------------------------------------------------------------
// From MAiwNotifyCallback
// Handles the Phonebooks AIW callbacks.
// ---------------------------------------------------------------------------
//
TInt TPhCntAiwServiceObserver::HandleNotifyL(
    TInt aCmdId,
    TInt aEventId,
    CAiwGenericParamList& aEventParamList,
    const CAiwGenericParamList& /*aInParamList*/ )
	{
	TInt response = KErrNone;
	if( iResult && iParams )
	    {
	    if( aCmdId == iParams->Command() ) 
	        {
	        switch( aEventId )
	            {
	            // User canceled the service.
	            case KAiwEventCanceled:
	                // if AIW service is shutting down (ie user has pressed end key), 
	                // do not indicate that response has received yet, because
	                // we still have to wait KAiwEventQueryExit.
	                if( !iAiwServiceShuttingDown )
	                    {
	                    ResponseReceived( KErrCancel );
	                    }
	                break;
	            
	            // Service provider had an internal error.
	            case KAiwEventError:
	                ResponseReceived( ParseErrorCode( aEventParamList ) );
	                break;
	                
	            // Service provider stopped from some reason.
	            case KAiwEventStopped:
	                ResponseReceived( KErrGeneral );
	                break;
	            
	            case KAiwEventCompleted:
	                iResult->ParseResult( aEventParamList );
	                ResponseReceived( KErrNone );
	                break;
	                
	            // We are not interested to check the client selection.
	            case KAiwEventOutParamCheck: 
	                response = ETrue;
	                break;
	                
	                
	            // QUERY EXIT is three phase operation.
	            // First time when KAiwEventQueryExit comes, Phonebook AIW service
	            // provider is asking if service is allowed to be terminated (KServiceTermiationAllowed).
	            // After that service request is completed with event KAiwEventCanceled.
	            // Last phase is KAiwEventQueryExit event with query to allow process termination 
	            // (KProcessTerminationAllowed).
	            case KAiwEventQueryExit:

	                TInt queryType;
                    if( !FindFirstTInt32Value( 
	                        aEventParamList, EGenericParamApplication, queryType ) )
                        {
                        if( queryType == KServiceTermiationAllowed )
                            { 
                            // AIW service is always allowed to exit. After this
                            // AIW service provider cancels the outstanding
                            // request with event KAiwEventCanceled.
                            response = iAiwServiceShuttingDown = ETrue;
                            }
                        else if( queryType == KProcessTerminationAllowed )
                            {
                            if ( RunningInPhoneAppProcess() )
                                {
                                // Phone app process is not allowed to be 
                                // terminated.
                                response = KDoNotAllowProcessTermination;    
                                }
                            else
                                {
                                response = KAllowProcessTermination;
                                }                          
                            ResponseReceived( KErrCancel );
                            }
                        }
                    break;
	              
	            // Other events are not handled and we wait for
	            // next event.
                default:
                    break;
	            }           
	        }
	    }
	    
	return response;
	}
	
// ---------------------------------------------------------------------------
// Takes error code from generic param list.
// ---------------------------------------------------------------------------
//	
TInt TPhCntAiwServiceObserver::ParseErrorCode( 
    const CAiwGenericParamList& aEventParamList )
    {
    TInt errorValue = KErrGeneral;
    FindFirstTInt32Value( aEventParamList, EGenericParamError, errorValue );
    return errorValue;
    }

// ---------------------------------------------------------------------------
// Takes integer value from aEventParamList.
// ---------------------------------------------------------------------------
//	    
TInt TPhCntAiwServiceObserver::FindFirstTInt32Value( 
    const CAiwGenericParamList& aEventParamList, 
    TGenericParamId aParameterId,
    TInt& aValue ) const
    {
    TInt err( KErrNotFound );
    TInt index( 0 );
    const TAiwGenericParam* param =
        aEventParamList.FindFirst( index, aParameterId );
    if ( index != KErrNotFound && param )
        {
        aValue = param->Value().AsTInt32();
        err = KErrNone;
        }
    return err;
    }

// ---------------------------------------------------------------------------
// Notifies observer.
// ---------------------------------------------------------------------------
//
void TPhCntAiwServiceObserver::ResponseReceived( TInt aError )
    {
    iObserver.ServiceRequestComplete( aError );
    iResult = NULL;
    iParams = NULL;
    iAiwServiceShuttingDown = EFalse;
    }
    
// ---------------------------------------------------------------------------
// Checks if running in phone process. The handling of KAiwEventQueryExit
// is done differently if running in phone process.
// ---------------------------------------------------------------------------
//
TBool TPhCntAiwServiceObserver::RunningInPhoneAppProcess() const
    {
    RProcess currentProcess;
	TBool phoneProcess( EFalse );
	TUid processID = currentProcess.Identity();
	if ( processID.iUid == KPhoneProcess )
        {
        phoneProcess = ETrue;    
        }
    return phoneProcess;
    }
    
