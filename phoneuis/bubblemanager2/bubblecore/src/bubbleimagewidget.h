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

#ifndef BUBBLEIMAGEWIDGET_H_
#define BUBBLEIMAGEWIDGET_H_

#include <hbwidget.h>

class BubbleImageManager;
class HbIconItem;

class BubbleImageWidget : public HbWidget
    {
    Q_OBJECT
    
public:
    explicit BubbleImageWidget(
        BubbleImageManager& imageManager,
        QGraphicsItem* parent = 0 );
    virtual ~BubbleImageWidget();

    void setImage(const QString& name);

protected slots:
    void imageLoaded(QString imageFileName);

protected:
    void paint(
        QPainter* painter,
        const QStyleOptionGraphicsItem* option,
        QWidget* widget);

private:
    BubbleImageManager& mImageManager;
    QString  mImageName;
    HbIconItem* mDefaultAvatar;
    };

#endif /* BUBBLEIMAGEWIDGET_H_ */
