/** Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of the License "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:
*
*/
#include <QDebug>
#include <smcmockclassincludes.h>
#include <hbaction.h>
#include "bubblemanager2.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// BubbleManager::BubbleManager
// -----------------------------------------------------------------------------
//
BubbleManager::BubbleManager( 
        QGraphicsItem * parent )
    : HbWidget( parent )
    {
    
    }


// -----------------------------------------------------------------------------
// BubbleManager::~BubbleManager
// -----------------------------------------------------------------------------
//
BubbleManager::~BubbleManager(  )
    {
    
    }


// -----------------------------------------------------------------------------
// BubbleManager::startChanges
// -----------------------------------------------------------------------------
//
void BubbleManager::startChanges(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// BubbleManager::endChanges
// -----------------------------------------------------------------------------
//
void BubbleManager::endChanges(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// BubbleManager::createCallHeader
// -----------------------------------------------------------------------------
//
int BubbleManager::createCallHeader(  )
    {
    SMC_MOCK_METHOD0( int )
    }


// -----------------------------------------------------------------------------
// BubbleManager::removeCallHeader
// -----------------------------------------------------------------------------
//
void BubbleManager::removeCallHeader( 
        int bubbleId )
    {
    SMC_MOCK_METHOD1( void, int, bubbleId )
    }


// -----------------------------------------------------------------------------
// BubbleManager::setState
// -----------------------------------------------------------------------------
//
void BubbleManager::setState( 
        int bubbleId,
        PhoneCallState state )
    {
    SMC_MOCK_METHOD2( void, int, bubbleId, 
        PhoneCallState, state )
    }


// -----------------------------------------------------------------------------
// BubbleManager::setLabel
// -----------------------------------------------------------------------------
//
void BubbleManager::setLabel( 
        int bubbleId,
        const QString & text,
        Qt::TextElideMode clipDirection )
    {
    SMC_MOCK_METHOD3( void, int, bubbleId, 
        const QString &, text, 
        Qt::TextElideMode, clipDirection )
    }


// -----------------------------------------------------------------------------
// BubbleManager::setCli
// -----------------------------------------------------------------------------
//
void BubbleManager::setCli( 
        int bubbleId,
        const QString & cliText,
        Qt::TextElideMode clipDirection )
    {
    SMC_MOCK_METHOD3( void, int, bubbleId, 
        const QString &, cliText, 
        Qt::TextElideMode, clipDirection )
    }


// -----------------------------------------------------------------------------
// BubbleManager::updateCLI
// -----------------------------------------------------------------------------
//
void BubbleManager::updateCLI( 
        int bubbleId,
        const QString & cliText,
        Qt::TextElideMode clipDirection )
    {
    SMC_MOCK_METHOD3( void, int, bubbleId, 
        const QString &, cliText, 
        Qt::TextElideMode, clipDirection )
    }


// -----------------------------------------------------------------------------
// BubbleManager::setSecondaryCli
// -----------------------------------------------------------------------------
//
void BubbleManager::setSecondaryCli( 
        int bubbleId,
        const QString & cliText,
        Qt::TextElideMode clipDirection )
    {
    SMC_MOCK_METHOD3( void, int, bubbleId, 
        const QString &, cliText, 
        Qt::TextElideMode, clipDirection )
    }


// -----------------------------------------------------------------------------
// BubbleManager::setCallTime
// -----------------------------------------------------------------------------
//
void BubbleManager::setCallTime( 
        int bubbleId,
        const QString & callTime )
    {
    SMC_MOCK_METHOD2( void, int, bubbleId, 
        const QString &, callTime )
    }


// -----------------------------------------------------------------------------
// BubbleManager::updateCallTime
// -----------------------------------------------------------------------------
//
void BubbleManager::updateCallTime( 
        int bubbleId,
        const QString & callTime )
    {
    SMC_MOCK_METHOD2( void, int, bubbleId, 
        const QString &, callTime )
    }


// -----------------------------------------------------------------------------
// BubbleManager::setCallObjectImage
// -----------------------------------------------------------------------------
//
void BubbleManager::setCallObjectImage( 
        int bubbleId,
        const QString & fileName )
    {
    SMC_MOCK_METHOD2( void, int, bubbleId, 
        const QString &, fileName )
    }


// -----------------------------------------------------------------------------
// BubbleManager::setCallObjectFromTheme
// -----------------------------------------------------------------------------
//
void BubbleManager::setCallObjectFromTheme( 
        int bubbleId )
    {
    SMC_MOCK_METHOD1( void, int, bubbleId )
    }


// -----------------------------------------------------------------------------
// BubbleManager::setCallFlags
// -----------------------------------------------------------------------------
//
void BubbleManager::setCallFlags( 
        int bubbleId,
        int flags )
    {
    SMC_MOCK_METHOD2( void, int, bubbleId, 
        int, flags )
    }


// -----------------------------------------------------------------------------
// BubbleManager::setCallFlag
// -----------------------------------------------------------------------------
//
void BubbleManager::setCallFlag( 
        int bubbleId,
        PhoneCallFlags flag,
        bool set )
    {
    SMC_MOCK_METHOD3( void, int, bubbleId, 
        /*PhoneCallFlags*/int, flag, 
        bool, set )
    }


// -----------------------------------------------------------------------------
// BubbleManager::setNumberType
// -----------------------------------------------------------------------------
//
void BubbleManager::setNumberType( 
        int bubbleId,
        PhoneNumberType type )
    {
    SMC_MOCK_METHOD2( void, int, bubbleId, 
        PhoneNumberType, type )
    }


// -----------------------------------------------------------------------------
// BubbleManager::setPhoneMuted
// -----------------------------------------------------------------------------
//
void BubbleManager::setPhoneMuted( 
        bool muted )
    {
    SMC_MOCK_METHOD1( void, bool, muted )
    }


// -----------------------------------------------------------------------------
// BubbleManager::createConference
// -----------------------------------------------------------------------------
//
int BubbleManager::createConference( 
        int bubble1,
        int bubble2 )
    {
    SMC_MOCK_METHOD2( int, int, bubble1, 
        int, bubble2 )
    }


// -----------------------------------------------------------------------------
// BubbleManager::removeConference
// -----------------------------------------------------------------------------
//
void BubbleManager::removeConference(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// BubbleManager::addRowToConference
// -----------------------------------------------------------------------------
//
void BubbleManager::addRowToConference( 
        int bubbleId )
    {
    SMC_MOCK_METHOD1( void, int, bubbleId )
    }


// -----------------------------------------------------------------------------
// BubbleManager::removeRowFromConference
// -----------------------------------------------------------------------------
//
void BubbleManager::removeRowFromConference( 
        int bubbleId )
    {
    SMC_MOCK_METHOD1( void, int, bubbleId )
    }


// -----------------------------------------------------------------------------
// BubbleManager::conferenceRowCount
// -----------------------------------------------------------------------------
//
int BubbleManager::conferenceRowCount(  ) const
    {
    SMC_MOCK_METHOD0( int )
    }


// -----------------------------------------------------------------------------
// BubbleManager::setSelectionInConference
// -----------------------------------------------------------------------------
//
void BubbleManager::setSelectionInConference( 
        int rowNumber )
    {
    SMC_MOCK_METHOD1( void, int, rowNumber )
    }


// -----------------------------------------------------------------------------
// BubbleManager::setSelectionIdInConference
// -----------------------------------------------------------------------------
//
void BubbleManager::setSelectionIdInConference( 
        int bubbleId )
    {
    SMC_MOCK_METHOD1( void, int, bubbleId )
    }


// -----------------------------------------------------------------------------
// BubbleManager::selectionInConference
// -----------------------------------------------------------------------------
//
int BubbleManager::selectionInConference(  ) const
    {
    SMC_MOCK_METHOD0( int )
    }


// -----------------------------------------------------------------------------
// BubbleManager::selectionIdInConference
// -----------------------------------------------------------------------------
//
int BubbleManager::selectionIdInConference(  ) const
    {
    SMC_MOCK_METHOD0( int )
    }


// -----------------------------------------------------------------------------
// BubbleManager::moveHighlightOneUpInConference
// -----------------------------------------------------------------------------
//
void BubbleManager::moveHighlightOneUpInConference(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// BubbleManager::moveHighlightOneDownInConference
// -----------------------------------------------------------------------------
//
void BubbleManager::moveHighlightOneDownInConference(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// BubbleManager::setExpandedConferenceCallHeader
// -----------------------------------------------------------------------------
//
void BubbleManager::setExpandedConferenceCallHeader( 
        bool expanded )
    {
    SMC_MOCK_METHOD1( void, bool, expanded )
    }


// -----------------------------------------------------------------------------
// BubbleManager::isConferenceExpanded
// -----------------------------------------------------------------------------
//
bool BubbleManager::isConferenceExpanded(  ) const
    {
    SMC_MOCK_METHOD0( bool )
    }


// -----------------------------------------------------------------------------
// BubbleManager::shownHeaderCount
// -----------------------------------------------------------------------------
//
int BubbleManager::shownHeaderCount(  ) const
    {
    SMC_MOCK_METHOD0( int )
    }


// -----------------------------------------------------------------------------
// BubbleManager::setParticipantListCli
// -----------------------------------------------------------------------------
//
void BubbleManager::setParticipantListCli( 
        int aBubbleId,
        ParticipantListCli aParticipantCli )
    {
    SMC_MOCK_METHOD2( void, int, aBubbleId, 
        ParticipantListCli, aParticipantCli )
    }


// -----------------------------------------------------------------------------
// BubbleManager::addAction
// -----------------------------------------------------------------------------
//
void BubbleManager::addAction( 
        int bubbleId,
        HbAction * a )
    {
    SMC_MOCK_METHOD2( void, int, bubbleId, 
        HbAction *, a )
    }


// -----------------------------------------------------------------------------
// BubbleManager::clearActions
// -----------------------------------------------------------------------------
//
void BubbleManager::clearActions( 
        int bubbleId )
    {
    SMC_MOCK_METHOD1( void, int, bubbleId )
    }


// -----------------------------------------------------------------------------
// BubbleManager::graphicsWidgetForAction
// -----------------------------------------------------------------------------
//
QGraphicsWidget * BubbleManager::graphicsWidgetForAction( 
        HbAction * a ) const
    {
    SMC_MOCK_METHOD1( QGraphicsWidget *, HbAction *, a )
    }


// -----------------------------------------------------------------------------
// BubbleManager::addParticipantListAction
// -----------------------------------------------------------------------------
//
void BubbleManager::addParticipantListAction( 
        HbAction * a )
    {
    SMC_MOCK_METHOD1( void, HbAction *, a )
    }


// -----------------------------------------------------------------------------
// BubbleManager::clearParticipantListActions
// -----------------------------------------------------------------------------
//
void BubbleManager::clearParticipantListActions(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// BubbleManager::expandedBubble
// -----------------------------------------------------------------------------
//
int BubbleManager::expandedBubble(  ) const
    {
    SMC_MOCK_METHOD0( int )
    }


// -----------------------------------------------------------------------------
// BubbleManager::setExpandAction
// -----------------------------------------------------------------------------
//
void BubbleManager::setExpandAction( 
        int bubbleId,
        HbAction * a )
    {
    SMC_MOCK_METHOD2( void, int, bubbleId, 
        HbAction *, a )
    }


// -----------------------------------------------------------------------------
// BubbleManager::setBubbleSelectionDisabled
// -----------------------------------------------------------------------------
//
void BubbleManager::setBubbleSelectionDisabled( 
        bool disabled )
    {
    SMC_MOCK_METHOD1( void, bool, disabled )
    }


// -----------------------------------------------------------------------------
// BubbleManager::handleOrientationChange
// -----------------------------------------------------------------------------
//
void BubbleManager::handleOrientationChange( 
        Qt::Orientation orientation )
    {
    SMC_MOCK_METHOD1( void, Qt::Orientation, orientation )
    }


