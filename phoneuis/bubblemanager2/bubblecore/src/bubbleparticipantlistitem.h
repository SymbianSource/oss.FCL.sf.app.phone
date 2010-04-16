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

#include <hblistviewitem.h>

class QGraphicsItem;
class HbPushButton;
class BubbleButtonStyle;
class HbAction;
class HbIconItem;

class BubbleParticipantListItem : public HbListViewItem
{
    Q_OBJECT

public:
    BubbleParticipantListItem(
        const QString& stylePluginName,
        BubbleButtonStyle& buttonStyle,
        QGraphicsItem *parent = 0);

    ~BubbleParticipantListItem();

    HbAbstractViewItem* createItem();

    void updateChildItems();
    void receivedFocus();
    void lostFocus();
    void setExpanded(bool expanded);
    void addAction(HbAction* action);
    void clearActions();

signals:
    void selectionChanged(int row);

private:
    void polish(HbStyleParameters& params);

private:
    QString mStylePluginName;
    int mStylePluginBaseId;
    BubbleButtonStyle& mButtonStyle;
    QGraphicsItem* mBackground;
    QGraphicsItem* mText;
    QGraphicsItem* mStatusIcon;
    QGraphicsItem* mCipheringIcon;
    HbIconItem*    mExpandIcon;
    HbPushButton*  mButton1;
    HbPushButton*  mButton2;
    QList<HbAction*> mActions;

    bool mExpanded;
};

#endif // BUBBLEPARTICIPANTLISTITEM_H
