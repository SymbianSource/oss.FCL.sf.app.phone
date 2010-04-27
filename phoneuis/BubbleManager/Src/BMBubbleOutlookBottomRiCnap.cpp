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
* Description:  Bottom right outlook with 3-lines.
*
*/


// INCLUDE FILES
#include    "BMBubbleManager.h" //for enumerations
#include    "BMBubbleOutlookBottomRiCnap.h"
#include    "BMBubbleImageManager.h"
#include    "BMResourceManager.h"
#include    "BMBubbleHeader.h"
#include    "BMLayout.h"
#include    "BMUtils.h"
#include    "BMLayout2.h"
#include    "telbubblecustomelement.h"

#include    <eiklabel.h> 
#include    <eikimage.h>
#include    <eikenv.h>
#include    <AknsUtils.h>
#include    <AknsDrawUtils.h>

// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//
CBubbleOutlookBottomRightCnap::CBubbleOutlookBottomRightCnap
    ( CBubbleManager& aBubbleManager )
    : CBubbleOutlookThreeLined( aBubbleManager )
    {
    }

// Symbian OS constructor can leave.
void CBubbleOutlookBottomRightCnap::ConstructL()
    {
    CBubbleOutlookThreeLined::ConstructL();
    }

    
// Destructor
CBubbleOutlookBottomRightCnap::~CBubbleOutlookBottomRightCnap()
    {
    Reset();
    }


// ---------------------------------------------------------------------------
// CBubbleOutlookBottomRightCnap::ReadBubbleHeader
//
//  
// ---------------------------------------------------------------------------
//
void CBubbleOutlookBottomRightCnap::ReadBubbleHeader( CBubbleHeader& aHeader )
    {
    iHeader = &aHeader;
    
    iBubble = iBubbleManager.ResourceManager().ReserveEikImage( ETrue );        
    
    if ( !iCallObjectDisplay )
        {
        GetCall1BubbleBitmaps();    
        }
    
    CBubbleOutlookThreeLined::ReadBubbleHeader( aHeader );
    }

// ---------------------------------------------------------------------------
// CBubbleOutlookBottomRightCnap::SizeChanged
// called by framwork when the view size is changed
//  
// ---------------------------------------------------------------------------
//
void CBubbleOutlookBottomRightCnap::SizeChanged()
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
// CBubbleOutlookBottomRightCnap::PositionChanged
// ---------------------------------------------------------------------------
//
void CBubbleOutlookBottomRightCnap::PositionChanged()
    {
    AknsUtils::RegisterControlPosition( this );
    }

// ---------------------------------------------------------------------------
// CBubbleOutlookBottomRightCnap::DoCall1Layout
// ---------------------------------------------------------------------------
//    
void CBubbleOutlookBottomRightCnap::DoCall1Layout()
    {
    }

// ---------------------------------------------------------------------------
// CBubbleOutlookBottomRightCnap::DoCall2Layout
// ---------------------------------------------------------------------------
//
void CBubbleOutlookBottomRightCnap::DoCall2LayoutL()
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
            // Outgoing call layout. Three lines of text.
            ///////////////////////////////////////////////////////////////////
            TAknLayoutRect bubbleRect;
            bubbleRect.LayoutRect( 
                rect, 
                BubbleLayout2::popup_call2_audio_out_background(5) );
            
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
                BubbleLayout2::popup_call2_audio_out_call_waiting_icon(5) );

            // Ciphering off
            BubbleUtils::LayoutControl( 
                iCyphOffImage, 
                rect,
                BubbleLayout2::popup_call2_audio_out_ciphering_icon(5) );                
            
            // Number type icon
            BubbleUtils::LayoutCustomElement(
                iNumberType, 
                rect, 
                BubbleLayout2::popup_call2_audio_out_call_number_type_icon(4) );

            // Text line 1
            BubbleUtils::LayoutLabel( 
                iTextLine1, 
                rect, 
                BubbleLayout2::popup_call2_audio_out_call_text_1(5) );
            
            // Smaller rect for text if brand image exists
            if ( iBrandImage )
                {
                // Text line 2
                BubbleUtils::LayoutLabel( 
                    iTextLine2, 
                    rect, 
                    BubbleLayout2::popup_call2_audio_out_call_text_2(5) );            
            
                // Text line 3
                    BubbleUtils::LayoutLabel( 
                        iTextLine3, 
                        rect, 
                        BubbleLayout2::popup_call2_audio_out_call_text_3(5) );
                
                //Brand image
                BubbleUtils::LayoutCustomElement(
                    iBrandImage,
                    rect,
                    BubbleLayout2::popup_call2_audio_out_brand_image(1) );
                }
            else
                {
                // Text line 2
                BubbleUtils::LayoutLabel( 
                    iTextLine2, 
                    rect, 
                    BubbleLayout2::popup_call2_audio_out_call_text_2(4) );            
            
                // Text line 3
                BubbleUtils::LayoutLabel( 
                    iTextLine3, 
                    rect, 
                    BubbleLayout2::popup_call2_audio_out_call_text_3(4) );
                }
                
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
        
        case CBubbleManager::EActive:
        case CBubbleManager::EOnHold:
        case CBubbleManager::EDisconnected:
            {
            ///////////////////////////////////////////////////////////////////
            // Second call layout for single call. Three lines of text.
            ///////////////////////////////////////////////////////////////////
            
            TAknLayoutRect bubbleRect;
            bubbleRect.LayoutRect( 
                rect, 
                BubbleLayout2::popup_call2_audio_second_call_background(14) );
            
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
                iFrameId = KAknsIIDQsnFrCall2BubbleSecondDisconn;
                coMaskFrameId = KAknsIIDQsnFrCall2BubbleSecondCoMask;            
                }
            else
                {
                iFrameId = KAknsIIDQsnFrCall2BubbleSecond;
                coMaskFrameId = KAknsIIDQsnFrCall2BubbleSecondCoMask;                
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
                BubbleLayout2::popup_call2_audio_second_call_status_icon(14) );
                
            // Ciphering off
            BubbleUtils::LayoutControl( 
                iCyphOffImage, 
                rect,
                BubbleLayout2::popup_call2_audio_second_call_cyphering_icon(14) );
            
            // Call type pane
            BubbleUtils::LayoutCallTypeIndicators(
                rect,
                BubbleLayout2::popup_call2_audio_second_call_type_icon(14),
                iTypeIndication1,   // Data/Fax
                iTypeIndication2 ); // ALS line 2               

            // Text line 1
            BubbleUtils::LayoutLabel( 
                iTextLine1, 
                rect, 
                BubbleLayout2::popup_call2_audio_second_call_cli_text(16) );
            
            // Text line 2 - phone number
            BubbleUtils::LayoutLabel( 
                iTextLine2, 
                rect, 
                BubbleLayout2::popup_call2_audio_second_call_number_text(16) );
            
            // Smaller rect for text if brand image exists
            if ( iBrandImage )
                {
                // Text line 3 - call state
                BubbleUtils::LayoutLabel( 
                    iTextLine3, 
                    rect, 
                    BubbleLayout2::popup_call2_audio_second_call_state_text(18) );
                
                // Text line 3 - call timer
                BubbleUtils::LayoutLabel( 
                    iTimerCost, 
                    rect, 
                    BubbleLayout2::popup_call2_audio_second_call_timer_text(7) );
                
                //Brand image
                BubbleUtils::LayoutCustomElement(
                    iBrandImage,
                    rect,
                    BubbleLayout2::popup_call2_audio_second_brand_image(2) );
                }
            else
                {
                // Text line 3 - call state
                BubbleUtils::LayoutLabel( 
                    iTextLine3, 
                    rect, 
                    BubbleLayout2::popup_call2_audio_second_call_state_text(15) );
                
                // Text line 3 - call timer
                BubbleUtils::LayoutLabel( 
                    iTimerCost, 
                    rect, 
                    BubbleLayout2::popup_call2_audio_second_call_timer_text(6) );
                }
                
            // Build call object bubble
            iBubble->SetRect( bubbleRect.Rect() );                                

            BubbleUtils::PrepareBubbleImageL( 
                iFrameId,
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
// CBubbleOutlookBottomRightCnap::GetCall1BubbleBitmaps
// ---------------------------------------------------------------------------
//    
void CBubbleOutlookBottomRightCnap::GetCall1BubbleBitmaps()
    {
    }

//  End of File  
