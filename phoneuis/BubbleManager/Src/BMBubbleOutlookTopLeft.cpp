/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Implementation of CBubbleOutlookTopLeft class.
*
*/


// INCLUDE FILES
#include    "BMBubbleManager.h" //for enumerations
#include    "BMBubbleOutlookTopLeft.h"
#include    "BMBubbleImageManager.h"
#include    "BMResourceManager.h"
#include    "BMBubbleHeader.h"
#include    "BMUtils.h"
#include    "BMLayout.h"
#include    "BMLayout2.h"

#include    <eiklabel.h> 
#include    <eikimage.h>
#include    <eikenv.h>
#include    <AknsUtils.h>
#include    <AknsDrawUtils.h>

// ========================= MEMBER FUNCTIONS ================================


// ---------------------------------------------------------------------------
// CBubbleOutlookTopLeft::CBubbleOutlookTopLeft
//
//  
// ---------------------------------------------------------------------------
//
CBubbleOutlookTopLeft::CBubbleOutlookTopLeft( 
                                   CBubbleManager& aBubbleManager ) 
: CBubbleOutlookOneLined( aBubbleManager )                               
    {
    }

// ---------------------------------------------------------------------------
// CBubbleOutlookTopLeft::ConstructL
// 
// ---------------------------------------------------------------------------
//
void CBubbleOutlookTopLeft::ConstructL()
    {
    CBubbleOutlookOneLined::ConstructL( );
    }

// ---------------------------------------------------------------------------
// CBubbleOutlookTopLeft::~CBubbleOutlookTopLeft
//  
// ---------------------------------------------------------------------------
//
CBubbleOutlookTopLeft::~CBubbleOutlookTopLeft()
    {

    }


// ---------------------------------------------------------------------------
// CBubbleOutlookTopLeft::ReadBubbleHeader
//
//  
// ---------------------------------------------------------------------------
//
void CBubbleOutlookTopLeft::ReadBubbleHeader( CBubbleHeader& aHeader )
    {
    iHeader = &aHeader;

    iBubble = iBubbleManager.ResourceManager().ReserveEikImage( ETrue );        
    
    if ( !iCallObjectDisplay )
        {
        GetCall1BubbleBitmaps();    
        }
    
    CBubbleOutlookOneLined::ReadBubbleHeader( aHeader );
    }



// ---------------------------------------------------------------------------
// CBubbleOutlookTopLeft::SizeChanged
// called by framwork when the view size is changed
//  
// ---------------------------------------------------------------------------
//
void CBubbleOutlookTopLeft::SizeChanged()
    {
    if( !iHeader )
        {
        return;
        }

    AknsUtils::RegisterControlPosition( this );
    
    if ( !iCallObjectDisplay )
        {
        DoCall1Layout();    
        }
    else
        {
        TRAPD( err, DoCall2LayoutL() );
        if ( err )    
            {
            iBubble->SetPicture( NULL, NULL );
                
            MAknsSkinInstance* skin = AknsUtils::SkinInstance();
            AknsDrawUtils::PrepareFrame( skin,
                                         iOuterRect,
                                         iInnerRect,
                                         iFrameId,
                                         KAknsIIDDefault );    
            }
        }     
    }

    
// ---------------------------------------------------------------------------
// CBubbleOutlookTopLeft::DoCall1Layout
// ---------------------------------------------------------------------------
//    
void CBubbleOutlookTopLeft::DoCall1Layout()
    {
    // Deprecated
    }

// ---------------------------------------------------------------------------
// CBubbleOutlookTopLeft::DoCall2Layout
// ---------------------------------------------------------------------------
//
void CBubbleOutlookTopLeft::DoCall2LayoutL()
    {
    iBubble->SetPictureOwnedExternally( EFalse );
    iBubble->SetPicture( NULL, NULL );
    
    const TRect rect = Rect();
    CBubbleManager::TPhoneCallState callState = iHeader->CallState();
    switch ( callState )
        {
        case CBubbleManager::EWaiting:
            {
            ///////////////////////////////////////////////////////////////////
            // Waiting call layout
            ///////////////////////////////////////////////////////////////////
            
            TAknLayoutRect bubbleRect;
            bubbleRect.LayoutRect( 
                rect, 
                BubbleLayout2::popup_call2_audio_wait_call_background() );
            
            TRect frameRect( TPoint(0,0), bubbleRect.Rect().Size() );
            TRect outerRect;
            TRect innerRect;
            BubbleLayout2::RectFrameInnerOuterRects( frameRect, 
                                                     outerRect, 
                                                     innerRect );
            
            iFrameId = ( callState == CBubbleManager::EAlertToDisconnected ) ?
                                      KAknsIIDQsnFrCall2RectDisconn : 
                                      KAknsIIDQsnFrCall2Rect;
            
            iOuterRect = outerRect;
            iOuterRect.Move( Rect().iTl );
            iInnerRect = innerRect;
            iInnerRect.Move( Rect().iTl );
            
            // Call icon
            BubbleUtils::LayoutCustomElement(
                iSmallCallIndication, 
                rect, 
                BubbleLayout2::popup_call2_audio_wait_call_status_icon() );
                
            // Ciphering off
            BubbleUtils::LayoutControl( 
                iCyphOffImage, 
                rect,
                BubbleLayout2::popup_call2_audio_wait_call_cyphering_icon() );
                
            
            // Call type pane    
            BubbleUtils::LayoutCallTypeIndicators(
                rect,
                BubbleLayout2::popup_call2_audio_wait_call_type_icon(),
                iTypeIndication1,   // Data/Fax
                iTypeIndication2 ); // ALS line 2
            
            // Text
            BubbleUtils::LayoutLabel( 
                iTextLine1, 
                rect, 
                BubbleLayout2::popup_call2_audio_wait_call_cli_text() );
                
            // Create call object bitmaps
            iBubble->SetRect( bubbleRect.Rect() );                
            
            BubbleUtils::PrepareBubbleImageL( 
                iFrameId,
                outerRect,
                innerRect,
                iBubble );
                
            if ( iHeader->CallObjectImage() &&
                 !iHeader->CallObjectText().Length() )
                {
                // Add transparency
                BubbleUtils::AddTransparencyToBubbleImageL( 
                    KAknsIIDQsnFrCall2RectCoMask, // gradient tp mask
                    outerRect,
                    innerRect,
                    iBubble );                    
                }                
            break;
            }
        case CBubbleManager::EDisconnected:
        case CBubbleManager::EActive:
        case CBubbleManager::EOnHold:
            {
            ///////////////////////////////////////////////////////////////////
            // First call layout. Two calls and number entry.
            ///////////////////////////////////////////////////////////////////
            
            // Create bubble image
            TAknLayoutRect bubbleRect;
            bubbleRect.LayoutRect( 
                rect, 
                BubbleLayout2::popup_call2_audio_first_call_background(15) );
            
            TRect frameRect( TPoint(0,0), bubbleRect.Rect().Size() );
            TRect outerRect;
            TRect innerRect;
            BubbleLayout2::BubbleFrameInnerOuterRects( frameRect, 
                                                       outerRect, 
                                                       innerRect);
													   
            if ( callState == CBubbleManager::EOnHold )
                {
                iFrameId = KAknsIIDQsnFrCall2Bubble;
                }
            else if ( callState == CBubbleManager::EDisconnected )
                {
                iFrameId = KAknsIIDQsnFrCall2BubbleFirstDisconn;
                }
            else
                {
                iFrameId = KAknsIIDQsnFrCall2BubbleFirst;
                }                
            
            iOuterRect = outerRect;
            iOuterRect.Move( Rect().iTl );
            iInnerRect = innerRect;
            iInnerRect.Move( Rect().iTl );

            // Call icon
            BubbleUtils::LayoutCustomElement( 
                iSmallCallIndication, 
                rect,
                BubbleLayout2::popup_call2_audio_first_call_status_icon(14) );
                
            // Ciphering off
            BubbleUtils::LayoutControl( 
                iCyphOffImage, 
                rect,
                BubbleLayout2::popup_call2_audio_first_call_window_ciphering_icon(13) );
            
            // Call type pane
            BubbleUtils::LayoutCallTypeIndicators(
                rect,
                BubbleLayout2::popup_call2_audio_first_call_type_icon(13),
                iTypeIndication1,   // Data/Fax
                iTypeIndication2 ); // ALS line 2
                
            // Text line 1
            BubbleUtils::LayoutLabel( 
                iTextLine1, 
                rect, 
                BubbleLayout2::popup_call2_audio_first_call_cli_text(17) );
                
            // Create call bubble
            iBubble->SetRect( bubbleRect.Rect() );                                
            
            BubbleUtils::PrepareBubbleImageL( 
                iFrameId,
                outerRect,
                innerRect,
                iBubble );
                       
            break;
            }
        case CBubbleManager::ENone:
        default:
            break;
        }
    }

// ---------------------------------------------------------------------------
// CBubbleOutlookTopLeft::GetCall1BubbleBitmaps
// ---------------------------------------------------------------------------
//    
void CBubbleOutlookTopLeft::GetCall1BubbleBitmaps()
    {
    CBubbleManager::TPhoneCallState callState = iHeader->CallState();
    
    TInt bubblePicture = KErrNotFound;
    TInt bubblePictureMask = KErrNotFound;
    
    // set call indications and bubble
        // set call indications and bubble
    switch ( callState )
        {
        case CBubbleManager::EWaiting:
            bubblePicture = EQgn_graf_call_rec_small_left;
            bubblePictureMask = EQgn_graf_call_rec_small_left_mask;
            break;
        case CBubbleManager::EActive:
            bubblePicture = EQgn_graf_call_first_three_active;
            bubblePictureMask = EQgn_graf_call_first_three_active_mask;
            break;
        case CBubbleManager::EOnHold:
            bubblePicture = EQgn_graf_call_first_three_held;
            bubblePictureMask = EQgn_graf_call_first_three_held_mask;
            break;
        case CBubbleManager::EDisconnected:
            bubblePicture = EQgn_graf_call_first_three_disconn;
            bubblePictureMask = EQgn_graf_call_first_three_disconn_mask; 
            break;
        case CBubbleManager::EIncoming:
        case CBubbleManager::EOutgoing:
        case CBubbleManager::EAlertToDisconnected:
        case CBubbleManager::EAlerting:
        case CBubbleManager::ENone:
        default:
            bubblePicture = KErrNotFound;
            bubblePictureMask = KErrNotFound;
            break;
        }

    if ( bubblePicture != KErrNotFound )
        {
        if ( bubblePictureMask == KErrNotFound )
            {
            iBubbleManager.ImageManager().SetBitmapToImage( 
                iBubble, (TBMIcons) bubblePicture );
            }
        else
            {
            iBubbleManager.ImageManager().SetBitmapToImage( 
                iBubble, 
                (TBMIcons) bubblePicture , 
                (TBMIcons) bubblePictureMask);
            }
        }
        
    }    

// End of File
