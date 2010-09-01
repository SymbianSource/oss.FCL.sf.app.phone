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
* Description:  Class for handling popup query.
*
*/


#ifndef EDCONTACTORPOPUPHANDLER_H
#define EDCONTACTORPOPUPHANDLER_H

#include <e32base.h>

//FORWARD DECLERATIONS
class CAiwServiceHandler;
class CCoeEnv;

/**
 *  CEDContactorPopupHandler
 *  Handling launch of popup query.
 *
 *  @code
 *
 *  @endcode
 *
 *  @lib ccacontactorservice
 *  @since S60 v5.0
 */
NONSHARABLE_CLASS(CEDContactorPopupHandler) :
    public CBase,
    public MAiwNotifyCallback
    {

public:

    /**
     * Two-phased constructor.
     */
    static CEDContactorPopupHandler* NewL();

    /**
     * Destructor.
     */
    virtual ~CEDContactorPopupHandler();

    /**
     * Launch popup.
     *
     * Leaves KErrNotFound if field data not found,
     *        KErrCancel if popup canceled
     *        KErrAbort if red key pressed
     *        KErrGeneral if error occurred
     *
     * @since S60 5.0
     * @param aCommAddressSelectType: Select Type.
     * @param aUseDefaults: Is default functionality enabled.
     */
    TDesC& LaunchPopupL(
        const TDesC8& aContactLinkArray,
        TAiwCommAddressSelectType aCommAddressSelectType,
        TBool aUseDefaults);

    void ClosePopupL();
    
private:

    // from base class MAiwNotifyCallback
    TInt HandleNotifyL(TInt aCmdId, TInt aEventId,
        CAiwGenericParamList& aEventParamList,
        const CAiwGenericParamList& aInParamList);

private:
    /**
     * Constructor.
     */
    CEDContactorPopupHandler();
    void ConstructL();

private: //new methods
    void PrepareResourcesL();
    void ReadFieldDataL(CAiwGenericParamList& aEventParamList);
    void AsyncStop();

private:
    // data

    /**
     * Pointer to AIW service handler
     * Own.
     */
    CAiwServiceHandler* iServiceHandler;

    /**
     * Pointer to scheduler wait.
     * Own.
     */
    CActiveSchedulerWait* iSchedulerWait;

    /**
     * Environment.
     * Not Own.
     */
    CCoeEnv& iCoeEnv;

    /**
     * Offset of loaded resource file.
     * Own.
     */
    TInt iResourceOffset;

    /**
     * Result string from the AIW service.
     * Own.
     */
    HBufC* iQueryResults;

    /**
     * For error handling.
     * Own.
     */
    TInt iError;
    
    };

#endif // EDCONTACTORPOPUPHANDLER_H

// End of File
