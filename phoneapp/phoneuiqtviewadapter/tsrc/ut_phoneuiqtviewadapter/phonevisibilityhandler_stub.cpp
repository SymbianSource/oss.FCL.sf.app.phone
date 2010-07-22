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
#include <xqcallinfo.h>
#include <MediatorDomainUIDs.h>
#include <callinformation.h>
#include <eikenv.h>
#include <xqserviceutil.h>
#include <devicelockaccessapi.h>
#include <qdebug.h>
#include <telinformationpskeys.h>
#include <coreapplicationuisdomainpskeys.h>
#include <apgtask.h>
#include "phonevisibilityhandler.h"
#include "cphonepubsubproxy.h"
#include "phoneconstants.h"
#include "phoneuiqtviewif.h"

bool m_bringToForegroundCalled;
bool m_sendToBackgroundCalled;
bool m_ordinalPositionCalled;

/*!
    PhoneVisibilityHandler::PhoneVisibilityHandler.
 */
PhoneVisibilityHandler::PhoneVisibilityHandler(PhoneUIQtViewIF &view, QObject *parent)
    : 
    QObject(parent),
    m_view(view),
    m_eikonEnv(0),
    m_carModeEnabled(false),
    m_hideDeviceDialogs(false)
{

}


/*!
    PhoneVisibilityHandler::~PhoneVisibilityHandler.
 */
PhoneVisibilityHandler::~PhoneVisibilityHandler()
{

}

/*!
    PhoneVisibilityHandler::bringToForeground.
 */
void PhoneVisibilityHandler::bringToForeground()
{
    m_bringToForegroundCalled = true;
}

/*!
    PhoneVisibilityHandler::sendToBackground.
 */
void PhoneVisibilityHandler::sendToBackground(bool homeScreenForeground)
{
    m_sendToBackgroundCalled = true;
}

/*!
    PhoneVisibilityHandler::hideDeviceDialogs.
 */
void PhoneVisibilityHandler::hideDeviceDialogs(bool hide)
{
    Q_UNUSED(hide)
}

/*!
    PhoneVisibilityHandler::ordinalPosition.
 */
bool PhoneVisibilityHandler::phoneVisible()
{
    m_ordinalPositionCalled = true;
    return false;
}

/*!
    PhoneVisibilityHandler::HandlePropertyChangedL.
 */
void PhoneVisibilityHandler::HandlePropertyChangedL(const TUid& aCategory, 
    const TUint aKey, const TInt aValue)
{
    Q_UNUSED(aCategory)
    Q_UNUSED(aKey)
    Q_UNUSED(aValue)
}

/*!
    PhoneVisibilityHandler::adjustVisibility.
 */
void PhoneVisibilityHandler::adjustVisibility(AdjustAction action)
{
    Q_UNUSED(action)
}

/*!
    PhoneVisibilityHandler::carModeChanged.
 */
void PhoneVisibilityHandler::carModeChanged()
{

}

