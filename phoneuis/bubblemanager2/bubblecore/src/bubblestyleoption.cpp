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

#include "bubblestyleoption.h"

BubbleStyleOption::BubbleStyleOption(): HbStyleOption()
{
    type = (int)Type;
    version = (int)Version;
    mText1 = "";
    mText2 = "";
    mText3 = "";
    mText1Clip = Qt::ElideNone;
    mText2Clip = Qt::ElideNone;
    mText3Clip = Qt::ElideNone;
    mCliLineNumber = 0;
    mTimerLineNumber = 0;
    mCallState = BubbleManagerIF::None;
    mCallFlags = 0;
    mNumberType = BubbleManagerIF::NotSet;
}

BubbleStyleOption::BubbleStyleOption(const BubbleStyleOption &other) :
    HbStyleOption(other)
{
    type = (int)Type;
    version = (int)Version;
    
    mText1 = other.mText1;
    mText2 = other.mText2;
    mText3 = other.mText3;
    mText1Clip = other.mText1Clip;
    mText2Clip = other.mText2Clip;
    mText3Clip = other.mText3Clip;
    mCliLineNumber = other.mCliLineNumber;
    mTimerLineNumber = other.mTimerLineNumber;
    mCallState = other.mCallState;
    mCallFlags = other.mCallFlags;
    mNumberType = other.mNumberType;
}

BubbleStyleOption::~BubbleStyleOption()
{
}

