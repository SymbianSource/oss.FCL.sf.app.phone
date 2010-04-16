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

#include <HbDataFormModel>
#include <HbCheckBox>
#include <psetwrapper.h>
#include "ut_barringplugingroup.h"
#include "qtestmains60.h"
#include "cpitemdatahelper.h"
#include "cpphonenotes.h"
#define private friend class UT_CpBarringPluginGroup; private
#include "cpbarringplugingroup.h"

void setNoteIdentifier(int &noteId, const QString& text)
{
    noteId = 1;
}

/*!
  UT_CpBarringPluginGroup::UT_CpBarringPluginGroup
 */
UT_CpBarringPluginGroup::UT_CpBarringPluginGroup() 
    : 
    m_barringpluginGroup(NULL),
    m_barringWrapperMock(NULL)
{

}

/*!
  UT_CpBarringPluginGroup::~UT_CpBarringPluginGroup
 */
UT_CpBarringPluginGroup::~UT_CpBarringPluginGroup()
{
    delete m_barringpluginGroup;
    delete m_barringWrapperMock;
}

/*!
  UT_CpBarringPluginGroup::init
 */
void UT_CpBarringPluginGroup::init()
{
    initialize();
    
    CPsetContainer &dummyContainer = reinterpret_cast<CPsetContainer &>(*this);
    m_barringWrapperMock = new PSetCallBarringWrapper(dummyContainer);
    EXPECT(PSetWrapper::callBarringWrapper).returns(m_barringWrapperMock);
    
    CpItemDataHelper itemDataHelper;
    m_barringpluginGroup = new CpBarringPluginGroup(itemDataHelper);
    
    m_dataFormModel = new HbDataFormModel();
    m_barringpluginGroup->setModel(m_dataFormModel);
    
    QVERIFY(verify());
}


/*!
  UT_CpBarringPluginGroup::cleanup
 */
void UT_CpBarringPluginGroup::cleanup()
{
    reset();
    
    delete m_barringpluginGroup;
    m_barringpluginGroup = NULL;
    delete m_barringWrapperMock;
    m_barringWrapperMock = NULL;
}

/*!
  UT_CpBarringPluginGroup::t_memleak
 */
void UT_CpBarringPluginGroup::t_memleak()
{
    
}


/*!
  UT_CpBarringPluginGroup::t_barringStatusRequestCompletedWithAnError
 */
void UT_CpBarringPluginGroup::t_barringStatusRequestCompletedWithAnError()
{
    EXPECT(CpPhoneNotes::cancelNote);
    EXPECT(CpPhoneNotes::showGlobalErrorNote);
    EXPECT(CpItemDataHelper::addConnection).times(0);
    
    QList<unsigned char> basicServiceGroupIds;
    const int errorCode = -1;
    m_barringpluginGroup->barringStatusRequestCompleted(
        errorCode,
        basicServiceGroupIds, 
        PSetCallBarringWrapper::BarringStatusUnavailable);
    
    QVERIFY(verify());
}


/*!
  UT_CpBarringPluginGroup::t_getBarringStatuses
  Tests functions 
      CpBarringPluginGroup::itemShown, 
      CpBarringPluginGroup::barringStatusRequestCompleted
 */
Q_DECLARE_METATYPE(QModelIndex)
void UT_CpBarringPluginGroup::t_getBarringStatuses()
{
    qRegisterMetaType<QModelIndex>("QModelIndex");
    
    EXPECT(PSetCallBarringWrapper::barringStatus)
        .with(ServiceGroupVoice, 
            PSetCallBarringWrapper::BarringTypeAllOutgoing);
    EXPECT(PSetCallBarringWrapper::barringStatus)
        .with(ServiceGroupVoice, 
            PSetCallBarringWrapper::BarringTypeOutgoingInternational);
    EXPECT(PSetCallBarringWrapper::barringStatus)
        .with(ServiceGroupVoice, 
            PSetCallBarringWrapper::BarringTypeOutgoingInternationalExceptToHomeCountry);
    EXPECT(PSetCallBarringWrapper::barringStatus)
        .with(ServiceGroupVoice, 
            PSetCallBarringWrapper::BarringTypeAllIncoming);
    EXPECT(PSetCallBarringWrapper::barringStatus)
        .with(ServiceGroupVoice, 
            PSetCallBarringWrapper::BarringTypeIncomingWhenRoaming);
    EXPECT(CpPhoneNotes::showGlobalProgressNote)
        .willOnce(invoke(setNoteIdentifier));
    EXPECT(CpItemDataHelper::addConnection).times(5);
    
    int numOfChilds = m_barringpluginGroup->childCount();
    for (int childIndex = 0; childIndex < numOfChilds; childIndex++) {
        HbDataFormModelItem *item = m_barringpluginGroup->childAt(childIndex);
        m_dataFormModel->appendDataFormItem(item);
        m_barringpluginGroup->itemShown(m_dataFormModel->indexFromItem(item));
    }
    
    QSignalSpy spy(
        m_dataFormModel, 
        SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)));
    QList<unsigned char> basicServiceGroupIds;
    m_barringpluginGroup->barringStatusRequestCompleted(
        PSetCallBarringWrapper::BarringErrorNone,
        basicServiceGroupIds, PSetCallBarringWrapper::BarringStatusActive);
    m_barringpluginGroup->barringStatusRequestCompleted(
        PSetCallBarringWrapper::BarringErrorNone,
        basicServiceGroupIds, PSetCallBarringWrapper::BarringStatusInactive);
    m_barringpluginGroup->barringStatusRequestCompleted(
        PSetCallBarringWrapper::BarringErrorNone,
        basicServiceGroupIds, PSetCallBarringWrapper::BarringStatusNotProvisioned);
    m_barringpluginGroup->barringStatusRequestCompleted(
        PSetCallBarringWrapper::BarringErrorNone,
        basicServiceGroupIds, PSetCallBarringWrapper::BarringStatusUnavailable);
    m_barringpluginGroup->barringStatusRequestCompleted(
        PSetCallBarringWrapper::BarringErrorNone,
        basicServiceGroupIds, PSetCallBarringWrapper::BarringStatusUnknown);
    
    QCOMPARE(spy.count(), 5);
    
    // Verify that barring status checking is not started on item show if 
    // status is already queried.
    EXPECT(PSetCallBarringWrapper::barringStatus).times(0);
    HbDataFormModelItem *item = m_barringpluginGroup->childAt(0);
    item->setContentWidgetData("checkState", QVariant(Qt::Checked));
    m_barringpluginGroup->itemShown(m_dataFormModel->indexFromItem(item));
    
    QVERIFY(verify());
}


/*!
  UT_CpBarringPluginGroup::t_enableBarringRequestCompleted
 */
void UT_CpBarringPluginGroup::t_enableBarringRequestCompleted()
{
    connect(
        this, SIGNAL(simulateCheckStateChange(int)), 
        m_barringpluginGroup, SLOT(changeBarringStateRequested(int)));
    
// request completed succesfully -case
    EXPECT(CpItemDataHelper::widgetFromModelIndex).returns(this).times(1);
    EXPECT(CpPhoneNotes::cancelNote);
    EXPECT(CpPhoneNotes::showGlobalNote);
    
    emit simulateCheckStateChange(Qt::Checked);
    m_barringpluginGroup->enableBarringRequestCompleted(
        0, 
        PSetCallBarringWrapper::BarringTypeAllBarrings,
        PSetCallBarringWrapper::BarringStatusActive,
        false);
    
    QVERIFY(verify());
    
// request completed with an error -case
    EXPECT(CpItemDataHelper::widgetFromModelIndex).returns(this).times(2);
    EXPECT(CpPhoneNotes::cancelNote);
    EXPECT(CpPhoneNotes::showGlobalErrorNote);
    EXPECT(HbCheckBox::checkState).returns(Qt::Checked);    
    EXPECT(HbCheckBox::setCheckState).with(Qt::Unchecked);
    
    emit simulateCheckStateChange(Qt::Checked);
    m_barringpluginGroup->enableBarringRequestCompleted(
        -1, 
        PSetCallBarringWrapper::BarringTypeAllBarrings,
        PSetCallBarringWrapper::BarringStatusActive,
        false);
    
    QVERIFY(verify());
}


/*!
  UT_CpBarringPluginGroup::t_disableBarringRequestCompleted
 */
void UT_CpBarringPluginGroup::t_disableBarringRequestCompleted()
{
    connect(
        this, SIGNAL(simulateCheckStateChange(int)), 
        m_barringpluginGroup, SLOT(changeBarringStateRequested(int)));
    
// request completed succesfully -case
    EXPECT(CpItemDataHelper::widgetFromModelIndex).returns(this).times(1);
    EXPECT(CpPhoneNotes::cancelNote);
    EXPECT(CpPhoneNotes::showGlobalNote);
    emit simulateCheckStateChange(Qt::Unchecked);
    
    m_barringpluginGroup->disableBarringRequestCompleted(
        0, 
        PSetCallBarringWrapper::BarringTypeAllBarrings,
        PSetCallBarringWrapper::BarringStatusActive,
        false);
    
    QVERIFY(verify());
    
// request completed with an error -case
    EXPECT(CpItemDataHelper::widgetFromModelIndex).returns(this).times(2);
    EXPECT(CpPhoneNotes::cancelNote);
    EXPECT(CpPhoneNotes::showGlobalErrorNote);
    EXPECT(HbCheckBox::checkState).returns(Qt::Unchecked);    
    EXPECT(HbCheckBox::setCheckState).with(Qt::Checked);
    
    emit simulateCheckStateChange(Qt::Unchecked);
    m_barringpluginGroup->disableBarringRequestCompleted(
        -1, 
        PSetCallBarringWrapper::BarringTypeAllBarrings,
        PSetCallBarringWrapper::BarringStatusActive,
        false);
    
    QVERIFY(verify());
}


/*!
  UT_CpBarringPluginGroup::t_changeBarringStateRequested
 */
void UT_CpBarringPluginGroup::t_changeBarringStateRequested()
{
// barring enable request case
    EXPECT(CpItemDataHelper::widgetFromModelIndex).returns(this);
    EXPECT(PSetCallBarringWrapper::enableBarring)
        .with(ServiceGroupVoice, 
            PSetCallBarringWrapper::BarringTypeAllOutgoing, QString("1234"));
    EXPECT(CpPhoneNotes::showGlobalProgressNote);
    connect(
        this, SIGNAL(simulateCheckStateChange(int)), 
        m_barringpluginGroup, SLOT(changeBarringStateRequested(int)));
    
    emit simulateCheckStateChange(Qt::Checked);
    
// barring disable request case
    EXPECT(CpItemDataHelper::widgetFromModelIndex).returns(this);
    EXPECT(PSetCallBarringWrapper::disableBarring)
        .with(ServiceGroupVoice, 
            PSetCallBarringWrapper::BarringTypeAllOutgoing, QString("1234"));
    EXPECT(CpPhoneNotes::showGlobalProgressNote);
    emit simulateCheckStateChange(Qt::Unchecked);
    
// barring item not found case
    EXPECT(PSetCallBarringWrapper::enableBarring).times(0);
    EXPECT(CpPhoneNotes::showGlobalProgressNote).times(0);
    emit simulateCheckStateChange(Qt::Checked);
    
    QVERIFY(verify());
}

QTEST_MAIN_S60(UT_CpBarringPluginGroup)
