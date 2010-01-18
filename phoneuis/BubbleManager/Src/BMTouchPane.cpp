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
* Description:  Container for touch controls.
*
*/


#include <layoutmetadata.cdl.h>
#include <barsread.h>

#include "BMTouchPane.h"
#include "BMPanic.h"
#include "BMLayout4.h"
#include "BMLayout6.h"
#include "BMTouchPaneButton.h"

// CONSTANTS


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// C++ default constructor
// ---------------------------------------------------------------------------
//
CBubbleTouchPane::CBubbleTouchPane()
    {
    }

// ---------------------------------------------------------------------------
// ConstructL
// ---------------------------------------------------------------------------
//
void CBubbleTouchPane::ConstructL()
    {
    }

// ---------------------------------------------------------------------------
// NewL
// ---------------------------------------------------------------------------
//
CBubbleTouchPane* CBubbleTouchPane::NewL()
    {
    CBubbleTouchPane* self = new(ELeave) CBubbleTouchPane;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CBubbleTouchPane::~CBubbleTouchPane()
    {
    iButtons.ResetAndDestroy();
    iButtons.Close();
    
    iButtonsToBeDeleted.ResetAndDestroy();
    iButtonsToBeDeleted.Close();
    delete iIdleProcessor; 
    }

// ---------------------------------------------------------------------------
// SizeChanged
// ---------------------------------------------------------------------------
//
void CBubbleTouchPane::SizeChanged( )
    {
    if ( !iButtons.Count() )
        return;
    
    // button index
    //  1 2  OR  1 2 3 4
    //  3 4
    TInt i = 0;
    
    TInt rows = 0;
    TInt columns = 0;
    BubbleLayout6::button_grp_row_column_count(0, rows, columns);
    for ( TInt row = 0; row < rows; row++ )
        {
        for ( TInt col = 0; col < columns; col++ )
            {
            TAknLayoutRect buttonPane;        
            buttonPane.LayoutRect( 
                Rect(), 
                BubbleLayout6::cell_call6_button_pane(0, col, row) );
            
            if ( i < iButtons.Count() && iButtons[i] )
                {
                // Button control layout according to background
                AknLayoutUtils::LayoutControl( 
                    iButtons[i], 
                    buttonPane.Rect(), 
                    BubbleLayout6::button_call6_background_graphics(0)); 
                
                TAknLayoutText text;
                text.LayoutText( buttonPane.Rect(), 
                        BubbleLayout6::button_call6_function_text(0));

                TAknLayoutRect icon;
                icon.LayoutRect( buttonPane.Rect(), 
                        BubbleLayout6::button_call6_function_graphics(0));   
                
                iButtons[i]->SetIconSize( icon.Rect().Size() );
                iButtons[i++]->SetLayout( text, icon.Rect() );
                }
            }            
        }
    }

// ---------------------------------------------------------------------------
// CountComponentControls
// ---------------------------------------------------------------------------
//
TInt CBubbleTouchPane::CountComponentControls( ) const
    {
    return iButtons.Count();            
    }

// ---------------------------------------------------------------------------
// ComponentControl
// ---------------------------------------------------------------------------
//
CCoeControl* CBubbleTouchPane::ComponentControl( TInt aIndex ) const
    {
    return iButtons[ aIndex ];
    }

// ---------------------------------------------------------------------------
// HandlePointerEventL
// ---------------------------------------------------------------------------
//
void CBubbleTouchPane::HandlePointerEventL
    ( const TPointerEvent& aPointerEvent )           
    {
    CCoeControl::HandlePointerEventL( aPointerEvent );
    }

// ---------------------------------------------------------------------------
// HandleControlEventL
// ---------------------------------------------------------------------------
//    
void CBubbleTouchPane::HandleControlEventL(
    CCoeControl* aControl, 
    TCoeEvent aEventType)
    {
    if ( aEventType != EEventStateChanged )
        {
        return;
        }
    
    CBubbleTouchPaneButton* tappedButton = NULL;
    for ( TInt i=0; i < iButtons.Count(); i++ )
        {
        if ( iButtons[i] == aControl )
            {
            tappedButton = iButtons[i];
            break;
            }    
        }
    if ( tappedButton )
        {
        iDeleteButtonsAsync = ETrue;
        CEikAppUi* appUi = CEikonEnv::Static()->EikAppUi();
        appUi->HandleCommandL( tappedButton->CommandId( ETrue ) );
        iDeleteButtonsAsync = EFalse;
        }    
    }

// ---------------------------------------------------------------------------
// HandleResourceChange
// ---------------------------------------------------------------------------
//    
void CBubbleTouchPane::HandleResourceChange( TInt aType )
    {
    for ( TInt i=0; i < iButtons.Count(); i++ )
        {
        iButtons[i]->HandleResourceChange( aType );
        }    
    }

// ---------------------------------------------------------------------------
// SetUpdating
// ---------------------------------------------------------------------------
//    
void CBubbleTouchPane::SetUpdating( TBool aUpdating )
    {
    iUpdating = aUpdating;    
    }

// ---------------------------------------------------------------------------
// IdleCallback
// ---------------------------------------------------------------------------
//     
TInt CBubbleTouchPane::IdleCallback( TAny* aThis )
    {
    CBubbleTouchPane* self = 
            static_cast<CBubbleTouchPane*>( aThis );
    self->DoDeleteButtonsInIdle();
    return KErrNone;                
    }

// ---------------------------------------------------------------------------
// DoDeleteButtonsInIdle
// ---------------------------------------------------------------------------
//
void CBubbleTouchPane::DoDeleteButtonsInIdle()
    {
    iButtonsToBeDeleted.ResetAndDestroy();
    }

// ---------------------------------------------------------------------------
// SetButtonSet
// ---------------------------------------------------------------------------
//
TInt CBubbleTouchPane::SetButtonSet( TInt aResourceId )
    {
    TRAPD( err, SetButtonSetL( aResourceId ) );
    
    return err;    
    }

// ---------------------------------------------------------------------------
// SetButtonSetL
// ---------------------------------------------------------------------------
//
void CBubbleTouchPane::SetButtonSetL( TInt aResourceId )
    {
    // reset touch pane
    if ( iDeleteButtonsAsync )
        {
        // swap buttons to another array
        for ( TInt i=0; i < iButtons.Count(); i++  )
            {
            iButtonsToBeDeleted.AppendL( iButtons[i] );    
            }
            
        iButtons.Reset();            
        }
    else
        {
        iButtons.ResetAndDestroy();    
        }        
        
    if ( iButtonsToBeDeleted.Count() )        
        {
        if ( !iIdleProcessor )
            {
            iIdleProcessor = CIdle::NewL( CActive::EPriorityIdle );                
            }
        
        if ( !iIdleProcessor->IsActive() )
            {
            TCallBack idleCallback( IdleCallback, this );
            iIdleProcessor->Start( idleCallback );            
            }            
        }
    
    iCurrentButtonSetId = 0;
    
    // load new buttons
    if ( aResourceId )    
        {
        TResourceReader reader;
        iCoeEnv->CreateResourceReaderLC( reader, aResourceId );
        LoadButtonsFromResourceL( reader );
        CleanupStack::PopAndDestroy();
        iCurrentButtonSetId = aResourceId;
        }
    }
    
// ---------------------------------------------------------------------------
// LoadButtonsFromResourceL
// ---------------------------------------------------------------------------
//
void CBubbleTouchPane::LoadButtonsFromResourceL( TResourceReader& aReader )
    {
    __ASSERT_DEBUG( (iButtons.Count() == 0), Panic( EBMPanicTouchPane ) );
    
    TInt flags = aReader.ReadInt16();
    
    TInt buttonCount = aReader.ReadInt16();
    for ( TInt i=0; i < buttonCount; i++ )    
        {
        TInt buttonId( aReader.ReadInt32() );
        TResourceReader buttonReader;
        iCoeEnv->CreateResourceReaderLC( buttonReader, buttonId );
        
        CBubbleTouchPaneButton* button = CBubbleTouchPaneButton::NewL( 
                iIconProvider, flags );
        CleanupStack::PushL( button );
        button->ConstructFromResouceL( buttonReader ); 
        
        CleanupStack::Pop( button );
        CleanupStack::PopAndDestroy(); // buttonReader
        
        iButtons.Append( button );
        }
        
    for ( TInt i=0; i < iButtons.Count(); i++ )
        {
        CBubbleTouchPaneButton* button = iButtons[i];
        button->SetContainerWindowL( *this );
        button->SetObserver( this );    
        button->ActivateL();
        }        
    
    SizeChanged();
    if ( IsVisible() )        
        {
        if ( iUpdating )
            {
            DrawDeferred();                
            }
        else
            {
            DrawNow();            
            }
        }
    }

// ---------------------------------------------------------------------------
// SetButtonState
// ---------------------------------------------------------------------------
//    
void CBubbleTouchPane::SetButtonState( TInt aCommand )
    {
    for ( TInt i=0; i < iButtons.Count(); i++ )
        {
        TBool drawNow = ( IsVisible() && !iUpdating ) ? ETrue : EFalse;
        iButtons[i]->SetState( aCommand, drawNow );    
        }        
    }

// ---------------------------------------------------------------------------
// SetButtonDimmed
// ---------------------------------------------------------------------------
//
void CBubbleTouchPane::SetButtonDimmed( TInt aCommand, TBool aDimmed )
    {
    TInt buttonCount = iButtons.Count();
    
    for ( TInt i=0; i < buttonCount; i++ )
        {
        if ( iButtons[i]->CommandId() == aCommand )
            {
            iButtons[i]->SetDimmed( aDimmed );                
            }
        }    
    }

// ---------------------------------------------------------------------------
// NumberOfButtonsInPane
// ---------------------------------------------------------------------------
//    
TInt CBubbleTouchPane::NumberOfButtonsInPane()  const
    {
    return CountComponentControls();
    }

// ---------------------------------------------------------------------------
// ButtonCommandId
// ---------------------------------------------------------------------------
//
TInt CBubbleTouchPane::ButtonCommandId( TInt aButtonIndex ) const
    {
    return iButtons[ aButtonIndex ]->CommandId();    
    }

// ---------------------------------------------------------------------------
// ButtonHasState
// ---------------------------------------------------------------------------
//    
TBool CBubbleTouchPane::ButtonHasState( TInt aButtonIndex, TInt aCommand ) const
    {
    return iButtons[ aButtonIndex ]->HasState( aCommand );   
    }

// ---------------------------------------------------------------------------
// ReplaceButton
// ---------------------------------------------------------------------------
//    
TInt CBubbleTouchPane::ReplaceButton( TInt aButtonIndex, TInt aResourceId )
    {
    TInt err;
    
    if ( 0 <= aButtonIndex && aButtonIndex < iButtons.Count() &&
         aResourceId )
        {
        TRAP( err, ReplaceButtonL( aButtonIndex, aResourceId ) );
        }
    else
        {
        err = KErrGeneral;    
        }        
            
    return err;    
    }

// ---------------------------------------------------------------------------
// ReplaceButton
// ---------------------------------------------------------------------------
//    
void CBubbleTouchPane::ReplaceButtonL( TInt aButtonIndex, TInt aResourceId )
    {
    TResourceReader reader;
    iCoeEnv->CreateResourceReaderLC( reader, aResourceId );
       
    CBubbleTouchPaneButton* button = CBubbleTouchPaneButton::NewL( 
                                     iIconProvider);
    CleanupStack::PushL( button );
    button->ConstructFromResouceL( reader ); 
        
    CleanupStack::Pop( button );
    CleanupStack::PopAndDestroy(); // reader
        
    button->SetContainerWindowL( *this );
    button->SetObserver( this );    
    button->ActivateL();
        
    if ( !iDeleteButtonsAsync )
        {
        delete iButtons[ aButtonIndex ];
        iButtons[ aButtonIndex ] = NULL;        
        }
    else
        {
        iButtonsToBeDeleted.Append( iButtons[ aButtonIndex ] );
        iButtons[ aButtonIndex ] = NULL;    
        if ( !iIdleProcessor )
            {
            iIdleProcessor = CIdle::NewL( CActive::EPriorityIdle );                
            }
            
        if ( !iIdleProcessor->IsActive() )
            {
            TCallBack idleCallback( IdleCallback, this );
            iIdleProcessor->Start( idleCallback );            
            }
        }            
        
    iButtons[ aButtonIndex ] = button;
        
    SizeChanged();
    if ( IsVisible() )        
        {
        DrawDeferred();            
        }
    }

// ---------------------------------------------------------------------------
// SetIconProvider
// ---------------------------------------------------------------------------
//
void CBubbleTouchPane::SetIconProvider(
    MBubbleTouchPaneIconProvider* aIconProvider )
    {
    iIconProvider = aIconProvider;    
    }
