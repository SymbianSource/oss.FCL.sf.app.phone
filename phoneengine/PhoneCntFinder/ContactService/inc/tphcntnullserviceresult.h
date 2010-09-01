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
* Description:  Empty service result.
*
*/


#ifndef TPHCNTNULLSERVICERESULT_H
#define TPHCNTNULLSERVICERESULT_H

#include <e32std.h>

#include "mphcntserviceresult.h"

/**
 *  Service result, which does not parse the result.
 *
 *  @lib PhoneCntFinder.lib
 *  @since S60 v3.2
 */
NONSHARABLE_CLASS( TPhCntNullServiceResult ) : public MPhCntServiceResult
    {
public:

    TPhCntNullServiceResult();

// from base class MPhCntServiceResult

    /**
     * From MPhCntServiceResult
     *
     * @since S60 v3.2
     * @see MPhCntServiceResult
     */
    void ParseResult( CAiwGenericParamList& aResult );

    };

#endif // TPHCNTNULLSERVICERESULT_H

