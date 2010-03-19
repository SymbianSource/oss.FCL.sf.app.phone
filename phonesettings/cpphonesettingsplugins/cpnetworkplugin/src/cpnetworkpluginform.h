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

#ifndef CPNETWORKPLUGINFORM_H
#define CPNETWORKPLUGINFORM_H

#include <hbdataform.h>
#include <QStringList>
#include <psetnetworkwrapper.h>
#include <hbmessagebox.h>

class HbDataFormModelItem;
class CpSettingsWrapper;
class PSetWrapper;
class HbListWidget;
class PSetNetworkWrapper;
class CpSettingFormItemData;
class CpPhoneNotes;

class CpNetworkPluginForm : public HbDataForm
{
    Q_OBJECT
    
public:
    
    explicit CpNetworkPluginForm(QGraphicsItem *parent = 0);
    
    virtual ~CpNetworkPluginForm();
    
signals:
    
    void showGlobalProgressNote(int &noteId, const QString& text);
    void showGlobalNote(
        int &noteId, const QString& text, HbMessageBox::MessageBoxType msgBoxType);
    void cancelNote(int noteId);

public slots: 

    void networkModeStateChanged(int index);
    void operatorSelectionStateChanged(bool index);
    void networkAccessModeGot(int mode);
    void availableNetworksGot(
        QList<PSetNetworkWrapper::NetworkInfo*> &m_networkInfoList);
    void networkReqestFailed(
        PSetNetworkWrapper::ErrorCode error, 
        PSetNetworkWrapper::RequestType type);
    void userCancel();
    void handleSearchingNetworks(PSetNetworkWrapper::RequestType &type);
    void handleRequestingSelectedNetwork(bool ongoing);
    void handleNetworkChanged(
        PSetNetworkWrapper::NetworkInfo& currentInfo,
        PSetNetworkWrapper::RegistrationStatus& status);

private:     
    HbDataFormModelItem *createNetworkModeItem();
    HbDataFormModelItem *createOperatorSelectionItem();
    
    void automaticOperatorSelection();
    void manualOperatorSelection();
    
    void dualModeSelection();
    void umtsSelection();
    void gsmSelection();
    
    HbDialog* createDialog(const QString& heading) const;
    void addItemToListWidget(
        HbListWidget* w, const QString& item, const int& data) const;
    
    void showManualSeletiondialog();
    void restoreUiSelection();
    QString networkName(PSetNetworkWrapper::NetworkInfo &info);

    void connectToNetworkWrapper(PSetNetworkWrapper &wrapper);
    
    void connectToPhoneNotes(CpPhoneNotes &notes);
    
    bool isPhoneOnLine();
    
private:
    
    QStringList mNetworkModeOptions;
    QStringList mOperatorSelectionOptions;
    int m_activeNoteId;
    int m_activeProgressNoteId;
    // Own
    PSetWrapper *m_pSetWrapper;
    CpSettingsWrapper *m_cpSettingsWrapper;
    
    // Not own
    PSetNetworkWrapper *m_psetNetworkWrapper;
    CpSettingFormItemData *m_NetworkModeOptionsItemData;
    CpSettingFormItemData *m_NetworkOperatorSelectionItemData;
    QList<PSetNetworkWrapper::NetworkInfo*> *m_networkInfoList;
};

#endif  // CPNETWORKPLUGINFORM_H
