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
#include <cpplugininterface.h>
#include <cpitemdatahelper.h>
#include <cpsettingformitemdata.h>
#include <smcmockclassincludes.h>
#include <hbdataform.h>
#include <hbdataformmodel.h>
#include "cptelephonypluginview.h"
#include <cppluginloader.h>


class DummyPlugin : public CpPluginInterface
{
public:
    DummyPlugin(){}
    ~DummyPlugin(){}
    
    QList<CpSettingFormItemData*> createSettingFormItemData(
            CpItemDataHelper &itemDataHelper) const
    {
        SMC_MOCK_METHOD1( QList<CpSettingFormItemData*>, CpItemDataHelper &, itemDataHelper )
    }
};

/*!
  UT_CpTelephonyPluginView::UT_CpTelephonyPluginView
 */
UT_CpTelephonyPluginView::UT_CpTelephonyPluginView() 
{
}

/*!
  UT_CpTelephonyPluginView::~UT_CpTelephonyPluginView
 */
UT_CpTelephonyPluginView::~UT_CpTelephonyPluginView()
{
}


/*!
  UT_CpTelephonyPluginView::init
 */
void UT_CpTelephonyPluginView::init()
{
    initialize();
}


/*!
  UT_CpTelephonyPluginView::cleanup
 */
void UT_CpTelephonyPluginView::cleanup()
{
    reset();
}


/*!
  UT_CpTelephonyPluginView::t_memleak
 */
void UT_CpTelephonyPluginView::t_memleak()
{
    QList<CpSettingFormItemData*> list;
    list.append(new CpSettingFormItemData);
    DummyPlugin *ret = new DummyPlugin;

    HbDataForm *form = new HbDataForm;
    EXPECT(HbView, widget).returns(form);

    EXPECT(CpPluginLoader, loadCpPluginInterface).
        with(QString("cpcallsplugin")).returns(ret); // Ownership change.
    EXPECT(DummyPlugin, createSettingFormItemData).returns(list);

    // For clarity, create a new pointer instead of using the existing one. 
    DummyPlugin *dummy = new DummyPlugin;
    EXPECT(CpPluginLoader, loadCpPluginInterface).
        with(QString("cpdivertplugin")).returns(dummy); // Ownership change.
    EXPECT(DummyPlugin, createSettingFormItemData).returns(list);

    QModelIndex index;
    EXPECT(HbDataFormModel, indexFromItem).returns<QModelIndex>(index);

    CpTelephonyPluginView *tmp = new CpTelephonyPluginView;
    QVERIFY(verify());
    delete tmp;
    delete form;

    // Test constructor that doesn't call any other functions.
    EXPECT(HbView, widget).returns<QGraphicsWidget *>(0);
    CpTelephonyPluginView *tmp2 = new CpTelephonyPluginView;
    QVERIFY(verify());
    delete tmp2;
}


void UT_CpTelephonyPluginView::t_construct2()
{
    QVariantList list;
    list.append("blah");
    list.append("blaah");

    // Test constructor that doesn't call any other functions.
    EXPECT(HbView, widget).returns<QGraphicsWidget *>(0);
    CpTelephonyPluginView *tmp = new CpTelephonyPluginView(list);
    QVERIFY(verify());
    delete tmp;

    // Do the "real" test.
    QModelIndex index;
    HbDataForm *form = new HbDataForm;
    EXPECT(HbView, widget).returns(form);

    DummyPlugin *dummy = new DummyPlugin;
    EXPECT(CpPluginLoader, loadCpPluginInterface).
        with(QString("blaah")).returns(dummy); // Ownership change.
    QList<CpSettingFormItemData*> list2;
    list2.append(new CpSettingFormItemData);
    EXPECT(DummyPlugin, createSettingFormItemData).returns(list2);

    EXPECT(HbDataFormModel, indexFromItem).returns<QModelIndex>(index);
    CpTelephonyPluginView *tmp2 = new CpTelephonyPluginView(list);
    QVERIFY(verify());
    delete form;
    delete tmp2;
}


QTEST_MAIN_S60UI(UT_CpTelephonyPluginView)
