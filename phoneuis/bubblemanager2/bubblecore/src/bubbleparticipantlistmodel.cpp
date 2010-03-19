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
* Description: Model for conference participant list.
*
*/

#include <QtCore>
#include "bubbleparticipantlistmodel.h"

class Participant
{
public:
    int mBubbleId;
    QString mName;
    int mState;

public:
    Participant(int bubbleId, const QString &name, int state);

};

Participant::Participant(int bubbleId, const QString &name, int state) :
    mBubbleId(bubbleId), mName(name), mState(state)
{
}


BubbleParticipantListModel::BubbleParticipantListModel()
{
}

BubbleParticipantListModel::~BubbleParticipantListModel()
{
    qDeleteAll(mParticipants);
}

int BubbleParticipantListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return mParticipants.count();
}

QVariant BubbleParticipantListModel::data(
    const QModelIndex &index,
    int role ) const
{
    Participant *player = mParticipants.at(index.row());

    if (role == Qt::DisplayRole) {
        return player->mName;
    } else if (role == Qt::DecorationRole) {
        return player->mState;
    } else {
        return QVariant();
    }
}

void BubbleParticipantListModel::addParticipant(
    int bubbleId,
    const QString &name,
    int state )
{
    Participant* p = new Participant(bubbleId,name,state);
    mParticipants.append(p);
}

int BubbleParticipantListModel::bubbleId(int row)
{
    if (mParticipants.count()>=row) {
        return mParticipants.at(row)->mBubbleId;
    } else {
        return -1;
    }
}

void BubbleParticipantListModel::reset()
{
    qDeleteAll(mParticipants);
    mParticipants.clear();
}

