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
* Description:  Observer interface for receiving contact.
*
*/


#ifndef MPHCNTCONTACTFETCHOBSERVER_H
#define MPHCNTCONTACTFETCHOBSERVER_H

class CPhCntContact;

/**
 *  Observer interface for receiving contact.
 *
 *  @lib PhoneCntFinder.lib
 *  @since S60 v3,1
 */
NONSHARABLE_CLASS( MPhCntContactFetchObserver ) 
	{
	
public:

    /**
     * Indication that contact has been received.
     * 
     * aContact is NULL if getting the contact failed.
     *
     * @since S60 v3.1
     * @param aContact Received contact. It is null, if there was an error
     * during reception of the contact. Ownership is given to observer.
     * @param aError Error code. 
     */
     virtual void ContactReceived( CPhCntContact* aContact, TInt aError ) = 0;

    };


#endif // MPHCNTCONTACTFECTHOBSERVER_H