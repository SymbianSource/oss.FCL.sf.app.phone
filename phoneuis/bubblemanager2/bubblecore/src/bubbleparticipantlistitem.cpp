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

#include <QGraphicsLinearLayout>
#include <hbiconitem.h>
#include <hbtextitem.h>
#include <hbaction.h>
#include <hbpushbutton.h>
#include <hbstyleloader.h>
#include <hbabstractitemview.h>
#include <hblistviewitem.h>

#include "bubbleparticipantlistitem.h"
#include "bubblemanagerif.h"
#include "bubbleutils.h"

BubbleParticipantListItem::BubbleParticipantListItem(
    QGraphicsItem *parent) :
    HbAbstractViewItem(parent),
    mText(0),
    mStatusIcon(0),
    mCipheringIcon(0),
    mExpandIcon(0),
    mButton1(0),
    mButton2(0),
    mExpanded(false)
{
    HbStyleLoader::registerFilePath(":/bubbleparticipantlistitem.css");
    HbStyleLoader::registerFilePath(":/bubbleparticipantlistitem.widgetml");
}

BubbleParticipantListItem::~BubbleParticipantListItem()
{
}

HbAbstractViewItem* BubbleParticipantListItem::createItem()
{
    return new BubbleParticipantListItem(*this);
}

void BubbleParticipantListItem::updateChildItems()
{
    HbAbstractViewItem::updateChildItems();

    setFocusPolicy(Qt::ClickFocus); // to enable expanding

    // create primitives
    if (!mText) {
        mText = new HbTextItem(this);
        style()->setItemName( mText, "text" );
    }

    if (!mCipheringIcon) {
        mCipheringIcon = new HbIconItem(this);
        style()->setItemName( mCipheringIcon, "ciphering" );
    }

    if (!mStatusIcon) {
        mStatusIcon = new HbIconItem(this);
        style()->setItemName( mStatusIcon, "icon" );
    }

    if (!mExpandIcon) {
        mExpandIcon = new HbIconItem(this);
        style()->setItemName( mExpandIcon, "expand-indi" );

        // for expand/collapse controls
        if (itemView()) {
            connect( itemView()->selectionModel(),
                     SIGNAL(currentChanged(QModelIndex,QModelIndex)),
                     this,
                     SLOT(currentIndexChanged(QModelIndex,QModelIndex)) );
        }
    }

    BubbleParticipantListItem* p =
            static_cast<BubbleParticipantListItem*>(prototype());

    if (mExpanded && !mButton1) {
        Q_ASSERT(p->mActions.count()==2);
        HbAction* action = p->mActions.at(0);
        mButton1 = new HbPushButton(this);
        mButton1->setIcon(action->icon());
        style()->setItemName( mButton1, "button-1" );

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
        mButton2->setIcon(action->icon());
        style()->setItemName( mButton2, "button-2" );

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

    if (mText) {
        mText->setText(modelIndex().data(Qt::DisplayRole).toString());
    }

    int state = (BubbleManagerIF::PhoneCallState)
        modelIndex().data(Qt::DecorationRole).toInt();

    if (mStatusIcon) {
        BubbleUtils::setCallStatusIcon(state,0,*mStatusIcon);
    }

    if (mCipheringIcon) {
        int flags = !modelIndex().data(Qt::StatusTipRole).toBool() ?
                    BubbleManagerIF::NoCiphering : 0;

        BubbleUtils::setCipheringIcon(state,flags,*mCipheringIcon);
    }

    if (mExpandIcon) {
        if (mExpanded) {
            mExpandIcon->setIcon(HbIcon("qtg_small_collapse"));
        } else {
            mExpandIcon->setIcon(HbIcon("qtg_small_expand"));
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

    HbAbstractViewItem::polish(params);
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

void BubbleParticipantListItem::currentIndexChanged(
    const QModelIndex &current,
    const QModelIndex &previous)
{
    Q_UNUSED(previous);

    if (modelIndex() == current) {
        if ( !mExpanded ) {
            setExpanded(true);
        }
    } else if (mExpanded) {
        setExpanded(false);
    }
}

int BubbleParticipantListItem::type() const
{
    return HbListViewItem::Type;
}

