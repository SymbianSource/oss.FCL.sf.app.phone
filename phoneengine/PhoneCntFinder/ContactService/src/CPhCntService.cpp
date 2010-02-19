/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Aiw service wrapper.
*
*/


#include "CPhCntService.h"
#include <AiwServiceHandler.h>
#include <AiwGenericParam.hrh>
#include <AiwCommon.hrh>
#include <AiwContactSelectionDataTypes.h>
#include "mphcntservicerequestparam.h"
#include "mphcntserviceresult.h"

_LIT8( KContentType ,EGenericParamContactItemStr );

// ---------------------------------------------------------------------------
// Static constructor
// ---------------------------------------------------------------------------
//
CPhCntService* CPhCntService::NewL() 
	{
	CPhCntService* self = 
	    new( ELeave )CPhCntService();
	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop( self );
	return self;
	}

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//	
CPhCntService::~CPhCntService() 
	{
	CancelRequest();
	if( iAiwService ) 
	    {
        iAiwService->Reset();	    
	    }
	iCriteriaArray.ResetAndDestroy();
	iCriteriaArray.Close();
	
	delete iAiwService;
	}
	
// ---------------------------------------------------------------------------
// Attach AIW criterias
// ---------------------------------------------------------------------------
//
void CPhCntService::AttachCriteriasL() 
	{
	// Note take array to member variable and detach from aiw when destructing.
	CAiwCriteriaItem* selectCriteria = 
		CAiwCriteriaItem::NewLC( KAiwCmdSelect, KAiwCmdSelect, KContentType ); 
    TUid uid;
	uid.iUid = KAiwClassBase;
	selectCriteria->SetServiceClass( uid );
	selectCriteria->SetMaxProviders( 1 );
	iCriteriaArray.AppendL( selectCriteria );
	CleanupStack::Pop( selectCriteria );
		
    CAiwCriteriaItem* assignCriteria = 
        CAiwCriteriaItem::NewLC( KAiwCmdAssign, KAiwCmdAssign, KContentType );
	
	uid.iUid = KAiwClassMenu;
	assignCriteria->SetServiceClass( uid );
	assignCriteria->SetMaxProviders( 1 );
	
	
	iCriteriaArray.AppendL( assignCriteria  );
	CleanupStack::Pop( assignCriteria );
	iAiwService->AttachL( iCriteriaArray ); 
	}

        
// ---------------------------------------------------------------------------
// Cancels request if request is outstanding.
// ---------------------------------------------------------------------------
//    
TInt CPhCntService::CancelRequest()
    {
    TInt err( KErrNone );
    if ( iParams )
    	{
    	if( iClientCanceledRequest )
    	    {
    	    *iClientCanceledRequest = ETrue;
    	    }
    	// Cancel old requests
	    iCommandOptions |= KAiwOptCancel;
	    // Cancel is synchronous and we have request ongoing so
	    // do not use base class MakeAsyncRequest, because it uses
	    // CActiveSchedulerWait to make synchronisation and the 
	    // wait scheduler is already waiting. If we would call the 
	    // wait scheduler again, it would crash.
    	TRAP( err, DoMakeAsyncRequestL() );
        iCommandOptions = 0; // Reset the command options
    	}
    return err;
    }

// ---------------------------------------------------------------------------
// From MPhCntService
// Executes AIW service rewuest.
// ---------------------------------------------------------------------------
//	
void CPhCntService::ExecuteRequestL( 
    MPhCntServiceRequestParam& aParams,
    MPhCntServiceResult& aResult )
    {
    if( !IsActive() )
        {
        // Destructor can indicate canceling of request using 
        // iClientCanceledRequest.
        TBool requestCanceledByClient = EFalse;
        iClientCanceledRequest = &requestCanceledByClient;
        
        SetRequestAndResult( aParams, aResult );
        TInt err( KErrNone );
        TRAPD( applicationShuttingdown, err = MakeAsyncRequest() );
        // If application using phonecntfinder is ended from task switcher, then
        // underlying CActiveSchedulerWait::Wait leaves. This needs to be
        // trapped because we are using clients aParams and aResult, which 
        // are stack based objects. 
        if( applicationShuttingdown )
            {
            // Cancel possible outstanding request
           	    CancelRequest();
           	    iResult = NULL;
                iParams = NULL;
           	    User::Leave( applicationShuttingdown );
           	    }
           
            if( !requestCanceledByClient )
                {
                iResult = NULL;
                iParams = NULL;
                }
            else
                {
                err = KErrCancel;
                }
    
        User::LeaveIfError( err );
        }
    }


// ---------------------------------------------------------------------------
// From MPhCntAiwServiceCompleteObserver
// Notifies link fetch observer that links are fetched.
// ---------------------------------------------------------------------------
//    
void CPhCntService::ServiceRequestComplete( TInt aError )
    {
    ResponseReceived( aError );
    }
	
void CPhCntService::ConstructL() 
	{
    BaseConstructL();
	iAiwService = CAiwServiceHandler::NewL();
	AttachCriteriasL();

	}
	
CPhCntService::CPhCntService() : 
    iAiwServiceObserver( *this )
	{
	}
	
// ---------------------------------------------------------------------------
// From CPhCntAsyncToSync
// Makes the actual aiw request.
// ---------------------------------------------------------------------------
//
void CPhCntService::DoMakeAsyncRequestL()  
    {
    iAiwService->ExecuteServiceCmdL( 
        iParams->Command(), 
        iParams->InParamList(), 
        iAiwService->OutParamListL(), 
        iCommandOptions, 
        &iAiwServiceObserver );
    }

// ---------------------------------------------------------------------------
// Sets variables
// ---------------------------------------------------------------------------
//    
void CPhCntService::SetRequestAndResult( 
    MPhCntServiceRequestParam& aServiceRequestParams, 
    MPhCntServiceResult& aResult )
    {
    iResult = &aResult;
   	iParams = &aServiceRequestParams;
   	iAiwServiceObserver.SetRequestAndResult( iParams, iResult );
    }
    
