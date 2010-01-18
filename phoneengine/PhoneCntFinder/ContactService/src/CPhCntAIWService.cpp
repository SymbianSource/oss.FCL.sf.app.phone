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


#include "CPhCntAIWService.h"
#include <AiwServiceHandler.h>
#include <AiwGenericParam.hrh>
#include <AiwCommon.hrh>
#include <AiwContactSelectionDataTypes.h>
#include "mphcntservicerequestparam.h"
#include "mphcntserviceresult.h"

// ---------------------------------------------------------------------------
// Static constructor
// ---------------------------------------------------------------------------
//
CPhCntAiwService* CPhCntAiwService::NewL() 
	{
	CPhCntAiwService* self = 
	    new( ELeave )CPhCntAiwService();
	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop( self );
	return self;
	}

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//	
CPhCntAiwService::~CPhCntAiwService() 
	{
	}

// ---------------------------------------------------------------------------
// Cancels request if request is outstanding.
// ---------------------------------------------------------------------------
//    
void CPhCntAiwService::Cancel()
    {
    if ( IsRequestActive() )
    	{
    	CancelRequest();
    	iObserver = NULL;
        iResult = NULL;
        iParams = NULL;
    	}
    }

// ---------------------------------------------------------------------------
// Response received from AIW.
// ---------------------------------------------------------------------------
//   
void CPhCntAiwService::ResponseReceived( 
    TInt aError )
    {
    if( iObserver )
        {
        iObserver->RequestComplete( aError );
        }
    iObserver = NULL;
    iResult = NULL;
    iParams = NULL;
    }

// ---------------------------------------------------------------------------
// Indicates if request is active.
// ---------------------------------------------------------------------------
//  
TBool CPhCntAiwService::IsRequestActive() const
    {
    return iObserver && iResult && iParams;
    }

// ---------------------------------------------------------------------------
// From MPhCntService
// Executes AIW service request.
// ---------------------------------------------------------------------------
//
void CPhCntAiwService::ExecuteRequestL( 
    MPhCntServiceRequestParam& aServiceRequestParams, 
    MPhCntServiceResult& aResult,
    MPhCntAiwServiceObserver& aObserver)
    {
    CancelRequest();
    iObserver = &aObserver;
    SetRequestAndResult( aServiceRequestParams,aResult );
   	DoMakeAsyncRequestL();
    }

// ---------------------------------------------------------------------------
// Second-phase constructor
// ---------------------------------------------------------------------------
//
void CPhCntAiwService::ConstructL() 
	{
	CPhCntService::ConstructL();
	}

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//	
CPhCntAiwService::CPhCntAiwService() :
    CPhCntService()
	{
	}
	

     
