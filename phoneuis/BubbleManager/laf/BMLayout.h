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
* Description:  Layout definitions from Application LAF
*
*/



#ifndef BMLAYOUT_H
#define BMLAYOUT_H

// INCLUDES
#include <calslbs.h>
#include <bldvariant.hrh> 
#include <AknLayoutDef.h>

/**
* Layout coordinates from Application LAF
* 
* @lib      bubblemanager.lib
* @since    2.0
*/
class BubbleLayout
    { 
    public: // Helper functions

        /**
        * Get background image colour
        * @return White colour
        */ 
        static TInt LayoutColourWhite();

        /**
        * Get main pane rect
        * @return the main pane rect
        * since   Series60_2.8
        */ 
        static const TRect MainPaneRect();

    public: // Layout coordinates

        /**
        * Popup window coordinates.
        * One function returns one line of coordinates is Application LAF.
        * @since 2.0
        * @param aLine Line number from the same item. From 1-n
        * @param aParentWidth Parent control's width
        * @param aParentHeight Parent control's height
        * @param aImageWidth Thumbnail image's width
        * @param aImageHeight Thumbnail image's height
        * @return Layout for current item.
        */

        // Table 78. Pop up windows (main pane as parent)
        static const TAknWindowLineLayout 
            popup_number_entry_window( TInt aIndex = 0 );
        static const TAknWindowLineLayout 
            popup_call_audio_in_window( TInt aLine );
        static const TAknWindowLineLayout 
            popup_call_audio_out_window( TInt aLine );
        static const TAknWindowLineLayout 
            popup_call_audio_first_window( TInt aLine );
        static const TAknWindowLineLayout 
            popup_call_audio_wait_window( TInt aLine );
        static const TAknWindowLineLayout 
            popup_call_audio_second_window( TInt aLine );
        static const TAknWindowLineLayout 
            popup_call_audio_conf_window( TInt aLine );

        // Table 80. Pop up window list pane descendants (call, conf)
        static const TAknWindowLineLayout 
            list_single_graphic_popup_conf_pane( TInt aLine );

        // Table 81. List pane elements (conf, single, graphic)
        static const TAknWindowLineLayout
            list_single_graphic_popup_conf_pane_elements_1();
        static const TAknWindowLineLayout 
            list_single_graphic_popup_conf_pane_elements_2();

        // Table 82. List pane texts (conf, single, graphic)
        static const TAknTextLineLayout 
            list_single_graphic_popup_conf_pane_texts_1();

        // Table 85. Highlight graphics 
        static const TAknWindowLineLayout 
            list_single_graphic_popup_conf_pane_graphics_1( const TRect& aParentRect ); // shadow
        static const TAknWindowLineLayout 
            list_single_graphic_popup_conf_pane_graphics_2( const TRect& aParentRect ); // highlight

        // Table 91. Number entry pop up window texts
        static const TAknTextLineLayout 
            popup_number_entry_window_texts_1( TInt aIndex );
        static const TAknTextLineLayout 
            popup_number_entry_window_texts_2( TInt aIndex, TInt aLine );
        static const TAknTextLineLayout 
            popup_number_entry_window_texts_3( TInt aLine );            

        // Table 92. Number entry pop up window graphics
        static const TAknWindowLineLayout 
            popup_number_entry_window_graphics_1( TInt aIndex );

        // Table 119. Incoming call pop up window elements
        static const TAknWindowLineLayout 
            popup_call_audio_in_window_1_elements_1();
        static const TAknWindowLineLayout 
            popup_call_audio_in_window_1_elements_2();
        static const TAknWindowLineLayout 
            popup_call_audio_in_window_1_elements_15();

        // Table 120. Incoming call pop up window texts
        // Use line 1 or 2 (or 3 CNAP). Right and Width values are calculated 
        // from the image width.
        static const TAknTextLineLayout 
            popup_call_audio_in_window_1_texts_1( TInt aLine, TInt aImageWidth );
        static const TAknTextLineLayout 
            popup_call_audio_in_window_1_texts_2( TInt aLine, TInt aImageWidth );

        // Table 121. Incoming call pop up window graphics
        static const TAknWindowLineLayout 
            popup_call_audio_in_window_1_graphics_1( TInt aLine );

        // Table 122. Incoming call pop-up window elements (ne)
        static const TAknWindowLineLayout 
            popup_call_audio_in_window_2_elements_1();
        static const TAknWindowLineLayout 
            popup_call_audio_in_window_2_elements_2();
        static const TAknWindowLineLayout 
            popup_call_audio_in_window_2_elements_15();

        // Table 123. Incoming call pop up window texts (ne)
        static const TAknTextLineLayout 
            popup_call_audio_in_window_2_texts_1( TInt aLine, 
                                                  TInt aImageWidth );
        static const TAknTextLineLayout 
            popup_call_audio_in_window_2_texts_2( TInt aLine, TInt aImageWidth );

        // Table 124. Incoming call pop up window graphics (ne) 
        static const TAknWindowLineLayout 
            popup_call_audio_in_window_2_graphics_1();

        // Table 125. Outgoing call pop up window elements (held)
        static const TAknWindowLineLayout 
            popup_call_audio_out_window_1_elements_1();
        static const TAknWindowLineLayout 
            popup_call_audio_out_window_1_elements_2();
        static const TAknWindowLineLayout 
            popup_call_audio_out_window_1_elements_15();

        // Table 126. Outgoing call pop up window texts (held)
        static const TAknTextLineLayout 
            popup_call_audio_out_window_1_texts_1( TInt aLine, 
                                                   TInt aImageWidth );

        // Table 127. Outgoing call pop up window graphics (held)
        static const TAknWindowLineLayout 
            popup_call_audio_out_window_1_graphics_1();

        // Table 128. Outgoing call pop up window elements (held, ne)
        static const TAknWindowLineLayout 
            popup_call_audio_out_window_2_elements_1();
        static const TAknWindowLineLayout 
            popup_call_audio_out_window_2_elements_2();
        static const TAknWindowLineLayout 
            popup_call_audio_out_window_2_elements_3();
        static const TAknWindowLineLayout 
            popup_call_audio_out_window_2_elements_15();

        // Table 129. Outgoing call pop up window texts (held, ne)
        static const TAknTextLineLayout 
            popup_call_audio_out_window_2_texts_1();

        // Table 130. Outgoing call pop up window graphics (held, ne)
        static const TAknWindowLineLayout 
            popup_call_audio_out_window_2_graphics_1();

        // Table 131. First call pop up window elements (one call)
        static const TAknWindowLineLayout 
            popup_call_audio_first_window_1_elements_1();
        static const TAknWindowLineLayout 
            popup_call_audio_first_window_1_elements_2();
        static const TAknWindowLineLayout 
            popup_call_audio_first_window_1_elements_3();
        
        // Table 132. First call pop up window texts (one call)
        static const TAknTextLineLayout 
            popup_call_audio_first_window_1_texts_1( TInt aLine, 
                            TInt aImageWidth );
        static const TAknTextLineLayout 
            popup_call_audio_first_window_1_texts_2( TInt aLine, 
                            TInt aImageWidth );

        static const TAknTextLineLayout 
            popup_call_audio_first_window_1_texts_3();

        // Table 133. First call pop up window graphics (one call)
        static const TAknWindowLineLayout 
            popup_call_audio_first_window_1_graphics_1();

        // Table 134. First call pop up window elements (two calls)
        static const TAknWindowLineLayout 
            popup_call_audio_first_window_2_elements_1();
        static const TAknWindowLineLayout 
            popup_call_audio_first_window_2_elements_2();
        static const TAknWindowLineLayout 
            popup_call_audio_first_window_2_elements_15();

        // Table 135. First call pop up window texts (two calls)
        static const TAknTextLineLayout 
            popup_call_audio_first_window_2_texts_1( TInt aLine , 
                                                     TInt aImageWidth );
        static const TAknTextLineLayout 
            popup_call_audio_first_window_2_texts_2( TInt aLine, 
                                                     TInt aImageWidth );
        static const TAknTextLineLayout 
            popup_call_audio_first_window_2_texts_3();

        // Table 136. First call pop up window graphics (two calls)
        static const TAknWindowLineLayout 
            popup_call_audio_first_window_2_graphics_1();

        // Table 137. First call pop up window elements (two calls, a waiting call)
        static const TAknWindowLineLayout 
            popup_call_audio_first_window_4_elements_1();
        static const TAknWindowLineLayout 
            popup_call_audio_first_window_4_elements_2();
        static const TAknWindowLineLayout 
            popup_call_audio_first_window_4_elements_3();

        // Table 138. First call pop up window texts (two calls, a waiting call)
        static const TAknTextLineLayout 
            popup_call_audio_first_window_4_texts_1();

        // Table 139. First call pop up window graphics (two calls, a waiting call)
        static const TAknWindowLineLayout 
            popup_call_audio_first_window_4_graphics_1();

        // Table 140. First call pop up window graphics (two, wait, ne)
        static const TAknWindowLineLayout 
            popup_call_audio_first_window_6_graphics_1();

        // Table 5.1. First call pop up window graphics (emergency call)
        static const AknLayoutUtils::SAknLayoutControl
            popup_call_audio_first_window_7_graphics_1();

        // Table 141. Waiting call pop up window elements (held, out, ne)
        static const TAknWindowLineLayout 
            popup_call_audio_wait_window_3_elements_1();
        static const TAknWindowLineLayout 
            popup_call_audio_wait_window_3_elements_2();
        static const TAknWindowLineLayout 
            popup_call_audio_wait_window_3_elements_3();

        // Table 142. Waiting call pop up window texts (held, out, ne)
        static const TAknTextLineLayout 
            popup_call_audio_wait_window_3_texts_1();

        // Table 143. Waiting call pop up window graphics (held, out, ne)
        static const TAknWindowLineLayout 
            popup_call_audio_wait_window_3_graphics_1();

        // Table 144. Waiting call pop up window graphics (out)
        static const TAknWindowLineLayout 
            popup_call_audio_wait_window_4_graphics_1();

        // Table 145. Second call pop up window elements (two)
        static const TAknWindowLineLayout 
            popup_call_audio_second_window_1_elements_1();
        static const TAknWindowLineLayout 
            popup_call_audio_second_window_1_elements_2();
        static const TAknWindowLineLayout 
            popup_call_audio_second_window_1_elements_3();

        // Table 146. Second call pop up window texts (two calls)
        static const TAknTextLineLayout 
            popup_call_audio_second_window_1_texts_1( TInt aLine , 
                                                      TInt aImageWidth );
        static const TAknTextLineLayout 
            popup_call_audio_second_window_1_texts_2( TInt aImageWidth );

        // Table 147. Second call pop up window graphics (two calls)
        static const TAknWindowLineLayout 
            popup_call_audio_second_window_1_graphics_1();

        // Table 148. Second call pop up window elements (two calls, waiting call)
        static const TAknWindowLineLayout 
            popup_call_audio_second_window_3_elements_1();
        static const TAknWindowLineLayout 
            popup_call_audio_second_window_3_elements_2();
        static const TAknWindowLineLayout 
            popup_call_audio_second_window_3_elements_3();

        // Table 149. Second call pop up window texts (two calls, waiting call)
        static const TAknTextLineLayout 
            popup_call_audio_second_window_3_texts_1();

        // Table 150. Second call pop up window graphics (two calls, waiting call)
        static const TAknWindowLineLayout 
            popup_call_audio_second_window_3_graphics_1();

        // Table 151. Conference call pop up window descendant and element
        static const TAknWindowLineLayout 
            popup_call_audio_conf_window_elements_1( const TRect& aParentRect );
        static const TAknWindowLineLayout 
            popup_call_audio_conf_window_elements_2( TInt aLine );

        // Non-specified
        static const TAknWindowLineLayout
            ConferencePane();

        // Table 152. Conference call pop up window texts
        static const TAknTextLineLayout 
            popup_call_audio_cont_window_texts_1( TInt aLine );

        // Table 153. Muted state elements
        static const TAknWindowLineLayout 
            main_pane_elements_muted( TBool aNumberEditorActive );

        // Non-specified
        static const TAknWindowLineLayout 
            call_type_pane_elements_1();
        static const TAknWindowLineLayout 
            call_type_pane_elements_2();

    };

#endif      // BMLAYOUT_H
            
// End of File
