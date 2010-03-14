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
* Description:  Easy dialing cenrep observer class.
*
*/


#ifndef __MEASYDIALINGCENREPLISTENEROBSERVER_H__
#define __MEASYDIALINGCENREPLISTENEROBSERVER_H__

/**
 * Easy dialing central repository observer.
 */
class MEasyDialingCenrepListenerObserver
    {
public:
    
    
    /**
     * Called when easy dialing feature setting (on / off) value is changed. 
     * @param   aValue  1 if setting is set on, otherwise 0.
     */
    virtual void EasyDialingSettingsChanged(TInt aValue) = 0;
    
    
    /**
     * Called when easy dialing thumbnail setting (shown / not shown) value is changed.
     * @param   aThumbnailSettingValue  1 if shown, otherwise 0.
     */
    virtual void EasyDialingContactThumbnailsSettingsChanged( TInt aThumbnailSettingValue ) = 0;
    };

#endif //__MEASYDIALINGCENREPLISTENEROBSERVER_H__
