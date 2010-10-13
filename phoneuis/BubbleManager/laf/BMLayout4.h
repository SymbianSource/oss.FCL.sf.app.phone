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
* Description:  Layout definitions from Application LAF (call4/touch layouts)
*
*/



#ifndef BMLAYOUT4_H
#define BMLAYOUT4_H

// INCLUDES
#include <calslbs.h>
#include <bldvariant.hrh> 
#include <aknlayoutdef.h>

/**
* Layout coordinates from Application LAF
* 
* @lib      bubblemanager.lib
* @since    S60 v5.0
*/
class BubbleLayout4
    {
    public:
        ///////////////////////////////////////////////////////////////////////
        // Utils 
        ///////////////////////////////////////////////////////////////////////
        static void button_grid_row_column_count( 
            TInt  aVariety,
            TInt& aRows, 
            TInt& aColumns );
     
    public:
        ///////////////////////////////////////////////////////////////////////
        // LAF Table 1.1: Main call4 pane contents 
        ///////////////////////////////////////////////////////////////////////
        static const TAknWindowLineLayout 
            call4_windows_pane( TInt aVariety );
            
        static const TAknWindowLineLayout 
            call4_button_grid_pane( TInt aVariety );
            
        static const TAknWindowLineLayout 
            call4_image_pane( TInt aVariety );            
            
        ///////////////////////////////////////////////////////////////////////
        // LAF Table 1.2: Call4 windows pane contents 
        ///////////////////////////////////////////////////////////////////////                        
        static const TAknWindowLineLayout 
            popup_call4_audio_first_window( TInt aVariety );
            
        static const TAknWindowLineLayout 
            popup_call4_audio_second_window( TInt aVariety );            
            
        static const TAknWindowLineLayout 
            popup_call4_audio_in_window( TInt aVariety );
            
        static const TAknWindowLineLayout 
            popup_call4_audio_wait_window( TInt aVariety );                        
        
        static const TAknWindowLineLayout 
            popup_call4_audio_conference_window( TInt aVariety );
            
        ///////////////////////////////////////////////////////////////////////
        // LAF Table 1.3: Call4 windows conf pane contents 
        ///////////////////////////////////////////////////////////////////////
        static const TAknWindowLineLayout 
            popup_call4_audio_conference_window_pane( TInt aVariety, 
                                                      TInt aRow );
        static const TAknWindowLineLayout 
            popup_call4_conf_pane_background();
            
        static const TAknWindowLineLayout 
            popup_call4_conf_cli_list();
            
        static const TAknWindowLineLayout 
            popup_call4_conf_single_list_graphic_pane( TInt aRow );            
        
        static const TAknWindowLineLayout 
            popup_call4_conf_single_list_highlight_pane();
            
        static const TAknWindowLineLayout 
            popup_call4_conf_single_list_pane_call_status_icon();
            
        static const TAknWindowLineLayout 
            popup_call4_conf_single_list_pane_ciphering_icon();
            
        static const TAknTextLineLayout 
            popup_call4_conf_single_list_pane_cli_text();                                                      
            
        ///////////////////////////////////////////////////////////////////////
        // LAF Table 1.4: Grid call4 button pane contents
        ///////////////////////////////////////////////////////////////////////
        static const TAknWindowLineLayout 
            cell_call4_button_pane( TInt aVariety, TInt aCol, TInt aRow );
            
        ///////////////////////////////////////////////////////////////////////
        // LAF Table 1.5: Cell call4 button pane contents
        ///////////////////////////////////////////////////////////////////////
        static const TAknWindowLineLayout 
            button_call4_background_graphics( TInt aVariety );
            
        static const TAknWindowLineLayout 
            button_call4_function_graphics( TInt aVariety );
            
        static const TAknTextLineLayout 
            button_call4_function_text( TInt aVariety );            
    };

#endif      // BMLAYOUT4_H
            
// End of File
