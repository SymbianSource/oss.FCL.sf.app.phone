/*
* Copyright (c) 2003-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Implementation of CCoUtlActive.
*
*/



// INCLUDE FILES
#include    "CCoUtlActive.h"
#include    <mmtsy_names.h>
#include    <nifvar.h>
#include    <pcktcs.h>
#include    <AknGlobalConfirmationQuery.h>
#include    <AknUtils.h>
#include    <StringLoader.h>
#include    <ConnectUtilRsc.rsg>
#include    <avkon.hrh>
#include    <e32property.h>
#include    <PSVariables.h>



// CONSTANTS

// Panic category.
_LIT( KCoUtlPanicCategory, "CoUtl" );

// Enumerates panic reasons
enum 
    {
    // Terminate is already called and operation has not been completed yet. 
    // Cancel before calling Terminate.
    ECoUtlPanicTerminateAlreadyActive = 0
    };

// MACROS

#if defined _DEBUG && !defined __WINS__
// Target UDEB environment - debug traces enabled

#define COUTL_RDEBUG(X)             RDebug::Print(X);
#define COUTL_RDEBUG_INT(X,Y)       RDebug::Print(X,Y);

#else           
// Other environments - debug traces disabled

#define COUTL_RDEBUG(X)
#define COUTL_RDEBUG_INT(X,Y)

#endif


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CCoUtlActive::CCoUtlActive
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CCoUtlActive::CCoUtlActive()
    : CActive( CActive::EPriorityStandard )
    {
    CActiveScheduler::Add( this );
    }

// -----------------------------------------------------------------------------
// CCoUtlActive::~CCoUtlActive
// Destructor.
// -----------------------------------------------------------------------------
//
/***************************************************** 
*   Series 60 Customer / ETel 
*   Series 60  ETel API 
*****************************************************/
CCoUtlActive::~CCoUtlActive()
    {
    Cancel();

    if ( iServer.Handle() )
        {
        // Error is ignored - loading might have failed.
        iServer.UnloadPhoneModule( KMmTsyModuleName ); 
        }

    delete iConfirmationQuery;
    delete iConfirmationText;

    iConnections.Close();
    iPacketService.Close();
    iMobilePhone.Close();
    iServer.Close();
    iConnectionMonitor.CancelNotifications();
    iConnectionMonitor.Close();
    }

// -----------------------------------------------------------------------------
// CCoUtlActive::CurrentState
// -----------------------------------------------------------------------------
//
CCoUtlActive::TState CCoUtlActive::CurrentState() const
    {
    return iState;
    }

// -----------------------------------------------------------------------------
// CCoUtlActive::Start
// -----------------------------------------------------------------------------
//
void CCoUtlActive::Start( TRequestStatus& aStatus )
    {
    if ( !IsActive() )
        {
        aStatus = KRequestPending;
        iRequestStatus = &aStatus;
        GoToStateAndComplete( EStarted );
        }
    else
        {
        User::Panic( KCoUtlPanicCategory, ECoUtlPanicTerminateAlreadyActive );
        }
    }

// -----------------------------------------------------------------------------
// CCoUtlActive::RunL
// -----------------------------------------------------------------------------
//
void CCoUtlActive::RunL()
    {
    COUTL_RDEBUG_INT( _L("CCoUtlActive::RunL - State %d: "), iState )
    COUTL_RDEBUG_INT( _L("CCoUtlActive::RunL - Status %d: "), iStatus.Int() )

    // Clean up parameters that might require uninitialisation.
    delete iConfirmationQuery;
    iConfirmationQuery = NULL;

    delete iConfirmationText;
    iConfirmationText = NULL;

    // Leave in case of error.
    User::LeaveIfError( iStatus.Int() );

    // Handle state transitions.
    switch ( iState )
        {
        case EStarted:
            iConnections.Reset();
            GetAttachStatusL();
            break;

        case ECheckIfAttached:
            if ( IsAttached() )
                {
                GetConnectionCountL();
                }
            break;
        
        case EGetConnectionCount:
            iConnectionsIndex = 0;
            CheckConnectionsL();
            break;

        case EGoThroughConnections:
            if ( iConnectionsIndex < iConnections.Count() )
                {
                GoThroughConnectionsGetBearerL();
                }
            else
                {
                GetNetworkModeL();
                }
            break;
        
        case EGoThroughConnectionsGetBearer:
            if ( AcceptConnectionBearer() )
                {
                GoThroughConnectionsGetStatusL();
                }
            else
                {
                // Not accepted - delete and continue to the next.
                iConnections.Remove( iConnectionsIndex );
                GoToStateAndComplete( EGoThroughConnections );
                }
            break;

        case EGoThroughConnectionsGetStatus:
            if ( !IsConnectionActive() )
                {
                // Not accepted - delete and continue to the next.
                iConnections.Remove( iConnectionsIndex );
                }
            else
                {
                // Accepted, go to next one.
                iConnectionsIndex++;
                }

            GoToStateAndComplete( EGoThroughConnections );
            break;

        case ECheckIfNetworkModeIII:
            if ( IsNetworkModeIII() )
                {
                StopConnectionsAndDetachL();
                }
            else
                {
                GoToStateAndComplete( ECheckConnectionCount );
                }
            break;

        case EStopConnectionsAndDetach:
            DetachL();
            break;

        case ECheckConnectionCount:
            if ( !IsConnections() )
                {
                GetAttachModeAndDetachIfRequiredL();
                }
            else 
                {
                ConfirmAllConnectionsTerminationL();
                }
            break;

        case EStopConnectionsAndCheckDetachRequired:
            GetAttachModeAndDetachIfRequiredL();
            break;

        case ECheckDetachRequired:
            if ( IsAttachModeOnDemand() )
                {
                DetachL();
                }
            break;

        case EGetBearerThenNameThenConfirmTermination:
            GetNameThenConfirmTerminationL();
            break;

        case EGetNameThenConfirmTermination:
            ConfirmTerminationL( iConnectionName );
            break;

        case EConfirmAllConnectionsTermination:
            if ( IsConfirmed() )
                {
                StopAllConnectionsAndDetachIfRequiredL();
                }
            break;

        case EDetach:
        case EIdle:
        default:
            break;
        }

    if ( !IsActive() )
        {
        // Operation has been finished.
        iState = EIdle;
        CompleteRequest( KErrNone );
        }

    COUTL_RDEBUG_INT( _L("CCoUtlActive::RunL finished - %d: "), iState )
    }

// -----------------------------------------------------------------------------
// CCoUtlActive::DoCancel
// -----------------------------------------------------------------------------
//
/***************************************************** 
*   Series 60 Customer / ETel 
*   Series 60  ETel API 
*****************************************************/
void CCoUtlActive::DoCancel()
    {
    COUTL_RDEBUG_INT( _L("CCoUtlActive::DoCancel - %d: "), iState )
    // Operation is canceled - cancel ongoing operation and 
    // complete original request with the KErrCancel error code.

    TInt packetServiceCancel = 0;
    TInt connectionMonitorCancel = 0;

    switch ( iState )
        {
        case ECheckIfNetworkModeIII:
            packetServiceCancel = EPacketGetMSClass;
            break;

        case EGetConnectionCount:
            connectionMonitorCancel = EConnMonGetConnectionCount;
            break;

        case EGoThroughConnectionsGetStatus:
        case EGoThroughConnectionsGetBearer:
        case EGetBearerThenNameThenConfirmTermination:
            connectionMonitorCancel = EConnMonGetIntAttribute;
            break;       

        case ECheckDetachRequired:
            packetServiceCancel = EPacketGetAttachMode;
            break;
        
        case EGetNameThenConfirmTermination:
            connectionMonitorCancel = EConnMonGetStringAttribute;
            break;

        case EConfirmAllConnectionsTermination:
            if ( iConfirmationQuery )
                {
                iConfirmationQuery->CancelConfirmationQuery();
                delete iConfirmationQuery;
                iConfirmationQuery = NULL;

                delete iConfirmationText;
                iConfirmationText = NULL;
                }
            break;

        case EDetach:
            packetServiceCancel = EPacketDetach;
            break;

        case EIdle:
        case EStarted:
        case ECheckIfAttached:     
        case EStopConnectionsAndDetach:
        case EGoThroughConnections:
        case ECheckConnectionCount:
        case EStopConnectionsAndCheckDetachRequired:
        default:
            break;
        }

    if ( packetServiceCancel && iPacketService.SubSessionHandle() )
        {
        iPacketService.CancelAsyncRequest( packetServiceCancel );
        }
    if ( connectionMonitorCancel && iConnectionMonitor.Handle() )
        {
        iConnectionMonitor.CancelAsyncRequest( connectionMonitorCancel );
        }

    CompleteRequest( KErrCancel );
    GoToState( EIdle );
    
    COUTL_RDEBUG( _L("CCoUtlActive::DoCancel finished") )
    }

// -----------------------------------------------------------------------------
// CCoUtlActive::RunError
// -----------------------------------------------------------------------------
//
TInt CCoUtlActive::RunError( TInt aError )
    {
    COUTL_RDEBUG_INT( _L("CCoUtlActive::RunError - %d: "), aError )

    // Handles exceptions. Either operation has completed with aError code,
    // or then exception has occured while performing RunL when state has
    // been specified.
    // However, RunL has been implemented in such a way that once asynchronous
    // request has been made, then exception is not raised in that RunL.

    switch ( iState )
        {
        case EStopConnectionsAndDetach:
            // Try to move on.
            GoToStateAndComplete( EDetach );
            break;
            
        case EGoThroughConnectionsGetBearer:
        case EGoThroughConnectionsGetStatus:
            if ( aError == KErrNotFound && 
                ( iConnectionsIndex < iConnections.Count() ) )
                {
                iConnections.Remove( iConnectionsIndex );
                GoToStateAndComplete( EGoThroughConnections );
                break;
                }
            //lint -fallthrough
        case EStarted: 
        case ECheckIfAttached:
        case ECheckIfNetworkModeIII:
        case EGetConnectionCount:
        case EGoThroughConnections:
        case ECheckConnectionCount:
        case EGetBearerThenNameThenConfirmTermination:
        case EGetNameThenConfirmTermination:
        case EConfirmAllConnectionsTermination:
        case EStopConnectionsAndCheckDetachRequired:
        case EDetach:
        case ECheckDetachRequired:
        case EIdle:
        default:
            CompleteRequest( aError );
            GoToState( EIdle );
            break;
        }

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CCoUtlActive::GetAttachStatusL
// -----------------------------------------------------------------------------
//
void CCoUtlActive::GetAttachStatusL()
    {
    TInt status = KErrNone;
    TInt statusWcdma = KErrNone;

    //Fetch attach status from RProperty.
    TInt error = RProperty::Get( KUidSystemCategory, KPSUidGprsStatusValue, status );
    if ( error != KErrNone )
        {
        // On error, assume attached - connection monitor will inform
        // detailed information.
        status = EPSGprsAttach;
        }
    error = RProperty::Get( KUidSystemCategory, KPSUidWcdmaStatusValue, statusWcdma );
    if ( error != KErrNone )
        {
        // On error, assume attached - connection monitor will inform
        // detailed information.
        statusWcdma = EPSWcdmaAttach;
        }
    iServiceStatus = status;
    iWcdmaConnectionStatus = statusWcdma;
    GoToStateAndComplete( ECheckIfAttached );
    }

// -----------------------------------------------------------------------------
// CCoUtlActive::GetNetworkModeL
// -----------------------------------------------------------------------------
//
void CCoUtlActive::GetNetworkModeL()
    {
    PacketServiceL().GetMSClass( iStatus, iCurrentClass, iMaxClass );
    SetActive();
    GoToState( ECheckIfNetworkModeIII );
    }

// -----------------------------------------------------------------------------
// CCoUtlActive::StopConnectionsAndDetachL
// -----------------------------------------------------------------------------
//
void CCoUtlActive::StopConnectionsAndDetachL()
    {
    TInt err = DoStopAllConnectionsL();
    GoToStateAndComplete( 
        EStopConnectionsAndDetach,
        err );
    }

// -----------------------------------------------------------------------------
// CCoUtlActive::GetConnectionCountL
// -----------------------------------------------------------------------------
//
void CCoUtlActive::GetConnectionCountL()
    {
    GoToState( EGetConnectionCount );
    ConnectionMonitorL().GetConnectionCount( iConnectionCount, iStatus );
    SetActive();
    }

// -----------------------------------------------------------------------------
// CCoUtlActive::CheckConnectionsL
// -----------------------------------------------------------------------------
//
void CCoUtlActive::CheckConnectionsL()
    {
    RConnectionMonitor& monitor = ConnectionMonitorL();
    iConnections.Reset();

    // Go through all connections
    // Note indexing from 1, 2, 3, .., iConnectionCount.
    for ( TUint index = 1; index <= iConnectionCount; index++ )
        {
        TConnectionId id;
        TUint subConnectionCount = 0;
        
        TInt err = 
            monitor.GetConnectionInfo( 
                index, 
                id.iConnectionId, 
                subConnectionCount );

        // SubConnectionIds are not used.
        id.iSubConnectionId = 0;

        if ( err != KErrNotFound )
            {
            // the KErrNotFound error code indicates that connection has
            // already been dropped, so those indices are simply ignored.
            User::LeaveIfError( err );
            User::LeaveIfError( iConnections.Append( id ) );
            }
        }

    GoToStateAndComplete( EGoThroughConnections );
    }

// -----------------------------------------------------------------------------
// CCoUtlActive::GoThroughConnectionsGetBearerL
// -----------------------------------------------------------------------------
//
void CCoUtlActive::GoThroughConnectionsGetBearerL()
    {
    TConnectionId& current = iConnections[ iConnectionsIndex ];
    ConnectionMonitorL().GetIntAttribute( 
        current.iConnectionId,
        current.iSubConnectionId,
        KBearer,
        iConnectionBearer,
        iStatus );
    SetActive();
    GoToState( EGoThroughConnectionsGetBearer );
    }

// -----------------------------------------------------------------------------
// CCoUtlActive::GoThroughConnectionsGetStatusL
// -----------------------------------------------------------------------------
//
void CCoUtlActive::GoThroughConnectionsGetStatusL()
    {
    TConnectionId& current = iConnections[ iConnectionsIndex ];
    ConnectionMonitorL().GetIntAttribute( 
        current.iConnectionId,
        current.iSubConnectionId,
        KConnectionStatus,
        iConnectionStatus,
        iStatus );
    SetActive();
    GoToState( EGoThroughConnectionsGetStatus );
    }

// -----------------------------------------------------------------------------
// CCoUtlActive::GetAttachModeAndDetachIfRequiredL
// -----------------------------------------------------------------------------
//
void CCoUtlActive::GetAttachModeAndDetachIfRequiredL()
    {
    PacketServiceL().GetAttachMode( iStatus, iAttachMode );
    SetActive();
    GoToState( ECheckDetachRequired );
    }

// -----------------------------------------------------------------------------
// CCoUtlActive::ConfirmAllConnectionsTerminationL
// -----------------------------------------------------------------------------
//
void CCoUtlActive::ConfirmAllConnectionsTerminationL()
    {
    if ( iConnections.Count() == 1 ) // Exactly one connection
        {
        GetBearerThenNameThenConfirmTerminationL();
        }
    else
        {
        ConfirmTerminationL( iConnections.Count() );
        }
    }

// -----------------------------------------------------------------------------
// CCoUtlActive::ConfirmTerminationL
// -----------------------------------------------------------------------------
//
void CCoUtlActive::ConfirmTerminationL( TInt aAmount )
    {
    HBufC* text =
        StringLoader::LoadLC( 
            R_COUTL_CONFIRM_CONNECTIONS,
            aAmount );

    TPtr ptr = text->Des();
    
    // Convert digits in the string to correct digit type.
    AknTextUtils::DisplayTextLanguageSpecificNumberConversion( ptr );

    DoConfirmTerminationL( *text );
    CleanupStack::Pop( text );
    iConfirmationText = text;
    }

// -----------------------------------------------------------------------------
// CCoUtlActive::ConfirmTerminationL
// -----------------------------------------------------------------------------
//
void CCoUtlActive::ConfirmTerminationL( const TDesC& aName )
    {
    HBufC* text = 
        StringLoader::LoadLC( 
            R_COUTL_CONFIRM_ONE_CONNECTION,
            aName );

    DoConfirmTerminationL( *text );
    CleanupStack::Pop( text );
    iConfirmationText = text;
    }

// -----------------------------------------------------------------------------
// CCoUtlActive::DoConfirmTerminationL
// -----------------------------------------------------------------------------
//
void CCoUtlActive::DoConfirmTerminationL( const TDesC& aText )
    {
    CAknGlobalConfirmationQuery* confirmationQuery =
        CAknGlobalConfirmationQuery::NewLC();
    confirmationQuery->ShowConfirmationQueryL(
        iStatus,
        aText );
    CleanupStack::Pop( confirmationQuery );
    
    SetActive();
    iConfirmationQuery = confirmationQuery;
    GoToState( EConfirmAllConnectionsTermination );
    }

// -----------------------------------------------------------------------------
// CCoUtlActive::GetBearerThenNameThenConfirmTerminationL
// -----------------------------------------------------------------------------
//
void CCoUtlActive::GetBearerThenNameThenConfirmTerminationL()
    {
    TConnectionId& id = iConnections[ 0 ]; // first
    ConnectionMonitorL().GetIntAttribute( 
        id.iConnectionId,
        id.iSubConnectionId,
        KBearer,
        iConnectionBearer,
        iStatus );
    SetActive();
    GoToState( EGetBearerThenNameThenConfirmTermination );
    }

// -----------------------------------------------------------------------------
// CCoUtlActive::GetNameThenConfirmTerminationL
// -----------------------------------------------------------------------------
//
void CCoUtlActive::GetNameThenConfirmTerminationL()
    {
    TConnectionId& id = iConnections[ 0 ]; // first

    TUint attribute = KIAPName;
    if ( iConnectionBearer == EBearerExternalGPRS || 
         iConnectionBearer == EBearerExternalEdgeGPRS ||
         iConnectionBearer == EBearerExternalWCDMA )
        {
        attribute = KAccessPointName;
        }

    ConnectionMonitorL().GetStringAttribute(
        id.iConnectionId,
        id.iSubConnectionId,
        attribute,
        iConnectionName,
        iStatus );
    SetActive();
    GoToState( EGetNameThenConfirmTermination );
    }

// -----------------------------------------------------------------------------
// CCoUtlActive::StopAllConnectionsAndDetachIfRequiredL
// -----------------------------------------------------------------------------
//
void CCoUtlActive::StopAllConnectionsAndDetachIfRequiredL()
    {
    TInt err = DoStopAllConnectionsL();
    GoToStateAndComplete( 
        EStopConnectionsAndCheckDetachRequired,
        err );
    }

// -----------------------------------------------------------------------------
// CCoUtlActive::DoStopAllConnectionsL
// -----------------------------------------------------------------------------
//
TInt CCoUtlActive::DoStopAllConnectionsL()
    {
    TInt err = KErrNone;

    for ( TInt index = iConnections.Count() - 1; index >= 0; index-- )
        {
        const TConnectionId& id = iConnections[ index ];

        TInt result = ConnectionMonitorL().SetBoolAttribute(
            id.iConnectionId, id.iSubConnectionId, KConnectionStop, ETrue );
        if ( result == KErrNotFound )
            {
            // the KErrNotFound error code is ignored, because connection
            // may have been ended by another client.
            result = KErrNone;
            }

        // Combine error codes.
        err = Min( err, result );
        }

    return err;
    }

// -----------------------------------------------------------------------------
// CCoUtlActive::DetachL
// -----------------------------------------------------------------------------
//
void CCoUtlActive::DetachL()
    {
    PacketServiceL().Detach( iStatus );
    SetActive();
    GoToState( EDetach );
    }

// -----------------------------------------------------------------------------
// CCoUtlActive::IsAttached
// -----------------------------------------------------------------------------
//
inline TBool CCoUtlActive::IsAttached() const
    {
    return ( 
        ( iServiceStatus != EPSGprsUnattached ) ||
        ( iWcdmaConnectionStatus != EPSWcdmaUnattached ) );
    }

// -----------------------------------------------------------------------------
// CCoUtlActive::IsNetworkModeIII
// -----------------------------------------------------------------------------
//
inline TBool CCoUtlActive::IsNetworkModeIII() const
    {
    return 
        ( iCurrentClass == RPacketService::EMSClassAlternateMode ) || 
        ( iCurrentClass == RPacketService::EMSClassPacketSwitchedOnly ) ||
        ( iCurrentClass == RPacketService::EMSClassCircuitSwitchedOnly );
    }

// -----------------------------------------------------------------------------
// CCoUtlActive::IsConnections
// -----------------------------------------------------------------------------
//
inline TBool CCoUtlActive::IsConnections() const
    {
    return iConnections.Count();
    }

// -----------------------------------------------------------------------------
// CCoUtlActive::AcceptConnectionBearer
// -----------------------------------------------------------------------------
//
inline TBool CCoUtlActive::AcceptConnectionBearer() const
    {
    return ( iConnectionBearer == EBearerGPRS ) ||
           ( iConnectionBearer == EBearerEdgeGPRS ) ||
           ( iConnectionBearer == EBearerWCDMA ) ||
           ( iConnectionBearer == EBearerExternalGPRS ) ||
           ( iConnectionBearer == EBearerExternalEdgeGPRS ) ||
           ( iConnectionBearer == EBearerExternalWCDMA );
    }

// -----------------------------------------------------------------------------
// CCoUtlActive::IsAttachModeOnDemand
// -----------------------------------------------------------------------------
//
inline TBool CCoUtlActive::IsAttachModeOnDemand() const
    {
    return ( iAttachMode == RPacketService::EAttachWhenNeeded );
    }

// -----------------------------------------------------------------------------
// CCoUtlActive::IsConnectionActive
// -----------------------------------------------------------------------------
//
inline TBool CCoUtlActive::IsConnectionActive() const
    {
    return ( iConnectionStatus != KConnectionClosed ) &&
           ( iConnectionStatus != KLinkLayerClosed );
    }

// -----------------------------------------------------------------------------
// CCoUtlActive::Confirmed
// -----------------------------------------------------------------------------
//
inline TBool CCoUtlActive::IsConfirmed() const
    {
    return iStatus.Int() == EAknSoftkeyYes;
    }

// -----------------------------------------------------------------------------
// CCoUtlActive::TelServerL
// -----------------------------------------------------------------------------
//
/*****************************************************
*   Series 60 Customer / TSY
*   Needs customer TSY implementation
*****************************************************/
inline RTelServer& CCoUtlActive::TelServerL()
    {
    if ( !iServer.Handle() )
        {
        // ETel server session will be closed if any of the following operations
        // fail. In this way, the above Handle condition is sufficient; if 
        // session has been opened, then it has been initialised properly as
        // well.
        CleanupClosePushL( iServer ); 
        User::LeaveIfError( iServer.Connect() );
        User::LeaveIfError( 
            iServer.SetExtendedErrorGranularity( 
                RTelServer::EErrorExtended ) );
        User::LeaveIfError( iServer.LoadPhoneModule( KMmTsyModuleName ) );
        CleanupStack::Pop(); // Success
        }

    return iServer;
    }

// -----------------------------------------------------------------------------
// CCoUtlActive::MobilePhoneL
// -----------------------------------------------------------------------------
//
/***************************************************** 
*   Series 60 Customer / ETel 
*   Series 60  ETel API 
*****************************************************/
inline RMobilePhone& CCoUtlActive::MobilePhoneL()
    {
    if ( !iMobilePhone.SubSessionHandle() )
        {
        User::LeaveIfError( iMobilePhone.Open( TelServerL(), KMmTsyPhoneName ) );
        }

    return iMobilePhone;
    }

// -----------------------------------------------------------------------------
// CCoUtlActive::PacketServiceL
// -----------------------------------------------------------------------------
//
/***************************************************** 
*   Series 60 Customer / ETel 
*   Series 60  ETel API 
*****************************************************/
inline RPacketService& CCoUtlActive::PacketServiceL()
    {
    if ( !iPacketService.SubSessionHandle() )
        {
        User::LeaveIfError( iPacketService.Open( MobilePhoneL() ) );
        }

    return iPacketService;
    }

// -----------------------------------------------------------------------------
// CCoUtlActive::ConnectionMonitorL
// -----------------------------------------------------------------------------
//
inline RConnectionMonitor& CCoUtlActive::ConnectionMonitorL()
    {
    if ( !iConnectionMonitor.Handle() )
        {
        User::LeaveIfError( iConnectionMonitor.ConnectL() );
        User::LeaveIfError( iConnectionMonitor.NotifyEventL( *this ) ); 
        }

    return iConnectionMonitor;
    }

// -----------------------------------------------------------------------------
// CCoUtlActive::CompleteRequest
// -----------------------------------------------------------------------------
//
void CCoUtlActive::CompleteRequest( TInt aErrorCode )
    {
    if ( iRequestStatus )
        {
        User::RequestComplete( iRequestStatus, aErrorCode );
        iRequestStatus = NULL;
        }
    }
    
// -----------------------------------------------------------------------------
// CCoUtlActive::EventL
// -----------------------------------------------------------------------------
//    
void CCoUtlActive::EventL( const CConnMonEventBase &aConnMonEvent )
    {
    COUTL_RDEBUG_INT( _L("CCoUtlActive::EventL - Event %d: "), aConnMonEvent.EventType() )
    switch ( aConnMonEvent.EventType() )
		{
        case EConnMonDeleteConnection:
            {
            if ( IsActive() && iConfirmationQuery )
                {
                COUTL_RDEBUG( _L("CCoUtlActive::EventL - Cancel()") )
                Cancel();
                }
		    }
			break;
		default:
			break;
		} 
    }

// -----------------------------------------------------------------------------
// CCoUtlActive::GoToState
// -----------------------------------------------------------------------------
//
inline void CCoUtlActive::GoToState( TState aNewState )
    {
    iState = aNewState;
    }

// -----------------------------------------------------------------------------
// CCoUtlActive::GoToStateAndComplete
// -----------------------------------------------------------------------------
//
inline void CCoUtlActive::GoToStateAndComplete( TState aNewState, TInt aError )
    {
    GoToState( aNewState );
    TRequestStatus* status = &iStatus;
    User::RequestComplete( status, aError );
    SetActive();
    }

//  End of File  
