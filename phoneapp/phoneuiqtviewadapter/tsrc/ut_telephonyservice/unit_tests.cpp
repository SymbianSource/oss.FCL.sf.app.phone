/*!
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
* Description:  Unit tests for telephonyservice.
*
*/

#include <QtTest/QtTest>
#include <QtGui>
#include <hbapplication.h>
#include <QSignalSpy>
#include <xqservicerequest.h>
#include <hbmenu.h>

#include "telephonyservice.h"
#include "phoneuiqtviewif.h"
#include "phoneuiqtviewadapter.h"

extern bool phoneAppStart;
extern bool inCallDialerStart;

#define PHONE_QT_NOTE_CONTROLLER_TEST_MAIN(TestObject) \
int main(int argc, char *argv[]) \
{ \
    HbApplication app(argc, argv); \
    TestObject tc; \
    QResource::registerResource("../hbcore.rcc"); \
    int ret = QTest::qExec(&tc, argc, argv); \
    /* Core dump if HbIconLoader instance is not destroyed before the application instance. */ \
    /* HbIconLoader uses QCoreApplication::aboutToQuit() signal to destroy itself. */ \
    /* app.exec() where the signal is normally emitted is not called here. */ \
    /* So, invoking the signal explicitly. */ \
    QMetaObject::invokeMethod(&app, "aboutToQuit", Qt::DirectConnection); \
    return ret; \
}

class TestTelephonyService : public QObject,  public PhoneUIQtViewIF
{
    Q_OBJECT
public:
    TestTelephonyService();
    virtual ~TestTelephonyService();
    
public: // PhoneUIQtViewIF
    
        BubbleManagerIF& bubbleManager (){};
        void addBubbleCommand ( 
                int bubbleId,
                const PhoneAction& action ){};
        void clearBubbleCommands (int bubbleId){};
        void addParticipantListAction(
                int commandId,  
                const QString &text, 
                const HbIcon &icon){};
        void clearParticipantListActions(){};
        void hideToolbar () {};
        void showToolbar () {};
        void setToolbarActions (const QList<PhoneAction*>& actions){};
        int volumeSliderValue () {};
        void removeVolumeSlider () {};
        void setVolumeSliderValue (
                int value, 
                int commandId, 
                int maxVolumeValue, 
                int minVolumeValue ) { };

        void setExpandAction(int bubbleId, int commandId) {};
        void removeExpandAction(int bubbleId) {};
        void showDialpad() {};
        void hideDialpad() {};
        bool isDialpadVisible() {};
        QString dialpadText() {return QString("1234567");};
        void clearAndHideDialpad(){};
        void bringToForeground() {;};
        void setMenuActions(const QList<PhoneAction*>& actions){;};
        void shutdownPhoneApp() {;};
        void setBackButtonVisible(bool visible) {;};        
        HbMenu &menuReference() { return mMenu; };
        
public slots:
    void initTestCase ();
    void cleanupTestCase ();
    void init ();
    void cleanup (); 
    
private slots:
    void testConstructionDestruction ();
    void startPhoneApp ();
    void startInCallDialer ();
    
private:
    TelephonyService *m_service; // class under test
    PhoneUIQtViewAdapter *m_viewstub; // stub test
    HbMenu mMenu;
};

TestTelephonyService::TestTelephonyService ()
{
}

TestTelephonyService::~TestTelephonyService ()
{
}

void TestTelephonyService::initTestCase ()
{
    m_viewstub = new PhoneUIQtViewAdapter (*this);
    m_service = new TelephonyService(m_viewstub, this);
}

void TestTelephonyService::cleanupTestCase ()
{
    if (m_viewstub) {
    delete m_viewstub;
    }
    
    if (m_service) {
    delete m_service;
    }
}

void TestTelephonyService::init ()
{
}

void TestTelephonyService::cleanup ()
{
}

void TestTelephonyService::testConstructionDestruction ()
{
// empty
}

void TestTelephonyService::startPhoneApp ()
{
    phoneAppStart = false;
    inCallDialerStart = false;
    int phoneApp(0);
    m_service->start(phoneApp);
    QVERIFY (phoneAppStart == true);
    QVERIFY (inCallDialerStart == false);
}

void TestTelephonyService::startInCallDialer ()
{
    phoneAppStart = false;
    inCallDialerStart = false;
    int dialer(1);
    m_service->start(dialer);
    QVERIFY (phoneAppStart == true);
    QVERIFY (inCallDialerStart == true);
}

PHONE_QT_NOTE_CONTROLLER_TEST_MAIN(TestTelephonyService)
#include "unit_tests.moc"
