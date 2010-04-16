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
#include "bubbleheadingwidget.h"
#include "bubbleprimitives.h"
#include "bubblestyleoption.h"
#include "bubblemanager2.h"
#include "bubbleutils.h"
#include "bubbleheader.h"

BubbleHeadingWidget::BubbleHeadingWidget(
    const QString& stylePluginName, QGraphicsItem* item)
    : HbWidget(item), mStylePluginName(stylePluginName), mStatusIcon(0),
      mNumberTypeIcon(0), mCipheringIcon(0), mText1(0), mText2(0), mText3(0)
{
    setPluginBaseId(style()->registerPlugin(mStylePluginName));
    Q_ASSERT(pluginBaseId()!=-1);

    createPrimitives();
}

BubbleHeadingWidget::~BubbleHeadingWidget()
{
    style()->unregisterPlugin(mStylePluginName);
}

void BubbleHeadingWidget::reset()
{
    mHeader = 0;
}

void BubbleHeadingWidget::readBubbleHeader(const BubbleHeader& header)
{
    mHeader = &header;
    updatePrimitives();
}

void BubbleHeadingWidget::createPrimitives()
{
    delete mText1;
    mText1 = style()->createPrimitive(
            (HbStyle::Primitive)(pluginBaseId()+BP_Text1_text), this);
    style()->setItemName( mText1, "text_line_1" );

    delete mText2;
    mText2 = style()->createPrimitive(
            (HbStyle::Primitive)(pluginBaseId()+BP_Text2_text), this);
    style()->setItemName( mText2, "text_line_2" );

    delete mText3;
    mText3 = style()->createPrimitive(
            (HbStyle::Primitive)(pluginBaseId()+BP_Text3_text), this);
    style()->setItemName( mText3, "text_line_3" );

    delete mNumberTypeIcon;
    mNumberTypeIcon = style()->createPrimitive(
            (HbStyle::Primitive)(pluginBaseId()+BP_NumberType_icon), this);
    style()->setItemName( mNumberTypeIcon, "number_type_icon" );

    delete mCipheringIcon;
    mCipheringIcon = style()->createPrimitive(
            (HbStyle::Primitive)(pluginBaseId()+BP_Ciphering_icon), this);
    style()->setItemName( mCipheringIcon, "ciphering_icon" );

    delete mStatusIcon;
    mStatusIcon = style()->createPrimitive(
            (HbStyle::Primitive)(pluginBaseId()+BP_CallStatus_icon), this);
    style()->setItemName( mStatusIcon, "status_icon" );
}

void BubbleHeadingWidget::updatePrimitives()
{
    BubbleStyleOption option;
    initStyleOption(option);

    if (mText1) {
        style()->updatePrimitive(
                mText1,
                (HbStyle::Primitive)(pluginBaseId()+BP_Text1_text),
                 &option);
    }

    if (mText2) {
        style()->updatePrimitive(
                mText2,
                (HbStyle::Primitive)(pluginBaseId()+BP_Text2_text),
                &option);
    }

    if (mText3) {
        style()->updatePrimitive(
                mText3,
                (HbStyle::Primitive)(pluginBaseId()+BP_Text3_text),
                &option);
    }

    if (mStatusIcon) {
        style()->updatePrimitive(
                mStatusIcon,
                (HbStyle::Primitive)(pluginBaseId()+BP_CallStatus_icon),
                &option);
    }

    if (mNumberTypeIcon) {
        style()->updatePrimitive(
                mNumberTypeIcon,
                (HbStyle::Primitive)(pluginBaseId()+BP_NumberType_icon),
                &option);
    }

    if (mCipheringIcon) {
        style()->updatePrimitive(
                mCipheringIcon,
                (HbStyle::Primitive)(pluginBaseId()+BP_Ciphering_icon),
                &option);
    }

    repolish();
}

void BubbleHeadingWidget::initStyleOption(BubbleStyleOption& option)
{
    HbWidget::initStyleOption(&option);

    if (mHeader!=0) {
        option.mCallState = mHeader->callState();
        option.mCallFlags = mHeader->callFlags();

        if (lines==3) {
            BubbleUtils::setCallHeaderTexts3Lines( *mHeader, option );
        } else if (lines==2) {
            BubbleUtils::setCallHeaderTexts2Lines( *mHeader, option );
        } else {
            // todo: 1-line
            BubbleUtils::setCallHeaderTexts2Lines( *mHeader, option );
        }

        mCallTimerTextLine = option.mTimerLineNumber;
    }
}

void BubbleHeadingWidget::polishEvent()
{
    if (mText1 && mText2 && mText3) {
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
        BubbleStyleOption option;

        if ( mCallTimerTextLine == 2 ) {
            option.mText2 = mHeader->timerCost();
            option.mText2Clip = Qt::ElideRight;
            option.mTimerLineNumber = mCallTimerTextLine;
            style()->updatePrimitive(
                    mText2,
                    (HbStyle::Primitive)(pluginBaseId()+BP_Text2_text),
                    &option);
        } else if ( mCallTimerTextLine == 3 ) {
            option.mText3 = mHeader->timerCost();
            option.mText3Clip = Qt::ElideRight;
            option.mTimerLineNumber = mCallTimerTextLine;
            style()->updatePrimitive(
                    mText3,
                    (HbStyle::Primitive)(pluginBaseId()+BP_Text3_text),
                    &option);
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

