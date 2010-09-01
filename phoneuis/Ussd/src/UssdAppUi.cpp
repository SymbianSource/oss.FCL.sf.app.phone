/*
* Copyright (c) 2002-2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Application UI class
*
*/

// INCLUDE FILES
#include <ussd.rsg>
#include <avkon.rsg>
#include <eikmenub.h>
#include <eikedwin.h>
#include <avkon.hrh>
#include <bldvariant.hrh>
#include <featmgr.h>
#include <AknDef.h>
#include <hlplch.h>   // For HlpLauncher

#include "UssdAppUi.h"
#include "UssdContainer.h"
#include "UssdComms.h"
#include "ussd.hrh"
#include "UssdLogger.h"
// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CUssdAppUi::ConstructL
// Creates all members
// -----------------------------------------------------------------------------
void CUssdAppUi::ConstructL()
    {
    _LOGSTRING( "CUssdAppUi::ConstructL =>" )
    // Sets up TLS, must be done before FeatureManager is used in USSD.
    FeatureManager::InitializeLibL();

    BaseConstructL(
        EAknEnableSkin | EAknEnableMSK | EAknSingleClickCompatible
        );

    // Softkeys at start:
    // With HELP: Options-Exit (defined in the resource file, default)
    // Without HELP: <empty>-Exit
    if ( !FeatureManager::FeatureSupported( KFeatureIdHelp ) )
        {
        // Feature not supported, set correct softkeys.
        Cba()->SetCommandSetL( R_AVKON_SOFTKEYS_EXIT );
        iCbaResource = R_AVKON_SOFTKEYS_EXIT;
        Cba()->DrawNow();
        }

    iComms = CUssdComms::NewL();

    iAppContainer = new( ELeave ) CUssdContainer( *this );
    iAppContainer->SetMopParent( this );
    iAppContainer->ConstructL( ClientRect() );
    AddToStackL( iAppContainer );
    iIsAddedToStack = ETrue;
    _LOGSTRING( "CUssdAppUi::ConstructL <=" )
    }

// -----------------------------------------------------------------------------
// CUssdAppUi::~CUssdAppUi
// Destructor
// Frees reserved resources
// -----------------------------------------------------------------------------
CUssdAppUi::~CUssdAppUi()
    {
    _LOGSTRING( "CUssdAppUi::~CUssdAppUi =>" )
    if ( iIsAddedToStack )
        {
        RemoveFromStack( iAppContainer );
        }

    delete iAppContainer;

    delete iComms;

    // Frees the TLS! Must be done after FeatureManager is used.
    FeatureManager::UnInitializeLib();
    _LOGSTRING( "CUssdAppUi::~CUssdAppUi <=" )
    }

// -----------------------------------------------------------------------------
// CUssdAppUi::HandleCommandL
// Handle commands from the user.
// -----------------------------------------------------------------------------
void CUssdAppUi::HandleCommandL( TInt aCommand )
    {
    _LOGSTRING2( "CUssdAppUi::HandleCommandL =>, aCommand=%d", 
    aCommand )
    switch ( aCommand )
        {
        case EAknCmdExit:
        case EEikCmdExit:
        case EAknSoftkeyExit:
        case EAknSoftkeyBack:
            {
            iComms->InformExitReason( EPhCltUserExit );
            Exit();
            break;
            }

        case EUssdMenuItemSend:
        case EUssdSoftkeySend:
            {
            // Send string and exit the application
            HBufC* buffer = iAppContainer->Editor().GetTextInHBufL();
            __ASSERT_DEBUG(
                buffer->Length() <= KUssdEditorMaxLenght ,
                User::Invariant() );

            if  ( buffer->Length() &&
                  buffer->Length() <= KUssdEditorMaxLenght &&
                  KErrNone == iComms->SendString( buffer->Des() ) )
                {
                iComms->InformExitReason( EPhCltSendCompleted );
                Exit();                
                }         

            delete buffer;
            break;
            }

        case EAknCmdHelp:
            {
            if ( FeatureManager::FeatureSupported( KFeatureIdHelp ) )
                {
                 // Feature supported, launch the help application.
                HlpLauncher::LaunchHelpApplicationL(
                    iEikonEnv->WsSession(), AppHelpContextL() );
                }
            break;
            }

        default:
            break;
        }
    _LOGSTRING( "CUssdAppUi::HandleCommandL <=" )
    }

// -----------------------------------------------------------------------------
// CUssdAppUi::HandleResourceChangeL
// -----------------------------------------------------------------------------
void CUssdAppUi::HandleResourceChangeL( TInt aType )
    {
    CAknAppUi::HandleResourceChangeL( aType );

    if( aType == KEikDynamicLayoutVariantSwitch )
        {
        iAppContainer->SetRect( ClientRect() );
        }
    }

// -----------------------------------------------------------------------------
// CUssdAppUi::HandleForegroundEventL
// Handle foreground event.
// -----------------------------------------------------------------------------
void CUssdAppUi::HandleForegroundEventL( TBool aForeground )
    {
    _LOGSTRING2( "CUssdAppUi::HandleForegroundEventL =>, aForeground=%d",
    aForeground )
    if ( iComms )
        {
        if ( aForeground )
            {
            // If app comes to foreground, we must know is the
            // editor needs clearing.
            if ( iComms->InformAppForeground() ) // ret ETrue if needs clearing
                {
                if ( iAppContainer )
                    {
                    iAppContainer->Editor().SetCursorPosL( 0, EFalse );
                    iAppContainer->Editor().Text()->Reset();
                    iAppContainer->Editor().HandleTextChangedL();

                    if ( !FeatureManager::FeatureSupported( KFeatureIdHelp ) )
                        {
                        SetSoftkeySendVisibleL( EFalse );
                        }

                    }
                }
            }
        else
            {
            iComms->InformAppBackground();
            }
        }

    // Refresh message character counter
    if( aForeground && iAppContainer )
        {
        iAppContainer->UpdateNavipaneMsgLengthL();
        }
    
    CAknAppUi::HandleForegroundEventL( aForeground );
    _LOGSTRING( "CUssdAppUi::HandleForegroundEventL <=" )
    }

// -----------------------------------------------------------------------------
// CUssdAppUi::DynInitMenuPaneL
// Set's Send option visible.
//
// If HELP is defined, the Options menu item 'Send' is removed if
// the editor is empty. If it is not defined, the left softkey is 'Send' or
// <empty>.
// -----------------------------------------------------------------------------
void CUssdAppUi::DynInitMenuPaneL(
    TInt aResourceId,
    CEikMenuPane* aMenuPane )
    {
    if ( FeatureManager::FeatureSupported( KFeatureIdHelp ) )
        {
        if ( aResourceId == R_USSD_MENU )
            {
            if ( aMenuPane )
                {
                aMenuPane->SetItemDimmed(
                    EUssdMenuItemSend,
                    iAppContainer->Editor().TextLength() == 0 );
                }
            // If aMenuPane == NULL, do nothing.
            }

        // If aResourceId != R_USSD_MENU, do nothing
        }

    // If help not defined, do nothing
    }

// -----------------------------------------------------------------------------
// CUssdAppUi::SetSofkeySendVisibleL
// Sets Send softkey (in)visible.
// -----------------------------------------------------------------------------
void CUssdAppUi::SetSoftkeySendVisibleL(
    TBool aVisible )
    {
    if ( !FeatureManager::FeatureSupported( KFeatureIdHelp ) )
        {
        // Help not defined
        TInt newResource;

        if ( aVisible )
            {
            if ( iCbaResource == R_USSD_SOFTKEYS_SEND_EXIT )
                {
                return; // Send is already visible
                }
            newResource = R_USSD_SOFTKEYS_SEND_EXIT; // change it
            }
        else
            {
            if ( iCbaResource == R_AVKON_SOFTKEYS_EXIT )
                {
                return; // Send is already invisible
                }
            newResource = R_AVKON_SOFTKEYS_EXIT; // change it.
            }

        // Update Softkeys
        Cba()->SetCommandSetL( newResource );
        iCbaResource = newResource;
        Cba()->DrawNow();
        }

    // If help defined, do nothing.
    }

// End of File
