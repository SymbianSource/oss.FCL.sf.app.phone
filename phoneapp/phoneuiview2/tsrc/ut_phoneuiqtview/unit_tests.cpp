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
* Description:  Unit tests for PhoneUIQtView.
*
*/

#include <QtTest/QtTest>
#include <mockservice.h>
#include <QtGui>
#include <QVariant>
#include <QKeyEvent>
#include <QTimer>
#include <QShortcutEvent>
#include <QSignalSpy>
#include <hbapplication.h>
#include <hbmainwindow.h>
#include <hbinstance.h>
#include <hbmainwindow.h>
#include <bubblemanagerif.h>
#include "phoneuiqtview_p.h"
#include <hbtoolbar.h>
#include <hbvolumesliderpopup.h>
#include <hbaction.h>
#include <dialpad.h>
#include <hblineedit.h>
#include "xqserviceutil.h"
#define private public
#include "phoneuiqtview.h"

//CONSTANTS
static const int KMAXVOLUME = 10;
static const int KMINVOLUME = 0;
static const int KVOLUMECOMMAND = 5;

#define PHONE_QT_VIEW_TEST_MAIN(TestObject) \
int main(int argc, char *argv[]) \
{ \
    QCoreApplication app(argc, argv); \
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

class TestPhoneUIQtView : public QObject, public MockService
{
    Q_OBJECT
    
public:
    TestPhoneUIQtView ();
    ~TestPhoneUIQtView ();


public slots:
    void initTestCase ();
    void cleanupTestCase (); 
    void init ();
    void cleanup ();
    
private slots:
    void testAddBubbleCommand ();
    void testHideToolbar ();
    void testShowToolbar ();
    void testAddToolbarActions ();
    void testSetVolumeSliderValue ();
    void testRemoveVolumeSlider ();
    void testVolumeSliderValue ();
    void testVolumeSliderChanged ();
    void testParticipantListAction ();
    void testSetExpandAction ();
    void testSetOrientation ();
    void testBackButtonClicked();
    void testShowDialpad();
    void testDialpadText();
    void testSetMenuActions();
    void testLongEndKeyPressEventOutsideTelephony();
    void testNetworkNameChanged();
    void testCaptureKey();
    void testRestrictedMode();


private:
    int createCallHeader();

private:
    PhoneUIQtView *m_view; // class under test
    HbMainWindow *m_main_window;
    HbLineEdit *m_lineEdit;
    HbToolBar *m_toolBar;
};

TestPhoneUIQtView::TestPhoneUIQtView ()
{

}

TestPhoneUIQtView::~TestPhoneUIQtView ()
{
}

void TestPhoneUIQtView::initTestCase ()
{	
    m_main_window = new HbMainWindow();
    m_lineEdit = new HbLineEdit();
    m_toolBar = new HbToolBar();
    SmcDefaultValue<QPointF>::SetL(QPointF(0, 0));
    SmcDefaultValue<QRectF>::SetL(QRectF(0, 0, 0, 0));
    SmcDefaultValue<QVariant>::SetL(QVariant(0));
    SmcDefaultValue<Qt::Orientation>::SetL(Qt::Horizontal);
    SmcDefaultValue<HbLineEdit *>::SetL(m_lineEdit);
    SmcDefaultValue<HbLineEdit &>::SetL(*m_lineEdit);
    
    EXPECT(HbView, setTitle).with(QString("Unit test network"));
    EXPECT(QtMobility::QSystemNetworkInfo, networkName).returns(QString("Unit test network"));
    m_view = new PhoneUIQtView(*m_main_window);
    QVERIFY(verify());
}

void TestPhoneUIQtView::cleanupTestCase ()
{
    SmcDefaultValue<QPointF>::Reset();
    SmcDefaultValue<QRectF>::Reset();
    SmcDefaultValue<QVariant>::Reset();
    SmcDefaultValue<Qt::Orientation>::Reset();
    SmcDefaultValue<HbLineEdit *>::Reset();
    SmcDefaultValue<HbLineEdit &>::Reset();
    delete m_view;
    delete m_lineEdit;
    delete m_toolBar;
    delete m_main_window;
}

void TestPhoneUIQtView::init()
{
    initialize();
}

void TestPhoneUIQtView::cleanup ()
{
    reset();
}

void TestPhoneUIQtView::testAddBubbleCommand ()
{
    m_view->bubbleManager().startChanges();
    int bubbleId = createCallHeader();
    int bubbleId2 = createCallHeader();
    
    HbAction *action1 = new HbAction;
    action1->setText("Command 1");
    action1->setProperty("command", 1);
    action1->setSoftKeyRole(QAction::NoSoftKey);
    
    HbAction *action2 = new HbAction;
    action2->setText("Command 2");
    action2->setProperty("command", 2);
    action2->setSoftKeyRole(QAction::PositiveSoftKey);
    
    HbAction *action3 = new HbAction;
    action3->setText("Command 3");
    action3->setProperty("command", 3);
    action3->setSoftKeyRole(QAction::NegativeSoftKey);
    
    m_view->addBubbleCommand(bubbleId, action1);
    m_view->addBubbleCommand(bubbleId, action2);
    m_view->addBubbleCommand(bubbleId2, action3);
    m_view->bubbleManager().endChanges();
    
    m_view->bubbleManager().startChanges();
    m_view->clearBubbleCommands(bubbleId);
    m_view->clearBubbleCommands(bubbleId2);
    m_view->clearBubbleCommands(bubbleId2);
    m_view->bubbleManager().endChanges();
}

void TestPhoneUIQtView::testHideToolbar ()
{
    EXPECT(HbView, toolBar).returns(m_toolBar);
    m_view->hideToolbar();
    QVERIFY(verify());
}

void TestPhoneUIQtView::testShowToolbar ()
{
    EXPECT(HbView, toolBar).returns(m_toolBar);
    m_view->showToolbar();
    QVERIFY(verify());
}

void TestPhoneUIQtView::testAddToolbarActions ()
{
    HbIcon icon ("foo.jpg");
    HbAction *testAction = new HbAction ();
    testAction->setIcon(icon);
    testAction->setText("foo");
    testAction->setProperty("command", 20);
    QList<HbAction*> actions;
    QList<QAction *> retActions;
    EXPECT(QGraphicsWidget, actions).returns(retActions);
    actions.append(testAction);
    
    m_view->setToolbarActions(actions);
    //QCOMPARE (m_action->icon(), icon);
    //QCOMPARE(true, m_toolBarUpdateCalled);
    
    actions[0] = new HbAction();
    EXPECT(QGraphicsWidget, actions).returns(retActions);
    m_view->setToolbarActions(actions);
    
    EXPECT(QGraphicsItem, update);
    HbAction *testAction2 = new HbAction ();
    testAction2->setIcon(icon);
    testAction2->setText("foo2");
    testAction2->setProperty("command", 21);
    actions[0] = new HbAction();
    actions.append(testAction2);
    
    EXPECT(QGraphicsWidget, actions).returns(retActions);
    m_view->setToolbarActions(actions);
    QVERIFY(verify());
    actions.removeOne(testAction2);
    
    EXPECT(QGraphicsItem, update);
    actions[0] = new HbAction();
    EXPECT(QGraphicsWidget, actions).returns(retActions);
    m_view->setToolbarActions(actions);
    QVERIFY(verify());
    
    actions.clear();
}

void TestPhoneUIQtView::testSetVolumeSliderValue ()
{
    EXPECT(HbPopup, setDismissPolicy).with(HbDialog::TapOutside);
    EXPECT(HbPopup, setTimeout).with(10000);
    EXPECT(HbSliderPopup, minimum).returns(0);
    EXPECT(HbSliderPopup, maximum).returns(100);
    EXPECT(HbSliderPopup, setRange).with(KMINVOLUME, KMAXVOLUME);
    EXPECT(HbSliderPopup, setValue).with(1);
    EXPECT(QGraphicsItem, isVisible).returns(false);
    //EXPECT(QGraphicsItem, show);
    m_view->setVolumeSliderValue(1, KVOLUMECOMMAND, KMAXVOLUME, KMINVOLUME);

    QVERIFY(verify());
    reset();
    
    // The second call shouldn't create a new instance
    EXPECT(HbPopup, setDismissPolicy).with(HbDialog::TapOutside).times(0);
    EXPECT(HbPopup, setTimeout).with(10000).times(0);
    EXPECT(HbSliderPopup, minimum).returns(0);
    EXPECT(HbSliderPopup, maximum).returns(100);
    EXPECT(HbSliderPopup, setRange).with(KMINVOLUME, KMAXVOLUME);
    EXPECT(HbSliderPopup, setValue).with(2);
    EXPECT(QGraphicsItem, isVisible).returns(true);
    //EXPECT(QGraphicsItem, show).times(0);
    m_view->setVolumeSliderValue (2, KVOLUMECOMMAND, KMAXVOLUME, KMINVOLUME);
    
    QVERIFY(verify());
}

void TestPhoneUIQtView::testRemoveVolumeSlider ()
{
    //EXPECT(QGraphicsItem, hide);
    m_view->setVolumeSliderValue (1, KVOLUMECOMMAND, KMAXVOLUME, KMINVOLUME);
    m_view->removeVolumeSlider ();
    QVERIFY(verify());
}

void TestPhoneUIQtView::testVolumeSliderValue ()
{
    EXPECT(HbSliderPopup, value).returns(4);
    int value = m_view->volumeSliderValue();
    QCOMPARE (value, -1);
    
    // Create slider
    m_view->setVolumeSliderValue (1, KVOLUMECOMMAND, KMAXVOLUME, KMINVOLUME);
    
    EXPECT(HbSliderPopup, value).returns(7);
    value = m_view->volumeSliderValue();
    QCOMPARE (value, 7);
    
    EXPECT(HbSliderPopup, value).returns(3);
    value = m_view->volumeSliderValue();
    QCOMPARE (value, 3);
}

void TestPhoneUIQtView::testVolumeSliderChanged ()
{
    m_view->setVolumeSliderValue(3, KVOLUMECOMMAND, KMAXVOLUME, KMINVOLUME);
    QSignalSpy spy(m_view, SIGNAL(command(int)));
    m_view->m_priv->volumeSliderChanged(1);
    QCOMPARE(spy.count(), 1);
    int command = qvariant_cast<int>(spy.at(0).at(0));
    QCOMPARE(command, (int)KVOLUMECOMMAND);
}

void TestPhoneUIQtView::testParticipantListAction ()
{
    m_view->bubbleManager ().startChanges ();
    m_view->addParticipantListAction (1,"Command 1", HbIcon ());
    m_view->addParticipantListAction (2,"Command 2", HbIcon ());
    m_view->bubbleManager ().endChanges ();

    m_view->bubbleManager ().startChanges ();
    m_view->clearParticipantListActions ();
    m_view->clearParticipantListActions ();
    m_view->bubbleManager ().endChanges ();
}

void TestPhoneUIQtView::testSetExpandAction ()
{
    m_view->bubbleManager ().startChanges ();
    int bubbleId = createCallHeader();
    int bubbleId2 = createCallHeader();

    m_view->setExpandAction(bubbleId, 1);
    m_view->setExpandAction(bubbleId2, 2);
    m_view->bubbleManager ().endChanges ();

    m_view->bubbleManager ().startChanges ();
    m_view->removeExpandAction(bubbleId);
    m_view->removeExpandAction(bubbleId2);
    m_view->bubbleManager().removeCallHeader(bubbleId);
    m_view->bubbleManager().removeCallHeader(bubbleId2);
    m_view->bubbleManager ().endChanges ();
}

void TestPhoneUIQtView::testSetOrientation ()
{
    EXPECT(HbToolBar, setOrientation).times(0);
    m_view->handleOrientationChange(Qt::Vertical);
    QVERIFY(verify());
    
    reset(); // Reset mock framework
    EXPECT(HbToolBar, setOrientation).times(1);
    m_view->handleOrientationChange(Qt::Horizontal);
    QVERIFY(verify());
}

void TestPhoneUIQtView::testBackButtonClicked()
{
    // not possible to mock xqservice because it is in \epoc32\include
    // calling backButtonClicked would send this test background.
    // m_view->backButtonClicked();
}

void TestPhoneUIQtView::testShowDialpad()
{
    EXPECT(QGraphicsItem, isVisible).returns(true);
    EXPECT(Dialpad, closeDialpad);
    m_view->hideDialpad();
    QVERIFY(verify());

    EXPECT(QGraphicsItem, isVisible).returns(false);
    EXPECT(Dialpad, openDialpad);
    m_view->showDialpad();
    QVERIFY(verify());
    
    reset();
    EXPECT(QGraphicsItem, isVisible).returns(true);
    EXPECT(Dialpad, openDialpad).times(0);
    m_view->showDialpad();
    QVERIFY(verify());
    
    EXPECT(Dialpad, closeDialpad);
    EXPECT(QGraphicsItem, isVisible).returns(true);
    m_view->hideDialpad();
    QVERIFY(verify());
}

void TestPhoneUIQtView::testDialpadText()
{
    m_view->showDialpad();
    m_view->clearDialpad();
    EXPECT(HbLineEdit, text).returns(QString("12345"));
    QCOMPARE(m_view->dialpadText(), QString("12345"));
    m_view->clearAndHideDialpad();
    QVERIFY(verify());
}

void TestPhoneUIQtView::testSetMenuActions()
{
    EXPECT(HbView, takeMenu);
    QList<HbAction *> actions;
    QList<QAction *> retActions;
    EXPECT(QGraphicsWidget, actions).returns(retActions).times(1);
    m_view->setMenuActions(actions);
    QVERIFY(verify());
    reset();
    
    HbAction* action = new HbAction; 
    action->setText(QString("test"));
    action->setProperty("command", 1);
    actions.append(action);
    
    retActions.append(new HbAction);
    EXPECT(HbView, takeMenu).times(0);
    EXPECT(QGraphicsWidget, actions).returns(retActions).times(1);
    m_view->setMenuActions(actions);
    actions.clear();
    retActions.clear();
    QVERIFY(verify());
    reset();
    
    EXPECT(HbView, takeMenu);
    EXPECT(QGraphicsWidget, actions).returns(retActions).times(1);
    m_view->setMenuActions(actions);
    
    QVERIFY(verify());
    reset();
}

//Private methods
int TestPhoneUIQtView::createCallHeader ()
{
    int bubble = m_view->bubbleManager().createCallHeader();

    return bubble;
}

void TestPhoneUIQtView::testLongEndKeyPressEventOutsideTelephony()
{
    /*QSignalSpy spy(m_view, SIGNAL(endKeyLongPress()));
    QKeyEvent* event = new QKeyEvent(QEvent::ShortcutOverride, Qt::Key_No, Qt::NoModifier);

    bool returnValue = m_view->eventFilter(new QObject(), event);

    QVERIFY(returnValue == true);
    QCOMPARE(spy.count(), 1);*/
}

void TestPhoneUIQtView::testNetworkNameChanged()
{
    // Title is changed for GmsMode
    EXPECT(HbView, setTitle).with(QString("test"));
    m_view->m_priv->networkNameChanged(QSystemNetworkInfo::GsmMode, QString("test"));

    // Title is changed for WcdmaMode
    EXPECT(HbView, setTitle).with(QString("test2"));
    m_view->m_priv->networkNameChanged(QSystemNetworkInfo::WcdmaMode, QString("test2"));

    // Other modes shouldn't affect the title
    EXPECT(HbView, setTitle).times(0);
    m_view->m_priv->networkNameChanged(QSystemNetworkInfo::CdmaMode, QString("another operator"));
    QVERIFY(verify());
}

void TestPhoneUIQtView::testCaptureKey()
{
    m_view->captureKey( Qt::Key_No, true );
    m_view->captureKey( Qt::Key_No, true );	
    m_view->captureKey( Qt::Key_Yes, false );
    m_view->captureKey( Qt::Key_No, false );
    m_view->captureKey( Qt::Key_No, false );		
}

void TestPhoneUIQtView::testRestrictedMode()
{
    qRegisterMetaType<QKeyEvent*>("QKeyEvent*");
    QKeyEvent yesDown(QEvent::KeyPress, Qt::Key_Yes, Qt::NoModifier);
    QKeyEvent yesUp(QEvent::KeyRelease, Qt::Key_Yes, Qt::NoModifier);
    QKeyEvent oneDown(QEvent::KeyPress, Qt::Key_1, Qt::NoModifier);
    QKeyEvent oneUp(QEvent::KeyRelease, Qt::Key_1, Qt::NoModifier);
    QSignalSpy downSpy(m_view, SIGNAL(keyPressed(QKeyEvent*)));
    QSignalSpy upSpy(m_view, SIGNAL(keyReleased(QKeyEvent*)));

    m_view->setRestrictedMode(true);
    
    EXPECT(HbLineEdit, text).returns(QString("1")).times(4);
    EXPECT(Dialpad, setCallButtonEnabled).times(0);
    EXPECT(Dialpad, isCallButtonEnabled).returns(false);
    /*m_view->m_priv->onEditorContentChanged(); // Simulate signal
    */
    m_view->m_priv->eventFilter(0, &oneDown);
    m_view->m_priv->eventFilter(0, &oneUp);
    m_view->m_priv->eventFilter(0, &yesDown);
    m_view->m_priv->eventFilter(0, &yesUp);
    QCOMPARE(downSpy.count(), 1); // Only Key_1 is passed
    QCOMPARE(upSpy.count(), 1); // Only Key_1 is passed
    QCOMPARE(m_view->m_priv->m_backAction->isEnabled(), false);
    
    // Test backbutton
    m_view->setBackButtonVisible(true);
    QCOMPARE(m_view->m_priv->m_backAction->isEnabled(), false);

    // Test return to normal mode
    downSpy.clear();
    upSpy.clear();
    m_view->setRestrictedMode(false);
    EXPECT(HbLineEdit, text).returns(QString("1")).times(5);
    EXPECT(Dialpad, setCallButtonEnabled).with(true);
    EXPECT(Dialpad, isCallButtonEnabled).returns(true).times(4);
    m_view->m_priv->onEditorContentChanged(); // Simulate signal
    m_view->m_priv->eventFilter(0, &oneDown);
    m_view->m_priv->eventFilter(0, &oneUp);
    m_view->m_priv->eventFilter(0, &yesDown);
    m_view->m_priv->eventFilter(0, &yesUp);
    QCOMPARE(downSpy.count(), 2);
    QCOMPARE(upSpy.count(), 2);
    QCOMPARE(m_view->m_priv->m_backAction->isEnabled(), true);
}

PHONE_QT_VIEW_TEST_MAIN(TestPhoneUIQtView)
Q_DECLARE_METATYPE(QKeyEvent *)
#include "unit_tests.moc"
