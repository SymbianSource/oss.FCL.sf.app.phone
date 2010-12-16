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
* Description: Implementation of CPhoneQueryController class.
*
*/


// INCLUDE FILES
#include <AknGlobalConfirmationQuery.h>
#include <eikenv.h>
#include <StringLoader.h>
#include <ccauireconnectquery.h>
#include <mcauireconnectqueryobserver.h>
#include <AknGlobalMsgQuery.h>
#include <AknGlobalNote.h> // for CAknGlobalNote

#include "phoneui.hrh"
#include "cphonequerycontroller.h"
#include "tphonecommandparam.h"
#include "tphonecmdparamreconnectquery.h"
#include "tphonecmdparamstring.h"
#include "phonelogger.h"
#include "cphonequerydialog.h"
#include "tphonecmdparamboolean.h"
#include "cphonedtmfnumberlistquery.h"
#include "cphonelistquerydialog.h"
#include "cphonetimer.h"

// CONSTANTS
const TInt KPhoneRestartWaitNoteInterval = 5000000;

// ================= MEMBER FUNCTIONS =======================
// C++ default constructor can NOT contain any code, that
// might leave.
//
CPhoneQueryController::CPhoneQueryController( CEikonEnv& aEikEnv )
    : CActive ( EPriorityUserInput ),
    iQueryDeleted( ETrue ),
    iEikEnv( aEikEnv )
    {
    CActiveScheduler::Add( this );
    }

// ---------------------------------------------------------
// CPhoneQueryController::NewL()
// Two-phased constructor
// (other items were commented in a header).
// ---------------------------------------------------------
CPhoneQueryController* CPhoneQueryController::NewL( CEikonEnv& aEikEnv )
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneQueryController::NewL( ) ");
    CPhoneQueryController* self =
        new( ELeave ) CPhoneQueryController( aEikEnv );

    return self;
    }

// ---------------------------------------------------------
// Destructor
// ---------------------------------------------------------
//
CPhoneQueryController::~CPhoneQueryController()
    {
    DestroyQuery();
    ClearTimer();
    DestroyGlobalQuery();
    DestroyGlobalMsgQuery();

    if( iCommandAsync )
        {
        delete iCommandAsync;
        iCommandAsync = NULL;
        }
    iQueryPreviousText.Close();
    }

// ---------------------------------------------------------
// CPhoneQueryController::CreateNoteL
// ---------------------------------------------------------
//
void CPhoneQueryController::CreateQueryL(
    TPhoneCommandParam* aCommandParam )
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneQueryController::CreateQueryL()" );

    // Cancel previous note. This prevents scheduler crashes.
    // Canceling must be done here since iActiveAvkonQuery may be overwritten.
    Cancel();

    TPhoneCmdParamQuery& params = *static_cast<TPhoneCmdParamQuery*>( aCommandParam );
    iCommandId = NULL;
    iNoConnectedCalls = EFalse;
    switch( params.ParamId() )
	    {
        case TPhoneCommandParam::EPhoneParamIdQuery:
	    case TPhoneCommandParam::EPhoneParamIdVideoCallSetupFailedCreateVoiceCallToTheSameContactQuery:
        case TPhoneCommandParam::EPhoneParamIdUnattendedTransferAcceptanceQuery:
        case TPhoneCommandParam::EPhoneParamIdGlobalWaitNote:
	    	CreateDefaultQueryL( params );
	    	break;

	    case TPhoneCommandParam::EPhoneParamIdReconnectQuery:
	    	CreateReconnectQueryL( aCommandParam );
	    	break;

	    case TPhoneCommandParam::EPhoneParamRebootQuery:
	        {
            ClearTimer();
            iTimer = CPhoneTimer::NewL();
            iTimer->After( KPhoneRestartWaitNoteInterval, this );

            iSimSwitchRestartWaitNote = CAknGlobalNote::NewL();
            iSimSwitchRestartWaitNote->SetSoftkeys( R_AVKON_SOFTKEYS_EMPTY );
            // Show the global note
            iSimSwitchRestartWaitNote->ShowNoteL( iStatus, 
                    EAknGlobalWaitNote, params.QueryPrompt()  );

            SetActive();        
	        }
	        break;

	    default:
	    	break;
	    }
    }

// ---------------------------------------------------------
// CPhoneQueryController::DestroyQuery
// ---------------------------------------------------------
//
void CPhoneQueryController::DestroyQuery()
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneQueryController::DestroyQuery( ) ");

    // Delete the query if it exists
    if ( iQuery )
        {
        delete iQuery;
        iQuery = NULL;
        }

    iQueryNoteText.Close();
    iQueryPrompt.Close();

    if( iCallUiReconnectQuery )
	    {
        delete iCallUiReconnectQuery;
        iCallUiReconnectQuery = NULL;
	    }

    if ( iDtmfNumberListQuery && !iQueryDeleted )
        {
        delete iDtmfNumberListQuery;
        iDtmfNumberListQuery = NULL;
        }

  	if ( iTextQuery )
  	    {
  	    delete iTextQuery;
  	    iTextQuery = NULL;
  	    }

    if ( iListQuery && !iQueryDeleted )
        {
        delete iListQuery;
        iListQuery = NULL;
        }

    if( iTextQueryDisplayer )
        {
        delete iTextQueryDisplayer;
        iTextQueryDisplayer = NULL;
        }
    }

// ---------------------------------------------------------
// CPhoneQueryController::DestroyGlobalQuery
// ---------------------------------------------------------
//
void CPhoneQueryController::DestroyGlobalQuery()
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneQueryController::DestroyGlobalQuery( ) ");

    if ( iGlobalConfirmationQuery )
        {
        iGlobalConfirmationQuery->CancelConfirmationQuery();

        delete iGlobalConfirmationQuery;
        iGlobalConfirmationQuery = NULL;
        }
    DestroyGlobalWaitNote();
    }

// ---------------------------------------------------------
// CPhoneQueryController::CreateQueryDialogL
// ---------------------------------------------------------
//
void CPhoneQueryController::CreateQueryDialogL(
    const TPhoneCmdParamQuery& aParams )
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneQueryController::CreateQueryDialogL( ) ");

    // Delete the existing query
    DestroyQuery();

    iQueryParam = aParams;

    // Create the query
	iQuery = CPhoneQueryDialog::NewL();

    if ( iQuery )
        {
        // Set the prompt if specified
        if ( aParams.QueryPrompt().Length() )
            {
            iQueryPrompt.Close();
            iQueryPrompt.Create( aParams.QueryPrompt() );
            iQuery->SetPromptL( iQueryPrompt );
            }

        // Display the query
		TInt buttonId = iQuery->ExecuteLD( aParams.QueryResourceId() );
        iQuery = NULL;

		if( buttonId != EPhoneCmdDoNothing )
			{
            if ( iQueryParam.QueryCommandObserver() )
                {
                // Let observer handle command.
                iQueryParam.QueryCommandObserver()->ProcessCommandL(
                        iStatus.Int() );
                return;
                }
            else
                {
                // Forward pressed CBA to the framework so that we can handle it
                // in our active state
                static_cast<MEikCommandObserver*>( iEikEnv.EikAppUi() )
                    ->ProcessCommandL( buttonId );
                }
			}
        }
    }

// ---------------------------------------------------------
// CPhoneQueryController::CreateTextQueryDialogL
// ---------------------------------------------------------
//
void CPhoneQueryController::CreateTextQueryDialogL(
        const TPhoneCmdParamQuery& aParams,
        TInt aEikBidOkCmd )
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneQueryController::CreateTextQueryDialogL( ) ");

    // Delete the existing query
    DestroyQuery();

    iQueryNoteText.Create( *aParams.DataText() );

    iTextQuery = new( ELeave ) CPhoneTextQuery(
        *this,
        iQueryNoteText,
        aParams.DefaultCba(),
        aParams.ContentCba(),
        aParams.SendKeyEnabled(),
        aEikBidOkCmd );

    // Set the prompt if specified
    if ( aParams.QueryPrompt().Length() )
        {
        iQueryPrompt.Create( aParams.QueryPrompt() );
        iTextQuery->SetPromptL( iQueryPrompt );
        }

    // Cut the call stack so that Phone UI is responsive
    iTextQueryDisplayer = CIdle::NewL( EPriorityNormal );
    iTextQueryDisplayer->Start( TCallBack( DisplayTextQueryCallBack, this ) );
    }

// ---------------------------------------------------------
// CPhoneQueryController::SetNoConnectedCalls
// ---------------------------------------------------------
//
void CPhoneQueryController::SetNoConnectedCalls( TPhoneCommandParam* aCommandParam )
    {
    TPhoneCmdParamBoolean* booleanParam =
            static_cast<TPhoneCmdParamBoolean*>( aCommandParam );
    iNoConnectedCalls = booleanParam->Boolean();
    __PHONELOG1( EBasic, EPhoneUIView,
           "CPhoneQueryController::SetNoConnectedCalls(%d)",
           iNoConnectedCalls );
    }

// ---------------------------------------------------------
// CPhoneQueryController::DisplayTextQueryCallBackL
// ---------------------------------------------------------
//
TInt CPhoneQueryController::DisplayTextQueryCallBack( TAny* aCPhoneQueryController )
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneQueryController::DisplayTextQueryCallBack( ) ");

    // Display the query
    CPhoneQueryController* queryController =
        static_cast<CPhoneQueryController*>( aCPhoneQueryController );

    TInt leaveError = KErrNone;
    TInt retVal = KErrNone;

    TRAP( leaveError, (
         retVal = queryController->iTextQuery->ExecuteLD( queryController->iQueryParam.QueryResourceId() ) ) );

    queryController->iTextQuery = NULL;

    // If iCommandId is EPhoneCmdEnd(User has pressed end dismiss DTMF query&dialog) or if iNoConnectedCalls
    // is TRUE it is an indication that remote party has terminated call and there is no connected call in device
    // therefore we dont need to reopen EPhoneInCallCmdDtmfListQuery.
    if ( retVal == KErrNone &&
         leaveError == KErrNone &&
         queryController->iQueryParam.QueryType() == EPhoneDtmfTextQuery &&
         queryController->iCommandId != EPhoneCmdEnd &&
         !queryController->iNoConnectedCalls )
        {
        //User has exited DMTF query with cancel, rule says that
        //we need to activate DTMF list query dialog.

        // Forward pressed CBA to the framework so that we can handle it
        // in our active state
        TRAP_IGNORE( static_cast<MEikCommandObserver*>( queryController->iEikEnv.EikAppUi() )
            ->ProcessCommandL( EPhoneInCallCmdDtmfListQuery ) );
        }

    queryController->iNoConnectedCalls = EFalse;
    return KErrNone;
    }

// ---------------------------------------------------------
// CPhoneQueryController::CreateListQueryL
// ---------------------------------------------------------
//
void CPhoneQueryController::CreateListQueryL( TInt aDialogResourceId,
                                              const RArray<TInt>& aCommands )
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneQueryController::CreateListQueryL( ) ");
    DestroyQuery();

    if ( !iListQuery )
       {
       iListQuery = new (ELeave)
                CPhoneListQueryDialog(
                static_cast<MEikCommandObserver&>( *iEikEnv.EikAppUi() ),
                *this, aCommands );
       }
    iQueryDeleted = EFalse;
    iCommandHandled = EFalse;
    iListQuery->ExecuteLD( aDialogResourceId );
    }

// ---------------------------------------------------------
// CPhoneQueryController::CreateCustomListQueryL
// ---------------------------------------------------------
//
void CPhoneQueryController::CreateCustomListQueryL(
        const TPhoneCmdParamQuery& aParams )
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneQueryController::CreateCustomListQueryL( ) ");
    DestroyQuery();
    RArray<TInt> commands;
    CleanupClosePushL( commands );
    const_cast<TPhoneCmdParamQuery&>(aParams).ItemTextArrayForListQuery();

    MDesCArray* itemArray = aParams.ItemTextArrayForListQuery();

    if ( NULL == itemArray )
        {
        User::Leave( KErrArgument );
        }

    for ( TInt i = 0; i < itemArray->MdcaCount(); i++ )
        {
        commands.Append( i );
        }

    if ( !iListQuery )
       {
       iListQuery = new (ELeave)
                CPhoneListQueryDialog(
                static_cast<MEikCommandObserver&>( *iEikEnv.EikAppUi() ),
                *this, commands );
       }

    iListQuery->PrepareLC( aParams.QueryResourceId() );


    iListQuery->SetItemTextArray( itemArray );
    iListQuery->SetOwnershipType( ELbmDoesNotOwnItemArray );
    iListQuery->SetTone( aParams.Tone() );

    iQueryDeleted = EFalse;
    iCommandHandled = EFalse;

    iListQuery->RunLD();

    CleanupStack::PopAndDestroy( &commands );
    }

// ---------------------------------------------------------
// CPhoneQueryController::CreateDtmfListQueryL
// ---------------------------------------------------------
//
void CPhoneQueryController::CreateDtmfListQueryL( TInt aDialogResourceId )
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneQueryController::CreateDtmfListQueryL( ) ");
    DestroyQuery();

    if ( !iDtmfNumberListQuery )
        {
        iDtmfNumberListQuery = CPhoneDtmfNumberListQuery::NewL(
            static_cast<MEikCommandObserver&>( *iEikEnv.EikAppUi() ),
                                               *this,
                                               iResultBuffer );
        }
    iQueryDeleted = EFalse;
    iDtmfNumberListQuery->ExecuteLD( aDialogResourceId );
    }

// ---------------------------------------------------------
// CPhoneQueryController::GetTextQueryContentL
// ---------------------------------------------------------
//
void CPhoneQueryController::GetTextQueryContentL(
    TPhoneCommandParam* aCommandParam )
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneQueryController::GetTextQueryContentL( ) ");

    TPhoneCmdParamString* stringParam =
        static_cast<TPhoneCmdParamString*>( aCommandParam );

    if ( iQueryPreviousText.Length() )
        {
        *stringParam->String() = iQueryPreviousText;
        iQueryPreviousText.Close();
        }
    else if ( iTextQuery  )
        {
        iTextQuery->GetContent( *stringParam->String() );
        }
    else if ( iResultBuffer )
        {
        stringParam->String()->Append( iResultBuffer->Des() );
        delete iResultBuffer;
        iResultBuffer = NULL;
        }

    __PHONELOG1( EBasic, EPhoneUIView,
        "CPhoneQueryController::GetTextQueryContentL(%S)",
        stringParam->String() );

    AknTextUtils::ConvertDigitsTo( *stringParam->String(), EDigitTypeWestern );

    __PHONELOG1( EBasic, EPhoneUIView,
        "CPhoneQueryController::GetTextQueryContentL(%S)",
        stringParam->String() );
    }

// ---------------------------------------------------------
// CPhoneQueryController::SetQueryDeletedFlag
// ---------------------------------------------------------
//
void CPhoneQueryController::SetQueryDeletedFlag( TBool aQueryDeleted )
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneQueryController::SetQueryDeletedFlag( ) ");
    switch( iActiveAvkonQuery )
        {
        case EPhoneContactsListQuery:
        case EPhoneCustomListQueryDialog:
             {
             iListQuery = NULL;

             if ( iQueryParam.QueryCommandObserver() &&
                  !iCommandHandled )
                 {
                 ReportQueryCompleteAsync( EAknSoftkeyCancel );
                 }
             }
             break;
        case EPhoneDtmfListQueryDialog:
             {
             iDtmfNumberListQuery = NULL;
             }
             break;
        default:
             break;
        }
    iQueryDeleted = aQueryDeleted;
    }


// ---------------------------------------------------------
// CPhoneQueryController::CreateDefaultQueryL
// ---------------------------------------------------------
//
void CPhoneQueryController::CreateDefaultQueryL( const TPhoneCmdParamQuery& aParams )
	{
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneQueryController::CreateDefaultQueryL( ) ");

    iQueryParam = aParams;
    iActiveAvkonQuery = aParams.QueryType();

    switch( iActiveAvkonQuery )
        {
        case EPhoneQueryDialog:
            CreateQueryDialogL( aParams );
            break;

        case EPhoneTextQueryDialog:
            CreateTextQueryDialogL( aParams );
            break;

       case EPhoneDtmfTextQuery:
       case EPhoneGenericTextQuery:
            CreateTextQueryDialogL( aParams,
                                    EPhoneCmdDtmfOk );
            break;

        case EPhoneGlobalQueryDialog:
            {
            if ( aParams.ParamId() ==
                TPhoneCommandParam::EPhoneParamIdVideoCallSetupFailedCreateVoiceCallToTheSameContactQuery )
                {
                iVideoCallSetupFailedQuery = ETrue;
                }
            else
                {
                iVideoCallSetupFailedQuery = EFalse;
                }
            // Use timeout if defined
            TTimeIntervalMicroSeconds32 timeout = aParams.TimeOut() * 1000000;
            if ( timeout.Int() > 0 && !iTimer )
                {
                iTimer = CPhoneTimer::NewL();
                // Start timer.
                iTimer->After( timeout, this );
                }
            // Create query
            CreateGlobalQueryDialogL( aParams );
            break;
            }
        case EPhoneCustomListQueryDialog:
              {
              CreateCustomListQueryL( aParams );
              }
            break;
        case EPhoneDtmfListQueryDialog:
              CreateDtmfListQueryL(
                aParams.QueryResourceId() );
             break;

        case EPhoneContactsListQuery:
              {
              RArray<TInt> commands;
              CleanupClosePushL( commands );
              commands.Append( EPhoneNumberAcqCmdSave );
              commands.Append( EPhoneNumberAcqCmdAddToName );
              CreateListQueryL( aParams.QueryResourceId(), commands );
              CleanupStack::PopAndDestroy(); // CleanupClosePushL
              }
            break;
        case EPhoneGlobalMsgQuery:
            {
            iTransferAcceptanceQuery = ( aParams.ParamId() ==
                TPhoneCommandParam::EPhoneParamIdUnattendedTransferAcceptanceQuery );

            CreateGlobalMsgQueryL( aParams );
            break;
            }
        case EPhoneGlobalWaitNote:
            {
            CreateGlobalWaitNoteL( aParams );
            break;
            }

        default:
            return;
        }
	}

// ---------------------------------------------------------
// CPhoneQueryController::CreateReconnectQueryL
// ---------------------------------------------------------
//
void CPhoneQueryController::CreateReconnectQueryL( TPhoneCommandParam* aCommandParam )
	{
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneQueryController::CreateReconnectQueryL( ) ");

    TPhoneCmdParamReconnectQuery* queryParam =
        static_cast<TPhoneCmdParamReconnectQuery*>( aCommandParam );

	if( !iCallUiReconnectQuery )
		{
		iCallUiReconnectQuery =
		    CCaUiReconnectQuery::NewL(
		        queryParam->ReconnectQueryObserver());
		}

   	iCallUiReconnectQuery->ShowL( queryParam->PhoneNumber(),
   	     queryParam->IncludeVideoCallOption() );
	}

// ---------------------------------------------------------
// CPhoneQueryController::CreateGlobalQueryDialogL
// ---------------------------------------------------------
//
void CPhoneQueryController::CreateGlobalQueryDialogL( const TPhoneCmdParamQuery& aParams )
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneQueryController::CreateGlobalQueryDialogL( ) ");

    // Delete the existing global query
    DestroyGlobalQuery();

    iGlobalConfirmationQuery = CAknGlobalConfirmationQuery::NewL();
    iGlobalConfirmationQuery->ShowConfirmationQueryL(
        iStatus,
        aParams.QueryPrompt(),
        aParams.DefaultCba(),
        R_QGN_NOTE_QUERY_ANIM,
        KNullDesC,
        0,
        0,
        CAknQueryDialog::TTone(aParams.Tone() ) );

    SetActive();
    }


// ---------------------------------------------------------------------------
// CPhoneQueryController::ReportQueryCompleteAsync
// ---------------------------------------------------------------------------
//
void CPhoneQueryController::ReportQueryCompleteAsync( TInt aCommand )
    {
    iCommandHandled = ETrue;
    Cancel();
    TRequestStatus* status = &iStatus;
    User::RequestComplete( status, aCommand );
    SetActive();
    }

// ---------------------------------------------------------
// CPhoneQueryController::RunL
// EAknSoftkeyYes and EAknSoftkeyNo need to be used with
// CAknGlobalConfirmationQuery. Others does not work
// ---------------------------------------------------------
//
void CPhoneQueryController::RunL()
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneQueryController::RunL( ) ");
    TInt buttonId( KErrNone );

    if ( iQueryParam.QueryCommandObserver() )
        {
        // Let observer handle command.
        iQueryParam.QueryCommandObserver()->ProcessCommandL(
                iStatus.Int() );
        return;
        }

    if ( iActiveAvkonQuery == EPhoneContactsListQuery ||
         iActiveAvkonQuery == EPhoneDtmfListQueryDialog )
        {
        buttonId = iStatus.Int();
        }
    else if ( iActiveAvkonQuery == EPhoneGlobalQueryDialog )
        {
        if ( iStatus.Int() == EAknSoftkeyYes )
            {
            if ( iVideoCallSetupFailedQuery )
                {
                buttonId = EPhoneCmdYesVideoFailedNoMemorySwitchToVoice;
                }
            else
                {
                buttonId = EPhoneCmdYesBtDisconnectQuery;
                }
            }
        else if ( iStatus.Int() == EAknSoftkeyNo  )
            {
            if ( iVideoCallSetupFailedQuery )
                {
                buttonId = EPhoneCmdNoVideoFailedNoMemorySwitchToVoice;
                }
            else
                {
                buttonId = EPhoneCmdNoBtDisconnectQuery;
                }
             }
        ClearTimer();
        iVideoCallSetupFailedQuery = EFalse;
        DestroyGlobalQuery();
        }
    else if ( iActiveAvkonQuery == EPhoneGlobalMsgQuery )
        {
        buttonId = iQueryParam.CbaCommandMapping( iStatus.Int() );
        if ( iTransferAcceptanceQuery )
            {
            iTransferAcceptanceQuery = EFalse;
            }

        ClearTimer();
        DestroyGlobalMsgQuery();
        }
    else if ( iActiveAvkonQuery == EPhoneGlobalWaitNote )
        {
        // If note is destroyed through DestroyGlobalQuery() then KErrCancel
        // is not mapped to any command.
        if ( iStatus.Int() == KErrCancel &&
             !iGlobalWaitNote )
            {
            ClearTimer();
            return;
            }
        else
            {
            buttonId = iQueryParam.CbaCommandMapping( iStatus.Int() );
            }

        ClearTimer();
        DestroyGlobalWaitNote();
        }
    // Forward pressed CBA to the framework so that we can handle it
    // in our active state
    static_cast<MEikCommandObserver*>( iEikEnv.EikAppUi() )
	    ->ProcessCommandL( buttonId );
    }

// ---------------------------------------------------------
// CPhoneQueryController::RunError
// ---------------------------------------------------------
//
TInt CPhoneQueryController::RunError( TInt aError )
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneQueryController::RunError( ) ");

    if ( aError )
        {
        __PHONELOG1( EBasic, EPhoneUIView,
        "CPhoneQueryController::RunError: error: %d",
        aError );
        }
    return aError;
    }

// ---------------------------------------------------------
// CPhoneQueryController::DoCancel
// ---------------------------------------------------------
//
void CPhoneQueryController::DoCancel()
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneQueryController::DoCancel( ) ");

    if ( iSimSwitchRestartWaitNote )
        {
        TRAP_IGNORE( iSimSwitchRestartWaitNote->CancelNoteL( KErrCancel ) );

        delete iSimSwitchRestartWaitNote;
        iSimSwitchRestartWaitNote = NULL;
        TRAP_IGNORE( static_cast<MEikCommandObserver*>( iEikEnv.EikAppUi() )
            ->ProcessCommandL( EPhoneCmdRestartPhone ) );
        return;
        }
    
    if ( iActiveAvkonQuery == EPhoneGlobalQueryDialog )
        {
        ClearTimer();
        DestroyGlobalQuery();
        if ( iVideoCallSetupFailedQuery )
            {
            iVideoCallSetupFailedQuery = EFalse;
            TRAP_IGNORE(
                static_cast<MEikCommandObserver*>( iEikEnv.EikAppUi() )
	                ->ProcessCommandL(
	                        EPhoneCmdNoVideoFailedNoMemorySwitchToVoice ) )
            }
        else if ( iQueryParam.QueryCommandObserver() )
            {
            // Let observer handle command.
            TRAP_IGNORE( iQueryParam.QueryCommandObserver()->ProcessCommandL(
                    EAknSoftkeyCancel ) );
            }
        }
    if ( EPhoneGlobalMsgQuery == iActiveAvkonQuery )
        {
        ClearTimer();
        DestroyGlobalMsgQuery();

        TInt buttonId = 0;
        TInt result = iQueryParam.GetCustomCommandForTimeOut( buttonId );
        if ( KErrNone == result && iTransferAcceptanceQuery )
            {
            // timeout equals reject of unattended transfer request
            iTransferAcceptanceQuery = EFalse;
            TRAP_IGNORE(
                static_cast<MEikCommandObserver*>( iEikEnv.EikAppUi() )
                    ->ProcessCommandL( buttonId ) )
            }
        }

    if ( EPhoneGlobalWaitNote == iActiveAvkonQuery )
        {
        ClearTimer();
        TInt buttonId = 0;
        TInt result = iQueryParam.GetCustomCommandForTimeOut( buttonId );
        if ( KErrNone == result && iGlobalWaitNote )
            {
            DestroyGlobalWaitNote();

            TRAP_IGNORE(
                static_cast<MEikCommandObserver*>( iEikEnv.EikAppUi() )
                    ->ProcessCommandL( buttonId ) )
            }
        }
    }

// ---------------------------------------------------------
// CPhoneQueryController::IsQueryActive
// ---------------------------------------------------------
//
void CPhoneQueryController::IsQueryActive( TPhoneCommandParam* aCommandParam )
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneQueryController::IsQueryActive() ");
    TPhoneCmdParamBoolean* booleanParam =
        booleanParam = static_cast<TPhoneCmdParamBoolean*>( aCommandParam );

    if ( iQuery ||
         iTextQuery ||
         iGlobalConfirmationQuery ||
         !iQueryDeleted )
        {
        booleanParam->SetBoolean( ETrue );
        }
    else
        {
        booleanParam->SetBoolean( EFalse );
        }
    }

// ---------------------------------------------------------
// CPhoneQueryController::IsDTMFQueryVisible
// ---------------------------------------------------------
//
TBool CPhoneQueryController::IsDTMFQueryVisible()
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneQueryController::IsDTMFQueryVisible() ");
    TBool dtmfVisible(EFalse);
    if( ( iTextQuery && iQueryParam.QueryType() == EPhoneDtmfTextQuery ) ||
        ( iDtmfNumberListQuery && iQueryParam.QueryType() == EPhoneDtmfListQueryDialog ) )
        {
        dtmfVisible = ETrue;
        }
    __PHONELOG1( EBasic, EPhoneUIView,
            "CPhoneQueryController::IsDTMFQueryVisible :%d",
            dtmfVisible );
    return dtmfVisible;
    }

// ---------------------------------------------------------
// CPhoneQueryController::SetListQueryStringL
// ---------------------------------------------------------
//
void CPhoneQueryController::SetListQueryStringL( TPhoneCommandParam* aCommandParam )
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneQueryController::SetListQueryStringL() ");
    //Create new object if needed
    if ( !iDtmfNumberListQuery )
        {
        iDtmfNumberListQuery = CPhoneDtmfNumberListQuery::NewL(
        static_cast<MEikCommandObserver&>( *iEikEnv.EikAppUi() ),
                                           *this,
                                           iResultBuffer );
        }
    TPhoneCmdParamString* stringParam =
        static_cast<TPhoneCmdParamString*>( aCommandParam );
    iDtmfNumberListQuery->AppendStringToArrayL( *stringParam->String() );
    }

// -----------------------------------------------------------------------------
// CPhoneQueryController::HandleTimeOutL
// -----------------------------------------------------------------------------
//
void CPhoneQueryController::HandleTimeOutL()
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneQueryController::HandleTimeOutL()");
    Cancel();
    }

// -----------------------------------------------------------------------------
// CPhoneQueryController::ClearTimer
// -----------------------------------------------------------------------------
//
void CPhoneQueryController::ClearTimer()
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneQueryController::ClearTimer()");
    if ( iTimer )
        {
        if ( iTimer->IsActive() )
            {
            iTimer->Cancel();
            }
        delete iTimer;
        iTimer = NULL;
        }
    }

// ---------------------------------------------------------
// CPhoneQueryController::CreateGlobalMsgQueryL
// ---------------------------------------------------------
//
void CPhoneQueryController::CreateGlobalMsgQueryL(
        const TPhoneCmdParamQuery& aParams )
    {
    iQueryNoteText.Close();
    iQueryNoteText.Create( *aParams.DataText() );
    iQueryPrompt.Close();
    iQueryPrompt.Create( aParams.QueryPrompt() );

    // use timeout if defined
    const TInt KSecToUsFactor = 1000000;
    TTimeIntervalMicroSeconds32 timeout = aParams.TimeOut() * KSecToUsFactor;
    if ( 0 < timeout.Int() && !iTimer )
        {
        iTimer = CPhoneTimer::NewL();
        iTimer->After( timeout, this );
        }

    DestroyGlobalMsgQuery();
    iGlobalMsgQuery = CAknGlobalMsgQuery::NewL();

    const TInt KNoImage     = 0;
    const TInt KNoImageMask = -1;

    iGlobalMsgQuery->ShowMsgQueryL( iStatus, iQueryNoteText, aParams.DefaultCba(),
            iQueryPrompt, KNullDesC(), KNoImage, KNoImageMask,
        static_cast<CAknQueryDialog::TTone>( aParams.Tone() ) );
    SetActive();
    }

// ---------------------------------------------------------
// CPhoneQueryController::DestroyGlobalMsgQuery
// ---------------------------------------------------------
//
void CPhoneQueryController::DestroyGlobalMsgQuery()
    {
    if ( iGlobalMsgQuery )
        {
        iGlobalMsgQuery->CancelMsgQuery();

        delete iGlobalMsgQuery;
        iGlobalMsgQuery = NULL;
        }
    }

// ---------------------------------------------------------
// CPhoneQueryController::CreateGlobalWaitNote
// ---------------------------------------------------------
//
void CPhoneQueryController::CreateGlobalWaitNoteL(
        const TPhoneCmdParamQuery& aParams )
    {
    __LOGMETHODSTARTEND(EPhoneUIView,
        "CPhoneQueryController::CreateGlobalWaitNote()");
    if ( aParams.DataText()->Length() > 0 )
        {
        iWaitNoteText.Close();
        iWaitNoteText.Create( *aParams.DataText() );
        // use timeout if defined
        const TInt KSecToUsFactor = 1000000;
        TTimeIntervalMicroSeconds32 timeout = aParams.TimeOut() * KSecToUsFactor;
        if ( 0 < timeout.Int() && !iTimer )
            {
            iTimer = CPhoneTimer::NewL();
            iTimer->After( timeout, this );
            }

        DestroyGlobalWaitNote();
        iGlobalWaitNote = CAknGlobalNote::NewL();
        iGlobalWaitNote->SetSoftkeys( aParams.DefaultCba() );
        // Show the global note
        iGlobalWaitNote->ShowNoteL( iStatus, EAknGlobalWaitNote, *aParams.DataText()  );

        SetActive();
        }
    }

// ---------------------------------------------------------
// CPhoneQueryController::DestroyGlobalWaitNote
// ---------------------------------------------------------
//
void CPhoneQueryController::DestroyGlobalWaitNote()
    {
    __LOGMETHODSTARTEND(EPhoneUIView,
          "CPhoneQueryController::DestroyGlobalWaitNote()");
    if ( iGlobalWaitNote )
        {
        ClearTimer();
        TRAP_IGNORE( iGlobalWaitNote->CancelNoteL( KErrCancel ) );

        delete iGlobalWaitNote;
        iGlobalWaitNote = NULL;
        }
    iWaitNoteText.Close();
    }

// ---------------------------------------------------------
// CPhoneQueryController::ProcessCommand
// ---------------------------------------------------------

void CPhoneQueryController::ProcessCommandL( TInt aCommandId )
    {
	__LOGMETHODSTARTEND(EPhoneUIView, "CPhoneQueryController::ProcessCommandL()");

	iCommandId = aCommandId;

    if ( iTextQuery )
        { // Store text for next content getting
        iQueryPreviousText.Close();
        iQueryPreviousText.Create( KPhoneMaxCharsInNote );
        iTextQuery->GetContent( iQueryPreviousText );
        }

    iCommandAsync = CIdle::NewL( EPriorityNormal );
    iCommandAsync->Start( TCallBack( ProcessCommandCallBack, this ) );
    }

// ---------------------------------------------------------
// CPhoneQueryController::ProcessCommandCallBack
// ---------------------------------------------------------
//
TInt CPhoneQueryController::ProcessCommandCallBack( TAny* aCPhoneQueryController )
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneQueryController::ProcessCommandCallBack() ");

    CPhoneQueryController* queryController =
        static_cast<CPhoneQueryController*>( aCPhoneQueryController );

    delete queryController->iCommandAsync;
    queryController->iCommandAsync = NULL;

    TRAP_IGNORE( static_cast<MEikCommandObserver*>( queryController->iEikEnv.EikAppUi() )
        ->ProcessCommandL( queryController->iCommandId ) );

    return KErrNone;
    }


// end of file

