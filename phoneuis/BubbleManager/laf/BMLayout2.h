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
* Description:  Layout definitions from Application LAF (Call2 layouts)
*
*/



#ifndef BMLAYOUT2_H
#define BMLAYOUT2_H

// INCLUDES
#include <calslbs.h>
#include <bldvariant.hrh> 
#include <aknlayoutdef.h>

/**
* Layout coordinates from Application LAF
* 
* @lib      bubblemanager.lib
* @since    2.0
*/
class BubbleLayout2
    { 
    public: // Helper functions
        /**
        * Get inner and outer rectangles for rectangle frame.
        * @param aFrameRect Frame rectangle.
        * @param aOuterRect Outer rectangle (return).
        * @param aInnerRect Inner rectangle (return).
        */ 
        static void RectFrameInnerOuterRects( const TRect& aFrameRect,
                                              TRect& aOuterRect,
                                              TRect& aInnerRect );

        /**
        * Get inner and outer rectangles for bubble frame.
        * @param aFrameRect Frame rectangle.
        * @param aOuterRect Outer rectangle (return).
        * @param aInnerRect Inner rectangle (return).
        */ 
        static void BubbleFrameInnerOuterRects( const TRect& aFrameRect,
                                                TRect& aOuterRect,
                                                TRect& aInnerRect );                                       

    public: // Layout coordinates
    
        ///////////////////////////////////////////////////////////////////////
        // LAF Table 4.50: bubble/rect window positions
        ///////////////////////////////////////////////////////////////////////
        static const TAknWindowLineLayout 
            popup_call2_audio_first_window( TInt aVariety );                        
            
        static const TAknWindowLineLayout 
            popup_call2_audio_in_window( TInt aVariety );
            
        static const TAknWindowLineLayout 
            popup_call2_audio_out_window( TInt aVariety );
            
        static const TAknWindowLineLayout 
            popup_call2_audio_second_window( TInt aVariety );
            
        static const TAknWindowLineLayout 
            popup_call2_audio_wait_window( TInt aVariety );
            
        static const TAknWindowLineLayout 
            popup_call2_audio_conf_window( TInt aVariety );

        static const TAknWindowLineLayout 
            call2_call_object_visual_pane();            

        ///////////////////////////////////////////////////////////////////////            
        // LAF Table 4.60: conference call bubble contents
        ///////////////////////////////////////////////////////////////////////
        static const TAknWindowLineLayout 
            popup_call2_conf_pane_background();
            
        static const TAknWindowLineLayout 
            popup_call2_conf_cli_list();
            
        static const TAknTextLineLayout 
            popup_call2_conf_window_call_timer_text();
    
        ///////////////////////////////////////////////////////////////////////
        // LAF Table 4.61: conference call bubble bg graphics
        ///////////////////////////////////////////////////////////////////////
        static const TAknWindowLineLayout 
            popup_call2_conf_single_list_graphic_pane( TInt aRow );                                                

        ///////////////////////////////////////////////////////////////////////
        // LAF Table 4.62: conference call members list entries
        ///////////////////////////////////////////////////////////////////////            
        static const TAknWindowLineLayout 
            popup_call2_conf_single_list_highlight_pane();
            
        static const TAknWindowLineLayout 
            popup_call2_conf_single_list_pane_call_status_icon();
            
        static const TAknWindowLineLayout 
            popup_call2_conf_single_list_pane_ciphering_icon();
            
        static const TAknTextLineLayout 
            popup_call2_conf_single_list_pane_cli_text();                                                

        ///////////////////////////////////////////////////////////////////////
        // LAF Table 4.59: second call bubble contents           
        ///////////////////////////////////////////////////////////////////////
        static const TAknWindowLineLayout 
            popup_call2_audio_second_call_background( TInt aVariety );
       
        static const TAknWindowLineLayout 
            popup_call2_audio_second_call_type_icon( TInt aVariety );
            
        static const TAknWindowLineLayout 
            popup_call2_audio_second_call_status_icon( TInt aVariety );
            
        static const TAknWindowLineLayout 
            popup_call2_audio_second_call_cyphering_icon( TInt aVariety );
            
        static const TAknTextLineLayout 
            popup_call2_audio_second_call_cli_text( TInt aVariety );
            
        static const TAknTextLineLayout 
            popup_call2_audio_second_call_timer_text( TInt aVariety );                                                
        
        static const TAknTextLineLayout 
            popup_call2_audio_second_call_state_text( TInt aVariety );
            
        static const TAknTextLineLayout
            popup_call2_audio_second_call_number_text( TInt aVariety );
            
        ///////////////////////////////////////////////////////////////////////
        // LAF Table 4.58: waiting call bubble contents
        ///////////////////////////////////////////////////////////////////////
        static const TAknWindowLineLayout 
            popup_call2_audio_wait_call_background();
            
        static const TAknWindowLineLayout 
            popup_call2_audio_wait_call_type_icon();
            
        static const TAknWindowLineLayout 
            popup_call2_audio_wait_call_status_icon();
            
        static const TAknWindowLineLayout 
            popup_call2_audio_wait_call_cyphering_icon();
            
        static const TAknTextLineLayout 
            popup_call2_audio_wait_call_cli_text();                                                            
            
        ///////////////////////////////////////////////////////////////////////
        // LAF Table 4.55: first call bubble contents
        ///////////////////////////////////////////////////////////////////////
        static const TAknWindowLineLayout 
            popup_call2_audio_first_call_background( TInt aVariety );
    
        static const TAknWindowLineLayout 
            popup_call2_audio_first_call_type_icon( TInt aVariety );
    
        static const TAknWindowLineLayout 
            popup_call2_audio_first_call_status_icon( TInt aVariety );
    
        static const TAknWindowLineLayout 
            popup_call2_audio_first_call_window_ciphering_icon( TInt aVariety );
    
        static const TAknTextLineLayout 
            popup_call2_audio_first_call_cli_text( TInt aVariety );
            
        static const TAknTextLineLayout 
            popup_call2_audio_first_call_number_text( TInt aVariety );            
    
        static const TAknTextLineLayout 
            popup_call2_audio_first_call_state_text( TInt aVariety );
    
        static const TAknTextLineLayout
            popup_call2_audio_first_call_text_5( TInt aVariety );
        
        ///////////////////////////////////////////////////////////////////////
        // LAF Table 4.56: CLI graphics (bubble)
        ///////////////////////////////////////////////////////////////////////
        static const TAknWindowLineLayout 
            popup_call2_bubble_pane_graphics();            
            
        static const TAknWindowLineLayout 
            popup_call2_bubble_pane_graphics_cli();
            
        ///////////////////////////////////////////////////////////////////////
        // LAF Table 4.54: outgoing call bubble
        ///////////////////////////////////////////////////////////////////////
        static const TAknWindowLineLayout 
            popup_call2_audio_out_background( TInt aVariety );
    
        static const TAknWindowLineLayout 
            popup_call2_audio_out_call_number_type_icon( TInt aVariety );
    
        static const TAknWindowLineLayout 
            popup_call2_audio_out_call_waiting_icon( TInt aVariety );
    
        static const TAknWindowLineLayout 
            popup_call2_audio_out_ciphering_icon( TInt aVariety );
    
        static const TAknTextLineLayout 
            popup_call2_audio_out_call_text_1( TInt aVariety );

        static const TAknTextLineLayout 
            popup_call2_audio_out_call_text_2( TInt aVariety );
            
        static const TAknTextLineLayout 
            popup_call2_audio_out_call_text_3( TInt aVariety );            
    
        static const TAknTextLineLayout 
            popup_call2_audio_out_cli_textual_1( TInt aVariety );

        static const TAknTextLineLayout 
            popup_call2_audio_out_cli_textual_2( TInt aVariety );

        static const TAknTextLineLayout 
            popup_call2_audio_out_cli_textual_3( TInt aVariety );                            

        ///////////////////////////////////////////////////////////////////////
        // LAF Table 4.54: incoming call bubble
        ///////////////////////////////////////////////////////////////////////
        static const TAknWindowLineLayout 
            popup_call2_audio_in_background( TInt aVariety );
            
        static const TAknWindowLineLayout 
            popup_call2_audio_in_call_waiting_icon( TInt aVariety );
    
        static const TAknWindowLineLayout 
            popup_call2_audio_in_call_number_type_icon( TInt aVariety );
    
        static const TAknWindowLineLayout 
            popup_call2_audio_in_ciphering_icon( TInt aVariety );
    
        static const TAknTextLineLayout 
            popup_call2_audio_in_call_text_1( TInt aVariety );
    
        static const TAknTextLineLayout 
            popup_call2_audio_in_call_text_2( TInt aVariety );

        static const TAknTextLineLayout 
            popup_call2_audio_in_call_text_3( TInt aVariety );
        
        static const TAknTextLineLayout 
            popup_call2_audio_in_cli_textual_1( TInt aVariety );

        static const TAknTextLineLayout 
            popup_call2_audio_in_cli_textual_2( TInt aVariety );
    
        static const TAknTextLineLayout 
            popup_call2_audio_in_cli_textual_3( TInt aVariety );

        static const TAknWindowLineLayout
            popup_call2_audio_in_brand_image( TInt aVariety );

        static const TAknWindowLineLayout
      	    popup_call2_audio_out_brand_image( TInt aVariety );

      	static const TAknWindowLineLayout
      	    popup_call2_audio_first_brand_image( TInt aVariety );
      	    
        // option 2
        static const TAknWindowLineLayout
      	    popup_call2_audio_first_brand_image2( TInt aVariety );      	    

      	static const TAknWindowLineLayout
      	    popup_call2_audio_second_brand_image( TInt aVariety );

        ///////////////////////////////////////////////////////////////////////
        // LAF Table 4.52: CLI graphics (rect)
        ///////////////////////////////////////////////////////////////////////
        static const TAknWindowLineLayout 
            popup_call2_bubble_rect_pane_graphics( TInt aVariety );
            
        static const TAknWindowLineLayout 
            popup_call2_bubble_rect_pane_graphics_cli();
            
        ///////////////////////////////////////////////////////////////////////
        // LAF Table 4.63: CLI graphics (mainpane)
        ///////////////////////////////////////////////////////////////////////
        static const TAknWindowLineLayout 
            call2_cli_visual_graphic_pane();
            
        static const TAknWindowLineLayout 
            call2_cli_visual_text_pane();
            
        static const TAknTextLineLayout 
            call2_main_pane_text( TInt aRow );
            
        static TInt call2_cli_visual_text_number_of_rows();
        
        static const TAknWindowLineLayout 
            call2_image_placing_area( TInt aVariety );                
                    
        ///////////////////////////////////////////////////////////////////////
        // LAF Table 4.65: CLI graphics (mainpane)
        ///////////////////////////////////////////////////////////////////////
        // Large image
        static const TAknWindowLineLayout 
            call2_cli_visual_graphic_pane_g1();
        // Tiny image    
        static const TAknWindowLineLayout 
            call2_cli_visual_graphic_pane_g2();            
        // Default call object            
        static const TAknWindowLineLayout 
            call2_cli_visual_graphic_pane_g3();            

        ///////////////////////////////////////////////////////////////////////
        // LAF Table 4.53: Rectangle frame
        ///////////////////////////////////////////////////////////////////////
        static const TAknWindowLineLayout 
            call2_rect_pane_fr_cor_center();
    
        static const TAknWindowLineLayout 
            call2_rect_pane_fr_cor_topleft();
    
        static const TAknWindowLineLayout 
            call2_rect_pane_fr_cor_topright();
    
        static const TAknWindowLineLayout 
            call2_rect_pane_fr_cor_bottomleft();

        static const TAknWindowLineLayout 
            call2_rect_pane_fr_cor_bottomright();

        static const TAknWindowLineLayout 
            call2_rect_pane_fr_side_top();
    
        static const TAknWindowLineLayout 
            call2_rect_pane_fr_side_bottom();
    
        static const TAknWindowLineLayout 
            call2_rect_pane_fr_side_left();
    
        static const TAknWindowLineLayout 
            call2_rect_pane_fr_side_right();
            
        ///////////////////////////////////////////////////////////////////////
        // LAF Table 4.57: Bubble frame
        ///////////////////////////////////////////////////////////////////////
        static const TAknWindowLineLayout 
            call2_bubble_pane_fr_cor_center();
    
        static const TAknWindowLineLayout 
            call2_bubble_pane_fr_cor_topleft();
    
        static const TAknWindowLineLayout 
            call2_bubble_pane_fr_cor_topright();
    
        static const TAknWindowLineLayout 
            call2_bubble_pane_fr_cor_bottomleft();

        static const TAknWindowLineLayout 
            call2_bubble_pane_fr_cor_bottomright();

        static const TAknWindowLineLayout 
            call2_bubble_pane_fr_side_top();
    
        static const TAknWindowLineLayout 
            call2_bubble_pane_fr_side_bottom();
    
        static const TAknWindowLineLayout 
            call2_bubble_pane_fr_side_left();
    
        static const TAknWindowLineLayout 
            call2_bubble_pane_fr_side_right();            
            
        
    };

#endif      // BMLAYOUT2_H
            
// End of File
