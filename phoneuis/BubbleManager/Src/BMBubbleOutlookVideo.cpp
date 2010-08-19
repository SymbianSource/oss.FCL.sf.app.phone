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
* Description:  Bubble outlook video ringtone.
*
*/


// INCLUDE FILES
#include    "BMBubbleManager.h" //for enumerations
#include    "BMBubbleOutlookVideo.h"
#include    "BMBubbleImageManager.h"
#include    "BMResourceManager.h"
#include    "BMBubbleHeader.h"
#include    "BMUtils.h"
#include    "BMLayout3.h"
#include    "BMVideoController.h"
#include    "telbubblecustomelement.h"

#include    <eiklabel.h> 
#include    <eikimage.h>
#include    <eikenv.h>
#include    <AknsUtils.h>
#include    <AknsDrawUtils.h>
#include    <AknLayout2ScalableDef.h>
#include    <layoutmetadata.cdl.h>


// ========================= MEMBER FUNCTIONS ================================


// ---------------------------------------------------------------------------
// CBubbleOutlookVideo
// ---------------------------------------------------------------------------
//
CBubbleOutlookVideo::CBubbleOutlookVideo( CBubbleManager& aBubbleManager )
: CBubbleOutlookTwoLined( aBubbleManager )
    {
    }

// ---------------------------------------------------------------------------
// ConstructL
// ---------------------------------------------------------------------------
//
void CBubbleOutlookVideo::ConstructL()
    { 
    CBubbleOutlookTwoLined::ConstructL();
    }

// ---------------------------------------------------------------------------
// ~CBubbleOutlookVideo
// ---------------------------------------------------------------------------
//
CBubbleOutlookVideo::~CBubbleOutlookVideo()
    {
    Reset();
    }

// ---------------------------------------------------------------------------
// ReadBubbleHeader
// ---------------------------------------------------------------------------
//
void CBubbleOutlookVideo::ReadBubbleHeader( CBubbleHeader& aHeader )
    {
    iHeader = &aHeader;
    
    iBubble = iBubbleManager.ResourceManager().ReserveEikImage( ETrue );        
    
    CBubbleOutlookTwoLined::ReadBubbleHeader( aHeader );
    }

// ---------------------------------------------------------------------------
// SizeChanged
// ---------------------------------------------------------------------------
//
void CBubbleOutlookVideo::SizeChanged()
    {
    if( !iHeader )
        {
        return;
        }
        
    const TRect rect( Rect() );
    
    // Layout labels
    BubbleUtils::LayoutLabel( 
        iTextLine1, 
        rect, 
        BubbleLayout3::popup_call3_audio_in_window_text_line_1(1));    
            
    BubbleUtils::LayoutLabel( 
        iTextLine2,
        rect, 
        BubbleLayout3::popup_call3_audio_in_window_text_line_2(1));

    // Call indicator
    BubbleUtils::LayoutCustomElement(
        iBigCallIndicator, 
        rect, 
        BubbleLayout3::popup_call3_audio_in_window_call_indication(1) );

    // Layout number type icon
    BubbleUtils::LayoutCustomElement( 
        iNumberType, 
        rect, 
        BubbleLayout3::popup_call3_audio_in_window_numbertype_indication(1));   

    // Layout cyphering indicator
    BubbleUtils::LayoutControl(
        iCyphOffImage, 
        rect, 
        BubbleLayout3::popup_call3_audio_in_window_cipheringoff_indication(1));
        
    // Layout and prepare frame
    TAknLayoutRect bubbleRect;
    bubbleRect.LayoutRect( 
        rect, 
        BubbleLayout3::popup_call3_audio_in_pane(1));
    
    if ( Layout_Meta_Data::IsMirrored() )
        {
        TAknLayoutRect bubbleTopRight;        
        bubbleTopRight.LayoutRect( 
            bubbleRect.Rect(), 
            BubbleLayout3::popup_call3_audio_in_window_fr_corner_topright());
            
        TAknLayoutRect bubbleBottomLeft;        
        bubbleBottomLeft.LayoutRect( 
            bubbleRect.Rect(), 
            BubbleLayout3::popup_call3_audio_in_window_fr_corner_bottomleft());        
        
        iOuterRect = TRect( bubbleTopRight.Rect().iTl, 
                                 bubbleBottomLeft.Rect().iBr );
        iInnerRect = TRect( bubbleTopRight.Rect().iBr, 
                                 bubbleBottomLeft.Rect().iTl );        
        }
    else
        {
        TAknLayoutRect bubbleTopLeft;        
        bubbleTopLeft.LayoutRect( 
            bubbleRect.Rect(), 
            BubbleLayout3::popup_call3_audio_in_window_fr_corner_topleft());
            
        TAknLayoutRect bubbleBottomRight;        
        bubbleBottomRight.LayoutRect( 
            bubbleRect.Rect(), 
            BubbleLayout3::popup_call3_audio_in_window_fr_corner_bottomright());        
        
        iOuterRect = TRect( bubbleTopLeft.Rect().iTl, 
                                 bubbleBottomRight.Rect().iBr );
        iInnerRect = TRect( bubbleTopLeft.Rect().iBr, 
                                 bubbleBottomRight.Rect().iTl );        
        }        
                    

    MAknsSkinInstance* skin = AknsUtils::SkinInstance();
    CBubbleManager::TPhoneCallState callState = iHeader->CallState();
    iFrameId = ( callState == CBubbleManager::EAlertToDisconnected ) ?
                                      KAknsIIDQsnFrCall3RectDisconn : 
                                      KAknsIIDQsnFrCall3Rect;
    AknsDrawUtils::PrepareFrame( skin,
                                 iOuterRect,
                                 iInnerRect,
                                 iFrameId,
                                 KAknsIIDDefault );
                                 
    // Pass video layouts to video controller.
    LayoutVideoPanes( rect );
    iBubbleManager.VideoController().SetVideoPaneBackgroundFrame(
        iFrameId, iOuterRect, iInnerRect );                                   
                                 
    // Layout rectangle for separator line
    TAknLayoutRect lineRect;
    lineRect.LayoutRect( rect, 
        BubbleLayout3::popup_call3_audio_in_window_separator_line( 0 )); 
    iLineRect = lineRect.Rect();        
    }

// ---------------------------------------------------------------------------
// PositionChanged
// ---------------------------------------------------------------------------
//
void CBubbleOutlookVideo::PositionChanged()
    {
    AknsUtils::RegisterControlPosition( this );
    }
    
    
// ---------------------------------------------------------------------------
// DrawBitmaps
// ---------------------------------------------------------------------------
//
void CBubbleOutlookVideo::DrawBitmaps( CBitmapContext& aGc ) const
    {
    // Check that proper call header is set
    if ( iHeader == NULL ) 
        { 
        return; 
        }

    if ( !iHeader->IsUsed() ) 
        { 
        return; 
        }

    if ( iHeader->IsInConference() ) 
        { 
        return; 
        }

    MAknsSkinInstance* skin = AknsUtils::SkinInstance();
    if ( iBubble->Bitmap() )
        {
        BubbleUtils::DrawMaskedImage( aGc , iBubble );    
        }
    else
        {
        // Draw bubble frame
        if ( !AknsDrawUtils::DrawFrame( skin,
                                        (CWindowGc&) aGc,  
                                        iOuterRect,
                                        iInnerRect,
                                        iFrameId,
                                        KAknsIIDDefault ) )
            {
            BubbleUtils::DrawBackgroundRect( aGc, Rect() );
            }        
        }
        
    CFbsBitmap* lineBmp = NULL;
    CFbsBitmap* lineBmpMask = NULL;
    AknsUtils::GetCachedMaskedBitmap( skin,
                                      KAknsIIDQgnGrafLineSecondaryHorizontal, 
                                      lineBmp, 
                                      lineBmpMask);
    if ( lineBmp )
        {
        AknIconUtils::SetSize( lineBmp, 
                               iLineRect.Size(), 
                               EAspectRatioNotPreserved);
        if ( lineBmpMask )
            {
            AknIconUtils::SetSize( lineBmpMask, iLineRect.Size(), 
                                   EAspectRatioNotPreserved);
            
            aGc.BitBltMasked( iLineRect.iTl, lineBmp, TRect( TPoint(0,0), 
                             lineBmp->SizeInPixels() ),
                             lineBmpMask, EFalse);
            }
        else
            {
            aGc.BitBlt( iLineRect.iTl, lineBmp );
            }
        }                 
    }    

// ---------------------------------------------------------------------------
// LayoutVideoPanes
// ---------------------------------------------------------------------------
//
void CBubbleOutlookVideo::LayoutVideoPanes( const TRect& aParentRect )
    {
     // Qcif video pane         
    TAknLayoutRect qcifVideoPane;
    qcifVideoPane.LayoutRect( 
        aParentRect, 
        BubbleLayout3::call3_video_qcif_pane(0) );

    // Qcif uncrop pane         
    TAknLayoutRect qcifUncropPane;
    qcifUncropPane.LayoutRect( 
        aParentRect, 
        BubbleLayout3::call3_video_qcif_uncrop_pane(0) );
        
    // Subqcif video pane         
    TAknLayoutRect subQcifVideoPane;
    subQcifVideoPane.LayoutRect( 
        aParentRect, 
        BubbleLayout3::call3_video_subqcif_pane(0) );

    // Subqcif uncrop pane         
    TAknLayoutRect subQcifUncropPane;
    subQcifUncropPane.LayoutRect( 
        aParentRect, 
        BubbleLayout3::call3_video_subqcif_uncrop_pane(0) );
        
    // Update video rects to video controller
    iBubbleManager.VideoController().SetQcifVideoPaneRects(
         qcifVideoPane.Rect(),
         qcifUncropPane.Rect() );
         
    iBubbleManager.VideoController().SetSubQcifVideoPaneRects(
         subQcifVideoPane.Rect(),
         subQcifUncropPane.Rect() );
    }

// End of File
