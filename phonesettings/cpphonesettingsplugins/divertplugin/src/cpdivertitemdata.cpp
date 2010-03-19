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
#include "cpdivertitemdata.h"
#include "cpdivertselectionitem.h"


CpDivertItemData::CpDivertItemData(
                PsCallDivertingCondition condition,
                PsServiceGroup serviceGroup,
                const QString &label,
                const QString &queryLabel,
                bool needTimeoutInfo,
                CpItemDataHelper &helper,
                const HbDataFormModelItem *parent) :
                CpSettingFormItemData(
                        static_cast<HbDataFormModelItem::DataItemType>(
                                CpDivertSelectionItem::CpDivertSelectionItemId),
                        label, parent ),
                m_serviceGroup(serviceGroup),
                m_condition(condition),
                m_queryLabel(queryLabel),
                m_needTimeoutInfo(needTimeoutInfo),
                m_helper(helper)
{
    m_helper.addConnection(
        this, SIGNAL(clicked()),
        this, SLOT(thisItemClicked()));
}

CpDivertItemData::~CpDivertItemData()
{

}

PsServiceGroup CpDivertItemData::service()
{
    return m_serviceGroup;
}

PsCallDivertingCondition CpDivertItemData::condition()
{
    return m_condition;
}

QString CpDivertItemData::queryLabel()
{
    return m_queryLabel;
}

bool CpDivertItemData::needTimeoutInfo()
{
    return m_needTimeoutInfo;
}

void CpDivertItemData::thisItemClicked()
{
    emit itemClicked(*this);
}

// end of file
