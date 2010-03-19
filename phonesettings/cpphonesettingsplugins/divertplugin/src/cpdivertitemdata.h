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
#ifndef DIVERTINGITEMDATA_H_
#define DIVERTINGITEMDATA_H_

#include <cpsettingformitemdata.h>
#include <psetwrappertypes.h>
#include <cpitemdatahelper.h>

class CpDivertItemData : public CpSettingFormItemData
{
    Q_OBJECT
public:
    CpDivertItemData(
        PsCallDivertingCondition condition,
        PsServiceGroup serviceGroup,
        const QString &label,
        const QString &queryLabel,
        bool needTimeoutInfo,
        CpItemDataHelper &helper,
        const HbDataFormModelItem *parent = 0);
    
    ~CpDivertItemData();
    
    PsServiceGroup service();
    
    PsCallDivertingCondition condition();
    
    QString queryLabel();
    
    bool needTimeoutInfo();

public slots:
    void thisItemClicked();

signals:
    void itemClicked(CpDivertItemData &item);

private:
    PsServiceGroup m_serviceGroup;
    PsCallDivertingCondition m_condition;
    QString m_queryLabel;
    bool m_needTimeoutInfo;
    CpItemDataHelper &m_helper;
};

#endif /* DIVERTINGITEMDATA_H_ */
