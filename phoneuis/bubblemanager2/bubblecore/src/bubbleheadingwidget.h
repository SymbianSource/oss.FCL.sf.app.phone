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
* Description: Bubble heading widget
*
*/

#ifndef BUBBLEHEADINGWIDGET_H
#define BUBBLEHEADINGWIDGET_H

#include <hbwidget.h>

class HbIconItem;
class HbTextItem;
class HbFontSpec;
class BubbleHeader;

class BubbleHeadingWidget : public HbWidget
{
    Q_OBJECT
    Q_PROPERTY(int lines READ lineCount WRITE setLineCount)
    Q_PROPERTY(QString layoutOption READ layout WRITE setLayout)

public:
    BubbleHeadingWidget(QGraphicsItem* item=0);
    virtual ~BubbleHeadingWidget();

    int lineCount() const;
    void setLineCount(int count);

    QString layout() const;
    void setLayout(const QString& layout);

    void reset();
    void readBubbleHeader(const BubbleHeader& header);
    void updateTimerDisplayNow();

private:
    void createPrimitives();
    void updatePrimitives();

protected:
    void polish(HbStyleParameters &params);

private:
    int lines;
    QString layoutOption;

    const BubbleHeader* mHeader;

    // primitives
    HbIconItem* mIndicator1;
    HbIconItem* mIndicator2;
    HbTextItem* mText1;
    HbTextItem* mText2;
    HbTextItem* mText3;
    int mCallTimerTextLine;
    HbFontSpec* mCliFont;
    HbFontSpec* mTextFont;
};

#endif // BUBBLEHEADINGWIDGET_H
