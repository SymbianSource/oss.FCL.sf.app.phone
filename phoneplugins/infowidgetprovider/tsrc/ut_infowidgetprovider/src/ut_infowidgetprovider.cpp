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
#include "ut_infowidgetprovider.h"
#include "infowidgetprovider.h"
#include "qtestmains60.h"

/*!
  UT_InfoWidgetProvider::UT_InfoWidgetProvider
 */
UT_InfoWidgetProvider::UT_InfoWidgetProvider() 
    : 
    m_infoWidgetProvider(0)
{
    
}


/*!
  UT_InfoWidgetProvider::~UT_InfoWidgetProvider
 */
UT_InfoWidgetProvider::~UT_InfoWidgetProvider()
{
    delete m_infoWidgetProvider;
}


/*!
  UT_InfoWidgetProvider::init
 */
void UT_InfoWidgetProvider::init()
{
    initialize();
    
    m_infoWidgetProvider = new InfoWidgetProvider();
    
    QVERIFY(verify());
}


/*!
  UT_InfoWidgetProvider::cleanup
 */
void UT_InfoWidgetProvider::cleanup()
{
    reset();
    
    delete m_infoWidgetProvider;
    m_infoWidgetProvider = 0;
}


/*!
  UT_InfoWidgetProvider::t_widgets
 */
void UT_InfoWidgetProvider::t_widgets()
{
    QList<HsWidgetToken> widgets = m_infoWidgetProvider->widgets();
    QVERIFY(widgets.count() == 1);
    
    const HsWidgetToken &widgetToken = widgets.at(0); 
    QVERIFY(widgetToken.mLibrary == QString("infowidgetprovider.dll"));
    QVERIFY(widgetToken.mUri == QString("infouri"));
    QVERIFY(widgetToken.mTitle == QString("Info"));
    QVERIFY(widgetToken.mIconUri == QString("info.png"));
    QVERIFY(widgetToken.mDescription.isEmpty());
    QVERIFY(widgetToken.mUserData.isNull());
    QVERIFY(widgetToken.mPreferences.isEmpty());
}


/*!
  UT_InfoWidgetProvider::t_createWidget
 */
void UT_InfoWidgetProvider::t_createWidget()
{
    HsWidgetToken token;
    token.mUri = QString("infouri");
    QScopedPointer<HsWidget> widget(m_infoWidgetProvider->createWidget(token));
    QVERIFY(0 != widget.data());
}


/*!
  UT_InfoWidgetProvider::t_createWidgetUnknownUri
 */
void UT_InfoWidgetProvider::t_createWidgetUnknownUri()
{
    HsWidgetToken token;
    token.mUri = QString("invaliduri");
    QScopedPointer<HsWidget> widget(m_infoWidgetProvider->createWidget(token));
    QVERIFY(0 == widget.data());
}

QTEST_MAIN_S60(UT_InfoWidgetProvider)
