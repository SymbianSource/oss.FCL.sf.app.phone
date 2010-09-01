/*
* Copyright (c) 2003-2004,2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Implementation of connection termination plug-in.
*
*/



#ifndef CCOUTLACTIVE_H
#define CCOUTLACTIVE_H

//  INCLUDES
#include    <ccoutlinterface.h>
#include    <etel.h>
#include    <etelmm.h>
#include    <etelpckt.h>
#include    <rconnmon.h>

// FORWARD DECLARATION
class CAknGlobalConfirmationQuery;

// CLASS DECLARATION

/**
*  CCoUtlInterface implementation.
*
*  @since 2.6
*/
class CCoUtlActive 
    : public CActive,
      public MConnectionMonitorObserver
    {
    public:  // Constructors and destructor
        
        /**
        * Constructor.
        */
        CCoUtlActive();
        
        /**
        * Destructor.
        */
        virtual ~CCoUtlActive();

    public: // New functions

        /**
        * Enumerates state of the operation.
        */
        enum TState
            {
            EIdle,
            EStarted,
            ECheckIfAttached,
            ECheckIfNetworkModeIII,
            EStopConnectionsAndDetach,
            EGetConnectionCount,
            EGoThroughConnections,
            EGoThroughConnectionsGetBearer,
            EGoThroughConnectionsGetStatus,
            ECheckConnectionCount,
            ECheckDetachRequired, 
            EStopConnectionsAndCheckDetachRequired, 
            EConfirmAllConnectionsTermination,
            EGetBearerThenNameThenConfirmTermination,
            EGetNameThenConfirmTermination,
            EDetach
            };

        /**
        * Returns current state.
        * @return current state.
        */
        TState CurrentState() const;

        /**
        * Starts termination.
        * @param aStatus request to be completed once finished.
        */
        void Start( TRequestStatus& aStatus );

    public: // Functions from base classes

        /**
        * From CActive, handles completion of current operation.
        */
        virtual void RunL();

        /**
        * From CActive, cancels ongoing asynchronous operation.
        */
        virtual void DoCancel();

        /**
        * From CActive, handles errors from CActive.
        * @param aError error to be handled.
        * @return error to be passed to scheduler.
        */
        virtual TInt RunError( TInt aError );
        
        /**
        * From MConnectionMonitorObserver, receive notifications.
        */
        void EventL( const CConnMonEventBase &aConnMonEvent );
        
    private:

        /**
        * Gets attach status.
        */
        void GetAttachStatusL();

        /**
        * Gets network mode.
        */
        void GetNetworkModeL();

        /**
        * Stop connections and detach.
        */
        void StopConnectionsAndDetachL();

        /**
        * Gets number of connections.
        */
        void GetConnectionCountL();

        /**
        * Goes through connections.
        */
        void CheckConnectionsL();

        /**
        * Goes through connections - get bearer.
        */
        void GoThroughConnectionsGetBearerL();

        /**
        * Goes through connections - get status.
        */
        void GoThroughConnectionsGetStatusL();

        /**
        * Gets attach mode - detach if required, 
        * i.e. attach mode is 'on demand'.
        */
        void GetAttachModeAndDetachIfRequiredL();
        
        /**
        * Confirms all connections termination.
        */
        void ConfirmAllConnectionsTerminationL();

        /**
        * Gets bearer of the connection.
        */
        void GetBearerThenNameThenConfirmTerminationL();

        /**
        * Gets name of connection.
        */
        void GetNameThenConfirmTerminationL();

        /**
        * Confirms termination.
        * @param aAmount amount of connections.
        */
        void ConfirmTerminationL( TInt aAmount );

        /**
        * Confirms termination.
        * @param aName name of connection.
        */
        void ConfirmTerminationL( const TDesC& aName );

        /**
        * Confirms termination.
        * @param aText text shown in query.
        */
        void DoConfirmTerminationL( const TDesC& aText );

        /**
        * Stop all connections and detach if required.
        */
        void StopAllConnectionsAndDetachIfRequiredL();

        /**
        * Stops all connections.
        * @return error code.
        */
        TInt DoStopAllConnectionsL();

        /**
        * Detaches from network.
        */
        void DetachL();

        /**
        * Checks if attached.
        * @return EFalse if not attached. Otherwise ETrue.
        */
        inline TBool IsAttached() const;

        /**
        * Checks if current network mode is III.
        * @return ETrue if operating in network mode III.
        *         Otherwise EFalse.
        */
        inline TBool IsNetworkModeIII() const;

        /**
        * Returns ETrue if there are connections.
        * @return ETrue if there are connections.
        *         Otherwise EFalse.
        */
        inline TBool IsConnections() const;

        /**
        * Returns ETrue if bearer is GPRS.
        * @return ETrue if bearer is GPRS.
        *         Otherwise EFalse.
        */
        inline TBool AcceptConnectionBearer() const;

        /**
        * Returns ETrue if connection is active.
        * @return ETrue if connection is active.
        *         Otherwise EFalse.
        */
        inline TBool IsConnectionActive() const;

        /**
        * Returns ETrue if attach mode is 'on demand'.
        * @return ETrue if attach mode is 'on demand'.
        *         Otherwise EFalse.
        */
        inline TBool IsAttachModeOnDemand() const;

        /**
        * Returns ETrue if user confirmed connection termination.
        * @return ETrue if termination has been confirmed.
        *         Otherwise EFalse.
        */
        inline TBool IsConfirmed() const;

        /**
        * Returns telephony server session.
        * @return ETel session.
        */
        inline RTelServer& TelServerL();

        /**
        * Returns mobile phone.
        * @return phone subsession.
        */
        inline RMobilePhone& MobilePhoneL();

        /**
        * Returns packet service.
        * @return packet service subsession.
        */
        inline RPacketService& PacketServiceL();

        /**
        * Returns connection monitor.
        * @return connection monitor session.
        */
        inline RConnectionMonitor& ConnectionMonitorL();

        /**
        * Completes client's request with aErrorCode code.
        * @param aErrorCode error code.
        */
        void CompleteRequest( TInt aErrorCode );

        /**
        * Go to specified state.
        * @param aNewState new state.
        */ 
        inline void GoToState( TState aNewState );

        /**
        * Go to specified state and complete - operation
        * continues with next RunL call.
        * @param aNewState new state.
        * @param aError error code, KErrNone by default.
        */
        inline void GoToStateAndComplete( 
            TState aNewState,
            TInt aError = KErrNone );

    private:    // Data
        
        // Current state.
        TState iState;

        // Packet service status.
        TInt iServiceStatus;

        // Packet service current class.
        RPacketService::TMSClass iCurrentClass;

        // Packet service max class.
        RPacketService::TMSClass iMaxClass;

        // Packet service attach mode.
        RPacketService::TAttachMode iAttachMode;

        // Amount of connections.
        TUint iConnectionCount;

        // Telephony server session.
        /***************************************************** 
        *   Series 60 Customer / ETel 
        *   Series 60  ETel API 
        *****************************************************/
        RTelServer iServer;

        // Owned mobile phone subsession.
        /***************************************************** 
        *   Series 60 Customer / ETel 
        *   Series 60  ETel API 
        *****************************************************/
        RMobilePhone iMobilePhone;

        // Owned packet service subsession.
        /***************************************************** 
        *   Series 60 Customer / ETel 
        *   Series 60  ETel API 
        *****************************************************/
        RPacketService iPacketService;

        // Owned connection monitor session.
        RConnectionMonitor iConnectionMonitor;

        struct TConnectionId
            {
            TUint iConnectionId;
            TUint iSubConnectionId;
            };

        // Owned array of connection ids.
        RArray< TConnectionId > iConnections;

        // Current index in connections array.
        TInt iConnectionsIndex;
        
        // Connection bearer .
        TInt iConnectionBearer;

        // Connection status.
        TInt iConnectionStatus;

        // Connection name.
        TName iConnectionName;

        // Owned confirmation query.
        CAknGlobalConfirmationQuery* iConfirmationQuery;

        // Owned text in confirmation query.
        HBufC* iConfirmationText;

        // Ref to request status to be completed. NULL if
        // operation is not ongoing.
        TRequestStatus* iRequestStatus;

        //WCDMA connection status.
        TInt iWcdmaConnectionStatus;

    };

#endif      // CCOUTLACTIVE_H
            
// End of File
