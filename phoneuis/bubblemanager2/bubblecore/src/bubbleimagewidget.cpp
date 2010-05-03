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
* Description:  Widget to display contact image.
*               Displays default avatar, when image is not set.
*
*/

#include <QPainter>
#include <hbstyle.h>
#include "bubbleimagemanager.h"
#include "bubbleimagewidget.h"
#include "bubbleprimitives.h"
#include "bubblestyleoption.h"

BubbleImageWidget::BubbleImageWidget(const QString& stylePluginName,
    BubbleImageManager& imageManager, QGraphicsItem* parent)
    : HbWidget(parent), mStylePluginName(stylePluginName),
      mImageManager(imageManager), mDefaultAvatar(0)
{
    setPluginBaseId(style()->registerPlugin(mStylePluginName));
    Q_ASSERT(pluginBaseId()!=-1);

    // create avatar
    delete mDefaultAvatar;
    mDefaultAvatar = style()->createPrimitive(
        (HbStyle::Primitive)(pluginBaseId()+BP_DefaultAvatar_icon), this);
    style()->setItemName( mDefaultAvatar, "default_avatar" );

    connect(&mImageManager,SIGNAL(pixmapReady(QString)),
            this,SLOT(imageLoaded(QString)));
}

BubbleImageWidget::~BubbleImageWidget()
{
    style()->unregisterPlugin(mStylePluginName);
}

void BubbleImageWidget::setImage(const QString& name)
{
    mImageName = name;

    if (!mImageName.isEmpty()) {
        mDefaultAvatar->setVisible(false);
        mImageManager.loadImage(mImageName);
    } else {
        BubbleStyleOption option;
        style()->updatePrimitive(
                mDefaultAvatar,
                (HbStyle::Primitive)(pluginBaseId()+BP_DefaultAvatar_icon),
                 &option);
        mDefaultAvatar->setVisible(true);
    }
}

void BubbleImageWidget::imageLoaded(QString imageFileName)
{
    if (imageFileName==mImageName) {
        update(); // does nothing if invisible
    }
}

void BubbleImageWidget::paint(
    QPainter* painter,
    const QStyleOptionGraphicsItem* option,
    QWidget* widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    
    if (mImageName.isEmpty())
        return;

    const QPixmap* pixmap =
        mImageManager.pixmap(mImageName);

    if (pixmap) {
        // up or downscales images to fill image area
        QSize imageSize(pixmap->size());
        QSize sourceSize(rect().size().toSize());
        sourceSize.scale(imageSize,Qt::KeepAspectRatio);
        QRect sourceRect(QPoint(0,0),sourceSize);
        int xOff = (imageSize.width() - sourceSize.width())/2;
        int yOff = (imageSize.height() - sourceSize.height())/2;
        sourceRect.moveLeft(xOff);
        sourceRect.moveTop(yOff);
        // qDebug() << "imageSize:" << imageSize;
        // qDebug() << "sourceRect:" << sourceRect;
        painter->setRenderHint(QPainter::SmoothPixmapTransform);

#ifdef __WINS__
        // sourceRect crashes emulator, RC ou1cimx1#320113
        painter->drawPixmap(rect().toRect(),*pixmap);
#else        
        painter->drawPixmap(rect().toRect(),*pixmap,sourceRect);
#endif        
        
    }
}
