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

#ifndef UT_SSSETTINGWRAPPER_H
#define UT_SSSETTINGWRAPPER_H

#include <QtTest/QtTest>
#include <mockservice.h>

class CpSettingsWrapper;

class UT_CpPluginCommon : public QObject, MockService
{
    Q_OBJECT

public:    
    UT_CpPluginCommon();
    ~UT_CpPluginCommon();
    
private slots:

    void init();
    void cleanup();
    
    void t_memleak();
    
    void t_showCallDuration();
    void t_setShowCallDuration();
    void t_readSoftRejectText();
    void t_writeSoftRejectText();
    void t_numberGroupingSupported();
    
    void t_loadCpPlugin();
    void t_unloadCpPlugin();
    
    void t_voipSupported();
    void t_videoSupported();
    void t_convertEtelMobileServiceCode();
    void t_errorCodeTextMapping();

private:
    CpSettingsWrapper *mWrapper;

};

#endif
