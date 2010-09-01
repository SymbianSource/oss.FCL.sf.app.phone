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
* Description:  Header place on the screen.
*
*/


// INCLUDE FILES
#include    "BMBubbleManager.h"  
#include    "BMBubblePlace.h"
#include    "BMBubbleOutlookBottom.h"
#include    "BMBubbleOutlookMiddle.h"
#include    "BMBubbleOutlookBottomRight.h"
#include    "BMBubbleOutlook.h"
#include    "BMBubbleOutlookTopLeft.h"
#include    "BMBubbleOutlookTopRight.h"
#include    "BMBubbleOutlookHide.h"
#include    "BMBubbleOutlookConference.h"
#include    "BMPanic.h"

#include    "BMBubbleOutlookBottomCnap.h"
#include    "BMBubbleOutlookMiddleCnap.h"
#include    "BMBubbleOutlookBottomText.h"
#include    "BMBubbleOutlookBottomImage.h"
#include    "BMBubbleOutlookVideo.h"
#include    "BMBubbleOutlookVideoCnap.h"
#include    "BMBubbleOutlookBottomRiCnap.h"
#include    "BMBubbleOutlookBottomImCnap.h"

#include    <AknsUtils.h>

// ========================= MEMBER FUNCTIONS ================================

// ---------------------------------------------------------------------------
// CBubblePlace::ConstructL
// 
// ---------------------------------------------------------------------------
//
void CBubblePlace::ConstructL( 
                              const TPhoneBubblePlace& aBubblePlace,
                              CBubbleManager& aBubbleManager )
    {
    
    iBubblePlace = aBubblePlace;
    iBubbleOutlook = NULL;
    iIsUsed  = EFalse; // By default.
    
    // Create corresponding BubbleOutlook:
    switch( iBubblePlace )
        {
        case EBottom:
            {
            CBubbleOutlookBottom* BOBottom = 
                new( ELeave ) CBubbleOutlookBottom( aBubbleManager );
            CleanupStack::PushL( BOBottom );
            BOBottom->SetContainerWindowL( *this );
            BOBottom->ConstructL();
            CleanupStack::Pop();
            iBubbleOutlook = BOBottom;
            }
            break;
        case EBottomRightActive:
        case EBottomRightHeld:
            {
            CBubbleOutlookBottomRight* BOBottomR = 
                new( ELeave ) CBubbleOutlookBottomRight( aBubbleManager );
            CleanupStack::PushL( BOBottomR );
            BOBottomR->SetContainerWindowL( *this );
            BOBottomR->ConstructL();
            CleanupStack::Pop();
            iBubbleOutlook = BOBottomR;
            }
            break;
        case EMiddleActive:
        case EMiddleHeld:
            {
            CBubbleOutlookMiddle* BOMiddle = 
                new( ELeave ) CBubbleOutlookMiddle( aBubbleManager );
            CleanupStack::PushL( BOMiddle );
            BOMiddle->SetContainerWindowL( *this );
            BOMiddle->ConstructL();
            CleanupStack::Pop();
            iBubbleOutlook = BOMiddle;
            }
            break;
        case ETopLeft:
            {
            CBubbleOutlookTopLeft* BOTL = 
                new( ELeave ) CBubbleOutlookTopLeft( aBubbleManager );
            CleanupStack::PushL( BOTL );
            BOTL->SetContainerWindowL( *this );
            BOTL->ConstructL();
            CleanupStack::Pop();
            iBubbleOutlook = BOTL;
            }
            break;
        case ETopRight:
            {
            CBubbleOutlookTopRight* BOTR = 
                new( ELeave ) CBubbleOutlookTopRight( aBubbleManager );
            CleanupStack::PushL( BOTR );
            BOTR->SetContainerWindowL( *this );
            BOTR->ConstructL();
            CleanupStack::Pop();
            iBubbleOutlook = BOTR;
            }
            break;
        case EHide:
            {
            CBubbleOutlookHide* BOH = 
                new( ELeave ) CBubbleOutlookHide( aBubbleManager );
            CleanupStack::PushL( BOH );
            BOH->SetContainerWindowL( *this );
            BOH->ConstructL();
            CleanupStack::Pop();
            iBubbleOutlook = BOH;
            }
            break;
        case E2InConf:
            {
            CBubbleOutlookConference* BOConf = 
                new( ELeave ) CBubbleOutlookConference( aBubbleManager );
            CleanupStack::PushL( BOConf );
            BOConf->SetContainerWindowL( *this );
            BOConf->ConstructL( 2 );
            CleanupStack::Pop();
            iBubbleOutlook = BOConf;
            }
            break;
        case E3InConf:
            {
            CBubbleOutlookConference* BOConf = 
                new( ELeave ) CBubbleOutlookConference( aBubbleManager );
            CleanupStack::PushL( BOConf );
            BOConf->SetContainerWindowL( *this );
            BOConf->ConstructL( 3 );
            CleanupStack::Pop();
            iBubbleOutlook = BOConf;
            }
            break;
        case E4InConf:
            {
            CBubbleOutlookConference* BOConf = 
                new( ELeave ) CBubbleOutlookConference( aBubbleManager );
            CleanupStack::PushL( BOConf );
            BOConf->SetContainerWindowL( *this );
            BOConf->ConstructL( 4 );
            CleanupStack::Pop();
            iBubbleOutlook = BOConf;
            }
            break;
        case E5InConf:
            {
            CBubbleOutlookConference* BOConf = 
                new( ELeave ) CBubbleOutlookConference( aBubbleManager );
            CleanupStack::PushL( BOConf );
            BOConf->SetContainerWindowL( *this );
            BOConf->ConstructL(  5 );
            CleanupStack::Pop();
            iBubbleOutlook = BOConf;
            }
            break;
        case EBottomText:
            {
            CBubbleOutlookBottomText* BOBottomText = 
                new( ELeave ) CBubbleOutlookBottomText( aBubbleManager );
            CleanupStack::PushL( BOBottomText );
            BOBottomText->SetContainerWindowL( *this );
            BOBottomText->ConstructL();
            CleanupStack::Pop();
            iBubbleOutlook = BOBottomText;
            }
            break;
        case EBottomImage:
            {
            CBubbleOutlookBottomImage* BOBottomImage = 
                new( ELeave ) CBubbleOutlookBottomImage( aBubbleManager );
            CleanupStack::PushL( BOBottomImage );
            BOBottomImage->SetContainerWindowL( *this );
            BOBottomImage->ConstructL();
            CleanupStack::Pop();
            iBubbleOutlook = BOBottomImage;
            }
            break;                        
        case EVideo:
            {
            CBubbleOutlookVideo* BOVideo = 
                new( ELeave ) CBubbleOutlookVideo( aBubbleManager );
            CleanupStack::PushL( BOVideo );
            BOVideo->SetContainerWindowL( *this );
            BOVideo->ConstructL();
            CleanupStack::Pop();
            iBubbleOutlook = BOVideo;
            }
            break;                                    
        case EBottomCnap:
            {
            CBubbleOutlookBottomCnap* BOBottomCnap = 
                new( ELeave ) CBubbleOutlookBottomCnap( aBubbleManager );
            CleanupStack::PushL( BOBottomCnap );
            BOBottomCnap->SetContainerWindowL( *this );
            BOBottomCnap->ConstructL();
            CleanupStack::Pop();
            iBubbleOutlook = BOBottomCnap;
            }
            break;
        case EMiddleCnap:
            {
            CBubbleOutlookMiddleCnap* BOMiddleCnap = 
                new( ELeave ) CBubbleOutlookMiddleCnap( aBubbleManager );
            CleanupStack::PushL( BOMiddleCnap );
            BOMiddleCnap->SetContainerWindowL( *this );
            BOMiddleCnap->ConstructL();
            CleanupStack::Pop();
            iBubbleOutlook = BOMiddleCnap;
            }
            break;
        case EVideoCnap:
            {
            CBubbleOutlookVideoCnap* BOVideoCnap = 
                new( ELeave ) CBubbleOutlookVideoCnap( aBubbleManager );
            CleanupStack::PushL( BOVideoCnap );
            BOVideoCnap->SetContainerWindowL( *this );
            BOVideoCnap->ConstructL();
            CleanupStack::Pop();
            iBubbleOutlook = BOVideoCnap;
            }
            break;
        case EBottomRightCnap:
            {
            CBubbleOutlookBottomRightCnap* BOBottomRightCnap = 
                new( ELeave ) CBubbleOutlookBottomRightCnap( aBubbleManager );
            CleanupStack::PushL( BOBottomRightCnap );
            BOBottomRightCnap->SetContainerWindowL( *this );
            BOBottomRightCnap->ConstructL();
            CleanupStack::Pop();
            iBubbleOutlook = BOBottomRightCnap;
            }
            break;
        case EBottomImageCnap:
            {
            CBubbleOutlookBottomImageCnap* BOBottomImageCnap = 
                new( ELeave ) CBubbleOutlookBottomImageCnap( aBubbleManager );
            CleanupStack::PushL( BOBottomImageCnap );
            BOBottomImageCnap->SetContainerWindowL( *this );
            BOBottomImageCnap->ConstructL();
            CleanupStack::Pop();
            iBubbleOutlook = BOBottomImageCnap;
            }
            break;
        case EBottomRightActiveTouch:
            {
            CBubbleOutlookBottomRight* BOBottomR = 
                new( ELeave ) CBubbleOutlookBottomRight( aBubbleManager );
            CleanupStack::PushL( BOBottomR );
            BOBottomR->SetContainerWindowL( *this );
            BOBottomR->ConstructL();
            CleanupStack::Pop();
            iBubbleOutlook = BOBottomR;    
            }
            break;
        case EBottomRightActiveCnapTouch:
            {
            CBubbleOutlookBottomRightCnap* BOBottomRightCnap = 
                new( ELeave ) CBubbleOutlookBottomRightCnap( aBubbleManager );
            CleanupStack::PushL( BOBottomRightCnap );
            BOBottomRightCnap->SetContainerWindowL( *this );
            BOBottomRightCnap->ConstructL();
            CleanupStack::Pop();
            iBubbleOutlook = BOBottomRightCnap;    
            }
            break;
        case EMiddleHeldTouch:
            {
            CBubbleOutlookTopLeft* BOTL = 
                new( ELeave ) CBubbleOutlookTopLeft( aBubbleManager );
            CleanupStack::PushL( BOTL );
            BOTL->SetContainerWindowL( *this );
            BOTL->ConstructL();
            CleanupStack::Pop();
            iBubbleOutlook = BOTL;    
            }
            break;
        default:
            Panic( EBMPanicUnhandledSwitchCase );
            break;
        } 


    iBubbleOutlook->SetParent( this );
        
    ActivateL();
    }
    
// ---------------------------------------------------------------------------
// CBubblePlace::~CBubblePlace
// Destructor.
//  
// ---------------------------------------------------------------------------
//
CBubblePlace::~CBubblePlace()
    {
    AknsUtils::DeregisterControlPosition( this );
    delete iBubbleOutlook;   
    }

// ---------------------------------------------------------------------------
// CBubblePlace::SizeChanged
// called by framwork when the view size is changed
//  
// ---------------------------------------------------------------------------
//
void CBubblePlace::SizeChanged()
    {  
    AknsUtils::RegisterControlPosition( this );
    iBubbleOutlook->SetRect ( Rect() );
    }

// ---------------------------------------------------------------------------
// CBubblePlace::PositionChanged
// ---------------------------------------------------------------------------
//
void CBubblePlace::PositionChanged()
    {
    AknsUtils::RegisterControlPosition( this );
    }

// ---------------------------------------------------------------------------
// CBubblePlace::CountComponentControls
//
//  
// ---------------------------------------------------------------------------
//
TInt CBubblePlace::CountComponentControls() const
    { 
    if ( iIsUsed )
        {
        return 1; // bubble outlook
        }
    return 0;
    }

// ---------------------------------------------------------------------------
// CBubblePlace::ComponentControl
//
//  
// ---------------------------------------------------------------------------
//
CCoeControl* CBubblePlace::ComponentControl( TInt aIndex ) const
    {
    if ( !iIsUsed ) 
        {
        return NULL;
        }
    
    switch ( aIndex )
        {
        case 0:
            return iBubbleOutlook;
        default:
            return NULL;
        }
    }





// ---------------------------------------------------------------------------
// CBubblePlace::SetIsUsed
//  
//  
// ---------------------------------------------------------------------------
//
void CBubblePlace::SetIsUsed( const TBool& aIsUsed )
    {
    iIsUsed = aIsUsed;
    if ( !iIsUsed )
        {
        iBubbleOutlook->Reset( );
        }
    }


// ---------------------------------------------------------------------------
// CBubblePlace::IsUsed
//  
//  
// ---------------------------------------------------------------------------
//
TBool CBubblePlace::IsUsed( ) const
    {
    return iIsUsed;
    }


// ---------------------------------------------------------------------------
// CBubblePlace::GetBubblePlace
//  
//  
// ---------------------------------------------------------------------------
//
CBubblePlace::TPhoneBubblePlace CBubblePlace::GetBubblePlace( ) const
    {
    return iBubblePlace;
    }


// ---------------------------------------------------------------------------
// CBubblePlace::GetBubbleOutlook
//  
//  
// ---------------------------------------------------------------------------
//
CBubbleOutlook& CBubblePlace::GetBubbleOutlook( ) const
    {
    return *iBubbleOutlook;
    }
    
// End of File
