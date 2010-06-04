/*
 * Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
 *
 */
#include <NetworkHandlingProxy.h>
#include <CNWSession.h>
#include <e32property.h>
#include <centralrepository.h>
#include <ProfileEngineSDKCRKeys.h>
#include <BTSapDomainPSKeys.h>
#include <startupdomainpskeys.h>

#include "cnetworklistener.h"
#include "mnetworklistenerobserver.h"
#include "networkhandlingstarterlogging.h"

// Constants

// 1-minute timeout before showing soft notification
const TInt KNetworkLostTimeout = 60*1000000;

// Offline profile, from ProfileEngineSDKCRKeys.h
const TInt KOfflineProfileId = 5;


// ======== MEMBER FUNCTIONS ========

/*!
    Constructor of CNetworkListener.
 */
CNetworkListener::CNetworkListener(MNetworkListenerObserver& aObserver) : 
    iRegistered(ETrue),
    iObserver(aObserver)
{
    DPRINT << ": IN";
    
    DPRINT << ": OUT";
}

/*!
    Constructor of CNetworkListener.
 */
void CNetworkListener::ConstructL()
{
    DPRINT << ": IN";
    
    //Create network handling engine session.
    iSession = CreateL(*this, iInfo);
    iTimer = CPeriodic::NewL(CActive::EPriorityStandard);
    iProfileApi = CRepository::NewL(KCRUidProfileEngine);
    
    DPRINT << ": OUT";
}

/*!
    Constructor of CNetworkListener.
 */
CNetworkListener* CNetworkListener::NewL(MNetworkListenerObserver& aObserver)
{
    DPRINT << ": IN";
    
    CNetworkListener* self = new(ELeave) CNetworkListener(aObserver);
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    
    DPRINT << ": OUT";
    return self;
 }
    
/*!
    Destructor of CNetworkListener.
 */
CNetworkListener::~CNetworkListener()
{
    DPRINT << ": IN";
    
    delete iSession;
    delete iTimer;
    delete iProfileApi;
    
    DPRINT << ": OUT";
}

/*!
    CNetworkListener::HandleNetworkMessage.
 */
void CNetworkListener::HandleNetworkMessage(const TNWMessages aMessage)
{
    DPRINT << ": IN";
    
    if(aMessage == ENWMessageNetworkRegistrationStatusChange) {
        switch(iInfo.iRegistrationStatus) {
            case ENWNotRegisteredNoService:
                // Fall-through
            case ENWNotRegisteredEmergencyOnly:
                // Fall-through
            case ENWNotRegisteredSearching:
                // Fall-through
            case ENWRegistrationDenied:
                HandleNetworkLost();
                break;
            
            case ENWRegisteredBusy:
                // Fall-through
            case ENWRegisteredOnHomeNetwork:
                // Fall-through
            case ENWRegisteredRoaming:
                HandleNetworkFound();
                break;
            
            case ENWRegistrationUnknown:
                // Take no action
            default:
                break;
        }
    }
    
    DPRINT << ": OUT";
}

/*!
    CNetworkListener::HandleNetworkError.
 */
void CNetworkListener::HandleNetworkError( 
    const TNWOperation /*aOperation*/, TInt /*aErrorCode*/)
    {
    DPRINT << ": IN";
    }

/*!
    CNetworkListener::NWLostDelayCallBack.
 */
TInt CNetworkListener::NWLostDelayCallBack(TAny* aParam)
{
    DPRINT << ": IN";
    
    CNetworkListener* self = 
        reinterpret_cast< CNetworkListener* >( aParam );
    
    if ( self ) {
        self->iTimer->Cancel();
        self->iObserver.ShowNote();
    }
    
    DPRINT << ": OUT";
    return KErrNone;
}
    
/*!
    CNetworkListener::HandleNetworkFound
 */
void CNetworkListener::HandleNetworkFound()
{
    DPRINT << ": IN";
    
    if (!iRegistered) {
        iTimer->Cancel();
        iObserver.RemoveNote();
    }
    iRegistered = ETrue;
    
    DPRINT << ": OUT";
}

/*!
    CNetworkListener::HandleNetworkLost
 */
void CNetworkListener::HandleNetworkLost()
{
    DPRINT << ": IN";
    
    if (iInfo.iSelectionSetting == ENWNetworkSelectionManual) {
        // See if we were registered before
        if (iRegistered) {
            iRegistered = EFalse;

            if (!IsOffLineMode() &&
                !IsBluetoothSAPConnected() &&
                IsSimOk()) {
                // Start 60 second timer and when expired show the
                // notifier
                iTimer->Cancel();
                iTimer->Start(KNetworkLostTimeout,
                              KNetworkLostTimeout,
                              TCallBack( NWLostDelayCallBack, this ));
             }
        }
    }
    
    DPRINT << ": OUT";
}

/*!
    CNetworkListener::IsOffLineMode
 */
TBool CNetworkListener::IsOffLineMode() const
{
    DPRINT << ": IN";
    
    TInt profileId;
    TInt err = iProfileApi->Get(KProEngActiveProfile, profileId);
    
    DPRINT << ": OUT";
    
    return profileId == KOfflineProfileId && err == KErrNone;
    }
    
/*!
    CNetworkListener::IsBluetoothSAPConnected
 */
TBool CNetworkListener::IsBluetoothSAPConnected() const
{
    DPRINT << ": IN";
    
    TInt btSapState( EBTSapNotConnected );
    TInt err = RProperty::Get(KPSUidBluetoothSapConnectionState,
                              KBTSapConnectionState,
                              btSapState );
    
    DPRINT << ": OUT";
    
    return btSapState != EBTSapNotConnected && err == KErrNone;
}
    
/*!
    CNetworkListener::IsSimOk
 */
TBool CNetworkListener::IsSimOk() const
{
    DPRINT << ": IN";
    
    TInt simState(ESimStatusUninitialized);
    TInt err = RProperty::Get(KPSUidStartup,
                              KPSSimStatus,
                              simState );
    
    DPRINT << ": OUT";
    
    return simState == ESimUsable && err == KErrNone;
}

// End of file
    
