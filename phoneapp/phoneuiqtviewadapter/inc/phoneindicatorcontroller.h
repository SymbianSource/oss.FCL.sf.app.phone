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
*/

#ifndef PHONEINDICATORCONTROLLER_H_
#define PHONEINDICATORCONTROLLER_H_


#include <QObject>
#include <xqsettingsmanager.h>
#include <hbindicator.h>

// FORWARD DECLARATION
class LogsModel;
class LogsFilter;
class XQSettingsKey;
class PhoneIndicatorController : public QObject
    {
    Q_OBJECT
public:
    PhoneIndicatorController(QObject *parent = 0);
    virtual ~PhoneIndicatorController();
    
    void setActiveCallData();
    void clearActiveCallData();
    void enableActiveCallIndicator();
    void disableActiveCallIndicator();
    
private slots:
    void updateMissedCallIndicator(const XQSettingsKey &key, const QVariant &value);
    void updateDivertIndicator(const XQSettingsKey &key, const QVariant &value);
    void setMissedallIndicatorData();
  
private:
    void updateDiverIndicator(bool activeDiverts);
    inline bool compareKeys(const XQSettingsKey &first, const XQSettingsKey &second);
    
private:
    HbIndicator         m_indicator;
    LogsModel           *m_logsModel;
    LogsFilter          *m_missedCallsFilter;
    XQSettingsManager   *m_setManager;

    QString             m_cli;
    QString             m_callImage;
    };

#endif /* PHONEINDICATORCONTROLLER_H_ */
