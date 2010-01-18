/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Outlook
*
*/


// INCLUDE FILES
#include    "BMBubbleOutlook.h"
#include    "BMLayout.h"
#include    <eiklabel.h> 
#include    <eikenv.h>

#include    <AknsBasicBackgroundControlContext.h>
#include    <AknsDrawUtils.h>
#include    <AknsUtils.h>

// ========================= MEMBER FUNCTIONS ================================

CBubbleOutlook::CBubbleOutlook( CBubbleManager& aBubbleManager )
: iBubbleManager( aBubbleManager )
    {
    }

// ---------------------------------------------------------------------------
// CBubbleOutlook::ConstructL(const TRect& aRect)
// Symbian OS two phased constructor
// 
// ---------------------------------------------------------------------------
//
void CBubbleOutlook::ConstructL( )
    {
    }

// Destructor
CBubbleOutlook::~CBubbleOutlook()
    {
    AknsUtils::DeregisterControlPosition( this );
    }

// ---------------------------------------------------------------------------
//  CBubbleOutlook::PositionChanged
//  
//  
// ---------------------------------------------------------------------------
//
void CBubbleOutlook::PositionChanged()
    {
    AknsUtils::RegisterControlPosition( this );
    }

// ---------------------------------------------------------------------------
//  CBubbleOutlook::CountComponentControls
//  
//  
// ---------------------------------------------------------------------------
//
TInt CBubbleOutlook::CountComponentControls( ) const
    {
    return 0; // Default. Child class may override.
    }

// ---------------------------------------------------------------------------
//  CBubbleOutlook::ComponentControl
//  
//  
// ---------------------------------------------------------------------------
//
CCoeControl* CBubbleOutlook::ComponentControl( TInt /*aIndex*/ ) const
    {
    return NULL; // Default. Child class may override.
    }

// ---------------------------------------------------------------------------
//  CBubbleOutlook::Draw
//  
//  
// ---------------------------------------------------------------------------
//
void CBubbleOutlook::Draw( const TRect& /*aRect*/ ) const
    {
    // Empty. Child class may override.
    }

// ---------------------------------------------------------------------------
//  CBubbleOutlook::DrawLabelNow
//  
//  
// ---------------------------------------------------------------------------
//
void CBubbleOutlook::DrawLabelNow( const CCoeControl* aLabel )
    {
    if ( aLabel == NULL )
        {
        return;
        }
    DrawLabelNow( *aLabel );
    }

// ---------------------------------------------------------------------------
//  CBubbleOutlook::DrawLabelNow
//  
//  
// ---------------------------------------------------------------------------
//
void CBubbleOutlook::DrawLabelNow( const CCoeControl& aLabel )
    {
    // Clears also the label background. If CCoeControl::DrawNow() would
    // be used then background drawing should be provided. However,
    // CCoeControl::DrawDeferred() serves the purpose here.
    aLabel.DrawDeferred();
    }

// ---------------------------------------------------------------------------
//  CBubbleOutlook::ReleaseBitmaps
// ---------------------------------------------------------------------------
//
void CBubbleOutlook::ReleaseBitmaps()
    {
    // default: do nothing. Derived class may override.
    }

// ---------------------------------------------------------------------------
//  CBubbleOutlook::RebuildBitmaps
// ---------------------------------------------------------------------------
//
void CBubbleOutlook::RebuildBitmaps()
    {
    // default: do nothing. Derived class may override.
    }

// ---------------------------------------------------------------------------
//  CBubbleOutlook::SetCallObjectDisplay
// ---------------------------------------------------------------------------
//    
void CBubbleOutlook::SetCallObjectDisplay( TBool aActive )
    {
    iCallObjectDisplay = aActive;    
    }

// End of File
