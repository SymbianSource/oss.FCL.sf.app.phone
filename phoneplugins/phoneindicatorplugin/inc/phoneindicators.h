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


static const char *IndicatorNameTemplate = "PhoneIndicator_%1";

enum Interaction
{
    InteractionNone,
    OpenMissedCallView,
    Deactivate
};

enum PhoneIndicatorTypes
{
    PhoneMissedCallIndicator = 0
};

inline QString indicatorName(int indicatorType) {
    return QString(IndicatorNameTemplate).arg(indicatorType);
}

struct PhoneIndicatorInfo
{
    const char *icon;
    const char *primaryText;
    const char *secondaryText;
    HbIndicatorInterface::GroupPriority priority;
    Interaction interaction;
};

Q_DECLARE_METATYPE(PhoneIndicatorInfo)

static const int PhoneIndicatorCount = 1;
static const PhoneIndicatorInfo IndicatorInfos[PhoneIndicatorCount] = {
    {"qtg_mono_info.svg",   "Missed call", "Name/Number", HbIndicatorInterface::GroupPriorityHigh, OpenMissedCallView } // TODO: change icon
};

#endif // PHONEINDICATORS_H
