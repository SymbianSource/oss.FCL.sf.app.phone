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
* Description:  Inteface for fetching contact links from AIW.
*
*/


#ifndef MPHCNTAIWSERVICE_H
#define MPHCNTAIWSERVICE_H

class MPhCntServiceRequestParam;
class MPhCntServiceResult;

NONSHARABLE_CLASS( MPhCntAiwServiceObserver )
    {
public:
    
    /**
     * Indicates that AIW service request is complete.
     * 
     * @since S60 v3.2
     * @param aError Possible error code.
     */
    virtual void RequestComplete( TInt aError ) = 0;
    };

/**
 * AIW service wrapper interface.
 *
 * @lib PhoneCntFinder.lib
 * @since S60 v3.2
 */
NONSHARABLE_CLASS( MPhCntAiwService )
    {
public:

    /**
     * Destructor.
     *
     * @since S60 v3.2
     */
    virtual ~MPhCntAiwService() {};
    
    /**
     * Cancels the possible outstanding request.
     * @since S60 v3.2
     */
    virtual void Cancel() = 0;
    
    /**
     * Determines if request is active.
     * @since S60 v3.2
     */
    virtual TBool IsRequestActive() const = 0;
         
    /**
     * Makes AIW service request
     *
     * @since S60 v3.2
     * @param aService Parameters used with AIW.
     * @param aResult Result of the request.
     * @param aObserver Observer, which is notified when request has completed.
     */
    virtual void ExecuteRequestL( 
        MPhCntServiceRequestParam& aServiceRequestParams, 
        MPhCntServiceResult& aResult,
        MPhCntAiwServiceObserver& aObserver) = 0;
    
    };

#endif // MPHCNTAIWSERVICE_H