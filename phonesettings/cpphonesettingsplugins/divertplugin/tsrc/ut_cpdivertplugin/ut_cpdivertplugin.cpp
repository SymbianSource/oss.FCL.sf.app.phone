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

#include "ut_cpdivertplugin.h"
#include "qtestmains60ui.h"
#include <psetcalldivertingwrapper.h>
#include <hbdialog.h>
#include <qaction>
#include <hblabel.h>
#include <HbListWidgetItem>
#include <HbListWidget>
#include <HbDialog>
#include <HbLineEdit>
#include <HbAbstractViewItem>
#include <HbDataFormModel>
#include "cpitemdatahelper.h"
#include "cpdivertitemdata.h"
#include "psetwrappertypes.h"
#include "cpdivertselectioncustomitem.h"

#define private public
#include "cpdivertplugin.h"
#include "cpdivertplugingroup.h"



Q_EXTERN_C const char * qt_plugin_query_verification_data();
Q_EXTERN_C ::QObject *  qt_plugin_instance();

class CPsetContainer
{
public:
    CPsetContainer(){};
    ~CPsetContainer(){};
};

/*!
  UT_CpDivertPlugin::UT_CpDivertPlugin
 */
UT_CpDivertPlugin::UT_CpDivertPlugin() 
    : m_divertplugin(NULL)
{
    const char * verificationData = qt_plugin_query_verification_data();
}

/*!
  UT_CpDivertPlugin::~UT_CpDivertPlugin
 */
UT_CpDivertPlugin::~UT_CpDivertPlugin()
{
    delete m_divertplugin;
}

/*!
  UT_CpDivertPlugin::init
 */
void UT_CpDivertPlugin::init()
{
    initialize();

    CPsetContainer tmpPsetContainer;
    PSetCallDivertingWrapper *tmpDivWrapper = 
            new PSetCallDivertingWrapper(tmpPsetContainer, this);
    expect("PSetWrapper::callDivertingWrapper").returns(tmpDivWrapper); // FIXME to mock framework
    m_divertplugin = (CpDivertPlugin*)qt_plugin_instance();
    
    QVERIFY(m_divertplugin);
    
    m_helper = new CpItemDataHelper;
    QList<CpSettingFormItemData*> list;
    list.append(m_divertplugin->createSettingFormItemData(*m_helper));
    m_divertpluginGroup = qobject_cast<CpDivertPluginGroup *>(list.takeFirst());
    QVERIFY(m_divertpluginGroup);
    
    QVERIFY(verify());
}

/*!
  UT_CpDivertPlugin::cleanup
 */
void UT_CpDivertPlugin::cleanup()
{
    reset();
    
    delete m_divertplugin;
    m_divertplugin = NULL;
    
    delete m_divertpluginGroup;
    m_divertpluginGroup = NULL;
    
    delete m_helper;
    m_helper = NULL;
}

/*!
  UT_CpDivertPlugin::t_memleak
 */
void UT_CpDivertPlugin::t_memleak()
{
    
}

/*!
  UT_CpDivertPlugin::t_createSettingFormItemData
 */
void UT_CpDivertPlugin::t_createSettingFormItemData()
{
    CpItemDataHelper itemDataHelper;
    CPsetContainer tmpContainer;
    PSetCallDivertingWrapper tmpDivWrapper(tmpContainer); 
    
    expect("PSetWrapper::callDivertingWrapper").returns(&tmpDivWrapper); 
    
    CpDivertPlugin* p = (CpDivertPlugin*)qt_plugin_instance();
    QList<CpSettingFormItemData*> list;
    list.append(p->createSettingFormItemData(itemDataHelper));
    qDeleteAll(list);
    QVERIFY(verify());
}

/*!
  UT_CpDivertPlugin::t_changeDivertingStateRequested
 */
void UT_CpDivertPlugin::t_changeDivertingStateRequested()
{
    //except user cancels
    appendAction("All voice calls:", selectAction, "Cancel");
    expect("PSetCallDivertingWrapper::getDefaultNumbers");
    m_divertpluginGroup->m_DataItemAllVoiceCalls->setContentWidgetData("number", "");
    m_divertpluginGroup->m_DataItemAllVoiceCalls->setContentWidgetData(
            "state", CpDivertSelectionCustomitem::Disabled);
    m_divertpluginGroup->m_DataItemAllVoiceCalls->thisItemClicked();
    QVERIFY(verify());
    
    //except user selects vmb and inserts number
    appendAction("All voice calls:", selectItem, "txt_phone_setlabel_voice_mbx");
    appendAction("Voice mail box number:", insertText, "12345");
    appendAction("Voice mail box number:", selectAction, "OK");
    expect("PSetCallDivertingWrapper::getDefaultNumbers");
    expect("SsSettingsWrapper::get");
    expect("PSetCallDivertingWrapper::setCallDiverting");
    expect("CpPhoneNotes::noteShowing").returns(false);
    expect("CpPhoneNotes::showGlobalProgressNote");
    m_divertpluginGroup->m_DataItemAllVoiceCalls->thisItemClicked();
    PSCallDivertingCommand command;
    command.iNumber = "12345";
    command.iStatus = DivertingStatusActive;
    command.iCondition = m_divertpluginGroup->m_DataItemAllVoiceCalls->condition();
    expect("CpPhoneNotes::showGlobalNote");
    m_divertpluginGroup->handleDivertingChanged(command, false);
    expect("CpPhoneNotes::cancelNote");
    m_divertpluginGroup->divertRequestProcessed();
    QVERIFY(verify());

    //except user selects one of the default numbers
    m_divertpluginGroup->m_DataItemAllVoiceCalls->setContentWidgetData("number","");
    m_divertpluginGroup->m_DataItemAllVoiceCalls->setContentWidgetData(
            "state", CpDivertSelectionCustomitem::Disabled);
    appendAction("All voice calls:", selectItem, "0401234567");
    expect("PSetCallDivertingWrapper::getDefaultNumbers");
    expect("SsSettingsWrapper::get");
    expect("PSetCallDivertingWrapper::setCallDiverting");
    expect("CpPhoneNotes::noteShowing").returns(false);
    expect("CpPhoneNotes::showGlobalProgressNote");
    m_divertpluginGroup->m_DataItemAllVoiceCalls->thisItemClicked();
    command.iNumber = "0401234567";
    command.iStatus = DivertingStatusActive;
    command.iCondition = m_divertpluginGroup->m_DataItemAllVoiceCalls->condition();
    expect("CpPhoneNotes::cancelNote");
    expect("CpPhoneNotes::showGlobalNote");
    expect("PSetCallDivertingWrapper::setNewDefaultNumber").with(QString("0401234567"));
    expect("Tools::voipSupported").returns(false);
    m_divertpluginGroup->handleDivertingChanged(command, true);
    expect("CpPhoneNotes::cancelNote");
    m_divertpluginGroup->divertRequestProcessed();
    QVERIFY(verify());
    
    // Divert is disabled
    expect("SsSettingsWrapper::get");
    expect("PSetCallDivertingWrapper::setCallDiverting"); // Disable divert
    expect("CpPhoneNotes::noteShowing").returns(false);
    expect("CpPhoneNotes::showGlobalProgressNote");
    m_divertpluginGroup->m_DataItemAllVoiceCalls->thisItemClicked();
    command.iNumber = "";
    command.iStatus = DivertingStatusInactive;
    command.iCondition = m_divertpluginGroup->m_DataItemAllVoiceCalls->condition();
    expect("CpPhoneNotes::showGlobalNote");
    m_divertpluginGroup->handleDivertingChanged(command, false);
    expect("CpPhoneNotes::cancelNote");
    m_divertpluginGroup->divertRequestProcessed();
    QVERIFY(verify());
    
    //except user selects other number, inserts number and cancels
    appendAction("All voice calls:", selectItem, "Other number");
    appendAction("Number:", insertText, "12345");
    appendAction("Number:", selectAction, "Cancel");
    expect("PSetCallDivertingWrapper::getDefaultNumbers");
    m_divertpluginGroup->m_DataItemAllVoiceCalls->thisItemClicked();
    QVERIFY(verify());
}

/*!
  UT_CpDivertPlugin::t_itemShown
 */
void UT_CpDivertPlugin::t_itemShown()
{
    HbDataFormModel* model = 
            qobject_cast<HbDataFormModel*>(m_divertpluginGroup->model());
    
    expect("PSetCallDivertingWrapper::getCallDivertingStatus");
    expect("CpPhoneNotes::noteShowing").returns(false);
    expect("CpPhoneNotes::showGlobalProgressNote");
    m_divertpluginGroup->itemShown(
            model->indexFromItem(m_divertpluginGroup->m_DataItemAllVoiceCalls));
    m_divertpluginGroup->itemShown(
            model->indexFromItem(m_divertpluginGroup->m_DataItemIfBusy));
    m_divertpluginGroup->itemShown(
            model->indexFromItem(m_divertpluginGroup->m_DataItemIfNotAnswered));
    m_divertpluginGroup->itemShown(
            model->indexFromItem(m_divertpluginGroup->m_DataItemIfOutOfReach));
    m_divertpluginGroup->itemShown(
            model->indexFromItem(m_divertpluginGroup->m_DataItemIfNotAvailable));
    
    QList<PSCallDivertingStatus*> list;
    PSCallDivertingStatus divertStatus;
    list.append(&divertStatus);
    divertStatus.iCondition = m_divertpluginGroup->m_DataItemAllVoiceCalls->condition();
    m_divertpluginGroup->handleDivertingStatus(list, true);
    m_divertpluginGroup->divertRequestProcessed();
    divertStatus.iCondition = m_divertpluginGroup->m_DataItemIfBusy->condition();
    m_divertpluginGroup->handleDivertingStatus(list, true);
    m_divertpluginGroup->divertRequestProcessed();
    divertStatus.iCondition = m_divertpluginGroup->m_DataItemIfNotAnswered->condition();
    m_divertpluginGroup->handleDivertingStatus(list, true);
    m_divertpluginGroup->divertRequestProcessed();
    divertStatus.iCondition = m_divertpluginGroup->m_DataItemIfOutOfReach->condition();
    m_divertpluginGroup->handleDivertingStatus(list, true);
    m_divertpluginGroup->divertRequestProcessed();
    divertStatus.iCondition = m_divertpluginGroup->m_DataItemIfNotAvailable->condition();
    m_divertpluginGroup->handleDivertingStatus(list, true);
    m_divertpluginGroup->divertRequestProcessed();
    
    m_divertpluginGroup->divertRequestProcessed(); // Test overflow

    // Test, Do not check status again
    m_divertpluginGroup->itemShown(
            model->indexFromItem(m_divertpluginGroup->m_DataItemAllVoiceCalls));
    m_divertpluginGroup->itemShown(
            model->indexFromItem(m_divertpluginGroup->m_DataItemIfBusy));
    m_divertpluginGroup->itemShown(
            model->indexFromItem(m_divertpluginGroup->m_DataItemIfNotAnswered));
    m_divertpluginGroup->itemShown(
            model->indexFromItem(m_divertpluginGroup->m_DataItemIfOutOfReach));
    m_divertpluginGroup->itemShown(
            model->indexFromItem(m_divertpluginGroup->m_DataItemIfNotAvailable));
    

    QVERIFY(verify());
}

/*!
  UT_CpDivertPlugin::t_popUpTimerQuery
 */
void UT_CpDivertPlugin::t_popUpTimerQuery()
{
    appendAction("If not answered:", selectItem, "0401234567");
    appendAction("Time out", selectAction, "Cancel");
    expect("PSetCallDivertingWrapper::getDefaultNumbers");
    m_divertpluginGroup->m_DataItemIfNotAnswered->setContentWidgetData("number", "");
    m_divertpluginGroup->m_DataItemIfNotAnswered->setContentWidgetData(
            "state", CpDivertSelectionCustomitem::Disabled);
    m_divertpluginGroup->m_DataItemIfNotAnswered->thisItemClicked();
    QVERIFY(verify());
    
    appendAction("If not answered:", selectItem, "Other number");
    appendAction("Number:", insertText, "12345");
    appendAction("Number:", selectAction, "OK");
    appendAction("Time out", selectItem, "15 second");
    //except user chooses other number and inserts number and timeout
    expect("PSetCallDivertingWrapper::getDefaultNumbers");    
    expect("SsSettingsWrapper::get");
    expect("PSetCallDivertingWrapper::setCallDiverting");
    m_divertpluginGroup->m_DataItemIfNotAnswered->thisItemClicked();
    PSCallDivertingCommand command;
    command.iNumber = "12345";
    command.iNoReplyTimer = 15;
    command.iStatus = DivertingStatusActive;
    command.iCondition = m_divertpluginGroup->m_DataItemIfNotAnswered->condition();
    expect("CpPhoneNotes::showGlobalNote");
    m_divertpluginGroup->handleDivertingChanged(command, false);
    expect("CpPhoneNotes::cancelNote");
    m_divertpluginGroup->divertRequestProcessed();
    QVERIFY(verify());
    
    expect("SsSettingsWrapper::get");
    expect("PSetCallDivertingWrapper::setCallDiverting");
    m_divertpluginGroup->m_DataItemIfNotAnswered->thisItemClicked();
    command.iNumber = "";
    command.iNoReplyTimer = 0;
    command.iStatus = DivertingStatusInactive;
    command.iCondition = m_divertpluginGroup->m_DataItemIfNotAnswered->condition();
    expect("CpPhoneNotes::showGlobalNote");
    m_divertpluginGroup->handleDivertingChanged(command, false);
    expect("CpPhoneNotes::cancelNote");
    m_divertpluginGroup->divertRequestProcessed();
    QVERIFY(verify());

}

/*!
  UT_CpDivertPlugin::t_handleDivertingChanged
 */
void UT_CpDivertPlugin::t_handleDivertingChanged()
{
    PSCallDivertingCommand c;
    
    c.iStatus = DivertingStatusActive;
    bool bRet = false;
    expect("PSetCallDivertingWrapper::setNewDefaultNumber");
    expect("Tools::voipSupported").returns(bRet);
    m_divertpluginGroup->handleDivertingChanged(c, true);
    
    expect("PSetCallDivertingWrapper::setNewDefaultNumber");
    bRet = true;
    c.iServiceGroup = ServiceGroupVoice;
    expect("Tools::voipSupported").returns(bRet);
    m_divertpluginGroup->handleDivertingChanged(c, false);
    
    c.iStatus = DivertingStatusInactive;
    m_divertpluginGroup->handleDivertingChanged(c, true);
    m_divertpluginGroup->handleDivertingChanged(c, false);
    
    c.iStatus = DivertingStatusNotRegistered;
    m_divertpluginGroup->handleDivertingChanged(c, true);
    
    c.iStatus = DivertingStatusNotProvisioned;
    m_divertpluginGroup->handleDivertingChanged(c, true);
    
    c.iStatus = DivertingStatusUnknown;
    m_divertpluginGroup->handleDivertingChanged(c, true);
    
    c.iStatus = (PsCallDivertingStatus)5;
    m_divertpluginGroup->handleDivertingChanged(c, true);
    
    QVERIFY(verify());
}

/*!
  UT_CpDivertPlugin::t_handleDivertingStatus
 */
void UT_CpDivertPlugin::t_handleDivertingStatus()
{
    QList<PSCallDivertingStatus*> list;
    PSCallDivertingStatus divertStatus;
    
    divertStatus.iTimeout =5;
    divertStatus.iStatus = DivertingStatusActive;
    list.append(&divertStatus);
    m_divertpluginGroup->handleDivertingStatus(list, true);
    divertStatus.iTimeout =0;
    list.replace(0, &divertStatus);
    m_divertpluginGroup->handleDivertingStatus(list, true);
    
    divertStatus.iStatus = DivertingStatusInactive;
    list.replace(0, &divertStatus);
    m_divertpluginGroup->handleDivertingStatus(list, true);
    
    divertStatus.iStatus = DivertingStatusNotRegistered;
    list.replace(0, &divertStatus);
    m_divertpluginGroup->handleDivertingStatus(list, true);
    m_divertpluginGroup->handleDivertingStatus(list, false);
    
    divertStatus.iStatus = DivertingStatusNotProvisioned;
    list.replace(0, &divertStatus);
    m_divertpluginGroup->handleDivertingStatus(list, true);
    
    divertStatus.iStatus = DivertingStatusUnknown;
    list.replace(0, &divertStatus);
    m_divertpluginGroup->handleDivertingStatus(list, true);
    
    divertStatus.iStatus = (PsCallDivertingStatus)5;
    list.replace(0, &divertStatus);
    m_divertpluginGroup->handleDivertingStatus(list, true);

}

/*!
  UT_CpDivertPlugin::t_handleDivertingError
 */
void UT_CpDivertPlugin::t_handleDivertingError()
{
    m_divertpluginGroup->handleDivertingError(-1);
}

/*!
  UT_CpDivertPlugin::appendAction
 */
void UT_CpDivertPlugin::appendAction(
        const QString& dialog, actionType actionType, const QString& action)
{
    if (!actionQueue.count()) {
        startTimer(2000);
    }
    actionQueue.append(new dialogAction(dialog, actionType, action));
}

/*!
  UT_CpDivertPlugin::executeAction
 */
void UT_CpDivertPlugin::executeAction( const dialogAction &action )
{
    switch (action.type) {
    case selectAction:
        doAndVerifyAction(action.dialog, action.item);
        break;
    case selectItem:
        selectItemFromListWidget(action.dialog, action.item);
        break;
    case insertText:
        HbDialog *dialog = visibleDialog(action.dialog);
        QVERIFY( dialog );
        HbLineEdit* editor = qobject_cast<HbLineEdit*>(
                dialog->contentWidget() );
        QVERIFY( editor );
        editor->setText(action.item);
        break;
    default:
        break;
    }
}

/*!
  UT_CpDivertPlugin::visibleDialog
 */
HbDialog *UT_CpDivertPlugin::visibleDialog( const QString &heading )
{
    QList<QGraphicsItem*> items = mainWindow->scene()->items();
    foreach (QGraphicsItem* item, items) {
        HbDialog *w = qobject_cast<HbDialog*>(item->parentWidget());
        if (w && w->isVisible() && ( qobject_cast<HbLabel*>(w->headingWidget())->plainText() == heading )) {
            qDebug() << "visibleDialog: " << w->getStaticMetaObject().className() <<
                    "contentWidget: " << w->contentWidget()->getStaticMetaObject().className();
            return w;
        }
    }
    
    return 0;
}

/*!
  UT_CpDivertPlugin::selectItemFromListWidget
 */
void UT_CpDivertPlugin::selectItemFromListWidget(
        const QString& dialog, const QString& item )
{
    HbDialog* d = visibleDialog(dialog);
    QVERIFY(d);
    HbListWidget *list = qobject_cast<HbListWidget*>(d->contentWidget());
    QVERIFY(list);
    
    bool ok=true;
    HbListWidgetItem *itemObject = 0;
    for (int i=0; i < list->count();i++) {
        itemObject = list->item(i);
        if (itemObject->text() == item) {
            ok=true;
            QSignalSpy spy( d, SIGNAL(aboutToClose()));
            list->setCurrentItem(itemObject);
            d->close();
            qDebug() << "selectItemFromListWidget: " << itemObject->text();
            while (!spy.count()) {
                QTest::qWait(50);
            }
            QTest::qWait(50);
        }
        
    }
    
    QVERIFY(ok);
}

/*!
  UT_CpDivertPlugin::doAndVerifyAction
 */
void UT_CpDivertPlugin::doAndVerifyAction(
        const QString& dialog, const QString& action )
{
    HbDialog* d = visibleDialog(dialog);
    QVERIFY(d);
    
    QAction* o;
    bool ok=false;
    foreach (o, d->actions()) {
        if (o->text() == action) {
            ok=true;
            QSignalSpy spy( d, SIGNAL(aboutToClose()));
            o->trigger();
            qDebug() << "doAndVerifyAction: " << o->text();
            while (!spy.count()) {
                QTest::qWait(50);
            }
            QTest::qWait(50);
        }
    }
    QVERIFY(ok);
}

/*!
  UT_CpDivertPlugin::timerEvent
 */
void UT_CpDivertPlugin::timerEvent(
        QTimerEvent* event )
{
    QString currentTest(QTest::currentTestFunction());
    qDebug() << "timerEvent:" << currentTest;
    killTimer(event->timerId());
    executeAction(*actionQueue.takeFirst());
    if (actionQueue.count()) {
        startTimer(1000);
    }

    qDebug() << "timerEvent, OUT";
}

QTEST_MAIN_S60UI(UT_CpDivertPlugin)
