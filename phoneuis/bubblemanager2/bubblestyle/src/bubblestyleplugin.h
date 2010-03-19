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
* Description:  Bubble style plugin.
*
*/

#ifndef BUBBLESTYLEPLUGIN_H
#define BUBBLESTYLEPLUGIN_H

#include <QtPlugin>

#include <hbstyleinterface.h>
#include <hbglobal.h>

class HbFontSpec;
class HbTextItem;
class BubbleStyleOption;
class BubbleAnimIconItem;

class BubbleStylePlugin: public QObject, public HbStyleInterface
{
    Q_OBJECT
    Q_INTERFACES(HbStyleInterface)

public:
    BubbleStylePlugin();
    virtual ~BubbleStylePlugin();

    int primitiveCount() const;
    HbWidgetBase *createPrimitive( HbStyle::Primitive primitive, QGraphicsItem *parent = 0 ) const;
    void updatePrimitive( QGraphicsItem *item, HbStyle::Primitive primitive, const QStyleOption *option ) const;
    QString layoutPath() const;
    void setFont(HbTextItem* item, int primitive, const BubbleStyleOption *option) const;

private:
    void setCallStatusIcons(BubbleAnimIconItem* anim, const BubbleStyleOption *opt) const;
    
private:
    HbFontSpec* mCliFont;
    HbFontSpec* mTextFont;
    HbFontSpec* mTimerFont;
};

#endif // BUBBLESTYLEPLUGIN_H
