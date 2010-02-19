/*
* Copyright (c) 2007 - 2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Observer for dialingextension plug-in.
*
*/

// INCLUDE FILES
#include <eikenv.h>
#include <eikappui.h>
#include <phoneappcommands.hrh>

// USER INCLUDE FILES
#include "cdialingextensionobserver.h"
#include "cdialernumberentry.h"
#include "cdialer.h"


// ---------------------------------------------------------------------------
//  CDialingExtensionObserver::CDialingExtensionObserver
//  
// 
// ---------------------------------------------------------------------------
//    
CDialingExtensionObserver* CDialingExtensionObserver::NewL( 
        CDialingExtensionInterface* aDialingExtension, 
        CDialerNumberEntry* aNumberEntry,
        CDialer* aDialer )
    {
    CDialingExtensionObserver* self = new (ELeave) CDialingExtensionObserver( 
            aDialingExtension, aNumberEntry, aDialer );
    return self;
    }

// ---------------------------------------------------------------------------
//  CDialingExtensionObserver::~CDialingExtensionObserver
//  
// 
// ---------------------------------------------------------------------------
//
CDialingExtensionObserver::~CDialingExtensionObserver()
    {
    // no owned data => no implementation needed
    }

// ---------------------------------------------------------------------------
//  CDialingExtensionObserver::HandleDialingExtensionEvent
//  
// 
// ---------------------------------------------------------------------------
//    
void CDialingExtensionObserver::HandleDialingExtensionEvent( MDialingExtensionObserver::TEvent aEvent )
    {
    switch ( aEvent )
        {
        case MDialingExtensionObserver::EFocusChanged:
            {
            // The basic idea of EFocusChanged event handling:
            // - If dialing extension gets in focus, focus is taken from number entry editor.
            // - If dialing extension gives up focus, focus is given to number entry editor.
            
            if ( iDialingExtension->IsFocused() && iNumberEntry->IsFocused() )
                {
                iNumberEntry->SetFocus(EFalse);
                UpdateCba();
                iDialer->UpdateToolbar();
                }
            else if ( !iDialingExtension->IsFocused() && !iNumberEntry->IsFocused() && iDialer->IsFocused() )
                {
                // Number entry editor is set focused only if the parent control is in focus.
                // If number entry editor was focused but parent was not, this may show as a
                // a blinking cursor in an inappropriate place.
                iNumberEntry->SetFocus(ETrue);
                UpdateCba();
                iDialer->UpdateToolbar();
                }
            
            iNumberEntry->DrawDeferred();
            }
            break;

        case MDialingExtensionObserver::ECCALauncherExit:
        case MDialingExtensionObserver::ECommunicationCancelled:
        case MDialingExtensionObserver::ECommunicationStarted:
            {
            // No actions. We should stay in dialer, and not go to idle.
            }
            break;
        
        case MDialingExtensionObserver::EEasyDialingEnabled:
            {
            // order dialer to refresh its layout
            iDialer->SetSize( iDialer->Size() );
            if (iDialingExtension && iNumberEntry)
                {
                // get matches for the current input
                TRAP_IGNORE( SearchL() );
                }
            iDialer->DrawDeferred();
            }
            break;

        case MDialingExtensionObserver::EEasyDialingDisabled:
            {
            // order parent to refresh its layout
            iDialer->SetSize( iDialer->Size() );
            iDialer->DrawDeferred();
            }
            break;
            
        default:
            break;
        }    
    }


// ---------------------------------------------------------------------------
//  CDialingExtensionObserver::CDialingExtensionObserver
//  
// 
// ---------------------------------------------------------------------------
//    
CDialingExtensionObserver::CDialingExtensionObserver( 
        CDialingExtensionInterface* aDialingExtension, 
        CDialerNumberEntry* aNumberEntry,
        CDialer* aDialer ) :
iDialingExtension( aDialingExtension ), 
iNumberEntry( aNumberEntry ), 
iDialer( aDialer ),
iEikonEnvironment( NULL )
    {
    }
        
        
// ---------------------------------------------------------------------------
//  CDialingExtensionObserver::CDialingExtensionObserver
//  
// 
// ---------------------------------------------------------------------------
//    
void CDialingExtensionObserver::UpdateCba()
    {
    if ( !iEikonEnvironment )
        {
        iEikonEnvironment = CEikonEnv::Static();
        }
    
    if( iEikonEnvironment && iEikonEnvironment->EikAppUi() )
        {
        TRAP_IGNORE( iEikonEnvironment->EikAppUi()->HandleCommandL( EPhoneCmdUpdateCba ));
        }
    }


// ---------------------------------------------------------------------------
//  CDialingExtensionObserver::SearchL
//  
// 
// ---------------------------------------------------------------------------
//    
void CDialingExtensionObserver::SearchL()
    {
    TPtrC searchString = iNumberEntry->Text();
    if ( searchString.Length() )
        {
        iDialingExtension->SetInputL( searchString );
        }
    }

// end of file
