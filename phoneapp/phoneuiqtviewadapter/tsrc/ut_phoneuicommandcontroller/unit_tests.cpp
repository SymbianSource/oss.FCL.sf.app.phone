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
#include <hbapplication.h>
#include <phoneappcommands.hrh>
#include "phoneuiqtviewif.h"
#include "pevirtualengine.h"
#include "bubblemanagerif.h"
#include "phonebubblewrapper.h"
#include "phoneuicommandcontroller.h"

extern bool m_setActions;
extern bool m_setInvalidCommand;
extern QList<HbAction*> m_menuActions;
extern bool m_setInvalidButtonCommands;

#define PHONE_QT_VIEW_ADAPTER_TEST_MAIN(TestObject) \
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


class TestPhoneUiCommandController : public QObject, public PhoneUIQtViewIF, public BubbleManagerIF
{
    Q_OBJECT
public:
    TestPhoneUiCommandController();
    virtual ~TestPhoneUiCommandController();

    // From PhoneUIQtViewIF
    BubbleManagerIF& bubbleManager ();
    void addBubbleCommand (int bubbleId, const PhoneAction& action);
    void clearBubbleCommands (int bubbleId);
    void addParticipantListAction(
            int commandId,  
            const QString &text, 
            const HbIcon &icon);
    void clearParticipantListActions();
    void hideToolbar () { };
    void showToolbar () { m_showToolbarCalled = true; };
    void setToolbarActions (const QList<PhoneAction*>& actions) {m_toolbarActionCount = actions.count(); };
    int volumeSliderValue () { m_volumeSliderValueCalled = true; return 5; };
    void removeVolumeSlider () { m_removeVolumeSliderCalled = true; };
    void setVolumeSliderValue (
            int value, 
            int commandId, 
            int maxVolumeValue, 
            int minVolumeValue ) { m_setVolumeSliderValueCalled = true; m_volumeSliderValue = value; };

    void setExpandAction(int bubbleId, int commandId) {m_expandAction[bubbleId]=commandId;};
    void removeExpandAction(int bubbleId) {};
    void showDialpad() {m_showDialpadCalled = true;};  
    void hideDialpad() {m_hideDialpadCalled = true;};  
    bool isDialpadVisible() 
        {m_dialpadVisibilityCalled = true;
    return m_isDialpadVisible; }
    QString dialpadText() {return m_dialpadText;};
    void clearAndHideDialpad() { m_clearAndHideDialpadCalled = true;};
    void bringToForeground() {;};
    void setMenuActions(const QList<PhoneAction*>& actions) { m_setMenuActionsCalled = true;};
    void shutdownPhoneApp() {;};
    void setBackButtonVisible(bool visible) {;};
    HbMenu &menuReference(){return m_menu;};
    
    // From BubbleManagerIF
    void startChanges () { m_startChangesCalled=true; };
    void endChanges () { m_endChangesCalled=true; };
    int createCallHeader () { return 0; };
    void removeCallHeader (int bubbleId) {};
    void setState (int bubbleId, PhoneCallState state) {};
    void setLabel ( 
        int bubbleId, 
        const QString& text, 
        Qt::TextElideMode clipDirection) {};
    void setCli (int bubbleId, 
                 const QString& cliText, 
                 Qt::TextElideMode clipDirection) {};
    void updateCLI ( 
        int bubbleId, 
        const QString& cliText, 
        Qt::TextElideMode clipDirection) {};
    void setSecondaryCli ( 
        int bubbleId, 
        const QString& cliText, 
        Qt::TextElideMode clipDirection = Qt::ElideLeft) {};
    void setCallTime (int bubbleId, const QString& callTime) {};
    void updateCallTime (int bubbleId, const QString& callTime) { m_updateCallTimeCalled=true; m_int=bubbleId; m_string=callTime; };
    void setCallObjectImage (int bubbleId, const QString &fileName) {};
    void setCallObjectFromTheme (int bubbleId) {};
    void setCallObjectText (int bubbleId, const QString &text) {};
    void setCallFlags (int bubbleId, int flags) {};
    void setCallFlag (int bubbleId, PhoneCallFlags flag, bool set) {};
    void setNumberType (int bubbleId, PhoneNumberType type) {};
    void updateCallHeaderText( 
        int bubbleId, 
        const QString &longText, 
        const QString &shortText, 
        Qt::TextElideMode clipDirection) {};
    void setPhoneMuted (bool muted) { m_setPhoneMutedCalled = muted; };
    int createConference (int bubble1, int bubble2) { return 1; };
    void removeConference () {};
    void addRowToConference (int bubbleId) {};
    void removeRowFromConference (int bubbleId) {};
    int conferenceRowCount () const { return 2; };
    void setSelectionInConference (int rowNumber) {};
    void setSelectionIdInConference (int bubbleId) {};
    int selectionInConference () const { return 0; };
    int selectionIdInConference () const { return 0; };
    void moveHighlightOneUpInConference () {};
    void moveHighlightOneDownInConference () {};
    void setExpandedConferenceCallHeader (bool expanded) { m_ExpandConferenceCalled = true; };
    bool isConferenceExpanded () const { return false; };
    int shownHeaderCount () const { return 1; };
    void setParticipantListCli (int aBubbleId, ParticipantListCli aParticipantCli) {};
    void addAction (int bubble, HbAction *action) {};
    void clearActions (int bubble) {};
    QGraphicsWidget* graphicsWidgetForAction (HbAction *action) const { return 0;};
    void addParticipantListAction (HbAction *action) {};
    int expandedBubble() const { return 1; };
    void setExpandAction(int bubbleId, HbAction* action) {};
    void setBubbleSelectionDisabled(bool b) {};
    
signals:
    void dialpadAboutToClose();
    void keyPressReleased(QKeyEvent *event);

public slots:
    void initTestCase ();
    void cleanupTestCase ();
    void init ();
    void cleanup ();
    
private slots:
    void testSetCallMenuActions();
    void testSetDialpadMenuActions();
    void testPushButtonActionsForCall();


private:
    PhoneUiCommandController *m_commandController; // class under test

    bool m_showToolbarCalled;
    bool m_updateCallTimeCalled;
    int m_int;
    QString m_string;
    HbAction *m_action;
    int  m_toolbarActionCount;
    bool m_startChangesCalled;
    bool m_endChangesCalled;
    bool m_volumeSliderValueCalled;
    bool m_removeVolumeSliderCalled;
    bool m_setVolumeSliderValueCalled;
    int m_volumeSliderValue;
    bool m_setPhoneMutedCalled;
    QMap<int, QList<int> *> m_actionMap;
    QMap<int,int> m_expandAction;
    bool m_showDialpadCalled;
    bool m_hideDialpadCalled;
    bool m_dialpadVisibilityCalled;
    bool m_clearAndHideDialpadCalled;
    bool m_ExpandConferenceCalled;
    bool m_setMenuActionsCalled;
    bool m_isDialpadVisible;
    QString m_dialpadText;
    HbMenu m_menu;

};

TestPhoneUiCommandController::TestPhoneUiCommandController ()
{
}

TestPhoneUiCommandController::~TestPhoneUiCommandController ()
{
}

void TestPhoneUiCommandController::initTestCase ()
{
    m_commandController = new PhoneUiCommandController (*this);
}

void TestPhoneUiCommandController::cleanupTestCase ()
{
    delete m_commandController;
}

void TestPhoneUiCommandController::init ()
{
    m_startChangesCalled = false;
    m_endChangesCalled = false;
}

void TestPhoneUiCommandController::cleanup ()
{
}

BubbleManagerIF& TestPhoneUiCommandController::bubbleManager ()
{
    return *this;
}

void TestPhoneUiCommandController::addBubbleCommand (
        int bubbleId, const PhoneAction& action)
{

}

void TestPhoneUiCommandController::clearBubbleCommands (int bubbleId)
{

}

void TestPhoneUiCommandController::addParticipantListAction(
    int commandId,
    const QString& text, 
    const HbIcon& icon)
{

}

void TestPhoneUiCommandController::clearParticipantListActions()
{

}

void TestPhoneUiCommandController::testSetCallMenuActions()
{
    QMap<int,int> callStates;
    QMap<int,int> serviceIds;
    int serviceId(2);
    int expandedCallId(1);
    callStates[expandedCallId] = EPEStateConnected;
    serviceIds[expandedCallId] = serviceId;
    
    
    m_commandController->setCallMenuActions(callStates,serviceIds,serviceId,expandedCallId);
    QVERIFY(m_setMenuActionsCalled);
    m_setMenuActionsCalled = false;
    
    m_setActions = true;
    m_commandController->setCallMenuActions(callStates,serviceIds,serviceId,expandedCallId);
    QVERIFY(m_setMenuActionsCalled);
    m_setMenuActionsCalled = false;
    m_setActions = false;
    
    m_setInvalidCommand = true;
    callStates[expandedCallId] = EPEStateHeld;
    m_commandController->setCallMenuActions(callStates,serviceIds,serviceId,expandedCallId);
    QVERIFY(m_setMenuActionsCalled);
    m_setMenuActionsCalled = false;
    m_setInvalidCommand = false;
    
    callStates[0] = EPEStateConnected;
    serviceIds[0] = serviceId;
    
    m_commandController->setCallMenuActions(callStates,serviceIds,serviceId,expandedCallId);
    QVERIFY(m_setMenuActionsCalled);
    m_setMenuActionsCalled = false;
    
    callStates[0] = EPEStateRinging;
    
    m_commandController->setCallMenuActions(callStates,serviceIds,serviceId,expandedCallId);
    QVERIFY(m_setMenuActionsCalled);
    m_setMenuActionsCalled = false;
    
    serviceIds[0] = 3;
    callStates[0] = EPEStateConnected;
    
    m_commandController->setCallMenuActions(callStates,serviceIds,3,expandedCallId);
    QVERIFY(m_setMenuActionsCalled);
    m_setMenuActionsCalled = false;
    
    callStates[2] = EPEStateRinging;
    serviceIds[2] = 3;
    
    m_commandController->setCallMenuActions(callStates,serviceIds,serviceId,expandedCallId);
    QVERIFY(m_setMenuActionsCalled);
    m_setMenuActionsCalled = false;
    
    callStates[3] = EPEStateDisconnecting;
    serviceIds[3] = 3;
    
    m_commandController->setCallMenuActions(callStates,serviceIds,serviceId,expandedCallId);
    QVERIFY(m_setMenuActionsCalled);
    m_setMenuActionsCalled = false;
}

void TestPhoneUiCommandController::testSetDialpadMenuActions()
{
    m_setMenuActionsCalled = false;
    m_commandController->setDialpadMenuActions();
    QVERIFY(m_setMenuActionsCalled);
}

void TestPhoneUiCommandController::testPushButtonActionsForCall()
{
    int callState = EPEStateConnected;
    bool emergencyCall = false;
    QMap<int,int> callStates;
    QMap<int,int> serviceIds;
    int serviceId(2);
    int callId(1);
    callStates[callId] = EPEStateConnected;
    serviceIds[callId] = serviceId;
    
    QMap<PhoneAction::ActionType, PhoneAction *> actions = 
        m_commandController->pushButtonActionsForCall(
            callState,
            emergencyCall,
            callStates,
            serviceIds,
            serviceId,
            callId);
    
    QVERIFY(0<actions.values().count());
    
    qDeleteAll(actions.values());
    actions.clear();
    
    m_setInvalidButtonCommands = true;
    actions =  m_commandController->pushButtonActionsForCall(
            callState,
            emergencyCall,
            callStates,
            serviceIds,
            serviceId,
            callId);
    
    QVERIFY(0==actions.values().count());
    qDeleteAll(actions.values());
    actions.clear();  
    m_setInvalidButtonCommands = false;
    
    actions = m_commandController->pushButtonActionsForCall(
            callState,
            true,
            callStates,
            serviceIds,
            serviceId,
            callId);
 
    QVERIFY(0<actions.values().count());
    qDeleteAll(actions.values());
    actions.clear();
    
    callStates[callId] = EPEStateHeld;
    callState = EPEStateHeld;
    actions = m_commandController->pushButtonActionsForCall(
            callState,
            emergencyCall,
            callStates,
            serviceIds,
            serviceId,
            callId);
 
    QVERIFY(0<actions.values().count());
    qDeleteAll(actions.values());
    actions.clear();
    
    callStates[callId] = EPEStateRinging;
    callState = EPEStateRinging;
    actions = m_commandController->pushButtonActionsForCall(
            callState,
            emergencyCall,
            callStates,
            serviceIds,
            serviceId,
            callId);
 
    QVERIFY(0<actions.values().count());
    qDeleteAll(actions.values());
    actions.clear();
    
    callStates[callId] = EPEStateIdle;
    callState = EPEStateIdle;
    actions = m_commandController->pushButtonActionsForCall(
            callState,
            emergencyCall,
            callStates,
            serviceIds,
            serviceId,
            callId);
 
    QVERIFY(0==actions.values().count());
    qDeleteAll(actions.values());
    actions.clear();

    callStates[callId] = EPEStateDisconnecting;
    callState = EPEStateDisconnecting;
    actions = m_commandController->pushButtonActionsForCall(
            callState,
            emergencyCall,
            callStates,
            serviceIds,
            serviceId,
            callId);
 
    QVERIFY(0==actions.values().count());
    qDeleteAll(actions.values());
    actions.clear();
    
    callStates[callId] = EPEStateConferenceIdle;
    callState = EPEStateConferenceIdle;
    actions = m_commandController->pushButtonActionsForCall(
            callState,
            emergencyCall,
            callStates,
            serviceIds,
            serviceId,
            callId);
 
    QVERIFY(0==actions.values().count());
    qDeleteAll(actions.values());
    actions.clear();
    
    callStates[callId] = EPEStateUnknown;
    callState = EPEStateUnknown;
    actions = m_commandController->pushButtonActionsForCall(
            callState,
            emergencyCall,
            callStates,
            serviceIds,
            serviceId,
            callId);
 
    QVERIFY(0==actions.values().count());
    qDeleteAll(actions.values());
    actions.clear();
    
    callStates[callId] = EPEStateDialing;
    callState = EPEStateDialing;
    actions = m_commandController->pushButtonActionsForCall(
            callState,
            emergencyCall,
            callStates,
            serviceIds,
            serviceId,
            callId);
 
    QVERIFY(0<actions.values().count());
    qDeleteAll(actions.values());
    actions.clear();
    
    callStates[callId] = EPEStateConnecting;
    callState = EPEStateConnecting;
    actions = m_commandController->pushButtonActionsForCall(
            callState,
            emergencyCall,
            callStates,
            serviceIds,
            serviceId,
            callId);
 
    QVERIFY(0<actions.values().count());
    qDeleteAll(actions.values());
    actions.clear();
    
    callStates[callId] = EPEStateConnectedConference;
    callState = EPEStateConnectedConference;
    actions = m_commandController->pushButtonActionsForCall(
            callState,
            emergencyCall,
            callStates,
            serviceIds,
            serviceId,
            callId);
 
    QVERIFY(0<actions.values().count());
    qDeleteAll(actions.values());
    actions.clear();
    
    callStates[callId] = EPEStateHeldConference;
    callState = EPEStateHeldConference;
    actions = m_commandController->pushButtonActionsForCall(
            callState,
            emergencyCall,
            callStates,
            serviceIds,
            serviceId,
            callId);
 
    QVERIFY(0<actions.values().count());
    qDeleteAll(actions.values());
    actions.clear();
    
    //////////////////////////////////////////////////////////////////////////
    //2 calls
    //////////////////////////////////////////////////////////////////////////
    
    callStates[2] = EPEStateHeldConference;
    actions = m_commandController->pushButtonActionsForCall(
            callState,
            emergencyCall,
            callStates,
            serviceIds,
            serviceId,
            callId);
    
    QVERIFY(0<actions.values().count());
    qDeleteAll(actions.values());
    actions.clear();
    
    callStates[2] = EPEStateHeldConference;
    callState = EPEStateHeldConference;
    actions = m_commandController->pushButtonActionsForCall(
            callState,
            emergencyCall,
            callStates,
            serviceIds,
            serviceId,
            callId);
    
    QVERIFY(0<actions.values().count());
    qDeleteAll(actions.values());
    actions.clear();
    
    serviceId = 3;
    serviceIds[callId] = serviceId;
    callStates[2] = EPEStateHeld;
    serviceIds[2] = serviceId;
    actions = m_commandController->pushButtonActionsForCall(
            callState,
            emergencyCall,
            callStates,
            serviceIds,
            serviceId,
            callId);
    
    QVERIFY(0<actions.values().count());
    qDeleteAll(actions.values());
    actions.clear();
    
    actions = m_commandController->pushButtonActionsForCall(
            callState,
            true,
            callStates,
            serviceIds,
            serviceId,
            callId);
    
    QVERIFY(0<actions.values().count());
    qDeleteAll(actions.values());
    actions.clear();
    
    callStates[2] = EPEStateHeldConference;
    callState = EPEStateHeldConference;
    actions = m_commandController->pushButtonActionsForCall(
            callState,
            emergencyCall,
            callStates,
            serviceIds,
            serviceId,
            callId);
    
    QVERIFY(0<actions.values().count());
    qDeleteAll(actions.values());
    actions.clear();
    
    callStates[2] = EPEStateConnectedConference;
    callState = EPEStateHeld;
    callStates[callId] = EPEStateHeld;
    actions = m_commandController->pushButtonActionsForCall(
            callState,
            emergencyCall,
            callStates,
            serviceIds,
            serviceId,
            callId);
    
    QVERIFY(0<actions.values().count());
    qDeleteAll(actions.values());
    actions.clear();
    
    callState = EPEStateConnectedConference;
    actions = m_commandController->pushButtonActionsForCall(
            callState,
            emergencyCall,
            callStates,
            serviceIds,
            serviceId,
            callId);
    
    QVERIFY(0<actions.values().count());
    qDeleteAll(actions.values());
    actions.clear();
    
    callStates[callId] = EPEStateConnected;
    callStates[2] = EPEStateHeldConference;
    callState = EPEStateConnected;
    
    callState = EPEStateConnectedConference;
    actions = m_commandController->pushButtonActionsForCall(
            callState,
            emergencyCall,
            callStates,
            serviceIds,
            serviceId,
            callId);
    
    QVERIFY(0<actions.values().count());
    qDeleteAll(actions.values());
    actions.clear();
    
    callStates[callId] = EPEStateConnected;
    callStates[2] = EPEStateHeldConference;
    callState = EPEStateHeldConference;
    
    //////////////////////////////////////////////////////////////////////////
    //3 calls
    //////////////////////////////////////////////////////////////////////////
    callStates[3] = EPEStateRinging;
    serviceIds[3] = serviceId;
    actions = m_commandController->pushButtonActionsForCall(
            callState,
            emergencyCall,
            callStates,
            serviceIds,
            serviceId,
            callId);
    
    QVERIFY(0<actions.values().count());
    qDeleteAll(actions.values());
    actions.clear();
    
    actions = m_commandController->pushButtonActionsForCall(
            EPEStateRinging,
            emergencyCall,
            callStates,
            serviceIds,
            serviceId,
            3);
    
    QVERIFY(0<actions.values().count());
    qDeleteAll(actions.values());
    actions.clear();
    
    actions = m_commandController->pushButtonActionsForCall(
            EPEStateHeld,
            emergencyCall,
            callStates,
            serviceIds,
            serviceId,
            2);
    
    QVERIFY(0<actions.values().count());
    qDeleteAll(actions.values());
    actions.clear();
    
}


PHONE_QT_VIEW_ADAPTER_TEST_MAIN(TestPhoneUiCommandController)
#include "unit_tests.moc"
