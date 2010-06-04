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

#include "ut_networkpluginform.h"
#include "qtestmains60ui.h"
#define private public
#include "cpnetworkpluginform.h"
#include "cpitemdatahelper.h"

class CPsetContainer
{
public:
    CPsetContainer(){};
    ~CPsetContainer(){};
};

/*!
  UT_CpNetworkPluginForm::UT_CpNetworkPluginForm
 */
UT_CpNetworkPluginForm::UT_CpNetworkPluginForm() 
    : m_networkPluginForm(NULL), i(0)
{
    SmcDefaultValue<PSetNetworkWrapper::ErrorCode>::SetL(PSetNetworkWrapper::ErrNoError);
}

/*!
  UT_CpNetworkPluginForm::~UT_CpNetworkPluginForm
 */
UT_CpNetworkPluginForm::~UT_CpNetworkPluginForm()
{

}

/*!
  UT_CpNetworkPluginForm::init
 */
void UT_CpNetworkPluginForm::init()
{
    initialize();

    QVERIFY(!m_networkPluginForm);
    CPsetContainer tmpPsetContainer;
    m_networkWrapper = new PSetNetworkWrapper(tmpPsetContainer);
    expect("PSetWrapper::networkWrapper").returns(m_networkWrapper);
    if(i) {
        m_NetworkSelectionMode = PSetNetworkWrapper::SelectionModeAutomatic;
    } else {
        m_NetworkSelectionMode = PSetNetworkWrapper::SelectionModeManual;
    }
    i++;
    expect("PSetNetworkWrapper::getNetworkSelectionMode").willOnce(invoke(this, &updateNetworkSelectionMode));
    m_networkPluginForm = new CpNetworkPluginForm();
    QVERIFY(m_networkPluginForm);
}


/*!
  UT_CpNetworkPluginForm::t_networkModeStateChanged
 */
void UT_CpNetworkPluginForm::t_networkModeStateChanged()
{
    expect("CpSettingsWrapper::isPhoneOffline").returns(false);
    m_networkPluginForm->networkModeStateChanged(0);

    expect("CpSettingsWrapper::isPhoneOffline").returns(false);
    m_networkPluginForm->networkModeStateChanged(1);

    expect("CpSettingsWrapper::isPhoneOffline").returns(false);
    m_networkPluginForm->networkModeStateChanged(2);
    
    expect("CpSettingsWrapper::isPhoneOffline").returns(false);
        m_networkPluginForm->networkModeStateChanged(3);
    
    expect("CpSettingsWrapper::isPhoneOffline").returns(true);
    m_networkPluginForm->networkModeStateChanged(0);
}

/*!
  UT_CpNetworkPluginForm::t_operatorSelectionStateChanged
 */
void UT_CpNetworkPluginForm::t_operatorSelectionStateChanged()
{
    expect("CpSettingsWrapper::isPhoneOffline").returns(false);
    m_NetworkSelectionMode = PSetNetworkWrapper::SelectionModeAutomatic;
    expect("PSetNetworkWrapper::getNetworkSelectionMode").willOnce(invoke(this, &updateNetworkSelectionMode));
    m_networkPluginForm->operatorSelectionStateChanged(true);

    expect("CpSettingsWrapper::isPhoneOffline").returns(false);
    m_NetworkSelectionMode = PSetNetworkWrapper::SelectionModeManual;
    expect("PSetNetworkWrapper::getNetworkSelectionMode").willOnce(invoke(this, &updateNetworkSelectionMode));
    m_networkPluginForm->operatorSelectionStateChanged(true);
    
    expect("CpSettingsWrapper::isPhoneOffline").returns(true);
    m_networkPluginForm->operatorSelectionStateChanged(true);
}

/*!
  UT_CpNetworkPluginForm::t_networkAccessModeGot
 */
void UT_CpNetworkPluginForm::t_networkAccessModeGot()
{
    expect("CpSettingFormItemData::setContentWidgetData");
    m_networkPluginForm->networkAccessModeGot(0);
    
    expect("CpSettingFormItemData::setContentWidgetData");
    m_networkPluginForm->networkAccessModeGot(1);
    
    expect("CpSettingFormItemData::setContentWidgetData");
    m_networkPluginForm->networkAccessModeGot(2);
    
    m_networkPluginForm->networkAccessModeGot(3);
}

/*!
  UT_CpNetworkPluginForm::t_availableNetworksGot
 */
void UT_CpNetworkPluginForm::t_availableNetworksGot()
{
    PSetNetworkWrapper::NetworkInfo temp;
    QList<PSetNetworkWrapper::NetworkInfo*> networkInfoList;
    networkInfoList.append(&temp);
    
    expect("HbDialog::exec");
    m_networkPluginForm->availableNetworksGot(networkInfoList);
}

/*!
  UT_CpNetworkPluginForm::t_networkReqestFailed
 */
void UT_CpNetworkPluginForm::t_networkReqestFailed()
{
    PSetNetworkWrapper::ErrorCode error(PSetNetworkWrapper::ErrCauseCallActive);
    PSetNetworkWrapper::RequestType type(PSetNetworkWrapper::RequestSetNetworkMode);
    expect("PSetNetworkWrapper::getNetworkAccessMode");
    m_networkPluginForm->networkReqestFailed(error, type);
    
    error = PSetNetworkWrapper::ErrNoNetworkService;
    type = PSetNetworkWrapper::RequestSetNetwork;
    expect("PSetNetworkWrapper::getNetworkAccessMode");
    expect("CpSettingFormItemData::setContentWidgetData");
    m_networkPluginForm->networkReqestFailed(error, type);
    
    error = PSetNetworkWrapper::ErrOfflineOpNotAllowed;
    type = PSetNetworkWrapper::RequestSetNetwork;
    expect("PSetNetworkWrapper::getNetworkAccessMode");
    expect("CpSettingFormItemData::setContentWidgetData");
    m_networkPluginForm->networkReqestFailed(error, type);
    
    PSetNetworkWrapper::NetworkInfo temp;
    QList<PSetNetworkWrapper::NetworkInfo*> networkInfoList;
    networkInfoList.append(&temp);
    m_networkPluginForm->availableNetworksGot(networkInfoList);
    error = PSetNetworkWrapper::ErrNoNetworkAccess;
    type = PSetNetworkWrapper::RequestSetNetwork;
    expect("HbDialog::exec");
    m_networkPluginForm->networkReqestFailed(error, type);
}

/*!
  UT_CpNetworkPluginForm::t_userCancel
 */
void UT_CpNetworkPluginForm::t_userCancel()
{
    m_NetworkSelectionMode = PSetNetworkWrapper::SelectionModeManual;
    expect("PSetNetworkWrapper::getNetworkSelectionMode").willOnce(invoke(this, &updateNetworkSelectionMode));
    m_networkPluginForm->userCancel();
    
    m_NetworkSelectionMode = PSetNetworkWrapper::SelectionModeAutomatic;
    expect("PSetNetworkWrapper::getNetworkSelectionMode").willOnce(invoke(this, &updateNetworkSelectionMode));
    m_networkPluginForm->userCancel();
}

/*!
  UT_CpNetworkPluginForm::t_handleSearchingNetworks
 */
void UT_CpNetworkPluginForm::t_handleSearchingNetworks()
{
    PSetNetworkWrapper::RequestType type(PSetNetworkWrapper::RequestNone);
    m_networkPluginForm->handleSearchingNetworks(type);
    
    type = PSetNetworkWrapper::RequestEnumerateNetworks;
    m_networkPluginForm->handleSearchingNetworks(type);
}

/*!
  UT_CpNetworkPluginForm::t_handleRequestingSelectedNetwork
 */
void UT_CpNetworkPluginForm::t_handleRequestingSelectedNetwork()
{
    m_networkPluginForm->handleRequestingSelectedNetwork(true);
    m_networkPluginForm->handleRequestingSelectedNetwork(false);
}

/*!
  UT_CpNetworkPluginForm::t_handleNetworkChanged
 */
void UT_CpNetworkPluginForm::t_handleNetworkChanged()
{
    PSetNetworkWrapper::NetworkInfo currentInfo;
    PSetNetworkWrapper::RegistrationStatus status(PSetNetworkWrapper::RegisteredOnHomeNetwork);
    m_networkPluginForm->handleNetworkChanged(currentInfo, status); 

    status = PSetNetworkWrapper::RegisteredRoaming;
    currentInfo.m_longName = "test";
    m_networkPluginForm->handleNetworkChanged(currentInfo, status);
    
    status = PSetNetworkWrapper::RegisteredRoaming;
    currentInfo.m_shortName = "test";
    m_networkPluginForm->handleNetworkChanged(currentInfo, status);
}
    
/*!
  UT_CpNetworkPluginForm::cleanup
 */
void UT_CpNetworkPluginForm::cleanup()
{
    reset();
    
    delete m_networkWrapper;
    m_networkWrapper = NULL;
    delete m_networkPluginForm;
    m_networkPluginForm = NULL;
}

/*!
  UT_CpNetworkPluginForm::t_memleak
 */
void UT_CpNetworkPluginForm::t_memleak()
{
    
}

/*!
  UT_CpNetworkPluginForm::updateNetworkSelectionMode
 */
void UT_CpNetworkPluginForm::updateNetworkSelectionMode(
    PSetNetworkWrapper::NetworkSelectionMode& mode)
{
    mode = m_NetworkSelectionMode;
}

QTEST_MAIN_S60UI(UT_CpNetworkPluginForm)
