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
* Description:  Contains call data.
*
*/


#ifndef C_PHONEBUBBLEEXTENSIONDATA_H
#define C_PHONEBUBBLEEXTENSIONDATA_H

#include <telbubbleextensiondata.h>

class TPhoneCmdParamCallHeaderData;

/**
 * Call data.
 * All data that plugins need to know about a call is collected here.
 * Note that the data is changed to reflect changed call info before plugin
 * is notified about changes, so plugin needs to save the current information
 * if it needs it for comparison or otherwise.
 *
 *  @lib ?library
 *  @since S60 5.0
 */
class CPhoneBubbleExtensionData : public CBase, public MTelBubbleExtensionData
    {
public:

    /**
     * Two-phased constructor.
     * 
     * @param aBubbleId Bubble Id this data relates to.
     * @param aParams Phoneapp parameters
     * @param aFirst Is this the first call. Default yes.
     */
    static CPhoneBubbleExtensionData* NewL(
            TInt aBubbleId,
            TPhoneCmdParamCallHeaderData* aParams,
            TBool aFirst = ETrue );

    /**
     * Two-phased constructor.
     * 
     * @param aBubbleId Bubble Id this data relates to.
     * @param aParams Phoneapp parameters
     * @param aFirst Is this the first call. Default yes.
     */
    static CPhoneBubbleExtensionData* NewLC( 
            TInt aBubbleId,
            TPhoneCmdParamCallHeaderData* aParams,
            TBool aFirst = ETrue );

    /**
    * Destructor.
    */
    virtual ~CPhoneBubbleExtensionData();

    /**
     * (Re)sets the call state
     *
     * @since S60 5.0
     * @param aState New state (TPEState, not bubble state)
     * @param aFirst Is this the first call. Default yes.
     */
    void SetState( TInt aState, TBool aFirst = ETrue );

// from base class MTelBubbleExtensionData

    /**
     * Bubble ID this call is connected to.
     * @return Bubble ID.
     */
    TInt BubbleId() const;

    /**
     * Call state information.
     * @return Call state.
     */
    TCallState State() const;

    /**
     * Call type information
     * @return Call type.
     */
    TCallType Type() const;

    /**
     * Call service id
     * @return Call service
     */
    TUint32 ServiceId() const;

    /**
     * Contact Link related to this call.
     * @return Reference to Contact Link
     */
    const TDesC8& ContactLink() const;

    /**
     * Remote phone number for this call.
     * @return Remote phone number
     */
    const TDesC& RemotePhoneNumber() const;
private:

    /**
     * First phase constructor.
     * 
     * @param aBubbleId Bubble Id this data relates to.
     * @param aParams Phoneapp parameters
     * @param aFirst Is this the first call
     */
    CPhoneBubbleExtensionData( 
            TInt aBubbleId,
            TPhoneCmdParamCallHeaderData* aParams, 
            TBool aFirst );

    /**
     * Second phase constructor.
     * 
     * @param aParams Phoneapp parameters
     * @param aFirst Is this the first call
     */
    void ConstructL( TPhoneCmdParamCallHeaderData* aParams );

private: // data

    /**
     * Bubble ID
     */
    TInt iBubbleId;

    /**
     * Call state
     */
    TCallState iState;

    /**
     * Call type
     */
    TCallType iType;

    /**
     * Service id
     */
    TUint32 iService;

    /**
     * Contact Link
     * Owned.
     */
    HBufC8* iContactLink;

    /**
     * Remote phone number
     * Owned.
     */
    HBufC* iRemotePhoneNumber;
    };


#endif // C_PHONEBUBBLEEXTENSIONDATA_H
