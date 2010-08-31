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
#include <hbglobal.h>
#include <CoreApplicationUIsSDKCRKeys.h>
#include <telephonyvariant.hrh>
#include <telinternalcrkeys.h>
#include <ctsydomainpskeys.h>
#include "cptelephonyutilsdefs.h"
#include "cpplugincommon.h"
#include "cppluginlogging.h"

const int CenrepKeyValueOff = 0;
const int CenrepKeyValueOn = 1;

const int SoftRejectTextDefault = 0; 
const int SoftRejectTextUserDefined = 1; 

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
  Tools::errorCodeTextMapping.
 */
bool Tools::errorCodeTextMapping(const int errorcode, QString &errorText)
{
    DPRINT;
    errorText.clear();
    bool errorTextFound(true);

    switch(errorcode){
        case KErrGsmSSSubscriptionViolation:
            errorText = hbTrId("txt_phone_info_barring_operation_not_successful");
            break;
        case KErrGsmSSUnknownSubscriber:
        case KErrGsmSSAbsentSubscriber:
        case KErrGsmSSIllegalOperation:
        case KErrGsmSSIllegalSubscriber:
        case KErrGsmSSIllegalEquipment:
        case KErrGsmSSCallBarred:
        case KErrGsmSSDataMissing:
            errorText = hbTrId("txt_phone_info_not_allowed");
            break;
        case KErrGsmSSIncompatibility:
            errorText = hbTrId("txt_phone_info_conflict_error");
            break;
        case KErrGsmSSSystemFailure:
            errorText = hbTrId("txt_phone_info_result_unknown");
            break;
        case KErrGsmSSUnexpectedDataValue:
        case KErrGsmSSResourcesUnavailable:
            errorText = hbTrId("txt_phone_info_request_rejected");
            break;
        case KErrGsmSSNegativePasswordCheck:
        case KErrGsmSSPasswordRegistrationFailure:
            errorText = hbTrId("Password error");
            break;
        case KErrGsmSSPasswordAttemptsViolation:
            errorText = hbTrId("txt_phone_info_barring_password_blocked");
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
                errorText = hbTrId("txt_phone_info_request_not_completed");
            }
            break;
        case KErrGsmSSUnknownAlphabet:
            errorText = hbTrId("txt_phone_info_invalid_phone_number");
            break;
        default:
            errorText = hbTrId("txt_phone_info_request_not_completed");
            break;
    }

    return errorTextFound;
}

CpSettingsWrapper::CpSettingsWrapper(QObject *parent): 
    QObject(parent),
    m_settings(NULL),
    m_deviceInfo(NULL)
{
    m_settings = new XQSettingsManager(this);
    m_deviceInfo = new QSystemDeviceInfo(this);
}

CpSettingsWrapper::~CpSettingsWrapper()
{
    
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

int CpSettingsWrapper::readVtVideoSending()
{
    DPRINT << ": IN";
    return readCenrepValue(KCRUidTelephonySettings.iUid, KSettingsVTVideoSending).toInt();
}

int CpSettingsWrapper::writeVtVideoSending(int value)
{
    DPRINT << ": IN";
    return writeCenrepValue(KCRUidTelephonySettings.iUid, KSettingsVTVideoSending, value);
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
 
 bool CpSettingsWrapper::forbiddenIconSupported() const
 {
     int keyValue = readCenrepValue(KCRUidTelVariation.iUid, KTelVariationFlags).toInt();
     return (KTelephonyLVFlagForbiddenIcon & keyValue);
 }

QVariant CpSettingsWrapper::readCenrepValue(
    const long int uid, const unsigned long int key) const
{
    XQSettingsKey settingsKey(XQSettingsKey::TargetCentralRepository, uid, key);
    QVariant ret = m_settings->readItemValue(settingsKey);
    DPRINT << "ret: " << ret;
    return ret;
}

QVariant CpSettingsWrapper::readPubSubValue(
    const long int uid, const unsigned long int key) const
{
    XQSettingsKey settingsKey(XQSettingsKey::TargetPublishAndSubscribe, uid, key);
    QVariant ret = m_settings->readItemValue(settingsKey);
    DPRINT << "PubSub ret: " << ret;
    return ret;
}

QString CpSettingsWrapper::readCenrepString(
    const long int uid, const unsigned long int key) const
{
    XQSettingsKey settingsKey(XQSettingsKey::TargetCentralRepository, uid, key);
    QString text = m_settings->readItemValue(settingsKey, XQSettingsManager::TypeString).toString();
    DPRINT << "text: " << text;
    return text;
}

int CpSettingsWrapper::writeCenrepValue(
    const long int uid, const unsigned long int key, const QVariant &settingsKeyValue ) const
{
    DPRINT << "uid:" << uid << ", key:" << key << ", settingsKeyValue:" << settingsKeyValue;
    XQSettingsKey settingsKey(XQSettingsKey::TargetCentralRepository, uid, key);
    int err = m_settings->writeItemValue(settingsKey, settingsKeyValue );
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
    bool networkConnectionAllowed = readCenrepValue(KCRUidCoreApplicationUIs.iUid,
                                KCoreAppUIsNetworkConnectionAllowed).toBool();
    // 0 = Offline Mode, 1 = Online Mode
    DPRINT << "networkConnectionAllowed: " << networkConnectionAllowed;
    return !networkConnectionAllowed;
}

bool CpSettingsWrapper::isOngoingCall() const
{
    bool callOngoing(false);
    if (EPSCTsyCallStateNone < 
            readPubSubValue(KPSUidCtsyCallInformation.iUid, KCTsyCallState).toInt()) {
        callOngoing = true; 
    }
    return callOngoing;
}
