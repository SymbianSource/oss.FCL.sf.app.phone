/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Outlook Bottom
*
*/


// INCLUDE FILES
#include    "BMBubbleManager.h" //for enumerations
#include    "BMBubbleOutlookBottom.h"
#include    "BMBubbleImageManager.h"
#include    "BMResourceManager.h"
#include    "BMBubbleHeader.h"
#include    "BMUtils.h"
#include    "BMLayout.h"
#include    "BMLayout2.h"
#include    "telbubblecustomelement.h"

#include    <eiklabel.h> 
#include    <eikimage.h>
#include    <eikenv.h>
#include    <AknsUtils.h>
#include    <AknsDrawUtils.h>


// ========================= MEMBER FUNCTIONS ================================


// ---------------------------------------------------------------------------
// CBubbleOutlookBottom::CBubbleOutlookBottom
//
//  
// ---------------------------------------------------------------------------
//
CBubbleOutlookBottom::CBubbleOutlookBottom( CBubbleManager& aBubbleManager )
: CBubbleOutlookTwoLined( aBubbleManager )
    {
    }

// ---------------------------------------------------------------------------
// CBubbleOutlookBottom::ConstructL
// 
// ---------------------------------------------------------------------------
//
void CBubbleOutlookBottom::ConstructL()
    { 
    CBubbleOutlookTwoLined::ConstructL();
    }

// ---------------------------------------------------------------------------
// CBubbleOutlookBottom::~CBubbleOutlookBottom
//
//  
// ---------------------------------------------------------------------------
//
CBubbleOutlookBottom::~CBubbleOutlookBottom()
    {
    Reset();
    }

// ---------------------------------------------------------------------------
// CBubbleOutlookBottom::ReadBubbleHeader
//
//  
// ---------------------------------------------------------------------------
//
void CBubbleOutlookBottom::ReadBubbleHeader( CBubbleHeader& aHeader )
    {
    iHeader = &aHeader;
    
    iBubble = iBubbleManager.ResourceManager().ReserveEikImage( ETrue );        
    
    if ( !iCallObjectDisplay )
        {
        GetCall1BubbleBitmaps();    
        }
    
    CBubbleOutlookTwoLined::ReadBubbleHeader( aHeader );
    }

// ---------------------------------------------------------------------------
// CBubbleOutlookBottom::SizeChanged
// called by framwork when the view size is changed
//  
// ---------------------------------------------------------------------------
//
void CBubbleOutlookBottom::SizeChanged()
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
                
            // Prepare frame for dynamic draw
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
// CBubbleOutlookBottom::PositionChanged
// ---------------------------------------------------------------------------
//
void CBubbleOutlookBottom::PositionChanged()
    {
    AknsUtils::RegisterControlPosition( this );
    }

// ---------------------------------------------------------------------------
// CBubbleOutlookBottom::DoCall1Layout
// ---------------------------------------------------------------------------
//    
void CBubbleOutlookBottom::DoCall1Layout()
    {
    // Deprecated
    }

// ---------------------------------------------------------------------------
// CBubbleOutlookBottom::DoCall2Layout
// ---------------------------------------------------------------------------
//
void CBubbleOutlookBottom::DoCall2LayoutL()
    {
    iBubble->SetPictureOwnedExternally( EFalse );
    iBubble->SetPicture( NULL, NULL );
        
    const TRect rect = Rect();
    CBubbleManager::TPhoneCallState callState = iHeader->CallState();
    switch ( callState )
        {
        case CBubbleManager::EWaiting:
        case CBubbleManager::EIncoming:
        case CBubbleManager::EOutgoing:
        case CBubbleManager::EAlerting:
        case CBubbleManager::EAlertToDisconnected:
            {
            ///////////////////////////////////////////////////////////////////
            // Incoming call layout for single call
            ///////////////////////////////////////////////////////////////////
            
            TAknLayoutRect bubbleRect;
            bubbleRect.LayoutRect( 
                rect, 
                BubbleLayout2::popup_call2_audio_in_background(1) );
            
            TRect frameRect( TPoint(0,0), bubbleRect.Rect().Size() );
            TRect outerRect;
            TRect innerRect;
            BubbleLayout2::RectFrameInnerOuterRects( frameRect, 
                                                     outerRect, 
                                                     innerRect);
            
            iFrameId = ( callState == CBubbleManager::EAlertToDisconnected ) ?
                                      KAknsIIDQsnFrCall2RectDisconn : 
                                      KAknsIIDQsnFrCall2Rect;
            
            // for possible frame draw
            iOuterRect = outerRect;
            iOuterRect.Move( Rect().iTl );
            iInnerRect = innerRect;
            iInnerRect.Move( Rect().iTl );

            // Call indicator
            BubbleUtils::LayoutCustomElement(
                iBigCallIndicator, 
                rect, 
                BubbleLayout2::popup_call2_audio_in_call_waiting_icon(1) );
            
            // Ciphering off
            BubbleUtils::LayoutControl( 
                iCyphOffImage, 
                rect,
                BubbleLayout2::popup_call2_audio_in_ciphering_icon(1) );                
            
            // Number type icon
            BubbleUtils::LayoutCustomElement(
                iNumberType, 
                rect, 
                BubbleLayout2::popup_call2_audio_in_call_number_type_icon(1) );

            // Text line 1
            BubbleUtils::LayoutLabel( 
                iTextLine1, 
                rect, 
                BubbleLayout2::popup_call2_audio_in_call_text_1(1) );
            
            // Smaller rect for text if brand image exists
            if ( iBrandImage )
                {
                // Text line 2
                BubbleUtils::LayoutLabel(
                    iTextLine2,
                    rect,
                    BubbleLayout2::popup_call2_audio_in_call_text_2(9) );

                //Brand image
                BubbleUtils::LayoutCustomElement(
                    iBrandImage,
                    rect,
                    BubbleLayout2::popup_call2_audio_in_brand_image(3) );
                }
            else
                {
                // Text line 2
                BubbleUtils::LayoutLabel(
                    iTextLine2,
                    rect,
                    BubbleLayout2::popup_call2_audio_in_call_text_2(1) );
                }
            
            // Build call object bubble
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
            // First call layout for single call
            ///////////////////////////////////////////////////////////////////
            
            TAknLayoutRect bubbleRect;
            bubbleRect.LayoutRect( 
                rect, 
                BubbleLayout2::popup_call2_audio_first_call_background(14) );
            
            TRect frameRect( TPoint(0,0), bubbleRect.Rect().Size() );
            TRect outerRect;
            TRect innerRect;
            BubbleLayout2::BubbleFrameInnerOuterRects( frameRect, 
                                                       outerRect, 
                                                       innerRect);
        
            TAknsItemID coMaskFrameId;
            if ( callState == CBubbleManager::EOnHold )
                {
                iFrameId = KAknsIIDQsnFrCall2Bubble;
                coMaskFrameId = KAknsIIDQsnFrCall2BubbleCoMask;    
                }
            else if ( callState == CBubbleManager::EDisconnected )
                {
                iFrameId = KAknsIIDQsnFrCall2BubbleFirstDisconn;
                coMaskFrameId = KAknsIIDQsnFrCall2BubbleFirstCoMask;            
                }
            else
                {
                iFrameId = KAknsIIDQsnFrCall2BubbleFirst;
                coMaskFrameId = KAknsIIDQsnFrCall2BubbleFirstCoMask;                
                }                
            
            // for possible frame draw
            iOuterRect = outerRect;
            iOuterRect.Move( Rect().iTl );
            iInnerRect = innerRect;
            iInnerRect.Move( Rect().iTl );

            // Call icon
            BubbleUtils::LayoutCustomElement( 
                iSmallCallIndication, 
                rect,
                BubbleLayout2::popup_call2_audio_first_call_status_icon(13) );
                
            // Ciphering off
            BubbleUtils::LayoutControl( 
                iCyphOffImage, 
                rect,
                BubbleLayout2::popup_call2_audio_first_call_window_ciphering_icon(12) );
            
            // Call type pane
            BubbleUtils::LayoutCallTypeIndicators(
                rect,
                BubbleLayout2::popup_call2_audio_first_call_type_icon(12),
                iTypeIndication1,   // Data/Fax
                iTypeIndication2 ); // ALS line 2               

            // Text line 1
            BubbleUtils::LayoutLabel( 
                iTextLine1, 
                rect, 
                BubbleLayout2::popup_call2_audio_first_call_cli_text(16) );
            
            // Smaller rect for text if bran image exists
            if ( iBrandImage )
                {
                // Text line 2 - call state
                BubbleUtils::LayoutLabel(
                    iTextLine2,
                    rect,
                    BubbleLayout2::popup_call2_audio_first_call_state_text(16) );

                // Text line 2 - call timer
                BubbleUtils::LayoutLabel(
                    iTimerCost,
                    rect,
                    BubbleLayout2::popup_call2_audio_first_call_state_text(17) );

                //Brand image
                BubbleUtils::LayoutCustomElement(
                    iBrandImage,
                    rect,
            	    BubbleLayout2::popup_call2_audio_first_brand_image(1) );
                }
            else
                {
                // Text line 2 - call state
                BubbleUtils::LayoutLabel(
                    iTextLine2,
                    rect,
                    BubbleLayout2::popup_call2_audio_first_call_state_text(12) );
                // Text line 2 - call timer
                BubbleUtils::LayoutLabel(
                    iTimerCost,
                   	rect,
                    BubbleLayout2::popup_call2_audio_first_call_state_text(13) );
                }
                
            // Build call object bubble
            iBubble->SetRect( bubbleRect.Rect() );                                

            BubbleUtils::PrepareBubbleImageL( 
                iFrameId,
                outerRect,
                innerRect,
                iBubble );
            
            // Add transparency
            BubbleUtils::AddTransparencyToBubbleImageL( 
                coMaskFrameId,
                outerRect,
                innerRect,
                iBubble );
                
                
            break;
            }
        default:
            break;
        }
    }

// ---------------------------------------------------------------------------
// CBubbleOutlookBottom::GetCall1BubbleBitmaps
// ---------------------------------------------------------------------------
//    
void CBubbleOutlookBottom::GetCall1BubbleBitmaps()
    {
    CBubbleManager::TPhoneCallState callState = iHeader->CallState();
    
    TInt bubblePicture = KErrNotFound;
    TInt bubblePictureMask = KErrNotFound;
    
    // set call indications and bubble
    switch ( callState )
        {
        case CBubbleManager::EWaiting:
        case CBubbleManager::EIncoming:
        case CBubbleManager::EOutgoing:
        case CBubbleManager::EAlerting:
            bubblePicture = EQgn_graf_call_rec_big;
            bubblePictureMask = EQgn_graf_call_rec_big_mask;
            break;
        case CBubbleManager::EAlertToDisconnected:
            bubblePicture = EQgn_graf_call_rec_big_disconn;
            bubblePictureMask = EQgn_graf_call_rec_big_disconn_mask;
            break;
        case CBubbleManager::EActive:
            bubblePicture = EQgn_graf_call_first_one_active;
            bubblePictureMask = EQgn_graf_call_first_one_active_mask;
            break;
        case CBubbleManager::EOnHold:
            bubblePicture = EQgn_graf_call_first_one_held;
            bubblePictureMask = EQgn_graf_call_first_one_held_mask;
            break;
        case CBubbleManager::EDisconnected:
            bubblePicture = EQgn_graf_call_first_one_disconn;
            bubblePictureMask = EQgn_graf_call_first_one_disconn_mask;
            break;
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
