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
  UT_CpPluginCommon::t_memleak
 */
void UT_CpPluginCommon::t_showCallDuration()
{
    expect("XQSettingsManager::readItemValue").
        returns(QVariant(23324));
    
    mWrapper->showCallDuration();
    QVERIFY(verify());
}

/*!
  UT_CpPluginCommon::t_memleak
 */
void UT_CpPluginCommon::t_setShowCallDuration()
{
    mWrapper->setShowCallDuration(true);
}

/*!
  UT_CpPluginCommon::t_memleak
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
    QVERIFY(verify());
}

/*!
  UT_CpPluginCommon::t_memleak
 */
void UT_CpPluginCommon::t_writeSoftRejectText()
{
    QString text="";
    bool userDefined=true;
    mWrapper->writeSoftRejectText(text, userDefined);
}

/*!
  UT_CpPluginCommon::t_memleak
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
  UT_CpPluginCommon::t_memleak
 */
void UT_CpPluginCommon::t_convertEtelMobileServiceCode()
{
    //
    QCOMPARE(Tools::convertEtelMobileServiceCode(
            RMobilePhone::EAllServices), AllTeleAndBearer);
    QCOMPARE(Tools::convertEtelMobileServiceCode(
            RMobilePhone::EAllTele), AllTele);

    QCOMPARE(Tools::convertEtelMobileServiceCode(
            RMobilePhone::EVoiceService), Telephony);
    QCOMPARE(Tools::convertEtelMobileServiceCode(
            RMobilePhone::ETelephony), Telephony);
    
    QCOMPARE(Tools::convertEtelMobileServiceCode(
            RMobilePhone::EAllDataTele), AllDataTele);
    QCOMPARE(Tools::convertEtelMobileServiceCode(
            RMobilePhone::EFaxService), Fax);
    QCOMPARE(Tools::convertEtelMobileServiceCode(
            RMobilePhone::EShortMessageService), Sms);
    QCOMPARE(Tools::convertEtelMobileServiceCode(
            RMobilePhone::EAllDataExSms), AllDataExSms);
    QCOMPARE(Tools::convertEtelMobileServiceCode(
            RMobilePhone::EAllTeleExcSms), AllTeleExcSms);
    QCOMPARE(Tools::convertEtelMobileServiceCode(
            RMobilePhone::EAllPlmnTele), AllPlmnTele);
    QCOMPARE(Tools::convertEtelMobileServiceCode(
            RMobilePhone::EPlmnTele1), PlmnTele1);
    QCOMPARE(Tools::convertEtelMobileServiceCode(
            RMobilePhone::EPlmnTele2), PlmnTele2);
    QCOMPARE(Tools::convertEtelMobileServiceCode(
            RMobilePhone::EPlmnTele3), PlmnTele3);
    QCOMPARE(Tools::convertEtelMobileServiceCode(
            RMobilePhone::EPlmnTele4), PlmnTele4);
    QCOMPARE(Tools::convertEtelMobileServiceCode(
            RMobilePhone::EPlmnTele5), PlmnTele5);
    QCOMPARE(Tools::convertEtelMobileServiceCode(
            RMobilePhone::EPlmnTele6), PlmnTele6);
    QCOMPARE(Tools::convertEtelMobileServiceCode(
            RMobilePhone::EPlmnTele7), PlmnTele7);
    QCOMPARE(Tools::convertEtelMobileServiceCode(
            RMobilePhone::EPlmnTele8), PlmnTele8);
    QCOMPARE(Tools::convertEtelMobileServiceCode(
            RMobilePhone::EPlmnTele9), PlmnTele9);
    QCOMPARE(Tools::convertEtelMobileServiceCode(
            RMobilePhone::EPlmnTeleA), PlmnTeleA);
    QCOMPARE(Tools::convertEtelMobileServiceCode(
            RMobilePhone::EPlmnTeleB), PlmnTeleB);
    QCOMPARE(Tools::convertEtelMobileServiceCode(
            RMobilePhone::EPlmnTeleC), PlmnTeleC);
    QCOMPARE(Tools::convertEtelMobileServiceCode(
            RMobilePhone::EPlmnTeleD), PlmnTeleD);
    QCOMPARE(Tools::convertEtelMobileServiceCode(
            RMobilePhone::EPlmnTeleE), PlmnTeleE);
    QCOMPARE(Tools::convertEtelMobileServiceCode(
            RMobilePhone::EPlmnTeleF), PlmnTeleF);
    QCOMPARE(Tools::convertEtelMobileServiceCode(
            RMobilePhone::EAllBearer), AllBearer);
    QCOMPARE(Tools::convertEtelMobileServiceCode(
            RMobilePhone::EAllAsync), AllAsync);
    QCOMPARE(Tools::convertEtelMobileServiceCode(
            RMobilePhone::EAllSync), AllSync);
    
    QCOMPARE(Tools::convertEtelMobileServiceCode(
            RMobilePhone::ECircuitDataService), SyncData);
    QCOMPARE(Tools::convertEtelMobileServiceCode(
            RMobilePhone::ESyncData), SyncData);
    
    QCOMPARE(Tools::convertEtelMobileServiceCode(
            RMobilePhone::EPacketDataService), AsyncData);
    QCOMPARE(Tools::convertEtelMobileServiceCode(
            RMobilePhone::EAsyncData), AsyncData);
    
    QCOMPARE(Tools::convertEtelMobileServiceCode(
            RMobilePhone::EPacketData), PacketData);
    QCOMPARE(Tools::convertEtelMobileServiceCode(
            RMobilePhone::EPadAccess), PadAccess);
    QCOMPARE(Tools::convertEtelMobileServiceCode(
            RMobilePhone::EAllPlmnBearer), AllPlmnBearer);
    QCOMPARE(Tools::convertEtelMobileServiceCode(
            RMobilePhone::EPlmnBearerServ1), PlmnBearerServ1);
    QCOMPARE(Tools::convertEtelMobileServiceCode(
            RMobilePhone::EPlmnBearerServ2), PlmnBearerServ2);
    QCOMPARE(Tools::convertEtelMobileServiceCode(
            RMobilePhone::EPlmnBearerServ3), PlmnBearerServ3);
    QCOMPARE(Tools::convertEtelMobileServiceCode(
            RMobilePhone::EPlmnBearerServ4), PlmnBearerServ4);
    QCOMPARE(Tools::convertEtelMobileServiceCode(
            RMobilePhone::EPlmnBearerServ5), PlmnBearerServ5);
    QCOMPARE(Tools::convertEtelMobileServiceCode(
            RMobilePhone::EPlmnBearerServ6), PlmnBearerServ6);
    QCOMPARE(Tools::convertEtelMobileServiceCode(
            RMobilePhone::EPlmnBearerServ7), PlmnBearerServ7);
    QCOMPARE(Tools::convertEtelMobileServiceCode(
            RMobilePhone::EPlmnBearerServ8), PlmnBearerServ8);
    QCOMPARE(Tools::convertEtelMobileServiceCode(
            RMobilePhone::EPlmnBearerServ9), PlmnBearerServ9);
    QCOMPARE(Tools::convertEtelMobileServiceCode(
            RMobilePhone::EPlmnBearerServA), PlmnBearerServA);
    QCOMPARE(Tools::convertEtelMobileServiceCode(
            RMobilePhone::EPlmnBearerServB), PlmnBearerServB);
    QCOMPARE(Tools::convertEtelMobileServiceCode(
            RMobilePhone::EPlmnBearerServC), PlmnBearerServC);
    QCOMPARE(Tools::convertEtelMobileServiceCode(
            RMobilePhone::EPlmnBearerServD), PlmnBearerServD);
    QCOMPARE(Tools::convertEtelMobileServiceCode(
            RMobilePhone::EPlmnBearerServE), PlmnBearerServE);
    QCOMPARE(Tools::convertEtelMobileServiceCode(
            RMobilePhone::EPlmnBearerServF), PlmnBearerServF);
    QCOMPARE(Tools::convertEtelMobileServiceCode(
            RMobilePhone::EAuxVoiceService), AltTele);
    QCOMPARE(Tools::convertEtelMobileServiceCode(
            RMobilePhone::EAltTele), AltTele);
    QCOMPARE(Tools::convertEtelMobileServiceCode(
            RMobilePhone::EServiceUnspecified), Unknown);
    // ??
    QCOMPARE(Tools::convertEtelMobileServiceCode(
            RMobilePhone::EVoiceGroupCall), Unknown);
    QCOMPARE(Tools::convertEtelMobileServiceCode(
            RMobilePhone::EVoiceBroadcast), Unknown);
    QCOMPARE(Tools::convertEtelMobileServiceCode(
            RMobilePhone::EAllGprsBearer), Unknown);

}

/*!
  UT_CpPluginCommon::t_memleak
 */
void UT_CpPluginCommon::t_errorCodeTextMapping()
{
    QString text = "";
    Tools::errorCodeTextMapping(KErrGsmSSSubscriptionViolation, text);
    QCOMPARE( text, QString(
            "Barring operation not successful. Contact your service provider"));
    Tools::errorCodeTextMapping(KErrGsmSSUnknownSubscriber, text);
    QCOMPARE( text, QString(
            "Not allowed"));
    Tools::errorCodeTextMapping(KErrGsmSSAbsentSubscriber, text);
    QCOMPARE( text, QString(
            "Not allowed"));
    Tools::errorCodeTextMapping(KErrGsmSSIllegalOperation, text);
    QCOMPARE( text, QString(
            "Not allowed"));
    Tools::errorCodeTextMapping(KErrGsmSSIllegalSubscriber, text);
    QCOMPARE( text, QString(
            "Not allowed"));
    Tools::errorCodeTextMapping(KErrGsmSSIllegalEquipment, text);
    QCOMPARE( text, QString(
            "Not allowed"));
    Tools::errorCodeTextMapping(KErrGsmSSCallBarred, text);
    QCOMPARE( text, QString(
            "Not allowed"));
    Tools::errorCodeTextMapping(KErrGsmSSDataMissing, text);
    QCOMPARE( text, QString(
            "Not allowed"));
    Tools::errorCodeTextMapping(KErrGsmSSIncompatibility, text);
    QCOMPARE( text, QString(
            "Services in conflict"));
    Tools::errorCodeTextMapping(KErrGsmSSSystemFailure, text);
    QCOMPARE( text, QString(
            "Result unknown"));
    Tools::errorCodeTextMapping(KErrGsmSSUnexpectedDataValue, text);
    QCOMPARE( text, QString(
            "Request rejected"));
    Tools::errorCodeTextMapping(KErrGsmSSResourcesUnavailable, text);
    QCOMPARE( text, QString(
            "Request rejected"));
    Tools::errorCodeTextMapping(KErrGsmSSNegativePasswordCheck, text);
    QCOMPARE( text, QString(
            "Password error"));
    Tools::errorCodeTextMapping(KErrGsmSSPasswordRegistrationFailure, text);
    QCOMPARE( text, QString(
            "Password error"));
    Tools::errorCodeTextMapping(KErrGsmSSPasswordAttemptsViolation, text);
    QCOMPARE( text, QString(
            "Password blocked"));
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
    QCOMPARE( text, QString("Invalid phone number"));
    
    expect("XQSysInfo::isSupported").returns(false);
    Tools::errorCodeTextMapping(KErrGsmOfflineOpNotAllowed, text);
    QCOMPARE( text, QString(
            "Not done"));
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
            "Not done"));
    
    QVERIFY( verify() );
        
}


QTEST_MAIN_S60(UT_CpPluginCommon)
