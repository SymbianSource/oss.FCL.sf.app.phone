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
#include <ut_cnetworklistener.h>
#include <qtestmains60.h>
#include <cnetworklistener.h>
#include <NetworkHandlingProxy.h>
#include <CNWSession.h>
#include <NWHandlingEngine.h>

TNWInfo* PoiterNWInfo;

CNWSession* CreateL( MNWMessageObserver& /*aMessage*/, TNWInfo& aTNWInfo )
{
    PoiterNWInfo = &aTNWInfo;
    aTNWInfo.iSelectionSetting = ENWNetworkSelectionManual;
    aTNWInfo.iRegistrationStatus = ENWRegistrationDenied;
    CNWSession* nullPointer(NULL);
    return nullPointer;
}

/*!
  ut_cnetworklistener::ut_cnetworklistener
 */
ut_cnetworklistener::ut_cnetworklistener() 
    : m_networkListener(0)
{
}

/*!
  ut_cnetworklistener::~ut_cnetworklistener
 */
ut_cnetworklistener::~ut_cnetworklistener()
{
    delete m_networkListener;
}

/*!
  ut_cnetworklistener::init
 */
void ut_cnetworklistener::init()
{
    initialize();
    
    m_networkListener = CNetworkListener::NewL(*this);
    
    QVERIFY(verify());
}

/*!
  ut_cnetworklistener::cleanup
 */
void ut_cnetworklistener::cleanup()
{
    delete m_networkListener;
    m_networkListener = 0;
    
    reset();
}

/*!
    From MNetworkListenerObserver
 */
void ut_cnetworklistener::ShowNote()
{
}

/*!
    From MNetworkListenerObserver
 */
void ut_cnetworklistener::RemoveNote()
{
}

/*!
  ut_cnetworklistener::t_HandleNetworkMessage
 */
void ut_cnetworklistener::t_HandleNetworkMessage()
{
    m_networkListener->HandleNetworkMessage(MNWMessageObserver::ENWMessageNetworkRegistrationStatusChange);
    
    PoiterNWInfo->iRegistrationStatus = ENWNotRegisteredNoService;
    m_networkListener->HandleNetworkMessage(MNWMessageObserver::ENWMessageNetworkRegistrationStatusChange);
    
    PoiterNWInfo->iRegistrationStatus = ENWNotRegisteredEmergencyOnly;
    m_networkListener->HandleNetworkMessage(MNWMessageObserver::ENWMessageNetworkRegistrationStatusChange);
    PoiterNWInfo->iRegistrationStatus = ENWNotRegisteredSearching;
    m_networkListener->HandleNetworkMessage(MNWMessageObserver::ENWMessageNetworkRegistrationStatusChange);
    
    PoiterNWInfo->iRegistrationStatus = ENWRegisteredBusy;
    m_networkListener->HandleNetworkMessage(MNWMessageObserver::ENWMessageNetworkRegistrationStatusChange);
    
    PoiterNWInfo->iRegistrationStatus = ENWRegisteredOnHomeNetwork;
    m_networkListener->HandleNetworkMessage(MNWMessageObserver::ENWMessageNetworkRegistrationStatusChange);
    
    PoiterNWInfo->iRegistrationStatus = ENWRegisteredRoaming;
    m_networkListener->HandleNetworkMessage(MNWMessageObserver::ENWMessageNetworkRegistrationStatusChange);
    
    PoiterNWInfo->iRegistrationStatus = ENWRegistrationUnknown;
    m_networkListener->HandleNetworkMessage(MNWMessageObserver::ENWMessageNetworkRegistrationStatusChange);
    
    PoiterNWInfo->iSelectionSetting = ENWNetworkSelectionAutomatic;
    PoiterNWInfo->iRegistrationStatus = ENWNotRegisteredNoService;
    m_networkListener->HandleNetworkMessage(MNWMessageObserver::ENWMessageNetworkRegistrationStatusChange);
    PoiterNWInfo->iRegistrationStatus = ENWRegisteredOnHomeNetwork;
    m_networkListener->HandleNetworkMessage(MNWMessageObserver::ENWMessageNetworkRegistrationStatusChange);
    
    m_networkListener->HandleNetworkMessage(MNWMessageObserver::ENWMessageNetworkModeChange);
    
    PoiterNWInfo->iRegistrationStatus = (TNWRegistrationStatus)123456789;
    m_networkListener->HandleNetworkMessage(MNWMessageObserver::ENWMessageNetworkRegistrationStatusChange);  
}

/*!
  ut_cnetworklistener::t_HandleNetworkError
 */
void ut_cnetworklistener::t_HandleNetworkError()
{
    m_networkListener->HandleNetworkError( 
        MNWMessageObserver::ENWGetServiceProviderName, KErrGeneral );
}

/*!
  ut_cnetworklistener::t_NWLostDelayCallBack
 */
void ut_cnetworklistener::t_NWLostDelayCallBack()
{
    TAny* pointer(NULL); 
    m_networkListener->NWLostDelayCallBack(pointer);
    
    CNetworkListener* networklistener = CNetworkListener::NewL(*this);; 
    m_networkListener->NWLostDelayCallBack( networklistener );
    delete networklistener;
}

QTEST_MAIN_S60(ut_cnetworklistener)
