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

#ifndef CPHCNTAIWSERVICE_H
#define CPHCNTAIWSERVICE_H

#include <e32base.h>
#include <AiwCommon.h>

#include "CPhCntService.h"
#include "mphcntaiwservice.h"

/**
 *  Wrapper for phonebook's AIW service.
 *
 *  @lib PhoneCntFinder.lib
 *  @since S60 v3.2
 */
NONSHARABLE_CLASS( CPhCntAiwService ):     
    public CPhCntService, 
    public MPhCntAiwService
    {
public:
    
    /**
     * Static constructor.
     *
     * @since S60 v3.2
     */
    static CPhCntAiwService* NewL();
    
    /**
     * Destructor.
     *
     * @since S60 v3.2
     */
    virtual ~CPhCntAiwService();

// from base class MPhCntService
    
    /**
     * From MPhCntService
     *
     * @since S60 v3.2
     * @see MPhCntService
     */  
    void ExecuteRequestL( 
        MPhCntServiceRequestParam& aServiceRequestParams, 
        MPhCntServiceResult& aResult,
        MPhCntAiwServiceObserver& aObserver );       
	    
	/**
     * From MPhCntAiwService
     *
     * @since S60 v3.2
     * @see MPhCntService
     */  
	void Cancel();
	
	/** 
	 * Indicates if service request is ongoing.
	 */
	TBool IsRequestActive() const;	

protected:

	/**
	 * Indication that MAiwNotifyCallback is called.
	 */
	void ResponseReceived( TInt aError );
	
protected:

    CPhCntAiwService();

    void ConstructL();
   	
    /**
     * Observer 
     * Not own.  
     */
	MPhCntAiwServiceObserver* iObserver;

    };

#endif // CPHCNTAIWSERVICE_H
