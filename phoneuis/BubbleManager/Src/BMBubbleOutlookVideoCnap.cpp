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
#include    "BMBubbleOutlookVideoCnap.h"
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
#include    <aknlayout2scalabledef.h>
#include    <layoutmetadata.cdl.h>


// ========================= MEMBER FUNCTIONS ================================

// ---------------------------------------------------------------------------
// CBubbleOutlookVideoCnap
// ---------------------------------------------------------------------------
//
CBubbleOutlookVideoCnap::CBubbleOutlookVideoCnap( 
    CBubbleManager& aBubbleManager ) : 
    CBubbleOutlookThreeLined( aBubbleManager )
    {
    }

// ---------------------------------------------------------------------------
// ConstructL
// ---------------------------------------------------------------------------
//
void CBubbleOutlookVideoCnap::ConstructL()
    { 
    CBubbleOutlookThreeLined::ConstructL();
    }

// ---------------------------------------------------------------------------
// ~CBubbleOutlookVideoCnap
// ---------------------------------------------------------------------------
//
CBubbleOutlookVideoCnap::~CBubbleOutlookVideoCnap()
    {
    Reset();
    }

// ---------------------------------------------------------------------------
// ReadBubbleHeader
// ---------------------------------------------------------------------------
//
void CBubbleOutlookVideoCnap::ReadBubbleHeader( CBubbleHeader& aHeader )
    {
    iHeader = &aHeader;
    
    iBubble = iBubbleManager.ResourceManager().ReserveEikImage( ETrue );        
    
    CBubbleOutlookThreeLined::ReadBubbleHeader( aHeader );
    }

// ---------------------------------------------------------------------------
// SizeChanged
// ---------------------------------------------------------------------------
//
void CBubbleOutlookVideoCnap::SizeChanged()
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
        BubbleLayout3::popup_call3_audio_in_window_text_line_1(2));    
            
    BubbleUtils::LayoutLabel( 
        iTextLine2,
        rect, 
        BubbleLayout3::popup_call3_audio_in_window_text_line_2(2));

    BubbleUtils::LayoutLabel( 
        iTextLine3,
        rect, 
        BubbleLayout3::popup_call3_audio_in_window_text_line_3(0));                

    // Call indicator
    BubbleUtils::LayoutCustomElement(
        iBigCallIndicator, 
        rect, 
        BubbleLayout3::popup_call3_audio_in_window_call_indication(2) );
    
    // Layout number type icon
    BubbleUtils::LayoutCustomElement( 
        iNumberType, 
        rect, 
        BubbleLayout3::popup_call3_audio_in_window_numbertype_indication(2));   

    // Layout cyphering indicator
    BubbleUtils::LayoutControl(
        iCyphOffImage, 
        rect, 
        BubbleLayout3::popup_call3_audio_in_window_cipheringoff_indication(2));      
        
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
        BubbleLayout3::popup_call3_audio_in_window_separator_line( 2 )); 
    iLineRect = lineRect.Rect();        
    }

// ---------------------------------------------------------------------------
// PositionChanged
// ---------------------------------------------------------------------------
//
void CBubbleOutlookVideoCnap::PositionChanged()
    {
    AknsUtils::RegisterControlPosition( this );
    }
    
// ---------------------------------------------------------------------------
// DrawBitmaps
// ---------------------------------------------------------------------------
//
void CBubbleOutlookVideoCnap::DrawBitmaps( CBitmapContext& aGc ) const
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
void CBubbleOutlookVideoCnap::LayoutVideoPanes( const TRect& aParentRect )
    {
     // Qcif video pane         
    TAknLayoutRect qcifVideoPane;
    qcifVideoPane.LayoutRect( 
        aParentRect, 
        BubbleLayout3::call3_video_qcif_pane(2) );

    // Qcif uncrop pane         
    TAknLayoutRect qcifUncropPane;
    qcifUncropPane.LayoutRect( 
        aParentRect, 
        BubbleLayout3::call3_video_qcif_uncrop_pane(2) );
        
    // Subqcif video pane         
    TAknLayoutRect subQcifVideoPane;
    subQcifVideoPane.LayoutRect( 
        aParentRect, 
        BubbleLayout3::call3_video_subqcif_pane(2) );

    // Subqcif uncrop pane         
    TAknLayoutRect subQcifUncropPane;
    subQcifUncropPane.LayoutRect( 
        aParentRect, 
        BubbleLayout3::call3_video_subqcif_uncrop_pane(2) );
        
    // Update video rects to video controller
    iBubbleManager.VideoController().SetQcifVideoPaneRects(
         qcifVideoPane.Rect(),
         qcifUncropPane.Rect() );
         
    iBubbleManager.VideoController().SetSubQcifVideoPaneRects(
         subQcifVideoPane.Rect(),
         subQcifUncropPane.Rect() );    
    }            

// End of File
