/*
* Copyright (c)  Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Definition of MPhCntContactFetcherObserver class.
*
*/


#ifndef MPHCNTCONTACTFETCHEROBSERVER_H
#define MPHCNTCONTACTFETCHEROBSERVER_H

class MVPbkStoreContact;

/**
 *  ?one_line_short_description
 *
 *  ?more_complete_description
 *
 *  @lib ?library
 *  @since S60 ?S60_version *** for example, S60 v3.0
 */
NONSHARABLE_CLASS( MPhCntContactFetcherObserver )
	{
	
public:

    /**
     * aContact is NULL if getting the contact failed.
     *
     * @since S60 ?S60_version
     * @param ?arg1 ?description
     * @param aContactLinks Links to contacts. Client should take a copy from the links, because
     * aContactLinks is not valid after call to this method. 
     * AContactLinks is length may be 0.
     * @return ?description
     */
     virtual void PhoneNumberReceived( const TDesC& aPhoneNumber ) = 0;

    };


#endif // MPHCNTCONTACTFETCHEROBSERVER_H