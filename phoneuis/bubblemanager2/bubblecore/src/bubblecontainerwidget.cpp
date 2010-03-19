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
* Description: Bubble container.
*
*/

#include <QtGui>
#include <hbstyle.h>
#include <hbframeitem.h>
#include <hbframedrawer.h>
#include "bubblecontainerwidget.h"
#include "bubbleprimitives.h"
#include "bubblestyleoption.h"

BubbleContainerWidget::BubbleContainerWidget(
    const QString& stylePluginName, QGraphicsItem* item)
    : HbWidget(item), mStylePluginName(stylePluginName),
      mBackground(0)
{
    setPluginBaseId(style()->registerPlugin(mStylePluginName));
    Q_ASSERT(pluginBaseId()!=-1);

    createPrimitives();
    updatePrimitives();
}

BubbleContainerWidget::~BubbleContainerWidget()
{
    style()->unregisterPlugin(mStylePluginName);
}

void BubbleContainerWidget::createPrimitives()
{
    QGraphicsItem *background = style()->createPrimitive(
        (HbStyle::Primitive)(pluginBaseId()+BP_Bubble_frame), this);
    style()->setItemName(mBackground, "background");

    delete mBackground;
    mBackground =
        qgraphicsitem_cast<HbFrameItem*>(background);
}

void BubbleContainerWidget::updatePrimitives()
{
    if (mBackground) {
        BubbleStyleOption option;
        style()->updatePrimitive(
            mBackground, (HbStyle::Primitive)(pluginBaseId()+BP_Bubble_frame),
            &option);
        mBackground->setVisible(false); // background in drawn in paint()
    }
}

void BubbleContainerWidget::mousePressEvent(
    QGraphicsSceneMouseEvent * event)
{
    if (event->button() != Qt::LeftButton) {
        event->ignore();
        return;
    }

    mPressed = true;
    event->accept();
}

void BubbleContainerWidget::mouseMoveEvent(
    QGraphicsSceneMouseEvent *event)
{
    if (!(event->buttons() & Qt::LeftButton)) {
        event->ignore();
        return;
    }

    if ( !rect().contains(event->pos()) && mPressed ) {
        ungrabMouse();
        mPressed = false;
    }
}

void BubbleContainerWidget::mouseReleaseEvent(
    QGraphicsSceneMouseEvent * event)
{
    if (event->button() != Qt::LeftButton) {
        event->ignore();
        return;
    }

    if (mPressed) {
        emit clicked();
        mPressed = false;
        event->accept();
    }
}

void BubbleContainerWidget::paint(
    QPainter *painter,
    const QStyleOptionGraphicsItem *option,
    QWidget *widget)
{
    Q_UNUSED(widget)
    Q_UNUSED(option)

    if (mBackground) {
        mBackground->frameDrawer().paint(painter,boundingRect());
    }
}

