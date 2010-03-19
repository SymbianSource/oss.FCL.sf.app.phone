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
#ifndef CPDIVERTPLUGINGROUP_H
#define CPDIVERTPLUGINGROUP_H

#include <QMap>
#include <QString>
#include <QQueue>
#include <hbdevicemessagebox.h>
#include <cpsettingformitemdata.h>
#include <psetcalldivertingwrapper.h>
#include "cptelephonyutilsdefs.h"
#include "cppluginlogging.h"


class HbDataFormViewItem; 
class PSetWrapper;
class SsSettingsWrapper;
class HbComboBox;
class PSCallDivertingCommand;
class HbListWidget;
class CpDivertItemData;

using namespace CpTelephonyUtils;

/*!
    \class CpDivertPluginGroup
    \brief The class CpDivertPluginGroup defines items shown on UI. 
 */
class CpDivertPluginGroup : public CpSettingFormItemData
{
    Q_OBJECT

    enum DivertRequest{
        ActivateDivert = 0, 
        DeactivateDivert, 
        CheckDivertStatus
    };  

    class CpDivertRequestQueueItem
    {
    public:
        DivertRequest request;
        CpDivertItemData *item;
    };
    
    
public:

    explicit CpDivertPluginGroup(CpItemDataHelper &helper);
    
    ~CpDivertPluginGroup();

public slots:
    
    // PSetCallDivertingWrapper signal handling
    /**
     Set info request results
     */
    void handleDivertingChanged(
            const PSCallDivertingCommand& aSetting, 
            bool aPlural);
    
    /**
     Get info request results
     */
    void handleDivertingStatus(
        QList<PSCallDivertingStatus*>& divertList, 
        bool plural);
    
    /**
     Request failed.
     */
    void handleDivertingError(int aReason);

    /**
     Adaptation has handled previous request.
     */
    void divertRequestProcessed();
    
    /**
     Slot which handles change requests according UI.
     */
    void changeDivertingStateRequested(CpDivertItemData &item);

    /**
     Slot which handles item updating when it is needed.
     */
    void itemShown(const QModelIndex& item);
private:     
    
    /**
     Creates group items.
     */
    void createVoiceCallItems();

    /**
     Creates group items.
     */
    void createVideoCallItems();
    
    /**
     Creates group items.
     */
    void createVoIPCallItems();
    
    /**
     Show to user divert number query list.
     */
    bool popUpVoiceNumberListQuery(
            const QString& heading, QString& result);
    
    /**
     Show to user number query.
     */
    bool popUpNumberEditor(
            const QString& heading, QString& result);
    
    
    /**
     Send comand to psetwrappercalldiverting.
     */
    void setCallDiverting(PSCallDivertingCommand& command);
    
    /**
     Show to user divert time out query list.
     */
    bool popUpTimerQuery(int &timeout) const;
    
    /**
     Desides which bsc parameters to use.
     */
    int bscParam();
    
    /**
     Adds divert data item for wanted parent.
     */
    void addDivertDataItem(CpDivertItemData* item, CpSettingFormItemData *parent);

    /**
     Process next divert request.
     */
    void processDivertingRequestQueue();

    /**
     Add new request to divert queue.
     */
    void addToDivertingRequestQueue(
            DivertRequest request, 
            CpDivertItemData &item);
    
    /**
     Changes divertItemData data.
     */
    void changeItemData(
            PsCallDivertingCondition condition, 
            PsCallDivertingStatus status,
            const QString& number, 
            int timeout);
    
    /**
     Reverts divertItemData data to the last known valid values.
     */
    void revertItemData(PsCallDivertingCondition condition);

private:
    
    /**
     Creates dialog with default parameters
     */
    HbDialog* createDialog( const QString& heading ) const;

    /**
     Adds item to given list widget
     */
    void addItemToListWidget(HbListWidget* w,
        const QString& item, const QString& data) const;

    /**
     Adds item to given list widget
     */
    void addItemToListWidget(HbListWidget* w,
        const QString& item, const int& data) const;

    /**
     Synchronizes content and status of divert options dependent from the 
     status of some other divert.
     */
    void updateDependentDivertOptions();
    
    /**
     Synchronizes content and status of not available divert option based on
     statuses of CFB, CFNry and CFNrc.
     */
    void updateNotAvailableDivertOption();
    
private:   // data

    // Setting wrappers 
    PSetWrapper *m_pSetWrapper;
    SsSettingsWrapper *m_ssSettingsWrapper;
    
    // Not own
    PSetCallDivertingWrapper* m_callDivertingWrapper;

    // Not owned objects
    CpSettingFormItemData *m_csVoiceCallPage;
    CpDivertItemData *m_DataItemAllVoiceCalls;
    CpDivertItemData *m_DataItemIfBusy;
    CpDivertItemData *m_DataItemIfNotAnswered;
    CpDivertItemData *m_DataItemIfOutOfReach;
    CpDivertItemData *m_DataItemIfNotAvailable;
    int m_activeNoteId;
    int m_activeProgressNoteId;

    // To prevent adding vmbx number to defaultlist
    bool m_divertToVoiceMailBox;

    /**
     Divert request queue
     */
    QQueue<CpDivertRequestQueueItem> m_divertRequestQueue;

    CpItemDataHelper &m_helper;
};

#endif // CPDIVERTPLUGINGROUP_H

// end of file
