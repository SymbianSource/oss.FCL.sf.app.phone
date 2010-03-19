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

#include "ut_callsplugingroup.h"
#include "cpcallsplugingroup.h"
#include "cpitemdatahelper.h"
#include "qtestmains60.h"
// mocked dependencies for the class under test
#include "psetwrapper.h"
#include "sssettingswrapper.h"
#include "cpplugincommon.h"
#include "cpphonenotes.h"

/*!
  Dummy CPsetContainer class
 */
class CPsetContainer
    {
public:
    CPsetContainer(){};
    ~CPsetContainer(){};
    };

/*!
  UT_CpCallsPluginGroup::UT_CpCallsPluginGroup
 */
UT_CpCallsPluginGroup::UT_CpCallsPluginGroup() 
    : m_callspluginGroup(NULL)
{

}

/*!
  UT_CpCallsPluginGroup::~UT_CpCallsPluginGroup
 */
UT_CpCallsPluginGroup::~UT_CpCallsPluginGroup()
{
    delete m_callspluginGroup;
}

/*!
  UT_CpCallsPluginGroup::init
 */
void UT_CpCallsPluginGroup::init()
{
    initialize();
    
    CpItemDataHelper itemDataHelper;
    CPsetContainer cont;
    PSetCallWaitingWrapper waitingWrapper(cont);
    
    EXPECT(PSetWrapper::callWaitingWrapper).returns(&waitingWrapper);
    
    static bool b=0;//CenrepKeyValueOff;
    if (b==0) {
        b=1;//CenrepKeyValueOn
    } else {
        b=0;
    }
    EXPECT(CpSettingsWrapper::showCallDuration).returns(b);
    
    static int i=0;//No Errors
    if (i==0) {
        i=1;//Error
    } else {
        i=0;
    }
    EXPECT(SsSettingsWrapper::get).returns(i); 
    
    EXPECT(CpSettingsWrapper::readSoftRejectText);

    if (qstrcmp(QTest::currentTestFunction(), 
        "t_handleCallWaitingGetStatusDistinguishEnabled") == 0) {
        EXPECT(
            CpSettingsWrapper::isFeatureCallWaitingDistiquishNotProvisionedEnabled)
            .returns(true);
    }
    
    m_callspluginGroup = new CpCallsPluginGroup(itemDataHelper);
    QVERIFY(verify());
}

/*!
  UT_CpCallsPluginGroup::t_showCallDurationStateChanged
 */
void UT_CpCallsPluginGroup::t_showCallDurationStateChanged()
{
    int iRet=0;
    
    EXPECT(CpSettingsWrapper::setShowCallDuration).returns(iRet);
    m_callspluginGroup->showCallDurationStateChanged(Qt::Checked);
    
    EXPECT(CpSettingsWrapper::setShowCallDuration).returns(iRet);
    m_callspluginGroup->showCallDurationStateChanged(Qt::Unchecked);
    
    QVERIFY(verify());
}

/*!
  UT_CpCallsPluginGroup::t_callWaitingCurrentIndexChanged
 */
void UT_CpCallsPluginGroup::t_callWaitingCurrentIndexChanged()
{
    EXPECT(PSetCallWaitingWrapper::setCallWaiting);
    m_callspluginGroup->callWaitingCurrentIndexChanged \
            (PSetCallWaitingWrapper::ActivateCallWaiting);
    
    EXPECT(PSetCallWaitingWrapper::setCallWaiting);
    m_callspluginGroup->callWaitingCurrentIndexChanged \
            (PSetCallWaitingWrapper::DeactivateCallWaiting);
    
    EXPECT(PSetCallWaitingWrapper::getCallWaitingStatus);
    m_callspluginGroup->callWaitingCurrentIndexChanged \
            (PSetCallWaitingWrapper::CheckCallWaitingStatus);
    
    QVERIFY(verify());
    reset();
    
    //Error, negative index , do nothing
    EXPECT(PSetCallWaitingWrapper::setCallWaiting).times(0);
    m_callspluginGroup->callWaitingCurrentIndexChanged(-1);
    //Do nothing, index is out of range
    EXPECT(PSetCallWaitingWrapper::setCallWaiting).times(0);
    m_callspluginGroup->callWaitingCurrentIndexChanged \
                (PSetCallWaitingWrapper::CheckCallWaitingStatus + 100);
    
    QVERIFY(verify());
}

/*!
  UT_CpCallsPluginGroup::t_cliCurrentIndexChanged
 */
void UT_CpCallsPluginGroup::t_cliCurrentIndexChanged()
{
    EXPECT(SsSettingsWrapper::set);
    m_callspluginGroup->cliCurrentIndexChanged(1);
    
    QVERIFY(verify());
    reset();
    
    //Do nothing, index is out of range
    EXPECT(PSetCallWaitingWrapper::setCallWaiting).times(0);
    m_callspluginGroup->cliCurrentIndexChanged(-1);
    
    QVERIFY(verify());
}

/*!
  UT_CpCallsPluginGroup::t_handleCallWaitingChanged
 */
void UT_CpCallsPluginGroup::t_handleCallWaitingChanged()
{
    EXPECT(CpPhoneNotes::cancelNote);
    EXPECT(CpPhoneNotes::showGlobalNote);
    m_callspluginGroup->handleCallWaitingChanged(
        PSetCallWaitingWrapper::ActivateCallWaiting , 0);
    
    EXPECT(CpPhoneNotes::cancelNote);
    EXPECT(CpPhoneNotes::showGlobalNote);
    m_callspluginGroup->handleCallWaitingChanged(
        PSetCallWaitingWrapper::DeactivateCallWaiting, 0);
    
    EXPECT(CpPhoneNotes::cancelNote);
    EXPECT(CpPhoneNotes::showGlobalNote);
    m_callspluginGroup->handleCallWaitingChanged(
        PSetCallWaitingWrapper::CheckCallWaitingStatus, 0);
    
    EXPECT(CpPhoneNotes::cancelNote);
    EXPECT(CpPhoneNotes::showGlobalNote);
    m_callspluginGroup->handleCallWaitingChanged(
        PSetCallWaitingWrapper::DeactivateCallWaiting , -1);
    
    QVERIFY(verify());
}

/*!
  UT_CpCallsPluginGroup::t_handleCallWaitingRequesting
 */
void UT_CpCallsPluginGroup::t_handleCallWaitingRequesting()
{
    EXPECT(CpPhoneNotes::cancelNote);
    m_callspluginGroup->handleCallWaitingRequesting( false, true ); 
    
    EXPECT(CpPhoneNotes::showGlobalProgressNote);
    m_callspluginGroup->handleCallWaitingRequesting( true, false );
    
    EXPECT(CpPhoneNotes::cancelNote);
    EXPECT(CpPhoneNotes::showGlobalProgressNote);
    m_callspluginGroup->handleCallWaitingRequesting( true, true );

    QVERIFY(verify());
    reset();
    
    EXPECT(CpPhoneNotes::cancelNote).times(0);
    EXPECT(CpPhoneNotes::showGlobalProgressNote).times(0);
    m_callspluginGroup->handleCallWaitingRequesting( false, false );
    
    QVERIFY(verify());
}

/*!
  UT_CpCallsPluginGroup::t_handleCallWaitingError
 */
void UT_CpCallsPluginGroup::t_handleCallWaitingError()
{
    EXPECT(CpPhoneNotes::cancelNote);
    EXPECT(CpPhoneNotes::showGlobalErrorNote);
    m_callspluginGroup->handleCallWaitingError( -1 );
    
    QVERIFY(verify());
}

/*!
  UT_CpCallsPluginGroup::t_softRejectTextChanged
 */
void UT_CpCallsPluginGroup::t_softRejectTextChanged()
{
    int iRet=0;
    EXPECT(CpSettingsWrapper::writeSoftRejectText).returns(iRet);
    m_callspluginGroup->softRejectTextChanged();
    
    QVERIFY(verify());
}

/*!
  UT_CpCallsPluginGroup::t_handleCallWaitingGetStatus
 */
void UT_CpCallsPluginGroup::t_handleCallWaitingGetStatus()
{
    QList<unsigned char> basicServiceGroupIds;
    
    EXPECT(CpPhoneNotes::cancelNote);
    EXPECT(CpPhoneNotes::showGlobalNote);
    m_callspluginGroup->handleCallWaitingGetStatus(
        PSetCallWaitingWrapper::StatusNotProvisioned, basicServiceGroupIds);
    
    EXPECT(CpPhoneNotes::cancelNote);
    EXPECT(CpPhoneNotes::showGlobalNote);
    m_callspluginGroup->handleCallWaitingGetStatus(
        PSetCallWaitingWrapper::StatusActive, basicServiceGroupIds);
    
    EXPECT(CpPhoneNotes::cancelNote);
    EXPECT(CpPhoneNotes::showBasicServiceList);
    basicServiceGroupIds << 0;
    m_callspluginGroup->handleCallWaitingGetStatus(
        PSetCallWaitingWrapper::StatusActive, basicServiceGroupIds);
    
    QVERIFY(verify());
}

/*!
  UT_CpCallsPluginGroup::t_handleCallWaitingGetStatusDistinguishEnabled
 */
void UT_CpCallsPluginGroup::t_handleCallWaitingGetStatusDistinguishEnabled()
{
    QList<unsigned char> basicServiceGroupIds;
    
    EXPECT(CpPhoneNotes::cancelNote);
    EXPECT(CpPhoneNotes::showGlobalNote);
    m_callspluginGroup->handleCallWaitingGetStatus(
        PSetCallWaitingWrapper::StatusNotProvisioned, basicServiceGroupIds);
    
    QVERIFY(verify());
}

/*!
  UT_CpCallsPluginGroup::cleanup
 */
void UT_CpCallsPluginGroup::cleanup()
{
    reset();
    
    delete m_callspluginGroup;
    m_callspluginGroup = NULL;
}

/*!
  UT_CpCallsPluginGroup::t_memleak
 */
void UT_CpCallsPluginGroup::t_memleak()
{
    
}

QTEST_MAIN_S60(UT_CpCallsPluginGroup)
