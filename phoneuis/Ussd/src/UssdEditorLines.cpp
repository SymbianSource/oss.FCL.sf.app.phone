/*
* Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Declares window owning control for editor lines.
*
*/


// INCLUDE FILES
#include    <AknsDrawUtils.h>
#include    <eikenv.h>

#include    "UssdEditorLines.h"
#include    "UssdLayout.h"

// CONSTANTS

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CUssdEditorLines::CUssdEditorLines
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CUssdEditorLines::CUssdEditorLines()
    {
    }


// -----------------------------------------------------------------------------
// CUssdEditorLines::ConstructL
// Symbian 2nd phase constructor can leave.
// 
// -----------------------------------------------------------------------------
//
void CUssdEditorLines::ConstructL( const CCoeControl* aParent )
    {
    // Create an own window
    CreateWindowL( aParent );
    
    // This window draws editor row lines, so enable window 
    // transparency to display the editor window content as background 
    // for this line-window.
    EnableWindowTransparency();
      
    // Create region buffer.
    iRegBuf = new ( ELeave ) RRegionBuf< KUssdMaxNumberOfEditorLines >;

    // Create rects

    iRects = new ( ELeave ) CArrayPtrFlat<TAknLayoutRect>( KUssdMaxNumberOfEditorLines );
    iRects->SetReserveL( KUssdMaxNumberOfEditorLines );

    TAknLayoutRect* rect = NULL;
    for ( TInt i = 0 ; i < KUssdMaxNumberOfEditorLines ; i++ )
        {
        rect = new ( ELeave ) TAknLayoutRect;
        iRects->InsertL( i , rect ); // Can't leave
        }

    SetRect( aParent->Rect() );
    SetFocus( ETrue );

    // activate control
    ActivateL();
    }


// Destructor
CUssdEditorLines::~CUssdEditorLines()
    {
    if ( iRects )
        {
        iRects->ResetAndDestroy() ;
        delete iRects;
        }

    if ( iRegBuf )
        {
        iRegBuf->Close() ;
        delete iRegBuf;
        }
    }


// -----------------------------------------------------------------------------
// CUssdEditorLines::SizeChanged
// Called by framework when the view size is changed
//  
// -----------------------------------------------------------------------------
//
void CUssdEditorLines::SizeChanged()
    {
    // Move rectangles to right position
    
    for ( TInt i = 0;
            i < iRects->Count() && i < UssdLayout::NumberOfEditorLines(); 
            i++ )
        {
        iRects->At( i )->LayoutRect( Rect() , 
            UssdLayout::MessageWritingLayoutElements6( i+1 ) );
        }
    }


// -----------------------------------------------------------------------------
// CUssdEditorLines::Draw
// 
// -----------------------------------------------------------------------------
//
void CUssdEditorLines::Draw( const TRect& /*aRect*/ ) const
    {
    // Take the colour from some ramdom line.
    // Here it is taken from the first line.

    TRgb lineColour( KRgbWhite );

    if ( iRects->Count() )
        {
        lineColour = iRects->At( 0 )->Color();
        }

    // Get skin colour if set.
    MAknsSkinInstance* skin = AknsUtils::SkinInstance();
    TRgb skinColor;
    TInt error = 
        AknsUtils::GetCachedColor( 
            skin, 
            skinColor, 
            KAknsIIDQsnLineColors,
            EAknsCIQsnLineColorsCG6 );

    if ( error == KErrNone )
        {
        lineColour = skinColor;
        }
    CWindowGc& gc = SystemGc();
    gc.SetBrushColor( lineColour );
    
    // Draw row lines by clearing rectangles corresponding to row locations.
    TInt count( iRects->Count() );
    TInt lines( UssdLayout::NumberOfEditorLines() );
    for ( TInt i = 0; i < count && i < lines; i++ )
        {
        gc.Clear( iRects->At( i )->Rect() );
        }
   }

// -----------------------------------------------------------------------------
// CUssdEditorLines::HandlePointerEventL
//
// -----------------------------------------------------------------------------
//
void CUssdEditorLines::HandlePointerEventL(const TPointerEvent& aPointerEvent)
    {
    // Temporary variant for text editor.
    CCoeControl *editor( Parent() ? Parent()->ComponentControl( 0 ) : NULL );
    // Redirect pointer event to CEikEdwin, so as that CEikEdwin can popup
    // virtual keyboard when user gives a click input.
    if ( editor )
        {
        editor->HandlePointerEventL( aPointerEvent );
        }
    }
// End of File  
