/*
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: CPhoneReconnectQuery implementation.
*
*/


// INCLUDE FILES
// <-- QT PHONE START -->
//#include <mcauireconnectqueryobserver.h>
// <-- QT PHONE END -->
#include <mpeengineinfo.h>

#include "cphonereconnectquery.h"
#include "phoneconstants.h"
#include "phonelogger.h"
#include "phonerssbase.h"
#include "cphonetimer.h"
#include "cphonestatehandle.h"
#include "tphonecmdparamboolean.h"
#include "tphonecmdparaminteger.h"
// <-- QT PHONE START -->
//#include "tphonecmdparammessageeditor.h"
//#include "tphonecmdparamreconnectquery.h"
// <-- QT PHONE END -->
#include "cphonemainresourceresolver.h"

// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CPhoneReconnectQuery::CPhoneReconnectQuery
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPhoneReconnectQuery::CPhoneReconnectQuery() : CCoeStatic( KUidReconnectQuerySingleton ),
    iReconQueryTimer ( NULL ),
    iIncludeVideo( EFalse )
    {
    }

// Destructor
CPhoneReconnectQuery::~CPhoneReconnectQuery()
    {
    CancelReconnectQueryTimer();
    }

// ---------------------------------------------------------
// CPhoneReconnectQuery::InstanceL
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C CPhoneReconnectQuery* CPhoneReconnectQuery::InstanceL()
    {
    __LOGMETHODSTARTEND( EPhoneControl, "CPhoneReconnectQuery::InstanceL( ) ");
    CPhoneReconnectQuery* instance = static_cast<CPhoneReconnectQuery*>(
        CCoeEnv::Static( KUidReconnectQuerySingleton ) );
    if ( !instance )
        {
        instance = new ( ELeave ) CPhoneReconnectQuery;
        }
    return instance;
    }

// ---------------------------------------------------------
// CPhoneReconnectQuery::CancelReconnectQueryTimer
// ---------------------------------------------------------
//
void CPhoneReconnectQuery::CancelReconnectQueryTimer()
    {
    __LOGMETHODSTARTEND( EPhoneControl, "CPhoneReconnectQuery::CancelReconnectQueryTimer( ) ");
    if( iReconQueryTimer && iReconQueryTimer->IsActive() )
        {
        iReconQueryTimer->CancelTimer();
        }
    delete iReconQueryTimer;
    iReconQueryTimer = NULL;
    }

// -----------------------------------------------------------
// CPhoneReconnectQuery::ShowReconnectQueryL
// -----------------------------------------------------------
//
EXPORT_C void CPhoneReconnectQuery::ShowReconnectQueryL( TBool aShowVideo )
    {
    __LOGMETHODSTARTEND( EPhoneControl, "CPhoneReconnectQuery::ShowReconnectQueryL( ) ");
    iIncludeVideo = aShowVideo;
    if( !iReconQueryTimer )
        {
        iReconQueryTimer = CPhoneTimer::NewL();
        }

    iReconQueryTimer->After( KInformationNoteDisplayTime, this );
    }

// -----------------------------------------------------------
// CPhoneReconnectQuery::HandleTimeOutL
// -----------------------------------------------------------
//
void CPhoneReconnectQuery::HandleTimeOutL()
    {
    __LOGMETHODSTARTEND( EPhoneControl, "CPhoneReconnectQuery::HandleTimeOutL( ) ");
// <-- QT PHONE START -->
/* TPhoneCmdParamReconnectQuery reconQueryParam( *this );
    // Recipient number to CallUIs reconnect query in case message selected
    TPtrC phoneNumber( CPhoneStateHandle::Instance()->StateMachine()->
        PhoneEngineInfo()->PhoneNumber() );
    reconQueryParam.SetPhoneNumber( phoneNumber );
    reconQueryParam.SetIncludeVideoOption( iIncludeVideo );
    // Display reconnect query
    CPhoneStateHandle::Instance()->ViewCommandHandle()->
        ExecuteCommandL( EPhoneViewShowQuery, &reconQueryParam );
    iDisplayingQuery = ETrue;*/
// <-- QT PHONE END -->

    }

// -----------------------------------------------------------
// CPhoneReconnectQuery::OptionSelected
// -----------------------------------------------------------
//
void CPhoneReconnectQuery::OptionSelected(
    MCaUiReconnectQueryObserver::TCaUiReconType aReconType )
    {
    __LOGMETHODSTARTEND( EPhoneControl,
        "CPhoneReconnectQuery::HandleReconConfQueryResultL( ) ");
    __PHONELOG1( EBasic, EPhoneControl,
        "CPhoneReconnectQuery::HandleReconConfQueryResultL aReconType = %d",
        aReconType );

    iDisplayingQuery = EFalse;

    switch( aReconType )
        {
        case ECallUiReconTypeVoice:
            CPhoneStateHandle::Instance()->StateMachine()->PhoneEngine()
                ->EngineInfo()->SetCallTypeCommand( EPECallTypeCSVoice );
            CPhoneStateHandle::Instance()->StateMachine()->PhoneEngine()
                ->HandleMessage( MPEPhoneModel::EPEMessageDial );
            break;

        case ECallUiReconTypeVideo:
            CPhoneStateHandle::Instance()->StateMachine()->PhoneEngine()
                ->EngineInfo()->SetCallTypeCommand( EPECallTypeVideo );
            CPhoneStateHandle::Instance()->StateMachine()->PhoneEngine()
                ->HandleMessage( MPEPhoneModel::EPEMessageDial );
            break;

        case ECallUiReconTypeMsg:
            // CallUi launches message editor
            break;

        default:
            // Do nothing
            break;
        }
    }

// -----------------------------------------------------------
// CPhoneReconnectQuery::ShowNumberEntryL
// -----------------------------------------------------------
//
void CPhoneReconnectQuery::ShowNumberEntryL()
    {
    __LOGMETHODSTARTEND( EPhoneControl, "CPhoneReconnectQuery::ShowNumberEntryL( ) ");
    // Show number entry
    TPhoneCmdParamBoolean booleanParam;
    booleanParam.SetBoolean( ETrue );
    CPhoneStateHandle::Instance()->ViewCommandHandle()->
        ExecuteCommandL( EPhoneViewSetNumberEntryVisible, &booleanParam );

    // Set Number Entry CBA
    TPhoneCmdParamInteger integerParam;
    integerParam.SetInteger( CPhoneMainResourceResolver::Instance()->
        ResolveResourceID( EPhoneNumberAcqCBA ) );
    CPhoneStateHandle::Instance()->ViewCommandHandle()->
        ExecuteCommandL( EPhoneViewUpdateCba, &integerParam );
    }

// -----------------------------------------------------------
// CPhoneReconnectQuery::IsDisplayingQuery
// -----------------------------------------------------------
//
TBool CPhoneReconnectQuery::IsDisplayingQuery()
    {
    return iDisplayingQuery;
    }


// End of File
