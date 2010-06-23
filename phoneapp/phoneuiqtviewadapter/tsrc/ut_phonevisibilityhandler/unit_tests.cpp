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
* Description:  Unit tests for PhoneUIQtViewAdapter.
*
*/

#include <QtTest/QtTest>
#include <QtGui>
#include <QList>
#include <QKeyEvent>
#include <QSignalSpy>
#include <HbApplication>
#include <xqcallinfo.h>
#include <callinformation.h>
#include <MediatorDomainUIDs.h>
#include <telinformationpskeys.h>
#include <coreapplicationuisdomainpskeys.h>
#include <ccallinfoiter.h>
#include "phoneuiqtviewif.h"
#define private public
#include "phonevisibilityhandler.h"
#undef public

int m_callCount = 0;

#define PHONE_TEST_MAIN(TestObject) \
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

TInt CCallInfoIter::Count() const
{
    return m_callCount;
}

class TestPhoneVisibilityHandler : public QObject, public PhoneUIQtViewIF
{
    Q_OBJECT
public:
    TestPhoneVisibilityHandler();
    virtual ~TestPhoneVisibilityHandler();

    // From PhoneUIQtViewIF
    BubbleManagerIF& bubbleManager () {};
    void addBubbleCommand (int bubbleId, const PhoneAction& action) {};
    void clearBubbleCommands (int bubbleId) {};
    void addParticipantListAction(
            int commandId,  
            const QString &text, 
            const HbIcon &icon) {};
    void clearParticipantListActions() {};
    void hideToolbar () {};
    void showToolbar () {};
    void setToolbarActions (const QList<PhoneAction*>& actions) {};
    int volumeSliderValue () {};
    void removeVolumeSlider () {};
    void setVolumeSliderValue (
            int value, 
            int commandId, 
            int maxVolumeValue, 
            int minVolumeValue ) {};

    void setExpandAction(int bubbleId, int commandId) {};
    void removeExpandAction(int bubbleId) {};
    void showDialpad() {};
    void hideDialpad() {};
    bool isDialpadVisible() {};
    QString dialpadText() {};
    void clearAndHideDialpad() {};
    void clearDialpad() {};
    void bringToForeground() { m_bringToForegroundCalled = true;};
    void setMenuActions(const QList<PhoneAction*>& actions) {};
    void shutdownPhoneApp() {};
    void setBackButtonVisible(bool visible) {};
    HbMenu &menuReference() {};
public slots:
    void initTestCase();

    void cleanupTestCase();

    void init();

    void cleanup();
    
private slots:
    void t_memleak();
    
    void t_normal();
    
    void t_carmode();
    
    void t_devicelock();

    
private:
    PhoneVisibilityHandler *m_handler;
    bool m_bringToForegroundCalled;
    int m_startPosition;
    int m_startPriority;
    int m_normalPriority;
};

TestPhoneVisibilityHandler::TestPhoneVisibilityHandler ()
{
}

TestPhoneVisibilityHandler::~TestPhoneVisibilityHandler ()
{
}

void TestPhoneVisibilityHandler::initTestCase ()
{
    m_normalPriority = CEikonEnv::Static()->RootWin().OrdinalPriority();
    
    m_handler = new PhoneVisibilityHandler (*this, this);
    
    m_startPosition = m_handler->ordinalPosition();
    QVERIFY(m_startPosition > 0); // Expect OrdinalPosition higher than 0
    m_startPriority = CEikonEnv::Static()->RootWin().OrdinalPriority();
    QCOMPARE(m_startPriority, (int)ECoeWinPriorityNeverAtFront);
}

void TestPhoneVisibilityHandler::cleanupTestCase ()
{
    delete m_handler;
}

void TestPhoneVisibilityHandler::init ()
{

}

void TestPhoneVisibilityHandler::cleanup ()
{
    m_bringToForegroundCalled = false;
}

void TestPhoneVisibilityHandler::t_memleak()
{
}

void TestPhoneVisibilityHandler::t_normal()
{
    // Test hide device dialogs when background ( false )
    m_handler->hideDeviceDialogs(false);
    QCOMPARE(CEikonEnv::Static()->RootWin().OrdinalPosition(), m_startPosition);
    QCOMPARE(CEikonEnv::Static()->RootWin().OrdinalPriority(), m_startPriority);
    
    // Test hide device dialogs when background ( true )
    m_handler->hideDeviceDialogs(true);
    QCOMPARE(CEikonEnv::Static()->RootWin().OrdinalPosition(), m_startPosition);
    QCOMPARE(CEikonEnv::Static()->RootWin().OrdinalPriority(), m_startPriority);
    
    // bringToForeground is call
    m_handler->bringToForeground();
    QVERIFY(m_bringToForegroundCalled);
    QCOMPARE(CEikonEnv::Static()->RootWin().OrdinalPosition(), 0);
    QCOMPARE(CEikonEnv::Static()->RootWin().OrdinalPriority(), ECoeWinPriorityAlwaysAtFront + 1);
    
    // Test hide device dialogs when foreground ( false )
    m_handler->hideDeviceDialogs(false);
    QCOMPARE(CEikonEnv::Static()->RootWin().OrdinalPosition(), 0);
    QCOMPARE(CEikonEnv::Static()->RootWin().OrdinalPriority(), m_normalPriority);

    // Test hide device dialogs when foreground ( true )
    m_handler->hideDeviceDialogs(true);
    QCOMPARE(CEikonEnv::Static()->RootWin().OrdinalPosition(), 0);
    QCOMPARE(CEikonEnv::Static()->RootWin().OrdinalPriority(), ECoeWinPriorityAlwaysAtFront + 1);
    
    // send backgroud is called last
    m_handler->sendToBackground(false);
    QCOMPARE(CEikonEnv::Static()->RootWin().OrdinalPosition(), m_startPosition);
    QCOMPARE(CEikonEnv::Static()->RootWin().OrdinalPriority(), m_startPriority);
}

void TestPhoneVisibilityHandler::t_carmode()
{
    QValueSpaceSubscriber* subscriber = new QValueSpaceSubscriber("/phone/carmode", this);
    QValueSpacePublisher* publisher = new QValueSpacePublisher("/phone",this);

    QVERIFY(m_handler->m_carModeEnabled == false);
    
    // 1 ongoing call and car mode switched on
    m_callCount = 1;
    publisher->setValue(QString("/carmode"),QVariant(true));
    QTest::qWait(100);
    QVERIFY(m_handler->m_carModeEnabled == true);
    QVERIFY(m_bringToForegroundCalled == false);

    // 1 ongoing call and car mode switched off
    publisher->setValue(QString("/carmode"),QVariant(false));
    QTest::qWait(100);
    QVERIFY(m_handler->m_carModeEnabled == false);
    QVERIFY(m_bringToForegroundCalled == true);
    
    // 0 ongoing calls and car mode switched on
    m_callCount = 0;
    m_bringToForegroundCalled = false;
    publisher->setValue(QString("/carmode"),QVariant(true));
    QTest::qWait(100);
    QVERIFY(m_handler->m_carModeEnabled == true);
    QVERIFY(m_bringToForegroundCalled == false);

    // 0 ongoing calls and car mode switched off
    publisher->setValue(QString("/carmode"),QVariant(false));
    QTest::qWait(100);
    QVERIFY(m_handler->m_carModeEnabled == false);
    QVERIFY(m_bringToForegroundCalled == false);
}

void TestPhoneVisibilityHandler::t_devicelock()
{
    m_handler->HandlePropertyChangedL(KPSUidCoreApplicationUIs, KCoreAppUIsAutolockStatus, ETimerLocked);
    QCOMPARE(CEikonEnv::Static()->RootWin().OrdinalPosition(), m_startPosition);
    QCOMPARE(CEikonEnv::Static()->RootWin().OrdinalPriority(), m_startPriority);
    
    // Test hide device dialogs when background ( false )
    m_handler->hideDeviceDialogs(false);
    QCOMPARE(CEikonEnv::Static()->RootWin().OrdinalPosition(), m_startPosition);
    QCOMPARE(CEikonEnv::Static()->RootWin().OrdinalPriority(), m_startPriority);
    
    // Test hide device dialogs when background ( true )
    m_handler->hideDeviceDialogs(true);
    QCOMPARE(CEikonEnv::Static()->RootWin().OrdinalPosition(), m_startPosition);
    QCOMPARE(CEikonEnv::Static()->RootWin().OrdinalPriority(), m_startPriority);
    
    // bringToForeground is call
    m_handler->bringToForeground();
    QVERIFY(m_bringToForegroundCalled);
    QCOMPARE(CEikonEnv::Static()->RootWin().OrdinalPosition(), 0);
    QCOMPARE(CEikonEnv::Static()->RootWin().OrdinalPriority(), ECoeWinPriorityAlwaysAtFront + 1);
    
    // Test hide device dialogs when foreground ( false )
    m_handler->hideDeviceDialogs(false);
    QCOMPARE(CEikonEnv::Static()->RootWin().OrdinalPosition(), 0);
    QCOMPARE(CEikonEnv::Static()->RootWin().OrdinalPriority(), (int)ECoeWinPriorityAlwaysAtFront);

    // Test hide device dialogs when foreground ( true )
    m_handler->hideDeviceDialogs(true);
    QCOMPARE(CEikonEnv::Static()->RootWin().OrdinalPosition(), 0);
    QCOMPARE(CEikonEnv::Static()->RootWin().OrdinalPriority(), ECoeWinPriorityAlwaysAtFront + 1);
    
    // Test lock status change
    m_handler->HandlePropertyChangedL(KPSUidCoreApplicationUIs, KCoreAppUIsAutolockStatus, EAutolockOff);
    QCOMPARE(CEikonEnv::Static()->RootWin().OrdinalPosition(), 0);
    QCOMPARE(CEikonEnv::Static()->RootWin().OrdinalPriority(), ECoeWinPriorityAlwaysAtFront + 1);

    
    // send backgroud is called last
    m_handler->sendToBackground(false);
    QCOMPARE(CEikonEnv::Static()->RootWin().OrdinalPosition(), m_startPosition);
    QCOMPARE(CEikonEnv::Static()->RootWin().OrdinalPriority(), m_startPriority);
    
}


PHONE_TEST_MAIN(TestPhoneVisibilityHandler)
#include "unit_tests.moc"
