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

#include "ut_infowidgetsathandlerprivate.h"
#define private public
#include "infowidgetsathandlerprivate.h"
#include "infowidgetsathandler.h"
#include "infowidgetlogging.h"
#include "qtestmains60.h"

void SimulateLeaveL()
{
    User::Leave(KErrGeneral);
}

void FillData( HBufC * aText,
        RSatService::TSatIconQualifier & aIconQualifier,
        TUint8 & aRecordNumber)
{
    _LIT(KText,"Idle Text");
    TPtr p(aText->Des());
    p.Copy(KText);
    aIconQualifier=RSatService::ESatIconNotSelfExplanatory;
    aRecordNumber=1;
}

void FillDataNoIcon( HBufC * aText,
        RSatService::TSatIconQualifier & aIconQualifier,
        TUint8 & aRecordNumber)
{
    Q_UNUSED(aIconQualifier);
    Q_UNUSED(aRecordNumber);
    _LIT(KText,"Idle Text");
    TPtr p(aText->Des());
    p.Copy(KText);
}

void FillDataSelfExplIcon( HBufC * aText,
        RSatService::TSatIconQualifier & aIconQualifier,
        TUint8 & aRecordNumber)
{
    Q_UNUSED(aText);
    aIconQualifier=RSatService::ESatIconSelfExplanatory;
    aRecordNumber=1;
}

/*!
  UT_InfoWidgetSatHandlerPrivate ::UT_InfoWidgetSatHandlerPrivate 
 */
UT_InfoWidgetSatHandlerPrivate::UT_InfoWidgetSatHandlerPrivate () //:
{
    DPRINT << "IN";
    
    m_satHandler = 0;
    m_iwSatHandler = 0;
    
    DPRINT << "OUT";
}

/*!
  UT_InfoWidgetSatHandlerPrivate ::~UT_InfoWidgetSatHandlerPrivate 
 */
UT_InfoWidgetSatHandlerPrivate::~UT_InfoWidgetSatHandlerPrivate ()
{
    DPRINT << "IN";
    
    delete m_satHandler;
    delete m_iwSatHandler;
    
    DPRINT << "OUT";
}

/*!
  UT_InfoWidgetSatHandlerPrivate ::init
 */
void UT_InfoWidgetSatHandlerPrivate::init()
{
    DPRINT << "IN";
    
    initialize();
    m_iwSatHandler = new InfoWidgetSatHandler;
    m_satHandler =  new InfoWidgetSatHandlerPrivate( m_iwSatHandler, m_satService);
    
    QVERIFY(verify());

    DPRINT << "OUT";
}

/*!
  UT_InfoWidgetSatHandlerPrivate ::cleanup
 */
void UT_InfoWidgetSatHandlerPrivate::cleanup()
{
    DPRINT << "IN";
    
    reset();
    delete m_satHandler;
    delete m_iwSatHandler;

    DPRINT << "OUT";
}

/*!
  UT_InfoWidgetSatHandlerPrivate::t_connect
 */
void UT_InfoWidgetSatHandlerPrivate::t_connect()
{
    DPRINT << "IN";
    
    expect("RSatSession::ConnectL");
    expect("RSatService::OpenL");
    m_satHandler->connect();
     
    expect("RSatSession::ConnectL")
        .willOnce(invokeWithoutArguments(SimulateLeaveL));
    m_satHandler->connect();
    
    expect("RSatService::OpenL")
        .willOnce(invokeWithoutArguments(SimulateLeaveL));
    m_satHandler->connect();

    QVERIFY(verify());

    DPRINT << "OUT";
}

/*!
  UT_InfoWidgetSatHandlerPrivate::t_disconnect
 */
void UT_InfoWidgetSatHandlerPrivate::t_disconnect()
{
    DPRINT << "IN";
    
    m_satHandler->disconnect();
    QVERIFY(verify());

    DPRINT << "OUT";
}

/*!
  UT_InfoWidgetSatHandlerPrivate::t_startObserving
 */
void UT_InfoWidgetSatHandlerPrivate::t_startObserving()
{
    DPRINT << "IN";
    
    int i = KErrGeneral;
    expect("RSatService::NotifySetupIdleModeTextChange").returns(i);
    expect("RSatService::GetSetupIdleModeTextL")
        .willOnce(invoke(FillData));
    m_satHandler->startObserving();
    
    expect("RSatService::NotifySetupIdleModeTextChange").returns(i);
    expect("RSatService::GetSetupIdleModeTextL")
        .willOnce(invoke(FillDataNoIcon));
    m_satHandler->startObserving();
    
    expect("RSatService::NotifySetupIdleModeTextChange").returns(i);
    expect("RSatService::GetSetupIdleModeTextL")
        .willOnce(invoke(FillDataSelfExplIcon));
    m_satHandler->startObserving();
    
    expect("RSatService::GetSetupIdleModeTextL")
        .willOnce(invokeWithoutArguments(SimulateLeaveL));
    m_satHandler->startObserving();
    
    QVERIFY(verify());

    DPRINT << "OUT";
}

/*!
  UT_InfoWidgetSatHandlerPrivate::t_runL
 */
void UT_InfoWidgetSatHandlerPrivate::t_runL()
{
    DPRINT << "IN";
    
    int i = 1;
    expect("RSatService::GetSetupIdleModeTextL")
          .willOnce(invoke(FillData));
    expect("RSatService::NotifySetupIdleModeTextChange").returns(i);
    m_satHandler->RunL();
    
    m_satHandler->iStatus = KErrGeneral;
    expect("RSatService::NotifySetupIdleModeTextChange").returns(i);
    m_satHandler->RunL();
        
    QVERIFY(verify());

    DPRINT << "OUT";
}

/*!
  UT_InfoWidgetSatHandlerPrivate::t_doCancel
 */
void UT_InfoWidgetSatHandlerPrivate::t_doCancel()
{    
    DPRINT << "IN";

    m_satHandler->DoCancel();
    QVERIFY(verify());

    DPRINT << "OUT";
}

QTEST_MAIN_S60(UT_InfoWidgetSatHandlerPrivate )
