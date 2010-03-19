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


#include <etelmm.h>
#include <xqsysinfo.h>
#include <xqsettingsmanager.h>
#include <xqsettingskey.h>
#include <e32base.h>
#include <etelmm.h>
#include <exterror.h>               
#include <gsmerror.h>
#include <settingsinternalcrkeys.h>
#include <LogsDomainCRKeys.h>
#include <BTSapDomainPSKeys.h>
#include <NumberGroupingCRKeys.h>
#include <QPluginLoader>
#include <cppluginplatinterface.h>
#include <hbglobal.h>
#include <CoreApplicationUIsSDKCRKeys.h>
#include "cptelephonyutilsdefs.h"
#include "cpplugincommon.h"
#include "cppluginlogging.h"

const int CenrepKeyValueOff = 0;
const int CenrepKeyValueOn = 1;

const int SoftRejectTextDefault = 0; 
const int SoftRejectTextUserDefined = 1; 

/*!
  Tools::loadCpPlugin
*/
CpPluginPlatInterface* Tools::loadCpPlugin(const QString& name)
{
    QPluginLoader loader("\\resource\\qt\\plugins\\controlpanel\\"+name+".qtplugin");
    return qobject_cast<CpPluginPlatInterface*> (loader.instance());
}

/*!
  Tools::unloadCpPlugin
*/
bool Tools::unloadCpPlugin(const QString& name)
{
    QPluginLoader loader("\\resource\\qt\\plugins\\controlpanel\\"+name+".qtplugin");
    return loader.unload();
}

/*!
  Tools::voipSupported
*/
bool Tools::voipSupported()
{
    return XQSysInfo::isSupported(KFeatureIdCommonVoip);
}

/*!
  Tools::videoSupported
*/
bool Tools::videoSupported()
{
    return XQSysInfo::isSupported(KFeatureIdCsVideoTelephony);
}

/*!
  Tools::convertEtelMobileServiceCode
*/
BasicServiceGroups Tools::convertEtelMobileServiceCode(int serviceCode)
{
    DPRINT;

    switch (serviceCode) {
        case RMobilePhone::EAllServices:
            return AllTeleAndBearer;
        case RMobilePhone::EAllTele:
            return AllTele;
        case RMobilePhone::EVoiceService:
        case RMobilePhone::ETelephony:
            return Telephony;
        case RMobilePhone::EAllDataTele:
            return AllDataTele;
        case RMobilePhone::EFaxService:
            return Fax;
        case RMobilePhone::EShortMessageService:
            return Sms;
        case RMobilePhone::EAllDataExSms:
            return AllDataExSms;
        case RMobilePhone::EAllTeleExcSms:
            return AllTeleExcSms;
        case RMobilePhone::EAllPlmnTele:
            return AllPlmnTele;
        case RMobilePhone::EPlmnTele1:
            return PlmnTele1;
        case RMobilePhone::EPlmnTele2:
            return PlmnTele2;
        case RMobilePhone::EPlmnTele3:
            return PlmnTele3;
        case RMobilePhone::EPlmnTele4:
            return PlmnTele4;
        case RMobilePhone::EPlmnTele5:
            return PlmnTele5;
        case RMobilePhone::EPlmnTele6:
            return PlmnTele6;
        case RMobilePhone::EPlmnTele7:
            return PlmnTele7;
        case RMobilePhone::EPlmnTele8:
            return PlmnTele8;
        case RMobilePhone::EPlmnTele9:
            return PlmnTele9;
        case RMobilePhone::EPlmnTeleA:
            return PlmnTeleA;
        case RMobilePhone::EPlmnTeleB:
            return PlmnTeleB;
        case RMobilePhone::EPlmnTeleC:
            return PlmnTeleC;
        case RMobilePhone::EPlmnTeleD:
            return PlmnTeleD;
        case RMobilePhone::EPlmnTeleE:
            return PlmnTeleE;
        case RMobilePhone::EPlmnTeleF:
            return PlmnTeleF;
        case RMobilePhone::EAllBearer:
            return AllBearer;
        case RMobilePhone::EAllAsync:
            return AllAsync;
        case RMobilePhone::EAllSync:
            return AllSync;
        case RMobilePhone::ECircuitDataService:
        case RMobilePhone::ESyncData:
            return SyncData;
        case RMobilePhone::EPacketDataService:
        case RMobilePhone::EAsyncData:
            return AsyncData;
        case RMobilePhone::EPacketData:
            return PacketData;
        case RMobilePhone::EPadAccess:
            return PadAccess;
        case RMobilePhone::EAllPlmnBearer:
            return AllPlmnBearer;
        case RMobilePhone::EPlmnBearerServ1:
            return PlmnBearerServ1;
        case RMobilePhone::EPlmnBearerServ2:
            return PlmnBearerServ2;
        case RMobilePhone::EPlmnBearerServ3:
            return PlmnBearerServ3;
        case RMobilePhone::EPlmnBearerServ4:
            return PlmnBearerServ4;
        case RMobilePhone::EPlmnBearerServ5:
            return PlmnBearerServ5;
        case RMobilePhone::EPlmnBearerServ6:
            return PlmnBearerServ6;
        case RMobilePhone::EPlmnBearerServ7:
            return PlmnBearerServ7;
        case RMobilePhone::EPlmnBearerServ8:
            return PlmnBearerServ8;
        case RMobilePhone::EPlmnBearerServ9:
            return PlmnBearerServ9;
        case RMobilePhone::EPlmnBearerServA:
            return PlmnBearerServA;
        case RMobilePhone::EPlmnBearerServB:
            return PlmnBearerServB;
        case RMobilePhone::EPlmnBearerServC:
            return PlmnBearerServC;
        case RMobilePhone::EPlmnBearerServD:
            return PlmnBearerServD;
        case RMobilePhone::EPlmnBearerServE:
            return PlmnBearerServE;
        case RMobilePhone::EPlmnBearerServF:
            return PlmnBearerServF;
        case RMobilePhone::EAuxVoiceService:
        case RMobilePhone::EAltTele:
            return AltTele;
        case RMobilePhone::EServiceUnspecified:
        default:
            qDebug() << "CpPhoneNotes::convertEtelMobileServiceCode, DEFAULT";
            return Unknown;
    }
}

/*!
  Tools::errorCodeTextMapping.
 */
bool Tools::errorCodeTextMapping(const int errorcode, QString &errorText)
{
    DPRINT;
    errorText.clear();
    bool errorTextFound(true);

    switch(errorcode){
        case KErrGsmSSSubscriptionViolation:
            errorText = hbTrId("Barring operation not successful. Contact your service provider");
            break;
        case KErrGsmSSUnknownSubscriber:
        case KErrGsmSSAbsentSubscriber:
        case KErrGsmSSIllegalOperation:
        case KErrGsmSSIllegalSubscriber:
        case KErrGsmSSIllegalEquipment:
        case KErrGsmSSCallBarred:
        case KErrGsmSSDataMissing:
            errorText = hbTrId("Not allowed");
            break;
        case KErrGsmSSIncompatibility:
            errorText = hbTrId("Services in conflict");
            break;
        case KErrGsmSSSystemFailure:
            errorText = hbTrId("Result unknown");
            break;
        case KErrGsmSSUnexpectedDataValue:
        case KErrGsmSSResourcesUnavailable:
            errorText = hbTrId("Request rejected");
            break;
        case KErrGsmSSNegativePasswordCheck:
        case KErrGsmSSPasswordRegistrationFailure:
            errorText = hbTrId("Password error");
            break;
        case KErrGsmSSPasswordAttemptsViolation:
            errorText = hbTrId("Password blocked");
            break;
        case KErrGsmSMSNoNetworkService:
        case KErrGsmNoService:
            errorText = hbTrId("No network coverage");
            break;
        case KErrSsActivationDataLost:
            errorText = hbTrId("Check network services");
            break;
        case KErrGsmOfflineOpNotAllowed:
            if(XQSysInfo::isSupported(KFeatureIdOfflineMode)){
                XQSettingsManager *xqsettingsManager = new XQSettingsManager();
                XQSettingsKey key(XQSettingsKey::TargetPublishAndSubscribe,
                        KPSUidBluetoothSapConnectionState.iUid,
                        KBTSapConnectionState);
                QVariant startupValue = xqsettingsManager->readItemValue(key);
                int value = startupValue.toInt();
                delete xqsettingsManager;
                xqsettingsManager = NULL;
                if (value == EBTSapConnected){
                    errorText = hbTrId("Operation not possible in SIM access profile mode");
                } else {
                    errorText = hbTrId("Operation not possible in Off-line mode");
                }
            } else {
                errorText = hbTrId("Not done");
            }
            break;
        case KErrGsmSSUnknownAlphabet:
            errorText = hbTrId("Invalid phone number");
            break;
        default:
            errorText = hbTrId("Not done");
            break;
    }

    return errorTextFound;
}

CpSettingsWrapper::CpSettingsWrapper(QObject *parent): 
    QObject(parent)
{
    m_Settings = new XQSettingsManager();
}

CpSettingsWrapper::~CpSettingsWrapper()
{
    delete m_Settings;
}

bool CpSettingsWrapper::showCallDuration()
{
    bool showDuration; 
    if (CenrepKeyValueOn == readCenrepValue(KCRUidLogs.iUid, KLogsShowCallDuration).toInt()) {
        showDuration = true; 
    } else {
        showDuration = false;
    }
    
    DPRINT << "show call duration:" << showDuration;
    return showDuration;
}

int CpSettingsWrapper::setShowCallDuration(bool value)
{
    int cenrepValue; 
    DPRINT << "show call duration:" << value;

    if (value) {
        cenrepValue = CenrepKeyValueOn;
    } else {
        cenrepValue = CenrepKeyValueOff; 
    }
    return writeCenrepValue(KCRUidLogs.iUid, KLogsShowCallDuration, cenrepValue );
}

void CpSettingsWrapper::readSoftRejectText(QString &text, bool &userDefined )
{
    if (SoftRejectTextDefault ==
        readCenrepValue(KCRUidTelephonySettings.iUid, KSettingsSoftRejectDefaultInUse ).toInt()) {
        userDefined = false; 
    } else {
        userDefined = true; 
    }
        
    text = readCenrepString(KCRUidTelephonySettings.iUid, KSettingsSoftRejectText);
    DPRINT << "text:" << text << " ,userDefined:" << userDefined;
}

int CpSettingsWrapper::writeSoftRejectText(const QString &text, bool userDefined )
{
    int err = writeCenrepValue(KCRUidTelephonySettings.iUid, KSettingsSoftRejectText, text);
    int cenrepValue; 
    if (userDefined) {
        cenrepValue = SoftRejectTextUserDefined; 
    } else {
        cenrepValue = SoftRejectTextDefault; 
    }
    err |= writeCenrepValue(KCRUidTelephonySettings.iUid, KSettingsSoftRejectDefaultInUse, cenrepValue);
    DPRINT << "text:" << text << " ,userDefined:" << userDefined << " , err:" << err;
    return err;
}

 bool CpSettingsWrapper::numberGroupingSupported() const
 {
     return readCenrepValue(KCRUidNumberGrouping.iUid, KNumberGrouping).toBool();
 }

QVariant CpSettingsWrapper::readCenrepValue(
    const long int uid, const unsigned long int key) const
{
    XQSettingsKey settingsKey(XQSettingsKey::TargetCentralRepository, uid, key);
    QVariant ret = m_Settings->readItemValue(settingsKey);
    DPRINT << "ret: " << ret;
    return ret;
}

QString CpSettingsWrapper::readCenrepString(
    const long int uid, const unsigned long int key) const
{
    XQSettingsKey settingsKey(XQSettingsKey::TargetCentralRepository, uid, key);
    QString text = m_Settings->readItemValue(settingsKey, XQSettingsManager::TypeString).toString();
    DPRINT << "text: " << text;
    return text;
}

int CpSettingsWrapper::writeCenrepValue(
    const long int uid, const unsigned long int key, const QVariant &settingsKeyValue ) const
{
    DPRINT << "uid:" << uid << ", key:" << key << ", settingsKeyValue:" << settingsKeyValue;
    XQSettingsKey settingsKey(XQSettingsKey::TargetCentralRepository, uid, key);
    int err = m_Settings->writeItemValue(settingsKey, settingsKeyValue );
    DPRINT << "err: " << err;
    return err;
}

bool CpSettingsWrapper::isFeatureCallWaitingDistiquishNotProvisionedEnabled()
{
    bool enabled = readCenrepValue(KCRUidPhoneSettings.iUid, KPSetCallWaiting).toBool();
    DPRINT << "enabled: " << enabled;
    return enabled;
}

bool CpSettingsWrapper::isPhoneOffline() const
{
    bool offLinesupport(false);
    if (XQSysInfo::isSupported(KFeatureIdOfflineMode)) {
        offLinesupport = !readCenrepValue(
                KCRUidCoreApplicationUIs.iUid, 
                KCoreAppUIsNetworkConnectionAllowed).toBool();
    }
    return offLinesupport;
}
