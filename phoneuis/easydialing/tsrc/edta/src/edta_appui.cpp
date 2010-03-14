/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Easy dialing test application.
*
*/

// INCLUDE FILES
#include <avkon.hrh>
#include <aknnotewrappers.h>
#include <stringloader.h>
#include <Edta.rsg>
#include <f32file.h>
#include <s32file.h>
#include <aiwservicehandler.h>
#include <centralrepository.h>
#include <easydialingcrkeys.h>

#include "edta.pan"
#include "edta_appui.h"
#include "edta_appview.h"
#include "edta.hrh"
#include "edta_querydialog.h"
#include <AknQueryDialog.h>




// CONSTANTS
const TInt KSearchStringMaxLen = 64;


// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CEdtaAppUi::ConstructL()
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CEdtaAppUi::ConstructL()
    {
    // Initialise app UI with standard value.
    BaseConstructL(CAknAppUi::EAknEnableSkin);

    // Create view object
    iAppView = CEdtaAppView::NewL( ClientRect() );
    
    // TODO: remove?
    iServiceHandler = CAiwServiceHandler::NewL();

    iRepository = CRepository::NewL( KCRUidEasyDialSettings );
    
    AddToStackL(iAppView);
    }

// -----------------------------------------------------------------------------
// CEdtaAppUi::CEdtaAppUi()
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CEdtaAppUi::CEdtaAppUi()
    {
    // No implementation required
    }

// -----------------------------------------------------------------------------
// CEdtaAppUi::~CEdtaAppUi()
// Destructor.
// -----------------------------------------------------------------------------
//
CEdtaAppUi::~CEdtaAppUi()
    {
    if ( iAppView )
        {
        RemoveFromStack(iAppView);
        delete iAppView;
        iAppView = NULL;
        }
    delete iServiceHandler;
    
    if ( iRepository )
        {
        // default setting
        iRepository->Set(KEasyDialingContactThumbnails, 1);
        }
    delete iRepository;
    }


TInt CreateTestContactDatabaseL(); // Should be moved into correct place

// -----------------------------------------------------------------------------
// CEdtaAppUi::HandleCommandL()
// Takes care of command handling.
// -----------------------------------------------------------------------------
//
void CEdtaAppUi::HandleCommandL( TInt aCommand )
    {
    DebugPrintF(_L("CEdtaAppUi::HandleCommandL enter: 0x%X"), aCommand );

    switch( aCommand )
        {
        case EEikCmdExit:
        case EAknSoftkeyExit:
            DebugPrintF(_L("CEdtaAppUi::HandleCommandL: EXIT"));
            Exit();
            break;

        case EEdtaSetPluginInput:
            {
            DebugPrintF(_L("CEdtaAppUi::HandleCommandL: EEdtaSetPluginInput"));

            if( CheckPluginLoadedAndShowNote() )
                {
                CAknTextQueryDialog* dlg = CAknTextQueryDialog::NewL( iQueryText );
                if( dlg->ExecuteLD(R_DIALOG_TEXT_EDIT_QUERY))
                    {
                    if (iAppView->GetEasyDialer())
                        {
                        iAppView->GetEasyDialer()->SetInputL( iQueryText );
                        }
                    }
                }
            }
            break;

        case EEdtaCommandShowContacts:
            {
            DebugPrintF(_L("CEdtaAppUi::HandleCommandL: EEdtaCommandShowContacts"));

            if( iAppView->IsPCSServerLoaded() )
                {
                CAknTextQueryDialog* dlg = CAknTextQueryDialog::NewL( iQueryText );
                if( dlg->ExecuteLD(R_DIALOG_TEXT_EDIT_QUERY))
                    {
                    iAppView->ShowMatchingContactsL(iQueryText);
                    }
                }
            }
            break;

        case EEdtaLaunchFirstContact:
            {
            DebugPrintF(_L("CEdtaAppUi::HandleCommandL: EEdtaLaunchFirstContact"));
            if( iAppView->IsPCSServerLoaded() )
                iAppView->LaunchNthContactL( 0 );
            }
            break;

        case EEdtaSelectAndLaunchContact:
            {
            DebugPrintF(_L("CEdtaAppUi::HandleCommandL: EEdtaSelectAndLaunchContact"));
            iAppView->SelectAndLaunchContactL();
            }
            break;

        case EEdtaCommandCheckPCSServer:
            {
            DebugPrintF(_L("CEdtaAppUi::HandleCommandL: EEdtaCommandCheckPCSServer"));
            if( iAppView->IsPCSServerLoaded() )
                iAppView->CheckPCSServerL();
            }
            break;

        case EEdtaCommandShowNameOrdering:
            {
            DebugPrintF(_L("CEdtaAppUi::HandleCommandL: EEdtaCommandShowNameOrdering"));
            iAppView->ShowNameOrderL();
            }
            break;

        case EEdtaCommandCreateTestContactBase:
            {
            DebugPrintF(_L("CEdtaAppUi::HandleCommandL: EEdtaCommandCreateTestContactBase"));
            TInt newContactsCount = CreateTestContactDatabaseL();
            iAppView->GetScreenBuffer()->WriteLineL(_L("Test contacts updated"));
            iAppView->GetScreenBuffer()->WriteLineL(_L("%d new contacts"), newContactsCount );
            }
            break;
            
        case EEdtaCommandTwoConsequtiveSearchesTest:
            DebugPrintF(_L("CEdtaAppUi::HandleCommandL: Two search test"));

             if( CheckPluginLoadedAndShowNote() )
                 {
                 CAknTextQueryDialog* dlg = CAknTextQueryDialog::NewL( iQueryText );
                 if( dlg->ExecuteLD(R_DIALOG_TEXT_EDIT_QUERY))
                     {
                     HBufC* anotherQueryText = HBufC::NewLC( KSearchStringMaxLen );
                     TPtr ptr = anotherQueryText->Des();
                     
                     CAknTextQueryDialog* dlg2 = CAknTextQueryDialog::NewL( ptr );
                     
                     if ( dlg2->ExecuteLD(R_DIALOG_TEXT_EDIT_QUERY) )
                         {                   
                         if (iAppView->GetEasyDialer())
                             {
                             iAppView->GetEasyDialer()->SetInputL( iQueryText );
                             iAppView->GetEasyDialer()->SetInputL( ptr );
                             }
                         }
                     CleanupStack::PopAndDestroy( anotherQueryText );
                     }
                 }
             break;

        case EEdtaCommandMiscCoverageTest:
            
            if (iAppView->GetEasyDialer())
                 {
                 iAppView->DoMiscellaneousTestsL(  );
                 }            
            break;
        
        case EEdtaCommandShowThumbnails:
            iRepository->Set(KEasyDialingContactThumbnails, 1);         
            break;
            
        case EEdtaCommandHideThumbnails:
            iRepository->Set(KEasyDialingContactThumbnails, 0);
            break;
            
        default:
            iServiceHandler->ExecuteMenuCmdL(
                aCommand,
                iServiceHandler->InParamListL(),    // No input parameters
                iServiceHandler->OutParamListL(),   // No output parameters
                0,                                  // No options used.
                NULL);                              // No need for callback    

            DebugPrintF(_L("CEdtaAppUi::HandleCommandL: AIW (or unknown) menu command: 0x%X"), aCommand );
            break;
        }

    DebugPrintF(_L("CEdtaAppUi::HandleCommandL exit"));
    }


void CEdtaAppUi::DynInitMenuPaneL(TInt aResourceId,CEikMenuPane* aMenuPane)
    {
    // First, offer menu pane to AIW framework. It might be the case, that the 
    // user is opening an AIW submenu. In this case, the AIW handles the menu.
    if ( iServiceHandler->HandleSubmenuL( *aMenuPane ) )
        {
        return;
        }
        
    // Add your normal (non-AIW) menu initialisation code here...      

    // Let AIW provider add its menu items to the menu.
    iServiceHandler->InitializeMenuPaneL(
        *aMenuPane, 
        aResourceId, 
        EEdtaCommandLast, 
        iServiceHandler->InParamListL());    
    }

// -----------------------------------------------------------------------------
//  Called by the framework when the application status pane
//  size is changed.  Passes the new client rectangle to the
//  AppView
// -----------------------------------------------------------------------------
//
void CEdtaAppUi::HandleStatusPaneSizeChange()
    {
    iAppView->SetRect( ClientRect() );
    }



// -----------------------------------------------------------------------------
// Display note id easydialing plugin is not loaded
// -----------------------------------------------------------------------------
//
TBool CEdtaAppUi::CheckPluginLoadedAndShowNote()
    {
    TBool plugin_loaded( ETrue );

    if( !(iAppView->GetEasyDialer()) )
        {
        plugin_loaded = EFalse;
        TBuf<KEdtaMaxLineWidth> tempStr;
        tempStr.Copy(_L("Easydialing plugin not loaded!"));
        
        TRAP_IGNORE(
                {
                CAknErrorNote* myNote = new (ELeave) CAknErrorNote();
                myNote->SetTimeout( CAknNoteDialog::ELongTimeout );
                myNote->ExecuteLD( tempStr );
                }
        );
        }

    DebugPrintF(_L("CEdtaAppUi::CheckPluginLoadedAndShowNote: Plugin loaded: %d"), plugin_loaded );

    return plugin_loaded;
    }

// End of File
