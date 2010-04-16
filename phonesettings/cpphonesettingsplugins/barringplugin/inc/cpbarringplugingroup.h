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
#ifndef CPBARRINGPLUGINGROUP_H
#define CPBARRINGPLUGINGROUP_H

#include <QQueue>
#include <cpsettingformitemdata.h>
#include <psetcallbarringwrapper.h>

class PSetWrapper;
class QModelIndex;
class CpPhoneNotes;

/*!
    \class CpBarringPluginGroup
    \brief The class CpBarringPluginGroup defines items shown on UI. 
 */
class CpBarringPluginGroup : public CpSettingFormItemData
{
    Q_OBJECT
    
public:

    explicit CpBarringPluginGroup(CpItemDataHelper &helper);
    ~CpBarringPluginGroup();

public slots:
    
    void itemShown(const QModelIndex& item);
    
private:
    
    void setupLocalization();
    void connectToWrapper();
    
    void createAllOutgoingBarringItem();
    void createOutgoingInternationalBarringItem();
    void createOutgoingInternationalExceptToHomeCountryBarringItem();
    void createAllIncomingBarringItem();
    void createIncomingWhenRoamingBarringItem();
    
    void processBarringStatusRequestQueue();
    void revertCheckStateForItem(CpSettingFormItemData *barringItem);

private slots:

    void barringStatusRequestCompleted(
        int result,
        const QList<unsigned char> & basicServiceGroupIds,
        PSetCallBarringWrapper::BarringStatus status);
    
    void enableBarringRequestCompleted(
        int result,
        PSetCallBarringWrapper::BarringType barringType,
        PSetCallBarringWrapper::BarringStatus barringStatus, 
        bool plural);

    void disableBarringRequestCompleted(
        int result,
        PSetCallBarringWrapper::BarringType barringType,
        PSetCallBarringWrapper::BarringStatus barringStatus, 
        bool plural);
    
    void changeBarringStateRequested(int checkState);
    
private: 
    
    CpItemDataHelper &m_helper;
    PSetWrapper *m_pSetWrapper;
    PSetCallBarringWrapper *m_barringWrapper;
    CpSettingFormItemData *m_allOutgoingBarringItem;
    CpSettingFormItemData *m_outgoingInternationalBarringItem;
    CpSettingFormItemData *m_outgoingInternationalExceptToHomeCountryBarringItem;
    CpSettingFormItemData *m_allIncomingBarringItem;
    CpSettingFormItemData *m_incomingWhenRoamingBarringItem;
    QQueue<CpSettingFormItemData *> m_barringRequestQueue;
    bool m_barringStatusRequestOngoing;
    int m_activeNoteId;
    CpPhoneNotes* m_phoneNotes;
};

#endif // CPBARRINGPLUGINGROUP_H
