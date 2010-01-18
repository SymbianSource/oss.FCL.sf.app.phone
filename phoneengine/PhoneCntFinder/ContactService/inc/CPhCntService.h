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
* Description:  AIW service wrapper
*
*/

#ifndef CPHCNTSERVICE_H
#define CPHCNTSERVICE_H

#include <e32base.h>
#include <AiwCommon.h>

#include "cphcntasynctosync.h"
#include "MPhCntService.h"
#include "tphcntaiwserviceobserver.h"

class CAiwServiceHandler;
class MPhCntServiceObserver;
class CAiwGenericParamList;

/**
 *  Wrapper for phonebook's AIW service.
 *
 *  @lib PhoneCntFinder.lib
 *  @since S60 v3.1
 */
NONSHARABLE_CLASS( CPhCntService ):     
    public CPhCntAsyncToSync, 
    public MPhCntService,
    public MPhCntAiwServiceCompleteObserver
    {
public:
    
    /**
     * Static constructor.
     *
     * @since S60 v3.1
     */
    static CPhCntService* NewL();
    
    /**
     * Destructor.
     *
     * @since S60 v3.1
     */
    virtual ~CPhCntService();


// from base class MPhCntService
		
    /**
     * From MPhCntService
     *
     * @since S60 v3.1
     * @see MPhCntService
     */
    void ExecuteRequestL( 
        MPhCntServiceRequestParam& aServiceRequestParams, 
        MPhCntServiceResult& aResult ); 

// from base class MPhCntAiwServiceCompleteObserver
    
    /**
     * From MPhCntAiwServiceCompleteObserver.
     *
     * Notification that AIW service request is done.
     *
     * @since S60 v3.2
     * @see MPhCntAiwServiceCompleteObserver
     */
    void ServiceRequestComplete( TInt aError );

protected:

    /**
     * Attach AIW criterias
     */
	void AttachCriteriasL();
	
	/**
	 * Cancels request.
	 * @return Error code.
	 */
	TInt CancelRequest();
	
protected:

    CPhCntService();

    void ConstructL();
    
    /**
     * From CPhCntAsyncToSync.
     *
     * @since S60 v3.2
     * @see CPhCntAsyncToSync.    
     */
    void DoMakeAsyncRequestL();
    
    void SetRequestAndResult( 
        MPhCntServiceRequestParam& aServiceRequestParams, 
        MPhCntServiceResult& aResult );
    
protected: // Protected because of unit testing.

	/**
	 * Result of the request.
	 * Now own.
	 */
	MPhCntServiceResult* iResult;
	
	/**
	 * Request params.
	 * Not own.
	 */
	MPhCntServiceRequestParam* iParams;
	
private: // data

    /**
     * Handle to AIW service
     * Own. 
     */
   	CAiwServiceHandler* iAiwService;
	
	/**
	 * Criteria array.
	 * Own.
	 */
	RCriteriaArray iCriteriaArray;
	
	/**
	 * Options for the command, see TAiwServiceCmdOptions in AiwCommon.hrh.
	 */	
	TUint iCommandOptions;
	
	/**
	 * Indication for that request has been canceled by client.
	 */
	TBool* iClientCanceledRequest;
	
protected:
	
	/**
	 * Observes the AIW service.
	 * Protected to ease unit testing.
	 */
	TPhCntAiwServiceObserver iAiwServiceObserver;

    };

#endif // CPHCNTSERVICE_H
