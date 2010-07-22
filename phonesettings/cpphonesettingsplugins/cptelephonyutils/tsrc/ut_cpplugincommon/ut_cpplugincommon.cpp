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

#include "ut_cpplugincommon.h"
#include "qtestmains60.h"
#include <gsmerror.h>
#include <exterror.h>
#include <etelmm.h>
#include <btsapdomainpskeys.h>
#define private public
#include "cpplugincommon.h"

/*!
  UT_CpPluginCommon::UT_CpPluginCommon
 */
UT_CpPluginCommon::UT_CpPluginCommon() 
    : mWrapper(NULL)
{
}

/*!
  UT_CpPluginCommon::~UT_CpPluginCommon
 */
UT_CpPluginCommon::~UT_CpPluginCommon()
{
    delete mWrapper;
}

/*!
  UT_CpPluginCommon::init
 */
void UT_CpPluginCommon::init()
{
    initialize();

    mWrapper = new CpSettingsWrapper();
}

/*!
  UT_CpPluginCommon::cleanup
 */
void UT_CpPluginCommon::cleanup()
{
    reset();
    
    delete mWrapper;
    mWrapper = NULL;
}

/*!
  UT_CpPluginCommon::t_memleak
 */
void UT_CpPluginCommon::t_memleak()
{
    
}

/*!
  UT_CpPluginCommon::t_showCallDuration
 */
void UT_CpPluginCommon::t_showCallDuration()
{
    expect("XQSettingsManager::readItemValue").
        returns(QVariant(23324));
    
    mWrapper->showCallDuration();
    QVERIFY(verify());
}

/*!
  UT_CpPluginCommon::t_setShowCallDuration
 */
void UT_CpPluginCommon::t_setShowCallDuration()
{
    mWrapper->setShowCallDuration(true);
    mWrapper->setShowCallDuration(false);
    QVERIFY(verify());
}

/*!
  UT_CpPluginCommon::t_readSoftRejectText
 */
void UT_CpPluginCommon::t_readSoftRejectText()
{
    QString text="";
    bool userDefined=true;
    expect("XQSettingsManager::readItemValue").
            returns(QVariant(true));
    
    expect("XQSettingsManager::readItemValue").
            returns(QVariant("text"));
    
    mWrapper->readSoftRejectText(text, userDefined);
    
    userDefined=false;
    expect("XQSettingsManager::readItemValue").
                    returns(QVariant(false));
    expect("XQSettingsManager::readItemValue").
            returns(QVariant("text"));
    mWrapper->readSoftRejectText(text, userDefined);
    
    QVERIFY(verify());
}

/*!
  UT_CpPluginCommon::t_writeSoftRejectText
 */
void UT_CpPluginCommon::t_writeSoftRejectText()
{
    QString text="";
    bool userDefined=true;
    mWrapper->writeSoftRejectText(text, userDefined);
    userDefined=false;
    mWrapper->writeSoftRejectText(text, userDefined);
    QVERIFY(verify());
}

/*!
  UT_CpPluginCommon::t_numberGroupingSupported
 */
void UT_CpPluginCommon::t_numberGroupingSupported()
{
    expect("XQSettingsManager::readItemValue").
            returns(QVariant(true));
    
    mWrapper->numberGroupingSupported();
    QVERIFY(verify());
}

/*!
  UT_CpPluginCommon::t_voipSupported
 */

void UT_CpPluginCommon::t_voipSupported()
{
    expect("XQSysInfo::isSupported").with(
            KFeatureIdCommonVoip).returns(false);
    QCOMPARE( Tools::voipSupported(), false );
    expect("XQSysInfo::isSupported").with(
            KFeatureIdCommonVoip).returns(true);
    QCOMPARE( Tools::voipSupported(), true );
    
    QVERIFY( verify() );
}

/*!
  UT_CpPluginCommon::t_videoSupported
 */
void UT_CpPluginCommon::t_videoSupported()
{
    expect("XQSysInfo::isSupported").with(
            KFeatureIdCsVideoTelephony).returns(true);
    QCOMPARE( Tools::videoSupported(), true );
    expect("XQSysInfo::isSupported").with(
            KFeatureIdCsVideoTelephony).returns(false);
    QCOMPARE( Tools::videoSupported(), false );
    
    QVERIFY( verify() );
}

/*!
  UT_CpPluginCommon::t_errorCodeTextMapping
 */
void UT_CpPluginCommon::t_errorCodeTextMapping()
{
    QString text = "";
    Tools::errorCodeTextMapping(KErrGsmSSSubscriptionViolation, text);
    QCOMPARE( text, QString(
            "txt_phone_info_barring_operation_not_successful"));
    Tools::errorCodeTextMapping(KErrGsmSSUnknownSubscriber, text);
    QCOMPARE( text, QString(
            "txt_phone_info_not_allowed"));
    Tools::errorCodeTextMapping(KErrGsmSSAbsentSubscriber, text);
    QCOMPARE( text, QString(
            "txt_phone_info_not_allowed"));
    Tools::errorCodeTextMapping(KErrGsmSSIllegalOperation, text);
    QCOMPARE( text, QString(
            "txt_phone_info_not_allowed"));
    Tools::errorCodeTextMapping(KErrGsmSSIllegalSubscriber, text);
    QCOMPARE( text, QString(
            "txt_phone_info_not_allowed"));
    Tools::errorCodeTextMapping(KErrGsmSSIllegalEquipment, text);
    QCOMPARE( text, QString(
            "txt_phone_info_not_allowed"));
    Tools::errorCodeTextMapping(KErrGsmSSCallBarred, text);
    QCOMPARE( text, QString(
            "txt_phone_info_not_allowed"));
    Tools::errorCodeTextMapping(KErrGsmSSDataMissing, text);
    QCOMPARE( text, QString(
            "txt_phone_info_not_allowed"));
    Tools::errorCodeTextMapping(KErrGsmSSIncompatibility, text);
    QCOMPARE( text, QString(
            "txt_phone_info_conflict_error"));
    Tools::errorCodeTextMapping(KErrGsmSSSystemFailure, text);
    QCOMPARE( text, QString(
            "txt_phone_info_result_unknown"));
    Tools::errorCodeTextMapping(KErrGsmSSUnexpectedDataValue, text);
    QCOMPARE( text, QString(
            "txt_phone_info_request_rejected"));
    Tools::errorCodeTextMapping(KErrGsmSSResourcesUnavailable, text);
    QCOMPARE( text, QString(
            "txt_phone_info_request_rejected"));
    Tools::errorCodeTextMapping(KErrGsmSSNegativePasswordCheck, text);
    QCOMPARE( text, QString(
            "Password error"));
    Tools::errorCodeTextMapping(KErrGsmSSPasswordRegistrationFailure, text);
    QCOMPARE( text, QString(
            "Password error"));
    Tools::errorCodeTextMapping(KErrGsmSSPasswordAttemptsViolation, text);
    QCOMPARE( text, QString(
            "txt_phone_info_barring_password_blocked"));
    Tools::errorCodeTextMapping(KErrGsmSMSNoNetworkService, text);
    QCOMPARE( text, QString(
            "No network coverage"));
    Tools::errorCodeTextMapping(KErrGsmNoService, text);
    QCOMPARE( text, QString(
            "No network coverage"));
    Tools::errorCodeTextMapping(KErrSsActivationDataLost, text);
    QCOMPARE( text, QString(
            "Check network services"));
    Tools::errorCodeTextMapping(KErrGsmSSUnknownAlphabet, text);
    QCOMPARE( text, QString("txt_phone_info_invalid_phone_number"));
    
    expect("XQSysInfo::isSupported").returns(false);
    Tools::errorCodeTextMapping(KErrGsmOfflineOpNotAllowed, text);
    QCOMPARE( text, QString(
            "txt_phone_info_request_not_completed"));
    QVERIFY( verify() );
    
    expect("XQSysInfo::isSupported").returns(true);
    expect("XQSettingsManager::readItemValue").returns(QVariant(EBTSapConnected));
    Tools::errorCodeTextMapping(KErrGsmOfflineOpNotAllowed, text);
    QCOMPARE( text, QString(
            "Operation not possible in SIM access profile mode"));
    QVERIFY( verify() );
    
    expect("XQSysInfo::isSupported").returns(true);
    expect("XQSettingsManager::readItemValue").returns(QVariant(EBTSapNotConnected));
    Tools::errorCodeTextMapping(KErrGsmOfflineOpNotAllowed, text);
    QCOMPARE( text, QString(
            "Operation not possible in Off-line mode"));
    
    Tools::errorCodeTextMapping(-1, text);
    QCOMPARE( text, QString(
            "txt_phone_info_request_not_completed"));
    
    QVERIFY( verify() );
        
}

/*!
  UT_CpPluginCommon::t_readVtVideoSending
 */
void UT_CpPluginCommon::t_readVtVideoSending()
{
    expect("XQSettingsManager::readItemValue").
            returns(QVariant(1));
    mWrapper->readVtVideoSending();
    QVERIFY( verify() );
}

/*!
  UT_CpPluginCommon::t_writeVtVideoSending
 */
void UT_CpPluginCommon::t_writeVtVideoSending()
{
    expect("XQSettingsManager::writeItemValue");//.
            //times(2);
    int i=0;
    mWrapper->writeVtVideoSending(i);
    QVERIFY( verify() );
}

/*!
  UT_CpPluginCommon::t_isFeatureCallWaitingDistiquishNotProvisionedEnabled
 */
void UT_CpPluginCommon::t_isFeatureCallWaitingDistiquishNotProvisionedEnabled()
{
    expect("XQSettingsManager::readItemValue").
                returns(QVariant(1));
    mWrapper->isFeatureCallWaitingDistiquishNotProvisionedEnabled();
    QVERIFY( verify() );
}

/*!
  UT_CpPluginCommon::t_isPhoneOffline
 */
void UT_CpPluginCommon::t_isPhoneOffline()
{
    expect("XQSysInfo::isSupported");
    mWrapper->isPhoneOffline();
    QVERIFY( verify() );
}

/*!
  UT_CpPluginCommon::t_isOngoingCall
 */
void UT_CpPluginCommon::t_isOngoingCall()
{
    expect("XQSettingsManager::readItemValue").
                    returns(QVariant(1));
    mWrapper->isOngoingCall();
    QVERIFY( verify() );
}

QTEST_MAIN_S60(UT_CpPluginCommon)
