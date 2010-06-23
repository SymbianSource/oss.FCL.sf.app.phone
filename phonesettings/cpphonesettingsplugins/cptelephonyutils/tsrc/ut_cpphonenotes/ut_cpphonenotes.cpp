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
#include <hbnotificationdialog.h>
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
  UT_CpPhoneNotes::t_showNotificationDialog
 */
void UT_CpPhoneNotes::t_showNotificationDialog()
{
    CpPhoneNotes::instance()->showNotificationDialog("NotificationDialog");
}

/*!
  UT_CpPhoneNotes::t_showProgressNote
 */
void UT_CpPhoneNotes::t_showProgressNote()
{
    CpPhoneNotes::instance()->showGlobalProgressNote(m_noteid1, "testshowProgressNote1");
    CpPhoneNotes::instance()->showGlobalProgressNote(m_noteid2, "testshowProgressNote2");
    QTest::qWait(5000);
}

/*!
  UT_CpPhoneNotes::t_showGlobalProgressNote
 */
void UT_CpPhoneNotes::t_showGlobalProgressNote()
{
    CpPhoneNotes::instance()->showGlobalProgressNote(m_noteid1, "testshowGPN");
    QTest::qWait(5000);
    CpPhoneNotes::instance()->showGlobalProgressNote(m_noteid1,
            hbTrId("txt_common_info_requesting"));
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
  UT_CpPhoneNotes::t_showPasswordQueryDialog
 */
void UT_CpPhoneNotes::t_showPasswordQueryDialog()
{
    QString queryTitle("Password query title");
    QString password = "";
    bool okPressed = false;
    QRegExp regExpression("\\d{4}");
    QRegExpValidator validator(regExpression, this);
    CpPhoneNotes::instance()->showPasswordQueryDialog(
        queryTitle, validator, 5);

}


/*!
  UT_CpPhoneNotes::t_cancelNote
 */
void UT_CpPhoneNotes::t_cancelNote()
{
    CpPhoneNotes::instance()->cancelNote(m_noteid1);
    CpPhoneNotes::instance()->showGlobalProgressNote(m_noteid1, "testcancelNote");
    CpPhoneNotes::instance()->noteShowing();
    CpPhoneNotes::instance()->cancelNote(m_noteid1);
        
    QTest::qWait(5000);
}

QTEST_MAIN_S60UI(UT_CpPhoneNotes)
