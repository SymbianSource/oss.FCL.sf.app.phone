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
#include "hbvolumesliderpopup.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// HbVolumeSliderPopup::HbVolumeSliderPopup
// -----------------------------------------------------------------------------
//
HbVolumeSliderPopup::HbVolumeSliderPopup(  )
    : HbSliderPopup()
    {
    
    }


// -----------------------------------------------------------------------------
// HbVolumeSliderPopup::~HbVolumeSliderPopup
// -----------------------------------------------------------------------------
//
HbVolumeSliderPopup::~HbVolumeSliderPopup(  )
    {
    
    }

// -----------------------------------------------------------------------------
// HbVolumeSliderPopup::setCurrentValueVisible
// -----------------------------------------------------------------------------
//
void HbVolumeSliderPopup::setCurrentValueVisible( 
        bool visible )
    {
    SMC_MOCK_METHOD1( void, bool, visible )
    }


// -----------------------------------------------------------------------------
// HbVolumeSliderPopup::isCurrentValueVisible
// -----------------------------------------------------------------------------
//
bool HbVolumeSliderPopup::isCurrentValueVisible(  ) const
    {
    SMC_MOCK_METHOD0( bool )
    }


// -----------------------------------------------------------------------------
// HbVolumeSliderPopup::iconToggled
// -----------------------------------------------------------------------------
//
void HbVolumeSliderPopup::iconToggled( 
        bool checked )
    {
    SMC_MOCK_METHOD1( void, bool, checked )
    }


// -----------------------------------------------------------------------------
// HbVolumeSliderPopup::HbVolumeSliderPopup
// -----------------------------------------------------------------------------
//
HbVolumeSliderPopup::HbVolumeSliderPopup( 
        HbVolumeSliderPopupPrivate & dd,
        QGraphicsItem * parent )
    //:
    //HbSliderPopup( /*dd, parent*/ )
    {
    
    }


// -----------------------------------------------------------------------------
// HbVolumeSliderPopup::keyPressEvent
// -----------------------------------------------------------------------------
//
void HbVolumeSliderPopup::keyPressEvent( 
        QKeyEvent * event )
    {
    SMC_MOCK_METHOD1( void, QKeyEvent *, event )
    }


// -----------------------------------------------------------------------------
// HbVolumeSliderPopup::keyReleaseEvent
// -----------------------------------------------------------------------------
//
void HbVolumeSliderPopup::keyReleaseEvent( 
        QKeyEvent * event )
    {
    SMC_MOCK_METHOD1( void, QKeyEvent *, event )
    }


