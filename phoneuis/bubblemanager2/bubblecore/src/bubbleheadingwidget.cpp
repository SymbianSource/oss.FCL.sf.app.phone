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

#include <hbstyle.h>
#include <hbmainwindow.h>
#include <hbevent.h>
#include <hbiconitem.h>
#include <hbtextitem.h>
#include <hbfontspec.h>
#include <hbdeviceprofile.h>
#include <hbstyleloader.h>

#include "bubbleheadingwidget.h"
#include "bubblemanager2.h"
#include "bubbleutils.h"
#include "bubbleheader.h"

BubbleHeadingWidget::BubbleHeadingWidget(QGraphicsItem* item)
    : HbWidget(item), mIndicator1(0), mIndicator2(0),
      mText1(0), mText2(0), mText3(0)
{
    createPrimitives();

    HbStyleLoader::registerFilePath(
        QLatin1String(":/bubbleheadingwidget.css"));
    HbStyleLoader::registerFilePath(
        QLatin1String(":/bubbleheadingwidget.widgetml"));

    // font is update in code, because cli position is changing
    mCliFont = new HbFontSpec(HbFontSpec::Primary);
    mTextFont = new HbFontSpec(HbFontSpec::Secondary);

    HbDeviceProfile profile;
    mCliFont->setTextHeight(4*HbDeviceProfile::current().unitValue());
    mTextFont->setTextHeight(4*HbDeviceProfile::current().unitValue());
}

BubbleHeadingWidget::~BubbleHeadingWidget()
{
    delete mCliFont;
    delete mTextFont;
}

void BubbleHeadingWidget::reset()
{
    mHeader = 0;
    mText1->setText(QString());
    mText2->setText(QString());
    mText3->setText(QString());
    mIndicator1->hide();
    mIndicator2->hide();
}

void BubbleHeadingWidget::readBubbleHeader(const BubbleHeader& header)
{
    mHeader = &header;
    updatePrimitives();
}

void BubbleHeadingWidget::createPrimitives()
{
    mText1 = new HbTextItem(this);
    style()->setItemName( mText1, QLatin1String("text_line_1"));

    mText2 = new HbTextItem(this);
    style()->setItemName( mText2, QLatin1String("text_line_2"));

    mText3 = new HbTextItem(this);
    style()->setItemName( mText3, QLatin1String("text_line_3"));

    mIndicator1 = new HbIconItem(this);
    style()->setItemName( mIndicator1, QLatin1String("indicator_icon_1"));

    mIndicator2 = new HbIconItem(this);
    style()->setItemName( mIndicator2, QLatin1String("indicator_icon_2"));
}

void BubbleHeadingWidget::updatePrimitives()
{
    if (mHeader!=0) {
        BubbleUtils::setIndicators(mHeader->callState(),
                                   mHeader->callFlags(),
                                   *mIndicator1,
                                   *mIndicator2);

        // update text lines
        int cliLine = 0;

        if (lines==3) {
            BubbleUtils::setCallHeaderTexts3Lines(
                *mHeader, *mText1, *mText2, *mText3, cliLine,
                mCallTimerTextLine );
        } else if (lines==2) {
            BubbleUtils::setCallHeaderTexts2Lines(
                *mHeader, *mText1, *mText2, cliLine,
                mCallTimerTextLine );
        } else {
            // todo: 1-line
            BubbleUtils::setCallHeaderTexts2Lines(
                *mHeader, *mText1, *mText2, cliLine,
                mCallTimerTextLine );
        }

        // update font
        if (cliLine==2) {
            mText1->setFontSpec(*mTextFont);
            mText2->setFontSpec(*mCliFont);
            mText3->setFontSpec(*mTextFont);
        } else {
            mText1->setFontSpec(*mCliFont);
            mText2->setFontSpec(*mTextFont);
            mText3->setFontSpec(*mTextFont);
        }
    }

    repolish();
}

void BubbleHeadingWidget::polish(HbStyleParameters &params)
{
    QString layout;

    if (lines == 3) {
        layout = QLatin1String("three_lines");
        mText1->setVisible(true);
        mText2->setVisible(true);
        mText3->setVisible(true);
    } else if (lines == 2) {
        layout = QLatin1String("two_lines");
        mText1->setVisible(true);
        mText2->setVisible(true);
        mText3->setVisible(false);
    } else if (lines == 1) {
        layout = QLatin1String("one_line");
        mText1->setVisible(true);
        mText2->setVisible(true);
        mText3->setVisible(false);
    }

    if (mIndicator2->isVisible()) {
        layout.append(QLatin1String("_2"));
    } else if (mIndicator1->isVisible()) {
        layout.append(QLatin1String("_1"));
    }

    setLayout(layout);

    HbWidget::polish(params);
}

int BubbleHeadingWidget::lineCount() const
{
    return lines;
}

void BubbleHeadingWidget::setLineCount(int count)
{
    lines = count;
    if (isVisible()) {
        repolish();
    }
}

QString BubbleHeadingWidget::layout() const
{
    return layoutOption;
}

void BubbleHeadingWidget::setLayout(const QString& layout)
{
    layoutOption = layout;
}

void BubbleHeadingWidget::updateTimerDisplayNow()
{
    if ( mHeader && ( mHeader->callState() == BubbleManager::Active ) ) {
        if ( mCallTimerTextLine == 2 ) {
            mText2->setText(mHeader->timerCost());
            mText2->setElideMode(Qt::ElideRight);
        } else if ( mCallTimerTextLine == 3 ) {
            mText3->setText(mHeader->timerCost());
            mText3->setElideMode(Qt::ElideRight);
        }
    }
}
