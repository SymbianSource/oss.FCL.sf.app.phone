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
#include "qtphonelog.h"

#include <QByteArray>
#include <hbicon.h>

#ifdef Q_OS_SYMBIAN
#include <logsmodel.h>
#include <logsfilter.h>
#include <LogsDomainCRKeys.h>
#include <ctsydomaincrkeys.h>
#endif

namespace PhoneIndicatorControllerKeys{
    const XQSettingsKey missedCallsSettingsKey( XQSettingsKey::TargetCentralRepository, 
        KCRUidLogs.iUid, KLogsNewMissedCalls );

    const XQSettingsKey unconditionalCFKey( XQSettingsKey::TargetCentralRepository, 
        KCRUidCtsyCallForwardingIndicator.iUid, KCtsyUnconditionalCFStatus );

    const XQSettingsKey currentCFKey( XQSettingsKey::TargetCentralRepository, 
            KCRUidCtsyCallForwardingIndicator.iUid, KCtsyCallForwardingIndicator );     
}



PhoneIndicatorController::PhoneIndicatorController(QObject *parent):
    QObject(parent), m_logsModel(0), m_missedCallsFilter(0)
{
    PHONE_TRACE
#ifdef Q_OS_SYMBIAN
    m_setManager = new XQSettingsManager(this);
    connect( m_setManager, 
            SIGNAL( valueChanged(const XQSettingsKey & ,const QVariant &)),
                this, SLOT(updateMissedCallIndicator(XQSettingsKey, 
                        QVariant)));
    
    connect( m_setManager, 
            SIGNAL( valueChanged(const XQSettingsKey & ,const QVariant &)),
                this, SLOT(updateDivertIndicator(XQSettingsKey, 
                        QVariant)));
    
    m_setManager->startMonitoring( PhoneIndicatorControllerKeys::missedCallsSettingsKey );
    m_setManager->startMonitoring( PhoneIndicatorControllerKeys::unconditionalCFKey, 
            XQSettingsManager::TypeByteArray);
    m_setManager->startMonitoring( PhoneIndicatorControllerKeys::currentCFKey,
            XQSettingsManager::TypeByteArray);
    
    qRegisterMetaType<PhoneIndicatorInfo>("PhoneIndicatorInfo");    
    QByteArray bytes = m_setManager->readItemValue(
            PhoneIndicatorControllerKeys::unconditionalCFKey).toByteArray();
    if ( m_setManager->error() == XQSettingsManager::NoError ){
        int cfStatus=0;
        memcpy(&cfStatus, bytes.data_ptr()->array, sizeof(int));
        updateDiverIndicator(cfStatus & KCFVoiceForwarded);
    }
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

void PhoneIndicatorController::setActiveCallData()
{
    m_cli = hbTrId("txt_phone_dblist_ongoing_call");
    m_callImage = "qtg_mono_call";//QString::fromUtf16 (icon.Ptr (), icon.Length ());
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

    if (compareKeys(key, PhoneIndicatorControllerKeys::missedCallsSettingsKey) ){
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

void PhoneIndicatorController::updateDivertIndicator(
        const XQSettingsKey &key, const QVariant &value)
{
    PHONE_TRACE
    if ( compareKeys( key, PhoneIndicatorControllerKeys::currentCFKey ) ){
        PHONE_DEBUG("currentCfKey");
        int cfStatus = value.toByteArray().toInt();
        updateDiverIndicator( cfStatus & KCFVoiceForwarded);
    } else if (compareKeys(key, PhoneIndicatorControllerKeys::unconditionalCFKey )){        
        QByteArray bytes = value.toByteArray();
        // Convert QByteArray elements into integer
        // Conversation does not care about sign bit
        int status;
        memcpy(&status, bytes.data_ptr()->array, sizeof(int));
        updateDiverIndicator(status & KCFVoiceForwarded);
    }
}

void PhoneIndicatorController::setMissedallIndicatorData()
{
#ifdef Q_OS_SYMBIAN

    QString indicatorType(indicatorName(PhoneMissedCallIndicator));
    
    int missedCallCount = m_setManager->readItemValue( 
            PhoneIndicatorControllerKeys::missedCallsSettingsKey,
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

void PhoneIndicatorController::updateDiverIndicator(bool activeDiverts)
{
    PHONE_TRACE
    QString indicatorType(indicatorName(PhoneDivertIndidicator));
    if(activeDiverts){
        QVariantMap parameters;
        
        // First row
       parameters.insert( 
               QVariant( HbIndicatorInterface::PrimaryTextRole ).toString(),
               hbTrId("txt_phone_dblist_active_diverts" ));
        
        QString iconName = "qtg_mono_call_diverted";
        parameters.insert(
                QVariant( HbIndicatorInterface::DecorationNameRole ).toString(),
                iconName );
        
        m_indicator.activate( indicatorType, parameters );
    }else{
        m_indicator.deactivate( indicatorType );
    }        
}

bool PhoneIndicatorController::compareKeys(
        const XQSettingsKey &first, const XQSettingsKey &second)
{   
    PHONE_TRACE
    return ( first.key() == second.key() && first.uid() == second.uid() );
}
