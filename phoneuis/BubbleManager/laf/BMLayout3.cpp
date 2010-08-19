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
* Description:  Call3 LAF
*
*/


// INCLUDE FILES
#include "BMLayout3.h"
#include "BMPanic.h"
#include <aknenv.h>
#include <applayout.cdl.h>
#include <AknUtils.h>
#include <AknLayout2ScalableDef.h>
#include <aknlayoutscalable_apps.cdl.h>
#include <layoutmetadata.cdl.h>


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// BubbleLayout3::popup_call3_audio_in_window_rect
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TRect BubbleLayout3::popup_call3_audio_in_window_rect( TInt aVariety )
    {
    TRect screenRect( CEikonEnv::Static()->ScreenDevice()->SizeInPixels() );

    TAknLayoutRect appRect;
    appRect.LayoutRect( 
        screenRect, 
        AknLayout::application_window( screenRect ) );
        
    TAknLayoutRect mainRect;
    
    if ( Layout_Meta_Data::IsLandscapeOrientation() )
        {
        mainRect.LayoutRect( 
            appRect.Rect(), 
            AknLayout::main_pane( appRect.Rect(), 0, 1, 1 ) ); // stacon included            
        }
    else
        {
        mainRect.LayoutRect( 
            appRect.Rect(), 
            AknLayout::main_pane( appRect.Rect(), 0, 1, 1 ) ); // statuspane excluded        
        }
        
    TAknLayoutRect mainCall3Pane;
    mainCall3Pane.LayoutRect( 
        mainRect.Rect(), 
        AknLayoutScalable_Apps::main_call3_pane().LayoutLine() );
        
    TAknLayoutRect call3WindowRect;
    if ( Layout_Meta_Data::IsLandscapeOrientation() )
        {
        aVariety += 2;            
        }    
    call3WindowRect.LayoutRect( 
        mainCall3Pane.Rect(), 
        AknLayoutScalable_Apps::popup_call3_audio_in_window( aVariety ).LayoutLine() );
        
    return call3WindowRect.Rect();                    
    }

// -----------------------------------------------------------------------------
// BubbleLayout3::main_call3_pane
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout BubbleLayout3::main_call3_pane()
    {
    return AknLayoutScalable_Apps::main_call3_pane().LayoutLine();    
    }

// -----------------------------------------------------------------------------
// BubbleLayout3::popup_call3_audio_in_window
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout BubbleLayout3::popup_call3_audio_in_window(
    TInt aVariety )
    {
    if ( Layout_Meta_Data::IsLandscapeOrientation() )
        {
        aVariety += 2;            
        }
        
    TAknWindowComponentLayout layout1 = 
        AknLayoutScalable_Apps::main_call3_pane();
    TAknWindowComponentLayout layout2 = 
        AknLayoutScalable_Apps::popup_call3_audio_in_window( aVariety );
    
    return TAknWindowComponentLayout::
        Compose( layout1, layout2 ).LayoutLine();
    }

// -----------------------------------------------------------------------------
// BubbleLayout3::popup_call3_audio_in_pane
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout BubbleLayout3::popup_call3_audio_in_pane(
    TInt aVariety )
    {    
    if ( Layout_Meta_Data::IsLandscapeOrientation() )
        {
        aVariety += 2;            
        }
    return AknLayoutScalable_Apps::bg_popup_call3_in_pane( aVariety).LayoutLine();
    }

// -----------------------------------------------------------------------------
// BubbleLayout3::popup_call3_audio_in_window_fr_center
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout BubbleLayout3::popup_call3_audio_in_window_fr_center()
    {
    return AknLayoutScalable_Apps::bg_popup_call3_rect_pane_g1().LayoutLine();
    }
    
// -----------------------------------------------------------------------------
// BubbleLayout3::popup_call3_audio_in_window_fr_corner_topleft
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout BubbleLayout3::popup_call3_audio_in_window_fr_corner_topleft()
    {
    return AknLayoutScalable_Apps::bg_popup_call3_rect_pane_g2().LayoutLine();
    }
    
// -----------------------------------------------------------------------------
// BubbleLayout3::popup_call3_audio_in_window_fr_corner_topright
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout BubbleLayout3::popup_call3_audio_in_window_fr_corner_topright()
    {
    return AknLayoutScalable_Apps::bg_popup_call3_rect_pane_g3().LayoutLine();
    }
    
// -----------------------------------------------------------------------------
// BubbleLayout3::popup_call3_audio_in_window_fr_corner_bottomleft
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout BubbleLayout3::popup_call3_audio_in_window_fr_corner_bottomleft()
    {
    return AknLayoutScalable_Apps::bg_popup_call3_rect_pane_g4().LayoutLine();
    }        
    
// -----------------------------------------------------------------------------
// BubbleLayout3::popup_call3_audio_in_window_fr_corner_bottomright
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout BubbleLayout3::popup_call3_audio_in_window_fr_corner_bottomright()
    {
    return AknLayoutScalable_Apps::bg_popup_call3_rect_pane_g5().LayoutLine();
    }
    
// -----------------------------------------------------------------------------
// BubbleLayout3::popup_call3_audio_in_window_call_indication
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout BubbleLayout3::popup_call3_audio_in_window_call_indication(
    TInt aVariety )
    {
    if ( Layout_Meta_Data::IsLandscapeOrientation() )
        {
        aVariety += 2;            
        }
    return AknLayoutScalable_Apps::popup_call3_audio_in_window_g1( aVariety ).LayoutLine();
    }
    
// -----------------------------------------------------------------------------
// BubbleLayout3::popup_call3_audio_in_window_numbertype_indication
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout BubbleLayout3::popup_call3_audio_in_window_numbertype_indication(
    TInt aVariety )
    {
    if ( Layout_Meta_Data::IsLandscapeOrientation() )
        {
        aVariety += 2;            
        }
    return AknLayoutScalable_Apps::popup_call3_audio_in_window_g2( aVariety ).LayoutLine();
    }
    
// -----------------------------------------------------------------------------
// BubbleLayout3::popup_call3_audio_in_window_cipheringoff_indication
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout BubbleLayout3::popup_call3_audio_in_window_cipheringoff_indication(
    TInt aVariety )
    {
    if ( Layout_Meta_Data::IsLandscapeOrientation() )
        {
        aVariety += 2;            
        }
    return AknLayoutScalable_Apps::popup_call3_audio_in_window_g3( aVariety ).LayoutLine();
    }
    
// -----------------------------------------------------------------------------
// BubbleLayout3::popup_call3_audio_in_window_separator_line
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout BubbleLayout3::popup_call3_audio_in_window_separator_line(
    TInt aVariety )
    {
    if ( Layout_Meta_Data::IsLandscapeOrientation() )
        {
        aVariety += 1;            
        }
    return AknLayoutScalable_Apps::popup_call3_audio_in_window_g4( aVariety ).LayoutLine();
    }                            

// -----------------------------------------------------------------------------
// BubbleLayout3::popup_call3_audio_in_window_text_line_1
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
const TAknTextLineLayout BubbleLayout3::popup_call3_audio_in_window_text_line_1(
    TInt aVariety )
    {
    if ( Layout_Meta_Data::IsLandscapeOrientation() )
        {
        aVariety += 2;            
        }
    return AknLayoutScalable_Apps::popup_call3_audio_in_window_t1( aVariety ).LayoutLine();
    }
    
// -----------------------------------------------------------------------------
// BubbleLayout3::popup_call3_audio_in_window_text_line_2
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
const TAknTextLineLayout BubbleLayout3::popup_call3_audio_in_window_text_line_2(
    TInt aVariety )
    {
    if ( Layout_Meta_Data::IsLandscapeOrientation() )
        {
        aVariety += 2;            
        }
    return AknLayoutScalable_Apps::popup_call3_audio_in_window_t2( aVariety ).LayoutLine();
    }    

// -----------------------------------------------------------------------------
// BubbleLayout3::popup_call3_audio_in_window_text_line_3
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
const TAknTextLineLayout BubbleLayout3::popup_call3_audio_in_window_text_line_3(
    TInt aVariety )
    {
    if ( Layout_Meta_Data::IsLandscapeOrientation() )
        {
        aVariety += 1;            
        }
    return AknLayoutScalable_Apps::popup_call3_audio_in_window_t3( aVariety ).LayoutLine();
    }

// -----------------------------------------------------------------------------
// BubbleLayout3::call3_video_qcif_pane
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout BubbleLayout3::call3_video_qcif_pane( 
    TInt aVariety )
    {
    if ( Layout_Meta_Data::IsLandscapeOrientation() )
        {
        aVariety += 1;            
        }
        
    return AknLayoutScalable_Apps::call3_video_qcif_pane( aVariety ).LayoutLine();
    }
    
// -----------------------------------------------------------------------------
// BubbleLayout3::call3_video_subqcif_pane
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout BubbleLayout3::call3_video_subqcif_pane( 
    TInt aVariety )
    {
    if ( Layout_Meta_Data::IsLandscapeOrientation() )
        {
        aVariety += 1;            
        }
    return AknLayoutScalable_Apps::call3_video_subqcif_pane( aVariety ).LayoutLine();
    }    

// -----------------------------------------------------------------------------
// BubbleLayout3::call3_video_qcif_uncrop_pane
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout BubbleLayout3::call3_video_qcif_uncrop_pane( 
    TInt aVariety )
    {
    if ( Layout_Meta_Data::IsLandscapeOrientation() )
        {
        aVariety += 1;            
        }
        
    return AknLayoutScalable_Apps::call3_video_qcif_uncrop_pane( aVariety ).LayoutLine();
    }
    
// -----------------------------------------------------------------------------
// BubbleLayout3::main_call3_video_subqcif_uncrop_pane
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout BubbleLayout3::call3_video_subqcif_uncrop_pane( 
    TInt aVariety )
    {
    if ( Layout_Meta_Data::IsLandscapeOrientation() )
        {
        aVariety += 1;            
        }
    return AknLayoutScalable_Apps::call3_video_subqcif_uncrop_pane( aVariety ).LayoutLine();
    }    
    

// End of File

