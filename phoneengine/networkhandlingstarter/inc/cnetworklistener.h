/*
* Copyright (c) 2009-2010 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Network info listener.
*
*/


#ifndef CNETWORKLISTENER_H
#define CNETWORKLISTENER_H

// System includes
#include <e32base.h>
#include <nwhandlingengine.h>
#include <qsysteminfo.h>

// User includes

// Forward declarations
class CNWSession;
class MNetworkListenerObserver;

using namespace QtMobility;

/**
 *  Network info listener.
 */
NONSHARABLE_CLASS( CNetworkListener ) : public CBase, 
    public MNWMessageObserver
    {
public:

    /**
     * Get pointer to network info listener.
     * @return pointer to network info listener.
     */
    static CNetworkListener* NewL( MNetworkListenerObserver& aObserver );
    
    ~CNetworkListener();

    // from base class MNWMessageObserver

    /**
     * From MNWMessageObserver.
     * Called by network handling engine when network info changes.
     * @param aMessage is type of the change.
     */
    void HandleNetworkMessage( const TNWMessages aMessage );

    /**
     * From MNWMessageObserver.
     * Called fi network handling engine fails.
     * @param aOperation is failed operation.
     * @param aErrorCode is fail reason.
     */
    void HandleNetworkError( const TNWOperation aOperation, TInt aErrorCode );
    
    static TInt NWLostDelayCallBack(TAny* aParam);

private:
    
    CNetworkListener( MNetworkListenerObserver& aObserver );
    
    void ConstructL();
    
    void HandleNetworkFound();
     
    void HandleNetworkLost();
    
    /**
     * Tests if offline mode is engaged.
     * @return ETrue if offline mode is currently active
     */
    TBool IsOffLineMode() const;
    
    /**
     * Tests if Bluetooth SAP is in connected mode.
     * @return ETrue if Bluetooth SAP is active.
     */
    TBool IsBluetoothSAPConnected() const;
    
    /**
     * Tests if SIM is in OK state.
     * @return ETrue if SIM is in OK state.
     */
    TBool IsSimOk() const;
    
private: // data

    /**
     * Session to network handling engine.
     * Own.
     */
    CNWSession* iSession;

    /**
     * Cached network info structure.
     */
    TNWInfo iInfo;
    
    /**
     * Timer to delay the showing of the notification if needed
     * Own
     */
    CPeriodic *iTimer;
    
    /**
     * A true value if registered to network.
     */
    TBool iRegistered;
    
    /**
     * Observer.
     */
    MNetworkListenerObserver& iObserver;

    /**
     * System device info API.
     * Own.
     */
    QSystemDeviceInfo* iDeviceInfo;
    };
    
#endif // CNETWORKLISTENER_H
