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
* Description:  Animated icon.
*
*/

#include <QTimer>
#include <QGraphicsSceneResizeEvent>
#include "bubbleanimiconitem.h"

BubbleAnimIconItem::BubbleAnimIconItem( int interval, QGraphicsItem* parent )
    : HbIconItem( parent ), mInterval(interval), mAnimationTimerId(0)
{
    reset();
}

BubbleAnimIconItem::~BubbleAnimIconItem()
{
    reset();
}

void BubbleAnimIconItem::appendIcon( const QString& iconName )
{
    HbIcon* icon = new HbIcon(iconName);
    icon->setSize(rect().size());
    mFrames.append(icon);
    if (mFrames.count()==1) {
        setIcon(*icon); // initial frame
    }
}

void BubbleAnimIconItem::startAnimation(int interval)
{
    mAnimationTimerId = startTimer(interval);
}

void BubbleAnimIconItem::stopAnimation()
{
     if ( mAnimationTimerId ) {
         killTimer(mAnimationTimerId);
         mAnimationTimerId = 0;
     }
}

void BubbleAnimIconItem::reset()
{
    stopAnimation();
    foreach (HbIcon* icon, mFrames) {
        delete icon;
    }
    mFrames.clear();
    mCurrentFrame = 0;
}

void BubbleAnimIconItem::timerEvent(QTimerEvent *event)
{
   Q_UNUSED(event)
   
   mCurrentFrame = (mCurrentFrame + 1) % mFrames.count();
   setIcon(*mFrames.at(mCurrentFrame));
   update();
}

void BubbleAnimIconItem::paint(
    QPainter *painter,
    const QStyleOptionGraphicsItem *option,
    QWidget *widget )
{
    HbIconItem::paint( painter, option, widget );

    if ( mFrames.count() && mAnimationTimerId == 0 ) {
        startAnimation(mInterval); // begin animation
    }
}

QVariant BubbleAnimIconItem::itemChange(
    GraphicsItemChange change,
    const QVariant& value )
{
    if (change==QGraphicsItem::ItemVisibleHasChanged) {
        if (mAnimationTimerId && value.toBool()==false) {
            stopAnimation(); // stop animation when going invisible
        }
    }

    return HbIconItem::itemChange( change, value );
}

void BubbleAnimIconItem::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    HbIconItem::resizeEvent(event);
    foreach (HbIcon* icon, mFrames) {
        icon->setSize(event->newSize());
    }
}
