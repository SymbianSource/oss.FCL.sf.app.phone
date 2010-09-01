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
* Description:  Abstract base class for service request param.
*
*/


#ifndef MPHCNTSERVICEREQUESTPARAM_H
#define MPHCNTSERVICEREQUESTPARAM_H

#include <e32std.h>
#include <AiwCommon.hrh>

class CAiwGenericParamList;

/**
 *  Abstract base class for service request param
 *
 *  @lib PhoneCntFinder
 *  @since S60 v3.1
 */
NONSHARABLE_CLASS( MPhCntServiceRequestParam )
    {
public:
    
    virtual ~MPhCntServiceRequestParam() {};
    
    /**
     * Gives the AIW command, that this parameter represents.
     * 
     * @since S60 v3,1
     * @return AIW command.
     */
    virtual TAiwServiceCommands Command() const = 0;

    /**
     * Gives the AIW InParamList
     * 
     * @since S60 v3,1
     * @return AIW param list..
     */
    virtual const CAiwGenericParamList& InParamList() const = 0;

    
    };


#endif // MPHCNTSERVICEREQUESTPARAM_H
