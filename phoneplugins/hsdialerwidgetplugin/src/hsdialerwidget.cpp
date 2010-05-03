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


#include <QPainter>
#include <hbframedrawer.h>
#include <hbframeitem.h>
#include <hbtextitem.h>
#ifdef Q_OS_SYMBIAN
#include <logsdomaincrkeys.h>
#include "qtphonelog.h"
#include <xqservicerequest.h>
#include <xqcallinfo.h>
#include <xqpublishandsubscribeutils.h>
#include <ctsydomainpskeys.h>
#include <logsservices.h>
#endif

#include "hsdialerwidget.h"

namespace
{
    const char KDialerWidgetIcon[] = "qtg_graf_hs_dialer";
    const char KMissedCallShortcutBadge[] = ":/icons/resource/qtg_fr_shortcut_badge_bg";
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
  : HbWidget(parent, flags),
    mXQCallInfo(0)
{
#ifdef Q_OS_SYMBIAN
    m_setManager = new XQSettingsManager(this);

    XQSettingsKey settingsKey( XQSettingsKey::TargetCentralRepository,
            KCRUidLogs.iUid, KLogsNewMissedCalls );

    bool ok = connect( m_setManager,
            SIGNAL( valueChanged(const XQSettingsKey & ,const QVariant &)),
                this, SLOT(updateMissedCallBadge(XQSettingsKey,
                        QVariant)));
    ASSERT( ok );
    ok = false;
    ok = m_setManager->startMonitoring( settingsKey );
    ASSERT(ok);

#endif

    HbFrameDrawer *drawer = new HbFrameDrawer(
            KDialerWidgetIcon, HbFrameDrawer::OnePiece);
    m_backgroud = new HbFrameItem(drawer, this);
    m_backgroud->setZValue(1);

    drawer = new HbFrameDrawer(KMissedCallShortcutBadge,
            HbFrameDrawer::ThreePiecesHorizontal );

    m_shortcutBadge = new HbFrameItem(drawer, this);
    m_shortcutBadge->setZValue(2);
    m_shortcutBadge->setVisible(true);
    m_missedCalls=2;
    setItemPositions();
    resize(preferredSize());
}

/*!
    Destructor.
*/
HsDialerWidget::~HsDialerWidget()
{
}

void HsDialerWidget::paint(
        QPainter *painter,
        const QStyleOptionGraphicsItem *option,
        QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setPen(QPen( Qt::transparent ) );

    QRectF rect(m_backgroud->boundingRect());
    painter->drawRect((int)m_backgroud->pos().x(),
                      (int)m_backgroud->pos().y(),
                      rect.toRect().width(),
                      rect.toRect().height());

    if ( m_shortcutBadge->isVisible())
        {
        rect = m_shortcutBadge->boundingRect();
        HbTextItem *text = new HbTextItem( m_shortcutBadge->graphicsItem());
        
        QFont badgefont = font();
#ifdef Q_OS_SYMBIAN
        badgefont.setPointSize( 4 );
#else
        badgefont.setPointSize( 8 );
#endif
        text->setFont(badgefont);
        text->setText(QString::number(m_missedCalls));
        text->setZValue(3);
        text->setTextColor(Qt::white);
       // text->font().setPointSizeF(2);

        text->setX( ( rect.toRect().width() / 2 ) - 2);
        text->setY( ( rect.toRect().height() / 5 ) - 1);

        painter->drawRect( (int)m_shortcutBadge->pos().x(),
                           (int)m_shortcutBadge->pos().y(),
                           rect.toRect().width(),
                           rect.toRect().height() );
    }
}

/*!
    \fn void HsDialerWidget::startDialer()

    Starts dialer widget via view activation service.
*/
void HsDialerWidget::startDialer()
{
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
        snd << (int)LogsServices::ViewAll;
        snd << true;
        int retValue;
        snd.send(retValue);

        //LogsServices::start(LogsServices::ViewAll, true);
    }
#endif
}

void HsDialerWidget::updateMissedCallBadge(
        const XQSettingsKey &key, const QVariant &value)
{
#ifdef Q_OS_SYMBIAN
    if ( key.key() == (quint32)KLogsNewMissedCalls ){
        m_missedCalls = value.toInt();
        m_shortcutBadge->setVisible(value.toBool());
        setItemPositions();
        m_backgroud->update();
    }
#else
   Q_UNUSED(key);
   Q_UNUSED(value);
#endif
}

/*!
    \fn void HsDialerWidget::onShow()

    Shows the widget
*/
void HsDialerWidget::onShow()
{
}

/*!
    \fn void HsDialerWidget::onHide()

    Hides the widget
*/
void HsDialerWidget::onHide()
{
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

void HsDialerWidget::setItemPositions()
{
    HbIcon icon(KDialerWidgetIcon);
    setPreferredWidth( icon.width());
    int badgeSize = 20;
    if ( m_shortcutBadge->isVisible()){
        m_backgroud->setGeometry(QRectF(QPointF(0,badgeSize / 2 ), 
                QSizeF(icon.width(),icon.height())));
        m_shortcutBadge->setGeometry(QRectF(
                QPointF(m_backgroud->boundingRect().width() - (badgeSize / 2),0 ),
                QSizeF(badgeSize, badgeSize)));
        setPreferredHeight(icon.height()+(badgeSize / 2));
    } else {
        m_backgroud->setGeometry(QRectF(QPointF(0,0), 
                QSizeF(icon.width(),icon.height())));
        setPreferredHeight(icon.height());
    }
}
