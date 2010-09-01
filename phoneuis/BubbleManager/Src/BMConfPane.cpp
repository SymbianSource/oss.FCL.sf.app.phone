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
* Description:  Conf Pane
*
*/


// INCLUDE FILES
#include    "BMConfPane.h"  //definition
#include    "BMLayout.h" //coordinates
#include    "BMLayout2.h" //coordinates
#include    "BMLayout4.h" //coordinates
#include    "BMUtils.h"     //utility functions

#include    <eiklabel.h>    //CEikLabel
#include    <eikimage.h>    //CEikImage
#include    <eikenv.h>      //environment
#include    <AknsDrawUtils.h>
#include    <AknsConstants.h>
#include    <skinlayout.cdl.h>

// ========================= MEMBER FUNCTIONS ================================

// ---------------------------------------------------------------------------
// CBubbleConfPane::ConstructL( )
// Symbian OS two phased constructor
// 
// ---------------------------------------------------------------------------
//
void CBubbleConfPane::ConstructL( TBool aTouchCallHandling )
    {
    iTouchCallHandling = aTouchCallHandling;
    iFullText.Set( NULL , 0 );
    
    iIsHighlighted = EFalse;
    ActivateL();
    }


// Destructor
CBubbleConfPane::~CBubbleConfPane()
    {
    Reset();
    }


// ---------------------------------------------------------------------------
// CBubbleConfPane::SizeChanged
// called by framework when the view size is changed
//  
// ---------------------------------------------------------------------------
//
void CBubbleConfPane::SizeChanged()
    { 
    AknsUtils::RegisterControlPosition( this );

    if ( iTouchCallHandling )
        {
        DoCall4Layout();                
        }
    else if ( !iCallObjectDisplay )
        {
        DoCall1Layout();            
        }
    else
        {
        DoCall2Layout();                
        }        
    }

// ---------------------------------------------------------------------------
// CBubbleConfPane::PositionChanged
// ---------------------------------------------------------------------------
//
void CBubbleConfPane::PositionChanged()
    {
    AknsUtils::RegisterControlPosition( this );
    }

// ---------------------------------------------------------------------------
// CBubbleConfPane::CountComponentControls
//
//  
// ---------------------------------------------------------------------------
//
TInt CBubbleConfPane::CountComponentControls() const
    {
    TInt amount = 0;
    if ( iCallIndication ) 
        {
        amount++;
        }
    if ( iCyphOffImage ) 
        {
        amount++;
        }
    if ( iTextLine )
        {
        amount++;
        }
    return amount;
    }

// ---------------------------------------------------------------------------
// CBubbleConfPane::ComponentControl
//
//  
// ---------------------------------------------------------------------------
//
CCoeControl* CBubbleConfPane::ComponentControl( TInt aIndex ) const
    {
    TInt amount = -1;
    if ( iCallIndication )
        {
        amount++;
        }
    if ( aIndex == amount ) 
        {
        return iCallIndication;
        }
    if ( iCyphOffImage )
        {
        amount++;
        }
    if ( aIndex == amount ) 
        {
        return iCyphOffImage;
        }
    if ( iTextLine ) 
        {
        amount++;
        }
    if ( aIndex == amount ) 
        {
        return iTextLine;
        }
    return NULL;
    }

// ---------------------------------------------------------------------------
// CBubbleConfPane::Draw
// 
//  
// ---------------------------------------------------------------------------
//
void CBubbleConfPane::Draw( const TRect& /*aRect*/ ) const
    {
    
    CWindowGc& gc = SystemGc( );
    gc.SetBrushColor( AKN_LAF_COLOR( BubbleLayout::LayoutColourWhite() ) );
    
    // Set texts to fit.
    BubbleUtils::ClipToLabel( iFullText , iTextLine , iTextClipDirection );

    // draw hightlight rectangles if needed
    if ( iIsHighlighted ) 
        {      
        TBool highlightDrawn = EFalse;
        TAknLayoutRect topLeft;
        topLeft.LayoutRect(
            iHighlight.Rect(), 
            SkinLayout::List_highlight_skin_placing__general__Line_2() );

        TAknLayoutRect bottomRight;
        bottomRight.LayoutRect(
            iHighlight.Rect(), 
            SkinLayout::List_highlight_skin_placing__general__Line_5() );

        TRect outerRect = TRect( topLeft.Rect().iTl, bottomRight.Rect().iBr );
        TRect innerRect = TRect( topLeft.Rect().iBr, bottomRight.Rect().iTl );
        
        MAknsSkinInstance *skin = AknsUtils::SkinInstance();
        gc.SetPenStyle( CGraphicsContext::ENullPen );
        highlightDrawn = AknsDrawUtils::DrawFrame( skin, gc, 
                                                   outerRect,
                                                   innerRect,
                                                   KAknsIIDQsnFrList, 
                                                   KAknsIIDQsnFrListCenter );

        if ( !highlightDrawn )
            {
            iShadow.DrawRect( gc );
            iHighlight.DrawRect( gc );
            }
        }
    }


// ---------------------------------------------------------------------------
// CBubbleConfPane::Reset
//  
// Put member variables to initial state.
// ---------------------------------------------------------------------------
//
void CBubbleConfPane::Reset( )
    {
    BubbleUtils::AddTextToEikLabel( iTextLine , KNullDesC );
    iFullText.Set( NULL , 0 );
    iIsHighlighted = EFalse;
    }

// ---------------------------------------------------------------------------
// CBubbleConfPane::SetHighlight
//  
//  
// ---------------------------------------------------------------------------
//
void CBubbleConfPane::SetHighlight( const TBool& aIsHighlighted )
    {
    iIsHighlighted = aIsHighlighted;
    }

// ---------------------------------------------------------------------------
// CBubbleConfPane::GetCallIndicationHandle
//  
//  Returns pointer to image so that bubble outlook can attach bitmap to it
// ---------------------------------------------------------------------------
//
CEikImage*& CBubbleConfPane::CallIndicationHandle( )
    {
    return iCallIndication;
    } 

// ---------------------------------------------------------------------------
// CBubbleConfPane::SetText
//  
//  
// ---------------------------------------------------------------------------
//
void CBubbleConfPane::SetText( TPtrC aText , 
                        const CBubbleManager::TPhoneClippingDirection& aDir )
    {
    iFullText.Set( aText );
    iTextClipDirection = aDir;
    }

// ---------------------------------------------------------------------------
// CBubbleConfPane::DoCall1Layout
// ---------------------------------------------------------------------------
//    
void CBubbleConfPane::DoCall1Layout()
    {
    // Deprecated
    }

// ---------------------------------------------------------------------------
// CBubbleConfPane::DoCall2Layout
// ---------------------------------------------------------------------------
//
void CBubbleConfPane::DoCall2Layout()
    {
    BubbleUtils::LayoutImage( 
        iCallIndication, 
        Rect() , 
        BubbleLayout2::popup_call2_conf_single_list_pane_call_status_icon() );

    BubbleUtils::LayoutImage( 
        iCyphOffImage, 
        Rect() , 
        BubbleLayout2::popup_call2_conf_single_list_pane_ciphering_icon() );

    BubbleUtils::LayoutLabel( 
        iTextLine, 
        Rect(), 
        BubbleLayout2::popup_call2_conf_single_list_pane_cli_text());

    iShadow.LayoutRect( 
        Rect() , 
        BubbleLayout2::popup_call2_conf_single_list_highlight_pane() );

    iHighlight.LayoutRect( 
        Rect() ,
        BubbleLayout2::popup_call2_conf_single_list_highlight_pane() ); 
    }

// ---------------------------------------------------------------------------
// CBubbleConfPane::DoCall4Layout
// ---------------------------------------------------------------------------
//
void CBubbleConfPane::DoCall4Layout()
    {
    BubbleUtils::LayoutImage( 
        iCallIndication, 
        Rect() , 
        BubbleLayout4::popup_call4_conf_single_list_pane_call_status_icon() );

    BubbleUtils::LayoutImage( 
        iCyphOffImage, 
        Rect() , 
        BubbleLayout4::popup_call4_conf_single_list_pane_ciphering_icon() );

    BubbleUtils::LayoutLabel( 
        iTextLine, 
        Rect(), 
        BubbleLayout4::popup_call4_conf_single_list_pane_cli_text());

    iShadow.LayoutRect( 
        Rect() , 
        BubbleLayout4::popup_call4_conf_single_list_highlight_pane() );

    iHighlight.LayoutRect( 
        Rect() ,
        BubbleLayout4::popup_call4_conf_single_list_highlight_pane() ); 
    }
    
// End of File
