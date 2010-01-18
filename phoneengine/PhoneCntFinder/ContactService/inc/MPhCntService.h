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


#ifndef MPHCNTSERVICE_H
#define MPHCNTSERVICE_H

class MPhCntServiceRequestParam;
class MPhCntServiceResult;

/**
 * AIW service wrapper interface.
 *
 * @lib PhoneCntFinder.lib
 * @since S60 v3.1
 */
NONSHARABLE_CLASS( MPhCntService )
    {
public:

    /**
     * Destructor.
     *
     * @since S60 v3.1
     */
    virtual ~MPhCntService() {};
         
    /**
     * Makes AIW service request, synchronous
     *
     * @since S60 v3.1
     * @param aService Parameters used with AIW.
     * @param aResult Result of the request.
     * @leaves KErrCancel - User or client canceled the request.
     *         Other system wide error codes possible.
     */
    virtual void ExecuteRequestL( 
        MPhCntServiceRequestParam& aServiceRequestParams, 
        MPhCntServiceResult& aResult ) = 0;
    
    };

#endif // MPHCNTSERVICE_H