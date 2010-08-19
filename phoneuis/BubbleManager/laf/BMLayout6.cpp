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


#include "BMLayout6.h"
#include "BMPanic.h"
#include "BMBubbleManager.h"
#include <aknenv.h>
#include <applayout.cdl.h>
#include <AknUtils.h>
#include <AknLayout2ScalableDef.h>
#include <aknlayoutscalable_apps.cdl.h>
#include <layoutmetadata.cdl.h>





const TAknWindowLineLayout
    BubbleLayout6::call6_button_grp_pane( TInt aVariety )
    {
    if ( Layout_Meta_Data::IsLandscapeOrientation() )
        {
        // portrait 0 == landscape 1
        aVariety += 1;              
        } 
        
    TAknLayoutScalableParameterLimits parLimits =
        AknLayoutScalable_Apps::call6_btn_grp_pane_ParamLimits( aVariety );
        
    if ( aVariety < parLimits.FirstVariety() || 
         aVariety > parLimits.LastVariety() )
        {
        Panic( EBMPanicLAF );
        }   
        
    return AknLayoutScalable_Apps::call6_btn_grp_pane( 
        aVariety ).LayoutLine();
    }


// -----------------------------------------------------------------------------
// BubbleLayout6::button_grp_row_column_count
// -----------------------------------------------------------------------------
//
void BubbleLayout6::button_grp_row_column_count( 
    TInt  aVariety,
    TInt& aRows, 
    TInt& aColumns )
    {
    if ( Layout_Meta_Data::IsLandscapeOrientation() )
        {
        // portrait 0 == landscape 1 
        aVariety += 1;              
        }
        
    TAknLayoutScalableParameterLimits parLimits =
        AknLayoutScalable_Apps::cell_call6_btn_pane_ParamLimits( aVariety );
    aRows = parLimits.LastRow() + 1;        
    aColumns = parLimits.LastColumn() + 1;        
    }


// -----------------------------------------------------------------------------
// BubbleLayout6::cell_call4_button_pane
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout6::cell_call6_button_pane( TInt aVariety, TInt aCol, TInt aRow )
    {
    if ( Layout_Meta_Data::IsLandscapeOrientation() )
        {
        // portrait 0 == landscape 1
        aVariety += 1;              
        }
    
    TAknLayoutScalableParameterLimits parLimits =
        AknLayoutScalable_Apps::cell_call6_btn_pane_ParamLimits( aVariety );
        
    if ( aVariety < parLimits.FirstVariety() || 
         aVariety > parLimits.LastVariety() )
        {
        Panic( EBMPanicLAF );
        }   
        
    return AknLayoutScalable_Apps::cell_call6_btn_pane( 
        aVariety, aCol, aRow ).LayoutLine();    
    }

const TAknTextLineLayout 
    BubbleLayout6::button_call6_function_text( TInt aVariety )
    {
    if ( Layout_Meta_Data::IsLandscapeOrientation() )
        {
        // portrait 0 == landscape 1 
        aVariety += 1;              
        }

    TAknLayoutScalableParameterLimits parLimits =
        AknLayoutScalable_Apps::cell_call6_btn_pane_ParamLimits( 
            aVariety );
        
    if ( aVariety < parLimits.FirstVariety() || 
         aVariety > parLimits.LastVariety() )
        {
        Panic( EBMPanicLAF );
        }   
        
    return AknLayoutScalable_Apps::cell_call6_btn_pane_t1( aVariety ).LayoutLine();    
    }


// -----------------------------------------------------------------------------
// BubbleLayout6::button_call6_function_graphics
// -----------------------------------------------------------------------------
//            
const TAknWindowLineLayout 
    BubbleLayout6::button_call6_function_graphics( TInt aVariety )
    {
    if ( Layout_Meta_Data::IsLandscapeOrientation() )
        {
        // portrait 0 == landscape 1 
        aVariety += 1;              
        }
    
    TAknLayoutScalableParameterLimits parLimits =
        AknLayoutScalable_Apps::cell_call6_btn_pane_ParamLimits( 
            aVariety );
    
    if ( aVariety < parLimits.FirstVariety() || 
         aVariety > parLimits.LastVariety() )
        {
        Panic( EBMPanicLAF );
        }   
        
    return AknLayoutScalable_Apps::cell_call6_btn_pane_g1( 
        aVariety ).LayoutLine();    
    }

const TAknWindowLineLayout
    BubbleLayout6::call6_windows_pane( TInt aVariety )
    {
    if ( Layout_Meta_Data::IsLandscapeOrientation() )
        {
        // portrait 0 == landscape 1
        aVariety += 1;              
        } 
        
    TAknLayoutScalableParameterLimits parLimits =
        AknLayoutScalable_Apps::call6_pane_g1_ParamLimits( aVariety );
        
    if ( aVariety < parLimits.FirstVariety() || 
         aVariety > parLimits.LastVariety() )
        {
        Panic( EBMPanicLAF );
        }   
        
    return AknLayoutScalable_Apps::call6_pane_g1( 
        aVariety ).LayoutLine();
    }

// -----------------------------------------------------------------------------
// BubbleLayout6::button_call6_background_graphics
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout6::button_call6_background_graphics( TInt aVariety )
    {
    if ( Layout_Meta_Data::IsLandscapeOrientation() )
        {
        // portrait 0 == landscape 1  
        aVariety += 1;              
        }        
         
    return AknLayoutScalable_Apps::bg_button_pane_cp15( 
        aVariety ).LayoutLine();    
    }

// -----------------------------------------------------------------------------
// BubbleLayout6::popup_call6_audio_first_window
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout6::popup_call6_audio_first_window( TInt aVariety )
    {
    
    if ( Layout_Meta_Data::IsLandscapeOrientation() )
        {
        // portrait 0 == landscape 1
        aVariety += 1;              
        }    
    
    TAknLayoutScalableParameterLimits parLimits =
        AknLayoutScalable_Apps::popup_call6_1st_window_ParamLimits( 
            aVariety );
        
    if ( aVariety < parLimits.FirstVariety() || 
         aVariety > parLimits.LastVariety() )
        {
        Panic( EBMPanicLAF );
        }   
        
    return AknLayoutScalable_Apps::popup_call6_1st_window( 
        aVariety ).LayoutLine();    
    }
 
 

