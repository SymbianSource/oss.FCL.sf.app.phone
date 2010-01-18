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
* Description:  Base class for service results.
*
*/


#ifndef MPHCNTSERVICERESULT_H
#define MPHCNTSERVICERESULT_H

class CAiwGenericParamList;

/**
 *  Interface of the MPhCntService results.
 *
 *  @lib PhoneCntFinder.lib
 *  @since S60 v3.2
 */
class MPhCntServiceResult 
    {
public:
    /**
     * Result from MPhCntService request.
     *
     * @since S60 v3.2
     * @param aResult Result of the service request.
     */
    virtual void ParseResult( 
        CAiwGenericParamList& aResult ) = 0;
    };


#endif // MPHCNTSERVICERESULT_H
