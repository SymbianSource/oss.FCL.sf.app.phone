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
* Description:  LAF coordinates
*
*/


// INCLUDE FILES
#include "BMLayout.h"
#include "BMPanic.h"
#include "BMBubbleManager.h"
#include <aknenv.h>
#include <applayout.cdl.h>
#include <AknUtils.h>
#include <AknLayout2ScalableDef.h>
#include <aknlayoutscalable_apps.cdl.h>
#include <layoutmetadata.cdl.h>

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// BubbleLayout::LayoutColourWhite
// -----------------------------------------------------------------------------
//
TInt BubbleLayout::LayoutColourWhite()
    {
    return 0;
    }

// -----------------------------------------------------------------------------
// BubbleLayout::MainPaneRect
// -----------------------------------------------------------------------------
//
const TRect BubbleLayout::MainPaneRect()
    {
    TSize screenSize = CEikonEnv::Static()->ScreenDevice()->SizeInPixels();
    TRect screenRect( TPoint(0,0), screenSize );

    TAknLayoutRect appRect;
    appRect.LayoutRect( screenRect, AknLayout::application_window( screenRect ) );
    TAknLayoutRect mainRect;
    mainRect.LayoutRect( appRect.Rect(), 
        AknLayout::main_pane( appRect.Rect(), 0, 1, 1 ) ); // 1 = normal status pane, control pane

    return mainRect.Rect();
    }

// -----------------------------------------------------------------------------
// BubbleLayout::popup_number_entry_window
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout::popup_number_entry_window( TInt aIndex )
    {
    if ( AknLayoutUtils::ScalableLayoutInterfaceAvailable() )
	    {
	    if ( Layout_Meta_Data::IsLandscapeOrientation() )
    	    {
    	    if ( aIndex < 4 ) 
        	    {
        	    // portrait 0..1 == landscape 2..3
                aIndex += 2;    	                	        
        	    }
        	else
        	    {
        	    // portrait 4 == landscape 5
        	    aIndex += 1;    
        	    }
    	    }
	            
        TAknWindowComponentLayout layout1 = 
            AknLayoutScalable_Apps::main_call_pane();
        TAknWindowComponentLayout layout2 = 
            AknLayoutScalable_Apps::popup_number_entry_window( aIndex );
        return TAknWindowComponentLayout::
            Compose( layout1, layout2 ).LayoutLine();
	    }
    else
        {
        return AppLayout::popup_number_entry_window();
        }
    }

// -----------------------------------------------------------------------------
// BubbleLayout::popup_call_audio_in_window
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout::popup_call_audio_in_window( TInt aLine )
    {
    if ( AknLayoutUtils::ScalableLayoutInterfaceAvailable() )
	    {
        if ( Layout_Meta_Data::IsLandscapeOrientation() )
            {
            // portrait 0..3 == landscape 4..7 
            aLine += 4;    	        
    	    }        
    	    
        if ( aLine < 1 || aLine > 8 )
            {
            Panic( EBMPanicLAF );
            }        	    
	    
	    return AknLayoutScalable_Apps::popup_call_audio_in_window( 
	                                   aLine - 1 ).LayoutLine();
	    }
    else
        {
        if ( aLine < 1 || aLine > 4 )
            {
            Panic( EBMPanicLAF );
            }    
        
        return AppLayout::popup_call_audio_in_window( aLine - 1 );                
        }	    
    }

// -----------------------------------------------------------------------------
// BubbleLayout::popup_call_audio_out_window
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout::popup_call_audio_out_window( TInt aLine )
    {
    if ( AknLayoutUtils::ScalableLayoutInterfaceAvailable() )
	    {
        if ( Layout_Meta_Data::IsLandscapeOrientation() )
            {
            // portrait 0..2 == landscape 3..6 
            aLine += 3;    	        
    	    }        
	    
	    return AknLayoutScalable_Apps::popup_call_audio_out_window( 
	                                   aLine - 1 ).LayoutLine();;
	    }
    else
        {
        if ( aLine < 1 || aLine > 2 )
            {
            Panic( EBMPanicLAF );
            }    
        
        return AppLayout::popup_call_audio_out_window( aLine - 1 );                
        }
    }

// -----------------------------------------------------------------------------
// BubbleLayout::popup_call_audio_first_window
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout::popup_call_audio_first_window( TInt aLine )
    {
    if ( AknLayoutUtils::ScalableLayoutInterfaceAvailable() )
	    {
        if ( Layout_Meta_Data::IsLandscapeOrientation() )
            {
            // portrait 0..6 == landscape 7..13 
            aLine += 7;    	        
    	    }        
	    
	    return AknLayoutScalable_Apps::popup_call_audio_first_window( 
	                                   aLine - 1 ).LayoutLine();;
	    }
    else
        {
        if ( aLine < 1 || aLine > 7 )
            {
            Panic( EBMPanicLAF );
            }  
        
        // aLine = 3, 2, 4, 6 called before bootup fails.
        TInt command1 = aLine - 1;
        TInt command2 = command1;
        return AppLayout::popup_call_audio_first_window( command1, command2 );
        }
    }

// -----------------------------------------------------------------------------
// BubbleLayout::popup_call_audio_wait_window
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout::popup_call_audio_wait_window( TInt aLine )
    {
    if ( AknLayoutUtils::ScalableLayoutInterfaceAvailable() )
	    {
        if ( Layout_Meta_Data::IsLandscapeOrientation() )
            {
            // portrait 0..3 == landscape 4..7 
            aLine += 4;    	        
    	    }        
	    
	    return AknLayoutScalable_Apps::popup_call_audio_wait_window( 
	                                   aLine - 1 ).LayoutLine();;
	    }
    else
        {
        if ( aLine < 1 || aLine > 4 )
            {
            Panic( EBMPanicLAF );
            } 
        
        return AppLayout::popup_call_audio_wait_window( aLine - 1 );
        }
    }

// -----------------------------------------------------------------------------
// BubbleLayout::popup_call_audio_second_window
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout::popup_call_audio_second_window( TInt aLine )
    {
    if ( AknLayoutUtils::ScalableLayoutInterfaceAvailable() )
	    {
        if ( Layout_Meta_Data::IsLandscapeOrientation() )
        	{
        	// portrait 0..4 == landscape 5..9 
            aLine += 5;    	        
        	}        
	    
	    return AknLayoutScalable_Apps::popup_call_audio_second_window(
	                                   aLine - 1 ).LayoutLine();
	    }
    else
        {
        if ( aLine < 1 || aLine > 5 )
            {
            Panic( EBMPanicLAF );
            }
        
        TInt command1 = aLine - 1;
        TInt command2 = command1;
        return AppLayout::popup_call_audio_second_window( command1, command2 );
        }
    }

// -----------------------------------------------------------------------------
// BubbleLayout::popup_call_audio_conf_window
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout::popup_call_audio_conf_window( TInt aLine )
    {
    if ( AknLayoutUtils::ScalableLayoutInterfaceAvailable() )
	    {
        if ( Layout_Meta_Data::IsLandscapeOrientation() )
        	{
        	// portrait 0..3 == landscape 4..7 
            aLine += 4;    	        
        	}        
	    
	    return AknLayoutScalable_Apps::popup_call_audio_conf_window(
	                                   aLine - 1 ).LayoutLine();
	    }
    else
        {
        if ( aLine < 1 || aLine > 4 )
            {
            Panic( EBMPanicLAF );
            }
        
        return AppLayout::popup_call_audio_conf_window( aLine - 1 );   
        }
    }

// -----------------------------------------------------------------------------
// BubbleLayout::list_single_graphic_popup_conf_pane
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout::list_single_graphic_popup_conf_pane( TInt aLine )
    {
    if ( aLine < 1 || aLine > 6 )
        {
        Panic( EBMPanicLAF );
        }
    return AppLayout::list_single_graphic_popup_conf_pane( aLine - 1 );     
    }

// -----------------------------------------------------------------------------
// BubbleLayout::list_single_graphic_popup_conf_pane_elements_1
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout
    BubbleLayout::list_single_graphic_popup_conf_pane_elements_1()
    {
    return AppLayout::List_pane_elements__conf_single_graphic__Line_1(); 
    }

// -----------------------------------------------------------------------------
// BubbleLayout::list_single_graphic_popup_conf_pane_elements_2
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout::list_single_graphic_popup_conf_pane_elements_2()
    {
    return AppLayout::List_pane_elements__conf_single_graphic__Line_2(); 
    }

// -----------------------------------------------------------------------------
// BubbleLayout::list_single_graphic_popup_conf_pane_texts_1
// -----------------------------------------------------------------------------
//
const TAknTextLineLayout 
    BubbleLayout::list_single_graphic_popup_conf_pane_texts_1()
    {    
    return AppLayout::List_pane_texts__conf_single_graphic__Line_1(); 
    }

// -----------------------------------------------------------------------------
// BubbleLayout::list_single_graphic_popup_conf_pane_graphics_1
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout::list_single_graphic_popup_conf_pane_graphics_1
    ( const TRect& aParentRect )
    {
    return AppLayout::Highlight_graphics_Line_1( aParentRect );
    }

// -----------------------------------------------------------------------------
// BubbleLayout::list_single_graphic_popup_conf_pane_graphics_2
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout::list_single_graphic_popup_conf_pane_graphics_2
    ( const TRect& aParentRect )
    {
    return AppLayout::Highlight_graphics_Line_2( aParentRect ); 
    }

// -----------------------------------------------------------------------------
// BubbleLayout::popup_number_entry_window_texts_1
// -----------------------------------------------------------------------------
//

const TAknTextLineLayout 
    BubbleLayout::popup_number_entry_window_texts_1( TInt aIndex )
    {
    if ( aIndex < 0 || aIndex > 1 )
        {
        Panic( EBMPanicLAF );
        }
    return AppLayout::Number_entry_pop_up_window_texts_Line_1( aIndex ); 
    }

// -----------------------------------------------------------------------------
// BubbleLayout::popup_number_entry_window_texts_2
// -----------------------------------------------------------------------------
//

const TAknTextLineLayout 
    BubbleLayout::popup_number_entry_window_texts_2( TInt aIndex, TInt aLine )
    {
    if ( ( aIndex < 0 || aIndex > 1 ) || ( aLine < 0 || aLine > 1 ) )
        {
        Panic( EBMPanicLAF );
        }

    if ( AknLayoutUtils::ScalableLayoutInterfaceAvailable() )
        {
        aIndex = 0;
        }
    return AppLayout::Number_entry_pop_up_window_texts_Line_2( aIndex, aLine );
    }

// -----------------------------------------------------------------------------
// BubbleLayout::popup_number_entry_window_texts_2
// -----------------------------------------------------------------------------
//    
const TAknTextLineLayout 
    BubbleLayout::popup_number_entry_window_texts_3( TInt aLine )
    {
    if ( aLine < 0 || aLine > 1 )
        {
        Panic( EBMPanicLAF );
        }
        
    if ( aLine == 0  )    
        {
        return AknLayoutScalable_Apps::text_title_cp2(1).LayoutLine();    
        }
    else
        {
        return AknLayoutScalable_Apps::popup_number_entry_window_t5(1).LayoutLine();    
        }            
    }

// -----------------------------------------------------------------------------
// BubbleLayout::popup_number_entry_window_graphics_1
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout::popup_number_entry_window_graphics_1( TInt aIndex )
    {
    return AppLayout::Number_entry_pop_up_window_graphics_Line_1( aIndex );
    }

// -----------------------------------------------------------------------------
// BubbleLayout::popup_call_audio_in_window_1_elements_1
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout::popup_call_audio_in_window_1_elements_1()
    {    
    return AppLayout::Incoming_call_pop_up_window_elements_Line_1();
    }

// -----------------------------------------------------------------------------
// BubbleLayout::popup_call_audio_in_window_1_elements_2
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout::popup_call_audio_in_window_1_elements_2()
    {
    return AppLayout::Incoming_call_pop_up_window_elements_Line_2(); 
    }

// -----------------------------------------------------------------------------
// BubbleLayout::popup_call_audio_in_window_1_elements_15
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout::popup_call_audio_in_window_1_elements_15()
    {
    return AppLayout::Incoming_call_pop_up_window_elements_Line_15(); 
    }

// -----------------------------------------------------------------------------
// BubbleLayout::popup_call_audio_in_window_1_texts_1
// -----------------------------------------------------------------------------
//

const TAknTextLineLayout 
    BubbleLayout::popup_call_audio_in_window_1_texts_1( TInt aLine, 
                                                        TInt aType )
    {
    if ( aLine < 1 && aLine > 2 )
        {
        Panic( EBMPanicLAF );
        }    
    TInt command1 = aLine - 1 ;
    TInt command2 = aType;
    return AppLayout::Incoming_call_pop_up_window_texts_Line_1( command1, command2 ); 
    }


// -----------------------------------------------------------------------------
// BubbleLayout::popup_call_audio_in_window_1_texts_2
// -----------------------------------------------------------------------------
//

const TAknTextLineLayout 
    BubbleLayout::popup_call_audio_in_window_1_texts_2( TInt aLine, 
                                                        TInt aImageType )
    {
    if ( aLine < 1 && aLine > 3 )
        {
        Panic( EBMPanicLAF );
        }    
    TInt command1 = aLine - 1 ;    
    TInt command2 = aImageType;
    return AppLayout::Incoming_call_pop_up_window_texts_Line_2( command1, command2 ); 
    }

// -----------------------------------------------------------------------------
// BubbleLayout::popup_call_audio_in_window_1_graphics_1
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout::popup_call_audio_in_window_1_graphics_1( TInt aLine )
    {
    if ( aLine < 1 && aLine > 2 )
        {
        Panic( EBMPanicLAF );
        }
    TInt index_h = aLine - 1;
    return AppLayout::Incoming_call_pop_up_window_graphics_Line_1( index_h ); 
    }

// -----------------------------------------------------------------------------
// BubbleLayout::popup_call_audio_in_window_2_elements_1
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout::popup_call_audio_in_window_2_elements_1()
    {
    return AppLayout::Incoming_call_pop_up_window_elements__NE__Line_1(); 
    }

// -----------------------------------------------------------------------------
// BubbleLayout::popup_call_audio_in_window_2_elements_2
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout::popup_call_audio_in_window_2_elements_2()
    {
    return AppLayout::Incoming_call_pop_up_window_elements__NE__Line_2();
    }

// -----------------------------------------------------------------------------
// BubbleLayout::popup_call_audio_in_window_2_elements_15
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout::popup_call_audio_in_window_2_elements_15()
    {
    return AppLayout::Incoming_call_pop_up_window_elements__NE__Line_15(); 
    }

// -----------------------------------------------------------------------------
// BubbleLayout::popup_call_audio_in_window_2_texts_1
// -----------------------------------------------------------------------------
//

const TAknTextLineLayout 
    BubbleLayout::popup_call_audio_in_window_2_texts_1( TInt aLine, 
                                                        TInt aType )
    {
    if ( aLine < 1 || aLine > 2 )
        {
        Panic( EBMPanicLAF );
        }
    TInt command1 = aLine - 1;
    TInt command2 = aType;
    return AppLayout::Incoming_call_pop_up_window_texts__NE__Line_1( command1, command2 );
    }

// -----------------------------------------------------------------------------
// BubbleLayout::popup_call_audio_in_window_2_texts_2
// -----------------------------------------------------------------------------
//

const TAknTextLineLayout 
    BubbleLayout::popup_call_audio_in_window_2_texts_2( TInt aLine, 
                                                        TInt aType )
    {
    if ( aLine < 1 || aLine > 3 )
        {
        Panic( EBMPanicLAF );
        }
    TInt command1 = aLine - 1;
    TInt command2 = aType;
    return AppLayout::Incoming_call_pop_up_window_texts__NE__Line_2( command1, command2 ); 
    }

// -----------------------------------------------------------------------------
// BubbleLayout::popup_call_audio_in_window_2_graphics_1
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout::popup_call_audio_in_window_2_graphics_1()
    {
    return AppLayout::Incoming_call_pop_up_window_graphics__NE__Line_1();
    }

// -----------------------------------------------------------------------------
// BubbleLayout::popup_call_audio_out_window_1_elements_1
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout::popup_call_audio_out_window_1_elements_1()
    {
    return AppLayout::Outgoing_call_pop_up_window_elements__held__Line_1(); 
    }

// -----------------------------------------------------------------------------
// BubbleLayout::popup_call_audio_out_window_1_elements_2
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout::popup_call_audio_out_window_1_elements_2()
    {
    return AppLayout::Outgoing_call_pop_up_window_elements__held__Line_2(); 
    }

// -----------------------------------------------------------------------------
// BubbleLayout::popup_call_audio_out_window_1_elements_15
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout::popup_call_audio_out_window_1_elements_15()
    {
    return AppLayout::Outgoing_call_pop_up_window_elements__held__Line_15();   
    }

// -----------------------------------------------------------------------------
// BubbleLayout::popup_call_audio_out_window_1_texts_1
// -----------------------------------------------------------------------------
//

const TAknTextLineLayout 
    BubbleLayout::popup_call_audio_out_window_1_texts_1( TInt aLine, 
                                                         TInt aType )
    {
    if ( aLine < 1 || aLine > 2 )
        {
        Panic( EBMPanicLAF );
        }
    TInt command1 = aLine - 1;
    TInt command2 = aType;
    return AppLayout::Outgoing_call_pop_up_window_texts__held__Line_1( command1, command2 ); 
    }

// -----------------------------------------------------------------------------
// BubbleLayout::popup_call_audio_out_window_1_graphics_1
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout::popup_call_audio_out_window_1_graphics_1()
    {
    return AppLayout::Outgoing_call_pop_up_window_graphics__held__Line_1();               
    }

// -----------------------------------------------------------------------------
// BubbleLayout::popup_call_audio_out_window_2_elements_1
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout::popup_call_audio_out_window_2_elements_1()
    {
    return AppLayout::Outgoing_call_pop_up_window_elements__held__Line_1();     
    }

// -----------------------------------------------------------------------------
// BubbleLayout::popup_call_audio_out_window_2_elements_2
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout::popup_call_audio_out_window_2_elements_2()
    {
    return AppLayout::Outgoing_call_pop_up_window_elements__held__Line_2();
    }

// -----------------------------------------------------------------------------
// BubbleLayout::popup_call_audio_out_window_2_elements_3
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout::popup_call_audio_out_window_2_elements_3()
    {
    return AppLayout::Outgoing_call_pop_up_window_elements__held__Line_3();
    }

// -----------------------------------------------------------------------------
// BubbleLayout::popup_call_audio_out_window_2_elements_15
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout::popup_call_audio_out_window_2_elements_15()
    {
    return AppLayout::Outgoing_call_pop_up_window_elements__held__Line_15();
    }

// -----------------------------------------------------------------------------
// BubbleLayout::popup_call_audio_out_window_2_texts_1
// -----------------------------------------------------------------------------
//

const TAknTextLineLayout  
    BubbleLayout::popup_call_audio_out_window_2_texts_1()
    {
    return AppLayout::Outgoing_call_pop_up_window_texts__held_NE__Line_1(); 
    }

// -----------------------------------------------------------------------------
// BubbleLayout::popup_call_audio_out_window_2_graphics_1
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout::popup_call_audio_out_window_2_graphics_1()
    {
    return AppLayout::Outgoing_call_pop_up_window_graphics__held_NE__Line_1(); 
    }

// -----------------------------------------------------------------------------
// BubbleLayout::popup_call_audio_first_window_1_elements_1
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout::popup_call_audio_first_window_1_elements_1()
    {
    return AppLayout::First_call_pop_up_window_elements__one_call__Line_1();
    }

// -----------------------------------------------------------------------------
// BubbleLayout::popup_call_audio_first_window_1_elements_2
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout::popup_call_audio_first_window_1_elements_2()
    {
    return AppLayout::First_call_pop_up_window_elements__one_call__Line_2(); 
    }

// -----------------------------------------------------------------------------
// BubbleLayout::popup_call_audio_first_window_1_elements_3
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout::popup_call_audio_first_window_1_elements_3()
    {
    return AppLayout::First_call_pop_up_window_elements__one_call__Line_3(); 
    }

// -----------------------------------------------------------------------------
// BubbleLayout::popup_call_audio_first_window_1_texts_1
// -----------------------------------------------------------------------------
//

const TAknTextLineLayout 
    BubbleLayout::popup_call_audio_first_window_1_texts_1( TInt aLine, 
                                                           TInt aType )
    {
    if ( aLine < 1 || aLine > 3 )
        {
        Panic( EBMPanicLAF );
        }
    TInt command1 = aLine - 1;
    TInt command2 = aType;
    return AppLayout::First_call_pop_up_window_texts__one_call__Line_1( command1, command2 ); 
    }

// -----------------------------------------------------------------------------
// BubbleLayout::popup_call_audio_first_window_1_texts_2
// -----------------------------------------------------------------------------
//

const TAknTextLineLayout 
    BubbleLayout::popup_call_audio_first_window_1_texts_2( TInt aLine, 
                                                           TInt aImageType )
    {
    if ( aLine < 1 || aLine > 3 )
        {
        Panic( EBMPanicLAF );
        }
    TInt command1 = ( aImageType > 0 ) ? 1 : 0;
    return AppLayout::First_call_pop_up_window_texts__one_call__Line_2( command1 ); 
    }

// -----------------------------------------------------------------------------
// BubbleLayout::popup_call_audio_first_window_1_texts_3
// -----------------------------------------------------------------------------
//

const TAknTextLineLayout 
    BubbleLayout::popup_call_audio_first_window_1_texts_3()
    {
    return AppLayout::First_call_pop_up_window_texts__one_call__Line_3(); 
    }


// -----------------------------------------------------------------------------
// BubbleLayout::popup_call_audio_first_window_1_graphics_1
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout::popup_call_audio_first_window_1_graphics_1()
    {
    return AppLayout::First_call_pop_up_window_graphics__one_call__Line_1(); 
    }

// -----------------------------------------------------------------------------
// BubbleLayout::popup_call_audio_first_window_2_elements_1
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout::popup_call_audio_first_window_2_elements_1()
    {
    return AppLayout::First_call_pop_up_window_elements__two_calls__Line_1(); 
    }

// -----------------------------------------------------------------------------
// BubbleLayout::popup_call_audio_first_window_2_elements_2
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout::popup_call_audio_first_window_2_elements_2()
    {
    return AppLayout::First_call_pop_up_window_elements__two_calls__Line_2();
    }

// -----------------------------------------------------------------------------
// BubbleLayout::popup_call_audio_first_window_2_elements_15
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout::popup_call_audio_first_window_2_elements_15()
    {
    return AppLayout::First_call_pop_up_window_elements__two_calls__Line_15(); 
    }

// -----------------------------------------------------------------------------
// BubbleLayout::popup_call_audio_first_window_2_texts_1
// -----------------------------------------------------------------------------
//

const TAknTextLineLayout 
    BubbleLayout::popup_call_audio_first_window_2_texts_1( TInt aLine , 
                                                           TInt aType )
    {
    if ( aLine < 1 || aLine > 2 )
        {
        Panic( EBMPanicLAF );
        }
    TInt command1 = aLine - 1;
    TInt command2 = aType;
    return AppLayout::First_call_pop_up_window_texts__two_calls__Line_1( command1, command2 ); 
    }

// -----------------------------------------------------------------------------
// BubbleLayout::popup_call_audio_first_window_2_texts_2
// -----------------------------------------------------------------------------
//
const TAknTextLineLayout 
    BubbleLayout::popup_call_audio_first_window_2_texts_2( TInt aLine , 
                                                           TInt aImageType )
    {
    if ( aLine < 1 || aLine > 2 )
        {
        Panic( EBMPanicLAF );
        }
    TInt command1 = ( aImageType > 0 ) ? 1 : 0;
    return AppLayout::First_call_pop_up_window_texts__two_calls__Line_2( command1 ); 
    }

// -----------------------------------------------------------------------------
// BubbleLayout::popup_call_audio_first_window_2_texts_3
// -----------------------------------------------------------------------------
//
const TAknTextLineLayout 
    BubbleLayout::popup_call_audio_first_window_2_texts_3()
    {
    return AppLayout::First_call_pop_up_window_texts__two_calls__Line_3(); 
    }

// -----------------------------------------------------------------------------
// BubbleLayout::popup_call_audio_first_window_2_graphics_1
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout::popup_call_audio_first_window_2_graphics_1()
    {
    return AppLayout::First_call_pop_up_window_graphics__two_calls__Line_1(); 
    }

// -----------------------------------------------------------------------------
// BubbleLayout::popup_call_audio_first_window_4_elements_1
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout::popup_call_audio_first_window_4_elements_1()
    {
    return AppLayout::First_call_pop_up_window_elements__two_calls_a_waiting_call__Line_1(); 
    }

// -----------------------------------------------------------------------------
// BubbleLayout::popup_call_audio_first_window_4_elements_2
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout::popup_call_audio_first_window_4_elements_2()
    {
    return AppLayout::First_call_pop_up_window_elements__two_calls_a_waiting_call__Line_2();  
    }

// -----------------------------------------------------------------------------
// BubbleLayout::popup_call_audio_first_window_4_elements_3
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout::popup_call_audio_first_window_4_elements_3()
    {
    return AppLayout::First_call_pop_up_window_elements__two_calls_a_waiting_call__Line_3(); 
    }

// -----------------------------------------------------------------------------
// BubbleLayout::popup_call_audio_first_window_4_texts_1
// -----------------------------------------------------------------------------
//

const TAknTextLineLayout 
    BubbleLayout::popup_call_audio_first_window_4_texts_1()
    {
    return AppLayout::First_call_pop_up_window_texts__two_calls_a_waiting_call__Line_1(); 
    }

// -----------------------------------------------------------------------------
// BubbleLayout::popup_call_audio_first_window_4_graphics_1
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout::popup_call_audio_first_window_4_graphics_1()
    {
    return AppLayout::First_call_pop_up_window_graphics__two_calls_a_waiting_call__Line_1(); 
    }

// -----------------------------------------------------------------------------
// BubbleLayout::popup_call_audio_first_window_6_graphics_1
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout::popup_call_audio_first_window_6_graphics_1()
    {
    return AppLayout::First_call_pop_up_window_graphics__two_wait_NE__Line_1();
    }

// -----------------------------------------------------------------------------
// BubbleLayout::popup_call_audio_first_window_7_graphics_1
// Three-line CDMA emergency call.
// -----------------------------------------------------------------------------
//
const AknLayoutUtils::SAknLayoutControl 
    BubbleLayout::popup_call_audio_first_window_7_graphics_1()
    {
    AknLayoutUtils::SAknLayoutControl layout = {ELayoutEmpty, 0, 0, 
        ELayoutEmpty, ELayoutEmpty, 176, 77};
    return layout;
    }

// -----------------------------------------------------------------------------
// BubbleLayout::popup_call_audio_wait_window_3_elements_1
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout::popup_call_audio_wait_window_3_elements_1()
    {
    return AppLayout::Waiting_call_pop_up_window_elements__held_out_NE__Line_1(); 
    }

// -----------------------------------------------------------------------------
// BubbleLayout::popup_call_audio_wait_window_3_elements_2
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout::popup_call_audio_wait_window_3_elements_2()
    {
    return AppLayout::Waiting_call_pop_up_window_elements__held_out_NE__Line_2(); 
    }

// -----------------------------------------------------------------------------
// BubbleLayout::popup_call_audio_wait_window_3_elements_3
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout::popup_call_audio_wait_window_3_elements_3()
    {
    return AppLayout::Waiting_call_pop_up_window_elements__held_out_NE__Line_3(); 
    }

// -----------------------------------------------------------------------------
// BubbleLayout::popup_call_audio_wait_window_3_texts_1
// -----------------------------------------------------------------------------
//
const TAknTextLineLayout 
    BubbleLayout::popup_call_audio_wait_window_3_texts_1()
    {
    return AppLayout::Waiting_call_pop_up_window_texts__held_out_NE__Line_1(); 
    }

// -----------------------------------------------------------------------------
// BubbleLayout::popup_call_audio_wait_window_3_graphics_1
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout::popup_call_audio_wait_window_3_graphics_1()
    {
    return AppLayout::First_call_pop_up_window_graphics__two_wait_NE__Line_1(); 
    }

// -----------------------------------------------------------------------------
// BubbleLayout::popup_call_audio_wait_window_4_graphics_1
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout::popup_call_audio_wait_window_4_graphics_1()
    {
    return AppLayout::Waiting_call_pop_up_window_graphics__out__Line_1(); 
    }

// -----------------------------------------------------------------------------
// BubbleLayout::popup_call_audio_second_window_1_elements_1
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout::popup_call_audio_second_window_1_elements_1()
    {
    return AppLayout::Second_call_pop_up_window_elements__two__Line_1(); 
    }

// -----------------------------------------------------------------------------
// BubbleLayout::popup_call_audio_second_window_1_elements_2
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout::popup_call_audio_second_window_1_elements_2()
    {
    return AppLayout::Second_call_pop_up_window_elements__two__Line_2(); 
    }

// -----------------------------------------------------------------------------
// BubbleLayout::popup_call_audio_second_window_1_elements_3
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout::popup_call_audio_second_window_1_elements_3()
    {
    return AppLayout::Second_call_pop_up_window_elements__two__Line_3(); 
    }

// -----------------------------------------------------------------------------
// BubbleLayout::popup_call_audio_second_window_1_texts_1
// -----------------------------------------------------------------------------
//

const TAknTextLineLayout 
    BubbleLayout::popup_call_audio_second_window_1_texts_1( TInt aLine , 
                                                            TInt aType )
    {
    if ( aLine < 1 || aLine > 2 )
        {
        Panic( EBMPanicLAF );
        }
    TInt command1 = aLine - 1;
    TInt command2 = aType;
    return AppLayout::Second_call_pop_up_window_texts__two_calls__Line_1( command1, command2 );
    }

// -----------------------------------------------------------------------------
// BubbleLayout::popup_call_audio_second_window_1_texts_2
// -----------------------------------------------------------------------------
//

const TAknTextLineLayout 
    BubbleLayout::popup_call_audio_second_window_1_texts_2( TInt aImageType )
    {
    TInt command1 = ( aImageType > 0 ) ? 1 : 0;
    return AppLayout::Second_call_pop_up_window_texts__two_calls__Line_2( command1 ); 
    }

// -----------------------------------------------------------------------------
// BubbleLayout::popup_call_audio_second_window_1_graphics_1
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout::popup_call_audio_second_window_1_graphics_1()
    {
    return AppLayout::Second_call_pop_up_window_graphics__two_calls__Line_1(); 
    }

// -----------------------------------------------------------------------------
// BubbleLayout::popup_call_audio_second_window_3_elements_1
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout::popup_call_audio_second_window_3_elements_1()
    {
    return AppLayout::Second_call_pop_up_window_elements__two_calls_waiting_call__Line_1();  
    }

// -----------------------------------------------------------------------------
// BubbleLayout::popup_call_audio_second_window_3_elements_2
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout::popup_call_audio_second_window_3_elements_2()
    {
    return AppLayout::Second_call_pop_up_window_elements__two_calls_waiting_call__Line_2();     
    }

// -----------------------------------------------------------------------------
// BubbleLayout::popup_call_audio_second_window_3_elements_3
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout::popup_call_audio_second_window_3_elements_3()
    {
    return AppLayout::Second_call_pop_up_window_elements__two_calls_waiting_call__Line_3(); 
    }

// -----------------------------------------------------------------------------
// BubbleLayout::popup_call_audio_second_window_3_texts_1
// -----------------------------------------------------------------------------
//
const TAknTextLineLayout 
    BubbleLayout::popup_call_audio_second_window_3_texts_1()
    {
    return AppLayout::Second_call_pop_up_window_texts__two_calls_waiting_call__Line_1(); 
    }

// -----------------------------------------------------------------------------
// BubbleLayout::popup_call_audio_second_window_3_graphics_1
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout::popup_call_audio_second_window_3_graphics_1()
    {
    return AppLayout::Second_call_pop_up_window_graphics__two_calls_waiting_call__Line_1();
    }

// -----------------------------------------------------------------------------
// BubbleLayout::popup_call_audio_conf_window_elements_1
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout::popup_call_audio_conf_window_elements_1( 
    const TRect& aParentRect )
    {
    return AppLayout::Conference_call_pop_up_window_descendants_and_elements_Line_1( aParentRect ); 
    }

// -----------------------------------------------------------------------------
// BubbleLayout::popup_call_audio_conf_window_elements_2
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout::popup_call_audio_conf_window_elements_2( TInt aLine )
    {

    if ( aLine < 1 || aLine > 4 )
        {
        Panic( EBMPanicLAF );
        }
    TInt command1 = aLine - 1;
    return AppLayout::list_conf_pane( command1 ); 
    }

// -----------------------------------------------------------------------------
// BubbleLayout::ConferencePaneHeight
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout::ConferencePane()
    {    
    TInt index_t = 0;
    return AppLayout::list_single_graphic_popup_conf_pane( index_t );
    }

// -----------------------------------------------------------------------------
// BubbleLayout::popup_call_audio_cont_window_texts_1
// -----------------------------------------------------------------------------
//

const TAknTextLineLayout 
    BubbleLayout::popup_call_audio_cont_window_texts_1( TInt aLine )
    {
    if ( aLine < 1 || aLine > 4 )
        {
        Panic( EBMPanicLAF );
        }
    TInt command1 = aLine - 1;
    return AppLayout::Conference_call_pop_up_window_texts_Line_1( command1 ); 
    }

// -----------------------------------------------------------------------------
// BubbleLayout::main_pane_elements_muted
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout BubbleLayout::main_pane_elements_muted( 
    TBool aNumberEditorActive )
    {
    if ( AknLayoutUtils::ScalableLayoutInterfaceAvailable() )
	    {
	    if ( aNumberEditorActive )
    	    {
    	    return AknLayoutScalable_Apps::call_muted_g2().LayoutLine();   
    	    }
	    else
	        {
	        return AknLayoutScalable_Apps::call_muted_g(0).LayoutLine();    
	        }
	    }
    else
        {
        return AppLayout::Muted_state_elements_Line_1();       
        }	    
    }

// -----------------------------------------------------------------------------
// BubbleLayout::call_type_pane_elements_1
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout::call_type_pane_elements_1()
    {
    return AppLayout::Call_type_pane_split_Line_1(); 
    }

// -----------------------------------------------------------------------------
// BubbleLayout::call_type_pane_elements_2
// -----------------------------------------------------------------------------
//
const TAknWindowLineLayout 
    BubbleLayout::call_type_pane_elements_2()
    {
    return AppLayout::Call_type_pane_split_Line_2();
    }

// End of File
