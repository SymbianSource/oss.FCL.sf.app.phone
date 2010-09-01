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



// INCLUDE FILES
#include    "UssdLayout.h"
#include    <aknenv.h>
#include    <bldvariant.hrh>
#include    <featmgr.h>
#include    <applayout.cdl.h>
#include <aknlayoutscalable_apps.cdl.h>


// CONSTANTS

#ifdef _DEBUG
// Panic types to be used in this file.
enum TUssdPanicTypes
    {
    EUssdPanicNOEL = 0,
    EUssdPanicPBL,
    EUssdPanicMWLE6,
    EUssdPanicMWT3,
    //
    EPhCltPanicLast
    };

#endif

// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// UssdLayout::CurrentLayout
// -----------------------------------------------------------------------------
//
TAknLayoutId UssdLayout::CurrentLayout()
    {
    TAknLayoutId layoutId;
    CAknEnv::Static()->GetCurrentLayoutId( layoutId );
    return layoutId;
    }

// -----------------------------------------------------------------------------
// UssdLayout::LayoutColourWhite
// -----------------------------------------------------------------------------
//
TInt UssdLayout::LayoutColourWhite()
    {
    return 0;  // White
    }

// -----------------------------------------------------------------------------
// UssdLayout::NumberOfEditorLines
// -----------------------------------------------------------------------------
//
TInt UssdLayout::NumberOfEditorLines()
    {
    TInt retVal = KErrNotFound;
	TAknLayoutScalableParameterLimits scalbleParamLimits;
	scalbleParamLimits = AknLayoutScalable_Apps::list_single_mce_message_pane_ParamLimits();    
    
    retVal = scalbleParamLimits.LastRow();
    return retVal;
    }

// -----------------------------------------------------------------------------
// UssdLayout::MessageWritingLayoutElements6
// -----------------------------------------------------------------------------
//
UssdLayout::TWindowLineLayoutType
    UssdLayout::MessageWritingLayoutElements6( TInt aLine )
    {
    __ASSERT_ALWAYS( 
        aLine >= 1 && aLine <= NumberOfEditorLines(), 
        User::Invariant() );

    TInt index_C = 0;
    TInt index_T = aLine - 1;
    return 
        AppLayout::Message_writing_layout_elements_Line_6( index_C, index_T );
    }

// -----------------------------------------------------------------------------
// UssdLayout::MessageWritingTexts3
// -----------------------------------------------------------------------------
//
UssdLayout::TTextLineLayoutType UssdLayout::MessageWritingTexts3()
    {
    TInt numberOfLines = NumberOfEditorLines();
    return
        AppLayout::Multiline_Message_writing_texts_Line_3( numberOfLines ); 
    }


//  End of File  
