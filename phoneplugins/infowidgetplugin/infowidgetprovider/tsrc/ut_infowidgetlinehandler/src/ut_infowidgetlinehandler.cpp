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
#include "ut_infowidgetlinehandler.h"
#include "infowidgetlinehandler.h"
#include "qtestmains60.h"

/*!
  UT_InfoWidgetLineHandler::UT_InfoWidgetLineHandler
 */
UT_InfoWidgetLineHandler::UT_InfoWidgetLineHandler() 
    : 
    m_lineHandler(0)
{
    
}


/*!
  UT_InfoWidgetLineHandler::~UT_InfoWidgetLineHandler
 */
UT_InfoWidgetLineHandler::~UT_InfoWidgetLineHandler()
{
    delete m_lineHandler;
}

/*!
  UT_InfoWidgetLineHandler::init
 */
void UT_InfoWidgetLineHandler::init()
{
    initialize();
    
    m_lineHandler =  new InfoWidgetLineHandler();
    
    QVERIFY(verify());
}


/*!
  UT_InfoWidgetLineHandler::cleanup
 */
void UT_InfoWidgetLineHandler::cleanup()
{
    reset();
    
    delete m_lineHandler;
    m_lineHandler = 0;
}

/*!
  UT_InfoWidgetLineHandler::t_logCurrentInfo
 */
void UT_InfoWidgetLineHandler::t_logCurrentInfo()
{
    m_lineHandler->logCurrentInfo();
}


/*!
  UT_InfoWidgetLineHandler::t_activeLine
 */
void UT_InfoWidgetLineHandler::t_activeLine()
{
    int activeLine = m_lineHandler->activeLine();
}


/*!
  UT_InfoWidgetLineHandler::t_setActiveLine
 */
void UT_InfoWidgetLineHandler::t_setActiveLine()
{
    const int KActiveLine = 0;
    m_lineHandler->setActiveLine(KActiveLine);
}

QTEST_MAIN_S60(UT_InfoWidgetLineHandler)
