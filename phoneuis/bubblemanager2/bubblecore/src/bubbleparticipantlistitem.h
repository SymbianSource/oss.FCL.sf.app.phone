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

#ifndef BUBBLEPARTICIPANTLISTITEM_H
#define BUBBLEPARTICIPANTLISTITEM_H

#include <hbabstractviewitem.h>

class QGraphicsItem;
class HbPushButton;
class HbAction;
class HbIconItem;
class HbFrameItem;
class HbTextItem;

class BubbleParticipantListItem : public HbAbstractViewItem
{
    Q_OBJECT

public:
    BubbleParticipantListItem(QGraphicsItem *parent = 0);

    ~BubbleParticipantListItem();

    HbAbstractViewItem* createItem();

    void updateChildItems();
    void setExpanded(bool expanded);
    void addAction(HbAction* action);
    void clearActions();

private:
    void polish(HbStyleParameters& params);

    int type() const;

private slots:
    void currentIndexChanged(const QModelIndex &current,
                             const QModelIndex &previous);

private:
    HbTextItem* mText;
    HbIconItem* mStatusIcon;
    HbIconItem* mCipheringIcon;
    HbIconItem* mExpandIcon;
    HbPushButton* mButton1;
    HbPushButton* mButton2;
    QList<HbAction*> mActions;

    bool mExpanded;
};

#endif // BUBBLEPARTICIPANTLISTITEM_H
