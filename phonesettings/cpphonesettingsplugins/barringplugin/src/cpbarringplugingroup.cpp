/*
 * Copyright (c) 2009-2010 Nokia Corporation and/or its subsidiary(-ies).
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

#include <hbdataformmodel.h>
#include <hbdataformmodelitem.h>
#include <HbCheckBox>
#include <QTranslator>
#include <QLocale>
#include <QApplication>
#include <QTimer>
#include <cpitemdatahelper.h>
#include <psetwrapper.h>
#include <psetcallbarringwrapper.h>
#include "cpbarringplugingroup.h"
#include "cpplugincommon.h"
#include "cpphonenotes.h"
#include "cppluginlogging.h"

Q_DECLARE_METATYPE(PSetCallBarringWrapper::BarringType)

// TODO: use logical identifiers for texts
// TODO: barring password implementation

/*!
  CpBarringPluginGroup::CpBarringPluginGroup.
 */
CpBarringPluginGroup::CpBarringPluginGroup(CpItemDataHelper &helper)
    :
    CpSettingFormItemData(
        HbDataFormModelItem::GroupItem, hbTrId("Call barring"), 0),
    m_helper(helper),
    m_pSetWrapper(0),
    m_barringWrapper(0),
    m_allOutgoingBarringItem(0),
    m_outgoingInternationalBarringItem(0),
    m_outgoingInternationalExceptToHomeCountryBarringItem(0),
    m_allIncomingBarringItem(0),
    m_incomingWhenRoamingBarringItem(0),
    m_barringStatusRequestOngoing(false),
    m_activeNoteId(0),
    m_phoneNotes(NULL)
{
    DPRINT << ": IN";
    
    // Registration needed, because BarringType is used as a custom meta 
    // information for barring items.
    qRegisterMetaType<PSetCallBarringWrapper::BarringType>(
        "PSetCallBarringWrapper::BarringType");
    
    setupLocalization();
    
    m_pSetWrapper = new PSetWrapper(this); 
    m_barringWrapper = &m_pSetWrapper->callBarringWrapper(); 
    connectToWrapper();
    
    // itemShown signal is used to trigger barring status query process 
    helper.connectToForm(
        SIGNAL(itemShown(QModelIndex)), 
        this, SLOT(itemShown(QModelIndex)));
    
    createAllOutgoingBarringItem();
    createOutgoingInternationalBarringItem();
    createOutgoingInternationalExceptToHomeCountryBarringItem();
    createAllIncomingBarringItem();
    createIncomingWhenRoamingBarringItem();
    
    m_phoneNotes = CpPhoneNotes::instance();
    
    DPRINT << ": OUT";
}


/*!
  CpBarringPluginGroup::~CpBarringPluginGroup.
 */
CpBarringPluginGroup::~CpBarringPluginGroup()
{
    DPRINT << ": IN";
    
    DPRINT << ": OUT";
}


/*!
  CpBarringPluginGroup::itemShown.
 */
void CpBarringPluginGroup::itemShown(const QModelIndex& item)
{
    DPRINT << ": IN";
    
    CpSettingFormItemData* formItem = 
        static_cast<CpSettingFormItemData*>(
            qobject_cast<HbDataFormModel*>(model())->itemFromIndex(item));
    
    if (formItem->contentWidgetData("checkState").isValid()) {
        // Initial status for the barring item is already queried. Do not 
        // start querying again if user does close/open for the barring group.
        return;
    }
    
    // start barring status query
    m_barringRequestQueue.enqueue(formItem);
    processBarringStatusRequestQueue();
}


/*!
  CpBarringPluginGroup::setupLocalization.
 */
void CpBarringPluginGroup::setupLocalization()
{
    DPRINT << ": IN";
    
    QTranslator translator; 
    QString lang = QLocale::system().name();
    QString path = "z:/resource/qt/translations/";
    QString fullName = path + "telephone_cp_" + lang;
    
    DPRINT << ": loading translation:" << fullName;
    bool translatorLoaded = translator.load(fullName);
    DPRINT << ": translator loaded: " << translatorLoaded; 
    
    if (translatorLoaded) {
        qApp->installTranslator(&translator);
        DPRINT << ": translator installed"; 
    }
}


/*!
  CpBarringPluginGroup::connectToWrapper.
 */
void CpBarringPluginGroup::connectToWrapper()
{
    DPRINT << ": IN";
    
    QObject::connect(
        m_barringWrapper, 
        SIGNAL(barringStatusRequestCompleted(
            int,
            const QList<unsigned char> &, 
            PSetCallBarringWrapper::BarringStatus)
        ),
        this, 
        SLOT(barringStatusRequestCompleted(
            int,
            const QList<unsigned char> &, 
            PSetCallBarringWrapper::BarringStatus)
        )
    );
    
    QObject::connect(
        m_barringWrapper, 
        SIGNAL(enableBarringRequestCompleted(
            int,
            PSetCallBarringWrapper::BarringType,
            PSetCallBarringWrapper::BarringStatus,
            bool)
        ),
        this, 
        SLOT(enableBarringRequestCompleted(
            int,
            PSetCallBarringWrapper::BarringType,
            PSetCallBarringWrapper::BarringStatus,
            bool)
        )
    );
    
    QObject::connect(
        m_barringWrapper, 
        SIGNAL(disableBarringRequestCompleted(
            int,
            PSetCallBarringWrapper::BarringType,
            PSetCallBarringWrapper::BarringStatus,
            bool)
        ),
        this, 
        SLOT(disableBarringRequestCompleted(
            int,
            PSetCallBarringWrapper::BarringType,
            PSetCallBarringWrapper::BarringStatus,
            bool)
        )
    );
}


/*!
  CpBarringPluginGroup::createAllOutgoingBarringItem.
 */
void CpBarringPluginGroup::createAllOutgoingBarringItem()
{
    DPRINT << ": IN";
    
    m_allOutgoingBarringItem = new CpSettingFormItemData(
        HbDataFormModelItem::CheckBoxItem, hbTrId(""), this);
    
    m_allOutgoingBarringItem->setContentWidgetData(
        "text", QVariant(hbTrId("Outgoing calls")));
    
    QVariant value;
    value.setValue(PSetCallBarringWrapper::BarringTypeAllOutgoing);
    m_allOutgoingBarringItem->setProperty("barringType", value);
    
    appendChild(m_allOutgoingBarringItem);
}


/*!
  CpBarringPluginGroup::createOutgoingInternationalBarringItem.
 */
void CpBarringPluginGroup::createOutgoingInternationalBarringItem()
{
    DPRINT << ": IN";
    
    m_outgoingInternationalBarringItem = new CpSettingFormItemData(
        HbDataFormModelItem::CheckBoxItem, hbTrId(""), this);
    
    m_outgoingInternationalBarringItem->setContentWidgetData(
        "text", QVariant(hbTrId("International calls")));
    
    QVariant value;
    value.setValue(PSetCallBarringWrapper::BarringTypeOutgoingInternational);
    m_outgoingInternationalBarringItem->setProperty("barringType", value);
    
    appendChild(m_outgoingInternationalBarringItem);
}


/*!
  CpBarringPluginGroup::
      createOutgoingInternationalExceptToHomeCountryBarringItem.
 */
void CpBarringPluginGroup::
    createOutgoingInternationalExceptToHomeCountryBarringItem()
{
    DPRINT << ": IN";
    
    m_outgoingInternationalExceptToHomeCountryBarringItem = 
        new CpSettingFormItemData(
            HbDataFormModelItem::CheckBoxItem, hbTrId(""), this);
    
    m_outgoingInternationalExceptToHomeCountryBarringItem->setContentWidgetData(
        "text", QVariant(hbTrId("International calls except to home country")));
    
    QVariant value;
    value.setValue(PSetCallBarringWrapper::BarringTypeOutgoingInternationalExceptToHomeCountry);
    m_outgoingInternationalExceptToHomeCountryBarringItem->setProperty(
        "barringType", value);
    
    appendChild(m_outgoingInternationalExceptToHomeCountryBarringItem);
    
}


/*!
  CpBarringPluginGroup::createAllIncomingBarringItem.
 */
void CpBarringPluginGroup::createAllIncomingBarringItem()
{
    DPRINT << ": IN";
    
    m_allIncomingBarringItem = new CpSettingFormItemData(
        HbDataFormModelItem::CheckBoxItem, hbTrId(""), this);
    
    m_allIncomingBarringItem->setContentWidgetData(
        "text", QVariant(hbTrId("Incoming calls")));
    
    QVariant value;
    value.setValue(PSetCallBarringWrapper::BarringTypeAllIncoming);
    m_allIncomingBarringItem->setProperty("barringType", value);
    
    appendChild(m_allIncomingBarringItem);
}


/*!
  CpBarringPluginGroup::createIncomingWhenRoamingBarringItem.
 */
void CpBarringPluginGroup::createIncomingWhenRoamingBarringItem()
{
    DPRINT << ": IN";
    
    m_incomingWhenRoamingBarringItem = new CpSettingFormItemData(
        HbDataFormModelItem::CheckBoxItem, hbTrId(""), this);
    
    m_incomingWhenRoamingBarringItem->setContentWidgetData(
        "text", QVariant(hbTrId("Incoming calls when abroad")));
    
    QVariant value;
    value.setValue(PSetCallBarringWrapper::BarringTypeIncomingWhenRoaming);
    m_incomingWhenRoamingBarringItem->setProperty("barringType", value);
    
    appendChild(m_incomingWhenRoamingBarringItem);
}


/*!
  CpBarringPluginGroup::barringStatusRequestCompleted.
 */
void CpBarringPluginGroup::barringStatusRequestCompleted(
    int result,
    const QList<unsigned char> & basicServiceGroupIds,
    PSetCallBarringWrapper::BarringStatus status)
{
    DPRINT << ": IN";
    Q_UNUSED(result)
    Q_UNUSED(basicServiceGroupIds)
    
    m_barringStatusRequestOngoing = false;
    
    if (PSetCallBarringWrapper::BarringErrorNone != result) {
        // Stop status query process for this time. Statuses are tried to 
        // query again for uncompleted items when user expands/opens barring 
        // view again.
        m_phoneNotes->cancelNote(m_activeNoteId);
        m_phoneNotes->showGlobalErrorNote(m_activeNoteId, result);
        m_barringRequestQueue.clear();
        return;
    }
    
    // Update check state to correct value. After setting valid value here
    // status query will not be started again for the item when user collapses
    // and expands view again.
    Qt::CheckState checkState = 
        (PSetCallBarringWrapper::BarringStatusActive == status) 
            ? Qt::Checked 
            : Qt::Unchecked;
    CpSettingFormItemData *itemForCompletedRequest = 
        m_barringRequestQueue.dequeue();
    itemForCompletedRequest->setContentWidgetData(
        "checkState", QVariant(checkState));
    
    // start to observe user initiated state changes
    m_helper.addConnection(
        itemForCompletedRequest, SIGNAL(stateChanged(int)),
        this, SLOT(changeBarringStateRequested(int)));
    
    if (m_barringRequestQueue.isEmpty()) {
        m_phoneNotes->cancelNote(m_activeNoteId);
    } else {
        processBarringStatusRequestQueue();
    }
}


/*!
  CpBarringPluginGroup::enableBarringRequestCompleted.
 */
void CpBarringPluginGroup::enableBarringRequestCompleted(
    int result,
    PSetCallBarringWrapper::BarringType barringType,
    PSetCallBarringWrapper::BarringStatus barringStatus, 
    bool plural)
{
    DPRINT << ": IN";
    Q_UNUSED(barringType)
    Q_UNUSED(barringStatus)
    Q_UNUSED(plural)
    
    m_phoneNotes->cancelNote(m_activeNoteId);
    
    CpSettingFormItemData *barringItem = m_barringRequestQueue.dequeue();
    if (PSetCallBarringWrapper::BarringErrorNone == result) {
        m_phoneNotes->showGlobalNote(
            m_activeNoteId, 
            hbTrId("Barring activated"),
            HbMessageBox::MessageTypeInformation);
    } else {
        revertCheckStateForItem(barringItem);
        
        m_phoneNotes->showGlobalErrorNote(m_activeNoteId, result);
    }
}


/*!
  CpBarringPluginGroup::disableBarringRequestCompleted.
 */
void CpBarringPluginGroup::disableBarringRequestCompleted(
    int result,
    PSetCallBarringWrapper::BarringType barringType,
    PSetCallBarringWrapper::BarringStatus barringStatus, 
    bool plural)
{
    DPRINT << ": IN";
    Q_UNUSED(barringType)
    Q_UNUSED(barringStatus)
    Q_UNUSED(plural)
    
    m_phoneNotes->cancelNote(m_activeNoteId);
    
    CpSettingFormItemData *barringItem = m_barringRequestQueue.dequeue();
    if (PSetCallBarringWrapper::BarringErrorNone == result) {
        m_phoneNotes->showGlobalNote(
            m_activeNoteId, 
            hbTrId("Barring cancelled"),
            HbMessageBox::MessageTypeInformation);
    } else {
        revertCheckStateForItem(barringItem);
        
        m_phoneNotes->showGlobalErrorNote(m_activeNoteId, result);
    }
}


/*!
  CpBarringPluginGroup::processBarringStatusRequestQueue.
 */
void CpBarringPluginGroup::processBarringStatusRequestQueue()
{
    DPRINT << ": IN";

    if ((m_barringStatusRequestOngoing == false) && 
        (!m_barringRequestQueue.isEmpty())) {
        
        CpSettingFormItemData *item = m_barringRequestQueue.head();
        PSetCallBarringWrapper::BarringType barringType =
            qvariant_cast<PSetCallBarringWrapper::BarringType>(
                item->property("barringType"));
        m_barringWrapper->barringStatus(ServiceGroupVoice, barringType);
        
        m_barringStatusRequestOngoing = true;
        
        if (0 == m_activeNoteId) {
            // Status request note is very first note to show. Launch progress
            // note only once for status update.
            m_phoneNotes->showGlobalProgressNote(
                m_activeNoteId, hbTrId("Requesting..."));
        }
    }
}


/*!
  CpBarringPluginGroup::revertCheckStateForItem.
 */
void CpBarringPluginGroup::revertCheckStateForItem(
    CpSettingFormItemData *barringItem)
{
    m_helper.removeConnection(
        barringItem, SIGNAL(stateChanged(int)),
        this, SLOT(changeBarringStateRequested(int)));
    
    HbDataFormModel *formModel = qobject_cast<HbDataFormModel*>(model());
    QModelIndex modelIndex = formModel->indexFromItem(barringItem);
    HbCheckBox *checkBox = static_cast<HbCheckBox*>(
        m_helper.widgetFromModelIndex(modelIndex));
    Qt::CheckState revertedCheckState = 
        (checkBox->checkState() == Qt::Checked) ? Qt::Unchecked : Qt::Checked;
    checkBox->setCheckState(revertedCheckState);
    
    m_helper.addConnection(
        barringItem, SIGNAL(stateChanged(int)),
        this, SLOT(changeBarringStateRequested(int)));
}


/*!
  CpBarringPluginGroup::changeBarringStateRequested.
 */
void CpBarringPluginGroup::changeBarringStateRequested(int checkState)
{
    QObject *signalSender = sender();
    
    // find form item for which user has requested barring status change
    HbDataFormModel *formModel = qobject_cast<HbDataFormModel*>(model());
    CpSettingFormItemData* barringItem = NULL;
    int numOfChilds = childCount();
    for (int i = 0; (i < numOfChilds) && (barringItem == NULL); i++) {
        HbDataFormModelItem* itemCandidate = childAt(i);
        QModelIndex modelIndex = formModel->indexFromItem(itemCandidate);
        if (signalSender == m_helper.widgetFromModelIndex(modelIndex)) {
            barringItem = static_cast<CpSettingFormItemData*>(itemCandidate);
        }
    }
    
    // TODO: remove hardcoded password when setting of password is implemented
    if (NULL != barringItem) {
        if (Qt::Checked == checkState) {
            m_barringWrapper->enableBarring(
                ServiceGroupVoice,
                qvariant_cast<PSetCallBarringWrapper::BarringType>(
                    barringItem->property("barringType")), 
                QString("1234"));
        } else {
            m_barringWrapper->disableBarring(
                ServiceGroupVoice,
                qvariant_cast<PSetCallBarringWrapper::BarringType>(
                    barringItem->property("barringType")), 
                QString("1234"));
        }
        
        m_barringRequestQueue.enqueue(barringItem);
        m_phoneNotes->showGlobalProgressNote(
            m_activeNoteId, hbTrId("Requesting..."));
    }
}

// End of File. 
