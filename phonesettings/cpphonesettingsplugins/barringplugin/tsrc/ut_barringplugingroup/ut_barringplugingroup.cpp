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

#include "ut_barringplugingroup.h"
#include "qtestmains60.h"
#define private public
#include "cpbarringplugingroup.h"
#include "cpitemdatahelper.h"

/*!
  UT_CpBarringPluginGroup::UT_CpBarringPluginGroup
 */
UT_CpBarringPluginGroup::UT_CpBarringPluginGroup() 
    : m_barringpluginGroup(NULL)
{

}

/*!
  UT_CpBarringPluginGroup::~UT_CpBarringPluginGroup
 */
UT_CpBarringPluginGroup::~UT_CpBarringPluginGroup()
{
    delete m_barringpluginGroup;
}

/*!
  UT_CpBarringPluginGroup::init
 */
void UT_CpBarringPluginGroup::init()
{
    initialize();
    CpItemDataHelper itemDataHelper;


    QVERIFY(!m_barringpluginGroup);
    m_barringpluginGroup = new CpBarringPluginGroup(itemDataHelper);
    QVERIFY(m_barringpluginGroup);
}



/*!
  UT_CpBarringPluginGroup::cleanup
 */
void UT_CpBarringPluginGroup::cleanup()
{
    reset();
    
    delete m_barringpluginGroup;
    m_barringpluginGroup = NULL;
}

/*!
  UT_CpBarringPluginGroup::t_memleak
 */
void UT_CpBarringPluginGroup::t_memleak()
{
    
}

QTEST_MAIN_S60(UT_CpBarringPluginGroup)
