/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This file contains the header of CenRep Listener class.
*
*/


#ifndef CSPCENREPLISTENER_H
#define CSPCENREPLISTENER_H

// INCLUDE FILES
#include <e32base.h>

// Forward declarations
class CRepository;
class MCSPCenRepObserver;

// BASE CLASS

// CLASS DECLARATION

/**
*   CSPCenRepListener manages getting notification on CenRep key changes.
*/
class CSPCenRepListener : public CActive
    {
    public:
        
        /**
        * Two-phased construction.
        *
        * @param aUid cenrep Uid
        * @param aKey key to be listened
        * @param aObserver observer for key change.
        * @return instance of the class
        */
        static CSPCenRepListener* NewL( TUid aUid, TUint32 aKey, 
                   MCSPCenRepObserver* aObserver );

        /**
        * Destructor.
        */
        virtual ~CSPCenRepListener();

    public:

        /**
        * Gets the current value of the monitored setting
        * @param aValue the current value of the monitored setting
        * @return TInt Symbian OS error code from central repository
        */
        TInt Get( TInt& aValue );


    protected: // From CActive

        void DoCancel();
        TInt RunError( TInt aError );
        void RunL();
        
    private:

        /**
        * Constructor
        * @param aUid cenrep Uid
        * @param aKey key to be listened
        * @param aObserver observer for key change.
        */    
        CSPCenRepListener( TUid aUid, TUint32 aKey, 
                           MCSPCenRepObserver* aObserver );
    

        /**
        * Private constructing.
        */
        void ConstructL();

        /**
        * Submits a notification request
        */
        void SubmitNotifyRequestL();
        

    private: // Owned by this object

        /**
        * UID of CR key.
        */
        TUid iUid;
        
        /**
        * Repository access.
        * Own.
        */
        CRepository* iRepository;
        
        /**
        * Identification number of the monitored setting.
        */
        TUint32 iMonitorSetting;
        
        MCSPCenRepObserver* iObserver;
};

#endif // CSPCENREPLISTENER_H
