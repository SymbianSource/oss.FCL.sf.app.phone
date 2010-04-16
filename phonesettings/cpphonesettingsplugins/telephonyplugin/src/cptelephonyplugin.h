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

#ifndef CPTELEPHONYPLUGIN_H
#define CPTELEPHONYPLUGIN_H

#include <qobject.h>
#include <cpplugininterface.h>

/*!
    \class CpTelephonyPlugin
    \brief The class CpTelephonyPlugin implements telephony plugin functionality. 
 */
class CpTelephonyPlugin
        : public QObject,
          public CpPluginInterface
{
    Q_OBJECT
    Q_INTERFACES(CpPluginInterface)
    
public:
 
    CpTelephonyPlugin();
    ~CpTelephonyPlugin();
    
    /*!
     From CpPluginInterface.
     @see CpPluginInterface.
     */
    QList<CpSettingFormItemData*> createSettingFormItemData(
            CpItemDataHelper &itemDataHelper) const;
};

#endif /* CPTELEPHONYPLUGIN_H */
