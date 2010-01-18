/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Bubble extension interface.
*
*/


#ifndef TELBUBBLEEXTENSIONINTERFACE_H
#define TELBUBBLEEXTENSIONINTERFACE_H

#include <e32base.h>

class MTelBubbleExtensionData;

class MTelBubbleExtensionInterface
    {
public:

    /**
     * Notifies that a new customizable bubble is available.
     *
     * This function is called when a new bubbleheader is created. Plugin
     * should initiate a new customization instance inside this method and
     * store the reference to the call data.
     *
     * @param aCallData Reference to call data.
     */
    virtual void StartCustomizedBubble(
            MTelBubbleExtensionData& aCallData ) = 0;

    /**
     * Notifies that a customizable bubble has been removed
     *
     * This function is called when a bubbleheader is going to be deleted.
     * Plugin should detach all customizations related to this call and
     * delete the local datas related to this call.
     *
     * @param aCallData Reference to call data.
     */
    virtual void StopCustomizedBubble(
            MTelBubbleExtensionData& aCallData ) = 0;

    /**
     * Indicates that currently bubble drawing is started.
     * Customizing is preferred to be done during this call.
     *
     * This function is called when the call bubbles are being updated.
     * The call datas are updated before this function is called. Plugin should
     * check whether any of the calls are changed so that change to the
     * customized element is needed and make the change. Doing the
     * changes during this method will prevent excess flickering and
     * redraws and therefore is suggested.
     */
    virtual void BubbleUpdating() = 0;

    };

#endif // TELBUBBLEEXTENSIONINTERFACE_H

// end of file
