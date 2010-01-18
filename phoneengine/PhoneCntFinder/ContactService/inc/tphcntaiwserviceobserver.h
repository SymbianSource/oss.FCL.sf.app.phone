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


#ifndef TPHCNTAIWSERVICEOBSERVER_H
#define TPHCNTAIWSERVICEOBSERVER_H

#include <e32std.h>

#include <AiwCommon.h>

class MPhCntServiceResult;
class MPhCntAiwServiceCompleteObserver;
class MPhCntServiceRequestParam;

/**
 * Callback to indicate when phonebook's AIW service request
 * is complete.
 *
 * @lib PhoneCngFinder.lib
 * @since S60 3.2
 */
NONSHARABLE_CLASS( MPhCntAiwServiceCompleteObserver )
    {
public:
    
    /**
     * Called when Phonebook's AIW service request is complete.
     * @param aError Error code.
     */
    virtual void ServiceRequestComplete( TInt aError ) = 0;
    
    };

/**
 *  Observes Phonebook AIW services.
 *
 *  @lib PhoneCntFinder.lib
 *  @since S60 3.2
 */
NONSHARABLE_CLASS( TPhCntAiwServiceObserver ) : public MAiwNotifyCallback
    {
public:

    /**
     * Constructor.
     * @param aObserver Callback to indicate when Phonebook's AIW service
     * request is complete.
     */
    TPhCntAiwServiceObserver(MPhCntAiwServiceCompleteObserver& aObserver);
    
    /**
     * Activates observing. When Phonebook AIW service request is complete,
     * aResult will be notfied that request is done and 
     * MPhCntAiwServiceCompleteObserver is notified.
     * 
     * @param aParams The requests parameters that this class is set to 
     * observ.
     * @aResult Result handler.
     */
    void SetRequestAndResult( 
        MPhCntServiceRequestParam* aParams,
        MPhCntServiceResult* aResult
        );
    
// from base class MAiwNotifyCallback
    
    /**
      * From MAiwNotifyCallback
      *
      * @since S60 v3.1
      * @see MAiwNotifyCallback
      */
     TInt HandleNotifyL(
		TInt aCmdId,
		TInt aEventId,
		CAiwGenericParamList& aEventParamList,
		const CAiwGenericParamList& aInParamList);
		
private:
    
    /** 
     * Parses error value from paramlist when KAiwEventError comes
     * from service provider.
     *
     * @since S60 v3.1
     * @parma aEventParamList Event parameters.
     */
	TInt ParseErrorCode( const CAiwGenericParamList& aEventParamList );
	
	/**
	 * Finds first TInt value from param list.
	 * 
	 * @since S60 v3.1
	 * @Returns KErrNotFound if paramter with aParameterId not found.
	 */
	TInt FindFirstTInt32Value( 
	    const CAiwGenericParamList& aEventParamList, 
	    TGenericParamId aParameterId,
	    TInt& aValue ) const;
	
	/**
	 * Indication that response for request has arrived.
	 * @param aError Possible error code.
	 */
	void ResponseReceived( TInt aError );

    /**
     * Checks if current process is Phone application process.
     * @since S60 v3.1
     * @return ETrue - Running in phone app process.
     */	
	TBool RunningInPhoneAppProcess() const;
		
private: 
    
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
	
	/**
	 * Service request observer.
	 * Not own.
	 */
	MPhCntAiwServiceCompleteObserver& iObserver;
	
	/**
	 * State variable indicating that AIW service is shutting down, because
	 * user has pressed end key.
	 */
	TBool iAiwServiceShuttingDown;

    };

    
#endif // TPHCNTAIWSERVICEOBSERVER_H
