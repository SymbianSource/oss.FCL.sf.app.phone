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
 
#include <rsatservice.h>
#include <rsatsession.h>
#include <QString>

class InfoWidgetSatHandler;

class InfoWidgetSatHandlerPrivate: public CActive
{
public:  
    
    InfoWidgetSatHandlerPrivate(InfoWidgetSatHandler *iwSatHandler, RSatService& iSatService);
    ~InfoWidgetSatHandlerPrivate();
 
    void startObserving();
    
    int connect();
    int disconnect();

private: 
 
    void activate();
    void getIdleModeData();
    
    void RunL();
    void DoCancel();
       
private: // Data
   
    InfoWidgetSatHandler *q_ptr;
    RSatService& m_satService;
    RSatSession m_satSession;
    RSatService::TSATIdleResult m_idleResult;
    
    QString m_idleTxt;
};