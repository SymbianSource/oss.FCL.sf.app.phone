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
* Description:  Interface for call data access.
*
*/


#ifndef TELBUBBLEEXTENSIONDATA_H
#define TELBUBBLEEXTENSIONDATA_H

#include <e32base.h>

class MTelBubbleExtensionData
    {
public:
    // Specifies call types    
    enum TCallType
        {
        EUninitialized = 0,
        ECsVoice = 1,  // Circuit switched voice call
        ECsVideo = 2, // Multimedia/Video call
        EPsVoice = 3 // Voice over IP call
        };
        
    // Specifies call states
    enum TCallState
        {
        ENone           = 0, // Default on creation
        EOnHold         = 1,
        EDisconnected   = 2,
        EActive         = 3,
        EIncoming       = 4,
        EWaiting        = 5,
        EAlertToDisconnected = 6,
        EOutgoing       = 7,
        EAlerting       = 8,
        };
        
public:

    /**
     * Bubble ID this call is connected to.
     * @return Bubble ID.
     */
    virtual TInt BubbleId() const = 0;

    /**
     * Call state information.
     * @return Call state.
     */
    virtual TCallState State() const = 0;
    
    /**
     * Call type information
     * @return Call type.
     */
    virtual TCallType Type() const = 0;
    
    /**
     * Call service id
     * @return Call service
     */
    virtual TUint32 ServiceId() const = 0;
    
    /**
     * Contact Link related to this call.
     * @return Reference to Contact Link
     */
    virtual const TDesC8& ContactLink() const = 0;
    
    /**
     * Remote party number related to this call.
     * @return Reference to remote party number.
     */
    virtual const TDesC& RemotePhoneNumber() const = 0;
    };

#endif // TELBUBBLEEXTENSIONDATA_H

// end of file
