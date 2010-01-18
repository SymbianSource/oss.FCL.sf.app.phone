/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Implementation of CPhoneNoteController class.
*
*/


// INCLUDE FILES
#include "cphonenotecontroller.h"

#include <eikenv.h>
#include <eikappui.h>
#include <eikprogi.h>
#include <StringLoader.h>
#include <aknnotedialog.h> // for CAknNoteDialog
#include <AknGlobalNote.h> // for CAknGlobalNote

#include "phoneui.pan"
#include "tphonecommandparam.h"
#include "tphonecmdparamnote.h"
#include "tphonecmdparamboolean.h"
#include "tphonecmdparamprogressnote.h" 
#include "tphonecmdparamglobalnote.h"
#include "tphonecmdparaminteger.h"
#include "phoneconstants.h"
#include "cphonemainresourceresolver.h"
#include "phonerssbase.h"
#include "phonelogger.h"
#include "cphonetimer.h"
#include "cphonenote.h"
#include "cphonedtmfsendingnote.h"
#include "cphoneuidisablednote.h"

// CONSTANTS
const TInt KPhoneIntervalDivider = 100;
// ================= MEMBER FUNCTIONS =======================
// C++ default constructor can NOT contain any code, that
// might leave.
//
CPhoneNoteController::CPhoneNoteController( CEikonEnv& aEikEnv ) : iGlobalNoteId( KErrNotFound ), 
                                                                   iEikEnv( aEikEnv ),
                                                                   iNoteText( NULL )
    {
    }

// ---------------------------------------------------------
// CPhoneNoteController::NewL()
// Two-phased constructor
// (other items were commented in a header).
// ---------------------------------------------------------
CPhoneNoteController* CPhoneNoteController::NewL( CEikonEnv& aEikEnv )
    {   
    CPhoneNoteController* self = 
        new( ELeave ) CPhoneNoteController( aEikEnv );
    
    return self;
    }

// ---------------------------------------------------------
// Destructor
// ---------------------------------------------------------
// 
CPhoneNoteController::~CPhoneNoteController()
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneNoteController::~CPhoneNoteController");
    DestroyNote();
    DestroyProgressNote();
    }

// ---------------------------------------------------------
// CPhoneNoteController::CreateNoteL
// ---------------------------------------------------------
//
void CPhoneNoteController::CreateNoteL( 
    TPhoneCommandParam* aCommandParam )
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneNoteController::CreateNoteL");
    TInt resourceId = 0;
    CAknNoteDialog::TTone tone = CAknNoteDialog::ENoTone;
    CAknNoteDialog::TTimeout timeout = CAknNoteDialog::ENoTimeout;

    TPhoneCmdParamNote* noteParam = static_cast<TPhoneCmdParamNote*>(
        aCommandParam );
    
    if( iNote && noteParam->Type() == EPhoneNoteDtmfSending )
        {
        // Dtmf sending ongoing.
        UpdateNoteTextL( aCommandParam );
        return;
        }
    
    // Delete the existing note
    DestroyNote();

    // Setup parameters according to the type of note
    switch ( noteParam->Type() )
        {
        case EPhoneNoteError:
            resourceId = CPhoneMainResourceResolver::Instance()->
                ResolveResourceID( EPhoneInformationErrorNote );
            timeout = CAknNoteDialog::ELongTimeout;
            tone = CAknNoteDialog::EErrorTone;
            break;
            
        case EPhoneNoteWarning:
            resourceId = CPhoneMainResourceResolver::Instance()->
                ResolveResourceID( EPhoneInformationWarningNote );
            timeout = CAknNoteDialog::ELongTimeout;
            tone = CAknNoteDialog::EWarningTone;
            break;
            
        case EPhoneNoteConfirmation:
            resourceId = CPhoneMainResourceResolver::Instance()->
                ResolveResourceID( EPhoneInformationConfirmationNote );
            timeout = CAknNoteDialog::EShortTimeout;
            tone = CAknNoteDialog::EConfirmationTone;
            break;
            
        case EPhoneNoteInformation:
            resourceId = CPhoneMainResourceResolver::Instance()->
                ResolveResourceID( EPhoneInformationNote );
            timeout = CAknNoteDialog::ELongTimeout;
            tone = CAknNoteDialog::ENoTone;
            break;
        
        case EPhoneNoteSecurity:
        case EPhoneNoteCustom: // Fall through
        case EPhoneNotePermanent:
        case EPhoneNoteDtmfSending:
            // Use the specified values
            resourceId = noteParam->ResourceId();
            timeout = noteParam->Timeout();
            tone = noteParam->Tone();
            break;

        case EPhoneNoteUIDisabled:
            // Use the specified values
            resourceId = noteParam->ResourceId();
            timeout = CAknNoteDialog::ENoTimeout;
            tone = CAknNoteDialog::ENoTone;
            break;
            
        default:
            Panic( EPhoneViewCaseNotHandled );
            break;
        }

	// Create correct note
	if( noteParam->Type() == EPhoneNoteDtmfSending )
		{
		iNote = new ( ELeave ) CPhoneDtmfSendingNote( reinterpret_cast<CEikDialog**>( &iNote ), *iEikEnv.EikAppUi() );						
		}
	else if( noteParam->Type() == EPhoneNoteUIDisabled )
        {
        iNote = new ( ELeave ) CPhoneUIDisabledNote( reinterpret_cast<CEikDialog**>( &iNote ), *iEikEnv.EikAppUi() );
        }
	else
		{
		iNote = new ( ELeave ) CPhoneNote( reinterpret_cast<CEikDialog**>( &iNote ), *iEikEnv.EikAppUi() );			
		}

    // Override text if specified
    if ( noteParam->Text().Length() )
        {
        if( !iNoteText )
            {
			iNoteText = noteParam->Text().AllocL();	            	
            }
        iNote->SetTextL( *iNoteText );
        }
    else if ( noteParam->TextResourceId() )
        {
        // Get the text associated with the resource Id
        TBuf<KPhoneMaxCharsInNote> noteText;
        StringLoader::Load( 
            noteText, 
            noteParam->TextResourceId(), 
            &iEikEnv );

        iNote->SetTextL( noteText );
        }

    // Override tone if specified
    if ( tone != CAknNoteDialog::ENoTone )
        {
        iNote->SetTone( tone );
        }

    // Override timeout if specified
    if ( timeout != CAknNoteDialog::ENoTimeout )
        {
        iNote->SetTimeout( timeout );
        }
    
    iNote->SetNoteType( noteParam->Type() );
 
    // Display the note
	iNote->ExecuteLD( resourceId );
    }

// ---------------------------------------------------------
// CPhoneNoteController::DestroyNote
// ---------------------------------------------------------
//
void CPhoneNoteController::DestroyNote()
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneNoteController::DestroyNote");
    
    // Delete note text if exists
    if( iNoteText )
        {
        delete iNoteText;
        iNoteText = NULL;            
        }
    
    // Delete the note if it exists
    if ( iNote )
        {
        delete iNote;
        iNote = NULL;
        }
    }

// ---------------------------------------------------------
// CPhoneNoteController::DestroyDTMFNote
// ---------------------------------------------------------
//
void CPhoneNoteController::DestroyDTMFNote()
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneNoteController::DestroyDTMFNote");
   
    // Delete DTFM note if exists
    if ( iNote && ( iNote->NoteType() == EPhoneNoteDtmfSending ) )
        {
        // Delete note text if exists
        if( iNoteText )
            {
            delete iNoteText;
            iNoteText = NULL;            
            }
        delete iNote;
        iNote = NULL;
        }
    }


// ---------------------------------------------------------
// CPhoneNoteController::DestroyGlobalNoteL
// ---------------------------------------------------------
//
void CPhoneNoteController::DestroyGlobalNoteL()
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneNoteController::DestroyGlobalNoteL");
    if( iGlobalNoteId != KErrNotFound )
        {
        CAknGlobalNote* glbNote = CAknGlobalNote::NewLC();
        glbNote->CancelNoteL( iGlobalNoteId );
        CleanupStack::PopAndDestroy( glbNote );        
        }
    }

// ---------------------------------------------------------
// CPhoneNoteController::CreateGlobalNoteL
// ---------------------------------------------------------
//
void CPhoneNoteController::CreateGlobalNoteL(
    TPhoneCommandParam* aCommandParam )
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneNoteController::CreateGlobalNoteL");
    TPhoneCmdParamGlobalNote* globalNoteParam = 
        static_cast<TPhoneCmdParamGlobalNote*>( aCommandParam );
    TBuf<KPhoneMaxCharsInNote> noteText;

    // Check if the resource Id is specified
    if ( globalNoteParam->TextResourceId() )
        {
        if ( globalNoteParam->Text().Length() )
            {
            HBufC* loadedText = StringLoader::LoadL( 
                globalNoteParam->TextResourceId(), 
                globalNoteParam->Text(), 
                CCoeEnv::Static() );
            noteText.Copy( *loadedText );
            delete loadedText;
            }
        else
            {
            StringLoader::Load( 
                noteText, 
                globalNoteParam->TextResourceId(), 
                &iEikEnv );
            }
        }
    else if ( globalNoteParam->Text() != KNullDesC )
        {
        noteText.Copy( globalNoteParam->Text() );
        }

    if ( noteText.Length() > 0 )
        {
        CAknGlobalNote* glbNote = CAknGlobalNote::NewLC();

        // Set the tone
        glbNote->SetTone( globalNoteParam->Tone() );
        if ( globalNoteParam->Softkeys())
            {
            glbNote->SetSoftkeys(  globalNoteParam->Softkeys() );   
            }

        // Show the global note
        
        if ( globalNoteParam->WaitForReady() )
            {
            TRequestStatus NoteReady;
            iGlobalNoteId = glbNote->ShowNoteL( NoteReady, globalNoteParam->Type(), noteText );
            User::WaitForRequest( NoteReady );
            }
        else
            {
            iGlobalNoteId = glbNote->ShowNoteL( globalNoteParam->Type(), noteText );
            }
        CleanupStack::PopAndDestroy( glbNote );
        }
    }

// ---------------------------------------------------------
// CPhoneNoteController::CreateProgressNoteL
// ---------------------------------------------------------
//
void CPhoneNoteController::CreateProgressNoteL(
    TPhoneCommandParam* aCommandParam )
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneNoteController::CreateProgressNoteL");
    TPhoneCmdParamProgressNote* noteParam = static_cast<
        TPhoneCmdParamProgressNote*>(aCommandParam );

    TInt resourceId = noteParam->ResourceId();
    iStartTime.UniversalTime();
    
    TTimeIntervalMicroSeconds32 waitTime =
        noteParam->WaitTime();
        
    if( !iProgressUpdateTimer )
        {
        iProgressUpdateTimer = CPhoneTimer::NewL();            
        }

    iWaitTime = waitTime.Int();
    
    TInt final = waitTime.Int();
    iInterval = final / KPhoneIntervalDivider;
    iProgressBarCount = 0;
        
    iProgressUpdateTimer->After( iInterval, this );
        
    CPhoneProgressNote* dlg = 
        new ( ELeave ) CPhoneProgressNote(
             ( reinterpret_cast< CEikDialog** >( &iProgressNote )),
             static_cast<MEikCommandObserver*>( iEikEnv.EikAppUi() ) );

    iProgressNote = dlg;
    dlg->PrepareLC( resourceId );

    CEikProgressInfo* info = dlg->GetProgressInfoL();
    
    if ( info )
        {
        info->SetFinalValue( KPhoneIntervalDivider );
        }
    
    dlg->RunLD();        
    }
    
// ---------------------------------------------------------
// CPhoneNoteController::DestroyProgressBar
// ---------------------------------------------------------
//
void CPhoneNoteController::DestroyProgressNote()
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneNoteController::DestroyProgressNote");

    // Delete the progress note if it exists
    if ( iProgressNote )
        {
        delete iProgressNote;
        iProgressNote = NULL;
        }
    if ( iProgressUpdateTimer )
        {
        if( iProgressUpdateTimer->IsActive() )
            {
            iProgressUpdateTimer->CancelTimer();                
            }
            
        delete iProgressUpdateTimer;
        iProgressUpdateTimer = NULL;            
        }
    iProgressBarCount = 0; 
    }
    
// ---------------------------------------------------------
// CPhoneNoteController::HandleTimeOutL
// ---------------------------------------------------------
//
void CPhoneNoteController::HandleTimeOutL()
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneNoteController::HandleTimeOutL");

    if ( iProgressNote )
        {
        TTime current;
        current.UniversalTime();
        
        CEikProgressInfo* info = iProgressNote->GetProgressInfoL();           

        if ( info )
            {
            TTime current;
            current.UniversalTime();
            TInt64 timeDelta = 
                    current.MicroSecondsFrom( iStartTime ).Int64();
                    
            iProgressBarCount = ( timeDelta / iInterval )+1 ;
            if ( iProgressBarCount > KPhoneIntervalDivider )
                {
                 iProgressBarCount = KPhoneIntervalDivider;           
                }
            info->SetAndDraw( iProgressBarCount );                     
            }        
        
        if ( !iProgressUpdateTimer->IsActive() )
            {
            iProgressUpdateTimer->After( iInterval, this );
            }
        }
    else
        {         
        if( iProgressUpdateTimer->IsActive() )
            {
            iProgressUpdateTimer->CancelTimer();                
            }
            
        delete iProgressUpdateTimer;
        iProgressUpdateTimer = NULL;
        iProgressBarCount = 0;            
        }
    }

// ---------------------------------------------------------
// CPhoneNoteController::IsNoteVisible
// ---------------------------------------------------------
//
void CPhoneNoteController::IsNoteVisible( TPhoneCommandParam* aCommandParam )
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneNoteController::IsNoteVisible");

    TPhoneCmdParamBoolean* booleanParam = 
        static_cast<TPhoneCmdParamBoolean*>( aCommandParam );

    if( iNote )
        {
        if( iNote->IsVisible() )
            {
            booleanParam->SetBoolean( ETrue );                
            }
        }
    else
        {
        booleanParam->SetBoolean( EFalse );                
        }
    }

// ---------------------------------------------------------
// CPhoneNoteController::IsProgressNoteVisible
// ---------------------------------------------------------
//
void CPhoneNoteController::IsProgressNoteVisible( TPhoneCommandParam* aCommandParam )
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneNoteController::IsProgressNoteVisible");

    TPhoneCmdParamBoolean* booleanParam = 
        static_cast<TPhoneCmdParamBoolean*>( aCommandParam );

    if( iProgressNote )
        {
        if( iProgressNote->IsVisible() )
            {
            booleanParam->SetBoolean( ETrue );
            }
        }
    else
        {
        booleanParam->SetBoolean( EFalse );                
        }
    }

// ---------------------------------------------------------
// CPhoneNoteController::IsNoteActive
// ---------------------------------------------------------
//
void CPhoneNoteController::IsNoteActive( TPhoneCommandParam* aCommandParam )
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneNoteController::IsNote");

    TPhoneCmdParamBoolean* booleanParam = 
        static_cast<TPhoneCmdParamBoolean*>( aCommandParam );

    if( iProgressNote || iNote )
        {
        booleanParam->SetBoolean( ETrue );                
        }
    else 
        {
        booleanParam->SetBoolean( EFalse );    
        }
    }

// ---------------------------------------------------------
// CPhoneNoteController::IsActiveNoteDissmissableByKeyEvent
// ---------------------------------------------------------
//
void CPhoneNoteController::IsActiveNoteDissmissableByKeyEvent( TPhoneCommandParam* aCommandParam )
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneNoteController::IsActiveNoteDissmissableByKeyEvent");

    TPhoneCmdParamBoolean* booleanParam = 
        static_cast<TPhoneCmdParamBoolean*>( aCommandParam );
    // check first that iNote is created. 
    if( ( iNote ) &&
        ( iNote->NoteType() == EPhoneNoteSecurity ) )
        {
        booleanParam->SetBoolean( ETrue );                
        }
    else 
        {
        booleanParam->SetBoolean( EFalse );    
        }
    }

// ---------------------------------------------------------
// CPhoneNoteController::UpdateNoteTextL
// ---------------------------------------------------------
//
void CPhoneNoteController::UpdateNoteTextL( TPhoneCommandParam* aCommandParam )
    {
    TPhoneCmdParamNote* noteParam = static_cast<TPhoneCmdParamNote*>(
           aCommandParam );
    
    // Delete note text if exists
    if( iNoteText )
        {
        delete iNoteText;
        iNoteText = NULL;           
        }
    
    if ( iNote )
        {
        // Override text if specified
        if ( noteParam->Text().Length() )
            {
            if( !iNoteText )
                {
                iNoteText = noteParam->Text().Alloc();                  
                }
            iNote->SetTextL( *iNoteText );
            }
        else if ( noteParam->TextResourceId() )
            {
            // Get the text associated with the resource Id
            TBuf<KPhoneMaxCharsInNote> noteText;
            StringLoader::Load( 
                noteText, 
                noteParam->TextResourceId(), 
                &iEikEnv );

            iNote->SetTextL( noteText );
            }
        }
    }

// ---------------------------------------------------------
// CPhoneNoteController::UpdateNoteSoftkeysL
// ---------------------------------------------------------
//
void CPhoneNoteController::UpdateNoteSoftkeysL( TPhoneCommandParam* aCommandParam )
    {
     
    TPhoneCmdParamInteger* integerParam = 
            static_cast<TPhoneCmdParamInteger*>( aCommandParam );
    
    if ( iNote )
        {
        iNote->UpdateSoftkeysL( integerParam->Integer() );
        }
    }

// End of File
