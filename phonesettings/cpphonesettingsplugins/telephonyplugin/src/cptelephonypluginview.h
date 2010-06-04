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

#ifndef CPTELEPHONYPLUGINVIEW_H_
#define CPTELEPHONYPLUGINVIEW_H_

#include <QObject>
#include <hbdataform.h>
#include <cpbasesettingview.h>

class HbDataFormModelItem;
class CpSettingFormItemData;

/*!
    \class CpTelephonyPluginView
    \brief The class CpTelephonyPluginView. 
 */
class CpTelephonyPluginView : public CpBaseSettingView
{
    Q_OBJECT

public:
    CpTelephonyPluginView();
    ~CpTelephonyPluginView();

private: 
    
    CpItemDataHelper* initializeItemDataHelper();

private: 
 
    QList<CpSettingFormItemData*> groupItemFromPlugin(const QString& plugin);
    
private:   // data
 
    CpItemDataHelper *m_helper;
};

#endif /* CPTELEPHONYPLUGINVIEW_H_ */
