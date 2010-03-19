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

#include "ut_cpphonenotes.h"
#include "qtestmains60ui.h"
#include <hbmainwindow.h>
#include <HbDeviceMessageBox.h>
#include <HbMessageBox.h>
#define private public
#include "cpphonenotes.h"

/*!
  UT_CpPhoneNotes::UT_CpPhoneNotes
 */
UT_CpPhoneNotes::UT_CpPhoneNotes() 
{
    CpPhoneNotes::instance();
}

/*!
  UT_CpPhoneNotes::~UT_CpPhoneNotes
 */
UT_CpPhoneNotes::~UT_CpPhoneNotes()
{
    //delete m_psui;
}

/*!
  UT_CpPhoneNotes::timerEvent
 */
void UT_CpPhoneNotes::timerEvent( QTimerEvent* event )
{
    QString currentTest(QTest::currentTestFunction());
    qDebug() << "timerEvent:" << currentTest;
    
    if (currentTest == "t_showGlobalErrorNote") {
        verifyGlobalNote(wantedNoteText);
    }
    
    if (m_noteid1) {
        CpPhoneNotes::instance()->cancelNote(m_noteid1);
        m_noteid1 = 0;
    }
    if (m_noteid2) {
        CpPhoneNotes::instance()->cancelNote(m_noteid2);
        m_noteid2 = 0;
    }
    if (m_noteid3) {
        CpPhoneNotes::instance()->cancelNote(m_noteid3);
        m_noteid3 = 0;
    }
    
    if (visibleDialog()) {
        visibleDialog()->close();
        QTest::qWait(1);
    }
	
}

/*!
  UT_CpPhoneNotes::visibleDialog
 */
HbDialog *UT_CpPhoneNotes::visibleDialog()
{
    QList<QGraphicsItem*> items = mainWindow->scene()->items();
    foreach (QGraphicsItem* item, items) {
        HbDialog *w = qobject_cast<HbDialog*>(item->parentWidget());
        if (w && w->isVisible()) {
            qDebug() << "visibleDialog: " << w->getStaticMetaObject().className() <<
                    "contentWidget: " << w->contentWidget()->getStaticMetaObject().className();
            return w;
        }
    }
    
    return 0;
}

/*!
  UT_CpPhoneNotes::verifyGlobalNote
 */
void UT_CpPhoneNotes::verifyGlobalNote( const QString& noteText )
{
    qDebug() << "verifyGlobalNote:" << noteText;
    HbDeviceMessageBox *note=0;
    QObject* o=0;
    QVERIFY(CpPhoneNotes::instance()->m_notesQueue->count());
    QVERIFY(o = CpPhoneNotes::instance()->m_notesQueue->at(0));
    QVERIFY(note = qobject_cast<HbDeviceMessageBox *>(o));
    QCOMPARE(note->text(), noteText);
    note->close();
    QTest::qWait(1);
}

/*!
  UT_CpPhoneNotes::appendAllEtelServices
 */
void UT_CpPhoneNotes::appendAllEtelServices(QList<PSCallDivertingStatus*>& list)
{   
    /** The call service has not been specified. */
    appendService(list, RMobilePhone::EServiceUnspecified);
    /** The API request applies to voice call services. */
    appendService(list, RMobilePhone::EVoiceService);
    /** The API request applies to auxiliary voice call services. */
    appendService(list, RMobilePhone::EAuxVoiceService);
    /** The API request applies to circuit switched data call services. */
    appendService(list, RMobilePhone::ECircuitDataService);
    /** The API request applies to packet data services. */
    appendService(list, RMobilePhone::EPacketDataService);
    /** The API request applies to fax call services. */
    appendService(list, RMobilePhone::EFaxService);
    /** The API request applies to short message services. */
    appendService(list, RMobilePhone::EShortMessageService);
    /** The API request applies to all mobile services. */
    appendService(list, RMobilePhone::EAllServices);
    /** All teleservices */
    appendService(list, RMobilePhone::EAllTele);
    /** Telephony */
    appendService(list, RMobilePhone::ETelephony);
    /** All data teleservices */
    appendService(list, RMobilePhone::EAllDataTele);
    /** Voice Broadcast Service (VBS) Bearer Service */
    appendService(list, RMobilePhone::EAllDataExSms);
    /** All teleservices except SMS */
    appendService(list, RMobilePhone::EAllTeleExcSms);
    /** All PLMN specific teleservices */
    appendService(list, RMobilePhone::EAllPlmnTele);
    /** PLMN specific teleservice 1 */
    appendService(list, RMobilePhone::EPlmnTele1);
    /** PLMN specific teleservice 2 */
    appendService(list, RMobilePhone::EPlmnTele2);
    /** PLMN specific teleservice 3 */
    appendService(list, RMobilePhone::EPlmnTele3);
    /** PLMN specific teleservice 4 */
    appendService(list, RMobilePhone::EPlmnTele4);
    /** PLMN specific teleservice 5 */
    appendService(list, RMobilePhone::EPlmnTele5);
    /** PLMN specific teleservice 6 */
    appendService(list, RMobilePhone::EPlmnTele6);
    /** PLMN specific teleservice 7 */
    appendService(list, RMobilePhone::EPlmnTele7);
    /** PLMN specific teleservice 8 */
    appendService(list, RMobilePhone::EPlmnTele8);
    /** PLMN specific teleservice 9 */
    appendService(list, RMobilePhone::EPlmnTele9);
    /** PLMN specific teleservice 10 */
    appendService(list, RMobilePhone::EPlmnTeleA);
    /** PLMN specific teleservice 11 */
    appendService(list, RMobilePhone::EPlmnTeleB);
    /** PLMN specific teleservice 12 */
    appendService(list, RMobilePhone::EPlmnTeleC);
    /** PLMN specific teleservice 13 */
    appendService(list, RMobilePhone::EPlmnTeleD);
    /** PLMN specific teleservice 14 */
    appendService(list, RMobilePhone::EPlmnTeleE);
    /** PLMN specific teleservice 15 */
    appendService(list, RMobilePhone::EPlmnTeleF);
    /** All bearer services */
    appendService(list, RMobilePhone::EAllBearer);
    /** All async services */
    appendService(list, RMobilePhone::EAllAsync);
    /** All sync services */
    appendService(list, RMobilePhone::EAllSync);
    /** All data circuit sync */
    appendService(list, RMobilePhone::ESyncData);
    /** All data circuit async */
    appendService(list, RMobilePhone::EAsyncData);
    /** All packet data services */
    appendService(list, RMobilePhone::EPacketData);
    /** All pad access services */
    appendService(list, RMobilePhone::EPadAccess);
    /** All PLMN specific bearer services */
    appendService(list, RMobilePhone::EAllPlmnBearer);
    /** PLMN specific bearer service 1 */
    appendService(list, RMobilePhone::EPlmnBearerServ1);
    /** PLMN specific bearer service 2 */
    appendService(list, RMobilePhone::EPlmnBearerServ2);
    /** PLMN specific bearer service 3 */
    appendService(list, RMobilePhone::EPlmnBearerServ3);
    /** PLMN specific bearer service 4 */
    appendService(list, RMobilePhone::EPlmnBearerServ4);
    /** PLMN specific bearer service 5 */
    appendService(list, RMobilePhone::EPlmnBearerServ5);
    /** PLMN specific bearer service 6 */
    appendService(list, RMobilePhone::EPlmnBearerServ6);
    /** PLMN specific bearer service 7 */
    appendService(list, RMobilePhone::EPlmnBearerServ7);
    /** PLMN specific bearer service 8 */
    appendService(list, RMobilePhone::EPlmnBearerServ8);
    /** PLMN specific bearer service 9 */
    appendService(list, RMobilePhone::EPlmnBearerServ9);
    /** PLMN specific bearer service 10 */
    appendService(list, RMobilePhone::EPlmnBearerServA);
    /** PLMN specific bearer service 11 */
    appendService(list, RMobilePhone::EPlmnBearerServB);
    /** PLMN specific bearer service 12 */
    appendService(list, RMobilePhone::EPlmnBearerServC);
    /** PLMN specific bearer service 13 */
    appendService(list, RMobilePhone::EPlmnBearerServD);
    /** PLMN specific bearer service 14 */
    appendService(list, RMobilePhone::EPlmnBearerServE);
    /** PLMN specific bearer service 15 */
    appendService(list, RMobilePhone::EPlmnBearerServF);
    /** Alternative tele services */
    appendService(list, RMobilePhone::EAltTele);
    /** Voice Group Call Service (VGCS) */
    appendService(list, RMobilePhone::EVoiceGroupCall);
    /** Voice Broadcast Service (VBS) */
    appendService(list, RMobilePhone::EVoiceBroadcast);
    /** All GPRS Bearer Services */
    appendService(list, RMobilePhone::EAllGprsBearer);
}

/*!
  UT_CpPhoneNotes::appendService
 */
void UT_CpPhoneNotes::appendService(
        QList<PSCallDivertingStatus*>& list, RMobilePhone::TMobileService service )
{
    PSCallDivertingStatus* item = new PSCallDivertingStatus;
    item->iServiceGroup = service;
    list.append(item);
}

/*!
  UT_CpPhoneNotes::init
 */
void UT_CpPhoneNotes::init()
{
    initialize();
    timerid = startTimer(3000);
    m_noteid1 = 0;
    m_noteid2 = 0;
    m_noteid3 = 0;
}

/*!
  UT_CpPhoneNotes::cleanup
 */
void UT_CpPhoneNotes::cleanup()
{
    reset();
    if (timerid) {
        killTimer(timerid);
        timerid = 0;
    }
}

/*!
  UT_CpPhoneNotes::t_memleak
 */
void UT_CpPhoneNotes::t_memleak()
{
    
}

/*!
  UT_CpPhoneNotes::t_showProgressNote
 */
void UT_CpPhoneNotes::t_showProgressNote()
{
    CpPhoneNotes::instance()->showGlobalProgressNote(m_noteid1, "test");
    CpPhoneNotes::instance()->showGlobalProgressNote(m_noteid2, "test");
    QTest::qWait(5000);
}

/*!
  UT_CpPhoneNotes::t_showGlobalProgressNote
 */
void UT_CpPhoneNotes::t_showGlobalProgressNote()
{
    CpPhoneNotes::instance()->showGlobalProgressNote(m_noteid1, "test");
    QTest::qWait(5000);
}

/*!
  UT_CpPhoneNotes::t_showGlobalNote
 */
void UT_CpPhoneNotes::t_showGlobalNote()
{
    CpPhoneNotes::instance()->showGlobalNote(m_noteid1, "MessageTypeQuestion",HbMessageBox::MessageTypeQuestion );
    CpPhoneNotes::instance()->showGlobalNote(m_noteid2, "MessageTypeInformation",HbMessageBox::MessageTypeInformation);
    CpPhoneNotes::instance()->showGlobalNote(m_noteid3, "MessageTypeWarning",HbMessageBox::MessageTypeWarning );
    
}

/*!
  UT_CpPhoneNotes::t_showGlobalErrorNote
 */
void UT_CpPhoneNotes::t_showGlobalErrorNote()
{
    int errorcode = -1;
    wantedNoteText = "";
    expect("Tools::errorCodeTextMapping").with(errorcode,wantedNoteText).returns(true);
    CpPhoneNotes::instance()->showGlobalErrorNote(m_noteid1, errorcode );
    QVERIFY( verify() );

}

/*!
  UT_CpPhoneNotes::t_showBasicServiceList
 */
void UT_CpPhoneNotes::t_showBasicServiceList()
{
    QList<unsigned char> basicServiceGroupIds;
    basicServiceGroupIds << Telephony;
    basicServiceGroupIds << AllDataTele;
    CpPhoneNotes::instance()->
        showBasicServiceList("Basic Service List", basicServiceGroupIds );
}

/*!
  UT_CpPhoneNotes::t_showBasicServiceCallDivertList
 */
void UT_CpPhoneNotes::t_showBasicServiceCallDivertList()
{
    QList<PSCallDivertingStatus*> statusList;
    int selection = 0;
    CallDivertType divertType = NoCallDivert;
    CallDivertType divertDetailType = NoCallDivert;
    CpPhoneNotes::instance()->showBasicServiceCallDivertList( 
        "title", statusList, selection, divertType, divertDetailType );
    
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EAllServices).returns(AllTeleAndBearer);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EAllTele).returns(AllTele);

    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EVoiceService).returns(Telephony);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::ETelephony).returns(Telephony);
    
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EAllDataTele).returns(AllDataTele);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EFaxService).returns(Fax);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EShortMessageService).returns(Sms);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EAllDataExSms).returns(AllDataExSms);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EAllTeleExcSms).returns(AllTeleExcSms);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EAllPlmnTele).returns(AllPlmnTele);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EPlmnTele1).returns(PlmnTele1);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EPlmnTele2).returns(PlmnTele2);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EPlmnTele3).returns(PlmnTele3);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EPlmnTele4).returns(PlmnTele4);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EPlmnTele5).returns(PlmnTele5);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EPlmnTele6).returns(PlmnTele6);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EPlmnTele7).returns(PlmnTele7);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EPlmnTele8).returns(PlmnTele8);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EPlmnTele9).returns(PlmnTele9);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EPlmnTeleA).returns(PlmnTeleA);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EPlmnTeleB).returns(PlmnTeleB);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EPlmnTeleC).returns(PlmnTeleC);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EPlmnTeleD).returns(PlmnTeleD);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EPlmnTeleE).returns(PlmnTeleE);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EPlmnTeleF).returns(PlmnTeleF);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EAllBearer).returns(AllBearer);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EAllAsync).returns(AllAsync);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EAllSync).returns(AllSync);
    
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::ECircuitDataService).returns(SyncData);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::ESyncData).returns(SyncData);
    
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EPacketDataService).returns(AsyncData);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EAsyncData).returns(AsyncData);
    
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EPacketData).returns(PacketData);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EPadAccess).returns(PadAccess);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EAllPlmnBearer).returns(AllPlmnBearer);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EPlmnBearerServ1).returns(PlmnBearerServ1);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EPlmnBearerServ2).returns(PlmnBearerServ2);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EPlmnBearerServ3).returns(PlmnBearerServ3);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EPlmnBearerServ4).returns(PlmnBearerServ4);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EPlmnBearerServ5).returns(PlmnBearerServ5);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EPlmnBearerServ6).returns(PlmnBearerServ6);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EPlmnBearerServ7).returns(PlmnBearerServ7);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EPlmnBearerServ8).returns(PlmnBearerServ8);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EPlmnBearerServ9).returns(PlmnBearerServ9);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EPlmnBearerServA).returns(PlmnBearerServA);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EPlmnBearerServB).returns(PlmnBearerServB);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EPlmnBearerServC).returns(PlmnBearerServC);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EPlmnBearerServD).returns(PlmnBearerServD);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EPlmnBearerServE).returns(PlmnBearerServE);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EPlmnBearerServF).returns(PlmnBearerServF);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EAuxVoiceService).returns(AltTele);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EAltTele).returns(AltTele);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EServiceUnspecified).returns(Unknown);
    // ??
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EVoiceGroupCall).returns(Unknown);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EVoiceBroadcast).returns(Unknown);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EAllGprsBearer).returns(Unknown);

    expect("Tools::videoSupported").returns(true);
    appendAllEtelServices( statusList );
    divertType = CallDivertWithNumber;
    CpPhoneNotes::instance()->showBasicServiceCallDivertList( 
        "title", statusList, selection, divertType, divertDetailType );
    QVERIFY( verify() );

    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EAllServices).returns(AllTeleAndBearer);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EAllTele).returns(AllTele);

    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EVoiceService).returns(Telephony);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::ETelephony).returns(Telephony);
    
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EAllDataTele).returns(AllDataTele);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EFaxService).returns(Fax);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EShortMessageService).returns(Sms);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EAllDataExSms).returns(AllDataExSms);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EAllTeleExcSms).returns(AllTeleExcSms);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EAllPlmnTele).returns(AllPlmnTele);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EPlmnTele1).returns(PlmnTele1);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EPlmnTele2).returns(PlmnTele2);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EPlmnTele3).returns(PlmnTele3);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EPlmnTele4).returns(PlmnTele4);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EPlmnTele5).returns(PlmnTele5);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EPlmnTele6).returns(PlmnTele6);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EPlmnTele7).returns(PlmnTele7);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EPlmnTele8).returns(PlmnTele8);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EPlmnTele9).returns(PlmnTele9);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EPlmnTeleA).returns(PlmnTeleA);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EPlmnTeleB).returns(PlmnTeleB);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EPlmnTeleC).returns(PlmnTeleC);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EPlmnTeleD).returns(PlmnTeleD);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EPlmnTeleE).returns(PlmnTeleE);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EPlmnTeleF).returns(PlmnTeleF);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EAllBearer).returns(AllBearer);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EAllAsync).returns(AllAsync);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EAllSync).returns(AllSync);
    
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::ECircuitDataService).returns(SyncData);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::ESyncData).returns(SyncData);
    
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EPacketDataService).returns(AsyncData);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EAsyncData).returns(AsyncData);
    
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EPacketData).returns(PacketData);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EPadAccess).returns(PadAccess);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EAllPlmnBearer).returns(AllPlmnBearer);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EPlmnBearerServ1).returns(PlmnBearerServ1);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EPlmnBearerServ2).returns(PlmnBearerServ2);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EPlmnBearerServ3).returns(PlmnBearerServ3);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EPlmnBearerServ4).returns(PlmnBearerServ4);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EPlmnBearerServ5).returns(PlmnBearerServ5);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EPlmnBearerServ6).returns(PlmnBearerServ6);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EPlmnBearerServ7).returns(PlmnBearerServ7);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EPlmnBearerServ8).returns(PlmnBearerServ8);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EPlmnBearerServ9).returns(PlmnBearerServ9);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EPlmnBearerServA).returns(PlmnBearerServA);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EPlmnBearerServB).returns(PlmnBearerServB);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EPlmnBearerServC).returns(PlmnBearerServC);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EPlmnBearerServD).returns(PlmnBearerServD);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EPlmnBearerServE).returns(PlmnBearerServE);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EPlmnBearerServF).returns(PlmnBearerServF);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EAuxVoiceService).returns(AltTele);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EAltTele).returns(AltTele);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EServiceUnspecified).returns(Unknown);
    // ??
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EVoiceGroupCall).returns(Unknown);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EVoiceBroadcast).returns(Unknown);
    expect("Tools::convertEtelMobileServiceCode").with(RMobilePhone::EAllGprsBearer).returns(Unknown);

    expect("Tools::videoSupported").returns(false);
    divertType = CallDivertWithNumberAndTimeout;
    CpPhoneNotes::instance()->showBasicServiceCallDivertList( 
        "title", statusList, selection, divertType, divertDetailType );
    QVERIFY( verify() );
}

/*!
  UT_CpPhoneNotes::t_showCallDivertDetails
 */
void UT_CpPhoneNotes::t_showCallDivertDetails()
{
    expect("CpSettingsWrapper::numberGroupingSupported").returns(QVariant(true));
    PSCallDivertingStatus status;
    CpPhoneNotes::instance()->showCallDivertDetails(status);
    QVERIFY( verify() );

    expect("CpSettingsWrapper::numberGroupingSupported").returns(QVariant(false));
    status.iTimeout = 30;
    CpPhoneNotes::instance()->showCallDivertDetails(status);
    QVERIFY( verify() );
}

/*!
  UT_CpPhoneNotes::t_cancelNote
 */
void UT_CpPhoneNotes::t_cancelNote()
{
    CpPhoneNotes::instance()->showGlobalProgressNote(m_noteid1, "test");
    QTest::qWait(5000);
}

QTEST_MAIN_S60UI(UT_CpPhoneNotes)
