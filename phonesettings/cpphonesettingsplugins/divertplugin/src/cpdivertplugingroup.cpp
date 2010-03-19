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

/*!
  CpDivertPluginGroup::CpDivertPluginGroup.
 */
CpDivertPluginGroup::CpDivertPluginGroup(CpItemDataHelper &helper)
     : CpSettingFormItemData(HbDataFormModelItem::GroupItem, 
                             hbTrId("txt_phone_subhead_call_divert"),0),
       m_csVoiceCallPage(NULL),
       m_DataItemAllVoiceCalls(NULL),
       m_DataItemIfBusy(NULL),
       m_DataItemIfNotAnswered(NULL),
       m_DataItemIfOutOfReach(NULL),
       m_DataItemIfNotAvailable(NULL),
       m_activeNoteId(0),
       m_helper(helper)
{
    DPRINT << ": IN";
    
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
    
    // Create grouped setting items
    createVoiceCallItems();
    createVideoCallItems();
    createVoIPCallItems();
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
void CpDivertPluginGroup::createVoiceCallItems()
{
    DPRINT << ": IN";
    m_csVoiceCallPage = new CpSettingFormItemData(
                HbDataFormModelItem::GroupPageItem,
                hbTrId("txt_phone_setlabel_service_val_voice_divert"));
    appendChild(m_csVoiceCallPage);

    m_DataItemAllVoiceCalls = new CpDivertItemData(
            DivertConditionUnconditional,
            ServiceGroupVoice,
            hbTrId("txt_phone_setlabel_all_calls"),
            hbTrId("All voice calls:"), false,
            m_helper,
            m_csVoiceCallPage);
    
    addDivertDataItem(m_DataItemAllVoiceCalls, m_csVoiceCallPage);

    m_DataItemIfBusy = new CpDivertItemData(
            DivertConditionBusy,
            ServiceGroupVoice,
            hbTrId("txt_phone_setlabel_if_busy"),
            hbTrId("If busy:"), false,
            m_helper,
            m_csVoiceCallPage);
    
    addDivertDataItem(m_DataItemIfBusy, m_csVoiceCallPage);

    m_DataItemIfNotAnswered = new CpDivertItemData(
            DivertConditionNoReply,
            ServiceGroupVoice,
            hbTrId("txt_phone_setlabel_if_not_answered"),
            hbTrId("If not answered:"), true,
            m_helper,
            m_csVoiceCallPage);
    
    addDivertDataItem(m_DataItemIfNotAnswered, m_csVoiceCallPage);
    m_DataItemIfNotAnswered->setContentWidgetData("timeoutText", hbTrId("txt_phone_setlabel_delay"));

    m_DataItemIfOutOfReach = new CpDivertItemData(
            DivertConditionNotReachable,
            ServiceGroupVoice,
            hbTrId("txt_phone_setlabel_if_out_of_reach"),
            hbTrId("If out of reach:"), false,
            m_helper,
            m_csVoiceCallPage);
    
    addDivertDataItem(m_DataItemIfOutOfReach, m_csVoiceCallPage);

    m_DataItemIfNotAvailable = new CpDivertItemData(
            DivertConditionAllConditionalCases,
            ServiceGroupVoice,
            hbTrId("txt_phone_setlabel_if_not_available"),
            hbTrId("If not available:"), true,
            m_helper,
            m_csVoiceCallPage);
    
    addDivertDataItem(m_DataItemIfNotAvailable, m_csVoiceCallPage);
    DPRINT << ": OUT";
}

/*!
 CpDivertPluginGroup::createVideoCallItems
 */
void CpDivertPluginGroup::createVideoCallItems()
{
    DPRINT << ": IN";
    appendChild( new CpSettingFormItemData(
        HbDataFormModelItem::GroupPageItem,
        hbTrId("txt_phone_setlabel_service_val_video_divert")) );
    DPRINT << ": OUT";
}

/*!
 CpDivertPluginGroup::createVoIPCallItems
 */
void CpDivertPluginGroup::createVoIPCallItems()
{
    DPRINT << ": IN";
    appendChild( new CpSettingFormItemData(
        HbDataFormModelItem::GroupPageItem,
        hbTrId("VoIP")) );
    DPRINT << ": OUT";
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
    
    if (!modelItem->contentWidgetData("number").isValid()) {
        if (modelItem==m_DataItemAllVoiceCalls) {
            addToDivertingRequestQueue(CheckDivertStatus, *m_DataItemAllVoiceCalls);
        } else if (modelItem==m_DataItemIfBusy) {
            addToDivertingRequestQueue(CheckDivertStatus, *m_DataItemIfBusy);
        } else if (modelItem==m_DataItemIfNotAnswered) {
            addToDivertingRequestQueue(CheckDivertStatus, *m_DataItemIfNotAnswered);
        } else if (modelItem==m_DataItemIfOutOfReach) {
            addToDivertingRequestQueue(CheckDivertStatus, *m_DataItemIfOutOfReach);
        } // do not update m_DataItemIfNotAvailable
    }


    DPRINT << ": OUT";
}

/*!
  CpDivertPluginGroup::addDivertDataItem.
 */
void CpDivertPluginGroup::addDivertDataItem(
        CpDivertItemData *item, CpSettingFormItemData *parent)
{
    parent->appendChild(item);
    QObject::connect(
        item, SIGNAL(itemClicked(CpDivertItemData&)),
        this, SLOT(changeDivertingStateRequested(CpDivertItemData&)));
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
        PsCallDivertingCondition condition, PsCallDivertingStatus status,
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
    case DivertConditionUnconditional:
        m_DataItemAllVoiceCalls->setContentWidgetData("number", number);
        m_DataItemAllVoiceCalls->setContentWidgetData("timeout", timeout);
        m_DataItemAllVoiceCalls->setContentWidgetData("state", itemState);
        break;
    case DivertConditionBusy:
        m_DataItemIfBusy->setContentWidgetData("number", number);
        m_DataItemIfBusy->setContentWidgetData("timeout", timeout);
        m_DataItemIfBusy->setContentWidgetData("state", itemState);
        break;
    case DivertConditionNoReply:
        m_DataItemIfNotAnswered->setContentWidgetData("number", number);
        m_DataItemIfNotAnswered->setContentWidgetData("timeout", timeout);
        m_DataItemIfNotAnswered->setContentWidgetData("state", itemState);
        break;
    case DivertConditionNotReachable:
        m_DataItemIfOutOfReach->setContentWidgetData("number", number);
        m_DataItemIfOutOfReach->setContentWidgetData("timeout", timeout);
        m_DataItemIfOutOfReach->setContentWidgetData("state", itemState);
        break;
    case DivertConditionAllConditionalCases:
        DPRINT << ": DivertConditionAllConditionalCases";
        
        m_DataItemIfNotAvailable->setContentWidgetData("number", number);
        m_DataItemIfNotAvailable->setContentWidgetData("timeout", timeout);
        m_DataItemIfNotAvailable->setContentWidgetData("state", itemState);
        
        // If not available effects also conditions below
        m_DataItemIfBusy->setContentWidgetData("number", number);
        m_DataItemIfBusy->setContentWidgetData("timeout", timeout);
        m_DataItemIfBusy->setContentWidgetData("state", itemState);
        m_DataItemIfNotAnswered->setContentWidgetData("number", number);
        m_DataItemIfNotAnswered->setContentWidgetData("timeout", timeout);
        m_DataItemIfNotAnswered->setContentWidgetData("state", itemState);
        m_DataItemIfOutOfReach->setContentWidgetData("number", number);
        m_DataItemIfOutOfReach->setContentWidgetData("timeout", timeout);
        m_DataItemIfOutOfReach->setContentWidgetData("state", itemState);
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
void CpDivertPluginGroup::revertItemData(PsCallDivertingCondition condition)
{
    DPRINT << ": IN";
    
    switch (condition) {
        case DivertConditionUnconditional:
            m_DataItemAllVoiceCalls->setContentWidgetData("state", 
                m_DataItemAllVoiceCalls->contentWidgetData("state"));
            break;
        case DivertConditionBusy:
            m_DataItemIfBusy->setContentWidgetData("state", 
                m_DataItemIfBusy->contentWidgetData("state"));
            break;
        case DivertConditionNoReply:
            m_DataItemIfNotAnswered->setContentWidgetData("state", 
                m_DataItemIfNotAnswered->contentWidgetData("state"));
            break;
        case DivertConditionNotReachable:
            m_DataItemIfOutOfReach->setContentWidgetData("state", 
                m_DataItemIfOutOfReach->contentWidgetData("state"));
            break;
        case DivertConditionAllConditionalCases:
            DPRINT << ": DivertConditionAllConditionalCases";
            m_DataItemIfNotAvailable->setContentWidgetData("state", 
                m_DataItemIfNotAvailable->contentWidgetData("state"));
            break;
        case DivertConditionAllCalls:
        case DivertConditionUnknown:
        default:
            break;
    }
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
    divertCommand.iServiceGroup = request.item->service();
    divertCommand.iCondition = request.item->condition();
    divertCommand.iStatus = DivertingStatusUnknown;
    divertCommand.iNumber = hbTrId("");
    divertCommand.iNoReplyTimer = 0;

    switch (request.request) {
        case ActivateDivert: {
            DPRINT << "activate";
            divertCommand.iSetting = RegisterDiverting;
            if (popUpVoiceNumberListQuery(
                    request.item->queryLabel(), divertCommand.iNumber)) {
                if (request.item->needTimeoutInfo()) {
                    if (popUpTimerQuery(divertCommand.iNoReplyTimer)) {
                        setCallDiverting(divertCommand);
                    } else {
                        // Query was cancelled
                        request.item->setContentWidgetData("number", 
                                request.item->contentWidgetData("number").toString());
                        request.item->setContentWidgetData("timeout", 
                                request.item->contentWidgetData("timeout").toInt());
                        m_divertRequestQueue.clear();
                    }
                } else {
                    setCallDiverting(divertCommand);
                }
            } else {
                // Query was cancelled
                request.item->setContentWidgetData("number", 
                        request.item->contentWidgetData("number").toString());
                request.item->setContentWidgetData("timeout", 
                        request.item->contentWidgetData("timeout").toInt());
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
                bscParam() );
            
            if (!CpPhoneNotes::instance()->noteShowing()) {
                CpPhoneNotes::instance()->showGlobalProgressNote(
                        m_activeProgressNoteId, hbTrId("Requesting"));
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
        aSetting.iCondition, aSetting.iStatus,
        aSetting.iNumber, aSetting.iNoReplyTimer);
    
    updateDependentDivertOptions();
    
    switch(aSetting.iStatus) {
        case DivertingStatusActive:
            if (aPlural) {
                CpPhoneNotes::instance()->showGlobalNote(m_activeNoteId,
                    hbTrId("Diverts activated"), 
                    HbMessageBox::MessageTypeInformation);
            } else {
                CpPhoneNotes::instance()->showGlobalNote(m_activeNoteId,
                    hbTrId("Divert activated"), 
                    HbMessageBox::MessageTypeInformation);
            }
            if (!m_divertToVoiceMailBox) {
                // Number, except vmbx number, will be added to defaultnumber list
                m_callDivertingWrapper->setNewDefaultNumber(aSetting.iNumber);
            }
            // Diverting calls does not affect Internet calls
            if (Tools::voipSupported() &&
                    aSetting.iServiceGroup == ServiceGroupVoice) {
                CpPhoneNotes::instance()->showGlobalNote(m_activeNoteId,
                    hbTrId("Diverting calls does not affect Internet calls"), 
                    HbMessageBox::MessageTypeInformation);
            }
            break;
        case DivertingStatusNotRegistered:
        case DivertingStatusInactive:
            if (aPlural) {
                CpPhoneNotes::instance()->showGlobalNote(m_activeNoteId,
                    hbTrId("Diverts cancelled"), 
                    HbMessageBox::MessageTypeInformation);
            } else {
                CpPhoneNotes::instance()->showGlobalNote(m_activeNoteId,
                    hbTrId("Divert cancelled"), 
                    HbMessageBox::MessageTypeInformation);
            }
            break;
        case DivertingStatusNotProvisioned:
        case DivertingStatusUnknown:
        default:
            CpPhoneNotes::instance()->showGlobalNote(m_activeNoteId,
                hbTrId("Not done"), 
                HbMessageBox::MessageTypeInformation);
    }
    
    if ((DivertConditionUnconditional == aSetting.iCondition) &&
            (DivertingStatusActive != aSetting.iStatus)) {
        // Must query data for diverts depending on all calls divert, because 
        // data may have been lost for registered diverts, which were 
        // automatically deactivated due to the activation of all calls divert.
        if (CpDivertSelectionCustomitem::Deactivated == 
                m_DataItemIfBusy->contentWidgetData("state").toInt()) {
            addToDivertingRequestQueue(
                CheckDivertStatus, *m_DataItemIfBusy);
        }
        
        if (CpDivertSelectionCustomitem::Deactivated == 
                m_DataItemIfNotAnswered->contentWidgetData("state").toInt()) {
            addToDivertingRequestQueue(
                CheckDivertStatus, *m_DataItemIfNotAnswered);
        }
        
        if (CpDivertSelectionCustomitem::Deactivated == 
                m_DataItemIfOutOfReach->contentWidgetData("state").toInt()) {
            addToDivertingRequestQueue(
                CheckDivertStatus, *m_DataItemIfOutOfReach);
        }
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
    revertItemData(m_divertRequestQueue.head().item->condition());
    
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
        const QString& heading, QString& result)
{
    DPRINT << ": IN";
    
    bool requestOK(false);
    m_divertToVoiceMailBox = false;
    QStringList defNumbers;
    HbDialog *dialog = createDialog(heading);
    HbListWidget *list = new HbListWidget(dialog);
    addItemToListWidget(
        list, hbTrId("txt_phone_setlabel_voice_mbx"), hbTrId("txt_phone_setlabel_voice_mbx") );
    // Add "old" divert number to list
    m_callDivertingWrapper->getDefaultNumbers(defNumbers);
    int count(defNumbers.count());
    for (int i = 0; i < count; i++) {
        addItemToListWidget(list, defNumbers[i], defNumbers[i]);
    }
    addItemToListWidget(list, hbTrId("Other number"), hbTrId("Other number") );
    // Connect list item activation signal to close the popup
    connect(
        list, SIGNAL(activated(HbListWidgetItem*)), 
        dialog, SLOT(close()));
    // Sets the "Cancel"-action/button
    HbAction *cancelAction = new HbAction("Cancel");
    dialog->setPrimaryAction(cancelAction);
    dialog->setContentWidget(list);
    // Launch popup and handle the response
    if (dialog->exec() != cancelAction) {
        // Update the view with selected text
        QString data = list->currentItem()->data().toString();
        if (data == hbTrId("txt_phone_setlabel_voice_mbx")) {
            m_callDivertingWrapper->getVoiceMailBoxNumber(result);
            if (result.size()) {
                // voicemailboxnumber found
                m_divertToVoiceMailBox = true;
                requestOK = true;
            }
        } else if (data == hbTrId("Other number")) {
            requestOK = popUpNumberEditor(hbTrId("Number:"), result);
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
    HbAction *okAction = new HbAction(hbTrId("OK"));
    dialog->addAction(okAction);
    HbAction *cancelAction = new HbAction(hbTrId("Cancel"));
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
                hbTrId("Enter number"), HbMessageBox::MessageTypeWarning);
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
    
    int result = m_callDivertingWrapper->setCallDiverting(command, bscParam());
    if (0 == result) {
        if (!CpPhoneNotes::instance()->noteShowing()) {
            CpPhoneNotes::instance()->showGlobalProgressNote(
                    m_activeProgressNoteId, hbTrId("Requesting"));
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
    HbAction *cancelAction = new HbAction(hbTrId("Cancel"));
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
int CpDivertPluginGroup::bscParam()
{
    DPRINT << ": IN";

    int bsc = AllTeleAndBearer;
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
void CpDivertPluginGroup::updateDependentDivertOptions()
{
    DPRINT << ": IN";
    
    // all calls divert activation deactivates automatically other diverts
    QVariant itemState = m_DataItemAllVoiceCalls->contentWidgetData("state");
    if ((itemState.isValid()) && 
        (itemState.toInt() == CpDivertSelectionCustomitem::Enabled)) {
        itemState = m_DataItemIfBusy->contentWidgetData("state");
        if ((itemState.isValid()) && 
            (itemState == CpDivertSelectionCustomitem::Enabled)) {
            m_DataItemIfBusy->setContentWidgetData(
                "state", CpDivertSelectionCustomitem::Deactivated);
        }
        
        itemState = m_DataItemIfNotAnswered->contentWidgetData("state");
        if ((itemState.isValid()) &&
            (itemState == CpDivertSelectionCustomitem::Enabled)) {
            m_DataItemIfNotAnswered->setContentWidgetData(
                "state", CpDivertSelectionCustomitem::Deactivated);
        }
        
        itemState = m_DataItemIfOutOfReach->contentWidgetData("state");
        if ((itemState.isValid()) &&
            (itemState == CpDivertSelectionCustomitem::Enabled)) {
            m_DataItemIfOutOfReach->setContentWidgetData(
                "state", CpDivertSelectionCustomitem::Deactivated);
        }
    }
    
    // update not available divert option
    updateNotAvailableDivertOption();
    
    DPRINT << ": OUT";
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
        m_DataItemIfBusy->contentWidgetData("state").toInt();
    int cfnryState = 
        m_DataItemIfNotAnswered->contentWidgetData("state").toInt();
    int cfnrcState = 
        m_DataItemIfOutOfReach->contentWidgetData("state").toInt();
    QVariant cfbNumber = 
        m_DataItemIfBusy->contentWidgetData("number");
    QVariant cfnryNumber = 
        m_DataItemIfNotAnswered->contentWidgetData("number");
    QVariant cfnrcNumber = 
        m_DataItemIfOutOfReach->contentWidgetData("number");
    
    if ((cfbNumber == cfnryNumber) && (cfbNumber == cfnrcNumber) &&
        (cfbState == cfnryState) && (cfbState == cfnrcState)) {
        m_DataItemIfNotAvailable->setContentWidgetData(
            "number", m_DataItemIfBusy->contentWidgetData("number"));
        m_DataItemIfNotAvailable->setContentWidgetData(
            "timeout", m_DataItemIfNotAnswered->contentWidgetData("timeout"));
        m_DataItemIfNotAvailable->setContentWidgetData(
            "state", m_DataItemIfBusy->contentWidgetData("state"));
    } else {
        m_DataItemIfNotAvailable->setContentWidgetData("number", "");
        m_DataItemIfNotAvailable->setContentWidgetData("timeout", 0);
        m_DataItemIfNotAvailable->setContentWidgetData(
            "state", CpDivertSelectionCustomitem::Disabled);
    }
    
    DPRINT << ": OUT";
}

// End of File. 
