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

class HbLabel;

class BubbleImageWidget : public HbWidget
    {
    Q_OBJECT
    
public:
    explicit BubbleImageWidget( QGraphicsItem* parent = 0 );
    virtual ~BubbleImageWidget();

    void setImage(const QString& name);

    void resizeEvent(
        QGraphicsSceneResizeEvent * event );
    
private:
    QString  mImageName;
    HbLabel* mLabel;
    };

#endif /* BUBBLEIMAGEWIDGET_H_ */
