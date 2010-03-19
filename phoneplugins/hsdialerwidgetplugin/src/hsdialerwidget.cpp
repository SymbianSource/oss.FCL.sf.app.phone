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

#include <QGraphicsLinearLayout>
#include <HbIconItem>
#include "hsdialerwidget.h"
#ifdef Q_OS_SYMBIAN
#include <xqservicerequest.h>
#include <XQSettingsManager>
#include <xqpublishandsubscribeutils.h>
#include <ctsydomainpskeys.h>
#include <logsservices.h>
#endif

namespace
{
    const char KDialerWidgetIcon[] = "qtg_graf_hs_dialer";
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
    mSetManager(0)
{
    HbIconItem *iconItem = new HbIconItem;
    HbIcon icon(KDialerWidgetIcon);
    iconItem->setIcon(icon);

    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout;
    layout->addItem(iconItem);
    setLayout(layout);

    setPreferredSize(icon.size());
    resize(preferredSize());
}

/*!
    Destructor.
*/
HsDialerWidget::~HsDialerWidget()
{
#ifdef Q_OS_SYMBIAN
    delete mSetManager;
#endif
}

/*!
    \fn void HsDialerWidget::startDialer()

    Starts dialer widget via view activation service.
*/
void HsDialerWidget::startDialer()
{
#ifdef Q_OS_SYMBIAN
    const quint32 KKCTsyCallState = 0x00000001;
    const qint32 KKPSUidCtsyCallInformation = {0x102029AC};
    if  (!mSetManager) {
        mSetManager = new XQSettingsManager();
        }

    XQPublishAndSubscribeSettingsKey callStateKey(KKPSUidCtsyCallInformation, KKCTsyCallState);
    int callState = mSetManager->readItemValue(callStateKey).toInt();
    if( (callState != EPSCTsyCallStateNone ) && (callState != EPSCTsyCallStateUninitialized) ) {
        int dialer(1);

        // ToDo: should telephone + dialpad be opened when there is a waiting call? 
        //if (callState == EPSCTsyCallStateRinging) {
        //dialer = 0;
        //}
        XQServiceRequest snd("com.nokia.services.telephony.uistarter", "start(int)", false);
        snd << dialer;
        QVariant retValue;
        snd.send(retValue);
    }
    else {
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
