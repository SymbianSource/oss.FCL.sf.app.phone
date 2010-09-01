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
* Description:  Outlook Bottom Right
*
*/


// INCLUDE FILES
#include    "BMBubbleManager.h" //for enumerations
#include    "BMBubbleOutlookBottomRight.h"
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
// CBubbleOutlookBottomRight::CBubbleOutlookBottomRight
//  
// ---------------------------------------------------------------------------
//
CBubbleOutlookBottomRight::CBubbleOutlookBottomRight( 
    CBubbleManager& aBubbleManager ) 
    : CBubbleOutlookTwoLined( aBubbleManager ) 
    {
    }

// ---------------------------------------------------------------------------
// CBubbleOutlookBottomRight::ConstructL
// 
// ---------------------------------------------------------------------------
//

void CBubbleOutlookBottomRight::ConstructL()
    {  
    CBubbleOutlookTwoLined::ConstructL();
    }

// ---------------------------------------------------------------------------
// CBubbleOutlookBottomRight::~CBubbleOutlookBottomRight
//
//  
// ---------------------------------------------------------------------------
//
CBubbleOutlookBottomRight::~CBubbleOutlookBottomRight()
    {
    Reset();
    }

// ---------------------------------------------------------------------------
// CBubbleOutlookBottomRight::ReadBubbleHeader
//
//  
// ---------------------------------------------------------------------------
//
void CBubbleOutlookBottomRight::ReadBubbleHeader( CBubbleHeader& aHeader )
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
// CBubbleOutlookBottomRight::SizeChanged
// called by framwork when the view size is changed
//  
// ---------------------------------------------------------------------------
//
void CBubbleOutlookBottomRight::SizeChanged()
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
// CBubbleOutlookBottomRight::DoCall1Layout
// ---------------------------------------------------------------------------
//    
void CBubbleOutlookBottomRight::DoCall1Layout()
    {
    // Deprecated
    }

// ---------------------------------------------------------------------------
// CBubbleOutlookBottomRight::DoCall2Layout
// ---------------------------------------------------------------------------
//
void CBubbleOutlookBottomRight::DoCall2LayoutL()
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
            // Outgoing call layout for second call.
            ///////////////////////////////////////////////////////////////////
            
            TAknLayoutRect bubbleRect;
            bubbleRect.LayoutRect( 
                rect, 
                BubbleLayout2::popup_call2_audio_out_background(1) );
            
            TRect frameRect( TPoint(0,0), bubbleRect.Rect().Size() );
            TRect outerRect;
            TRect innerRect;
            BubbleLayout2::RectFrameInnerOuterRects( frameRect, 
                                                     outerRect, 
                                                     innerRect);
            
            iFrameId = ( callState == CBubbleManager::EAlertToDisconnected ) ?
                                      KAknsIIDQsnFrCall2RectDisconn : 
                                      KAknsIIDQsnFrCall2Rect;
            
            iOuterRect = outerRect;
            iOuterRect.Move( Rect().iTl );
            iInnerRect = innerRect;
            iInnerRect.Move( Rect().iTl );
            
            
            // Call indicator
            BubbleUtils::LayoutCustomElement(
                iBigCallIndicator, 
                rect, 
                BubbleLayout2::popup_call2_audio_out_call_waiting_icon(1) );
            
            // Ciphering off
            BubbleUtils::LayoutControl( 
                iCyphOffImage, 
                rect,
                BubbleLayout2::popup_call2_audio_out_ciphering_icon(1) );                
            
            // Number type icon
            BubbleUtils::LayoutCustomElement(
                iNumberType, 
                rect, 
                BubbleLayout2::popup_call2_audio_out_call_number_type_icon(1) );

            // Text line 1
            BubbleUtils::LayoutLabel( 
                iTextLine1, 
                rect, 
                BubbleLayout2::popup_call2_audio_out_call_text_1(1) );
            
            // Smaller rect for text if brand image exists
            if ( iBrandImage )
                {
                // Text line 2
                BubbleUtils::LayoutLabel(
                    iTextLine2,
                    rect,
                    BubbleLayout2::popup_call2_audio_out_call_text_2(3) );

                //Brand image
                BubbleUtils::LayoutCustomElement(
                    iBrandImage,
                   	rect,
                    BubbleLayout2::popup_call2_audio_out_brand_image(0) );
                }
            else
                {
                // Text line 2
                BubbleUtils::LayoutLabel(
                    iTextLine2,
                    rect,
                    BubbleLayout2::popup_call2_audio_out_call_text_2(1) );
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
        case CBubbleManager::EDisconnected:
        case CBubbleManager::EActive:
        case CBubbleManager::EOnHold:
            {
            ///////////////////////////////////////////////////////////////////
            // Second call layout.
            ///////////////////////////////////////////////////////////////////
            
            // Create bubble image
            TAknLayoutRect bubbleRect;
            bubbleRect.LayoutRect( 
                rect, 
                BubbleLayout2::popup_call2_audio_second_call_background(9) );
            
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
                iFrameId = KAknsIIDQsnFrCall2BubbleSecondDisconn;
                }
            else
                {
                iFrameId = KAknsIIDQsnFrCall2BubbleSecond;
                }
            
            iOuterRect = outerRect;
            iOuterRect.Move( Rect().iTl );
            iInnerRect = innerRect;
            iInnerRect.Move( Rect().iTl );

            // Call icon
            BubbleUtils::LayoutCustomElement( 
                iSmallCallIndication, 
                rect,
                BubbleLayout2::popup_call2_audio_second_call_status_icon(9) );
                
            // Ciphering off
            BubbleUtils::LayoutControl( 
                iCyphOffImage, 
                rect,
                BubbleLayout2::popup_call2_audio_second_call_cyphering_icon(9) );
            
            // Call type pane    
            BubbleUtils::LayoutCallTypeIndicators(
                rect,
                BubbleLayout2::popup_call2_audio_second_call_type_icon(9),
                iTypeIndication1,   // Data/Fax
                iTypeIndication2 ); // ALS line 2

            // Text line 1
            BubbleUtils::LayoutLabel( 
                iTextLine1, 
                rect, 
                BubbleLayout2::popup_call2_audio_second_call_cli_text(11) );
            
            // Smaller rect for text if brand image exists
            if ( iBrandImage )
                {
                // Text line 2 - call state
                BubbleUtils::LayoutLabel(
                    iTextLine2,
                    rect,
                    BubbleLayout2::popup_call2_audio_second_call_state_text(5) );

                // Text line 2 - call timer
                BubbleUtils::LayoutLabel(
                    iTimerCost,
                    rect,
                    BubbleLayout2::popup_call2_audio_second_call_timer_text(5) );

                //Brand image
                BubbleUtils::LayoutCustomElement(
            	    iBrandImage,
                    rect,
                    BubbleLayout2::popup_call2_audio_second_brand_image(0) );
                }
            else
                {
                // Text line 2 - call state
                BubbleUtils::LayoutLabel(
                    iTextLine2,
                    rect,
                    BubbleLayout2::popup_call2_audio_second_call_state_text(4) );

                // Text line 2 - call timer
                BubbleUtils::LayoutLabel(
                    iTimerCost,
                    rect,
                    BubbleLayout2::popup_call2_audio_second_call_timer_text(4) );
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

// ---------------------------------------------------------------------------
// CBubbleOutlookBottomRight::GetCall1BubbleBitmaps
// ---------------------------------------------------------------------------
//    
void CBubbleOutlookBottomRight::GetCall1BubbleBitmaps()
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
            bubblePicture = EQgn_graf_call_rec_big_right;
            bubblePictureMask = EQgn_graf_call_rec_big_right_mask;
            break;
        case CBubbleManager::EAlertToDisconnected:
            bubblePicture = EQgn_graf_call_rec_big_right_disconn;
            bubblePictureMask = EQgn_graf_call_rec_big_right_disconn_mask;
            break;
        case CBubbleManager::EActive:
            bubblePicture = EQgn_graf_call_second_two_active;
            bubblePictureMask = EQgn_graf_call_second_two_active_mask;
            break;
        case CBubbleManager::EOnHold:
            bubblePicture = EQgn_graf_call_second_two_held;
            bubblePictureMask = EQgn_graf_call_second_two_held_mask;
            break;
        case CBubbleManager::EDisconnected:
            bubblePicture = EQgn_graf_call_second_two_disconn;
            bubblePictureMask = EQgn_graf_call_second_two_disconn_mask;
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
