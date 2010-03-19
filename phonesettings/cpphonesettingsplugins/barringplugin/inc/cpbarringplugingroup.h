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
#ifndef CPBARRINGPLUGINGROUP_H
#define CPBARRINGPLUGINGROUP_H

#include <cpsettingformitemdata.h>
#include "cptelephonyutilsdefs.h"

using namespace CpTelephonyUtils;

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
    
private: 
    
    CpItemDataHelper &m_helper;
};

#endif // CPBARRINGPLUGINGROUP_H
