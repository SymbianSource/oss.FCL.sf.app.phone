/*!
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
* Description:  Bubble style option.
*
*/

#ifndef BUBBLESTYLEOPTION_H
#define BUBBLESTYLEOPTION_H

#include <hbstyleoption.h>
#include "bubblemanagerif.h"

class BubbleStyleOption : public HbStyleOption
{
public:
    BubbleStyleOption();
    BubbleStyleOption(const BubbleStyleOption &other);
    ~BubbleStyleOption();

    enum StyleOptionType { Type = HbSO_CustomBase + 1 };
    enum StyleOptionVersion { Version = 1 };

    QString mText1;
    Qt::TextElideMode mText1Clip;
    QString mText2;
    Qt::TextElideMode mText2Clip;
    QString mText3;
    Qt::TextElideMode mText3Clip;
    int mCliLineNumber;
    int mTimerLineNumber;
    BubbleManagerIF::PhoneCallState mCallState;
    BubbleManagerIF::PhoneNumberType mNumberType;
    int mCallFlags;
};

#endif // BUBBLESTYLEOPTION_H
