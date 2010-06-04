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
#include <hbiconanimationmanager.h>
#include <hbiconanimator.h>

#include "bubbleheadingwidget.h"
#include "bubblemanager2.h"
#include "bubbleutils.h"
#include "bubbleheader.h"

BubbleHeadingWidget::BubbleHeadingWidget(QGraphicsItem* item)
    : HbWidget(item), mStatusIcon(0), mNumberTypeIcon(0),
      mCipheringIcon(0), mText1(0), mText2(0), mText3(0)
{
    createPrimitives();

    HbStyleLoader::registerFilePath(":/bubbleheadingwidget.css");
    HbStyleLoader::registerFilePath(":/bubbleheadingwidget.widgetml");

    // font is update in code, because cli position is changing
    mCliFont = new HbFontSpec(HbFontSpec::Primary);
    mTextFont = new HbFontSpec(HbFontSpec::Secondary);

    HbDeviceProfile profile;
    mCliFont->setTextHeight(4*HbDeviceProfile::current().unitValue());
    mTextFont->setTextHeight(4*HbDeviceProfile::current().unitValue());

    HbIconAnimationManager *mgr = HbIconAnimationManager::global();
    mgr->addDefinitionFile(":/bubble_icon_anim.axml");
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
    mStatusIcon->hide();
    mStatusIcon->animator().stopAnimation();
    mNumberTypeIcon->hide();
    mCipheringIcon->hide();
}

void BubbleHeadingWidget::readBubbleHeader(const BubbleHeader& header)
{
    mHeader = &header;
    updatePrimitives();
}

void BubbleHeadingWidget::createPrimitives()
{
    mText1 = new HbTextItem(this);
    style()->setItemName( mText1, "text_line_1" );

    mText2 = new HbTextItem(this);
    style()->setItemName( mText2, "text_line_2" );

    mText3 = new HbTextItem(this);
    style()->setItemName( mText3, "text_line_3" );

    //mStatusIcon = new BubbleAnimIconItem(BUBBLE_ICON_ANIM_INTERVAL, this);
    mStatusIcon = new HbIconItem(this);
    style()->setItemName( mStatusIcon, "status_icon" );

    mNumberTypeIcon = new HbIconItem(this);
    style()->setItemName( mNumberTypeIcon, "number_type_icon" );

    mCipheringIcon = new HbIconItem(this);
    style()->setItemName( mCipheringIcon, "ciphering_icon" );
}

void BubbleHeadingWidget::updatePrimitives()
{
    if (mHeader!=0) {
        BubbleUtils::setCallStatusIcon(
            mHeader->callState(), mHeader->callFlags(), *mStatusIcon);

        BubbleUtils::setNumberTypeIcon(
            mHeader->callState(), mHeader->callFlags(), *mNumberTypeIcon);

        BubbleUtils::setCipheringIcon(
            mHeader->callState(), mHeader->callFlags(), *mCipheringIcon);

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

void BubbleHeadingWidget::polishEvent()
{
    if (lines == 3) {
        setLayout("three_lines");
        mText1->setVisible(true);
        mText2->setVisible(true);
        mText3->setVisible(true);
    } else if (lines == 2) {
        setLayout("two_lines");
        mText1->setVisible(true);
        mText2->setVisible(true);
        mText3->setVisible(false);
    } else if (lines == 1) {
        setLayout("one_line");
        mText1->setVisible(true);
        mText2->setVisible(true);
        mText3->setVisible(false);
    }

    HbWidget::polishEvent();
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

void BubbleHeadingWidget::changeEvent(QEvent *event)
{
    if (event->type() == HbEvent::ThemeChanged) {
        updatePrimitives();
    }

    HbWidget::changeEvent(event);
}

void BubbleHeadingWidget::showEvent(QShowEvent *event)
{
    Q_UNUSED(event)
    mStatusIcon->animator().startAnimation();
}
