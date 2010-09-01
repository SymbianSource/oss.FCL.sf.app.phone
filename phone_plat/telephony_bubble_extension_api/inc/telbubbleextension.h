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
* Description:  Base class for extension plug-in.
*
*/


#ifndef TELBUBBLEEXTENSION_H
#define TELBUBBLEEXTENSION_H

// INCLUDES
#include <e32base.h>
#include "telbubbleextensioninterface.h"

// CONSTANTS
// Constants used in ECOM implementation
const TUid KTelBubbleExtensionInterfaceUid    = { 0x102078F6 };

class MTelBubbleExtensionObserver;
class MTelBubbleExtensionData;

class CTelBubbleExtension : public CBase,
                            public MTelBubbleExtensionInterface
    {
public:
    /**
    * Two phase constructor
    *
    * @leave Uses Leave code KErrNotFound if implementation is not found.
    * @param aImplementationUid Implementation UID of the plugin to be
    *        created.
    */
    static CTelBubbleExtension* NewL(
        TUid aImplementationUid );

    /** Destructor */
    virtual inline ~CTelBubbleExtension();

    /**
     * Initialize plug-in. Called after NewL
     * @param aBubbles Interface for bubbles
     */
    virtual void InitializeL( MTelBubbleExtensionObserver& aBubbles ) = 0;

// from MTelBubbleExtensionInterface

    /**
     * Notifies that a new customizable bubble is available
     *
     * @param aCallData Reference to call data.
     */
    virtual void StartCustomizedBubble(
            MTelBubbleExtensionData& aCallData ) = 0;

    /**
     * Notifies that a customizable bubble has been removed
     *
     * @param aCallData Reference to call data.
     */
    virtual void StopCustomizedBubble(
            MTelBubbleExtensionData& aCallData ) = 0;

    /**
     * Indicates that currently bubble drawing is started.
     * Customizing is preferred to be done during this call.
     */
    virtual void BubbleUpdating() = 0;

private: // data

    /**
     * ECOM plugin instance UID.
     */
    TUid iDtor_ID_Key;
    };

#include "telbubbleextension.inl"

#endif // TELBUBBLEEXTENSION_H

// end of file
