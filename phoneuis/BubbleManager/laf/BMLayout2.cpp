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
* Description:  LAF coordinates (Call2)
*
*/


// INCLUDE FILES
#include "BMLayout2.h"
#include "BMPanic.h"
#include "BMBubbleManager.h"
#include <aknenv.h>
#include <applayout.cdl.h>
#include <AknUtils.h>
#include <aknlayout2scalabledef.h>
#include <aknlayoutscalable_apps.cdl.h>
#include <layoutmetadata.cdl.h>


// ============================ MEMBER FUNCTIONS ===============================

    
////////////////////////////////////////////////////////////////////////////////
//
// Call2 Layouts
//
////////////////////////////////////////////////////////////////////////////////    


// -----------------------------------------------------------------------------
// BubbleLayout2::RectFrameInnerOuterRects
// -----------------------------------------------------------------------------
//
void BubbleLayout2::RectFrameInnerOuterRects( const TRect& aFrameRect,
                                              TRect& aOuterRect,
                                              TRect& aInnerRect )
    {
    if ( Layout_Meta_Data::IsMirrored() )
        {
        TAknLayoutRect frameTopRight;        
        frameTopRight.LayoutRect( 
            aFrameRect, 
            BubbleLayout2::call2_rect_pane_fr_cor_topright() );                
                
        TAknLayoutRect frameBottomLeft;        
        frameBottomLeft.LayoutRect( 
            aFrameRect, 
            BubbleLayout2::call2_rect_pane_fr_cor_bottomleft() );                        

        aOuterRect = TRect( frameTopRight.Rect().iTl, 
                            frameBottomLeft.Rect().iBr );
                
        aInnerRect = TRect( frameTopRight.Rect().iBr, 
                            frameBottomLeft.Rect().iTl );    
        }
    else
        {
        TAknLayoutRect frameTopLeft;        
        frameTopLeft.LayoutRect( 
            aFrameRect, 
            BubbleLayout2::call2_rect_pane_fr_cor_topleft() );

        TAknLayoutRect frameBottomRight;        
        frameBottomRight.LayoutRect( 
            aFrameRect, 
            BubbleLayout2::call2_rect_pane_fr_cor_bottomright() );
        
        aOuterRect = TRect( frameTopLeft.Rect().iTl, 
                            frameBottomRight.Rect().iBr );
        aInnerRect = TRect( frameTopLeft.Rect().iBr, 
                            frameBottomRight.Rect().iTl );    
        }                    
    }

// -----------------------------------------------------------------------------
// BubbleLayout2::BubbleFrameInnerOuterRects
// -----------------------------------------------------------------------------
//
void BubbleLayout2::BubbleFrameInnerOuterRects( const TRect& aFrameRect,
                                                TRect& aOuterRect,
                                                TRect& aInnerRect )
    {
    if ( Layout_Meta_Data::IsMirrored() )
        {
        TAknLayoutRect bubbleTopRight;        
        bubbleTopRight.LayoutRect( 
            aFrameRect, 
            BubbleLayout2::call2_bubble_pane_fr_cor_topright() );                
                
        TAknLayoutRect bubbleBottomLeft;        
        bubbleBottomLeft.LayoutRect( 
            aFrameRect, 
            BubbleLayout2::call2_bubble_pane_fr_cor_bottomleft() );                        

        aOuterRect = TRect( bubbleTopRight.Rect().iTl, 
                            bubbleBottomLeft.Rect().iBr );
                
        aInnerRect = TRect( bubbleTopRight.Rect().iBr, 
                            bubbleBottomLeft.Rect().iTl );    
        }
    else
        {
        TAknLayoutRect bubbleTopLeft;        
        bubbleTopLeft.LayoutRect( 
            aFrameRect, 
            BubbleLayout2::call2_bubble_pane_fr_cor_topleft() );

        TAknLayoutRect bubbleBottomRight;        
        bubbleBottomRight.LayoutRect( 
            aFrameRect, 
            BubbleLayout2::call2_bubble_pane_fr_cor_bottomright() );
        
        aOuterRect = TRect( bubbleTopLeft.Rect().iTl, 
                            bubbleBottomRight.Rect().iBr );
        aInnerRect = TRect( bubbleTopLeft.Rect().iBr, 
                            bubbleBottomRight.Rect().iTl );    
        }    
    }

////////////////////////////////////////////////////////////////////////////////    
// LAF Table 4.50: bubble/rect window positions
////////////////////////////////////////////////////////////////////////////////    

// -----------------------------------------------------------------------------
// BubbleLayout2::popup_call2_audio_first_window
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout2::popup_call2_audio_first_window( TInt aVariety )
    {
    TAknLayoutScalableParameterLimits parLimits =
        AknLayoutScalable_Apps::popup_call2_audio_first_window_ParamLimits();

    if ( Layout_Meta_Data::IsLandscapeOrientation() )
        {
        if ( aVariety < 17 )
            {
            // portrait 0..6 == landscape 7..13 
            aVariety += 7;    	        
            }
        else
            {
            // portrait 18 == landscape 17 
            aVariety -= 1;    
            }            
    	} 
    
    if ( aVariety < parLimits.FirstVariety() || 
         aVariety > parLimits.LastVariety() )
        {
        Panic( EBMPanicLAF );
        }
        
    TAknWindowComponentLayout layout1 = 
        AknLayoutScalable_Apps::main_call2_pane();
    TAknWindowComponentLayout layout2 = 
        AknLayoutScalable_Apps::popup_call2_audio_first_window( aVariety );
    
    return TAknWindowComponentLayout::
        Compose( layout1, layout2 ).LayoutLine();    
    }
// -----------------------------------------------------------------------------
// BubbleLayout2::popup_call2_audio_in_window
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout2::popup_call2_audio_in_window( TInt aVariety )
    {
    TAknLayoutScalableParameterLimits parLimits =
        AknLayoutScalable_Apps::popup_call2_audio_in_window_ParamLimits();

    if ( Layout_Meta_Data::IsLandscapeOrientation() )
        {
        if ( aVariety == 8 )
            {
            aVariety = 9;    
            }
        else
            {
            // portrait 0..3 == landscape 4..7 
            aVariety += 4;    	                
            }
    	}        
    
    if ( aVariety < parLimits.FirstVariety() || 
         aVariety > parLimits.LastVariety() )
        {
        Panic( EBMPanicLAF );
        }
    
    TAknWindowComponentLayout layout1 = 
        AknLayoutScalable_Apps::main_call2_pane();
    TAknWindowComponentLayout layout2 = 
        AknLayoutScalable_Apps::popup_call2_audio_in_window( aVariety );
    
    return TAknWindowComponentLayout::
        Compose( layout1, layout2 ).LayoutLine();    
    }
// -----------------------------------------------------------------------------
// BubbleLayout2::popup_call2_audio_out_window
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout2::popup_call2_audio_out_window( TInt aVariety )
    {
    TAknLayoutScalableParameterLimits parLimits =
        AknLayoutScalable_Apps::popup_call2_audio_out_window_ParamLimits();


    if ( Layout_Meta_Data::IsLandscapeOrientation() )
        {
        // portrait 0..2 == landscape 3..6 
        aVariety += 3;    	        
    	} 
    	    
    if ( aVariety < parLimits.FirstVariety() || 
         aVariety > parLimits.LastVariety() )
        {
        Panic( EBMPanicLAF );
        }
    
    TAknWindowComponentLayout layout1 = 
        AknLayoutScalable_Apps::main_call2_pane();
    TAknWindowComponentLayout layout2 = 
        AknLayoutScalable_Apps::popup_call2_audio_out_window( aVariety );
    
    return TAknWindowComponentLayout::
        Compose( layout1, layout2 ).LayoutLine();    
    }
    
// -----------------------------------------------------------------------------
// BubbleLayout2::popup_call2_audio_second_window
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout2::popup_call2_audio_second_window( TInt aVariety )
    {
    TAknLayoutScalableParameterLimits parLimits =
        AknLayoutScalable_Apps::popup_call2_audio_second_window_ParamLimits();

    if ( Layout_Meta_Data::IsLandscapeOrientation() )
        {
        // portrait 0..4 == landscape 5..9 
        aVariety += 5;    	        
        }

    if ( aVariety < parLimits.FirstVariety() || 
         aVariety > parLimits.LastVariety() )
        {
        Panic( EBMPanicLAF );
        }
    
    TAknWindowComponentLayout layout1 = 
        AknLayoutScalable_Apps::main_call2_pane();
    TAknWindowComponentLayout layout2 = 
        AknLayoutScalable_Apps::popup_call2_audio_second_window( aVariety );
    
    return TAknWindowComponentLayout::
        Compose( layout1, layout2 ).LayoutLine();    
    }
    
// -----------------------------------------------------------------------------
// BubbleLayout2::popup_call2_audio_wait_window
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout2::popup_call2_audio_wait_window( TInt aVariety )
    {
    TAknLayoutScalableParameterLimits parLimits =
        AknLayoutScalable_Apps::popup_call2_audio_wait_window_ParamLimits();

    if ( Layout_Meta_Data::IsLandscapeOrientation() )
        {
        if ( aVariety < 8 )
            {
            // portrait 0..3 == landscape 4..7 
            aVariety += 4;    	        
            }
        else
            {
            // portrait 9 == landscape 8 
            aVariety -= 1;    
            }            
    	}

    if ( aVariety < parLimits.FirstVariety() || 
         aVariety > parLimits.LastVariety() )
        {
        Panic( EBMPanicLAF );
        }
    
    TAknWindowComponentLayout layout1 = 
        AknLayoutScalable_Apps::main_call2_pane();
    TAknWindowComponentLayout layout2 = 
        AknLayoutScalable_Apps::popup_call2_audio_wait_window( aVariety );
    
    return TAknWindowComponentLayout::
        Compose( layout1, layout2 ).LayoutLine();    
    }                    
    
// -----------------------------------------------------------------------------
// BubbleLayout2::popup_call2_audio_conf_window
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout2::popup_call2_audio_conf_window( TInt aVariety )
    {
    TAknLayoutScalableParameterLimits parLimits =
        AknLayoutScalable_Apps::popup_call2_audio_conf_window_ParamLimits();
        
    if ( Layout_Meta_Data::IsLandscapeOrientation() )
        {
        // portrait 0..3 == landscape 4..7 
        aVariety += 4;    	        
        }

    if ( aVariety < parLimits.FirstVariety() || 
         aVariety > parLimits.LastVariety() )
        {
        Panic( EBMPanicLAF );
        }
    
    TAknWindowComponentLayout layout1 = 
        AknLayoutScalable_Apps::main_call2_pane();
    TAknWindowComponentLayout layout2 = 
        AknLayoutScalable_Apps::popup_call2_audio_conf_window( aVariety );
    
    return TAknWindowComponentLayout::
        Compose( layout1, layout2 ).LayoutLine();    
    }
    
// -----------------------------------------------------------------------------
// BubbleLayout2::call2_call_object_visual_pane
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout2::call2_call_object_visual_pane()
    {
    return AknLayoutScalable_Apps::call2_cli_visual_pane().LayoutLine();
    }    
    
////////////////////////////////////////////////////////////////////////////////            
// LAF Table 4.60: conference call bubble contents
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// BubbleLayout2::popup_call2_conf_pane_bg
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout2::popup_call2_conf_pane_background()
    {
        return AknLayoutScalable_Apps::bg_popup_call2_act_pane_cp03().LayoutLine();
    }
    
// -----------------------------------------------------------------------------
// BubbleLayout2::popup_call2_conf_cli_list
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout2::popup_call2_conf_cli_list()
    {
    return AknLayoutScalable_Apps::list_conf_pane_cp().LayoutLine();
    }
    
// -----------------------------------------------------------------------------
// BubbleLayout2::popup_call2_conf_window_call_timer_txt
// -----------------------------------------------------------------------------
//
const TAknTextLineLayout 
    BubbleLayout2::popup_call2_conf_window_call_timer_text()
    {
    return AknLayoutScalable_Apps::popup_call2_audio_conf_window_t1().LayoutLine();
    }            
    
    
////////////////////////////////////////////////////////////////////////////////
// LAF Table 4.61: conference call bubble bg graphics
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// BubbleLayout2::popup_call2_conf_single_list_graphic_pane
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout2::popup_call2_conf_single_list_graphic_pane( TInt aRow )
    {
    TAknLayoutScalableParameterLimits parLimits =
        AknLayoutScalable_Apps::list_single_graphic_popup_conf2_pane_ParamLimits();

    /*if ( aRow < parLimits.FirstVariety() || aRow > parLimits.LastVariety() )
        {
        Panic( EBMPanicLAF );
        }*/
    
    return AknLayoutScalable_Apps::
        list_single_graphic_popup_conf2_pane( aRow ).LayoutLine();    
    }
    

////////////////////////////////////////////////////////////////////////////////
// LAF Table 4.62: conference call members list entries
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// BubbleLayout2::popup_call2_conf_single_list_highlight_pane
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout2::popup_call2_conf_single_list_highlight_pane()
    {
    return AknLayoutScalable_Apps::list_highlight_pane_cp04().LayoutLine();
    }
    
// -----------------------------------------------------------------------------
// BubbleLayout2::popup_call2_conf_single_list_pane_call_status_icon
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout2::popup_call2_conf_single_list_pane_call_status_icon()
    {
    return AknLayoutScalable_Apps::list_single_graphic_popup_conf2_pane_g1().LayoutLine();
    }
    
// -----------------------------------------------------------------------------
// BubbleLayout2::popup_call2_conf_single_list_pane_ciphering_icon
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout2::popup_call2_conf_single_list_pane_ciphering_icon()
    {
    return AknLayoutScalable_Apps::list_single_graphic_popup_conf2_pane_g2().LayoutLine();
    }
    
// -----------------------------------------------------------------------------
// BubbleLayout2::popup_call2_conf_single_list_pane_cli_text
// -----------------------------------------------------------------------------
//
const TAknTextLineLayout 
    BubbleLayout2::popup_call2_conf_single_list_pane_cli_text()
    {
    return AknLayoutScalable_Apps::list_single_graphic_popup_conf2_pane_t1().LayoutLine();
    }


////////////////////////////////////////////////////////////////////////////////
// LAF Table 4.59: second call bubble contents           
////////////////////////////////////////////////////////////////////////////////
  
// -----------------------------------------------------------------------------
// BubbleLayout2::popup_call2_audio_second_call_background
// -----------------------------------------------------------------------------
//    
const TAknWindowLineLayout 
    BubbleLayout2::popup_call2_audio_second_call_background( TInt aVariety )
    {
    TAknLayoutScalableParameterLimits parLimits =
        AknLayoutScalable_Apps::bg_popup_call2_act_pane_cp01_ParamLimits();

    if ( aVariety < parLimits.FirstVariety() || aVariety > parLimits.LastVariety() )
        {
        Panic( EBMPanicLAF );
        }
    
    return AknLayoutScalable_Apps::bg_popup_call2_act_pane_cp01( 
        aVariety ).LayoutLine(); 
    }
    
// -----------------------------------------------------------------------------
// BubbleLayout2::popup_call2_audio_second_call_type_icon
// -----------------------------------------------------------------------------
//    
const TAknWindowLineLayout 
    BubbleLayout2::popup_call2_audio_second_call_type_icon( TInt aVariety )
    {
    TAknLayoutScalableParameterLimits parLimits =
        AknLayoutScalable_Apps::call_type_pane_cp05_ParamLimits();

    if ( aVariety < parLimits.FirstVariety() || aVariety > parLimits.LastVariety() )
        {
        Panic( EBMPanicLAF );
        }
    
    return AknLayoutScalable_Apps::call_type_pane_cp05( aVariety ).LayoutLine();    
    }
    
// -----------------------------------------------------------------------------
// BubbleLayout2::popup_call2_audio_second_call_status_icon
// -----------------------------------------------------------------------------
//    
const TAknWindowLineLayout 
    BubbleLayout2::popup_call2_audio_second_call_status_icon( TInt aVariety )
    {
    TAknLayoutScalableParameterLimits parLimits =
        AknLayoutScalable_Apps::popup_call2_audio_second_window_g1_ParamLimits();

    if ( aVariety < parLimits.FirstVariety() || aVariety > parLimits.LastVariety() )
        {
        Panic( EBMPanicLAF );
        }
    
    return AknLayoutScalable_Apps::popup_call2_audio_second_window_g1( 
        aVariety ).LayoutLine(); 
    }
    
// -----------------------------------------------------------------------------
// BubbleLayout2::popup_call2_audio_second_call_cyphering_icon
// -----------------------------------------------------------------------------
//    
const TAknWindowLineLayout 
    BubbleLayout2::popup_call2_audio_second_call_cyphering_icon( TInt aVariety )
    {
    TAknLayoutScalableParameterLimits parLimits =
        AknLayoutScalable_Apps::popup_call2_audio_second_window_g2_ParamLimits();

    if ( aVariety < parLimits.FirstVariety() || aVariety > parLimits.LastVariety() )
        {
        Panic( EBMPanicLAF );
        }
    
    return AknLayoutScalable_Apps::popup_call2_audio_second_window_g2( 
        aVariety ).LayoutLine(); 
    }
    
// -----------------------------------------------------------------------------
// BubbleLayout2::popup_call2_audio_second_call_cli_text
// -----------------------------------------------------------------------------
//    
const TAknTextLineLayout 
    BubbleLayout2::popup_call2_audio_second_call_cli_text( TInt aVariety )
    {
    TAknLayoutScalableParameterLimits parLimits =
        AknLayoutScalable_Apps::popup_call2_audio_second_window_t1_ParamLimits();

    if ( aVariety < parLimits.FirstVariety() || aVariety > parLimits.LastVariety() )
        {
        Panic( EBMPanicLAF );
        }
    
    return AknLayoutScalable_Apps::popup_call2_audio_second_window_t1( 
        aVariety ).LayoutLine(); 
    }                
     
// -----------------------------------------------------------------------------
// BubbleLayout2::popup_call2_audio_second_call_timer_text
// -----------------------------------------------------------------------------
//    
const TAknTextLineLayout 
    BubbleLayout2::popup_call2_audio_second_call_timer_text( TInt aVariety )
    {
    TAknLayoutScalableParameterLimits parLimits =
        AknLayoutScalable_Apps::popup_call2_audio_second_window_t2_ParamLimits();

    if ( aVariety < parLimits.FirstVariety() || aVariety > parLimits.LastVariety() )
        {
        Panic( EBMPanicLAF );
        }
    
    return AknLayoutScalable_Apps::popup_call2_audio_second_window_t2( 
        aVariety ).LayoutLine(); 
    }
    
// -----------------------------------------------------------------------------
// BubbleLayout2::popup_call2_audio_second_call_state_text
// -----------------------------------------------------------------------------
//    
const TAknTextLineLayout 
    BubbleLayout2::popup_call2_audio_second_call_state_text( TInt aVariety )
    {
    TAknLayoutScalableParameterLimits parLimits = ( aVariety >= 15 ) ?
      AknLayoutScalable_Apps::popup_call2_audio_second_window_t4_ParamLimits(
       aVariety ):
      AknLayoutScalable_Apps::popup_call2_audio_second_window_t3_ParamLimits();

    if ( aVariety < parLimits.FirstVariety() || aVariety > parLimits.LastVariety() )
        {
        Panic( EBMPanicLAF );
        }
    
    if ( aVariety >= 15 )
        {
        return AknLayoutScalable_Apps::popup_call2_audio_second_window_t4( 
            aVariety ).LayoutLine(); 
        }
    else
        {
        return AknLayoutScalable_Apps::popup_call2_audio_second_window_t3( 
            aVariety ).LayoutLine();     
        }        
    }                                                         

// -----------------------------------------------------------------------------
// BubbleLayout2::popup_call2_audio_second_call_number_text
// -----------------------------------------------------------------------------
//
const TAknTextLineLayout 
    BubbleLayout2::popup_call2_audio_second_call_number_text( TInt aVariety )
    {
    TAknLayoutScalableParameterLimits parLimits =
        AknLayoutScalable_Apps::popup_call2_audio_second_window_t4_ParamLimits( aVariety );

    if ( aVariety < parLimits.FirstVariety() || aVariety > parLimits.LastVariety() )
        {
        Panic( EBMPanicLAF );
        }
    
    return AknLayoutScalable_Apps::popup_call2_audio_second_window_t4( 
        aVariety ).LayoutLine();    
    }
    
////////////////////////////////////////////////////////////////////////////////
// LAF Table 4.58: waiting call bubble contents
////////////////////////////////////////////////////////////////////////////////
    
// -----------------------------------------------------------------------------
// BubbleLayout2::popup_call2_wait_call_background
// -----------------------------------------------------------------------------
//    
const TAknWindowLineLayout 
    BubbleLayout2::popup_call2_audio_wait_call_background()
    {
    return AknLayoutScalable_Apps::bg_popup_call2_in_pane_cp02().LayoutLine(); 
    }
    
// -----------------------------------------------------------------------------
// BubbleLayout2::popup_call2_audio_wait_call_type_icon
// -----------------------------------------------------------------------------
//    
const TAknWindowLineLayout 
    BubbleLayout2::popup_call2_audio_wait_call_type_icon()
    {
    return AknLayoutScalable_Apps::call_type_pane_cp04().LayoutLine(); 
    }
    
// -----------------------------------------------------------------------------
// BubbleLayout2::popup_call2_audio_wait_call_status_icon
// -----------------------------------------------------------------------------
//    
const TAknWindowLineLayout 
    BubbleLayout2::popup_call2_audio_wait_call_status_icon()
    {
    return AknLayoutScalable_Apps::popup_call2_audio_wait_window_g1().LayoutLine(); 
    }
    
// -----------------------------------------------------------------------------
// BubbleLayout2::popup_call2_audio_wait_call_cyphering_icon
// -----------------------------------------------------------------------------
//    
const TAknWindowLineLayout 
    BubbleLayout2::popup_call2_audio_wait_call_cyphering_icon()
    {
    return AknLayoutScalable_Apps::popup_call2_audio_wait_window_g2().LayoutLine(); 
    }
    
// -----------------------------------------------------------------------------
// BubbleLayout2::popup_call2_audio_wait_call_cli_text
// -----------------------------------------------------------------------------
//    
const TAknTextLineLayout 
    BubbleLayout2::popup_call2_audio_wait_call_cli_text()
    {
    return AknLayoutScalable_Apps::popup_call2_audio_wait_window_t3().LayoutLine(); 
    }

    
////////////////////////////////////////////////////////////////////////////////
// LAF Table 4.55: first call bubble contents
////////////////////////////////////////////////////////////////////////////////
       
// -----------------------------------------------------------------------------
// BubbleLayout2::popup_call2_audio_first_call_background
// -----------------------------------------------------------------------------
//    
const TAknWindowLineLayout 
    BubbleLayout2::popup_call2_audio_first_call_background( TInt aVariety )
    {
    TAknLayoutScalableParameterLimits parLimits =
        AknLayoutScalable_Apps::bg_popup_call2_act_pane_ParamLimits();
        
    if ( aVariety < parLimits.FirstVariety() || aVariety > parLimits.LastVariety() )
        {
        Panic( EBMPanicLAF );
        }   
    
    return AknLayoutScalable_Apps::bg_popup_call2_act_pane( aVariety ).LayoutLine();
    }
    
// -----------------------------------------------------------------------------
// BubbleLayout2::popup_call2_audio_first_call_type_icon
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout2::popup_call2_audio_first_call_type_icon( TInt aVariety )
    {
    TAknLayoutScalableParameterLimits parLimits =
        AknLayoutScalable_Apps::call_type_pane_cp03_ParamLimits();
        
    if ( aVariety < parLimits.FirstVariety() || aVariety > parLimits.LastVariety() )
        {
        Panic( EBMPanicLAF );
        }   
        
    return AknLayoutScalable_Apps::call_type_pane_cp03( aVariety ).LayoutLine();
    }    
    
// -----------------------------------------------------------------------------
// BubbleLayout2::popup_call2_audio_first_call_status_icon
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout2::popup_call2_audio_first_call_status_icon( TInt aVariety )
    {
    TAknLayoutScalableParameterLimits parLimits =
        AknLayoutScalable_Apps::popup_call2_audio_first_window_g1_ParamLimits();
        
    if ( aVariety < parLimits.FirstVariety() || aVariety > parLimits.LastVariety() )
        {
        Panic( EBMPanicLAF );
        }   
        
    return AknLayoutScalable_Apps::popup_call2_audio_first_window_g1( 
        aVariety ).LayoutLine();
    }
    
// -----------------------------------------------------------------------------
// BubbleLayout2::popup_call2_audio_first_call_window_ciphering_icon
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout2::popup_call2_audio_first_call_window_ciphering_icon( TInt aVariety )
    {
    TAknLayoutScalableParameterLimits parLimits =
        AknLayoutScalable_Apps::popup_call2_audio_first_window_g2_ParamLimits();
        
    if ( aVariety < parLimits.FirstVariety() || aVariety > parLimits.LastVariety() )
        {
        Panic( EBMPanicLAF );
        }   
        
    return AknLayoutScalable_Apps::popup_call2_audio_first_window_g2( 
        aVariety ).LayoutLine();
    }        

// -----------------------------------------------------------------------------
// BubbleLayout2::popup_call2_audio_first_call_cli_text
// -----------------------------------------------------------------------------
//
const TAknTextLineLayout 
    BubbleLayout2::popup_call2_audio_first_call_cli_text( TInt aVariety )
    {
    TAknLayoutScalableParameterLimits parLimits =
        AknLayoutScalable_Apps::popup_call2_audio_first_window_t1_ParamLimits();
        
    if ( aVariety < parLimits.FirstVariety() || aVariety > parLimits.LastVariety() )
        {
        Panic( EBMPanicLAF );
        }   
        
    return AknLayoutScalable_Apps::popup_call2_audio_first_window_t1( 
        aVariety ).LayoutLine();
    }

// -----------------------------------------------------------------------------
// BubbleLayout2::popup_call2_audio_first_call_cli_text
// -----------------------------------------------------------------------------
//    
const TAknTextLineLayout 
    BubbleLayout2::popup_call2_audio_first_call_number_text( TInt aVariety )
    {
    TAknLayoutScalableParameterLimits parLimits =
        AknLayoutScalable_Apps::popup_call2_audio_first_window_t4_ParamLimits();
        
    if ( aVariety < parLimits.FirstVariety() || aVariety > parLimits.LastVariety() )
        {
        Panic( EBMPanicLAF );
        }   
        
    return AknLayoutScalable_Apps::popup_call2_audio_first_window_t4( 
        aVariety ).LayoutLine();            
    }
    
// -----------------------------------------------------------------------------
// BubbleLayout2::popup_call2_audio_first_call_state_text
// -----------------------------------------------------------------------------
//
const TAknTextLineLayout 
    BubbleLayout2::popup_call2_audio_first_call_state_text( TInt aVariety )
    {
    if ( aVariety < 18 )
        {
        TAknLayoutScalableParameterLimits parLimits =
            AknLayoutScalable_Apps::popup_call2_audio_first_window_t4_ParamLimits();
        
        if ( aVariety < parLimits.FirstVariety() || aVariety > parLimits.LastVariety() )
            {
            Panic( EBMPanicLAF );
            }   
        
        return AknLayoutScalable_Apps::popup_call2_audio_first_window_t4( 
            aVariety ).LayoutLine();
        }
    else
        {
        TAknLayoutScalableParameterLimits parLimits =
            AknLayoutScalable_Apps::popup_call2_audio_first_window_t6_ParamLimits( aVariety );
        
        if ( aVariety < parLimits.FirstVariety() || aVariety > parLimits.LastVariety() )
            {
            Panic( EBMPanicLAF );
            }   
        
        return AknLayoutScalable_Apps::popup_call2_audio_first_window_t6( 
            aVariety ).LayoutLine();
        }
        
    }
    
// -----------------------------------------------------------------------------
// BubbleLayout2::popup_call2_audio_first_call_text_5
// -----------------------------------------------------------------------------
//
const TAknTextLineLayout
    BubbleLayout2::popup_call2_audio_first_call_text_5( TInt aVariety )
    {
    TAknLayoutScalableParameterLimits parLimits =
        AknLayoutScalable_Apps::popup_call2_audio_first_window_t5_ParamLimits();
        
    if ( aVariety < parLimits.FirstVariety() || aVariety > parLimits.LastVariety() )
        {
        Panic( EBMPanicLAF );
        }   
        
    return AknLayoutScalable_Apps::popup_call2_audio_first_window_t5( 
        aVariety ).LayoutLine();
    }


////////////////////////////////////////////////////////////////////////////////
// LAF Table 4.56: CLI graphics (bubble)
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// BubbleLayout2::popup_call2_bubble_pane_graphics
// -----------------------------------------------------------------------------
//    
const TAknWindowLineLayout 
    BubbleLayout2::popup_call2_bubble_pane_graphics()
    {
    return AknLayoutScalable_Apps::bg_popup_call2_bubble_pane().LayoutLine();
    }
    
// -----------------------------------------------------------------------------
// BubbleLayout2::popup_call2_bubble_pane_graphics_cli
// -----------------------------------------------------------------------------
//    
const TAknWindowLineLayout 
    BubbleLayout2::popup_call2_bubble_pane_graphics_cli()
    {
    return AknLayoutScalable_Apps::bg_popup_call2_act_pane_g1().LayoutLine();
    }    

    
////////////////////////////////////////////////////////////////////////////////
// LAF Table 4.54: outgoing call bubble
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// BubbleLayout2::popup_call2_audio_out_background
// -----------------------------------------------------------------------------
//    
const TAknWindowLineLayout 
    BubbleLayout2::popup_call2_audio_out_background( TInt aVariety )
    {
    TAknLayoutScalableParameterLimits parLimits =
        AknLayoutScalable_Apps::bg_popup_call2_in_pane_cp01_ParamLimits();
        
    if ( aVariety < parLimits.FirstVariety() 
        || aVariety > parLimits.LastVariety() )
        {
        Panic( EBMPanicLAF );
        }   
    
    return AknLayoutScalable_Apps::bg_popup_call2_in_pane_cp01( 
        aVariety ).LayoutLine();
    }
    
// -----------------------------------------------------------------------------
// BubbleLayout2::popup_call2_audio_out_call_number_type_icon
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout2::popup_call2_audio_out_call_number_type_icon( TInt aVariety )
    {
    TAknLayoutScalableParameterLimits parLimits =
        AknLayoutScalable_Apps::popup_call2_audio_out_window_g2_ParamLimits();
        
    if ( aVariety < parLimits.FirstVariety() || 
         aVariety > parLimits.LastVariety() )
        {
        Panic( EBMPanicLAF );
        }   
        
    return AknLayoutScalable_Apps::popup_call2_audio_out_window_g2( 
        aVariety ).LayoutLine();
    }    
    
// -----------------------------------------------------------------------------
// BubbleLayout2::popup_call2_audio_out_call_waiting_icon
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout2::popup_call2_audio_out_call_waiting_icon( TInt aVariety )
    {
    TAknLayoutScalableParameterLimits parLimits =
        AknLayoutScalable_Apps::popup_call2_audio_out_window_g1_ParamLimits();
        
    if ( aVariety < parLimits.FirstVariety() || 
         aVariety > parLimits.LastVariety() )
        {
        Panic( EBMPanicLAF );
        }   
        
    return AknLayoutScalable_Apps::popup_call2_audio_out_window_g1( 
        aVariety ).LayoutLine();
    }
    
// -----------------------------------------------------------------------------
// BubbleLayout2::popup_call2_audio_out_ciphering_icon
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout2::popup_call2_audio_out_ciphering_icon( TInt aVariety )
    {
    TAknLayoutScalableParameterLimits parLimits =
        AknLayoutScalable_Apps::popup_call2_audio_out_window_g3_ParamLimits();
        
    if ( aVariety < parLimits.FirstVariety() || 
         aVariety > parLimits.LastVariety() )
        {
        Panic( EBMPanicLAF );
        }   
        
    return AknLayoutScalable_Apps::popup_call2_audio_out_window_g3( 
        aVariety ).LayoutLine();
    }        

// -----------------------------------------------------------------------------
// BubbleLayout2::popup_call2_audio_out_call_text_1
// -----------------------------------------------------------------------------
//
const TAknTextLineLayout 
    BubbleLayout2::popup_call2_audio_out_call_text_1( TInt aVariety )
    {
    TAknLayoutScalableParameterLimits parLimits =
        AknLayoutScalable_Apps::popup_call2_audio_out_window_t1_ParamLimits();
        
    if ( aVariety < parLimits.FirstVariety() || 
         aVariety > parLimits.LastVariety() )
        {
        Panic( EBMPanicLAF );
        }   
        
    return AknLayoutScalable_Apps::popup_call2_audio_out_window_t1( 
        aVariety ).LayoutLine();
    }

// -----------------------------------------------------------------------------
// BubbleLayout2::popup_call2_audio_out_call_text_2
// -----------------------------------------------------------------------------
//
const TAknTextLineLayout 
    BubbleLayout2::popup_call2_audio_out_call_text_2( TInt aVariety )
    {
    TAknLayoutScalableParameterLimits parLimits =
        AknLayoutScalable_Apps::popup_call2_audio_out_window_t2_ParamLimits();
        
    if ( aVariety < parLimits.FirstVariety() || 
         aVariety > parLimits.LastVariety() )
        {
        Panic( EBMPanicLAF );
        }   
        
    return AknLayoutScalable_Apps::popup_call2_audio_out_window_t2( 
        aVariety ).LayoutLine();
    }
    
// -----------------------------------------------------------------------------
// BubbleLayout2::popup_call2_audio_out_call_text_3
// -----------------------------------------------------------------------------
//
const TAknTextLineLayout 
    BubbleLayout2::popup_call2_audio_out_call_text_3( TInt aVariety )
    {
    TAknLayoutScalableParameterLimits parLimits =
        AknLayoutScalable_Apps::popup_call2_audio_out_window_t6_ParamLimits( aVariety );
        
    if ( aVariety < parLimits.FirstVariety() || 
         aVariety > parLimits.LastVariety() )
        {
        Panic( EBMPanicLAF );
        }   
        
    return AknLayoutScalable_Apps::popup_call2_audio_out_window_t6( 
        aVariety ).LayoutLine();
    }    
    
// -----------------------------------------------------------------------------
// BubbleLayout2::popup_call2_audio_out_cli_textual_1
// -----------------------------------------------------------------------------
//
const TAknTextLineLayout 
    BubbleLayout2::popup_call2_audio_out_cli_textual_1( TInt aVariety )
    {
    TAknLayoutScalableParameterLimits parLimits =
        AknLayoutScalable_Apps::popup_call2_audio_out_window_t3_ParamLimits();
        
    if ( aVariety < parLimits.FirstVariety() || 
         aVariety > parLimits.LastVariety() )
        {
        Panic( EBMPanicLAF );
        }   
        
    return AknLayoutScalable_Apps::popup_call2_audio_out_window_t3( 
        aVariety ).LayoutLine();
    }
    
// -----------------------------------------------------------------------------
// BubbleLayout2::popup_call2_audio_out_cli_textual_2
// -----------------------------------------------------------------------------
//
const TAknTextLineLayout 
    BubbleLayout2::popup_call2_audio_out_cli_textual_2( TInt aVariety )
    {
    TAknLayoutScalableParameterLimits parLimits =
        AknLayoutScalable_Apps::popup_call2_audio_out_window_t4_ParamLimits();
        
    if ( aVariety < parLimits.FirstVariety() || 
         aVariety > parLimits.LastVariety() )
        {
        Panic( EBMPanicLAF );
        }   
        
    return AknLayoutScalable_Apps::popup_call2_audio_out_window_t4( 
        aVariety ).LayoutLine();
    }
    
// -----------------------------------------------------------------------------
// BubbleLayout2::popup_call2_audio_out_cli_textual_3
// -----------------------------------------------------------------------------
//
const TAknTextLineLayout 
    BubbleLayout2::popup_call2_audio_out_cli_textual_3( TInt aVariety )
    {
    TAknLayoutScalableParameterLimits parLimits =
        AknLayoutScalable_Apps::popup_call2_audio_out_window_t5_ParamLimits();
        
    if ( aVariety < parLimits.FirstVariety() || 
         aVariety > parLimits.LastVariety() )
        {
        Panic( EBMPanicLAF );
        }   
        
    return AknLayoutScalable_Apps::popup_call2_audio_out_window_t5( 
        aVariety ).LayoutLine();
    }                


////////////////////////////////////////////////////////////////////////////////
// LAF Table 4.54: incoming call bubble
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// BubbleLayout2::popup_call2_audio_in_background
// -----------------------------------------------------------------------------
//    
const TAknWindowLineLayout 
    BubbleLayout2::popup_call2_audio_in_background( TInt aVariety )
    {
    TAknLayoutScalableParameterLimits parLimits =
        AknLayoutScalable_Apps::bg_popup_call2_in_pane_ParamLimits();
        
    if ( aVariety < parLimits.FirstVariety() || 
         aVariety > parLimits.LastVariety() )
        {
        Panic( EBMPanicLAF );
        }   
    
    return AknLayoutScalable_Apps::bg_popup_call2_in_pane( aVariety ).LayoutLine();
    }
    
// -----------------------------------------------------------------------------
// BubbleLayout2::popup_call2_audio_in_call_waiting_icon
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout2::popup_call2_audio_in_call_waiting_icon( TInt aVariety )
    {
    TAknLayoutScalableParameterLimits parLimits =
        AknLayoutScalable_Apps::popup_call2_audio_in_window_g1_ParamLimits();
        
    if ( aVariety < parLimits.FirstVariety() || 
         aVariety > parLimits.LastVariety() )
        {
        Panic( EBMPanicLAF );
        }   
        
    return AknLayoutScalable_Apps::popup_call2_audio_in_window_g1( 
        aVariety ).LayoutLine();
    }
    
// -----------------------------------------------------------------------------
// BubbleLayout2::popup_call2_audio_in_call_number_type_icon
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout2::popup_call2_audio_in_call_number_type_icon( TInt aVariety )
    {
    TAknLayoutScalableParameterLimits parLimits =
        AknLayoutScalable_Apps::popup_call2_audio_in_window_g2_ParamLimits();
        
    if ( aVariety < parLimits.FirstVariety() || 
         aVariety > parLimits.LastVariety() )
        {
        Panic( EBMPanicLAF );
        }   
        
    return AknLayoutScalable_Apps::popup_call2_audio_in_window_g2( 
        aVariety ).LayoutLine();
    }    
    
// -----------------------------------------------------------------------------
// BubbleLayout2::popup_call2_audio_in_ciphering_icon
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout2::popup_call2_audio_in_ciphering_icon( TInt aVariety )
    {
    TAknLayoutScalableParameterLimits parLimits =
        AknLayoutScalable_Apps::popup_call2_audio_in_window_g3_ParamLimits();
        
    if ( aVariety < parLimits.FirstVariety() ||
         aVariety > parLimits.LastVariety() )
        {
        Panic( EBMPanicLAF );
        }   
        
    return AknLayoutScalable_Apps::popup_call2_audio_in_window_g3( 
        aVariety ).LayoutLine();
    }        

// -----------------------------------------------------------------------------
// BubbleLayout2::popup_call2_audio_in_brand_image
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout
    BubbleLayout2::popup_call2_audio_in_brand_image( TInt aVariety )
    {
    TAknLayoutScalableParameterLimits parLimits =
        AknLayoutScalable_Apps::popup_call2_audio_in_window_g4_ParamLimits(0);

   	if ( aVariety < parLimits.FirstVariety() ||
   	    aVariety > parLimits.LastVariety() )
   	    {
   	    Panic( EBMPanicLAF );
   	    }

    return AknLayoutScalable_Apps::popup_call2_audio_in_window_g4(
   	    aVariety ).LayoutLine();
    }

// -----------------------------------------------------------------------------
// BubbleLayout2::popup_call2_audio_out_brand_image
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout
    BubbleLayout2::popup_call2_audio_out_brand_image( TInt aVariety )
    {
    TAknLayoutScalableParameterLimits parLimits =
        AknLayoutScalable_Apps::popup_call2_audio_out_window_g4_ParamLimits(0);

   	if ( aVariety < parLimits.FirstVariety() ||
   	    aVariety > parLimits.LastVariety() )
   	    {
   	    Panic( EBMPanicLAF );
   	    }

    return AknLayoutScalable_Apps::popup_call2_audio_out_window_g4(
   	    aVariety ).LayoutLine();
    }

// -----------------------------------------------------------------------------
// BubbleLayout2::popup_call2_audio_first_brand_image
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout
    BubbleLayout2::popup_call2_audio_first_brand_image( TInt aVariety )
    {
    TAknLayoutScalableParameterLimits parLimits =
        AknLayoutScalable_Apps::popup_call2_audio_first_window_g4_ParamLimits(0);

   	if ( aVariety < parLimits.FirstVariety() ||
   	    aVariety > parLimits.LastVariety() )
   	    {
   	    Panic( EBMPanicLAF );
   	    }

    return AknLayoutScalable_Apps::popup_call2_audio_first_window_g4(
   	    aVariety ).LayoutLine();
    }
    
// -----------------------------------------------------------------------------
// BubbleLayout2::popup_call2_audio_first_brand_image2
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout
    BubbleLayout2::popup_call2_audio_first_brand_image2( TInt aVariety )
    {
    TAknLayoutScalableParameterLimits parLimits =
        AknLayoutScalable_Apps::popup_call2_audio_first_window_g5_ParamLimits(0);

   	if ( aVariety < parLimits.FirstVariety() ||
   	    aVariety > parLimits.LastVariety() )
   	    {
   	    Panic( EBMPanicLAF );
   	    }

    return AknLayoutScalable_Apps::popup_call2_audio_first_window_g5(
   	    aVariety ).LayoutLine();
    }    

// -----------------------------------------------------------------------------
// BubbleLayout2::popup_call2_audio_second_brand_image
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout
    BubbleLayout2::popup_call2_audio_second_brand_image( TInt aVariety )
    {
    TAknLayoutScalableParameterLimits parLimits =
        AknLayoutScalable_Apps::popup_call2_audio_second_window_g3_ParamLimits(0);

   	if ( aVariety < parLimits.FirstVariety() ||
   	    aVariety > parLimits.LastVariety() )
   	    {
   	    Panic( EBMPanicLAF );
   	    }

    return AknLayoutScalable_Apps::popup_call2_audio_second_window_g3(
   	    aVariety ).LayoutLine();
    }
// -----------------------------------------------------------------------------
// BubbleLayout2::popup_call2_audio_in_call_text_1
// -----------------------------------------------------------------------------
//
const TAknTextLineLayout 
    BubbleLayout2::popup_call2_audio_in_call_text_1( TInt aVariety )
    {
    TAknLayoutScalableParameterLimits parLimits =
        AknLayoutScalable_Apps::popup_call2_audio_in_window_t1_ParamLimits();
        
    if ( aVariety < parLimits.FirstVariety() || 
         aVariety > parLimits.LastVariety() )
        {
        Panic( EBMPanicLAF );
        }   
        
    return AknLayoutScalable_Apps::popup_call2_audio_in_window_t1( 
        aVariety ).LayoutLine();
    }

// -----------------------------------------------------------------------------
// BubbleLayout2::popup_call2_audio_out_call_text_2
// -----------------------------------------------------------------------------
//
const TAknTextLineLayout 
    BubbleLayout2::popup_call2_audio_in_call_text_2( TInt aVariety )
    {
    TAknLayoutScalableParameterLimits parLimits =
        AknLayoutScalable_Apps::popup_call2_audio_in_window_t2_ParamLimits();
        
    if ( aVariety < parLimits.FirstVariety() || 
         aVariety > parLimits.LastVariety() )
        {
        Panic( EBMPanicLAF );
        }   
        
    return AknLayoutScalable_Apps::popup_call2_audio_in_window_t2( 
        aVariety ).LayoutLine();
    }
    
// -----------------------------------------------------------------------------
// BubbleLayout2::popup_call2_audio_out_call_text_3
// -----------------------------------------------------------------------------
//
const TAknTextLineLayout 
    BubbleLayout2::popup_call2_audio_in_call_text_3( TInt aVariety )
    {
    TAknLayoutScalableParameterLimits parLimits =
        AknLayoutScalable_Apps::popup_call2_audio_in_window_t3_ParamLimits();
        
    if ( aVariety < parLimits.FirstVariety() || 
         aVariety > parLimits.LastVariety() )
        {
        Panic( EBMPanicLAF );
        }   
        
    return AknLayoutScalable_Apps::popup_call2_audio_in_window_t3(
        aVariety ).LayoutLine();
    }    
    
// -----------------------------------------------------------------------------
// BubbleLayout2::popup_call2_audio_in_cli_textual_1
// -----------------------------------------------------------------------------
//
const TAknTextLineLayout 
    BubbleLayout2::popup_call2_audio_in_cli_textual_1( TInt aVariety )
    {
    TAknLayoutScalableParameterLimits parLimits =
        AknLayoutScalable_Apps::popup_call2_audio_in_window_t4_ParamLimits();
        
    if ( aVariety < parLimits.FirstVariety() ||
         aVariety > parLimits.LastVariety() )
        {
        Panic( EBMPanicLAF );
        }   
        
    return AknLayoutScalable_Apps::popup_call2_audio_in_window_t4( 
        aVariety ).LayoutLine();
    }
    
// -----------------------------------------------------------------------------
// BubbleLayout2::popup_call2_audio_in_cli_textual_2
// -----------------------------------------------------------------------------
//
const TAknTextLineLayout 
    BubbleLayout2::popup_call2_audio_in_cli_textual_2( TInt aVariety )
    {
    TAknLayoutScalableParameterLimits parLimits =
        AknLayoutScalable_Apps::popup_call2_audio_in_window_t5_ParamLimits();
        
    if ( aVariety < parLimits.FirstVariety() || 
         aVariety > parLimits.LastVariety() )
        {
        Panic( EBMPanicLAF );
        }   
        
    return AknLayoutScalable_Apps::popup_call2_audio_in_window_t5( 
        aVariety ).LayoutLine();
    }
    
// -----------------------------------------------------------------------------
// BubbleLayout2::popup_call2_audio_in_cli_textual_3
// -----------------------------------------------------------------------------
//
const TAknTextLineLayout 
    BubbleLayout2::popup_call2_audio_in_cli_textual_3( TInt aVariety )
    {
    TAknLayoutScalableParameterLimits parLimits =
        AknLayoutScalable_Apps::popup_call2_audio_in_window_t6_ParamLimits();
        
    if ( aVariety < parLimits.FirstVariety() || 
         aVariety > parLimits.LastVariety() )
        {
        Panic( EBMPanicLAF );
        }   
        
    return AknLayoutScalable_Apps::popup_call2_audio_in_window_t6( 
        aVariety ).LayoutLine();
    }


////////////////////////////////////////////////////////////////////////////////
// LAF Table 4.52: CLI graphics (rect)
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// BubbleLayout2::popup_call2_bubble_rect_pane_graphics
// -----------------------------------------------------------------------------
//    
const TAknWindowLineLayout 
    BubbleLayout2::popup_call2_bubble_rect_pane_graphics( TInt aVariety )
    {
    TAknLayoutScalableParameterLimits parLimits =
        AknLayoutScalable_Apps::bg_popup_call2_rect_pane_ParamLimits();
        
    if ( aVariety < parLimits.FirstVariety() || 
         aVariety > parLimits.LastVariety() )
        {
        Panic( EBMPanicLAF );
        }   

    return AknLayoutScalable_Apps::bg_popup_call2_rect_pane(
        aVariety ).LayoutLine();
    }
    
// -----------------------------------------------------------------------------
// BubbleLayout2::popup_call2_bubble_rect_pane_graphics_cli
// -----------------------------------------------------------------------------
//    
const TAknWindowLineLayout 
    BubbleLayout2::popup_call2_bubble_rect_pane_graphics_cli()
    {
    return AknLayoutScalable_Apps::bg_popup_call2_in_pane_g1().LayoutLine();
    } 


////////////////////////////////////////////////////////////////////////////////
// LAF Table 4.63: CLI graphics (mainpane)
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// BubbleLayout2::popup_call2_bubble_rect_pane_graphics_cli
// -----------------------------------------------------------------------------
//    
const TAknWindowLineLayout 
    BubbleLayout2::call2_cli_visual_graphic_pane()
    {
    return AknLayoutScalable_Apps::call2_cli_visual_graphic_pane().LayoutLine();
    }
    
// -----------------------------------------------------------------------------
// BubbleLayout2::popup_call2_bubble_rect_pane_graphics_cli
// -----------------------------------------------------------------------------
//    
const TAknWindowLineLayout 
    BubbleLayout2::call2_cli_visual_text_pane()
    {
    return AknLayoutScalable_Apps::call2_cli_visual_text_pane().LayoutLine();
    }    

// -----------------------------------------------------------------------------
// BubbleLayout2::popup_call2_bubble_rect_pane_graphics_cli
// -----------------------------------------------------------------------------
//
const TAknTextLineLayout BubbleLayout2::call2_main_pane_text( TInt aRow )
    {
    TAknLayoutScalableParameterLimits parLimits =
        AknLayoutScalable_Apps::call2_cli_visual_pane_t1_ParamLimits();
        
    if ( aRow > parLimits.LastRow() )
        {
        Panic( EBMPanicLAF );
        }
    
    return AknLayoutScalable_Apps::call2_cli_visual_pane_t1( aRow ).LayoutLine();        
    }

// -----------------------------------------------------------------------------
// BubbleLayout2::call2_cli_visual_text_number_of_rows
// -----------------------------------------------------------------------------
//    
TInt BubbleLayout2::call2_cli_visual_text_number_of_rows()
    {
    TAknLayoutScalableParameterLimits parLimits =
        AknLayoutScalable_Apps::call2_cli_visual_pane_t1_ParamLimits();
        
    return parLimits.LastRow();
    }

// -----------------------------------------------------------------------------
// BubbleLayout2::call2_image_placing_area
// -----------------------------------------------------------------------------
//    
const TAknWindowLineLayout BubbleLayout2::call2_image_placing_area( 
    TInt aVariety )
    {
    TAknLayoutScalableParameterLimits parLimits =
        AknLayoutScalable_Apps::aid_image_placing_area_ParamLimits( aVariety );
     
    if ( Layout_Meta_Data::IsLandscapeOrientation() )
        {
        // portrait 0..1 == landscape 2..3 
        aVariety += 2;    	        
    	}
        
    if ( aVariety > parLimits.LastVariety() )
        {
        Panic( EBMPanicLAF );
        }
    
    return AknLayoutScalable_Apps::aid_image_placing_area( 
        aVariety ).LayoutLine();
    }

////////////////////////////////////////////////////////////////////////////////
// LAF Table 4.65: CLI graphics (mainpane)
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// BubbleLayout2::call2_cli_visual_graphic_pane_g1
// -----------------------------------------------------------------------------
//    
const TAknWindowLineLayout 
    BubbleLayout2::call2_cli_visual_graphic_pane_g1() // image
    {
    return AknLayoutScalable_Apps::call2_cli_visual_graphic_pane_g1().LayoutLine();
    }
    
// -----------------------------------------------------------------------------
// BubbleLayout2::call2_cli_visual_graphic_pane_g2
// -----------------------------------------------------------------------------
//    
const TAknWindowLineLayout 
    BubbleLayout2::call2_cli_visual_graphic_pane_g2() // small graphics
    {
    return AknLayoutScalable_Apps::call2_cli_visual_graphic_pane_g2().LayoutLine();
    }
    
// -----------------------------------------------------------------------------
// BubbleLayout2::call2_cli_visual_graphic_pane_g3
// -----------------------------------------------------------------------------
//    
const TAknWindowLineLayout 
    BubbleLayout2::call2_cli_visual_graphic_pane_g3() // default object
    {
    return AknLayoutScalable_Apps::call2_cli_visual_graphic_pane_g3().LayoutLine();
    }        


////////////////////////////////////////////////////////////////////////////////
// LAF Table 4.53: Rectangle frame
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// BubbleLayout2::call2_rect_pane_fr_cor_center
// -----------------------------------------------------------------------------
//    
const TAknWindowLineLayout 
    BubbleLayout2::call2_rect_pane_fr_cor_center()
    {
    return AknLayoutScalable_Apps::bg_popup_call2_rect_pane_g1().LayoutLine();
    }
    
// -----------------------------------------------------------------------------
// BubbleLayout2::call2_rect__pane_fr_cor_topleft
// -----------------------------------------------------------------------------
//    
const TAknWindowLineLayout 
    BubbleLayout2::call2_rect_pane_fr_cor_topleft()
    {
    return AknLayoutScalable_Apps::bg_popup_call2_rect_pane_g2().LayoutLine();
    }
    
// -----------------------------------------------------------------------------
// BubbleLayout2::call2_rect__pane_fr_cor_topright
// -----------------------------------------------------------------------------
//    
const TAknWindowLineLayout 
    BubbleLayout2::call2_rect_pane_fr_cor_topright()
    {
    return AknLayoutScalable_Apps::bg_popup_call2_rect_pane_g3().LayoutLine();
    }
    
// -----------------------------------------------------------------------------
// BubbleLayout2::call2_rect_pane_fr_cor_bottomleft
// -----------------------------------------------------------------------------
//    
const TAknWindowLineLayout 
    BubbleLayout2::call2_rect_pane_fr_cor_bottomleft()
    {
    return AknLayoutScalable_Apps::bg_popup_call2_rect_pane_g4().LayoutLine();
    } 
    
// -----------------------------------------------------------------------------
// BubbleLayout2::call2_rect_pane_fr_cor_bottomright
// -----------------------------------------------------------------------------
//    
const TAknWindowLineLayout 
    BubbleLayout2::call2_rect_pane_fr_cor_bottomright()
    {
    return AknLayoutScalable_Apps::bg_popup_call2_rect_pane_g5().LayoutLine();
    }
    
// -----------------------------------------------------------------------------
// BubbleLayout2::call2_rect_pane_fr_side_sidetop
// -----------------------------------------------------------------------------
//    
const TAknWindowLineLayout 
    BubbleLayout2::call2_rect_pane_fr_side_top()
    {
    return AknLayoutScalable_Apps::bg_popup_call2_rect_pane_g6().LayoutLine();
    }
    
// -----------------------------------------------------------------------------
// BubbleLayout2::call2_rect_pane_fr_side_bottom
// -----------------------------------------------------------------------------
//    
const TAknWindowLineLayout 
    BubbleLayout2::call2_rect_pane_fr_side_bottom()
    {
    return AknLayoutScalable_Apps::bg_popup_call2_rect_pane_g7().LayoutLine();
    }                        

// -----------------------------------------------------------------------------
// BubbleLayout2::call2_rect_pane_fr_side_left
// -----------------------------------------------------------------------------
//    
const TAknWindowLineLayout 
    BubbleLayout2::call2_rect_pane_fr_side_left()
    {
    return AknLayoutScalable_Apps::bg_popup_call2_rect_pane_g8().LayoutLine();
    }

// -----------------------------------------------------------------------------
// BubbleLayout2::call2_rect_pane_fr_side_right
// -----------------------------------------------------------------------------
//    
const TAknWindowLineLayout 
    BubbleLayout2::call2_rect_pane_fr_side_right()
    {
    return AknLayoutScalable_Apps::bg_popup_call2_rect_pane_g9().LayoutLine();
    }    
    

////////////////////////////////////////////////////////////////////////////////
// LAF Table 4.57: Bubble frame
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// BubbleLayout2::call2_bubble_pane_fr_cor_center
// -----------------------------------------------------------------------------
//    
const TAknWindowLineLayout 
    BubbleLayout2::call2_bubble_pane_fr_cor_center()
    {
    return AknLayoutScalable_Apps::bg_popup_call2_bubble_pane_g1().LayoutLine();
    }
    
// -----------------------------------------------------------------------------
// BubbleLayout2::call2_bubble__pane_fr_cor_topleft
// -----------------------------------------------------------------------------
//    
const TAknWindowLineLayout 
    BubbleLayout2::call2_bubble_pane_fr_cor_topleft()
    {
    return AknLayoutScalable_Apps::bg_popup_call2_bubble_pane_g2().LayoutLine();
    }
    
// -----------------------------------------------------------------------------
// BubbleLayout2::call2_bubble__pane_fr_cor_topright
// -----------------------------------------------------------------------------
//    
const TAknWindowLineLayout 
    BubbleLayout2::call2_bubble_pane_fr_cor_topright()
    {
    return AknLayoutScalable_Apps::bg_popup_call2_bubble_pane_g3().LayoutLine();
    }
    
// -----------------------------------------------------------------------------
// BubbleLayout2::call2_bubble_pane_fr_cor_bottomleft
// -----------------------------------------------------------------------------
//    
const TAknWindowLineLayout 
    BubbleLayout2::call2_bubble_pane_fr_cor_bottomleft()
    {
    return AknLayoutScalable_Apps::bg_popup_call2_bubble_pane_g4().LayoutLine();
    } 
    
// -----------------------------------------------------------------------------
// BubbleLayout2::call2_bubble_pane_fr_cor_bottomright
// -----------------------------------------------------------------------------
//    
const TAknWindowLineLayout 
    BubbleLayout2::call2_bubble_pane_fr_cor_bottomright()
    {
    return AknLayoutScalable_Apps::bg_popup_call2_bubble_pane_g5().LayoutLine();
    }
    
// -----------------------------------------------------------------------------
// BubbleLayout2::call2_bubble_pane_fr_side_sidetop
// -----------------------------------------------------------------------------
//    
const TAknWindowLineLayout 
    BubbleLayout2::call2_bubble_pane_fr_side_top()
    {
    return AknLayoutScalable_Apps::bg_popup_call2_bubble_pane_g6().LayoutLine();
    }
    
// -----------------------------------------------------------------------------
// BubbleLayout2::call2_bubble_pane_fr_side_bottom
// -----------------------------------------------------------------------------
//    
const TAknWindowLineLayout 
    BubbleLayout2::call2_bubble_pane_fr_side_bottom()
    {
    return AknLayoutScalable_Apps::bg_popup_call2_bubble_pane_g7().LayoutLine();
    }                        

// -----------------------------------------------------------------------------
// BubbleLayout2::call2_bubble_pane_fr_side_left
// -----------------------------------------------------------------------------
//    
const TAknWindowLineLayout 
    BubbleLayout2::call2_bubble_pane_fr_side_left()
    {
    return AknLayoutScalable_Apps::bg_popup_call2_bubble_pane_g8().LayoutLine();
    }

// -----------------------------------------------------------------------------
// BubbleLayout2::call2_bubble_pane_fr_side_right
// -----------------------------------------------------------------------------
//    
const TAknWindowLineLayout 
    BubbleLayout2::call2_bubble_pane_fr_side_right()
    {
    return AknLayoutScalable_Apps::bg_popup_call2_bubble_pane_g9().LayoutLine();
    }    

// End of File
