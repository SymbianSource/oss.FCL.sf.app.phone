/*!
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
* Description:  Handles phone indicators.
*
*/

#include "phoneindicatorcontroller.h"
#include "phoneindicators.h"

#include <QList>
#include <QPair>

#ifdef Q_OS_SYMBIAN
#include <logsmodel.h>
#include <logsfilter.h>
#include <logsdomaincrkeys.h>
#include <hbicon.h>
#endif

PhoneIndicatorController::PhoneIndicatorController(QObject *parent):
    QObject(parent), m_logsModel(0), m_missedCallsFilter(0)
{
#ifdef Q_OS_SYMBIAN
    m_setManager = new XQSettingsManager(this);
 
    XQSettingsKey settingsKey( XQSettingsKey::TargetCentralRepository, 
            KCRUidLogs.iUid, KLogsNewMissedCalls );
    
    bool ok = connect( m_setManager, 
            SIGNAL( valueChanged(const XQSettingsKey & ,const QVariant &)),
                this, SLOT(updateMissedCallIndicator(XQSettingsKey, 
                        QVariant)));
    ASSERT( ok );
    ok = false;
    ok = m_setManager->startMonitoring( settingsKey );
    ASSERT(ok);
    
    qRegisterMetaType<PhoneIndicatorInfo>("PhoneIndicatorInfo");

#endif
}

PhoneIndicatorController::~PhoneIndicatorController()
{   
#ifdef Q_OS_SYMBIAN
    XQPublishAndSubscribeSettingsKey missedCallCountKey(
                    (qint32)KCRUidLogs.iUid, (quint32)KLogsNewMissedCalls );
    m_setManager->stopMonitoring(missedCallCountKey);
    delete m_setManager;
    delete m_missedCallsFilter;
    delete m_logsModel;
#endif
}

void PhoneIndicatorController::setActiveCallData( 
        const TDesC &text, const TDesC &icon )
{
    m_cli = QString::fromUtf16 (text.Ptr (), text.Length ());
    m_callImage = QString::fromUtf16 (icon.Ptr (), icon.Length ());
}

void PhoneIndicatorController::clearActiveCallData()
{
    m_cli.clear();
    m_callImage.clear();
    disableActiveCallIndicator();
}

void PhoneIndicatorController::enableActiveCallIndicator()
{
    if (!m_cli.isEmpty()){
        QString indicatorType(indicatorName(PhoneActiveCallIndicator));
        QVariantMap parameters;
        
        parameters.insert(
                QVariant(HbIndicatorInterface::PrimaryTextRole ).toString(),
                m_cli);
        
        parameters.insert(QVariant( HbIndicatorInterface::DecorationNameRole).toString(),
                m_callImage);
        
        m_indicator.activate(indicatorType, parameters);
    }
}
void PhoneIndicatorController::disableActiveCallIndicator()
{
    QString indicatorType(indicatorName(PhoneActiveCallIndicator));
    m_indicator.deactivate(indicatorType);
}

void PhoneIndicatorController::updateMissedCallIndicator(
        const XQSettingsKey &key, const QVariant &value)
{ 
#ifdef Q_OS_SYMBIAN
    if ( key.key() == (quint32)KLogsNewMissedCalls ){
        if ( value.toInt() == 0 ){
            QString indicatorType(indicatorName(PhoneMissedCallIndicator));
            m_indicator.deactivate(indicatorType);
            delete m_missedCallsFilter;
            m_missedCallsFilter = NULL;
            delete m_logsModel;
            m_logsModel = NULL;
        } else {
            m_logsModel = new LogsModel(LogsModel::LogsFullModel);
            m_missedCallsFilter = new LogsFilter(LogsFilter::Missed);
            connect( m_missedCallsFilter, 
                    SIGNAL(rowsInserted(const QModelIndex &, int, int )),
                    this, SLOT(setMissedallIndicatorData()));
            m_missedCallsFilter->setMaxSize(value.toInt() + 1);
            m_missedCallsFilter->setSourceModel(m_logsModel);
        }
    }
#endif  
}

void PhoneIndicatorController::setMissedallIndicatorData()
{
#ifdef Q_OS_SYMBIAN

    XQSettingsKey settingsKey( XQSettingsKey::TargetCentralRepository, 
            KCRUidLogs.iUid, KLogsNewMissedCalls );
    QString indicatorType(indicatorName(PhoneMissedCallIndicator));
    
    int missedCallCount = m_setManager->readItemValue( settingsKey,
            XQSettingsManager::TypeInt ).toInt();
    
    if ( missedCallCount > 0 ) {
        QVariantMap parameters;
        QString lastMissedCallFrom;
        int repeatedMissedCalls(0);
        
        if ( m_missedCallsFilter->rowCount() > 0 ){
                lastMissedCallFrom = m_missedCallsFilter->data(
                        m_missedCallsFilter->index(0,0),
                        Qt::DisplayRole).toStringList().takeFirst();
                repeatedMissedCalls = 1;
            }
        for( int i = 1; i < missedCallCount && i 
                < m_missedCallsFilter->rowCount(); i++ ){
            QStringList displayData = m_missedCallsFilter->data(
                    m_missedCallsFilter->index(i,0),
                    Qt::DisplayRole).toStringList();
            if ( lastMissedCallFrom == displayData.at(0) ){
                repeatedMissedCalls++;
            } else {
                i = missedCallCount; // Break
            }
        }
        // First row
        parameters.insert( 
                QVariant( HbIndicatorInterface::PrimaryTextRole ).toString(),
                ( hbTrId("txt_phone_dblist_ln_missed_calls", 
                        missedCallCount)));
        
        // Second row
        if ( missedCallCount >= repeatedMissedCalls )
            {
            parameters.insert( 
                    QVariant( HbIndicatorInterface::SecondaryTextRole ).toString(), 
                    lastMissedCallFrom );
            }
        
        // icon
        //QList<QVariant> icons = m_missedCallsFilter->data( 
        //        m_missedCallsFilter->index(0,0), 
        //        Qt::DecorationRole).value<QList<QVariant> >();
        //QString iconName = icons.first().value<HbIcon>().iconName();
        QString iconName = "qtg_mono_missed_call_unseen";
        parameters.insert(
                QVariant( HbIndicatorInterface::DecorationNameRole ).toString(),
                iconName );
        
        m_indicator.activate(indicatorType, parameters);
    } else {
        m_indicator.deactivate( indicatorType );
    }
#endif
}
