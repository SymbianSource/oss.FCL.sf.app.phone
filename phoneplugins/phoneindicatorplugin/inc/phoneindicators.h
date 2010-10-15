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

#ifndef PHONEINDICATORS_H
#define PHONEINDICATORS_H

#include <hbindicatorinterface.h>
#include <QObject>
#include <QString>


static const char *IndicatorNameTemplate = "com.nokia.symbian.phoneindicatorplugin_%1";

enum Interaction
{
    InteractionNone,
    OpenMissedCallView,
    OpenCallUi,
    OpenDiverSettingsView,
    Deactivate
};

enum PhoneIndicatorTypes
{
    PhoneMissedCallIndicator = 0,
    PhoneActiveCallIndicator,
    PhoneDivertIndidicator
};

inline QString indicatorName(int indicatorType) {
    return QString(IndicatorNameTemplate).arg(indicatorType);
}

struct PhoneIndicatorInfo
{
    const char *icon;
    const char *primaryText;
    const char *secondaryText;
    Interaction interaction;
    HbIndicatorInterface::Category catecory;
};

Q_DECLARE_METATYPE(PhoneIndicatorInfo)

static const int PhoneIndicatorCount = 3;
static const PhoneIndicatorInfo IndicatorInfos[PhoneIndicatorCount] = {
    {"qtg_mono_missed_call_unseen",   "Missed call", "Name/Number", OpenMissedCallView, HbIndicatorInterface::NotificationCategory },
    {"qtg_mono_call",   "Active call", " ", OpenCallUi, HbIndicatorInterface::NotificationCategory },
    {"qtg_mono_call_diverted",   "Active diverts", " ", OpenDiverSettingsView, HbIndicatorInterface::SettingCategory }
};

#endif // PHONEINDICATORS_H
