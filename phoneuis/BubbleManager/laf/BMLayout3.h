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
* Description:  Call3 layout definitions from Application LAF
*
*/



#ifndef BMLAYOUT3_H
#define BMLAYOUT3_H

// INCLUDES
#include <calslbs.h>
#include <bldvariant.hrh> 
#include <AknLayoutDef.h>

/**
* Call3 (video cli) layout coordinates from Application LAF 
* 
* @lib      bubblemanager.lib
* @since    3.1
*/
class BubbleLayout3
    { 
    public: 
        /**
        * Window layout for video ringing tone
        * @since Series 60 3.1
        * @param aVariety LAF variety
        * @return Window rectange for video ringing tone
        */
        static TRect popup_call3_audio_in_window_rect( TInt aVariety );
        
        
        /**
        * Wrappers for layout data functions.
        * @since Series 60 3.1
        * @param aVariety LAF variety
        * @return Component layout
        */
        static const TAknWindowLineLayout 
            main_call3_pane();
        
        static const TAknWindowLineLayout 
            popup_call3_audio_in_window( TInt aVariety );
        
        static const TAknWindowLineLayout
            popup_call3_audio_in_pane( TInt aVariety );
        
        static const TAknWindowLineLayout
            popup_call3_audio_in_window_fr_center();
        
        static const TAknWindowLineLayout 
            popup_call3_audio_in_window_fr_corner_topleft();
        
        static const TAknWindowLineLayout 
            popup_call3_audio_in_window_fr_corner_topright();
            
        static const TAknWindowLineLayout 
            popup_call3_audio_in_window_fr_corner_bottomleft();            
        
        static const TAknWindowLineLayout
            popup_call3_audio_in_window_fr_corner_bottomright();
        
        static const TAknWindowLineLayout 
            popup_call3_audio_in_window_call_indication( TInt aVariety );
        
        static const TAknWindowLineLayout
            popup_call3_audio_in_window_numbertype_indication( TInt aVariety );
        
        static const TAknWindowLineLayout
            popup_call3_audio_in_window_cipheringoff_indication( TInt aVariety );
        
        static const TAknWindowLineLayout
            popup_call3_audio_in_window_separator_line( TInt aVariety );
        
        static const TAknTextLineLayout
            popup_call3_audio_in_window_text_line_1( TInt aVariety );
        
        static const TAknTextLineLayout
            popup_call3_audio_in_window_text_line_2( TInt aVariety );
       
        static const TAknTextLineLayout
            popup_call3_audio_in_window_text_line_3( TInt aVariety );
        
        static const TAknWindowLineLayout
            call3_video_qcif_pane( TInt aVariety );
        
        static const TAknWindowLineLayout
            call3_video_subqcif_pane( TInt aVariety );

        static const TAknWindowLineLayout
            call3_video_qcif_uncrop_pane( TInt aVariety );
        
        static const TAknWindowLineLayout
            call3_video_subqcif_uncrop_pane( TInt aVariety );            
    };

#endif // BMLAYOUT3_H
            
// End of File
