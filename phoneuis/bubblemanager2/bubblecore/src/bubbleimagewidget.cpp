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
*
*/

#include <hblabel.h>
#include "bubbleimagewidget.h"

BubbleImageWidget::BubbleImageWidget(QGraphicsItem* parent)
    : HbWidget(parent), mLabel(new HbLabel(this))
{
    mLabel->setAlignment(Qt::AlignCenter);
    mLabel->setAspectRatioMode(Qt::KeepAspectRatioByExpanding);
}

BubbleImageWidget::~BubbleImageWidget()
{
}

void BubbleImageWidget::setImage(const QString& name)
{
    if (name != mImageName) {
        HbIcon icon(name);
        mLabel->setIcon(icon);
    }
}

void BubbleImageWidget::resizeEvent(
    QGraphicsSceneResizeEvent *event)
{
    Q_UNUSED(event);
    // set geometry without layout manager, because
    // image size is not defined in docml
    mLabel->setGeometry(rect());
}
