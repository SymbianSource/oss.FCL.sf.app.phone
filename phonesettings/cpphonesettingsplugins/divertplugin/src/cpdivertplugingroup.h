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
class QEventLoop; 
class HbLineEdit; 
class HbDataFormModelItem;

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
    
    // Activate divert phases
    enum ActivateDivertPhases
    {
        NonePhase,
        PopUpVoiceNumberListQueryPhase,
        PopUpTimerQueryPhase
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
    
    void voiceNumberListQueryClosed(HbAction* action);
    void popUpNumberEditorClosed(HbAction* action);
    void popUpTimerQueryClosed(HbAction* action);
    
private:     
    
    /**
     Creates group items.
     */
    void createVoiceCallItems(CpSettingFormItemData *group);

    /**
     Creates group items.
     */
    void createVideoCallItems(CpSettingFormItemData *parent);
    
    /**
     Creates single item.
     */
    CpDivertItemData *createDivertItem(
            PsCallDivertingCondition condition,
            PsServiceGroup serviceGroup,
            const QString &label,
            const QString &queryLabel,
            bool needTimeoutInfo,
            CpSettingFormItemData *parent = 0);
    
    /**
     Show to user divert number query list.
     */
    void popUpVoiceNumberListQuery(
            const QString& heading, PsServiceGroup serviceGroup);
    
    /**
     Show to user number query.
     */
    void popUpNumberEditor(
            const QString& heading);
    
    
    /**
     Send comand to psetwrappercalldiverting.
     */
    void setCallDiverting(PSCallDivertingCommand& command);
    
    /**
     Show to user divert time out query list.
     */
    void popUpTimerQuery();
    
    /**
     Desides which bsc parameters to use.
     */
    int bscParam(PsServiceGroup serviceGroup);
    
    /**
     Adds divert data item for wanted parent.
     */
    void addDivertDataItem(
            CpDivertItemData* item, CpSettingFormItemData *parent);

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
            PsServiceGroup serviceGroup,
            PsCallDivertingCondition condition, 
            PsCallDivertingStatus status,
            const QString& number, 
            int timeout);
    
    /**
     Reverts divertItemData data to the last known valid values.
     */
    void revertItemData(
            PsServiceGroup serviceGroup, PsCallDivertingCondition condition);

    /**
     Helper function which fetches model item according service and condition.
     */
    CpSettingFormItemData* item(
            PsService service, PsCallDivertingCondition condition);
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
    void updateDependentDivertOptions(bool fetchFromNetwork = false);
    void activateDependentDivertOption(CpDivertItemData* item, bool fetchFromNetwork);
    void deactivateDependentDivertOption(CpDivertItemData* item) const;
    
    /**
     Synchronizes content and status of not available divert option based on
     statuses of CFB, CFNry and CFNrc.
     */
    void updateNotAvailableDivertOption();
    
    void nextPhaseForActivateDivert(bool ok);
    
    bool isDivertSettingsItem(HbDataFormModelItem* modelItem);
     
private:   // data

    // Setting wrappers 
    PSetWrapper *m_pSetWrapper;
    SsSettingsWrapper *m_ssSettingsWrapper;
    
    // Not own
    PSetCallDivertingWrapper* m_callDivertingWrapper;

    // Not owned objects
    CpDivertItemData *m_DataItemVoiceAllCalls;
    CpDivertItemData *m_DataItemVoiceIfBusy;
    CpDivertItemData *m_DataItemVoiceIfNotAnswered;
    CpDivertItemData *m_DataItemVoiceIfOutOfReach;
    CpDivertItemData *m_DataItemVoiceIfNotAvailable;
    
    CpDivertItemData *m_DataItemVideoAllCalls;
    CpDivertItemData *m_DataItemVideoIfBusy;
    CpDivertItemData *m_DataItemVideoIfNotAnswered;
    CpDivertItemData *m_DataItemVideoIfOutOfReach;
    CpDivertItemData *m_DataItemVideoIfNotAvailable;

    int m_activeNoteId;
    int m_activeProgressNoteId;

    /**
     Divert request queue
     */
    QQueue<CpDivertRequestQueueItem> m_divertRequestQueue;

    CpItemDataHelper &m_helper;
    
    ActivateDivertPhases m_activateDivertPhase;
    PSCallDivertingCommand m_divertCommand;
    int m_timerId;
    HbDialog *m_dialog;

};

#endif // CPDIVERTPLUGINGROUP_H


