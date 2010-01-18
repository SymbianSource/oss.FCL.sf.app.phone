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
* Description:  Bottom outlook with 3-lines and call image.
*
*/


// INCLUDE FILES
#include    "BMBubbleManager.h" //for enumerations
#include    "BMBubbleOutlookBottomImCnap.h"
#include    "BMBubbleImageManager.h"
#include    "BMResourceManager.h"
#include    "BMBubbleHeader.h"
#include    "BMLayout.h"
#include    "BMUtils.h"
#include    "BMLayout2.h"
#include    <telbubblecustomelement.h>

#include    <eiklabel.h> 
#include    <eikimage.h>
#include    <eikenv.h>
#include    <AknsUtils.h>
#include    <AknsDrawUtils.h>

// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//
CBubbleOutlookBottomImageCnap::CBubbleOutlookBottomImageCnap
    ( CBubbleManager& aBubbleManager )
    : CBubbleOutlookThreeLined( aBubbleManager )
    {
    }

// Symbian OS constructor can leave.
void CBubbleOutlookBottomImageCnap::ConstructL()
    {
    CBubbleOutlookThreeLined::ConstructL();
    }

    
// Destructor
CBubbleOutlookBottomImageCnap::~CBubbleOutlookBottomImageCnap()
    {
    Reset();
    }


// ---------------------------------------------------------------------------
// CBubbleOutlookBottomImageCnap::ReadBubbleHeader
//
//  
// ---------------------------------------------------------------------------
//
void CBubbleOutlookBottomImageCnap::ReadBubbleHeader( CBubbleHeader& aHeader )
    {
    iHeader = &aHeader;
    
    iBubble = iBubbleManager.ResourceManager().ReserveEikImage( ETrue );        
    
    CBubbleOutlookThreeLined::ReadBubbleHeader( aHeader );
    }

// ---------------------------------------------------------------------------
// CBubbleOutlookBottomImageCnap::SizeChanged
// called by framwork when the view size is changed
//  
// ---------------------------------------------------------------------------
//
void CBubbleOutlookBottomImageCnap::SizeChanged()
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
// CBubbleOutlookBottomImageCnap::PositionChanged
// ---------------------------------------------------------------------------
//
void CBubbleOutlookBottomImageCnap::PositionChanged()
    {
    AknsUtils::RegisterControlPosition( this );
    }

// ---------------------------------------------------------------------------
// CBubbleOutlookBottomImageCnap::DoCall1Layout
// ---------------------------------------------------------------------------
//    
void CBubbleOutlookBottomImageCnap::DoCall1Layout()
    {
    }

// ---------------------------------------------------------------------------
// CBubbleOutlookBottomImageCnap::DoCall2Layout
// ---------------------------------------------------------------------------
//
void CBubbleOutlookBottomImageCnap::DoCall2LayoutL()
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
                // Text line 2 - call state
                BubbleUtils::LayoutLabel(
                    iTextLine3,
                    rect,
                    BubbleLayout2::popup_call2_audio_first_call_state_text(18) );

                // Text line 2 - call timer
                BubbleUtils::LayoutLabel(
                    iTimerCost,
                    rect,
                    BubbleLayout2::popup_call2_audio_first_call_state_text(20) );

                //Brand image
                BubbleUtils::LayoutCustomElement(
                    iBrandImage,
                    rect,
                    BubbleLayout2::popup_call2_audio_first_brand_image2(22) );
                }
            else
                {
                // Text line 2 - call state
                BubbleUtils::LayoutLabel(
                    iTextLine3,
                    rect,
                    BubbleLayout2::popup_call2_audio_first_call_state_text(18) );

                // Text line 2 - call timer
                BubbleUtils::LayoutLabel(
                    iTimerCost,
                    rect,
                    BubbleLayout2::popup_call2_audio_first_call_state_text(20) );
                }
                
            // Create call object bitmaps
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
// CBubbleOutlookBottomImageCnap::GetCall1BubbleBitmaps
// ---------------------------------------------------------------------------
//    
void CBubbleOutlookBottomImageCnap::GetCall1BubbleBitmaps()
    {
    }

//  End of File  
