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
* Description:
*
*/

#include <QtGui>
#include <QtTest/QtTest>

#include <hbapplication.h>
#include <hbmainwindow.h>
#include <hbaction.h>

#include "bubbletest.h"
#include "bubblestyleoption.h"

class ut_BubbleStyleOption : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    void testStyleOption();

private:
};

void ut_BubbleStyleOption::initTestCase()
{
}

void ut_BubbleStyleOption::cleanupTestCase()
{
}

void ut_BubbleStyleOption::testStyleOption()
{
    BubbleStyleOption option;
    Q_ASSERT(option.mText1=="");
    option.mText1="John";
    BubbleStyleOption option2(option);
    Q_ASSERT(option2.mText1 == "John");
}

BUBBLE_TEST_MAIN(ut_BubbleStyleOption)
#include "ut_bubblestyleoption.moc"
