/** Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of the License "Symbian Foundation License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.symbianfoundation.org/legal/sfl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:
*
*/
#include <smcmockclassincludes.h>
#include <TelephonyAudioRoutingSession.h>
#include <MTelephonyAudioRoutingObserver.h>

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// RTelephonyAudioRoutingSession::RTelephonyAudioRoutingSession
// -----------------------------------------------------------------------------
//
RTelephonyAudioRoutingSession::RTelephonyAudioRoutingSession(  )
    {
    
    }


// -----------------------------------------------------------------------------
// RTelephonyAudioRoutingSession::ConnectL
// -----------------------------------------------------------------------------
//
TInt RTelephonyAudioRoutingSession::ConnectL( 
        CTelephonyAudioRouting & aAudioRouting,
        MTelephonyAudioRoutingObserver & aObserver )
    {
    SMC_MOCK_METHOD2( TInt, CTelephonyAudioRouting &, aAudioRouting, 
        MTelephonyAudioRoutingObserver &, aObserver )
    }


// -----------------------------------------------------------------------------
// RTelephonyAudioRoutingSession::Close
// -----------------------------------------------------------------------------
//
void RTelephonyAudioRoutingSession::Close(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// RTelephonyAudioRoutingSession::Version
// -----------------------------------------------------------------------------
//
TVersion RTelephonyAudioRoutingSession::Version(  ) const
    {
    SMC_MOCK_METHOD0( TVersion )
    }


// -----------------------------------------------------------------------------
// RTelephonyAudioRoutingSession::SetOutput
// -----------------------------------------------------------------------------
//
void RTelephonyAudioRoutingSession::SetOutput( 
        CTelephonyAudioRouting::TAudioOutput aOutput,
        TBool aShowNote )
    {
    SMC_MOCK_METHOD2( void, CTelephonyAudioRouting::TAudioOutput, aOutput, 
        TBool, aShowNote )
    }


// -----------------------------------------------------------------------------
// RTelephonyAudioRoutingSession::GetAvailableOutputsL
// -----------------------------------------------------------------------------
//
void RTelephonyAudioRoutingSession::GetAvailableOutputsL(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// RTelephonyAudioRoutingSession::CancelRequest
// -----------------------------------------------------------------------------
//
void RTelephonyAudioRoutingSession::CancelRequest( 
        TTelAudRtngServRqst aRequest )
    {
    SMC_MOCK_METHOD1( void, TTelAudRtngServRqst, aRequest )
    }


// -----------------------------------------------------------------------------
// RTelephonyAudioRoutingSession::NotifyIfOutputChanged
// -----------------------------------------------------------------------------
//
void RTelephonyAudioRoutingSession::NotifyIfOutputChanged(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// RTelephonyAudioRoutingSession::NotifyIfAvailableOutputsChanged
// -----------------------------------------------------------------------------
//
void RTelephonyAudioRoutingSession::NotifyIfAvailableOutputsChanged(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// RTelephonyAudioRoutingSession::CurrentAudioOutputPkg
// -----------------------------------------------------------------------------
//
TPckgBuf <CTelephonyAudioRouting::TAudioOutput > & RTelephonyAudioRoutingSession::CurrentAudioOutputPkg(  )
    {
    SMC_MOCK_METHOD0( TPckgBuf <CTelephonyAudioRouting::TAudioOutput > & )
    }


// -----------------------------------------------------------------------------
// RTelephonyAudioRoutingSession::PreviousAudioOutputPkg
// -----------------------------------------------------------------------------
//
TPckgBuf <CTelephonyAudioRouting::TAudioOutput > & RTelephonyAudioRoutingSession::PreviousAudioOutputPkg(  )
    {
    SMC_MOCK_METHOD0( TPckgBuf <CTelephonyAudioRouting::TAudioOutput > & )
    }


// -----------------------------------------------------------------------------
// RTelephonyAudioRoutingSession::ErrPkg
// -----------------------------------------------------------------------------
//
TPckgBuf <TInt > & RTelephonyAudioRoutingSession::ErrPkg(  )
    {
    SMC_MOCK_METHOD0( TPckgBuf <TInt > & )
    }


// -----------------------------------------------------------------------------
// RTelephonyAudioRoutingSession::AvailableOutputs
// -----------------------------------------------------------------------------
//
RArray <CTelephonyAudioRouting::TAudioOutput > & RTelephonyAudioRoutingSession::AvailableOutputs(  )
    {
    SMC_MOCK_METHOD0( RArray <CTelephonyAudioRouting::TAudioOutput > & )
    }


// -----------------------------------------------------------------------------
// RTelephonyAudioRoutingSession::SetShowNoteMode
// -----------------------------------------------------------------------------
//
void RTelephonyAudioRoutingSession::SetShowNoteMode(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// RTelephonyAudioRoutingSession::ShowNoteMode
// -----------------------------------------------------------------------------
//
TPckgBuf <TBool > & RTelephonyAudioRoutingSession::ShowNoteMode(  )
    {
    SMC_MOCK_METHOD0( TPckgBuf <TBool > & )
    }


// -----------------------------------------------------------------------------
// RTelephonyAudioRoutingSession::ResetShowNoteMode
// -----------------------------------------------------------------------------
//
void RTelephonyAudioRoutingSession::ResetShowNoteMode(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// RTelephonyAudioRoutingSession::OkToGetShowNoteMode
// -----------------------------------------------------------------------------
//
TBool RTelephonyAudioRoutingSession::OkToGetShowNoteMode(  )
    {
    SMC_MOCK_METHOD0( TBool )
    }


// -----------------------------------------------------------------------------
// RTelephonyAudioRoutingSession::GetDefaultValuesL
// -----------------------------------------------------------------------------
//
void RTelephonyAudioRoutingSession::GetDefaultValuesL(  )
    {
    SMC_MOCK_METHOD0( void )
    }


