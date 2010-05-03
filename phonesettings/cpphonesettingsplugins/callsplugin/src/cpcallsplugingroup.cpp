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

#include "cpcallsplugingroup.h"
#include "cpplugincommon.h"
#include "cppluginlogging.h"
#include "cpphonenotes.h"
#include <hbdataformmodel.h>
#include <hbdataformmodelitem.h>
#include <hbdataformviewitem.h>
#include <hbcheckbox.h>
#include <QString>
#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <hblineedit.h>
#include <hbcombobox.h>
#include <hbdeviceprogressdialog.h>
#include <cpitemdatahelper.h>
#include <sssettingswrapper.h>
#include <psetwrapper.h>
#include <psetcliwrapper.h>


/*!
  CpCallsPluginGroup::CpCallsPluginGroup.
 */
CpCallsPluginGroup::CpCallsPluginGroup(CpItemDataHelper &helper)
    :CpSettingFormItemData(HbDataFormModelItem::GroupItem, hbTrId("txt_phone_subhead_call_settings"),0),
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

    m_cpSettingsWrapper = new CpSettingsWrapper;
    m_pSetWrapper = new PSetWrapper; 
    DPRINT << "PSetWrapper created";
    
    m_callWaitingWrapper = &m_pSetWrapper->callWaitingWrapper(); 
    DPRINT << "call waiting wrapper created";
    QObject::connect(
        m_callWaitingWrapper, 
        SIGNAL(handleCallWaitingGetStatus(
            PSetCallWaitingWrapper::PsCallWaitingStatus, const QList<unsigned char> &)),
        this, 
        SLOT(handleCallWaitingGetStatus(
            PSetCallWaitingWrapper::PsCallWaitingStatus, const QList<unsigned char> &)));
    QObject::connect(
        m_callWaitingWrapper, 
        SIGNAL(handleCallWaitingChanged(PSetCallWaitingWrapper::PsCallWaitingCommand, int)),
        this, 
        SLOT(handleCallWaitingChanged(PSetCallWaitingWrapper::PsCallWaitingCommand, int)));
    QObject::connect(
        m_callWaitingWrapper, 
        SIGNAL(handleCallWaitingRequesting( bool, bool )),
        this, 
        SLOT(handleCallWaitingRequesting( bool, bool )));
    QObject::connect(
        m_callWaitingWrapper, 
        SIGNAL(handleCallWaitingError( int )),
        this, 
        SLOT(handleCallWaitingError( int )));

    m_ssSettingsWrapper = new SsSettingsWrapper; 
    DPRINT << "SsSettingsWrapper created";
    
    CpPhoneNotes *phoneNotes = CpPhoneNotes::instance();
    QObject::connect(
        this, 
        SIGNAL(showBasicServiceList(
            const QString&, const QList<unsigned char> &)),
        phoneNotes, 
        SLOT(showBasicServiceList(
            const QString&, const QList<unsigned char> &)));
    QObject::connect(
        this, 
        SIGNAL(showGlobalProgressNote(int &, const QString&)),
        phoneNotes, 
        SLOT(showGlobalProgressNote(int &, const QString&)));
    QObject::connect(
        this, 
        SIGNAL(showGlobalNote(int &, const QString&, HbMessageBox::MessageBoxType)),
        phoneNotes, 
        SLOT(showGlobalNote(int &, const QString&, HbMessageBox::MessageBoxType)));
    QObject::connect(
        this, 
        SIGNAL(showGlobalErrorNote(int &, int)),
        phoneNotes, 
        SLOT(showGlobalErrorNote(int &, int)));
    QObject::connect(
        this, 
        SIGNAL(cancelNote(int)),
        phoneNotes, 
        SLOT(cancelNote(int)));
    
    // Create combobox string <-> setting mappings 
    insertMappedListItems(); 
       
    // Greate settings items
    createCLIItem();
    createCallWaitingtem();
    createSoftRejectItem();
    createShowCallDurationItem();
    
    // Connect setting item signals
    connectCLIItem();
    connectCallWaitingItem();
    connectSoftRejectItem();
    connectShowCallDurationItem();
    
    m_callWaitingDistinguishEnabled = 
       m_cpSettingsWrapper->isFeatureCallWaitingDistiquishNotProvisionedEnabled();
    
    DPRINT << ": OUT";
}

/*!
  CpCallsPluginGroup::~CpCallsPluginGroup.
 */
CpCallsPluginGroup::~CpCallsPluginGroup()
{
    DPRINT << ": IN";
    
    delete m_cpSettingsWrapper;
    delete m_pSetWrapper;
    delete m_ssSettingsWrapper;
    
    DPRINT << ": OUT";
}

/*!
  CpCallsPluginGroup::createShowCallDurationItem.
 */
void CpCallsPluginGroup::createShowCallDurationItem()
{
    DPRINT << ": IN";
    
    // Read show call duration value from Cenrep 
    bool showCallDurationStatus = m_cpSettingsWrapper->showCallDuration();
    DPRINT << "showCallDurationStatus:" << showCallDurationStatus;
    m_DataItemShowCallDuration = new CpSettingFormItemData(
        HbDataFormModelItem::CheckBoxItem, hbTrId("") ,this);
    m_DataItemShowCallDuration->setContentWidgetData(
        "text", QVariant(hbTrId("Show call duration")));
    if (showCallDurationStatus) {
        m_DataItemShowCallDuration->setContentWidgetData(
            "checkState", QVariant(Qt::Checked));
    } else {
        m_DataItemShowCallDuration->setContentWidgetData(
            "checkState", QVariant(Qt::Unchecked));
    }
    appendChild( m_DataItemShowCallDuration ); 
    
    DPRINT << ": OUT";
}
    
/*!
  CpCallsPluginGroup::createSoftRejectItem.
 */
void CpCallsPluginGroup::createSoftRejectItem()
{
    DPRINT << ": IN";
    
    bool userDefinedTextInUse = false;
    QString softRejectText;
    m_cpSettingsWrapper->readSoftRejectText(
        softRejectText, userDefinedTextInUse);
    if(!userDefinedTextInUse) {
        softRejectText =
            hbTrId("txt_phone_setlabel_soft_reject_val_default_text");
    }
    m_DataItemSoftRejectTextEditor = 
        new CpSettingFormItemData(
            HbDataFormModelItem::TextItem,
            hbTrId("txt_phone_setlabel_soft_reject"),
            this);
    m_DataItemSoftRejectTextEditor->setContentWidgetData(
        "text", QVariant(softRejectText));
    m_DataItemSoftRejectTextEditor->setContentWidgetData(
        "maxLength", QVariant(160));
    m_DataItemSoftRejectTextEditor->setContentWidgetData(
        "maxRows", QVariant(5));
    m_DataItemSoftRejectTextEditor->setContentWidgetData(
        "minRows", QVariant(1));
    appendChild( m_DataItemSoftRejectTextEditor ); 
    
    DPRINT << ": OUT";
}

/*!
  CpCallsPluginGroup::createCLIItem.
 */
void CpCallsPluginGroup::createCLIItem()
{
    DPRINT << ": IN";

    m_DataItemCLI = 
        new CpSettingFormItemData(
            HbDataFormModelItem::ComboBoxItem,
            hbTrId("txt_phone_setlabel_send_my_caller_id"),
            this);
    // CLI combo box items string list 
    QStringList list;
    QMapIterator<int, QString> cliSettingMapIterator(m_cliSettingMap);
    while (cliSettingMapIterator.hasNext()) {
        cliSettingMapIterator.next();
        list << cliSettingMapIterator.value(); 
        };
    m_DataItemCLI->setContentWidgetData("items", QVariant(list));
    appendChild(m_DataItemCLI);
    
    DPRINT << ": OUT";
}

/*!
  CpCallsPluginGroup::createCallWaitingtem.
 */
void CpCallsPluginGroup::createCallWaitingtem()
{
    DPRINT << ": IN";

    m_DataItemCallWaiting = 
        new CpSettingFormItemData(
            HbDataFormModelItem::ComboBoxItem,
            hbTrId("txt_phone_setlabel_call_waiting"),
            this);
    QStringList list;
    QMapIterator<int, QString> mapIterator(m_callWaitingSettingMap);
    while (mapIterator.hasNext()) {
        mapIterator.next();
        list << mapIterator.value(); 
        }; 
    m_DataItemCallWaiting->setContentWidgetData("items", QVariant(list));
    appendChild(m_DataItemCallWaiting);
    
    DPRINT << ": OUT";
}

/*!
  CpCallsPluginGroup::connectShowCallDurationItem.
 */
void CpCallsPluginGroup::connectShowCallDurationItem()
{
    DPRINT << ": IN";
    
    m_helper.addConnection(
        m_DataItemShowCallDuration, SIGNAL(stateChanged(int)),
        this, SLOT(showCallDurationStateChanged(int)));

    DPRINT << ": OUT";
}
 
/*!
  CpCallsPluginGroup::connectSoftRejectItem.
 */
void CpCallsPluginGroup::connectSoftRejectItem()
{
    DPRINT << ": IN";

    m_helper.addConnection(
        m_DataItemSoftRejectTextEditor, SIGNAL(editingFinished()),
        this, SLOT(softRejectTextChanged()));

    DPRINT << ": OUT";
}

/*!
  CpCallsPluginGroup::connectCLIItem.
 */
void CpCallsPluginGroup::connectCLIItem()
{
    DPRINT << ": IN";
    
    m_helper.addConnection(
        m_DataItemCLI, SIGNAL(currentIndexChanged(int)),
        this, SLOT(cliCurrentIndexChanged(int)));
    // Fetch current CLI value 
    int cliValue(ClirNetworkDefault);
    int err = m_ssSettingsWrapper->get(Clir, cliValue);
    if (!err){
        QVariant indexValue(cliValue);
        m_DataItemCLI->setContentWidgetData(
            QString("currentIndex"), indexValue);
    } else {
        DPRINT << "Error: " << err;
    }
    
    DPRINT << ": OUT";
}

/*!
  CpCallsPluginGroup::connectCallWaitingItem.
 */
void CpCallsPluginGroup::connectCallWaitingItem()
{
    DPRINT << ": IN";

    m_helper.addConnection(
        m_DataItemCallWaiting, SIGNAL(currentIndexChanged(int)),
        this, SLOT(callWaitingCurrentIndexChanged(int)));

    DPRINT << ": OUT";
}

/*!
  CpCallsPluginGroup::showCallDurationStateChanged.
 */
void CpCallsPluginGroup::showCallDurationStateChanged(int state)
{
    DPRINT << ": IN";

    DPRINT << "state:" << state;
    if (state == Qt::Checked) {
        m_cpSettingsWrapper->setShowCallDuration(true);
    } else {
        m_cpSettingsWrapper->setShowCallDuration(false);
    }

    DPRINT << ": OUT";
}

/*!
  CpCallsPluginGroup::softRejectTextChanged.
 */
void CpCallsPluginGroup::softRejectTextChanged()
{
    DPRINT << ": IN";
    
    QVariant text = m_DataItemSoftRejectTextEditor->contentWidgetData("text");
    QString softRejectText = text.toString();  
    if (!softRejectText.isNull()) {
        DPRINT << "softRejectText:" << softRejectText;
        m_cpSettingsWrapper->writeSoftRejectText(softRejectText, true);
    }

    DPRINT << ": OUT";
}

/*!
  CpCallsPluginGroup::callWaitingCurrentIndexChanged.
 */
void CpCallsPluginGroup::callWaitingCurrentIndexChanged(int index)
{
    DPRINT << ": IN";
    DPRINT << "index:" << index;

    if ( index >= 0 ) {
        QString settingMapString = m_callWaitingSettingMap.value(index);
        DPRINT << "settingMapString:" << settingMapString;
        switch (index) {
            case PSetCallWaitingWrapper::ActivateCallWaiting: {
                DPRINT << "activate";
                m_callWaitingWrapper->setCallWaiting(
                        PSetCallWaitingWrapper::ActivateCallWaiting,
                        AllTeleAndBearer);
                }
                break; 
            case PSetCallWaitingWrapper::DeactivateCallWaiting: {
                DPRINT << "deactivate";
                m_callWaitingWrapper->setCallWaiting(
                        PSetCallWaitingWrapper::DeactivateCallWaiting,
                        AllTeleAndBearer);
                }
                break; 
            case PSetCallWaitingWrapper::CheckCallWaitingStatus: {
                DPRINT << "check status";
                m_callWaitingWrapper->getCallWaitingStatus();
                }
                break;
            default: 
                DPRINT << "Error: unknown enum value";
                break; 
        }
    } else {
        DPRINT << "Error: negative index!";
    }
    
    DPRINT << ": OUT";
}

/*!
  CpCallsPluginGroup::cliCurrentIndexChanged.
 */
void CpCallsPluginGroup::cliCurrentIndexChanged(int index)
{
    DPRINT << ": IN";

    if (index >= 0) {
        QString settingMapString = m_cliSettingMap.value(index);
        DPRINT << "settingMapString:" << settingMapString;
        int err = m_ssSettingsWrapper->set(Clir, index);
        DPRINT << "m_ssSettingsWrapper->set() err: " << err;
    } else {
        DPRINT << "Error: negative index!";
    }

    DPRINT << ": OUT";
}
    
/*!
  CpCallsPluginGroup::insertMappedListItems.
 */
void CpCallsPluginGroup::insertMappedListItems()
{
    DPRINT << ": IN";

    m_cliSettingMap.insert(ClirNetworkDefault, hbTrId("txt_phone_setlabel_val_default"));
    m_cliSettingMap.insert(ClirExplicitSuppress, hbTrId("txt_phone_setlabel_val_yes"));
    m_cliSettingMap.insert(ClirExplicitInvoke, hbTrId("txt_phone_setlabel_val_no"));

    m_callWaitingSettingMap.insert(
        PSetCallWaitingWrapper::ActivateCallWaiting, hbTrId("txt_phone_setlabel_call_waiting_val_on"));
    m_callWaitingSettingMap.insert(
        PSetCallWaitingWrapper::DeactivateCallWaiting, hbTrId("txt_phone_setlabel_call_waiting_val_off"));
    m_callWaitingSettingMap.insert(
        PSetCallWaitingWrapper::CheckCallWaitingStatus, hbTrId("Check status"));
    
    DPRINT << ": OUT";
}

/*!
  CpCallsPluginGroup::handleCallWaitingGetStatus.
 */
void CpCallsPluginGroup::handleCallWaitingGetStatus( 
    PSetCallWaitingWrapper::PsCallWaitingStatus status,
    const QList<unsigned char> &basicServiceGroupIds)
{
    DPRINT << ": IN";
    emit cancelNote(m_activeNoteId);
    
    if (m_callWaitingDistinguishEnabled &&
            status == PSetCallWaitingWrapper::StatusNotProvisioned) {
        emit showGlobalNote(m_activeNoteId, 
            hbTrId("txt_phone_info_request_not_completed"), HbMessageBox::MessageTypeInformation);
    } else if (status != PSetCallWaitingWrapper::StatusActive) {
        emit showGlobalNote(m_activeNoteId,
            hbTrId("txt_phone_info_call_waiting_deactivated"), HbMessageBox::MessageTypeInformation);
    } else {
        emit showGlobalNote(m_activeNoteId,
            hbTrId("txt_phone_info_call_waiting_activated"), HbMessageBox::MessageTypeInformation);
    }
    
    DPRINT << ": OUT";
}

/*!
  CpCallsPluginGroup::handleCallWaitingChanged.
 */
void CpCallsPluginGroup::handleCallWaitingChanged( 
    PSetCallWaitingWrapper::PsCallWaitingCommand command,
    int result )
{
    DPRINT << ": IN";
    emit cancelNote(m_activeNoteId);
    if (result) {
        emit showGlobalNote(m_activeNoteId,
            hbTrId("txt_phone_info_request_not_confirmed"), 
            HbMessageBox::MessageTypeWarning);
    } else {
        switch (command){
            case PSetCallWaitingWrapper::ActivateCallWaiting:
                emit showGlobalNote(m_activeNoteId,
                    hbTrId("txt_phone_info_call_waiting_activated"), 
                    HbMessageBox::MessageTypeInformation);
                break;
            case PSetCallWaitingWrapper::DeactivateCallWaiting:
                emit showGlobalNote(m_activeNoteId,
                    hbTrId("txt_phone_info_call_waiting_deactivated"), 
                    HbMessageBox::MessageTypeInformation);
                break;
            default: 
                emit showGlobalNote(m_activeNoteId,
                    hbTrId("txt_phone_info_result_unknown"), 
                    HbMessageBox::MessageTypeInformation);
                break;
        }
    }
    DPRINT << ": OUT";
} 

/*!
  CpCallsPluginGroup::handleCallWaitingRequesting.
 */
void CpCallsPluginGroup::handleCallWaitingRequesting(bool ongoing, bool interrupted)
{
    DPRINT << ": IN";
    DPRINT << "ongoing:" << ongoing << ", interrupted:" << interrupted;
    Q_ASSERT(!(ongoing && interrupted));
    
    if (ongoing) {
        emit showGlobalProgressNote(m_activeNoteId, hbTrId("txt_phone_info_requesting"));
    }
    
    if (interrupted) {
        emit cancelNote(m_activeNoteId);
    }
    
    DPRINT << ": OUT";
}

/*!
  CpCallsPluginGroup::handleCallWaitingError.
 */
void CpCallsPluginGroup::handleCallWaitingError(int errorCode)
{
    DPRINT << ": IN";
    DPRINT << "errorCode:" << errorCode;
    
    emit cancelNote(m_activeNoteId);
    
    emit showGlobalErrorNote(m_activeNoteId, errorCode);
    
    DPRINT << ": OUT";
}

// End of File. 
