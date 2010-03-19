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

#ifndef BUBBLEANIMICONITEM_H_
#define BUBBLEANIMICONITEM_H_

#include <QObject>
#include <hbiconitem.h>

class BubbleAnimIconItem : public HbIconItem
    {
    Q_OBJECT // for timer
    
public:
    explicit BubbleAnimIconItem( int interval, QGraphicsItem* parent = 0 );
    virtual ~BubbleAnimIconItem();
    
    void appendIcon( const QString& iconName );
    void reset();

protected:
    void startAnimation(int interval);
    void stopAnimation();
    void timerEvent(QTimerEvent *event);
    void paint (QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
    QVariant itemChange ( GraphicsItemChange change, const QVariant & value ); 
    void resizeEvent( QGraphicsSceneResizeEvent * event );
    
protected:
    int     mInterval;
    int     mAnimationTimerId;
    int     mCurrentFrame;
    QList<HbIcon*> mFrames;
    };

#endif /* BUBBLEANIMICONITEM_H_ */
