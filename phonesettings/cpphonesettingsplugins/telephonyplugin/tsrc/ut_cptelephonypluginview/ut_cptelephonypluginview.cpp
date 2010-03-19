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

#include "ut_cptelephonypluginview.h"
#include "qtestmains60ui.h"
#include <CpPluginPlatInterface.h>
#include <cpitemdatahelper.h>
#include <CpSettingFormItemData.h>
#include <smcmockclassincludes.h>
#include <HbDataForm.h>
#include <HbDataFormModel.h>
#include <hbsettingformitem.h>
#define private public
#include "cptelephonypluginview.h"


class DummyPlugin : public CpPluginPlatInterface
{
public:
    DummyPlugin(){}
    ~DummyPlugin(){}
    
    CpSettingFormItemData *createSettingFormItemData(
            CpItemDataHelper &itemDataHelper) const
    {
    SMC_MOCK_METHOD1( CpSettingFormItemData*, CpItemDataHelper &, itemDataHelper )
    }
    
};

/*!
  UT_CpTelephonyPluginView::UT_CpTelephonyPluginView
 */
UT_CpTelephonyPluginView::UT_CpTelephonyPluginView() 
    : m_callspluginview(NULL)
{

}

/*!
  UT_CpTelephonyPluginView::~UT_CpTelephonyPluginView
 */
UT_CpTelephonyPluginView::~UT_CpTelephonyPluginView()
{
    delete m_callspluginview;
}



/*!
  UT_CpTelephonyPluginView::init
 */
void UT_CpTelephonyPluginView::init()
{
    initialize();
    CpPluginPlatInterface *nullPlugin=0;
    expect("Tools::loadCpPlugin").with(QString("cpcallsplugin")).returns(nullPlugin);
    DummyPlugin *ret = new DummyPlugin;
    //CpSettingFormItemData *nullItemData=0;
    //expect("Tools::loadCpPlugin").with(QString("cpdivertplugin")).returns(ret);
    //expect("DummyPlugin::createSettingFormItemData").returns(nullItemData);
    
    //ret = new DummyPlugin;
    CpSettingFormItemData *itemData = new CpSettingFormItemData;
    expect("Tools::loadCpPlugin").with(QString("cpdivertplugin")).returns(ret); // cpnetworkplugin
    expect("DummyPlugin::createSettingFormItemData").returns(itemData);
    
    m_callspluginview = new CpTelephonyPluginView;
    QVERIFY( verify() );
}

/*!
  UT_CpTelephonyPluginView::cleanup
 */
void UT_CpTelephonyPluginView::cleanup()
{
    reset();
    
    delete m_callspluginview;
    m_callspluginview = NULL;
}

/*!
  UT_CpTelephonyPluginView::t_memleak
 */
void UT_CpTelephonyPluginView::t_memleak()
{
    CpSettingFormItemData *itemData = new CpSettingFormItemData;
    DummyPlugin *ret = new DummyPlugin;
    expect("Tools::loadCpPlugin").with(QString("cpcallsplugin")).returns(ret);
    expect("DummyPlugin::createSettingFormItemData").returns(itemData);
    
    ret = new DummyPlugin;
    expect("Tools::loadCpPlugin").with(QString("cpdivertplugin")).returns(ret);
    expect("DummyPlugin::createSettingFormItemData").returns(itemData);
    
    //CpPluginPlatInterface *nullPlugin=0;
    //expect("Tools::loadCpPlugin").with(QString("cpnetworkplugin")).returns(nullPlugin);
    
    CpTelephonyPluginView *tmp = new CpTelephonyPluginView;
    QVERIFY( verify() );
    delete tmp;
}

/*!
  UT_CpTelephonyPluginView::t_onConnectionRemoved
 */
void UT_CpTelephonyPluginView::t_onConnectionRemoved()
{
    HbDataForm *w = m_callspluginview->settingForm();
    HbDataFormModel *m = static_cast<HbDataFormModel *>(w->model());

    m_callspluginview->m_helper->addConnection(
            m->item(0),SIGNAL(currentIndexChanged(int)),
            this, SLOT(tmp(int)));
    
    m_callspluginview->m_helper->removeConnection(
            m->item(0),SIGNAL(currentIndexChanged(int)),
            this, SLOT(tmp(int)));
}

/*!
  UT_CpTelephonyPluginView::t_onPrototypeAdded
 */
void UT_CpTelephonyPluginView::t_onPrototypeAdded()
{
    HbSettingFormItem* item= new HbSettingFormItem;
    m_callspluginview->m_helper->addItemPrototype(item);
    QVERIFY( m_callspluginview->settingForm()->
        itemPrototypes().contains(item) );
    
    delete item;
}



QTEST_MAIN_S60UI(UT_CpTelephonyPluginView)
