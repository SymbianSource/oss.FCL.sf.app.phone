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
* Description:  Bubble to display gallery images.
*
*/


// INCLUDE FILES
#include    "BMBubbleManager.h" //for enumerations
#include    "BMBubbleOutlookBottomImage.h"
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
#include    <AknUtils.h>
#include    <AknsDrawUtils.h>

// ========================= MEMBER FUNCTIONS ================================

// ---------------------------------------------------------------------------
// CBubbleOutlookBottomImage::CBubbleOutlookBottomImage
//
//  
// ---------------------------------------------------------------------------
//
CBubbleOutlookBottomImage::CBubbleOutlookBottomImage( 
    CBubbleManager& aBubbleManager ) : CBubbleOutlookTwoLined( aBubbleManager )
    {
    }

// ---------------------------------------------------------------------------
// CBubbleOutlookBottomImage::ConstructL
// 
// ---------------------------------------------------------------------------
//
void CBubbleOutlookBottomImage::ConstructL()
    { 
    CBubbleOutlookTwoLined::ConstructL();
    }

// ---------------------------------------------------------------------------
// CBubbleOutlookBottomImage::~CBubbleOutlookBottomImage
//
//  
// ---------------------------------------------------------------------------
//
CBubbleOutlookBottomImage::~CBubbleOutlookBottomImage()
    {
    Reset();
    }

// ---------------------------------------------------------------------------
// CBubbleOutlookBottomImage::ReadBubbleHeader
//
//  
// ---------------------------------------------------------------------------
//
void CBubbleOutlookBottomImage::ReadBubbleHeader( CBubbleHeader& aHeader )
    {
    iHeader = &aHeader;
    
    CBubbleManager::TPhoneCallState callState = iHeader->CallState();
    
    iBubble = iBubbleManager.ResourceManager().ReserveEikImage( ETrue );        
    
    CBubbleOutlookTwoLined::ReadBubbleHeader( aHeader );
    }

// ---------------------------------------------------------------------------
// CBubbleOutlookBottomImage::SizeChanged
// called by framework when the view size is changed
//  
// ---------------------------------------------------------------------------
//
void CBubbleOutlookBottomImage::SizeChanged()
    {
    if( !iHeader )
        {
        return;
        }

    AknsUtils::RegisterControlPosition( this );
    
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

// ---------------------------------------------------------------------------
// CBubbleOutlookBottomImage::PositionChanged
// ---------------------------------------------------------------------------
//
void CBubbleOutlookBottomImage::PositionChanged()
    {
    AknsUtils::RegisterControlPosition( this );
    }

// ---------------------------------------------------------------------------
// CBubbleOutlookBottomImage::DoCall2Layout
// ---------------------------------------------------------------------------
//
void CBubbleOutlookBottomImage::DoCall2LayoutL()
    {
    iBubble->SetPictureOwnedExternally( EFalse );
    iBubble->SetPicture( NULL, NULL );
    
    const TRect rect = Rect();
    CBubbleManager::TPhoneCallState callState = iHeader->CallState();
    switch ( callState )
        {
        case CBubbleManager::EDisconnected:
        case CBubbleManager::EActive:
        case CBubbleManager::EOnHold:
            {
            ///////////////////////////////////////////////////////////////////
            // First incall layout for single call. Call object image.
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

            // Smaller rect for text if brand image exists
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
                    BubbleLayout2::popup_call2_audio_first_brand_image2(1) );
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
                
            // Create call object bitmaps
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

// End of File
