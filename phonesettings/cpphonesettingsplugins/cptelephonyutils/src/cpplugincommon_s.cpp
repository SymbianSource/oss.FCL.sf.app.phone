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

#include "cpplugincommon.h"
#include "cppluginlogging.h"
#include <hbglobal.h>
#include <cppluginplatinterface.h>
#include <QPluginLoader>


CpPluginPlatInterface* Tools::loadCpPlugin(const QString& name)
{
    QPluginLoader loader("C:/ControlPanel/debug/bin/"+name+".dll");
    return qobject_cast<CpPluginPlatInterface*> (loader.instance());
}

bool Tools::unloadCpPlugin(const QString& name)
{
    QPluginLoader loader("C:/ControlPanel/debug/bin/"+name+".dll");
    return loader.unload();
}

bool Tools::voipSupported()
{
    DPRINT << "DUMMY WRAPPER";
    return true;
}

bool Tools::videoSupported()
{
    DPRINT << "DUMMY WRAPPER";
    return true;
}

/*!
  Tools::convertEtelMobileServiceCode.
*/
BasicServiceGroups Tools::convertEtelMobileServiceCode(int serviceCode)
{
    DPRINT << "DUMMY WRAPPER: DEFAULT code";
    return Unknown;
}


/*!
  Tools::errorCodeTextMapping.
 */
bool Tools::errorCodeTextMapping(const int errorcode, QString &errorText)
{
    DPRINT << "DUMMY WRAPPER";
    errorText.clear();
    bool errorTextFound(true);
    errorText = hbTrId("Not done");
    return errorTextFound;
}


CpSettingsWrapper::CpSettingsWrapper(QObject *parent): 
    QObject(parent)
{
    DPRINT << "DUMMY WRAPPER";
}

CpSettingsWrapper::~CpSettingsWrapper()
{
    DPRINT << "DUMMY WRAPPER";
}

bool CpSettingsWrapper::showCallDuration()
{
    DPRINT << "DUMMY WRAPPER";
    return 0;
}

int CpSettingsWrapper::setShowCallDuration(bool value)
{
    DPRINT << "DUMMY WRAPPER: value:" << value;
    return 0;
}

void CpSettingsWrapper::readSoftRejectText(QString &text, bool &userDefined)
{
    DPRINT << "DUMMY WRAPPER: text:" << text << " ,userDefined:" << userDefined;
}

int CpSettingsWrapper::writeSoftRejectText(const QString &text, bool userDefined)
{
    DPRINT << "DUMMY WRAPPER: text:" << text << " ,userDefined:" << userDefined;
    return 0;
}

bool CpSettingsWrapper::numberGroupingSupported() const
{
    DPRINT << "DUMMY WRAPPER";
    return true;
}


bool CpSettingsWrapper::isFeatureCallWaitingDistiquishNotProvisionedEnabled()
{
    bool enabled = false;
    DPRINT << "DUMMY WRAPPER: enabled: " << enabled;
    return enabled;
}


bool CpSettingsWrapper::isPhoneOffline() const
{
    bool enabled = false;
    DPRINT << "DUMMY WRAPPER: enabled: " << enabled;
    return enabled;
}
