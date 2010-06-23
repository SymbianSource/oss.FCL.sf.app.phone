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
* Description: Adjusts visibility of Phone app in device lock situations.
*
*/
#include <eikenv.h>
#include <xqserviceutil.h>
#include <telinformationpskeys.h>
#include <coreapplicationuisdomainpskeys.h>
#include <apgtask.h>
#include <ccallinformation.h>
#include <ccallinfoiter.h>
#include "phonevisibilityhandler.h"
#include "cphonepubsubproxy.h"
#include "phoneconstants.h"
#include "phoneuiqtviewif.h"
#include "qtphonelog.h"

/*!
    PhoneVisibilityHandler::PhoneVisibilityHandler.
 */
PhoneVisibilityHandler::PhoneVisibilityHandler(PhoneUIQtViewIF &view, QObject *parent)
    : 
    QObject(parent),
    m_view(view),
    m_eikonEnv(CEikonEnv::Static()),
    m_carModeEnabled(false),
    m_hideDeviceDialogs(false),
    m_deviceLockEnabled(false),
    m_carModeSubscriber(0),
    m_carModePublisher(0)
{
    PHONE_TRACE;
    sendToBackground(false); // Send phone to background

    m_carModePublisher = new QValueSpacePublisher("/phone",this);
    m_carModePublisher->setValue(QString("/carmode"),QVariant(false));
    
    m_carModeSubscriber = new QValueSpaceSubscriber("/phone/carmode", this);
    connect(m_carModeSubscriber, SIGNAL(contentsChanged()), this, SLOT(carModeChanged()));
    m_carModeEnabled = m_carModeSubscriber->value().toBool();

/*    
    // Define car mode pub sub key
    int err = RProperty::Define(
        KPSUidTelCarMode,
        KTelCarMode,
        RProperty::EInt,
        KPhoneReadPolicy,
        KPhoneWritePolicy);
    
    // Start listening to car mode changes
    if(err == KErrNone) {
        QT_TRAP_THROWING(CPhonePubSubProxy::Instance()->NotifyChangeL(
                KPSUidTelCarMode, KTelCarMode, this));
    }
*/    
    int autlockValue = CPhonePubSubProxy::Instance()->Value(
                KPSUidCoreApplicationUIs, KCoreAppUIsAutolockStatus);
    m_deviceLockEnabled = (EAutolockOff < autlockValue);

    
    // Start listening to device lock changes
    QT_TRAP_THROWING(CPhonePubSubProxy::Instance()->NotifyChangeL(
            KPSUidCoreApplicationUIs, KCoreAppUIsAutolockStatus, this));
}


/*!
    PhoneVisibilityHandler::~PhoneVisibilityHandler.
 */
PhoneVisibilityHandler::~PhoneVisibilityHandler()
{
    PHONE_TRACE;
    CPhonePubSubProxy::Instance()->CancelAllNotifications(this);
}

/*!
    PhoneVisibilityHandler::bringToForeground.
 */
void PhoneVisibilityHandler::bringToForeground()
{
    PHONE_TRACE2("m_carModeEnabled=", m_carModeEnabled);

    if(!m_carModeEnabled) {
        m_view.bringToForeground();
        adjustVisibility(BringForwards);
    }
    

}

/*!
    PhoneVisibilityHandler::hideDeviceDialogs.
 */
void PhoneVisibilityHandler::hideDeviceDialogs(bool hide)
{
    PHONE_TRACE2(": hide =", hide);
    m_hideDeviceDialogs = hide;
    adjustVisibility(KeepCurrentPos);
}

/*!
    PhoneVisibilityHandler::ordinalPosition.
 */
int PhoneVisibilityHandler::ordinalPosition()
{
    return m_eikonEnv->RootWin().OrdinalPosition();
}

/*!
    PhoneVisibilityHandler::sendToBackground.
 */
void PhoneVisibilityHandler::sendToBackground(bool homeScreenForeground)
{
    PHONE_TRACE2(": homeScreenForeground =", homeScreenForeground);

    // Send phone back on WSERV stack
    adjustVisibility(SendToBack);
    
    // Fetch homescreen to foreground if needed
    if (homeScreenForeground) {
        _LIT(KPhoneHsAppName,"hsapplication");
        TApaTaskList taskList(m_eikonEnv->WsSession());
        TApaTask task = taskList.FindApp(KPhoneHsAppName);
        task.BringToForeground();
    }
}

/*!
    PhoneVisibilityHandler::HandlePropertyChangedL.
 */
void PhoneVisibilityHandler::HandlePropertyChangedL(const TUid& aCategory, 
    const TUint aKey, const TInt aValue)
{
    if((aCategory == KPSUidCoreApplicationUIs) &&
            (aKey == KCoreAppUIsAutolockStatus)) {
        // Adjust visibility according mode
        m_deviceLockEnabled = (EAutolockOff < aValue);
        PHONE_TRACE2(": m_deviceLockEnabled=", m_deviceLockEnabled);
        adjustVisibility(KeepCurrentPos);

    }
}

/*!
    PhoneVisibilityHandler::adjustVisibility.
	 -1 Ordinal position is lowest ( not visible )
	  0 Ordinal position is highest ( visible )
	  1 - ... Ordinal postition under one or more window group
 */
void PhoneVisibilityHandler::adjustVisibility(AdjustAction action)
{
    PHONE_TRACE1(": START");
    int ordinalPos = m_eikonEnv->RootWin().OrdinalPosition();
    PHONE_TRACE2(": current pos:", ordinalPos);

    if (m_carModeEnabled || (action == SendToBack)) {
        PHONE_TRACE1(": SendPhoneToBackground");
        m_eikonEnv->RootWin().SetOrdinalPosition(-1, ECoeWinPriorityNeverAtFront);
        
    } else if ((KeepCurrentPos == action) &&
        (m_eikonEnv->RootWin().OrdinalPriority() == ECoeWinPriorityNeverAtFront)) {
        // Skip situations where phone is put to back
        // and action is not to bring it up
        // Execution must come here if there is no calls
        PHONE_TRACE1(": Skip");
        
    } else if (m_hideDeviceDialogs) {
        PHONE_TRACE1(": Hide dialogs");
        m_eikonEnv->RootWin().SetOrdinalPosition(0, ECoeWinPriorityAlwaysAtFront + 1);
    
    } else if (m_deviceLockEnabled) {
        // critical notes are allowed to show on top of Phone application
        PHONE_TRACE1(": Devicelock");
        m_eikonEnv->RootWin().SetOrdinalPosition(0, ECoeWinPriorityAlwaysAtFront);
    
    } else if (BringForwards == action) {
        // Try to show phone with normal priority
        PHONE_TRACE1(": Bring forward");
        m_eikonEnv->RootWin().SetOrdinalPosition(0, ECoeWinPriorityNormal);
    
    } else {
        // Normalize visiblity after ie. device lock
        PHONE_TRACE1(": Normalize");
        m_eikonEnv->RootWin().SetOrdinalPosition(ordinalPos, ECoeWinPriorityNormal);
        
    }

    PHONE_TRACE1(": END");
}

void PhoneVisibilityHandler::carModeChanged()
{
    PHONE_TRACE;
    
    m_carModeEnabled = m_carModeSubscriber->value().toBool();

    CCallInformation* callInfos = CCallInformation::NewL();
    Q_ASSERT(callInfos != 0);
    int amountOfCalls = callInfos->GetCallsL().Count();

    if(!m_carModeEnabled && (amountOfCalls > 0)) {
        bringToForeground();
    }
    
    delete callInfos;

    // Adjust visibility according mode (TODO!)
    PHONE_TRACE2(": m_carModeEnabled=", m_carModeEnabled);
    //adjustVisibility(KeepCurrentPos);	
}
