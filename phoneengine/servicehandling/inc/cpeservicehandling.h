/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Provides service enabling implementation.
*
*/


#ifndef C_PESERVICEHANDLING_H
#define C_PESERVICEHANDLING_H

#include <e32base.h>
#include <cch.h>
#include "mpeservicehandling.h"

class  MPEPhoneModelInternal;

/**
 *  CPEServiceHandling implements the intefaces MPEServiceHandling and 
 *  thus provides the service enabling imlementation for Phone Engine.
 *  
 *  @lib peservicehandling.lib
 *  @since S60 v5.0
 */
NONSHARABLE_CLASS( CPEServiceHandling ): public CBase,
                                         public MCchServiceStatusObserver,
                                         public MPEServiceHandling
    {

	public: // Constructors and destructor

	    /**
	     * Two-phased constructor.
	     */
	    IMPORT_C static CPEServiceHandling* NewL( MPEPhoneModelInternal& aModel );

	    /**
	     * Two-phased constructor.
	     */
	    IMPORT_C static CPEServiceHandling* NewLC( MPEPhoneModelInternal& aModel );

	    /**
	     * Destructors.
	     */
	    IMPORT_C virtual ~CPEServiceHandling();

	public: // from MPEServiceHandling
	
        /**
         * From MPEServiceHandling.
         * Enables service defined by the given identifier.
         * This function does the service enabling and service status error checking.
         * If service is enabled successfully, send message 
         * (EPEMessageServiceEnabled) to PhoneEngine. 
         * 
         * @since   S60 5.0
         * @param   aServiceId      Identifier of the service.
         */	                                    
	    void EnableServiceL( TInt aServiceId );
	   
        /**
         * From MPEServiceHandling.
         * Cancel current service.
         */ 
	    void CancelServiceEnabling() const;
	    
        /**
         * From MPEServiceHandling.
         * Disable service.
         */ 
        void DisableService() const;
    
    public: // from MCchServiceStatusObserver
        
        /**
         * From MCchServiceStatusObserver.
         * Signaled when service status or error changes.
         *
         * @param   aServiceId      Identifier of the service.
         * @param   aType           Service type.
         * @param   aServiceStatus  Service status.
         */
        void ServiceStatusChanged( TInt aServiceId, 
                                   const TCCHSubserviceType aType,
                                   const TCchServiceStatus& aServiceStatus );
	private: // Constructors

        /**
         * C++ default constructor.
         */
        CPEServiceHandling( MPEPhoneModelInternal& aModel );
        
        /**
         * By default Symbian 2nd phase constructor is private.
         */
        void ConstructL();
	    
    private: // functions
    
        /**
         * Enable service, if not already enabled.
         *
         * @param  aState       Service state.
         * @param  aService     Service.
         * @return Error code.
         */
        TInt EnableServiceIfNeeded( const TCCHSubserviceState& aState, 
                                    CCchService& aService );
        /**
        * Sends ServiceHandlingError message to PhoneEngine and saves 
        * errorcode to member variable.
        * 
        * @param  aErrorCode, Cch errorcode.
        */	    
        void SendErrorMessage( TInt aErrorCode );
       
	private: // data
	    
	    /**
	     * CCH client.
	     * Own.
	     */
	    CCch* iCchClient;

	    /**
	     * PhoneModel.
	     * Not own.
	     */
	    MPEPhoneModelInternal& iModel;
	    
	    /**
	     * Current service identifier.
	     */
	    TInt iCurrentServiceId;
    };

#endif // C_PESERVICEHANDLING_H
