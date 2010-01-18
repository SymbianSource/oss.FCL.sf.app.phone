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
* Description:  Result of getting user selected contact link.
*
*/


#ifndef TPhCntContactLinkResult_H
#define TPhCntContactLinkResult_H

#include <e32std.h>

#include "mphcntserviceresult.h"

/**
 *  Result of the request to get the user selected phone numbers link.
 *
 *  @lib PhoneCntFinder.lib
 *  @since S60 v3.2
 */
NONSHARABLE_CLASS( TPhCntContactLinkResult )
    : public MPhCntServiceResult
    {
    
public: 

    /**
     * Constructor.
     *
     * @since S60 v3.2
     * @param aLink Descriptor where the contact link is set (may be null) when
     * MPhCntService request completes. 
     */
    TPhCntContactLinkResult( HBufC8*& aLink );
    
// from base class MPhCntServiceResult

    /**
     * From MPhCntServiceResult
     * Parses the result.
     *
     * @since S60 v3.2
     * @see MPhCntServiceResult
     */
    void ParseResult( 
        CAiwGenericParamList& aResult );

private: // data

    /**
     * Link
     * Now own.
     */
    HBufC8*& iLink;

    };

#endif // TPhCntContactLinkResult_H
