/*
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:
*     Singleton class for implementing reconnect query.
*
*/


#ifndef CPHONERECONNECTQUERY_H
#define CPHONERECONNECTQUERY_H

// INCLUDES
#include <mcauireconnectqueryobserver.h>
#include <coemain.h>
#include "mphonetimer.h"

// FORWARD DECLARATIONS
class CPhoneTimer;

// CLASS DECLARATION

class CPhoneReconnectQuery : public CCoeStatic, public MPhoneTimer,
    public MCaUiReconnectQueryObserver
    {
    public:

        /**
         * Returns an instance of this class. When called for the first
         * time, a new instance is created and returned.  After that,
         * calling InstanceL returns the same instance that was created
         * earlier.
         *
         * @return A pointer to a CPhoneReconnectQuery object
         */
        IMPORT_C static CPhoneReconnectQuery* InstanceL();

        /**
        * Destructor.
        */
        virtual ~CPhoneReconnectQuery();

        /**
        * This function is called after on timeout.
        */
        void HandleTimeOutL();

        /**
        * Called when MCaUiReconnectQueryObserver has got a key press.
        *
        * @param aReconType The reconnect choice of the user.
        */
        virtual void OptionSelected( MCaUiReconnectQueryObserver::TCaUiReconType aOption );

        /**
        * Shows reconnect query.
        * External DLL is loaded and request is delegated to
        * the dll.
        * @param aShowVideo - ETrue to show video option, otherwise EFalse
        */
        IMPORT_C void ShowReconnectQueryL( TBool aShowVideo );

        /**
        * Cancels reconnect query timer.
        */
        void CancelReconnectQueryTimer();

        /**
        * Fetches the information is reconnect query displayed on the
        * screen or not.
        * @return ETrue if being displayed otherwise EFalse.
        */
        TBool IsDisplayingQuery();


    private:

        /**
         * Default constructor is private because we are using the
         * singleton design pattern.
         */
        CPhoneReconnectQuery();

        /**
        * Shows number entry and sets number entry CBAs.
        */
        void ShowNumberEntryL();


    private:

        /**
        * This timer delays reconnect query to give time to information note
        */
        CPhoneTimer* iReconQueryTimer;

        /**
        * If video call option is needed in reconnect query this is true
        * otherwise false.
        */
        TBool iIncludeVideo;

        /**
        * ETrue if reconnect query is currently displayed on the screen.
        * EFalse if not.
        */
        TBool iDisplayingQuery;

    };

#endif // CPHONERECONNECTQUERY_H

// End of File
