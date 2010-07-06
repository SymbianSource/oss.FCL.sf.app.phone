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
* Description:
*
*/

#include "dialerwidgetengine.h"

#include <logsdomaincrkeys.h>
#include <logsservices.h>

DialerWidgetEngine::DialerWidgetEngine():
    m_missedCalls(-1)
{
}

DialerWidgetEngine::~DialerWidgetEngine()
{
    delete m_settingsManager;
    m_settingsManager = NULL;
}

bool DialerWidgetEngine::initialize()
{
#ifdef Q_OS_SYMBIAN
    m_settingsManager = new XQSettingsManager(this);
    if ( !m_settingsManager ){
        return false;
    }
    XQSettingsKey settingsKey( XQSettingsKey::TargetCentralRepository,
                KCRUidLogs.iUid, KLogsNewMissedCalls );
    
    if ( !connect( m_settingsManager,
                SIGNAL( valueChanged(const XQSettingsKey & ,const QVariant &)),
                    this, SLOT(valueChanged(XQSettingsKey,
                            QVariant)))){
        return false;
    }
    
    if(!m_settingsManager->startMonitoring( settingsKey )){
        return false;
    }
    
#endif
    return true;
}

void DialerWidgetEngine::valueChanged( 
        const XQSettingsKey &key, 
        const QVariant &value )
{
    if ( key.key() == (quint32)KLogsNewMissedCalls ){
        m_missedCalls = value.toInt(); 
        emit missedCallsCountChanged( m_missedCalls );
    }
}

int DialerWidgetEngine::missedCallsCount()
{
    return m_missedCalls;
}
