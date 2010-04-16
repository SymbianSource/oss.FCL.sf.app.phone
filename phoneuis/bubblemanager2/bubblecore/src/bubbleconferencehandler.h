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
* Description:  Conference call bubble handler.
*
*/

#ifndef BUBBLECONFERENCEHANDLER_H_
#define BUBBLECONFERENCEHANDLER_H_

#include "bubblehandler.h"
#include "bubblewidgetmanager.h"

class BubbleHeader;
class HbAction;
class HbPushButton;
class HbLabel;
class BubbleConferenceHeader;
class HbListWidgetItem;
class BubbleParticipantListItem;
class HbListView;
class BubbleParticipantListModel;
class QTimer;

class BubbleConferenceHandler : public BubbleHandler
    {
    Q_OBJECT

public:
    explicit BubbleConferenceHandler(
            BubbleWidgetManager& widgetManager,
            BubbleWidgetManager::View view,
            BubbleWidgetManager::Container container,
            QObject* parent = 0);
    virtual ~BubbleConferenceHandler();

protected:
    void reset();

    void readBubbleHeader( const BubbleHeader& header );

    void setButtons(const QList<HbAction*>& actions);

    QGraphicsWidget* graphicsWidgetForAction( HbAction* action ) const;
    
    void updateTimerDisplayNow();

    void conferenceMemberRemoved(int bubbleId);

    void conferenceRemoved();

protected slots:
    void handleItemSelected(int row);
    void clearSelection();

protected: //data
    const BubbleConferenceHeader* mHeader;
    HbLabel* mTimerLabel;
    HbPushButton* mButtonCenter;
    HbPushButton* mButtonLeft;
    HbPushButton* mButtonRight;
    HbListView* mList;
    BubbleParticipantListModel* mModel;
    BubbleParticipantListItem* mPrototype; // not owned
    QTimer* mSelectionTimer;
    };

#endif /* BUBBLECONFERENCEHANDLER_H_ */
