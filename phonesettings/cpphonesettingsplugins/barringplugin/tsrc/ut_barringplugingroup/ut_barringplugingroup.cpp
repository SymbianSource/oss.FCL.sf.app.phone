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
    Q_UNUSED(text)
    
    noteId = 1;
}


const QString KCurrentPassword = "1234";
void setCurrentPasswordParams(
        const QString &title,
        const QValidator &validator,
        int maxPasswordLength)
{
    Q_UNUSED(title)
    Q_UNUSED(validator)
    Q_UNUSED(maxPasswordLength)
}


const QString KNewAndVerifiedPassword = "4321";
void setNewAndVerifiedPasswordParams(
    const QString &title, 
    const QValidator &validator,
    int maxPasswordLength)
{
    Q_UNUSED(title)
    Q_UNUSED(validator)
    Q_UNUSED(maxPasswordLength)
}


void setPasswordParamsCancel(
    const QString &title, 
    const QValidator &validator,
    int maxPasswordLength)
{
    Q_UNUSED(title)
    Q_UNUSED(validator)
    Q_UNUSED(maxPasswordLength)
}


/*!
  UT_CpBarringPluginGroup::UT_CpBarringPluginGroup
 */
UT_CpBarringPluginGroup::UT_CpBarringPluginGroup() 
    : 
    m_barringpluginGroup(NULL),
    m_barringWrapperMock(NULL),
    m_dataFormModel(NULL)
{

}


/*!
  UT_CpBarringPluginGroup::~UT_CpBarringPluginGroup
 */
UT_CpBarringPluginGroup::~UT_CpBarringPluginGroup()
{
    delete m_barringWrapperMock;
    delete m_dataFormModel;
}


/*!
  UT_CpBarringPluginGroup::init
 */
void UT_CpBarringPluginGroup::init()
{
    initialize();
    QT_TRAP_THROWING(SmcDefaultValue<QString>::SetL(QString("")));
    
    CPsetContainer &dummyContainer = reinterpret_cast<CPsetContainer &>(*this);
    m_barringWrapperMock = new PSetCallBarringWrapper(dummyContainer);
    EXPECT(PSetWrapper::callBarringWrapper).returns(m_barringWrapperMock);
    
    CpItemDataHelper itemDataHelper;
    m_barringpluginGroup = new CpBarringPluginGroup(itemDataHelper);
    
    m_dataFormModel = new HbDataFormModel();
    m_dataFormModel->appendDataFormItem(m_barringpluginGroup);
    m_barringpluginGroup->m_model = m_dataFormModel;
    
    QVERIFY(verify());
    
    connect(
        this, SIGNAL(simulateCheckStateChange(int)), 
        m_barringpluginGroup, SLOT(changeBarringStateRequested(int)));
}


/*!
  UT_CpBarringPluginGroup::cleanup
 */
void UT_CpBarringPluginGroup::cleanup()
{
    reset();
    
    disconnect(
        this, SIGNAL(simulateCheckStateChange(int)), 
        m_barringpluginGroup, SLOT(changeBarringStateRequested(int)));
    
    delete m_dataFormModel;
    m_dataFormModel = NULL;
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
  UT_CpBarringPluginGroup::t_itemShownNotBarringItem
  Tests that itemShown() for other setting group's item is not handled.
 */
void UT_CpBarringPluginGroup::t_itemShownNotBarringItem()
{
    EXPECT(PSetCallBarringWrapper::barringStatus).times(0);
    
    QScopedPointer<CpSettingFormItemData> item(new CpSettingFormItemData(
        HbDataFormModelItem::CheckBoxItem, hbTrId(""), NULL));
    m_dataFormModel->appendDataFormItem(item.data(), m_barringpluginGroup);
    m_barringpluginGroup->itemShown(
        m_dataFormModel->indexFromItem(item.data()));
    m_dataFormModel->removeItem(item.take());
    
    QVERIFY(verify());
}


/*!
  UT_CpBarringPluginGroup::t_itemShownBarringStatusRequestOngoing
  Tests that another progress note is not launched if one is already shown.
 */
void UT_CpBarringPluginGroup::t_itemShownBarringStatusRequestOngoing()
{
    EXPECT(PSetCallBarringWrapper::barringStatus);
    EXPECT(CpPhoneNotes::noteShowing).returns(true);
    EXPECT(CpPhoneNotes::showGlobalProgressNote).times(0);
    
    HbDataFormModelItem *item = m_barringpluginGroup->childAt(0);
    m_barringpluginGroup->itemShown(m_dataFormModel->indexFromItem(item));
    
    QVERIFY(verify());
}


/*!
  UT_CpBarringPluginGroup::t_barringStatusRequestCompletedForUpdateInquiry
 */
void UT_CpBarringPluginGroup::t_barringStatusRequestCompletedForUpdateInquiry()
{
    // simulate successfull initial status inquiry
    QList<unsigned char> basicServiceGroupIds;
    const int errorCode = 0;
    int numOfChilds = m_barringpluginGroup->childCount();
    for (int childIndex = 0; childIndex < numOfChilds - 1; childIndex++) {
        HbDataFormModelItem *item = m_barringpluginGroup->childAt(childIndex);
        m_barringpluginGroup->itemShown(m_dataFormModel->indexFromItem(item));
        
        m_barringpluginGroup->barringStatusRequestCompleted(
            errorCode,
            basicServiceGroupIds, 
            PSetCallBarringWrapper::BarringStatusActive);
    }
    
    // simulate barring enable request
    EXPECT(CpItemDataHelper::widgetFromModelIndex).returns(this).times(1);
    EXPECT(CpPhoneNotes::showPasswordQueryDialog)
        .willOnce(invoke(setCurrentPasswordParams));
    emit simulateCheckStateChange(Qt::Checked);
    m_barringpluginGroup->completeBarringStateChangeRequestHandling(
            KCurrentPassword, true);
    
    // verify that already connected items are not enabled/connected again
    EXPECT(CpItemDataHelper::removeConnection).times(1);
    EXPECT(CpItemDataHelper::addConnection).times(1);
    m_barringpluginGroup->barringStatusRequestCompleted(
        errorCode,
        basicServiceGroupIds, 
        PSetCallBarringWrapper::BarringStatusActive);
    
    QVERIFY(verify());
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
    EXPECT(CpItemDataHelper::addConnection).times(6);
    
    int numOfChilds = m_barringpluginGroup->childCount();
    for (int childIndex = 0; childIndex < numOfChilds; childIndex++) {
        HbDataFormModelItem *item = m_barringpluginGroup->childAt(childIndex);
        m_barringpluginGroup->itemShown(m_dataFormModel->indexFromItem(item));
    }
    
    connect(
        m_dataFormModel, 
        SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)),
        this,
        SLOT(dataChanged(const QModelIndex &, const QModelIndex &)));
    
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
    QCOMPARE(spy.count(), 11);
    
    QVERIFY(verify());
    
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
// request completed succesfully -case
    EXPECT(CpItemDataHelper::widgetFromModelIndex).returns(this).times(1);
    EXPECT(CpPhoneNotes::showPasswordQueryDialog)
        .willOnce(invoke(setCurrentPasswordParams));
    EXPECT(CpPhoneNotes::cancelNote);
    EXPECT(CpPhoneNotes::showNotificationDialog);
    
    emit simulateCheckStateChange(Qt::Checked);
    m_barringpluginGroup->completeBarringStateChangeRequestHandling(
            KCurrentPassword, true);

    m_barringpluginGroup->enableBarringRequestCompleted(
        0, 
        PSetCallBarringWrapper::BarringTypeAllBarrings,
        PSetCallBarringWrapper::BarringStatusActive,
        false);
    
    QVERIFY(verify());
    QList<unsigned char> basicServiceGroupIds;
    
// request completed with an error -case
    HbDataFormModelItem *item = m_barringpluginGroup->childAt(0);
    item->setContentWidgetData("checkState", QVariant(Qt::Checked));
    EXPECT(CpItemDataHelper::widgetFromModelIndex).returns(this);
    EXPECT(CpPhoneNotes::showPasswordQueryDialog)
        .willOnce(invoke(setCurrentPasswordParams));
    EXPECT(CpItemDataHelper::removeConnection);
    EXPECT(CpItemDataHelper::addConnection);
    EXPECT(CpPhoneNotes::cancelNote);
    EXPECT(CpPhoneNotes::showGlobalErrorNote);
    
    emit simulateCheckStateChange(Qt::Checked);
    m_barringpluginGroup->completeBarringStateChangeRequestHandling(
            KCurrentPassword, true);
    
    m_barringpluginGroup->enableBarringRequestCompleted(
        -1, 
        PSetCallBarringWrapper::BarringTypeAllBarrings,
        PSetCallBarringWrapper::BarringStatusActive,
        false);

    QVERIFY(verify());
    
// request completed successfully and dependent barring setting needs 
// status inquiry
    EXPECT(CpItemDataHelper::widgetFromModelIndex).returns(this);
    EXPECT(CpPhoneNotes::showPasswordQueryDialog)
        .willOnce(invoke(setCurrentPasswordParams));
    // some other (outgoing) barring is enabled, status inquiery for that
    // should be done.
    item = m_barringpluginGroup->childAt(1);
    item->setContentWidgetData("checkState", QVariant(Qt::Checked));
    EXPECT(PSetCallBarringWrapper::barringStatus);
    EXPECT(CpPhoneNotes::cancelNote).times(0);
    EXPECT(CpPhoneNotes::showGlobalNote).times(0);
    
    emit simulateCheckStateChange(Qt::Checked);
    m_barringpluginGroup->completeBarringStateChangeRequestHandling(
            KCurrentPassword, true);
    
    m_barringpluginGroup->enableBarringRequestCompleted(
        0, 
        PSetCallBarringWrapper::BarringTypeAllBarrings,
        PSetCallBarringWrapper::BarringStatusActive,
        false);
    
    QVERIFY(verify());
    m_barringpluginGroup->barringStatusRequestCompleted(
        PSetCallBarringWrapper::BarringErrorNone,
        basicServiceGroupIds, PSetCallBarringWrapper::BarringStatusActive);
}


/*!
  UT_CpBarringPluginGroup::t_enableBarringRequestCompletedUnknownBarring
 */
void UT_CpBarringPluginGroup::t_enableBarringRequestCompletedUnknownBarring()
{
    // something very weird has happened and enable request completes for unknown
    // barring type.
    HbDataFormModelItem *item = m_barringpluginGroup->childAt(5);
    QModelIndex modelIndex = m_dataFormModel->indexFromItem(item);
    
    EXPECT(CpItemDataHelper::widgetFromModelIndex)
        .with<const QModelIndex &>(modelIndex).returns(this);
    EXPECT(CpPhoneNotes::showPasswordQueryDialog)
        .willOnce(invoke(setCurrentPasswordParams));
    EXPECT(CpPhoneNotes::cancelNote);
    EXPECT(CpPhoneNotes::showNotificationDialog);
    // completion of unknown barring type should not lead to updating
    EXPECT(PSetCallBarringWrapper::barringStatus).times(0);
    emit simulateCheckStateChange(Qt::Checked);
    m_barringpluginGroup->completeBarringStateChangeRequestHandling(
            KCurrentPassword, true);
    
    m_barringpluginGroup->enableBarringRequestCompleted(
        0, 
        PSetCallBarringWrapper::BarringTypeAllBarrings,
        PSetCallBarringWrapper::BarringStatusActive,
        false);
    
    QVERIFY(verify());
}


/*!
  UT_CpBarringPluginGroup::t_enableBAOCRequestCompletedStatusUpdate
 */
void UT_CpBarringPluginGroup::t_enableBAOCRequestCompletedStatusUpdate()
{
    int numOfChilds = m_barringpluginGroup->childCount();
    for (int childIndex = 0; childIndex < numOfChilds - 1; childIndex++) {
        HbDataFormModelItem *item = m_barringpluginGroup->childAt(childIndex);
        item->setContentWidgetData("checkState", QVariant(Qt::Checked));
    }
    
    QList<unsigned char> basicServiceGroupIds;
    EXPECT(CpItemDataHelper::widgetFromModelIndex).returns(this);
    EXPECT(CpPhoneNotes::showPasswordQueryDialog)
        .willOnce(invoke(setCurrentPasswordParams));
    emit simulateCheckStateChange(Qt::Checked);
    m_barringpluginGroup->completeBarringStateChangeRequestHandling(
            KCurrentPassword, true);
    
    EXPECT(PSetCallBarringWrapper::barringStatus).with(
        ServiceGroupVoice, PSetCallBarringWrapper::BarringTypeOutgoingInternational);
    EXPECT(PSetCallBarringWrapper::barringStatus).with(
        ServiceGroupVoice, 
        PSetCallBarringWrapper::BarringTypeOutgoingInternationalExceptToHomeCountry);
    m_barringpluginGroup->enableBarringRequestCompleted(
        0, 
        PSetCallBarringWrapper::BarringTypeAllOutgoing,
        PSetCallBarringWrapper::BarringStatusActive,
        false);
    m_barringpluginGroup->barringStatusRequestCompleted(
        PSetCallBarringWrapper::BarringErrorNone,
        basicServiceGroupIds, PSetCallBarringWrapper::BarringStatusActive);
    m_barringpluginGroup->barringStatusRequestCompleted(
        PSetCallBarringWrapper::BarringErrorNone,
        basicServiceGroupIds, PSetCallBarringWrapper::BarringStatusActive);
    QVERIFY(verify());
}


/*!
  UT_CpBarringPluginGroup::t_enableBOICRequestCompletedStatusUpdate
 */
void UT_CpBarringPluginGroup::t_enableBOICRequestCompletedStatusUpdate()
{
    QModelIndex modelIndex;
    int numOfChilds = m_barringpluginGroup->childCount();
    for (int childIndex = 0; childIndex < numOfChilds - 1; childIndex++) {
        HbDataFormModelItem *item = m_barringpluginGroup->childAt(childIndex);
        item->setContentWidgetData("checkState", QVariant(Qt::Checked));
        if (childIndex == 1) {
            modelIndex = m_dataFormModel->indexFromItem(item);
        }
    }
    
    QList<unsigned char> basicServiceGroupIds;
    EXPECT(CpItemDataHelper::widgetFromModelIndex).with(modelIndex).returns(this);
    EXPECT(CpPhoneNotes::showPasswordQueryDialog)
        .willOnce(invoke(setCurrentPasswordParams));
    emit simulateCheckStateChange(Qt::Checked);
    m_barringpluginGroup->completeBarringStateChangeRequestHandling(
            KCurrentPassword, true);
    
    EXPECT(PSetCallBarringWrapper::barringStatus).with(
        ServiceGroupVoice, 
        PSetCallBarringWrapper::BarringTypeAllOutgoing);
    EXPECT(PSetCallBarringWrapper::barringStatus).with(
        ServiceGroupVoice, 
        PSetCallBarringWrapper::BarringTypeOutgoingInternationalExceptToHomeCountry);
    m_barringpluginGroup->enableBarringRequestCompleted(
        0, 
        PSetCallBarringWrapper::BarringTypeOutgoingInternational,
        PSetCallBarringWrapper::BarringStatusActive,
        false);
    m_barringpluginGroup->barringStatusRequestCompleted(
        PSetCallBarringWrapper::BarringErrorNone,
        basicServiceGroupIds, PSetCallBarringWrapper::BarringStatusActive);
    m_barringpluginGroup->barringStatusRequestCompleted(
        PSetCallBarringWrapper::BarringErrorNone,
        basicServiceGroupIds, PSetCallBarringWrapper::BarringStatusActive);
    QVERIFY(verify());
}


/*!
  UT_CpBarringPluginGroup::t_enableBOICexHCRequestCompletedStatusUpdate
 */
void UT_CpBarringPluginGroup::t_enableBOICexHCRequestCompletedStatusUpdate()
{
    QModelIndex modelIndex;
    int numOfChilds = m_barringpluginGroup->childCount();
    for (int childIndex = 0; childIndex < numOfChilds - 1; childIndex++) {
        HbDataFormModelItem *item = m_barringpluginGroup->childAt(childIndex);
        item->setContentWidgetData("checkState", QVariant(Qt::Checked));
        if (childIndex == 2) {
            modelIndex = m_dataFormModel->indexFromItem(item);
        }
    }
    
    QList<unsigned char> basicServiceGroupIds;
    EXPECT(CpItemDataHelper::widgetFromModelIndex).with(modelIndex).returns(this);
    EXPECT(CpPhoneNotes::showPasswordQueryDialog)
        .willOnce(invoke(setCurrentPasswordParams));
    emit simulateCheckStateChange(Qt::Checked);
    m_barringpluginGroup->completeBarringStateChangeRequestHandling(
            KCurrentPassword, true);
    
    EXPECT(PSetCallBarringWrapper::barringStatus).with(
        ServiceGroupVoice, 
        PSetCallBarringWrapper::BarringTypeAllOutgoing);
    EXPECT(PSetCallBarringWrapper::barringStatus).with(
        ServiceGroupVoice, 
        PSetCallBarringWrapper::BarringTypeOutgoingInternational);
    EXPECT(PSetCallBarringWrapper::barringStatus).with(
        ServiceGroupVoice, 
        PSetCallBarringWrapper::BarringTypeOutgoingInternationalExceptToHomeCountry);
    m_barringpluginGroup->enableBarringRequestCompleted(
        0, 
        PSetCallBarringWrapper::BarringTypeOutgoingInternationalExceptToHomeCountry,
        PSetCallBarringWrapper::BarringStatusActive,
        false);
    m_barringpluginGroup->barringStatusRequestCompleted(
        PSetCallBarringWrapper::BarringErrorNone,
        basicServiceGroupIds, PSetCallBarringWrapper::BarringStatusActive);
    m_barringpluginGroup->barringStatusRequestCompleted(
        PSetCallBarringWrapper::BarringErrorNone,
        basicServiceGroupIds, PSetCallBarringWrapper::BarringStatusActive);
    m_barringpluginGroup->barringStatusRequestCompleted(
        PSetCallBarringWrapper::BarringErrorNone,
        basicServiceGroupIds, PSetCallBarringWrapper::BarringStatusActive);
    QVERIFY(verify());
}


/*!
  UT_CpBarringPluginGroup::t_enableBAICRequestCompletedStatusUpdate
 */
void UT_CpBarringPluginGroup::t_enableBAICRequestCompletedStatusUpdate()
{
    QModelIndex modelIndex;
    int numOfChilds = m_barringpluginGroup->childCount();
    for (int childIndex = 0; childIndex < numOfChilds - 1; childIndex++) {
        HbDataFormModelItem *item = m_barringpluginGroup->childAt(childIndex);
        item->setContentWidgetData("checkState", QVariant(Qt::Checked));
        if (childIndex == 3) {
            modelIndex = m_dataFormModel->indexFromItem(item);
        }
    }
    
    QList<unsigned char> basicServiceGroupIds;
    EXPECT(CpItemDataHelper::widgetFromModelIndex).with(modelIndex).returns(this);
    EXPECT(CpPhoneNotes::showPasswordQueryDialog)
        .willOnce(invoke(setCurrentPasswordParams));
    emit simulateCheckStateChange(Qt::Checked);
    m_barringpluginGroup->completeBarringStateChangeRequestHandling(
            KCurrentPassword, true);
    
    EXPECT(PSetCallBarringWrapper::barringStatus).with(
        ServiceGroupVoice, 
        PSetCallBarringWrapper::BarringTypeIncomingWhenRoaming);
    m_barringpluginGroup->enableBarringRequestCompleted(
        0, 
        PSetCallBarringWrapper::BarringTypeAllIncoming,
        PSetCallBarringWrapper::BarringStatusActive,
        false);
    m_barringpluginGroup->barringStatusRequestCompleted(
        PSetCallBarringWrapper::BarringErrorNone,
        basicServiceGroupIds, PSetCallBarringWrapper::BarringStatusActive);
    QVERIFY(verify());
}


/*!
  UT_CpBarringPluginGroup::t_enableBICRoamRequestCompletedStatusUpdate
 */
void UT_CpBarringPluginGroup::t_enableBICRoamRequestCompletedStatusUpdate()
{
    QModelIndex modelIndex;
    int numOfChilds = m_barringpluginGroup->childCount();
    for (int childIndex = 0; childIndex < numOfChilds - 1; childIndex++) {
        HbDataFormModelItem *item = m_barringpluginGroup->childAt(childIndex);
        item->setContentWidgetData("checkState", QVariant(Qt::Checked));
        if (childIndex == 4) {
            modelIndex = m_dataFormModel->indexFromItem(item);
        }
    }
    
    QList<unsigned char> basicServiceGroupIds;
    EXPECT(CpItemDataHelper::widgetFromModelIndex).with(modelIndex).returns(this);
    EXPECT(CpPhoneNotes::showPasswordQueryDialog)
        .willOnce(invoke(setCurrentPasswordParams));
    emit simulateCheckStateChange(Qt::Checked);
    m_barringpluginGroup->completeBarringStateChangeRequestHandling(
            KCurrentPassword, true);
    
    EXPECT(PSetCallBarringWrapper::barringStatus).with(
        ServiceGroupVoice, 
        PSetCallBarringWrapper::BarringTypeAllIncoming);
    m_barringpluginGroup->enableBarringRequestCompleted(
        0, 
        PSetCallBarringWrapper::BarringTypeIncomingWhenRoaming,
        PSetCallBarringWrapper::BarringStatusActive,
        false);
    m_barringpluginGroup->barringStatusRequestCompleted(
        PSetCallBarringWrapper::BarringErrorNone,
        basicServiceGroupIds, PSetCallBarringWrapper::BarringStatusActive);
    QVERIFY(verify());
}


/*!
  UT_CpBarringPluginGroup::t_disableBarringRequestCompleted
 */
void UT_CpBarringPluginGroup::t_disableBarringRequestCompleted()
{
// request completed succesfully -case
    EXPECT(CpItemDataHelper::widgetFromModelIndex).returns(this).times(1);
    EXPECT(CpPhoneNotes::showPasswordQueryDialog)
        .willOnce(invoke(setCurrentPasswordParams));
    EXPECT(CpPhoneNotes::cancelNote);
    EXPECT(CpPhoneNotes::showNotificationDialog);
    emit simulateCheckStateChange(Qt::Unchecked);
    m_barringpluginGroup->completeBarringStateChangeRequestHandling(
            KCurrentPassword, true);
    
    m_barringpluginGroup->disableBarringRequestCompleted(
        0, 
        PSetCallBarringWrapper::BarringTypeAllBarrings,
        PSetCallBarringWrapper::BarringStatusActive,
        false);
    
    QVERIFY(verify());
    
// request completed with an error -case
    HbDataFormModelItem *item = m_barringpluginGroup->childAt(0);
    item->setContentWidgetData("checkState", QVariant(Qt::Unchecked));
    EXPECT(CpItemDataHelper::widgetFromModelIndex).returns(this);
    EXPECT(CpPhoneNotes::showPasswordQueryDialog)
        .willOnce(invoke(setCurrentPasswordParams));
    EXPECT(CpItemDataHelper::removeConnection);
    EXPECT(CpItemDataHelper::addConnection);
    EXPECT(CpPhoneNotes::cancelNote);
    EXPECT(CpPhoneNotes::showGlobalErrorNote);
    
    emit simulateCheckStateChange(Qt::Unchecked);
    m_barringpluginGroup->completeBarringStateChangeRequestHandling(
            KCurrentPassword, true);
    
    m_barringpluginGroup->disableBarringRequestCompleted(
        -1, 
        PSetCallBarringWrapper::BarringTypeAllBarrings,
        PSetCallBarringWrapper::BarringStatusActive,
        false);
    
    QVERIFY(verify());
}


/*!
  UT_CpBarringPluginGroup::t_barringPasswordChangeRequestCompleted
 */
void UT_CpBarringPluginGroup::t_barringPasswordChangeRequestCompleted()
{
// request completed with no error
    EXPECT(CpPhoneNotes::cancelNote);
    EXPECT(CpPhoneNotes::showNotificationDialog);
    
    m_barringpluginGroup->barringPasswordChangeRequestCompleted(0);
    
    QVERIFY(verify());

// request completed with an error
    EXPECT(CpPhoneNotes::cancelNote);
    EXPECT(CpPhoneNotes::showGlobalErrorNote);
    
    m_barringpluginGroup->barringPasswordChangeRequestCompleted(-1);
    
    QVERIFY(verify());
}


/*!
  UT_CpBarringPluginGroup::t_changeBarringStateRequested
 */
void UT_CpBarringPluginGroup::t_changeBarringStateRequested()
{
// barring enable request case
    EXPECT(CpItemDataHelper::widgetFromModelIndex).returns(this);
    EXPECT(CpPhoneNotes::showPasswordQueryDialog)
        .willOnce(invoke(setCurrentPasswordParams));
    EXPECT(PSetCallBarringWrapper::enableBarring)
        .with(ServiceGroupVoice, 
            PSetCallBarringWrapper::BarringTypeAllOutgoing, KCurrentPassword);
    EXPECT(CpPhoneNotes::showGlobalProgressNote);
    
    HbDataFormModelItem *item = m_barringpluginGroup->childAt(0);
    item->setContentWidgetData("checkState", QVariant(Qt::Checked));
    emit simulateCheckStateChange(Qt::Checked);
    m_barringpluginGroup->completeBarringStateChangeRequestHandling(
            KCurrentPassword, true);
    
    QVERIFY(verify());
    
// barring disable request case
    EXPECT(CpItemDataHelper::widgetFromModelIndex).returns(this);
    EXPECT(CpPhoneNotes::showPasswordQueryDialog)
        .willOnce(invoke(setCurrentPasswordParams));
    EXPECT(PSetCallBarringWrapper::disableBarring)
        .with(ServiceGroupVoice, 
            PSetCallBarringWrapper::BarringTypeAllOutgoing, KCurrentPassword);
    EXPECT(CpPhoneNotes::showGlobalProgressNote);
    
    item = m_barringpluginGroup->childAt(0);
    item->setContentWidgetData("checkState", QVariant(Qt::Unchecked));
    emit simulateCheckStateChange(Qt::Unchecked);
    m_barringpluginGroup->completeBarringStateChangeRequestHandling(
            KCurrentPassword, true);
    
    QVERIFY(verify());

// cancel pressed while querying barring password
    EXPECT(CpItemDataHelper::widgetFromModelIndex).returns(this);
    EXPECT(CpPhoneNotes::showPasswordQueryDialog)
        .willOnce(invoke(setPasswordParamsCancel));
    EXPECT(CpItemDataHelper::removeConnection);
    EXPECT(CpItemDataHelper::addConnection);
    EXPECT(PSetCallBarringWrapper::disableBarring).times(0);
    EXPECT(CpPhoneNotes::showGlobalProgressNote).times(0);
    
    item = m_barringpluginGroup->childAt(0);
    item->setContentWidgetData("checkState", QVariant(Qt::Unchecked));
    emit simulateCheckStateChange(Qt::Unchecked);
    m_barringpluginGroup->completeBarringStateChangeRequestHandling(
                "", false);
    
    QVERIFY(verify());
    
// barring item not found case
    EXPECT(PSetCallBarringWrapper::enableBarring).times(0);
    EXPECT(CpPhoneNotes::showGlobalProgressNote).times(0);
    
    emit simulateCheckStateChange(Qt::Checked);
    
    QVERIFY(verify());
}


/*!
  UT_CpBarringPluginGroup::t_changeBarringPasswordRequested
 */
void UT_CpBarringPluginGroup::t_changeBarringPasswordRequested()
{
    connect(
        this, SIGNAL(simulateEditPasswordButtonClicked(bool)), 
        m_barringpluginGroup, SLOT(changeBarringPasswordRequested()));
    
    // cancel from current password query
    EXPECT(CpPhoneNotes::showPasswordQueryDialog)
        .willOnce(invoke(setPasswordParamsCancel));
    emit simulateEditPasswordButtonClicked(false);
    m_barringpluginGroup->changeBarringPasswordPhasesHandling("", false);
    QVERIFY(verify());
    
    // cancel from new password query
    EXPECT(CpPhoneNotes::showPasswordQueryDialog)
        .willOnce(invoke(setCurrentPasswordParams));
    EXPECT(CpPhoneNotes::showPasswordQueryDialog)
        .willOnce(invoke(setPasswordParamsCancel));
    emit simulateEditPasswordButtonClicked(false);
    m_barringpluginGroup->changeBarringPasswordPhasesHandling(
            KCurrentPassword, true);
    m_barringpluginGroup->changeBarringPasswordPhasesHandling(
            "", false);
    QVERIFY(verify());
    
    // cancel from verify new password query
    EXPECT(CpPhoneNotes::showPasswordQueryDialog)
        .willOnce(invoke(setCurrentPasswordParams));
    EXPECT(CpPhoneNotes::showPasswordQueryDialog)
        .willOnce(invoke(setNewAndVerifiedPasswordParams));
    EXPECT(CpPhoneNotes::showPasswordQueryDialog)
        .willOnce(invoke(setPasswordParamsCancel));
    emit simulateEditPasswordButtonClicked(false);
    m_barringpluginGroup->changeBarringPasswordPhasesHandling(
            KCurrentPassword, true);
    m_barringpluginGroup->changeBarringPasswordPhasesHandling(
            KNewAndVerifiedPassword, true);
    m_barringpluginGroup->changeBarringPasswordPhasesHandling(
            "", false);
    QVERIFY(verify());
    
    // all data successfully queried
    EXPECT(CpPhoneNotes::showPasswordQueryDialog)
        .willOnce(invoke(setCurrentPasswordParams));
    EXPECT(CpPhoneNotes::showPasswordQueryDialog)
        .willOnce(invoke(setNewAndVerifiedPasswordParams));
    EXPECT(CpPhoneNotes::showPasswordQueryDialog)
        .willOnce(invoke(setNewAndVerifiedPasswordParams));
    EXPECT(PSetCallBarringWrapper::changeBarringPassword)
        .with(KCurrentPassword, KNewAndVerifiedPassword, KNewAndVerifiedPassword);
    emit simulateEditPasswordButtonClicked(false);
    m_barringpluginGroup->changeBarringPasswordPhasesHandling(
            KCurrentPassword, true);
    m_barringpluginGroup->changeBarringPasswordPhasesHandling(
            KNewAndVerifiedPassword, true);
    m_barringpluginGroup->changeBarringPasswordPhasesHandling(
            KNewAndVerifiedPassword, true);
    QVERIFY(verify());
}

QTEST_MAIN_S60(UT_CpBarringPluginGroup)
