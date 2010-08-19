/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Easy dialing contac data observer class.
*
*/

#ifndef __MCONTACTDATAMANAGEROBSERVER_H__
#define __MCONTACTDATAMANAGEROBSERVER_H__

/**
 *  MContactDataManagerObserver
 *  Contact data manager observer interface
 */
class MContactDataManagerObserver
    {
public:
    virtual void AllContactDataLoaded() = 0;
    
    virtual void NameOrderChanged() = 0;
    
    virtual void FavouritesChanged() = 0;
    
    virtual void StoreConfigurationChanged() = 0;
    };

#endif //__MCONTACTDATAMANAGEROBSERVER_H__