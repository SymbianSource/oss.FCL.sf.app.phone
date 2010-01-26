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
* Description: Implementation of CBubbleOutlookBottomCnap class.
*
*/


// INCLUDE FILES
#include    "BMBubbleManager.h" //for enumerations
#include    "BMBubbleOutlookBottomCnap.h"
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
CBubbleOutlookBottomCnap::CBubbleOutlookBottomCnap
    ( CBubbleManager& aBubbleManager )
    : CBubbleOutlookThreeLined( aBubbleManager )
    {
    }

// Symbian OS constructor can leave.
void CBubbleOutlookBottomCnap::ConstructL()
    {
    CBubbleOutlookThreeLined::ConstructL();
    }

    
// Destructor
CBubbleOutlookBottomCnap::~CBubbleOutlookBottomCnap()
    {
    Reset();
    }


// ---------------------------------------------------------------------------
// CBubbleOutlookBottomCnap::ReadBubbleHeader
//
//  
// ---------------------------------------------------------------------------
//
void CBubbleOutlookBottomCnap::ReadBubbleHeader( CBubbleHeader& aHeader )
    {
    iHeader = &aHeader;
    
    CBubbleManager::TPhoneCallState callState = iHeader->CallState();
    
    iBubble = iBubbleManager.ResourceManager().ReserveEikImage( ETrue );        
    
    if ( !iCallObjectDisplay )
        {
        GetCall1BubbleBitmaps();    
        }
    
    CBubbleOutlookThreeLined::ReadBubbleHeader( aHeader );
    }

// ---------------------------------------------------------------------------
// CBubbleOutlookBottomCnap::SizeChanged
// called by framwork when the view size is changed
//  
// ---------------------------------------------------------------------------
//
void CBubbleOutlookBottomCnap::SizeChanged()
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
// CBubbleOutlookBottomCnap::PositionChanged
// ---------------------------------------------------------------------------
//
void CBubbleOutlookBottomCnap::PositionChanged()
    {
    AknsUtils::RegisterControlPosition( this );
    }

// ---------------------------------------------------------------------------
// CBubbleOutlookBottomCnap::DoCall1Layout
// ---------------------------------------------------------------------------
//    
void CBubbleOutlookBottomCnap::DoCall1Layout()
    {
    // Deprecated
    }

// ---------------------------------------------------------------------------
// CBubbleOutlookBottomCnap::DoCall2Layout
// ---------------------------------------------------------------------------
//
void CBubbleOutlookBottomCnap::DoCall2LayoutL()
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
            // Incoming call layout for single call. Three lines of text.
            ///////////////////////////////////////////////////////////////////
            
            TAknLayoutRect bubbleRect;
            bubbleRect.LayoutRect( 
                rect, 
                BubbleLayout2::popup_call2_audio_in_background(2) );
            
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
                BubbleLayout2::popup_call2_audio_in_call_waiting_icon(2) );
            
            // Ciphering off
            BubbleUtils::LayoutControl( 
                iCyphOffImage, 
                rect,
                BubbleLayout2::popup_call2_audio_in_ciphering_icon(2) );                
            
            // Number type icon
            BubbleUtils::LayoutCustomElement(
                iNumberType, 
                rect, 
                BubbleLayout2::popup_call2_audio_in_call_number_type_icon(2) );

            // Text line 1
            BubbleUtils::LayoutLabel( 
                iTextLine1, 
                rect, 
                BubbleLayout2::popup_call2_audio_in_call_text_1(2) );
            
            // Text line 2
            BubbleUtils::LayoutLabel( 
                iTextLine2, 
                rect, 
                BubbleLayout2::popup_call2_audio_in_call_text_2(2) );
                
            // Smaller rect for text if brand image exists
            if ( iBrandImage )
                {
                // Text line 3
                    BubbleUtils::LayoutLabel(
                    iTextLine3,
                    rect,
                    BubbleLayout2::popup_call2_audio_in_call_text_3(3) );

                //Brand image
                BubbleUtils::LayoutCustomElement(
                    iBrandImage,
                    rect,
                    BubbleLayout2::popup_call2_audio_in_brand_image(0) );
                }
            else
                {
                // Text line 3
                BubbleUtils::LayoutLabel(
                    iTextLine3,
                    rect,
                    BubbleLayout2::popup_call2_audio_in_call_text_3(0) );
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
        
        case CBubbleManager::EDisconnected:
        case CBubbleManager::EActive:
        case CBubbleManager::EOnHold:
            {
            ///////////////////////////////////////////////////////////////////
            // First call layout for single call. Three lines of text.
            ///////////////////////////////////////////////////////////////////
            
            TAknLayoutRect bubbleRect;
            bubbleRect.LayoutRect( 
                rect, 
                BubbleLayout2::popup_call2_audio_first_call_background(20) );
            
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
                BubbleLayout2::popup_call2_audio_first_call_status_icon(19) );
                
            // Ciphering off
            BubbleUtils::LayoutControl( 
                iCyphOffImage, 
                rect,
                BubbleLayout2::popup_call2_audio_first_call_window_ciphering_icon(16) );
            
            // Call type pane
            BubbleUtils::LayoutCallTypeIndicators(
                rect,
                BubbleLayout2::popup_call2_audio_first_call_type_icon(16),
                iTypeIndication1,   // Data/Fax
                iTypeIndication2 ); // ALS line 2               

            // Text line 1 - cli
            BubbleUtils::LayoutLabel( 
                iTextLine1, 
                rect, 
                BubbleLayout2::popup_call2_audio_first_call_cli_text(22) );
            
            // Text line 2 - phone number
            BubbleUtils::LayoutLabel( 
                iTextLine2, 
                rect, 
                BubbleLayout2::popup_call2_audio_first_call_number_text(18) );
            
            // Smaller rect for text if brand image exists
            if ( iBrandImage )
                {
                // Text line 3
                    BubbleUtils::LayoutLabel(
                    iTextLine3,
                    rect,
                    BubbleLayout2::popup_call2_audio_in_call_text_3(3) );

                //Brand image
                BubbleUtils::LayoutCustomElement(
                    iBrandImage,
                    rect,
                    BubbleLayout2::popup_call2_audio_in_brand_image(0) );
                }
            else
                {
                // Text line 3
                BubbleUtils::LayoutLabel(
                    iTextLine3,
                    rect,
                    BubbleLayout2::popup_call2_audio_first_call_state_text(18) );
                }
            
            // Line 3 - call timer
            BubbleUtils::LayoutLabel( 
                iTimerCost, 
                rect, 
                BubbleLayout2::popup_call2_audio_first_call_state_text(19) );
                
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
// CBubbleOutlookBottomCnap::GetCall1BubbleBitmaps
// ---------------------------------------------------------------------------
//    
void CBubbleOutlookBottomCnap::GetCall1BubbleBitmaps()
    {
    CBubbleManager::TPhoneCallState callState = iHeader->CallState();
    
    TInt bubblePicture = KErrNotFound;
    TInt bubblePictureMask = KErrNotFound;
    
    switch ( callState )
        {
        case CBubbleManager::EWaiting:
        case CBubbleManager::EIncoming:
            bubblePicture = EQgn_graf_call_rec_bigger;
            bubblePictureMask = EQgn_graf_call_rec_bigger_mask;
            break;
        case CBubbleManager::EActive:
            bubblePicture = EQgn_graf_call_first_one_active_emergency;
            bubblePictureMask = EQgn_graf_call_first_one_active_emergency_mask;
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
