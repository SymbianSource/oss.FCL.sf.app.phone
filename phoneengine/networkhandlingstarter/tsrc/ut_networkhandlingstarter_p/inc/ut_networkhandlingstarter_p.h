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

#ifndef UT_NETWORKHANDLINGSTARTER_P_H
#define UT_NETWORKHANDLINGSTARTER_P_H

#include <QtTest/QtTest>
#include <mockservice.h>
#include <hbmainwindow.h>

class NetworkHandlingStarterPrivate;

class ut_networkhandlingstarter_p : public QObject, public MockService
{
    Q_OBJECT

public:
    
    ut_networkhandlingstarter_p();
    ~ut_networkhandlingstarter_p();
    
private slots:

    void init();
    void cleanup();
    
    void t_LaunchNetworksettingsPlugin();
    void t_ViewDone();
    void t_ShowNote();
    void t_RemoveNote();

    
private:
    
    NetworkHandlingStarterPrivate *m_NetworkHandlingStarterPrivate;
    HbMainWindow m_HbMainWindow;
    QList<HbMainWindow *> m_MainWindowList;

};

#endif // UT_NETWORKHANDLINGSTARTER_P_H
