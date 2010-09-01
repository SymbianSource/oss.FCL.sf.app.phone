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
* Description:  Converts asynchronous operation to synchronous
*
*/


#ifndef CPHCNTASYNCTOSYNC_H
#define CPHCNTASYNCTOSYNC_H

#include <e32base.h>

/**
 *  Converts asynchronous operation to synchronous
 *
 *  @lib PhoneCntFinder
 *  @since S60 v3.1
 */
NONSHARABLE_CLASS( CPhCntAsyncToSync ) : public CBase
    {
protected:
 
    /**
     * Converts asynchronous call from DoMakeAsyncRequestL to synchronous. 
     * Derived class has to implement the DoMakeAsyncRequestL method and make
     * asynchronous call from there.
     *
     * @since S60 v3.1
     */
    TInt MakeAsyncRequest();
     
    /**
     * Indication that asynchronous call from DoMakeAsyncRequestL has 
     * completed. Derived class has to call this method, 
     * when the asynchronous request completes.
     *
     * @since S60 v3.1
     * @param Error code of the response.
     */
    virtual void ResponseReceived( TInt aErrorCode );

    /**
     * Checks if there are pending asynchronous requests available.
     *
     * @since S60 v5.0
     */
    TBool IsActive();
     
    CPhCntAsyncToSync();

    void BaseConstructL();
    
    virtual ~CPhCntAsyncToSync();

    /**
     * Template method, which derived class has to implement. This is the
     * place to make the asynchronous call, which is converted to synchronous.
     *
     * @since S60 v3.1
     */
    virtual void DoMakeAsyncRequestL() = 0;

private: // data
   
    /**
     * To make asynchronous operation synchronous
     * Own.
     */
    CActiveSchedulerWait* iWait;
    
    /**
     * Indication of response received from the async request.
     */
    TBool iResponseReceived;
   
    /**
     * Error code from response.
     */
    TInt iResponseError;
    };


#endif // CPHCNTASYNCTOSYNC_H
