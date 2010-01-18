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
#ifndef BMLAYOUT6_H_
#define BMLAYOUT6_H_


#include <calslbs.h>
#include <bldvariant.hrh> 
#include <aknlayoutdef.h>
/**
* Layout coordinates from Application LAF
* 
* @lib      bubblemanager.lib
* @since    S60 9.2
*/
class BubbleLayout6
    {
    public:
        
        
        static const TAknWindowLineLayout call6_button_grp_pane( TInt aVariety );
        
        static void button_grp_row_column_count( TInt  aVariety, TInt& aRows, TInt& aColumns );
        
        static const TAknWindowLineLayout cell_call6_button_pane( TInt aVariety, TInt aCol, TInt aRow );
        
        static const TAknTextLineLayout button_call6_function_text( TInt aVariety );
        
        static const TAknWindowLineLayout button_call6_function_graphics( TInt aVariety );
        
        static const TAknWindowLineLayout call6_windows_pane( TInt aVariety );
        
        static const TAknWindowLineLayout button_call6_background_graphics( TInt aVariety );
        
        static const TAknWindowLineLayout popup_call6_audio_first_window( TInt aVariety );
        
    };

#endif /* BMLAYOUT6_H_ */
