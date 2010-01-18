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
* Description:  Contact match strategy interface.
*
*/


#ifndef MPHCNTCONTACTMATCHSTRATEGY_H
#define MPHCNTCONTACTMATCHSTRATEGY_H

#include <e32std.h>

/**
 * Interface for finding contacts.
 *
 *  @lib PhoneCntFinder.lib
 *  @since S60 v3.1
 */
NONSHARABLE_CLASS( MPhCntContactMatchStrategy ) 
    {

public:

    virtual ~MPhCntContactMatchStrategy() {};

    /**
     * Starts finding contacts, which contact field matches with aMatchString.
     *
     * @since S60 v3.1
     * @param aMatchString String which is matched against contact fields.
     */
    virtual void FindMatchesL( const TDesC& aMatchString ) = 0;

    };

#endif // MPHCNTCONTACTMATCHSTRATEGY_H
