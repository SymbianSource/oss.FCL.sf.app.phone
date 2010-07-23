/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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

#include <QGraphicsSceneMouseEvent>

#include <HbStyle>
#include <HbStyleLoader>
#include <HbFrameItem>
#include <HbFrameDrawer>
#include <HbIconItem>
#include <HbTextItem>
#include <HbTouchArea>
#include <HbInstantFeedback>
#include "dialerwidgetengine.h"
#include "dialerwidget.h"
#include "qtphonelog.h"

#ifdef Q_OS_SYMBIAN
#include "qtphonelog.h"
#include <xqappmgr.h>
#include <xqservicerequest.h>
#include <xqcallinfo.h>
#include <xqpublishandsubscribeutils.h>
#include <logsservices.h>
#include <xqrequestinfo.h>
#endif

namespace
{
    const char KDialerWidgetIconNormal[] = "qtg_graf_hs_dialer_normal";
    const char KDialerWidgetIconPressed[] = "qtg_graf_hs_dialer_pressed";
    const char KMissedCallShortcutBadge[] = "qtg_fr_shortcut_badge_bg";
    const char KDialerWidgetWidgetml[] = ":/data/resource/dialerwidget.widgetml";
    const char KDialerWidgetCss[] = ":/data/resource/dialerwidget.css";
}

/*!
    \class DialerWidget

    \ingroup group_dialerwidgetplugin
    \brief Implementation for the homescreen dialer launcher.

*/

/*!
    Constructs dialer widget with given \a parent and given window \a flags.
*/
DialerWidget::DialerWidget(QGraphicsItem *parent, Qt::WindowFlags flags)
  : HsWidget(parent, flags),
    m_background(0), m_badgeBackground(0), m_text(0), m_touchArea(0),
    m_engine(0)
{   
    PHONE_TRACE 
}

/*!
    Destructor.
*/
DialerWidget::~DialerWidget()
{
}

/*!
    \fn void DialerWidget::startDialer()

    Starts dialer widget via view activation service.
*/
void DialerWidget::startDialer()
{
    PHONE_TRACE
#ifdef Q_OS_SYMBIAN
    PHONE_DEBUG("DialerWidget::startDialer");
    
    QList<CallInfo> calls;
    QScopedPointer<XQCallInfo> callInfo(XQCallInfo::create());
    callInfo->getCalls(calls);
    QList<QVariant> args;
    QString service;
    QString interface;
    QString operation;

    if (0 < calls.count()) {
        PHONE_DEBUG("call ongoing, bring Telephone to foreground");
        service = "phoneui";
        interface = "com.nokia.symbian.IStart";
        operation = "start(int)";
        int openDialer(0);
        args << openDialer;
    } else {
        PHONE_DEBUG("no calls, open Dialer");
        service = "logs";
        interface = "com.nokia.symbian.ILogsView";
        operation = "show(QVariantMap)";
        QVariantMap map;
        map.insert("view_index", QVariant(int(LogsServices::ViewAll)));
        map.insert("show_dialpad", QVariant(true));
        map.insert("dialpad_text", QVariant(QString()));
        args.append(QVariant(map));
    }

    XQApplicationManager appManager;
    QScopedPointer<XQAiwRequest> request(appManager.create(service, interface, operation, false));
    if (request == NULL) {
        return;
    }
    request->setArguments(args);
    XQRequestInfo info;
    info.setForeground(true);
    request->setInfo(info);
    bool ret = request->send();
    PHONE_TRACE2("request sent successfully:", ret);
#endif

}

void DialerWidget::onInitialize()
{
    PHONE_TRACE
    QT_TRY{
        // basic ui
        createPrimitives();
        Q_ASSERT(HbStyleLoader::registerFilePath(KDialerWidgetWidgetml));
        Q_ASSERT(HbStyleLoader::registerFilePath(KDialerWidgetCss));
        // Engine construction is 2 phased 
        m_engine = new DialerWidgetEngine();
        connect(m_engine, SIGNAL( exceptionOccured(const int&) )
                ,this, SLOT( onEngineException(const int&) ) );
        
        if(!m_engine->initialize()){
            //engine construction failed. Give up.
            emit error();
            return;
            }
        connect( m_engine, SIGNAL(missedCallsCountChanged(const int&)),
                this, SLOT(onMissedCallsCountChange(const int&)));
    }
    QT_CATCH(...){
        emit error();
    }
}

/*!
    \fn void DialerWidget::onShow()

    Shows the widget
*/
void DialerWidget::onShow()
{
    PHONE_TRACE
    updatePrimitives();
}

/*!
    \fn void DialerWidget::onHide()

    Hides the widget
*/
void DialerWidget::onHide()
{
    PHONE_TRACE
}

void DialerWidget::onUninitialize()
{
    PHONE_TRACE
    HbStyleLoader::unregisterFilePath(KDialerWidgetWidgetml);
    HbStyleLoader::unregisterFilePath(KDialerWidgetCss);
}

void DialerWidget::onEngineException(const int& exc)
{
    Q_UNUSED(exc);
    emit error();
}

void DialerWidget::onMissedCallsCountChange(const int& count)
{
    m_text->setText( QLocale::system().toString(count));
    if ( count ){
        m_text->setVisible(true);
        m_badgeBackground->setVisible(true);
    } else {
        m_text->setVisible(false);
        m_badgeBackground->setVisible(false);
    }
}

void DialerWidget::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    PHONE_TRACE;
    Q_UNUSED(event)
    setBackgroundToPressed();
}

void DialerWidget::handleMouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (contains(event->pos())) {
        setBackgroundToPressed();
    } else {
        setBackgroundToNormal();
    }
}

void DialerWidget::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    PHONE_TRACE;
    Q_UNUSED(event);
    HbInstantFeedback::play(HbFeedback::Basic);
    setBackgroundToNormal();
    startDialer();    
}

bool DialerWidget::sceneEvent(QEvent *event)
{
    if (event->type() == QEvent::UngrabMouse) {
        setBackgroundToNormal();
    }

    return HsWidget::sceneEvent(event);;
}

HsWidget::StartResult DialerWidget::onStart()
{
    return StartResultRunning;
}
HsWidget::StopResult DialerWidget::onStop()
{
    return StopResultFinished;
}
HsWidget::SuspendResult DialerWidget::onSuspend()
{
    return SuspendResultSuspended;
}
HsWidget::ResumeResult DialerWidget::onResume()
{
    return ResumeResultRunning;
}

void DialerWidget::createPrimitives()
{   
    setPreferredSize(100,100);
    // Background
    if (!m_background) {
        HbFrameDrawer *drawer = new HbFrameDrawer(
            KDialerWidgetIconNormal, HbFrameDrawer::OnePiece);
        m_background = new HbFrameItem(drawer, this);
        style()->setItemName(m_background, QLatin1String("background"));
        m_background->moveBy(0,10);
        m_background->resize(81,81);
    }
    
    // Badge background
    if (!m_badgeBackground) {
        HbFrameDrawer *badgedrawer = new HbFrameDrawer(
            KMissedCallShortcutBadge, HbFrameDrawer::ThreePiecesHorizontal);
        m_badgeBackground = new HbFrameItem(badgedrawer, this);
        style()->setItemName(m_background, QLatin1String("badgeBackground"));
        m_badgeBackground->resize(20,20);
        m_badgeBackground->moveBy(70,0);
        m_badgeBackground->setVisible(true);
        m_badgeBackground->setVisible( false );
    }

    // Text
    if (!m_text) {
        m_text = new HbTextItem(this);
        style()->setItemName(m_text, QLatin1String("text"));
        m_text->resize(20,20);
        m_text->moveBy(76,0);
        m_text->setVisible(true);
        HbFontSpec *textFont = new HbFontSpec(HbFontSpec::Primary);
        textFont->setTextHeight(3*HbDeviceProfile::current().unitValue());
        m_text->setFontSpec(*textFont);
        m_text->setText("0");
        m_text->setVisible( false);
    }

    // Touch Area
    if (!m_touchArea) {
        m_touchArea = new HbTouchArea(this);
        m_touchArea->installEventFilter(this);
        style()->setItemName(m_touchArea, QLatin1String("touch_area"));
        m_touchArea->moveBy(0,10);
        m_touchArea->resize(81,81);
    }
}

void DialerWidget::setBackgroundToNormal()
{
    if (m_background) {
        m_background->frameDrawer().
            setFrameGraphicsName(KDialerWidgetIconNormal);
    }
}

void DialerWidget::setBackgroundToPressed()
{
    if (m_background) {
        m_background->frameDrawer().
            setFrameGraphicsName(KDialerWidgetIconPressed);
    }
}

