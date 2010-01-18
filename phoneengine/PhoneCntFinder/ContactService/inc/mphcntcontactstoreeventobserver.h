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
* Description:  Observer interface for contact store events.
*
*/


#ifndef MPHCNTCONTACTSTOREEVENTOBSERVER_H
#define MPHCNTCONTACTSTOREEVENTOBSERVER_H

/**
 *  Observer interface for contact store events.
 *
 *  @lib PhoneCntFinder.lib
 *  @since S60 v3.2
 */
class MPhCntContactStoreEventObserver
    {

public:

    /**
     * Indication that contact store availability has changed.
     *
     * @since S60 v3.2
     */
    virtual void ContactStoreAvailabilityChanged() = 0;

protected:

    };


#endif // MPHCNTCONTACTSTOREEVENTOBSERVER_H
