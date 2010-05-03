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

#include <hbdataformmodel.h>
#include <hbdataformmodelitem.h>
#include <hblineedit.h>
#include <hbinputeditorinterface.h>
#include <hbcombobox.h>
#include <hbdeviceprogressdialog.h>
#include <sssettingswrapper.h>
#include <psetwrapper.h>
#include <hbaction.h>
#include <hblistwidget.h>
#include <hblistwidgetitem.h>
#include <hblabel.h>
#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <cpitemdatahelper.h>
#include "cpdivertselectionitem.h"
#include "cpdivertplugingroup.h"
#include "cpplugincommon.h"
#include "cpphonenotes.h"
#include "cppluginlogging.h"
#include "cpdivertitemdata.h"
#include "cpdivertselectioncustomitem.h"

Q_DECLARE_METATYPE(PsCallDivertingCondition)
Q_DECLARE_METATYPE(PsServiceGroup)

/*!
  CpDivertPluginGroup::CpDivertPluginGroup.
 */
CpDivertPluginGroup::CpDivertPluginGroup(CpItemDataHelper &helper)
     : CpSettingFormItemData(HbDataFormModelItem::GroupItem, 
                             hbTrId("txt_phone_subhead_call_divert"),0),
       m_DataItemVoiceAllCalls(NULL),
       m_DataItemVoiceIfBusy(NULL),
       m_DataItemVoiceIfNotAnswered(NULL),
       m_DataItemVoiceIfOutOfReach(NULL),
       m_DataItemVoiceIfNotAvailable(NULL),
       m_DataItemVideoAllCalls(NULL),
       m_DataItemVideoIfBusy(NULL),
       m_DataItemVideoIfNotAnswered(NULL),
       m_DataItemVideoIfOutOfReach(NULL),
       m_DataItemVideoIfNotAvailable(NULL),
       m_activeNoteId(0),
       m_activeProgressNoteId(0),
       m_divertToVoiceMailBox(false),
       m_helper(helper)
{
    DPRINT << ": IN";
    
    // Registration needed, because PsCallDivertingCondition and PsServiceGroup
    // is used as a custom meta information for barring items.
    qRegisterMetaType<PsCallDivertingCondition>(
        "PsCallDivertingCondition");
    
    qRegisterMetaType<PsServiceGroup>(
        "PsServiceGroup");
    
    // Localization file loading
    QTranslator translator; 
    QString lang = QLocale::system().name();
    QString path = "z:/resource/qt/translations/";
    DPRINT << ": loading translation: " << QString(path + "telephone_cp_" + lang);
    bool translatorLoaded = translator.load(path + "telephone_cp_" + lang);
    DPRINT << ": translator loaded: " << translatorLoaded; 
    if (translatorLoaded) {
        qApp->installTranslator(&translator);
        DPRINT << ": translator installed"; 
    }
    
    m_pSetWrapper = new PSetWrapper; 
    DPRINT << ": PSetWrapper created";
    
    m_ssSettingsWrapper = new SsSettingsWrapper; 
    DPRINT << ": SsSettingsWrapper created";
    
    m_callDivertingWrapper = &m_pSetWrapper->callDivertingWrapper();
    QObject::connect(
        m_callDivertingWrapper, 
        SIGNAL(handleDivertingChanged(const PSCallDivertingCommand&,bool)),
        this, 
        SLOT(handleDivertingChanged(const PSCallDivertingCommand&,bool)));
    QObject::connect(
        m_callDivertingWrapper, 
        SIGNAL(handleDivertingStatus(QList<PSCallDivertingStatus*>&, bool)),
        this, 
        SLOT(handleDivertingStatus(QList<PSCallDivertingStatus*>&, bool)));
    QObject::connect(
        m_callDivertingWrapper, 
        SIGNAL(handleDivertingError(int)),
        this, 
        SLOT(handleDivertingError(int)));
    
    QObject::connect(
        m_callDivertingWrapper, 
        SIGNAL(requestDone()),
        this,
        SLOT(divertRequestProcessed()));

    // Create custom item prototype
    m_helper.addItemPrototype(new CpDivertSelectionItem());
    
    // Listen form item visibility change
    m_helper.connectToForm(
            SIGNAL(itemShown(QModelIndex)), this, SLOT(itemShown(QModelIndex)));
    
    // Create grouped setting items
    createVoiceCallItems(this);
    createVideoCallItems(this);
    DPRINT << ": OUT";
}

/*!
  CpDivertPluginGroup::~CpDivertPluginGroup.
 */
CpDivertPluginGroup::~CpDivertPluginGroup()
{
    DPRINT << ": IN";

    delete m_pSetWrapper;
    delete m_ssSettingsWrapper;
    
    DPRINT << ": OUT";
}

/*!
 CpDivertPluginGroup::createVoiceCallItems.
 */
void CpDivertPluginGroup::createVoiceCallItems(CpSettingFormItemData *parent)
{
    DPRINT << ": IN";
    CpSettingFormItemData *page = new CpSettingFormItemData(
                HbDataFormModelItem::GroupPageItem,
                hbTrId("txt_phone_setlabel_service_val_voice_divert"));
    parent->appendChild(page);

    m_DataItemVoiceAllCalls = createDivertItem(
            DivertConditionUnconditional,
            ServiceGroupVoice,
            hbTrId("txt_phone_setlabel_all_calls"),
            hbTrId("txt_phone_setlabel_all_calls"), false,
            page);

    m_DataItemVoiceIfBusy = createDivertItem(
            DivertConditionBusy,
            ServiceGroupVoice,
            hbTrId("txt_phone_setlabel_if_busy"),
            hbTrId("txt_phone_setlabel_if_busy"), false,
            page);

    m_DataItemVoiceIfNotAnswered = createDivertItem(
            DivertConditionNoReply,
            ServiceGroupVoice,
            hbTrId("txt_phone_setlabel_if_not_answered"),
            hbTrId("txt_phone_setlabel_if_not_answered"), true,
            page);
    m_DataItemVoiceIfNotAnswered->setContentWidgetData(
            "timeoutText", hbTrId("txt_phone_setlabel_delay"));

    m_DataItemVoiceIfOutOfReach = createDivertItem(
            DivertConditionNotReachable,
            ServiceGroupVoice,
            hbTrId("txt_phone_setlabel_if_out_of_reach"),
            hbTrId("txt_phone_setlabel_if_out_of_reach"), false,
            page);

    m_DataItemVoiceIfNotAvailable = createDivertItem(
            DivertConditionAllConditionalCases,
            ServiceGroupVoice,
            hbTrId("txt_phone_setlabel_if_not_available"),
            hbTrId("txt_phone_setlabel_if_not_available"), true,
            page);

    DPRINT << ": OUT";
}

/*!
 CpDivertPluginGroup::createVideoCallItems
 */
void CpDivertPluginGroup::createVideoCallItems(CpSettingFormItemData *parent)
{
    DPRINT << ": IN";
    CpSettingFormItemData *page = new CpSettingFormItemData(
            HbDataFormModelItem::GroupPageItem,
            hbTrId("txt_phone_setlabel_service_val_video_divert"));
    
    parent->appendChild(page);
    
    m_DataItemVideoAllCalls = createDivertItem(
            DivertConditionUnconditional,
            ServiceGroupData,
            hbTrId("txt_phone_setlabel_all_calls"),
            hbTrId("txt_phone_setlabel_all_calls"), false,
            page);

    m_DataItemVideoIfBusy = createDivertItem(
            DivertConditionBusy,
            ServiceGroupData,
            hbTrId("txt_phone_setlabel_if_busy"),
            hbTrId("txt_phone_setlabel_if_busy"), false,
            page);

    m_DataItemVideoIfNotAnswered = createDivertItem(
            DivertConditionNoReply,
            ServiceGroupData,
            hbTrId("txt_phone_setlabel_if_not_answered"),
            hbTrId("txt_phone_setlabel_if_not_answered"), true,
            page);
    m_DataItemVideoIfNotAnswered->setContentWidgetData(
            "timeoutText", hbTrId("txt_phone_setlabel_delay"));

    m_DataItemVideoIfOutOfReach = createDivertItem(
            DivertConditionNotReachable,
            ServiceGroupData,
            hbTrId("txt_phone_setlabel_if_out_of_reach"),
            hbTrId("txt_phone_setlabel_if_out_of_reach"), false,
            page);

    m_DataItemVideoIfNotAvailable = createDivertItem(
            DivertConditionAllConditionalCases,
            ServiceGroupData,
            hbTrId("txt_phone_setlabel_if_not_available"),
            hbTrId("txt_phone_setlabel_if_not_available"), true,
            page);
    
    DPRINT << ": OUT";
}

/*!
 CpDivertPluginGroup::createDivertItem
 */
CpDivertItemData *CpDivertPluginGroup::createDivertItem(
        PsCallDivertingCondition condition,
        PsServiceGroup serviceGroup,
        const QString &label,
        const QString &queryLabel,
        bool needTimeoutInfo,
        CpSettingFormItemData *parent)
{
    DPRINT << ": IN";

    CpDivertItemData *item = new CpDivertItemData(
        static_cast<HbDataFormModelItem::DataItemType>
            (CpDivertSelectionItem::CpDivertSelectionItemId), 
            label,
            m_helper,
            parent);
    QVariant conditionVar;
    conditionVar.setValue(condition);
    item->setProperty("condition", conditionVar);
    QVariant serviceGroupVar;
    serviceGroupVar.setValue(serviceGroup);
    item->setProperty("serviceGroup", serviceGroupVar);
    item->setProperty("queryLabel", queryLabel);
    item->setProperty("needTimeoutInfo", needTimeoutInfo);
    item->setEnabled(false);
    
    // Connect signals
    QObject::connect(
        item, SIGNAL(itemClicked(CpDivertItemData&)),
        this, SLOT(changeDivertingStateRequested(CpDivertItemData&)));
    
    parent->appendChild(item);
    DPRINT << ": OUT";
    return item;
}

/*!
  CpDivertPluginGroup::itemShown.
 */
void CpDivertPluginGroup::itemShown(const QModelIndex& item)
{
    DPRINT << ": IN";
    DPRINT << "item:" << item;
    HbDataFormModelItem* modelItem = 
            qobject_cast<HbDataFormModel*>(model())->itemFromIndex(item);
    
    if (!modelItem->contentWidgetData("number").isValid() &&
        (static_cast<HbDataFormModelItem::DataItemType>
            (CpDivertSelectionItem::CpDivertSelectionItemId == modelItem->type()))){
        CpDivertItemData *item = static_cast<CpDivertItemData*>(modelItem); 
        if(qvariant_cast<PsCallDivertingCondition>(item->property("condition")) !=
                DivertConditionAllConditionalCases) {
            addToDivertingRequestQueue(CheckDivertStatus, *item);
        }
    }

    DPRINT << ": OUT";
}

/*!
  CpDivertPluginGroup::addToDivertingRequestQueue.
 */
void CpDivertPluginGroup::addToDivertingRequestQueue(
        DivertRequest request, CpDivertItemData &item)
{
    DPRINT << ": IN";
    CpDivertRequestQueueItem i;
    i.request = request;
    i.item = &item;
    m_divertRequestQueue.enqueue(i);

    if (m_divertRequestQueue.count()==1) {
       // Process if first item was added, process other later
        try {
            processDivertingRequestQueue();
        } catch(...) {
            DPRINT << "error!!";
        }
    }


    DPRINT << ": OUT";
}

/*!
  CpDivertPluginGroup::divertRequestProcessed.
 */
void CpDivertPluginGroup::divertRequestProcessed()
{
    DPRINT << ": IN";
    // Remove previous request and process next one
    if (!m_divertRequestQueue.isEmpty()) {
        m_divertRequestQueue.dequeue();
        processDivertingRequestQueue();
    }

    if (m_divertRequestQueue.isEmpty()) {
        // Queue empty so cancel process note
        CpPhoneNotes::instance()->cancelNote(m_activeProgressNoteId);
    }
    
    DPRINT << ": OUT";
}

/*!
  CpDivertPluginGroup::changeItemData.
 */
void CpDivertPluginGroup::changeItemData(
        PsServiceGroup serviceGroup,
        PsCallDivertingCondition condition,
        PsCallDivertingStatus status,
        const QString& number, int timeout)
{
    DPRINT << ": IN";
    
    CpDivertSelectionCustomitem::State itemState = 
        CpDivertSelectionCustomitem::Disabled;
    if (status == DivertingStatusActive) {
        itemState = CpDivertSelectionCustomitem::Enabled;
    } else if (status == DivertingStatusInactive) {
        itemState = CpDivertSelectionCustomitem::Deactivated;
    } else {
        itemState = CpDivertSelectionCustomitem::Disabled;
    }
    
    switch (condition) {
    case DivertConditionAllConditionalCases:
        DPRINT << ": DivertConditionAllConditionalCases";
        
        // If not available effects also conditions below
        changeItemData(serviceGroup, DivertConditionBusy, status, number, timeout);
        changeItemData(serviceGroup, DivertConditionNoReply, status, number, timeout);
        changeItemData(serviceGroup, DivertConditionNotReachable, status, number, timeout);
        // Fall trough
    case DivertConditionUnconditional:
    case DivertConditionBusy:
    case DivertConditionNoReply:
    case DivertConditionNotReachable:
        if (serviceGroup & ServiceGroupVoice) {
            item(ServiceGroupVoice, condition)->setContentWidgetData("number", number);
            item(ServiceGroupVoice, condition)->setContentWidgetData("timeout", timeout);
            item(ServiceGroupVoice, condition)->setContentWidgetData("state", itemState);
            item(ServiceGroupVoice, condition)->setEnabled(true);
        }
        
        if (serviceGroup & ServiceGroupData) {
            item(ServiceGroupData, condition)->setContentWidgetData("number", number);
            item(ServiceGroupData, condition)->setContentWidgetData("timeout", timeout);
            item(ServiceGroupData, condition)->setContentWidgetData("state", itemState);
            item(ServiceGroupData, condition)->setEnabled(true);
        }
        break;
    case DivertConditionAllCalls:
    case DivertConditionUnknown:
    default:
        break;
    }
}

/*!
  CpDivertPluginGroup::revertItemData.
 */
void CpDivertPluginGroup::revertItemData(
        PsServiceGroup serviceGroup, PsCallDivertingCondition condition)
{
    DPRINT << ": IN";
    if (serviceGroup & ServiceGroupVoice) {
        item(ServiceGroupVoice, condition)->setContentWidgetData("state",
            item(ServiceGroupVoice, condition)->contentWidgetData("state"));
    }
    
    if (serviceGroup & ServiceGroupData) {
        item(ServiceGroupData, condition)->setContentWidgetData("state",
            item(ServiceGroupData, condition)->contentWidgetData("state"));
    }

    DPRINT << ": OUT";
}

/*!
  CpDivertPluginGroup::item.
 */
CpSettingFormItemData* CpDivertPluginGroup::item(
        PsService service, PsCallDivertingCondition condition)
{
    CpSettingFormItemData *ret = 0;
    switch (condition) {
        case DivertConditionUnconditional:
            if (service == ServiceGroupVoice) {
                ret = m_DataItemVoiceAllCalls;
            } else {
                ret = m_DataItemVideoAllCalls;
            }
            break;
        case DivertConditionBusy:
            if (service == ServiceGroupVoice) {
                ret = m_DataItemVoiceIfBusy;
            } else {
                ret = m_DataItemVideoIfBusy;
            }
            break;
        case DivertConditionNoReply:
            if (service == ServiceGroupVoice) {
                ret = m_DataItemVoiceIfNotAnswered;
            } else {
                ret = m_DataItemVideoIfNotAnswered;
            }
            break;
        case DivertConditionNotReachable:
            if (service == ServiceGroupVoice) {
                ret = m_DataItemVoiceIfOutOfReach;
            } else {
                ret = m_DataItemVideoIfOutOfReach;
            }
            break;
        case DivertConditionAllConditionalCases:
            if (service == ServiceGroupVoice) {
                ret = m_DataItemVoiceIfNotAvailable;
            } else {
                ret = m_DataItemVideoIfNotAvailable;
            }
            break;
        case DivertConditionAllCalls:
        case DivertConditionUnknown:
        default:
            break;
    }
    Q_CHECK_PTR(ret);
    return ret;
}

/*!
  CpDivertPluginGroup::processDivertingRequestQueue.
 */
void CpDivertPluginGroup::processDivertingRequestQueue()
{
    DPRINT << ": IN";
    if (m_divertRequestQueue.isEmpty()) {
        DPRINT << "queue empty : OUT";
        return;
    }
    CpDivertRequestQueueItem request = m_divertRequestQueue.head();
    // Command param
    PSCallDivertingCommand divertCommand;
    divertCommand.iServiceGroup = qvariant_cast<PsServiceGroup>(
            request.item->property("serviceGroup"));
    divertCommand.iCondition = qvariant_cast<PsCallDivertingCondition>(
            request.item->property("condition"));
    divertCommand.iStatus = DivertingStatusUnknown;
    divertCommand.iNumber = "";
    divertCommand.iNoReplyTimer = 0;

    switch (request.request) {
        case ActivateDivert: {
            DPRINT << "activate";
            divertCommand.iSetting = RegisterDiverting;
            if (popUpVoiceNumberListQuery(
                    request.item->property("queryLabel").toString(),
                    divertCommand.iNumber,
                    divertCommand.iServiceGroup)) {
                if (request.item->property("needTimeoutInfo").toBool()) {
                    if (popUpTimerQuery(divertCommand.iNoReplyTimer)) {
                        setCallDiverting(divertCommand);
                    } else {
                        // Query was cancelled
                        revertItemData(
                                divertCommand.iServiceGroup, 
                                divertCommand.iCondition);
                        m_divertRequestQueue.clear();
                    }
                } else {
                    setCallDiverting(divertCommand);
                }
            } else {
                // Query was cancelled
                revertItemData(
                        divertCommand.iServiceGroup, 
                        divertCommand.iCondition);
                m_divertRequestQueue.clear();
            }
        }
            break;
        case DeactivateDivert: {
            DPRINT << "deactivate";
            divertCommand.iSetting = EraseDiverting;
            setCallDiverting(divertCommand);
        }
            break;
        case CheckDivertStatus: {
            DPRINT << "check status";
            m_callDivertingWrapper->getCallDivertingStatus(
                divertCommand.iServiceGroup,
                divertCommand.iCondition,
                bscParam(divertCommand.iServiceGroup) );
            
            if (!CpPhoneNotes::instance()->noteShowing()) {
                CpPhoneNotes::instance()->showGlobalProgressNote(
                        m_activeProgressNoteId, hbTrId("txt_phone_info_requesting"));
            }
        }
            break;
        default:
            DPRINT << "Error: unknown enum value";
            break;
    }

    DPRINT << ": OUT";
}

/*!
  CpDivertPluginGroup::changeDivertingStateRequested.
 */
void CpDivertPluginGroup::changeDivertingStateRequested(
        CpDivertItemData &item)
{
    DPRINT << ": IN";
    
    DivertRequest event=ActivateDivert;
    if (CpDivertSelectionCustomitem::Enabled == 
            item.contentWidgetData("state").toInt()) {
        event = DeactivateDivert;
    } else {
        event = ActivateDivert;
    }
    
    addToDivertingRequestQueue(event, item);
    DPRINT << ": OUT";
}

/*!
  CpDivertPluginGroup::handleDivertingChanged.
 */
void CpDivertPluginGroup::handleDivertingChanged(
        const PSCallDivertingCommand& aSetting, 
        bool aPlural)
{
    DPRINT << ": IN";
    
    CpPhoneNotes::instance()->cancelNote(m_activeNoteId);
    DPRINT << "aPlural:" << aPlural;
    DPRINT << "iCondition:" << aSetting.iCondition;
    DPRINT << "iNoReplyTimer:" << aSetting.iNoReplyTimer;
    DPRINT << "iNumber:" << aSetting.iNumber;
    DPRINT << "iServiceGroup:" << aSetting.iServiceGroup;
    DPRINT << "iSetting:" << aSetting.iSetting;
    DPRINT << "iStatus:" << aSetting.iStatus;

    changeItemData(
        aSetting.iServiceGroup,
        aSetting.iCondition, aSetting.iStatus,
        aSetting.iNumber, aSetting.iNoReplyTimer);
    
    updateDependentDivertOptions(
            (DivertConditionUnconditional == aSetting.iCondition) &&
            (DivertingStatusActive != aSetting.iStatus) );
    
    switch(aSetting.iStatus) {
        case DivertingStatusActive:
            if (aPlural) {
                CpPhoneNotes::instance()->showGlobalNote(m_activeNoteId,
                    hbTrId("txt_phone_info_diverts_activated"), 
                    HbMessageBox::MessageTypeInformation);
            } else {
                CpPhoneNotes::instance()->showGlobalNote(m_activeNoteId,
                    hbTrId("txt_phone_info_divert_activated"), 
                    HbMessageBox::MessageTypeInformation);
            }
            if (!m_divertToVoiceMailBox) {
                // Number, except vmbx number, will be added to defaultnumber list
                m_callDivertingWrapper->setNewDefaultNumber(aSetting.iNumber);
            }
            // Diverting calls does not affect Internet calls
            if (Tools::voipSupported() &&
                    (aSetting.iServiceGroup & ServiceGroupVoice)) {
                CpPhoneNotes::instance()->showGlobalNote(m_activeNoteId,
                    hbTrId("Diverting calls does not affect Internet calls"), 
                    HbMessageBox::MessageTypeInformation);
            }
            break;
        case DivertingStatusNotRegistered:  
        case DivertingStatusInactive:
            if (aPlural) {
                CpPhoneNotes::instance()->showGlobalNote(m_activeNoteId,
                    hbTrId("txt_phone_info_diverts_deactivated"), 
                    HbMessageBox::MessageTypeInformation);
            } else {
                CpPhoneNotes::instance()->showGlobalNote(m_activeNoteId,
                    hbTrId("txt_phone_info_divert_deactivated"), 
                    HbMessageBox::MessageTypeInformation);
            }
            break;
        case DivertingStatusNotProvisioned:
        case DivertingStatusUnknown:
        default:
            CpPhoneNotes::instance()->showGlobalNote(m_activeNoteId,
                hbTrId("txt_phone_info_request_not_completed"), 
                HbMessageBox::MessageTypeInformation);
    }
    
    DPRINT << ": OUT";
}
   
/*!
  CpDivertPluginGroup::handleDivertingStatus.
 */
void CpDivertPluginGroup::handleDivertingStatus(
        QList<PSCallDivertingStatus*> &divertList, 
        bool plural)
{
    DPRINT << ": IN";
    DPRINT << "divertList.Size():" << divertList.size();
    DPRINT << "plural:" << plural;

    foreach(PSCallDivertingStatus* status, divertList) {
        DPRINT << status->iCondition;
        DPRINT << status->iNumber;
        DPRINT << status->iServiceGroup;
        DPRINT << status->iStatus;
        DPRINT << status->iTimeout;
        changeItemData(
            status->iServiceGroup,
            status->iCondition, status->iStatus, 
            status->iNumber, status->iTimeout);
    
    }
    
    updateDependentDivertOptions();
    
    DPRINT << ": OUT";
}

/*!
  CpDivertPluginGroup::handleDivertingError.
 */
void CpDivertPluginGroup::handleDivertingError(int aReason)
{
    DPRINT << ": IN : aReason:" << aReason;
    
    // Update view item for failed request
    if (m_divertRequestQueue.count()) {
        revertItemData(
            qvariant_cast<PsServiceGroup>(
                    m_divertRequestQueue.head().item->property("serviceGroup")),
            qvariant_cast<PsCallDivertingCondition>(
                    m_divertRequestQueue.head().item->property("condition")));
    }
    
    // Clear queue
    m_divertRequestQueue.clear();
    
    // Cancel previous note
    CpPhoneNotes::instance()->cancelNote(m_activeNoteId);
    
    // Show error note
    CpPhoneNotes::instance()->showGlobalErrorNote(m_activeNoteId, aReason);
    
    DPRINT << ": OUT";
}

/*!
  CpDivertPluginGroup::popUpVoiceNumberListQuery.
 */
bool CpDivertPluginGroup::popUpVoiceNumberListQuery(
        const QString& heading, QString& result, PsServiceGroup serviceGroup)
{
    DPRINT << ": IN";
    const QString KVoiceMail("voiceMail");
    const QString KVideoMail("voiceMail");
    const QString KOtherNumber("otherNumber");
    
    bool requestOK(false);
    m_divertToVoiceMailBox = false;
    QStringList defNumbers;
    HbDialog *dialog = createDialog(heading);
    HbListWidget *list = new HbListWidget(dialog);
    if (serviceGroup == ServiceGroupVoice) {
        addItemToListWidget(
            list, hbTrId("txt_phone_setlabel_voice_mbx"), KVoiceMail );
    } else if(serviceGroup == ServiceGroupData) {
        // TODO: Implement video voicemail
    }

    // Add "old" divert number to list
    m_callDivertingWrapper->getDefaultNumbers(defNumbers);
    int count(defNumbers.count());
    for (int i = 0; i < count; i++) {
        addItemToListWidget(list, defNumbers[i], defNumbers[i]);
    }
    addItemToListWidget(list, hbTrId("txt_phone_list_enter_number_manually"), KOtherNumber );
    // Connect list item activation signal to close the popup
    connect(
        list, SIGNAL(activated(HbListWidgetItem*)), 
        dialog, SLOT(close()));
    // Sets the "Cancel"-action/button
    HbAction *cancelAction = new HbAction(hbTrId("txt_common_button_cancel"));
    dialog->setPrimaryAction(cancelAction);
    dialog->setContentWidget(list);
    // Launch popup and handle the response
    if (dialog->exec() != cancelAction) {
        // Update the view with selected text
        QString data = list->currentItem()->data().toString();
        if (data == KVoiceMail) {
            m_callDivertingWrapper->getVoiceMailBoxNumber(result);
            if (result.size()) {
                // voicemailboxnumber found
                m_divertToVoiceMailBox = true;
                requestOK = true;
            }
        } else if (data == KOtherNumber) {
            requestOK = popUpNumberEditor(hbTrId("txt_phone_info_number"), result);
        } else {
            //TODO if matched contact name not work
            result = data;
            requestOK = true;
        }
    }
    else {
        DPRINT << ": Cancel";
    }
    disconnect(
        list, SIGNAL(activated(HbListWidgetItem*)), 
        dialog, SLOT(close()));
    delete dialog;
    
    DPRINT << ": OUT : requestOK :" << requestOK;
    return requestOK;
}

/*!
  CpDivertPluginGroup::popUpNumberEditor.
 */
bool CpDivertPluginGroup::popUpNumberEditor(
        const QString& heading, QString& result)
{
    DPRINT << ": IN";

    bool requestOK(false);
    HbDialog *dialog = createDialog(heading);
    HbLineEdit *editor = new HbLineEdit(dialog);
    HbEditorInterface editorInterface(editor);
    editorInterface.setUpAsPhoneNumberEditor();
    dialog->setContentWidget(editor);
    HbAction *okAction = new HbAction(hbTrId("txt_common_button_ok"));
    dialog->addAction(okAction);
    HbAction *cancelAction = new HbAction(hbTrId("txt_common_button_cancel"));
    dialog->setSecondaryAction(cancelAction);
    
    HbAction *resultAction = dialog->exec();
    if (resultAction == cancelAction) {
        DPRINT << ": canceled";
    }
    else {
        result = editor->text();
        DPRINT << ": number "
            << result;
        if (result.count()) {
            requestOK = true;
        }
        else {
            CpPhoneNotes::instance()->showGlobalNote(m_activeNoteId, 
                hbTrId("txt_phone_info_invalid_phone_number"), HbMessageBox::MessageTypeWarning);
        }
    }
    delete dialog;
    
    DPRINT << ": OUT : requestOK :" << requestOK;
    return requestOK;
}

/*!
  CpDivertPluginGroup::setCallDiverting.
 */
void CpDivertPluginGroup::setCallDiverting(PSCallDivertingCommand& command)
{
    DPRINT << ": IN";
    DPRINT << "iCondition:" << command.iCondition;
    DPRINT << "iNoReplyTimer:" << command.iNoReplyTimer;
    DPRINT << "iNumber:" << command.iNumber;
    DPRINT << "iServiceGroup:" << command.iServiceGroup;
    DPRINT << "iSetting:" << command.iSetting;
    DPRINT << "iStatus:" << command.iStatus;
    
    int result = m_callDivertingWrapper->setCallDiverting(
            command, bscParam(command.iServiceGroup));
    if (0 == result) {
        if (!CpPhoneNotes::instance()->noteShowing()) {
            CpPhoneNotes::instance()->showGlobalProgressNote(
                    m_activeProgressNoteId, hbTrId("txt_phone_info_requesting"));
        }
    } else {
        handleDivertingError(result);
    }
    
    DPRINT << ": OUT ";
}

/*!
  CpDivertPluginGroup::popUpTimerQuery.
 */
bool CpDivertPluginGroup::popUpTimerQuery(int &timeout) const
{
    DPRINT << ": IN";
    
    bool requestOK(false);
    HbDialog *dialog = createDialog(hbTrId("Time out"));
    HbListWidget *list = new HbListWidget(dialog);
    addItemToListWidget(list, hbTrId("5 second"), 5 );
    addItemToListWidget(list, hbTrId("10 second"), 10);
    addItemToListWidget(list, hbTrId("15 second"), 15);
    addItemToListWidget(list, hbTrId("20 second"), 20);
    addItemToListWidget(list, hbTrId("25 second"), 25);
    addItemToListWidget(list, hbTrId("30 second"), 30);
    // Connect list item activation signal to close the popup
    connect(
        list, SIGNAL(activated(HbListWidgetItem*)), 
        dialog, SLOT(close()));
    // Sets the "Cancel"-action/button
    HbAction *cancelAction = new HbAction(hbTrId("txt_common_button_cancel"));
    dialog->setPrimaryAction(cancelAction);
    dialog->setContentWidget(list);
    // Launch popup and handle the response
    if (dialog->exec() != cancelAction) {
        // Update the view with selected text
        if (list->currentItem()) {
            timeout = list->currentItem()->data().toInt();
        }
        requestOK = true;
    }
    else {
        DPRINT << ": Cancel";
    }

    disconnect(
        list, SIGNAL(activated(HbListWidgetItem*)), 
        dialog, SLOT(close()));
    delete dialog;
    
    DPRINT << ": OUT : requestOK :" << requestOK;
    return requestOK;
}

/*!
  CpDivertPluginGroup::setbscParam.
 */
int CpDivertPluginGroup::bscParam(PsServiceGroup serviceGroup)
{
    DPRINT << ": IN";

    int bsc = AllTeleAndBearer;

    if (serviceGroup & ServiceGroupVoice) {
        int alsLine(AlsNotSupported);
        m_ssSettingsWrapper->get(Als,alsLine);
        DPRINT << ": alsLine " << alsLine;
        
        if ((alsLine == AlsNotSupported) || (alsLine == AlsPrimary)) {
            // Etelephony only activates voice service nothing else or causes
            // voice service status request.
            bsc = Telephony;
        }
        else { // ESSSettingsAlsAlternate
            // EAltTele only activates alternate service nothing else or causes
            // alternate service status request.
            bsc = AltTele;
        }
    }
    
    if (serviceGroup & ServiceGroupData) {
        bsc = AllBearer;
    }


    
    DPRINT << ": OUT : bsc :" << bsc;
    return bsc;
}

/*!
  CpDivertPluginGroup::createDialog.
 */
HbDialog* CpDivertPluginGroup::createDialog( const QString& heading ) const
{
    DPRINT << ": IN";
 
    HbDialog *dialog = new HbDialog();
    dialog->setDismissPolicy(HbDialog::NoDismiss);
    dialog->setTimeout(HbDialog::NoTimeout);
    dialog->setHeadingWidget(new HbLabel(heading));
    
    DPRINT << ": OUT";
    return dialog;
}

/*!
  CpDivertPluginGroup::addItemToListWidget.
 */
void CpDivertPluginGroup::addItemToListWidget(HbListWidget* w,
        const QString& item, const QString& data) const
{
    DPRINT << ": IN";
 
    HbListWidgetItem* o = new HbListWidgetItem();
    o->setText(item);
    o->setData(data);
    w->addItem(o);
    
    DPRINT << ": OUT";
}

/*!
  CpDivertPluginGroup::addItemToListWidget.
 */
void CpDivertPluginGroup::addItemToListWidget(HbListWidget* w,
        const QString& item, const int& data) const
{
    DPRINT << ": IN";
 
    HbListWidgetItem* o = new HbListWidgetItem();
    o->setText(item);
    o->setData(data);
    w->addItem(o);
    
    DPRINT << ": OUT";
}

/*!
  CpDivertPluginGroup::updateDependentDivertOptions.
  Updates statuses of diverts which are dependent from the status of some
  other divert. Logic for dependent changes are:
  1. Activation of all calls divert deactivates other diverts/puts them 
  into quiescent state.
  2. Deactivation of all calls divert enables diverts in quiescent state.
  3. Not available option is an abstraction of CFB, CFNry, CFNrc diverts.
     Enabling/disabling that options changes all the aforementioned diverts 
     at once.
*/
void CpDivertPluginGroup::updateDependentDivertOptions(bool fetchFromNetwork)
{
    DPRINT << ": IN";
    
    // all calls divert activation deactivates automatically other diverts
    QVariant itemState = m_DataItemVoiceAllCalls->contentWidgetData("state");
    if ((itemState.isValid()) && 
        (itemState.toInt() == CpDivertSelectionCustomitem::Enabled)) {
        deActivateDependentDivertOption(m_DataItemVoiceIfBusy);
        deActivateDependentDivertOption(m_DataItemVoiceIfNotAnswered);
        deActivateDependentDivertOption(m_DataItemVoiceIfOutOfReach);
    } else {
        // Must query data for diverts depending on all calls divert, because 
        // data may have been lost for registered diverts, which were 
        // automatically deactivated due to the activation of all calls divert.
        activateDependentDivertOption(m_DataItemVoiceIfBusy, fetchFromNetwork);
        activateDependentDivertOption(m_DataItemVoiceIfNotAnswered, fetchFromNetwork);
        activateDependentDivertOption(m_DataItemVoiceIfOutOfReach, fetchFromNetwork);
    }
    
    // all calls divert activation deactivates automatically other diverts
    itemState = m_DataItemVideoAllCalls->contentWidgetData("state");
    if ((itemState.isValid()) && 
        (itemState.toInt() == CpDivertSelectionCustomitem::Enabled)) {
        deActivateDependentDivertOption(m_DataItemVideoIfBusy);
        deActivateDependentDivertOption(m_DataItemVideoIfNotAnswered);
        deActivateDependentDivertOption(m_DataItemVideoIfOutOfReach);
    } else {
        // Must query data for diverts depending on all calls divert, because 
        // data may have been lost for registered diverts, which were 
        // automatically deactivated due to the activation of all calls divert.
        activateDependentDivertOption(m_DataItemVideoIfBusy, fetchFromNetwork);
        activateDependentDivertOption(m_DataItemVideoIfNotAnswered, fetchFromNetwork);
        activateDependentDivertOption(m_DataItemVideoIfOutOfReach, fetchFromNetwork);
    }
    
    // update not available divert option
    updateNotAvailableDivertOption();
    
    DPRINT << ": OUT";
}

/*!
  CpDivertPluginGroup::deActivateDependentDivertOption.
*/
void CpDivertPluginGroup::deActivateDependentDivertOption(
        CpDivertItemData* item) const
{
    QVariant itemState = item->contentWidgetData("state");
    if ((itemState.isValid()) && 
        (itemState == CpDivertSelectionCustomitem::Enabled)) {
        item->setContentWidgetData(
            "state", CpDivertSelectionCustomitem::Deactivated);
    }
    item->setEnabled(false);
}

/*!
  CpDivertPluginGroup::activateDependentDivertOption.
*/
void CpDivertPluginGroup::activateDependentDivertOption(
        CpDivertItemData* item, bool fetchFromNetwork)
{
    if ((CpDivertSelectionCustomitem::Deactivated == 
            item->contentWidgetData("state").toInt()) &&
            fetchFromNetwork) {
        addToDivertingRequestQueue(
            CheckDivertStatus, *item);
    }
    item->setEnabled(true);
}

/*!
  CpDivertPluginGroup::updateNotAvailableDivertOption.
 */
void CpDivertPluginGroup::updateNotAvailableDivertOption()
{
    DPRINT << ": IN";
    
    // We must check that both states & numbers match amongst CFB, CRNry and 
    // CFNrc before concluding that not available divert is enabled, because
    // some networks may not return divert number for inactive diverts.
    int cfbState = 
        m_DataItemVoiceIfBusy->contentWidgetData("state").toInt();
    int cfnryState = 
        m_DataItemVoiceIfNotAnswered->contentWidgetData("state").toInt();
    int cfnrcState = 
        m_DataItemVoiceIfOutOfReach->contentWidgetData("state").toInt();
    QVariant cfbNumber = 
        m_DataItemVoiceIfBusy->contentWidgetData("number");
    QVariant cfnryNumber = 
        m_DataItemVoiceIfNotAnswered->contentWidgetData("number");
    QVariant cfnrcNumber = 
        m_DataItemVoiceIfOutOfReach->contentWidgetData("number");
    
    if ((cfbNumber == cfnryNumber) && (cfbNumber == cfnrcNumber) &&
        (cfbState == cfnryState) && (cfbState == cfnrcState)) {
        m_DataItemVoiceIfNotAvailable->setContentWidgetData(
            "number", m_DataItemVoiceIfBusy->contentWidgetData("number"));
        m_DataItemVoiceIfNotAvailable->setContentWidgetData(
            "timeout", m_DataItemVoiceIfNotAnswered->contentWidgetData("timeout"));
        m_DataItemVoiceIfNotAvailable->setContentWidgetData(
            "state", m_DataItemVoiceIfBusy->contentWidgetData("state"));
    } else {
        m_DataItemVoiceIfNotAvailable->setContentWidgetData("number", "");
        m_DataItemVoiceIfNotAvailable->setContentWidgetData("timeout", 0);
        m_DataItemVoiceIfNotAvailable->setContentWidgetData(
            "state", CpDivertSelectionCustomitem::Disabled);

    }
    // Item dimming
    m_DataItemVoiceIfNotAvailable->setEnabled(
            m_DataItemVoiceIfBusy->isEnabled() &&
            m_DataItemVoiceIfNotAnswered->isEnabled() &&
            m_DataItemVoiceIfOutOfReach->isEnabled()); 
    
    
    // We must check that both states & numbers match amongst CFB, CRNry and 
    // CFNrc before concluding that not available divert is enabled, because
    // some networks may not return divert number for inactive diverts.
    cfbState = 
        m_DataItemVideoIfBusy->contentWidgetData("state").toInt();
    cfnryState = 
        m_DataItemVideoIfNotAnswered->contentWidgetData("state").toInt();
    cfnrcState = 
        m_DataItemVideoIfOutOfReach->contentWidgetData("state").toInt();
    cfbNumber = 
        m_DataItemVideoIfBusy->contentWidgetData("number");
    cfnryNumber = 
        m_DataItemVideoIfNotAnswered->contentWidgetData("number");
    cfnrcNumber = 
        m_DataItemVideoIfOutOfReach->contentWidgetData("number");
    
    if ((cfbNumber == cfnryNumber) && (cfbNumber == cfnrcNumber) &&
        (cfbState == cfnryState) && (cfbState == cfnrcState)) {
        m_DataItemVideoIfNotAvailable->setContentWidgetData(
            "number", m_DataItemVideoIfBusy->contentWidgetData("number"));
        m_DataItemVideoIfNotAvailable->setContentWidgetData(
            "timeout", m_DataItemVideoIfNotAnswered->contentWidgetData("timeout"));
        m_DataItemVideoIfNotAvailable->setContentWidgetData(
            "state", m_DataItemVideoIfBusy->contentWidgetData("state"));
    } else {
        m_DataItemVideoIfNotAvailable->setContentWidgetData("number", "");
        m_DataItemVideoIfNotAvailable->setContentWidgetData("timeout", 0);
        m_DataItemVideoIfNotAvailable->setContentWidgetData(
            "state", CpDivertSelectionCustomitem::Disabled);

    }
    
    // Item dimming
    m_DataItemVideoIfNotAvailable->setEnabled(
            m_DataItemVideoIfBusy->isEnabled() &&
            m_DataItemVideoIfNotAnswered->isEnabled() &&
            m_DataItemVideoIfOutOfReach->isEnabled()); 
    
    DPRINT << ": OUT";
}

// End of File. 
