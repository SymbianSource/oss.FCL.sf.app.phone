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
* Description:  Outlook Top Right
*
*/


// INCLUDE FILES
#include    "BMBubbleManager.h" //for enumerations
#include    "BMBubbleOutlookTopRight.h"
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
// CBubbleOutlookTopRight::CBubbleOutlookTopRight
//
//  
// ---------------------------------------------------------------------------
//
CBubbleOutlookTopRight::CBubbleOutlookTopRight( CBubbleManager& aBubbleManager ) 
: CBubbleOutlookOneLined( aBubbleManager )
    {
    }


// ---------------------------------------------------------------------------
// CBubbleOutlookTopRight::ConstructL
// 
// ---------------------------------------------------------------------------
//
void CBubbleOutlookTopRight::ConstructL()
    {
    CBubbleOutlookOneLined::ConstructL( );
    }

// Destructor
CBubbleOutlookTopRight::~CBubbleOutlookTopRight()
    {

    }

// ---------------------------------------------------------------------------
// CBubbleOutlookTopRight::ReadBubbleHeader
//
//  
// ---------------------------------------------------------------------------
//
void CBubbleOutlookTopRight::ReadBubbleHeader( CBubbleHeader& aHeader )
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
// CBubbleOutlookTopRight::SizeChanged
// called by framwork when the view size is changed
//  
// ---------------------------------------------------------------------------
//
void CBubbleOutlookTopRight::SizeChanged()
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
// CBubbleOutlookTopRight::DoCall1Layout
// ---------------------------------------------------------------------------
//    
void CBubbleOutlookTopRight::DoCall1Layout()
    {
    // Deprecated
    }

// ---------------------------------------------------------------------------
// CBubbleOutlookTopRight::DoCall2Layout
// ---------------------------------------------------------------------------
//
void CBubbleOutlookTopRight::DoCall2LayoutL()
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
            // Outgoing call layout. Two calls and number entry.
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
            
            // Call icon
            BubbleUtils::LayoutCustomElement(
                iSmallCallIndication, 
                rect, 
                BubbleLayout2::popup_call2_audio_out_call_waiting_icon(2) );
            
            // Ciphering off
            BubbleUtils::LayoutControl( 
                iCyphOffImage, 
                rect,
                BubbleLayout2::popup_call2_audio_out_ciphering_icon(2) );
            
            // Call type pane
            BubbleUtils::LayoutCallTypeIndicators(
                rect,
                BubbleLayout2::popup_call2_audio_second_call_type_icon(0),
                iTypeIndication1,   // Data/Fax
                iTypeIndication2 ); // ALS line 2
                
            // Text
            BubbleUtils::LayoutLabel( 
                iTextLine1, 
                rect, 
                BubbleLayout2::popup_call2_audio_out_call_text_1(2) );
            
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
            // Second call layout. Two calls and number entry.
            ///////////////////////////////////////////////////////////////////
            
            // Create bubble image
            TAknLayoutRect bubbleRect;
            bubbleRect.LayoutRect( 
                rect, 
                BubbleLayout2::popup_call2_audio_second_call_background(0) );
            
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
                BubbleLayout2::popup_call2_audio_second_call_status_icon(11) );
                
            // Ciphering off
            BubbleUtils::LayoutControl( 
                iCyphOffImage, 
                rect,
                BubbleLayout2::popup_call2_audio_second_call_cyphering_icon(11) );
            
            
            // Call type pane
            BubbleUtils::LayoutCallTypeIndicators(
                rect,
                BubbleLayout2::popup_call2_audio_second_call_type_icon(11),
                iTypeIndication1,   // Data/Fax
                iTypeIndication2 ); // ALS line 2
                
            // Text line 1
            BubbleUtils::LayoutLabel( 
                iTextLine1, 
                rect, 
                BubbleLayout2::popup_call2_audio_second_call_cli_text(13) );
                
            // Create bubble
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
// CBubbleOutlookTopRight::GetCall1BubbleBitmaps
// ---------------------------------------------------------------------------
//    
void CBubbleOutlookTopRight::GetCall1BubbleBitmaps()
    {
    CBubbleManager::TPhoneCallState callState = iHeader->CallState();
    
    TInt bubblePicture = KErrNotFound;
    TInt bubblePictureMask = KErrNotFound;
    
    // set call indications and bubble
    switch ( callState )
        {
        case CBubbleManager::EAlertToDisconnected:
            bubblePicture = EQgn_graf_call_rec_small_right_disconn;
            bubblePictureMask = EQgn_graf_call_rec_small_right_disconn_mask;
            break;
        case CBubbleManager::EOutgoing:
        case CBubbleManager::EWaiting:
        case CBubbleManager::EAlerting:
            bubblePicture = EQgn_graf_call_rec_small_right;
            bubblePictureMask = EQgn_graf_call_rec_small_right_mask;
            break;
        case CBubbleManager::EActive:
            bubblePicture = EQgn_graf_call_second_three_active;
            bubblePictureMask = EQgn_graf_call_second_three_active_mask;
            break;
        case CBubbleManager::EOnHold:
            bubblePicture = EQgn_graf_call_second_three_held;
            bubblePictureMask = EQgn_graf_call_second_three_held_mask;
            break;
        case CBubbleManager::EDisconnected:
            bubblePicture = EQgn_graf_call_second_three_disconn;
            bubblePictureMask = EQgn_graf_call_second_three_disconn_mask;
            break;
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
