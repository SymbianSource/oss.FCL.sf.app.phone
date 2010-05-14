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
#include <hbdataform.h>
#include <hbdataformmodel.h>
#include <hbdataformmodelitem.h>
#include <hbdataformviewitem.h>
#include <hbdialog.h>
#include <hbaction.h>
#include <hblistwidget.h>
#include <hblistwidgetitem.h>
#include <hblabel.h>
#include <cpsettingformitemdata.h>
#include <psetwrapper.h>

#include "cpnetworkpluginform.h"
#include "cpplugincommon.h"
#include "cppluginlogging.h"
#include "cpphonenotes.h"
#include "cellulardatasettings.h"

/*!
  CpNetworkPluginForm::CpNetworkPluginForm
 */
CpNetworkPluginForm::CpNetworkPluginForm(QGraphicsItem *parent) :
    HbDataForm(parent),
    m_activeNoteId(0),
    m_activeProgressNoteId(0),
    m_pSetWrapper(NULL),
    m_cpSettingsWrapper(NULL),
    mCellularSettings(),
    m_dialog(NULL)
{
    DPRINT << ": IN";
    
    setHeading(hbTrId("txt_cp_subhead_mobile_network"));
    
    QScopedPointer<PSetWrapper> pSetWrapperGuard(new PSetWrapper);
    m_psetNetworkWrapper = &pSetWrapperGuard->networkWrapper();
    mCellularSettings = QSharedPointer<CellularDataSettings>(new CellularDataSettings);
    connectToNetworkWrapper(*m_psetNetworkWrapper);
    connectToPhoneNotes(*CpPhoneNotes::instance());
    
    QScopedPointer<HbDataFormModel> model(new HbDataFormModel);
    QScopedPointer<CpSettingsWrapper> cpSettingsWrapperGuard(new CpSettingsWrapper);

    // #1 Read Network mode value from Cenrep
    model->appendDataFormItem(createNetworkModeItem());
    // #2 Use phone settings for operator selection
    model->appendDataFormItem(createOperatorSelectionItem());
    // Home network cellular data usage
    model->appendDataFormItem(createHomeDataUsageItem());
    // Cellular network data usage when roaming
    model->appendDataFormItem(createRoamDataUsageItem());
    setModel(model.take());
    m_pSetWrapper = pSetWrapperGuard.take();
    m_cpSettingsWrapper = cpSettingsWrapperGuard.take();

    if(!isPhoneOnLine()) {
        DPRINT << "offline, set dimmed";
        m_NetworkOperatorSelectionItemData->setEnabled(false);
    }

    DPRINT << ": OUT";
}


/*!
  CpNetworkPluginForm::~CpNetworkPluginForm
 */
CpNetworkPluginForm::~CpNetworkPluginForm()
{
    DPRINT << ": IN";
    
    delete m_pSetWrapper;
    delete m_cpSettingsWrapper;
    
    DPRINT << ": OUT";
}


/*!
  CpNetworkPluginForm::createNetworkModeItem
  */
HbDataFormModelItem *CpNetworkPluginForm::createNetworkModeItem()
{
    // #1A Read network mode value from Cenrep 
    DPRINT << ": IN";

    m_NetworkModeOptionsItemData = new CpSettingFormItemData(
        HbDataFormModelItem::ComboBoxItem, 
        hbTrId("txt_cp_setlabel_network_mode"), 
        NULL );
    QScopedPointer<CpSettingFormItemData> settingFormItemGuard(
        m_NetworkModeOptionsItemData);
    
    mNetworkModeOptions << hbTrId("txt_cp_setlabel_network_mode_val_dual_mode")
                        << hbTrId("txt_cp_setlabel_network_mode_val_umts")
                        << hbTrId("txt_cp_setlabel_network_mode_val_gsm");
    m_NetworkModeOptionsItemData->setContentWidgetData(
        "items", QVariant(mNetworkModeOptions));
    
    QObject::connect(
        m_psetNetworkWrapper, 
        SIGNAL(networkAccessModeGot(int)),
        this, 
        SLOT(networkAccessModeGot(int)));
    QObject::connect(
        m_psetNetworkWrapper, 
        SIGNAL(availableNetworksGot(QList<PSetNetworkWrapper::NetworkInfo*> &)),
        this, 
        SLOT(availableNetworksGot(QList<PSetNetworkWrapper::NetworkInfo*> &)));
    
    m_psetNetworkWrapper->getNetworkAccessMode();
    
    addConnection(
        m_NetworkModeOptionsItemData, 
        SIGNAL(currentIndexChanged(int)),
        this, 
        SLOT(networkModeStateChanged(int)));
    
    settingFormItemGuard.take();
    DPRINT << ": OUT";
    return m_NetworkModeOptionsItemData;
}


/*!
  CpNetworkPluginForm::createOperatorSelectionItem
  */
HbDataFormModelItem *CpNetworkPluginForm::createOperatorSelectionItem()
{
    // #2A Use phone settings for operator selection
    DPRINT << ": IN";
    
    m_NetworkOperatorSelectionItemData = new CpSettingFormItemData(
        HbDataFormModelItem::ToggleValueItem, 
        hbTrId("txt_cp_setlabel_operator_selection"), 
        NULL);
    QScopedPointer<CpSettingFormItemData> settingFormItemGuard(
        m_NetworkOperatorSelectionItemData);
    
    PSetNetworkWrapper::NetworkSelectionMode mode;
    PSetNetworkWrapper::ErrorCode err = 
        m_psetNetworkWrapper->getNetworkSelectionMode(mode);
    if(err == PSetNetworkWrapper::ErrNoError) {
        switch (mode) {
            case PSetNetworkWrapper::SelectionModeAutomatic: 
                m_NetworkOperatorSelectionItemData->setContentWidgetData(
                    "text", QVariant(hbTrId("txt_cp_setlabel_operator_selection_val_automatic")));
                break;
            case PSetNetworkWrapper::SelectionModeManual: 
                m_NetworkOperatorSelectionItemData->setContentWidgetData(
                    "text", QVariant(hbTrId("txt_cp_setlabel_operator_selection_val_manual")));
                break;
            default:
                break;
        }
    }
    
    addConnection(
        m_NetworkOperatorSelectionItemData, SIGNAL(clicked(bool) ),
        this, SLOT(operatorSelectionStateChanged(bool)));
    
    settingFormItemGuard.take();
    DPRINT << ": OUT";
    return m_NetworkOperatorSelectionItemData;
}

/*!
  CpNetworkPluginForm::createHomeDataUsageItem
  */
HbDataFormModelItem *CpNetworkPluginForm::createHomeDataUsageItem()
{
    DPRINT << ": IN";
    
    mCellularUseHomeSelectionItemData = new CpSettingFormItemData(
        HbDataFormModelItem::ComboBoxItem, 
        hbTrId("txt_cp_setlabel_data_usage_in_home_network"), 
        NULL);
    QScopedPointer<CpSettingFormItemData> settingFormItemGuard(
        mCellularUseHomeSelectionItemData);

    // Populate the dropdown with selection items
    mCellularUseHomeOptions
        << hbTrId("txt_cp_setlabel_data_usage_in_val_automatic")
        << hbTrId("txt_cp_setlabel_data_usage_in_val_confirm")
        << hbTrId("txt_cp_setlabel_data_usage_in_val_disabled");
    mCellularUseHomeSelectionItemData->setContentWidgetData(
        "items",
        QVariant(mCellularUseHomeOptions));

    // Set initial selection based on current setting
    mCellularUseHomeSelectionItemData->setContentWidgetData(
        "currentIndex",
        mCellularSettings->dataUseHome());      // NOTE: Indexes must match!

    addConnection(
        mCellularUseHomeSelectionItemData, 
        SIGNAL(currentIndexChanged(int)),
        this, 
        SLOT(cellularDataUseHomeStateChanged(int)));
    
    settingFormItemGuard.take();
    DPRINT << ": OUT";
    return mCellularUseHomeSelectionItemData;
}

/*!
  CpNetworkPluginForm::cellularDataUseHomeStateChanged
  */
void CpNetworkPluginForm::cellularDataUseHomeStateChanged(int index)
{
    DPRINT << ": IN : index: " << index;

    mCellularSettings->setDataUseHome(index);
    
    DPRINT << ": OUT";
}

/*!
  CpNetworkPluginForm::createRoamDataUsageItem
  */
HbDataFormModelItem *CpNetworkPluginForm::createRoamDataUsageItem()
{
    DPRINT << ": IN";
    
    mCellularUseRoamSelectionItemData = new CpSettingFormItemData(
        HbDataFormModelItem::ComboBoxItem, 
        hbTrId("txt_cp_setlabel_data_usage_when_roaming"), 
        NULL);
    QScopedPointer<CpSettingFormItemData> settingFormItemGuard(
        mCellularUseRoamSelectionItemData);

    // Populate the dropdown with selection items
    mCellularUseRoamOptions
        << hbTrId("txt_cp_setlabel_data_usage_when_val_automatic")
        << hbTrId("txt_cp_setlabel_data_usage_when_val_confirm")
        << hbTrId("txt_cp_setlabel_data_usage_when_val_disabled");
    mCellularUseRoamSelectionItemData->setContentWidgetData(
        "items",
        QVariant(mCellularUseRoamOptions));

    // Set initial selection based on current setting
    mCellularUseRoamSelectionItemData->setContentWidgetData(
        "currentIndex",
        mCellularSettings->dataUseRoam());      // NOTE: Indexes must match!

    addConnection(
        mCellularUseRoamSelectionItemData, 
        SIGNAL(currentIndexChanged(int)),
        this, 
        SLOT(cellularDataUseRoamStateChanged(int)));
    
    settingFormItemGuard.take();
    DPRINT << ": OUT";
    return mCellularUseRoamSelectionItemData;
}

/*!
  CpNetworkPluginForm::cellularDataUseRoamStateChanged
  */
void CpNetworkPluginForm::cellularDataUseRoamStateChanged(int index)
{
    DPRINT << ": IN : index: " << index;

    mCellularSettings->setDataUseRoam(index);
    
    DPRINT << ": OUT";
}

/*!
  CpNetworkPluginForm::networkModeStateChanged
  */
void CpNetworkPluginForm::networkModeStateChanged(int index)
{
    // #1C network mode changed
    DPRINT << ": IN : index: " << index;
    
    if(isPhoneOnLine()) {
        if(index == 0) {
            dualModeSelection();
        } else if(index == 1) {
            umtsSelection();
        } else if(index == 2) {
            gsmSelection();
        }
    } else {
        // offline mode.
        emit showGlobalNote(
            m_activeNoteId, 
            emit hbTrId("Operation not possible in Off-line mode"), 
            HbMessageBox::MessageTypeInformation);
    }
        
    DPRINT << ": OUT";
}


/*!
  CpNetworkPluginForm::operatorSelectionStateChanged
  */
void CpNetworkPluginForm::operatorSelectionStateChanged(bool)
{
    // #2C operator selection mode changed
    DPRINT << ": IN ";
    
    if (isPhoneOnLine()) {
        PSetNetworkWrapper::NetworkSelectionMode mode;
        m_psetNetworkWrapper->getNetworkSelectionMode(mode);
        switch (mode) {
            case PSetNetworkWrapper::SelectionModeAutomatic: 
                manualOperatorSelection();
                m_NetworkOperatorSelectionItemData->setContentWidgetData(
                    "text", QVariant(hbTrId("txt_cp_setlabel_operator_selection_val_manual")));
                break;
            case PSetNetworkWrapper::SelectionModeManual: 
                automaticOperatorSelection();
                m_NetworkOperatorSelectionItemData->setContentWidgetData(
                    "text", QVariant(hbTrId("txt_cp_setlabel_operator_selection_val_automatic")));
                break;
            default:
                break;
            }
    } else {
        // offline mode.
        emit showGlobalNote(
            m_activeNoteId, 
            emit hbTrId("Operation not possible in Off-line mode"), 
            HbMessageBox::MessageTypeInformation);
    }
    
    DPRINT << ": OUT";
}


/*!
  CpNetworkPluginForm::automaticOperatorSelection
  */
void CpNetworkPluginForm::automaticOperatorSelection()
{
    DPRINT << ": IN";
    
    PSetNetworkWrapper::NetworkInfo info;
    info.m_mode = PSetNetworkWrapper::SelectionModeAutomatic;
    m_psetNetworkWrapper->selectNetwork(info);
    
    DPRINT << ": OUT";
}


/*!
  CpNetworkPluginForm::manualOperatorSelection
  */
void CpNetworkPluginForm::manualOperatorSelection()
{
    DPRINT << ": IN";
    
    m_psetNetworkWrapper->getAvailableNetworks();
        
    DPRINT << ": OUT";
}


/*!
  CpNetworkPluginForm::dualModeSelection
  */
void CpNetworkPluginForm::dualModeSelection()
{
    DPRINT << ": IN";
    
    m_psetNetworkWrapper->setNetworkAccessMode(
        PSetNetworkWrapper::AccessModeDual);
    
    DPRINT << ": OUT";
}


/*!
  CpNetworkPluginForm::umtsSelection
  */
void CpNetworkPluginForm::umtsSelection()
{
    DPRINT << ": IN";
    
    m_psetNetworkWrapper->setNetworkAccessMode(
        PSetNetworkWrapper::AccessModeUmts);
    
    DPRINT << ": OUT";
}


/*!
  CpNetworkPluginForm::gsmSelection
  */
void CpNetworkPluginForm::gsmSelection()
{
    DPRINT << ": IN";
    
    m_psetNetworkWrapper->setNetworkAccessMode(
        PSetNetworkWrapper::AccessModeGsm);
    
    DPRINT << ": OUT";
}


/*!
  CpNetworkPluginForm::networkAccessModeGot
  */
void CpNetworkPluginForm::networkAccessModeGot(
    int mode)
{
    DPRINT << ": IN";
    
    switch (mode) {
        case PSetNetworkWrapper::AccessModeDual: 
            m_NetworkModeOptionsItemData->setContentWidgetData(
                "currentIndex", 0);
            break;
        case PSetNetworkWrapper::AccessModeUmts: 
            m_NetworkModeOptionsItemData->setContentWidgetData(
                "currentIndex", 1);
            break;
        case PSetNetworkWrapper::AccessModeGsm: 
            m_NetworkModeOptionsItemData->setContentWidgetData(
                "currentIndex", 2);
            break;
        default:
            break;
    }
    
    DPRINT << ": OUT";
}


/*!
  CpNetworkPluginForm::availableNetworksGot
  */
void CpNetworkPluginForm::availableNetworksGot(
        QList<PSetNetworkWrapper::NetworkInfo*> &networkInfoList)
{
    DPRINT << ": IN";
    
    m_networkInfoList = &networkInfoList;
    showManualSelectiondialog();
    
    DPRINT << ": OUT";
}


/*!
  CpNetworkPluginForm::createDialog
  */
HbDialog* CpNetworkPluginForm::createDialog(const QString& heading) const
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
  CpNetworkPluginForm::createDialog
  */
void CpNetworkPluginForm::addItemToListWidget(HbListWidget* w,
        const QString& item, const int& data) const
{
    DPRINT << ": IN";
    Q_ASSERT(w != 0);
    
    HbListWidgetItem* o = new HbListWidgetItem();
    o->setText(item);
    o->setData(data);
    w->addItem(o);
    
    DPRINT << ": OUT";
}


/*!
  CpNetworkPluginForm::networkReqestFailed
  */
void CpNetworkPluginForm::networkReqestFailed(
        PSetNetworkWrapper::ErrorCode error,
        PSetNetworkWrapper::RequestType type)
{
    DPRINT << ": IN";
    
    QString text;
    switch(error) {
        case PSetNetworkWrapper::ErrCauseCallActive:
            text = hbTrId("txt_cp_info_active_calls_must_be_disconnected_befo");
            break;
        case PSetNetworkWrapper::ErrNoNetworkService:
            text = hbTrId("txt_cp_info_no_operators_found");
            break;
        case PSetNetworkWrapper::ErrOfflineOpNotAllowed:
            text = hbTrId("Off-line, not allowed");
            break;
        case PSetNetworkWrapper::ErrNoNetworkAccess:
            text = hbTrId("txt_cp_info_no_access_to_selected_operators_netwo");
            break;
        default:
            break;
    }
    emit showGlobalNote(
        m_activeNoteId, text, HbMessageBox::MessageTypeInformation);
    
    // Update UI
    if(type == PSetNetworkWrapper::RequestSetNetworkMode) {
       m_psetNetworkWrapper->getNetworkAccessMode();
       }
    else if(type == PSetNetworkWrapper::RequestSetNetwork && 
            error == PSetNetworkWrapper::ErrNoNetworkAccess) {
        showManualSelectiondialog();
    }
    else if(type == PSetNetworkWrapper::RequestSetNetwork ||
            type == PSetNetworkWrapper::RequestSetNetworkAutomatic ||
            type == PSetNetworkWrapper::RequestGetNetworkInfo) {
        restoreUiSelection();
    }
    
    DPRINT << ": OUT";
}


/*!
  CpNetworkPluginForm::userCancel
  */
void CpNetworkPluginForm::userCancel()
{
    DPRINT << ": IN";
    
    m_psetNetworkWrapper->cancelRequest();
    restoreUiSelection();
    
    DPRINT << ": OUT";
}


/*!
  CpNetworkPluginForm::handleSearchingNetworks
  */
void CpNetworkPluginForm::handleSearchingNetworks(
    PSetNetworkWrapper::RequestType &type)
{
    DPRINT << ": IN";
    
    if (type != PSetNetworkWrapper::RequestNone) {
        QObject::connect(
            CpPhoneNotes::instance(), SIGNAL(progressNoteCanceled()),
            this, SLOT(userCancel()));
        emit showGlobalProgressNote(m_activeProgressNoteId, hbTrId("txt_cp_info_updating"));
    } else {
        QObject::disconnect(
            CpPhoneNotes::instance(), SIGNAL(progressNoteCanceled()),
            this, SLOT(userCancel()));
        emit cancelNote(m_activeProgressNoteId);
        m_activeProgressNoteId = 0;
    }
    
    DPRINT << ": OUT";
}


/*!
  CpNetworkPluginForm::handleRequestingSelectedNetwork
  */
void CpNetworkPluginForm::handleRequestingSelectedNetwork(bool ongoing)
{
    DPRINT << ": IN";
    
    if (ongoing) {
        QObject::connect(
            CpPhoneNotes::instance(), SIGNAL(progressNoteCanceled()),
            this, SLOT(userCancel()));
        emit showGlobalProgressNote(m_activeProgressNoteId, hbTrId("Requesting"));
    } else {
        QObject::disconnect(
            CpPhoneNotes::instance(), SIGNAL(progressNoteCanceled()),
            this, SLOT(userCancel()));
        emit cancelNote(m_activeProgressNoteId);
        m_activeProgressNoteId = 0;
    }
    
    DPRINT << ": OUT";
}


/*!
  CpNetworkPluginForm::handleNetworkChanged
  */
void CpNetworkPluginForm::handleNetworkChanged(
    PSetNetworkWrapper::NetworkInfo& currentInfo,
    PSetNetworkWrapper::RegistrationStatus& status)
{
    DPRINT << ": IN";
    
    QString text;
    switch (status){
        case PSetNetworkWrapper::RegisteredOnHomeNetwork:
            text = hbTrId("Home network selected");
            break;
        case PSetNetworkWrapper::RegisteredRoaming: {
            text = hbTrId("Selected network: ");
            text.append(networkName(currentInfo));
        }
            break; 
    }
    
    emit showGlobalNote(
        m_activeNoteId, text, HbMessageBox::MessageTypeInformation);
    
    DPRINT << ": OUT";
}


/*!
  CpNetworkPluginForm::showManualSelectiondialog
  */
void CpNetworkPluginForm::showManualSelectiondialog()
{
    DPRINT << ": IN";
    
    QScopedPointer<HbDialog> dialog(
        createDialog(hbTrId("txt_cp_title_select_operator")));
    HbListWidget *list = new HbListWidget(dialog.data());
    //then insert found networks
    int itemsCount = m_networkInfoList->count();
    for (int i = 0; i < itemsCount; i++) {
        PSetNetworkWrapper::NetworkInfo *info = m_networkInfoList->at(i);
        QString text = networkName(*info);
        addItemToListWidget(list, text, i);
        DPRINT << ":  " << info;
    }
    // Connect list item activation signal to close the popup
    connect(
        list, SIGNAL(activated(HbListWidgetItem*)), 
        dialog.data(), SLOT(close()));
    // Sets the "Cancel"-action/button
    HbAction *cancelAction = new HbAction(hbTrId("txt_common_button_cancel"));
    dialog->addAction(cancelAction);
    dialog->setContentWidget(list);
    // Launch popup and handle the response
    dialog->open(this, SLOT(finishedManualSelectiondialog(HbAction*)));
    if (m_dialog) {
        m_dialog->deleteLater();
        m_dialog = NULL;
    }
    m_dialog = dialog.take();
    
    DPRINT << ": OUT";
}


/*!
  CpNetworkPluginForm::finishedManualSelectiondialog.
 */
void CpNetworkPluginForm::finishedManualSelectiondialog(HbAction* action)
{
    DPRINT << ": IN";
    
    if (m_dialog) {
        QList<QAction *> actionList = m_dialog->actions();
        bool cancelAction = actionList.contains(action);
        if (!cancelAction) {
            // Update the view with selected text
            QGraphicsWidget *contectWidget = m_dialog->contentWidget();
            HbListWidget *list(NULL);
            if (contectWidget) {
                list = qobject_cast<HbListWidget *>(contectWidget);
            }
            if (list) {
                int seletion = 0;
                if(list->currentItem()) {
                    seletion = list->currentItem()->data().toInt();
                    DPRINT << ": seletion : " << seletion;
                }
                PSetNetworkWrapper::NetworkInfo param;
                param.m_id.m_countryCode = 
                    m_networkInfoList->at(seletion)->m_id.m_countryCode;
                param.m_id.m_networkCode = 
                    m_networkInfoList->at(seletion)->m_id.m_networkCode;
                param.m_mode = PSetNetworkWrapper::SelectionModeManual;
                DPRINT << ": m_countryCode : " << param.m_id.m_countryCode;
                DPRINT << ": m_networkCode : " << param.m_id.m_networkCode;
                DPRINT << ": m_mode : " << param.m_mode;
                m_psetNetworkWrapper->selectNetwork(param);
            }
        } else {
            DPRINT << ": Cancel";
            m_psetNetworkWrapper->cancelRequest();
            restoreUiSelection();
        }
        m_dialog->deleteLater();
        m_dialog = NULL;
    }
    
    DPRINT << ": OUT";
}

/*!
  CpNetworkPluginForm::restoreUiSelection
  */
void CpNetworkPluginForm::restoreUiSelection()
{
    DPRINT << ": IN";
    
    removeConnection(
        m_NetworkOperatorSelectionItemData, SIGNAL(currentIndexChanged(int)),
        this, SLOT(operatorSelectionStateChanged(int)));
    PSetNetworkWrapper::NetworkSelectionMode mode;
    PSetNetworkWrapper::ErrorCode err = 
        m_psetNetworkWrapper->getNetworkSelectionMode(mode);
    if(err == PSetNetworkWrapper::ErrNoError) {
        switch (mode) {
            case PSetNetworkWrapper::SelectionModeAutomatic: 
                m_NetworkOperatorSelectionItemData->setContentWidgetData(
                    "text", QVariant(hbTrId("txt_cp_setlabel_operator_selection_val_automatic")));
                break;
            case PSetNetworkWrapper::SelectionModeManual: 
                m_NetworkOperatorSelectionItemData->setContentWidgetData(
                    "text", QVariant(hbTrId("txt_cp_setlabel_operator_selection_val_manual")));
                break;
            default:
                break;
            }
    }
    addConnection(
        m_NetworkOperatorSelectionItemData, SIGNAL(currentIndexChanged(int)),
        this, SLOT(operatorSelectionStateChanged(int)));
    
    DPRINT << ": OUT";
}


/*!
  CpNetworkPluginForm::networkName()
  */
QString CpNetworkPluginForm::networkName(
    PSetNetworkWrapper::NetworkInfo &info)
{
    DPRINT << ": IN";
    
    QString text;
    if (!info.m_longName.isEmpty())
        {
        text.append(info.m_longName);
        }
    else if (!info.m_shortName.isEmpty())
        {
        text.append(info.m_shortName);
        }
    else //network returned only ids
        {
        text.append(info.m_id.m_countryCode);
        text.append(" ");
        text.append(info.m_id.m_networkCode);
        }
    
    DPRINT << ": OUT : " << text;
    return text;
}


/*!
  CpNetworkPluginForm::connectToNetworkWrapper
 */
void CpNetworkPluginForm::connectToNetworkWrapper(PSetNetworkWrapper &wrapper)
{
    QObject::connect(
        &wrapper, 
        SIGNAL(networkReqestFailed(
            PSetNetworkWrapper::ErrorCode,PSetNetworkWrapper::RequestType)),
        this, 
        SLOT(networkReqestFailed(
            PSetNetworkWrapper::ErrorCode,PSetNetworkWrapper::RequestType)));
    
    QObject::connect(
        &wrapper, 
        SIGNAL(searchingNetworks(PSetNetworkWrapper::RequestType &)),
        this, 
        SLOT(handleSearchingNetworks(PSetNetworkWrapper::RequestType &)));
    
    QObject::connect(
        &wrapper, 
        SIGNAL(requestingSelectedNetwork(bool)),
        this, 
        SLOT(handleRequestingSelectedNetwork(bool)));
    
    QObject::connect(
        &wrapper, 
        SIGNAL(networkChanged(
            PSetNetworkWrapper::NetworkInfo&, 
            PSetNetworkWrapper::RegistrationStatus&)),
        this, 
        SLOT(handleNetworkChanged(
            PSetNetworkWrapper::NetworkInfo&, 
            PSetNetworkWrapper::RegistrationStatus&)));
}


/*!
  CpNetworkPluginForm::connectToPhoneNotes
 */
void CpNetworkPluginForm::connectToPhoneNotes(CpPhoneNotes &notes)
{
    QObject::connect(
        this, 
        SIGNAL(showGlobalProgressNote(int &, const QString&)),
        &notes, 
        SLOT(showGlobalProgressNote(int &, const QString&)));
    
    QObject::connect(
        this, 
        SIGNAL(showGlobalNote(
            int &, const QString&, HbMessageBox::MessageBoxType)),
        &notes, 
        SLOT(showGlobalNote(
            int &, const QString&, HbMessageBox::MessageBoxType)));
    
    QObject::connect(
        this, 
        SIGNAL(cancelNote(int)),
        &notes, 
        SLOT(cancelNote(int)));
}

/*!
  CpNetworkPluginForm::isPhoneOnLine
 */
bool CpNetworkPluginForm::isPhoneOnLine()
{
    DPRINT << ": IN";
    
    bool onLine(true);
    if (m_cpSettingsWrapper->isPhoneOffline()) {
        onLine = false;
    }
    
    DPRINT << ": OUT : onLine : " << onLine;
    return onLine;
}

/*!
  CpNetworkPluginForm::searchAvailableNetworks
 */
void CpNetworkPluginForm::searchAvailableNetworks()
{
    DPRINT << ": IN";
    
    m_psetNetworkWrapper->cancelRequest();
    manualOperatorSelection();
        
    DPRINT << ": OUT";
}

// End of File. 
