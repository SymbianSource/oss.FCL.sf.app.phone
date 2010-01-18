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
* Description:  Layout definitions from Application LAF (Call4 layouts)
*
*/


// INCLUDE FILES
#include "BMLayout4.h"
#include "BMPanic.h"
#include "BMBubbleManager.h"
#include <aknenv.h>
#include <applayout.cdl.h>
#include <AknUtils.h>
#include <AknLayout2ScalableDef.h>
#include <aknlayoutscalable_apps.cdl.h>
#include <layoutmetadata.cdl.h>


// ============================ MEMBER FUNCTIONS ===============================


////////////////////////////////////////////////////////////////////////////////
//
// Utility functions
//
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// BubbleLayout4::button_grid_row_column_count
// -----------------------------------------------------------------------------
//
void BubbleLayout4::button_grid_row_column_count( 
    TInt  aVariety,
    TInt& aRows, 
    TInt& aColumns )
    {
    if ( Layout_Meta_Data::IsLandscapeOrientation() )
        {
        // portrait 0..1 == landscape 2..3 
        aVariety += 1;    	        
    	}
    	
    TAknLayoutScalableParameterLimits parLimits =
        AknLayoutScalable_Apps::cell_call4_button_pane_ParamLimits( aVariety );
    aRows = parLimits.LastRow() + 1;        
    aColumns = parLimits.LastColumn() + 1;        
    }

    
////////////////////////////////////////////////////////////////////////////////
//
// Call4 Layouts
//
////////////////////////////////////////////////////////////////////////////////    

// -----------------------------------------------------------------------------
// BubbleLayout4::call4_windows_pane
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout
    BubbleLayout4::call4_windows_pane( TInt aVariety )
    {
    if ( Layout_Meta_Data::IsLandscapeOrientation() )
        {
        // portrait 0..1 == landscape 2..3 
        aVariety += 1;    	        
    	} 
    	
    TAknLayoutScalableParameterLimits parLimits =
        AknLayoutScalable_Apps::call4_windows_pane_ParamLimits( aVariety );
        
    if ( aVariety < parLimits.FirstVariety() || 
         aVariety > parLimits.LastVariety() )
        {
        Panic( EBMPanicLAF );
        }   
        
    return AknLayoutScalable_Apps::call4_windows_pane( 
        aVariety ).LayoutLine();
    }

// -----------------------------------------------------------------------------
// BubbleLayout4::call4_button_grid_pane
// -----------------------------------------------------------------------------
//            
const TAknWindowLineLayout
    BubbleLayout4::call4_button_grid_pane( TInt aVariety )
    {
    if ( Layout_Meta_Data::IsLandscapeOrientation() )
        {
        // portrait 0..1 == landscape 2..3 
        aVariety += 1;    	        
    	} 
    	
    TAknLayoutScalableParameterLimits parLimits =
        AknLayoutScalable_Apps::grid_call4_button_pane_ParamLimits( aVariety );
        
    if ( aVariety < parLimits.FirstVariety() || 
         aVariety > parLimits.LastVariety() )
        {
        Panic( EBMPanicLAF );
        }   
        
    return AknLayoutScalable_Apps::grid_call4_button_pane( 
        aVariety ).LayoutLine();
    }

// -----------------------------------------------------------------------------
// BubbleLayout4::call4_image_pane
// -----------------------------------------------------------------------------
//    
const TAknWindowLineLayout 
    BubbleLayout4::call4_image_pane( TInt aVariety )
    {
    if ( Layout_Meta_Data::IsLandscapeOrientation() )
        {
        // portrait 2 == landscape 1 
        aVariety -= 1;    	        
    	} 
    	
    TAknLayoutScalableParameterLimits parLimits =
        AknLayoutScalable_Apps::call4_image_pane_ParamLimits( aVariety );
        
    if ( aVariety < parLimits.FirstVariety() || 
         aVariety > parLimits.LastVariety() )
        {
        Panic( EBMPanicLAF );
        }   
        
    return AknLayoutScalable_Apps::call4_image_pane( 
        aVariety ).LayoutLine();    
    }

// -----------------------------------------------------------------------------
// BubbleLayout4::popup_call4_audio_first_window
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout4::popup_call4_audio_first_window( TInt aVariety )
    {
    TAknLayoutScalableParameterLimits parLimits =
        AknLayoutScalable_Apps::popup_call4_audio_first_window_ParamLimits( 
            aVariety );
        
    if ( aVariety < parLimits.FirstVariety() || 
         aVariety > parLimits.LastVariety() )
        {
        Panic( EBMPanicLAF );
        }   
        
    return AknLayoutScalable_Apps::popup_call4_audio_first_window( 
        aVariety ).LayoutLine();    
    }
    
// -----------------------------------------------------------------------------
// BubbleLayout4::popup_call4_audio_second_window
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout4::popup_call4_audio_second_window( TInt aVariety )
    {
    TAknLayoutScalableParameterLimits parLimits =
        AknLayoutScalable_Apps::popup_call4_audio_second_window_ParamLimits( 
            aVariety );
        
    if ( aVariety < parLimits.FirstVariety() || 
         aVariety > parLimits.LastVariety() )
        {
        Panic( EBMPanicLAF );
        }   
        
    return AknLayoutScalable_Apps::popup_call4_audio_second_window( 
        aVariety ).LayoutLine();    
    }
    
// -----------------------------------------------------------------------------
// BubbleLayout4::popup_call4_audio_in_window
// -----------------------------------------------------------------------------
//    
const TAknWindowLineLayout 
    BubbleLayout4::popup_call4_audio_in_window( TInt aVariety )
    {
    return AknLayoutScalable_Apps::popup_call4_audio_in_window( 
        aVariety ).LayoutLine();    
    }
    
// -----------------------------------------------------------------------------
// BubbleLayout4::popup_call4_audio_wait_window
// -----------------------------------------------------------------------------
//    
const TAknWindowLineLayout 
    BubbleLayout4::popup_call4_audio_wait_window( TInt aVariety )
    {
    return AknLayoutScalable_Apps::popup_call4_audio_wait_window( 
        aVariety ).LayoutLine();    
    }                    
    
// -----------------------------------------------------------------------------
// BubbleLayout4::popup_call4_audio_conference_window
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout4::popup_call4_audio_conference_window( TInt aVariety )
    {
    // same variety in portrait and landscape
    return AknLayoutScalable_Apps::call4_windows_conf_pane( 
        aVariety ).LayoutLine();    
    }    

// -----------------------------------------------------------------------------
// BubbleLayout4::popup_call4_audio_conference_window
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout4::popup_call4_audio_conference_window_pane( 
        TInt aVariety,
        TInt aRow )
    {
    // same variety in portrait and landscape
    TAknLayoutScalableParameterLimits parLimits =
        AknLayoutScalable_Apps::popup_call4_audio_conf_window_ParamLimits(
            aVariety );
        
    if ( aVariety < parLimits.FirstVariety() || 
         aVariety > parLimits.LastVariety() )
        {
        Panic( EBMPanicLAF );
        }    

    return AknLayoutScalable_Apps::popup_call4_audio_conf_window( 
        aVariety, 0, aRow ).LayoutLine();    
    }
    
// -----------------------------------------------------------------------------
// BubbleLayout4::popup_call4_conf_pane_bg
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout4::popup_call4_conf_pane_background()
    {
    return AknLayoutScalable_Apps::bg_popup_call2_act_pane_cp02().LayoutLine();
    }
    
// -----------------------------------------------------------------------------
// BubbleLayout4::popup_call4_conf_cli_list
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout4::popup_call4_conf_cli_list()
    {
    return AknLayoutScalable_Apps::call4_list_conf_pane().LayoutLine();
    }
    
// -----------------------------------------------------------------------------
// BubbleLayout4::popup_call4_conf_single_list_graphic_pane
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout4::popup_call4_conf_single_list_graphic_pane( TInt aRow )
    {
    TAknLayoutScalableParameterLimits parLimits =
        AknLayoutScalable_Apps::list_single_graphic_popup_conf4_pane_ParamLimits();

    /*if ( aRow < parLimits.FirstVariety() || aRow > parLimits.LastVariety() )
        {
        Panic( EBMPanicLAF );
        }*/
    
    return AknLayoutScalable_Apps::
        list_single_graphic_popup_conf4_pane( 0, 0, aRow ).LayoutLine();    
    }        
    
// -----------------------------------------------------------------------------
// BubbleLayout4::popup_call4_conf_single_list_highlight_pane
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout4::popup_call4_conf_single_list_highlight_pane()
    {
    return AknLayoutScalable_Apps::list_highlight_pane_cp022().LayoutLine();
    }
    
// -----------------------------------------------------------------------------
// BubbleLayout4::popup_call2_conf_single_list_pane_call_status_icon
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout4::popup_call4_conf_single_list_pane_call_status_icon()
    {
    return AknLayoutScalable_Apps::list_single_graphic_popup_conf4_pane_g1().LayoutLine();
    }
    
// -----------------------------------------------------------------------------
// BubbleLayout4::popup_call4_conf_single_list_pane_ciphering_icon
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout4::popup_call4_conf_single_list_pane_ciphering_icon()
    {
    return AknLayoutScalable_Apps::list_single_graphic_popup_conf4_pane_g2().LayoutLine();
    }
    
// -----------------------------------------------------------------------------
// BubbleLayout4::popup_call2_conf_single_list_pane_cli_text
// -----------------------------------------------------------------------------
//
const TAknTextLineLayout 
    BubbleLayout4::popup_call4_conf_single_list_pane_cli_text()
    {
    return AknLayoutScalable_Apps::list_single_graphic_popup_conf4_pane_t1().LayoutLine();
    }    

// -----------------------------------------------------------------------------
// BubbleLayout4::cell_call4_button_pane
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout4::cell_call4_button_pane( TInt aVariety, TInt aCol, TInt aRow )
    {
    if ( Layout_Meta_Data::IsLandscapeOrientation() )
        {
        // portrait 0..1 == landscape 2..3 
        aVariety += 1;    	        
    	}
    
    TAknLayoutScalableParameterLimits parLimits =
        AknLayoutScalable_Apps::cell_call4_button_pane_ParamLimits( aVariety );
        
    if ( aVariety < parLimits.FirstVariety() || 
         aVariety > parLimits.LastVariety() )
        {
        Panic( EBMPanicLAF );
        }   
        
    return AknLayoutScalable_Apps::cell_call4_button_pane( 
        aVariety, aCol, aRow ).LayoutLine();    
    }

// -----------------------------------------------------------------------------
// BubbleLayout4::button_call4_background_graphics
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout4::button_call4_background_graphics( TInt aVariety )
    {
    TAknLayoutScalableParameterLimits parLimits =
        AknLayoutScalable_Apps::bg_button_pane_cp09_ParamLimits( aVariety );
        
    if ( Layout_Meta_Data::IsLandscapeOrientation() )
        {
        // portrait 0..1 == landscape 2..3 
        aVariety += 2;    	        
    	}        
        
    if ( aVariety < parLimits.FirstVariety() || 
         aVariety > parLimits.LastVariety() )
        {
        Panic( EBMPanicLAF );
        }   
        
    return AknLayoutScalable_Apps::bg_button_pane_cp09( 
        aVariety ).LayoutLine();    
    }

// -----------------------------------------------------------------------------
// BubbleLayout4::button_call4_function_graphics
// -----------------------------------------------------------------------------
//            
const TAknWindowLineLayout 
    BubbleLayout4::button_call4_function_graphics( TInt aVariety )
    {
    if ( Layout_Meta_Data::IsLandscapeOrientation() )
        {
        // portrait 0..1 == landscape 2..3 
        aVariety += 2;    	        
    	}
    
    TAknLayoutScalableParameterLimits parLimits =
        AknLayoutScalable_Apps::cell_call4_button_pane_g1_ParamLimits( 
            aVariety );
        
    if ( aVariety < parLimits.FirstVariety() || 
         aVariety > parLimits.LastVariety() )
        {
        Panic( EBMPanicLAF );
        }   
        
    return AknLayoutScalable_Apps::cell_call4_button_pane_g1( 
        aVariety ).LayoutLine();    
    }

// -----------------------------------------------------------------------------
// BubbleLayout4::button_call4_function_text
// -----------------------------------------------------------------------------
//            
const TAknTextLineLayout 
    BubbleLayout4::button_call4_function_text( TInt aVariety )
    {
    if ( Layout_Meta_Data::IsLandscapeOrientation() )
        {
        // portrait 0..1 == landscape 2..3 
        aVariety += 2;    	        
    	}
    
    TAknLayoutScalableParameterLimits parLimits =
        AknLayoutScalable_Apps::cell_call4_button_pane_t1_ParamLimits( 
            aVariety );
        
    if ( aVariety < parLimits.FirstVariety() || 
         aVariety > parLimits.LastVariety() )
        {
        Panic( EBMPanicLAF );
        }   
        
    return AknLayoutScalable_Apps::cell_call4_button_pane_t1( 
        aVariety ).LayoutLine();    
    }


    

// End of File
