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
* Description: List item for conference participant list.
*
*/

#include <QtGui>
#include <hbiconitem.h>
#include <hbframeitem.h>
#include <hbtextitem.h>
#include <hbiconitem.h>
#include <hbaction.h>
#include <hbpushbutton.h>
#include <hblabel.h>

#include "bubbleparticipantlistitem.h"
#include "bubbleprimitives.h"
#include "bubblestyleoption.h"
#include "bubblebuttonstyle.h"

BubbleParticipantListItem::BubbleParticipantListItem(
    const QString& stylePluginName,
    BubbleButtonStyle& buttonStyle,
    QGraphicsItem *parent) :
    HbListViewItem(parent),
    mStylePluginName(stylePluginName),
    mButtonStyle(buttonStyle),
    mBackground(0),
    mText(0),
    mStatusIcon(0),
    mCipheringIcon(0),
    mExpandIcon(0),
    mButton1(0),
    mButton2(0),
    mExpanded(false)
{
    mStylePluginBaseId = style()->registerPlugin(mStylePluginName);
    Q_ASSERT(pluginBaseId()!=-1);
}

BubbleParticipantListItem::~BubbleParticipantListItem()
{
    if (prototype()==(HbAbstractViewItem*)this) {
        style()->unregisterPlugin(mStylePluginName);
    }
}

HbAbstractViewItem* BubbleParticipantListItem::createItem()
{
    return new BubbleParticipantListItem(*this);
}

void BubbleParticipantListItem::updateChildItems()
{
    setPluginBaseId(mStylePluginBaseId);
    setFocusPolicy(Qt::ClickFocus); // to enable expanding

    // create primitives
    if (!mBackground) {
        mBackground = style()->createPrimitive(
                (HbStyle::Primitive)(pluginBaseId()+BP_Bubble_frame), this);
        if ( mBackground ) {
            style()->setItemName( mBackground, "background" );
        }
    }

    if (!mText) {
        mText = style()->createPrimitive(
                (HbStyle::Primitive)(pluginBaseId()+BP_Text1_text), this);
        if ( mText ) {
            style()->setItemName( mText, "text" );
        }
    }

    if (!mCipheringIcon) {
        mCipheringIcon = style()->createPrimitive(
                (HbStyle::Primitive)(pluginBaseId()+BP_Ciphering_icon), this);
        if ( mCipheringIcon ) {
            style()->setItemName( mCipheringIcon, "ciphering" );
        }
    }

    if (!mStatusIcon) {
        mStatusIcon = style()->createPrimitive(
                (HbStyle::Primitive)(pluginBaseId()+BP_CallStatus_icon), this);
        if ( mStatusIcon ) {
            style()->setItemName( mStatusIcon, "icon" );
        }
    }

    if (!mExpandIcon) {
        mExpandIcon = new HbIconItem(this);
        if ( mExpandIcon ) {
            style()->setItemName( mExpandIcon, "expand-indi" );
        }
    }

    BubbleParticipantListItem* p =
            static_cast<BubbleParticipantListItem*>(prototype());

    if (mExpanded && !mButton1) {
        Q_ASSERT(p->mActions.count()==2);
        HbAction* action = p->mActions.at(0);
        mButton1 = new HbPushButton(this);
        mButton1->setStyle(&mButtonStyle);
        mButton1->setIcon(action->icon());
        style()->setItemName( mButton1, "button-1" );
        HbTextItem* label = static_cast<HbTextItem*>(
            mButton1->primitive(HbStyle::P_PushButton_text));
        if (label) {
            label->setTextColor( Qt::white );
        }

        // connect first action
        connect(mButton1,
                SIGNAL(clicked()),
                action,
                SLOT(trigger()),
                Qt::QueuedConnection);
    } else {
        delete mButton1;
        mButton1 = 0;
    }

    if (mExpanded && !mButton2) {
        Q_ASSERT(p->mActions.count()==2);
        HbAction* action = p->mActions.at(1);
        mButton2 = new HbPushButton(this);
        mButton2->setStyle(&mButtonStyle);
        mButton2->setIcon(action->icon());
        style()->setItemName( mButton2, "button-2" );
        HbTextItem* label = static_cast<HbTextItem*>(
            mButton2->primitive(HbStyle::P_PushButton_text));
        if (label) {
            label->setTextColor( Qt::white );
        }

        // connect second action
        connect(mButton2,
                SIGNAL(clicked()),
                action,
                SLOT(trigger()),
                Qt::QueuedConnection );

    } else {
        delete mButton2;
        mButton2 = 0;
    }

    // update primitives
    BubbleStyleOption option;
    HbWidget::initStyleOption(&option);

    // check canConvert()
    option.mCallState = (BubbleManagerIF::PhoneCallState)
                         modelIndex().data(Qt::DecorationRole).toInt();
    option.mText1 = modelIndex().data(Qt::DisplayRole).toString();
    option.mCliLineNumber = 1;
    bool ciphering = modelIndex().data(Qt::StatusTipRole).toBool();
    if (!ciphering) {
        option.mCallFlags = BubbleManagerIF::NoCiphering;
    }

    if (mBackground) {
        style()->updatePrimitive(
            mBackground,
            (HbStyle::Primitive)(pluginBaseId()+BP_Bubble_frame),
            &option);
    }

    if (mText) {
        style()->updatePrimitive(
            mText,
            (HbStyle::Primitive)(pluginBaseId()+BP_Text1_text),
            &option);
    }

    if (mStatusIcon) {
        style()->updatePrimitive(
            mStatusIcon,
            (HbStyle::Primitive)(pluginBaseId()+BP_CallStatus_icon),
            &option );
    }

    if (mCipheringIcon) {
        style()->updatePrimitive(
            mCipheringIcon,
            (HbStyle::Primitive)(pluginBaseId()+BP_Ciphering_icon),
            &option );
    }

    if (mExpandIcon) {
        if (mExpanded) {
            mExpandIcon->setIcon(HbIcon("qtg_small_expand"));
        } else {
            mExpandIcon->setIcon(HbIcon("qtg_small_collapse"));
        }
    }

    repolish();
}

void BubbleParticipantListItem::polish(HbStyleParameters& params)
{
    if (mExpanded) {
        setProperty("layoutOption","expanded");
    } else {
        setProperty("layoutOption","collapsed");
    }

    HbListViewItem::polish(params);
}

void BubbleParticipantListItem::receivedFocus()
{
    HbListViewItem::receivedFocus();
    BubbleParticipantListItem* p =
        static_cast<BubbleParticipantListItem*>(prototype());
    if ( p->mActions.count() == 2 ) {
        // supports two buttons layout only

        setExpanded(true);
        emit p->selectionChanged(modelIndex().row());
    }
}

void BubbleParticipantListItem::lostFocus()
{
    HbListViewItem::lostFocus();
    setExpanded(false);
}

void BubbleParticipantListItem::setExpanded(bool expanded)
{
    mExpanded = expanded;
    updateChildItems();
}

void BubbleParticipantListItem::addAction(HbAction* action)
{
    mActions.append(action);
}

void BubbleParticipantListItem::clearActions()
{
    mActions.clear();
    mExpanded = false;
}



