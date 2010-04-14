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
* Description: Implementation of CBubbleOutMiddleCnap class.
*
*/


// INCLUDE FILES
#include    "BMBubbleManager.h" //for enumerations
#include    "BMBubbleOutlookMiddleCnap.h"
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
CBubbleOutlookMiddleCnap::CBubbleOutlookMiddleCnap
    ( CBubbleManager& aBubbleManager )
    : CBubbleOutlookThreeLined( aBubbleManager )
    {
    }

// Symbian OS default constructor can leave.
void CBubbleOutlookMiddleCnap::ConstructL()
    {
    CBubbleOutlookThreeLined::ConstructL();
    }

    
// Destructor
CBubbleOutlookMiddleCnap::~CBubbleOutlookMiddleCnap()
    {
    Reset();
    }


// ---------------------------------------------------------------------------
// CBubbleOutlookMiddleCnap::ReadBubbleHeader
//
//  
// ---------------------------------------------------------------------------
//
void CBubbleOutlookMiddleCnap::ReadBubbleHeader( CBubbleHeader& aHeader )
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
// CBubbleOutlookMiddleCnap::SizeChanged
// called by framwork when the view size is changed
//  
// ---------------------------------------------------------------------------
//
void CBubbleOutlookMiddleCnap::SizeChanged()
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
// CBubbleOutlookMiddleCnap::DoCall1Layout
// ---------------------------------------------------------------------------
//    
void CBubbleOutlookMiddleCnap::DoCall1Layout()
    {
    // Deprecated
    }

// ---------------------------------------------------------------------------
// CBubbleOutlookMiddleCnap::DoCall2Layout
// ---------------------------------------------------------------------------
//
void CBubbleOutlookMiddleCnap::DoCall2LayoutL()
    {
    iBubble->SetPictureOwnedExternally( EFalse );
    iBubble->SetPicture( NULL, NULL );
    
    const TRect rect = Rect();
    CBubbleManager::TPhoneCallState callState = iHeader->CallState();
    switch ( callState )
        {
        case CBubbleManager::EIncoming:
        case CBubbleManager::EOutgoing:
        case CBubbleManager::EAlerting:
        case CBubbleManager::EWaiting:
        case CBubbleManager::EAlertToDisconnected:
            {
            ///////////////////////////////////////////////////////////////////
            // Incoming call layout. Three lines of text. Number entry open.
            ///////////////////////////////////////////////////////////////////
            
            // Layout create bubble image
            TAknLayoutRect bubbleRect;
            bubbleRect.LayoutRect( 
                rect, 
                BubbleLayout2::popup_call2_audio_in_background(6) );
            
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
                BubbleLayout2::popup_call2_audio_in_ciphering_icon(6) );                
            
            // Number type icon
            BubbleUtils::LayoutCustomElement(
                iNumberType, 
                rect, 
                BubbleLayout2::popup_call2_audio_in_call_number_type_icon(6) );

            // Text line 1
            BubbleUtils::LayoutLabel( 
                iTextLine1, 
                rect, 
                BubbleLayout2::popup_call2_audio_in_call_text_1(6) );
            
            // Text line 2
            BubbleUtils::LayoutLabel( 
                iTextLine2, 
                rect, 
                BubbleLayout2::popup_call2_audio_in_call_text_2(6) );

            // Smaller rect for text if brand image exists
            if ( iBrandImage )
                {
                // Text line 3
                BubbleUtils::LayoutLabel(
                    iTextLine3,
                    rect,
                    BubbleLayout2::popup_call2_audio_in_call_text_3(4) );

                //Brand image
                BubbleUtils::LayoutCustomElement(
            	    iBrandImage,
                    rect,
                    BubbleLayout2::popup_call2_audio_in_brand_image(2) );
                }
            else
                {
                // Text line 3
                BubbleUtils::LayoutLabel(
                    iTextLine3,
                    rect,
                    BubbleLayout2::popup_call2_audio_in_call_text_3(2) );
                }
                
            // Create call object bitmaps
            iBubble->SetRect( bubbleRect.Rect() );                

            BubbleUtils::PrepareBubbleImageL( 
                iFrameId,
                outerRect,
                innerRect,
                iBubble );
                
            if ( iHeader->CallObjectImage()  &&
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
            // First call layout. Three lines of text. Two calls.  
            ///////////////////////////////////////////////////////////////////

            TAknLayoutRect bubbleRect;
            bubbleRect.LayoutRect( 
                rect, 
                BubbleLayout2::popup_call2_audio_first_call_background(21) );
            
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
                BubbleLayout2::popup_call2_audio_first_call_status_icon(20) );
                
            // Ciphering off
            BubbleUtils::LayoutControl( 
                iCyphOffImage, 
                rect,
                BubbleLayout2::popup_call2_audio_first_call_window_ciphering_icon(17) );
            
            // Call type pane
            BubbleUtils::LayoutCallTypeIndicators(
                rect,
                BubbleLayout2::popup_call2_audio_first_call_type_icon(17),
                iTypeIndication1,   // Data/Fax
                iTypeIndication2 ); // ALS line 2               

            // Text line 1
            BubbleUtils::LayoutLabel( 
                iTextLine1, 
                rect, 
                BubbleLayout2::popup_call2_audio_first_call_cli_text(23) );
            
            // Text line 2 - phone number
            BubbleUtils::LayoutLabel( 
                iTextLine2, 
                rect, 
                BubbleLayout2::popup_call2_audio_first_call_number_text(19) );
            
            // Text line 2 - call state
            BubbleUtils::LayoutLabel( 
                iTextLine3, 
                rect, 
                BubbleLayout2::popup_call2_audio_first_call_state_text(19) );
            
            // Text line 2 - call timer
            BubbleUtils::LayoutLabel( 
                iTimerCost, 
                rect, 
                BubbleLayout2::popup_call2_audio_first_call_state_text(21) );
                
            // Build call object bubble
            iBubble->SetRect( bubbleRect.Rect() );                                

            BubbleUtils::PrepareBubbleImageL( 
                iFrameId,
                outerRect,
                innerRect,
                iBubble );
                           
            if ( iHeader->CallObjectImage() )
                {
                BubbleUtils::PrepareCallObjectImageL(
                    *iHeader,
                    iBubbleManager.CallObjectImageIncallSize() );
                
                TAknLayoutRect cliRect;
                cliRect.LayoutRect( 
                    frameRect, 
                    BubbleLayout2::popup_call2_bubble_pane_graphics_cli() );
                    
                // Combine image to bubble image
                BubbleUtils::PrepareCallObjectToBubbleImageL(
                    iHeader->CallObjectImage(),
                    iHeader->CallObjectImageMask(),
                    cliRect.Rect(),
                    coMaskFrameId, // gradient image mask
                    outerRect,
                    innerRect,
                    iBubble );
                }
                
            break;
            }
            
        default:
            break;
        }
    }

// ---------------------------------------------------------------------------
// CBubbleOutlookMiddleCnap::GetCall1BubbleBitmaps
// ---------------------------------------------------------------------------
//    
void CBubbleOutlookMiddleCnap::GetCall1BubbleBitmaps()
    {
    CBubbleManager::TPhoneCallState callState = iHeader->CallState();
    
    TInt bubblePicture = KErrNotFound;
    TInt bubblePictureMask = KErrNotFound;
    
    switch ( callState )
        {
        case CBubbleManager::EWaiting:
        case CBubbleManager::EIncoming:
            bubblePicture = EQgn_graf_call_rec_big_left;
            bubblePictureMask = EQgn_graf_call_rec_big_left_mask;
            break;
        case CBubbleManager::EActive:
            bubblePicture = EQgn_graf_call_two_active_emergency;
            bubblePictureMask = EQgn_graf_call_two_active_emergency_mask; 
            break;
        case CBubbleManager::EAlertToDisconnected:
        case CBubbleManager::EOnHold:
        case CBubbleManager::EDisconnected:
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

//  End of File  
