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

#include <HbStyle>
#include <HbStyleLoader>
#include <HbFrameItem>
#include <HbFrameDrawer>
#include <HbIconItem>
#include <HbTextItem>
#include <HbTouchArea>
#include "dialerwidgetengine.h"

#ifdef Q_OS_SYMBIAN
#include "qtphonelog.h"
#include <xqservicerequest.h>
#include <xqcallinfo.h>
#include <xqpublishandsubscribeutils.h>
#include <logsservices.h>
#include <xqrequestinfo.h>
#else
#define PHONE_TRACE
#define PHONE_TRACE1(A)
#define PHONE_TRACE2(A, B)
#define PHONE_TRACE3(A, B, C)
#define PHONE_TRACE4(A, B, C, D)
#endif

#include "hsdialerwidget.h"


namespace
{
    const char KDialerWidgetIcon[] = ":/icons/resource/qtg_graf_hs_dialer";
    const char KMissedCallShortcutBadge[] = ":/icons/resource/qtg_fr_shortcut_badge_bg";
    const char KDialerWidgetWidgetml[] = ":/data/resource/dialerwidget.widgetml";
    const char KDialerWidgetCss[] = ":/data/resource/dialerwidget.css";
}

/*!
    \class HsDialerWidget

    \ingroup group_hsdialerwidgetplugin
    \brief Implementation for the homescreen dialer launcher.

*/

/*!
    Constructs dialer widget with given \a parent and given window \a flags.
*/
HsDialerWidget::HsDialerWidget(QGraphicsItem *parent, Qt::WindowFlags flags)
  : HsWidget(parent, flags),
    mXQCallInfo(0), m_background(0), m_badgeBackground(0), m_text(0), m_touchArea(0)
{   
    PHONE_TRACE 
}

/*!
    Destructor.
*/
HsDialerWidget::~HsDialerWidget()
{
    PHONE_TRACE
}

/*!
    \fn void HsDialerWidget::startDialer()

    Starts dialer widget via view activation service.
*/
void HsDialerWidget::startDialer()
{
    PHONE_TRACE
#ifdef Q_OS_SYMBIAN
    PHONE_DEBUG("HsDialerWidget::startDialer");
    
    if (!mXQCallInfo) {
        mXQCallInfo = XQCallInfo::create();
        mXQCallInfo->setParent(this);
    }    
    QList<CallInfo> calls;
    mXQCallInfo->getCalls(calls);
        
    if( 0 < calls.count() ){
        PHONE_DEBUG("call ongoing, bring Telephone to foreground");
        
        // ToDo: should telephone + dialpad be opened when there is a waiting call?
        int dialer(1);
        XQServiceRequest snd("com.nokia.services.telephony.uistarter", "start(int)", false);
        snd << dialer;
        QVariant retValue;
        snd.send(retValue);
    }
    else {
        PHONE_DEBUG("no calls, open Dialer");
        XQServiceRequest snd("com.nokia.services.logsservices.starter",
            "start(int,bool)", false);
        XQRequestInfo info;
        info.setForeground(true);
        snd.setInfo(info);
        snd << (int)LogsServices::ViewAll;
        snd << true;
        int retValue;
        snd.send(retValue);

        //LogsServices::start(LogsServices::ViewAll, true);
    }
#endif

}

void HsDialerWidget::onInitialize()
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
    \fn void HsDialerWidget::onShow()

    Shows the widget
*/
void HsDialerWidget::onShow()
{
    PHONE_TRACE
    updatePrimitives();
}

/*!
    \fn void HsDialerWidget::onHide()

    Hides the widget
*/
void HsDialerWidget::onHide()
{
    PHONE_TRACE
}

void HsDialerWidget::onUninitialize()
{
    PHONE_TRACE
    HbStyleLoader::unregisterFilePath(KDialerWidgetWidgetml);
    HbStyleLoader::unregisterFilePath(KDialerWidgetCss);
}

void HsDialerWidget::onEngineException(const int& exc)
{
    Q_UNUSED(exc);
    emit error();
}

void HsDialerWidget::onMissedCallsCountChange(const int& count)
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

/*!
    \fn void HsDialerWidget::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)

    Dialer widget start is triggered from release \a event.
    \sa startDialer()
*/
void HsDialerWidget::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{   
    Q_UNUSED(event);
    startDialer();
}

HsWidget::StartResult HsDialerWidget::onStart()
{
    return StartResultRunning;
}
HsWidget::StopResult HsDialerWidget::onStop()
{
    return StopResultFinished;
}
HsWidget::SuspendResult HsDialerWidget::onSuspend()
{
    return SuspendResultSuspended;
}
HsWidget::ResumeResult HsDialerWidget::onResume()
{
    return ResumeResultRunning;
}

void HsDialerWidget::createPrimitives()
{   
    setPreferredSize(100,100);
    // Background
    if (!m_background) {
        HbFrameDrawer *drawer = new HbFrameDrawer(
                KDialerWidgetIcon, HbFrameDrawer::OnePiece);
        m_background = new HbFrameItem(drawer, this);
        style()->setItemName(m_background, /*QLatin1String(*/"background"/*)*/);
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
