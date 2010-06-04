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

#include "ut_cpdivertselectionitem.h"
#include "qtestmains60ui.h"
#include <HbDataFormModel>
#include <HbWidget>
#include <HbDataForm>
#include "cpdivertselectioncustomitem.h"

#define protected public
#define private public
#include "cpdivertselectionitem.h"



/*!
  UT_CpDivertSelectionItem::UT_CpDivertSelectionItem
 */
UT_CpDivertSelectionItem::UT_CpDivertSelectionItem() 
    : m_item(NULL)
{

}

/*!
  UT_CpDivertSelectionItem::~UT_CpDivertSelectionItem
 */
UT_CpDivertSelectionItem::~UT_CpDivertSelectionItem()
{

}

/*!
  UT_CpDivertSelectionItem::init
 */
void UT_CpDivertSelectionItem::init()
{
    initialize();
    m_item = new CpDivertSelectionItem();
    m_form = new HbDataForm;
    m_form->setModel(new HbDataFormModel);
    
    QList<HbAbstractViewItem *> prototypes = m_form->itemPrototypes();
    prototypes.append(m_item);
    m_form->setItemPrototypes(prototypes);
}

/*!
  UT_CpDivertSelectionItem::cleanup
 */
void UT_CpDivertSelectionItem::cleanup()
{
    reset();
    if (m_form) {
        delete m_form->model();
    }
    delete m_form;
    m_form = NULL;
}

/*!
  UT_CpDivertSelectionItem::t_memleak
 */
void UT_CpDivertSelectionItem::t_memleak()
{
    
}

/*!
  UT_CpDivertSelectionItem::t_createItem
 */
void UT_CpDivertSelectionItem::t_createItem()
{
    delete m_item->createItem();
}

/*!
  UT_CpDivertSelectionItem::t_canSetModelIndex
 */
void UT_CpDivertSelectionItem::t_canSetModelIndex()
{
    QModelIndex index = qobject_cast<HbDataFormModel*>(m_form->model())->indexFromItem(
            qobject_cast<HbDataFormModel*>(m_form->model())->appendDataFormItem(
                    HbDataFormModelItem::TextItem, "test"));
    
    QVERIFY( !m_item->canSetModelIndex(index) );

    index = qobject_cast<HbDataFormModel*>(m_form->model())->indexFromItem(
                qobject_cast<HbDataFormModel*>(m_form->model())->appendDataFormItem(
                    static_cast<HbDataFormModelItem::DataItemType>(
                            CpDivertSelectionItem::CpDivertSelectionItemId), 
                            "test2"));
    
    QVERIFY( m_item->canSetModelIndex(index) );
}

/*!
  UT_CpDivertSelectionItem::t_createCustomWidget
 */
void UT_CpDivertSelectionItem::t_createCustomWidget()
{
    QVERIFY( !m_item->createCustomWidget() );
    
    QModelIndex index = qobject_cast<HbDataFormModel*>(m_form->model())->indexFromItem(
            qobject_cast<HbDataFormModel*>(m_form->model())->appendDataFormItem(
                    static_cast<HbDataFormModelItem::DataItemType>(
                            CpDivertSelectionItem::CpDivertSelectionItemId), 
                            "test3"));
    
    m_item->setModelIndex(index);
    HbWidget *widget=NULL;
    QVERIFY( widget = m_item->createCustomWidget() );
    QVERIFY( widget->setProperty("number", "12345") );
    QVERIFY( widget->property("number").toString() == QString("12345") );
    
    QVERIFY( widget->setProperty("timeoutText", "Delay:") );
    QVERIFY( widget->property("timeoutText").toString() == QString("Delay:") );
    
    QVERIFY( widget->setProperty("timeout", "25") );
    QVERIFY( widget->property("timeout").toInt() == 25 );
    
    QVERIFY( widget->setProperty("state", "0"));
    QVERIFY( widget->property("state").toInt() 
        == CpDivertSelectionCustomitem::Enabled);
    QVERIFY( widget->setProperty("state", "1"));
    QVERIFY( widget->property("state").toInt() 
        == CpDivertSelectionCustomitem::Deactivated);
    QVERIFY( widget->setProperty("state", "2"));
    QVERIFY( widget->property("state").toInt() 
        == CpDivertSelectionCustomitem::Disabled);
    
    delete widget;
}

QTEST_MAIN_S60UI(UT_CpDivertSelectionItem)
