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
#include "ut_infowidgetlayoutmanager.h"
#include "infowidgetlayoutmanager.h"
#include "qtestmains60.h"

/*!
  UT_InfoWidgetLayoutManager::UT_InfoWidgetLayoutManager
 */
UT_InfoWidgetLayoutManager::UT_InfoWidgetLayoutManager() 
    : 
    m_layoutManager(0)
{
    
}


/*!
  UT_InfoWidgetLayoutManager::~UT_InfoWidgetLayoutManager
 */
UT_InfoWidgetLayoutManager::~UT_InfoWidgetLayoutManager()
{
    delete m_layoutManager;
}


/*!
  UT_InfoWidgetLayoutManager::init
 */
void UT_InfoWidgetLayoutManager::init()
{
    initialize();
    
    m_layoutManager = new InfoWidgetLayoutManager();
    
    QVERIFY(verify());
}


/*!
  UT_InfoWidgetLayoutManager::cleanup
 */
void UT_InfoWidgetLayoutManager::cleanup()
{
    reset();
    
    delete m_layoutManager;
    m_layoutManager = 0;
}

QTEST_MAIN_S60(UT_InfoWidgetLayoutManager)
