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
* Description:  PhoneUI Query Controller
*
*/


#ifndef CPHONEQUERYCONTROLLER_H
#define CPHONEQUERYCONTROLLER_H



//  INCLUDES
#include "cphonetextquery.h"
#include "mphonequeryobserver.h"
#include "mphonetimer.h"
#include "tphonecmdparamquery.h"

// DATA TYPES
enum TPhoneAvkonQueryType
    {
    EPhoneAvkonNone,
    EPhoneAvkonGlobalConfirmation,
    EPhoneAvkonGlobalList
    };

// FORWARD DECLARATIONS
class TPhoneCommandParam;
class CAknGlobalConfirmationQuery;
class CPhoneQueryDialog;
class CPhoneDtmfNumberListQuery;
class CPhoneListQueryDialog;
class CCaUiReconnectQuery;
class CPhoneTimer;
class CAknGlobalMsgQuery;
class CAknGlobalNote;

class CPhoneQueryController :
    public CActive,
    public MEikCommandObserver,
    public MPhoneQueryObserver,
    protected MPhoneTimer
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CPhoneQueryController* NewL( CEikonEnv& aEikEnv );

        /**
        * Destructor.
        */
        virtual ~CPhoneQueryController();   

        /**
        * Creates query dialog from the given command parameter
        * @param aCommandParam command parameter holding data for
        * populating the needed query dialog.
        */
        void CreateQueryL( TPhoneCommandParam* aCommandParam );

        /**
        * Destructs all existing query dialogs except global.
        */
        void DestroyQuery();
        
        /**
        * Destructs global query dialog.
        */
        void DestroyGlobalQuery();

        /**
        * Destroys global wait note
        */
        void DestroyGlobalWaitNote();

        /**
        * Fetches query dialog's content
        * @param aCommandParam pointer where content will be written
        */
        void GetTextQueryContentL( TPhoneCommandParam* aCommandParam );

        /**
        * Set by the query when it is asynchronously deleted.
        * @param aQueryDeleted ETrue if the query is deleted
        */
        void SetQueryDeletedFlag( TBool aQueryDeleted ); 
        
        /**
        * Get active status of queries
        * @param aCommandParam command parameter
        */
        void IsQueryActive( TPhoneCommandParam* aCommandParam );
        
        /**
        * Sets string for List query dialog
        * @param aCommandParam command parameter
        */
        void SetListQueryStringL( TPhoneCommandParam* aCommandParam );
    
        /**
        * Reports query dialog's exit
        * @param aCommand user selected action.
        */        
        void ReportQueryCompleteAsync( TInt aCommand );
        
        /**
        * Check if dtmf test query or dtmf dialog is visible.
        */        
        TBool IsDTMFQueryVisible();
        
        /**
        * Sets member variable NoConnectedCalls value.
        */ 
        void SetNoConnectedCalls( TPhoneCommandParam* aCommandParam );

    protected: // from CActive

        /**
        * See CActive documentation.
        */  
        void RunL();
    
        /**
        * See CActive documentation.
        */
        TInt RunError( TInt aError );

        /**
        * See CActive documentation.
        */    
        void DoCancel();
        
        /**
        * @see CPhoneTimer, handles timeout.
        */
        virtual void HandleTimeOutL();        
 
        /**
        * @see MEikCommandObserver
        */
        void ProcessCommandL( TInt aCommandId );
    private:  // Constructors
        
        /**
        * C++ default constructor.
        */
        CPhoneQueryController( CEikonEnv& aEikEnv );

        /**
        * Creates query dialog
        * @param aParams a TPhoneCmdParamQuery holding the parameters for the query
        */
        void CreateQueryDialogL( 
            const TPhoneCmdParamQuery& aParams );
        
        /**
        * Creates text query dialog
        * @param aParams a TPhoneCmdParamQuery holding the parameters for the query
        */            
        void CreateTextQueryDialogL( 
            const TPhoneCmdParamQuery& aParams,
            TInt aOkButtonCmd = EEikBidOk );              
        
        /*
        * Generates DTMF list query.
		* @param aDialogResourceId -Id of the list query to be loaded
        */
        void CreateDtmfListQueryL( TInt aDialogResourceId );
        
        /*
        * Generates list query.
		* @param aDialogResourceId -Id of the list query to be loaded
        */
        void CreateListQueryL( TInt aDialogResourceId, const RArray<TInt>& aCommands );

        /*
        * Generates custom list query.
        * @param aDialogResourceId -Id of the list query to be loaded
        * @param aItemTextArray Text array for list items.
        * @param aTone Query tone.
        */
        void CreateCustomListQueryL( const TPhoneCmdParamQuery& aParams );
        
        /**
        * Creates the default query.
        * @param aParams ommand parameter holding data for
        * populating the needed query dialog.
        */    
        void CreateDefaultQueryL( const TPhoneCmdParamQuery& aParams );
        
        /**
        * Creates the reconnect query.
        * @param aParams ommand parameter holding data for
        * populating the needed query dialog.
        */
        void CreateReconnectQueryL( TPhoneCommandParam* aCommandParam );
                
        /**
        * Creates the global query.
        * @param aParams a TPhoneCmdParamQuery holding the parameters for the query
        */        
        void CreateGlobalQueryDialogL( const TPhoneCmdParamQuery& aParams ); 

        /**
        * Callback function that CIdle calls when we are using Text Query Dialog.
        * This method displayes the actual query.
        * @param aCPhoneQueryController - pointer to itself
        * @return KErrNone
        */
        static TInt DisplayTextQueryCallBack( TAny* aCPhoneQueryController );
        
        /**
        * Clear timer used in global query
        */
        void ClearTimer();
 
        /**
         * Creates global message query dialog.
         * @param aParams a TPhoneCmdParamQuery holding the parameters for the query
         */    
        void CreateGlobalMsgQueryL( const TPhoneCmdParamQuery& aParams );
        
        /**
         * Destroys global message query.
         */
        void DestroyGlobalMsgQuery();

        /**
         * Creates global wait note.
         * @param aParams a TPhoneCmdParamQuery holding the parameters for the query
         */     
        void CreateGlobalWaitNoteL( const TPhoneCmdParamQuery& aParams );
       
        /**
        * Callback function that CIdle calls when we are handling process command.
        * This method forwards the actual command
        * @param aCPhoneQueryController - pointer to itself
        * @return KErrNone
        */
        static TInt ProcessCommandCallBack( TAny* aCPhoneQueryController );
        
    private:  // Data

        /**
        * Query dialog
        */
        CPhoneQueryDialog* iQuery;
        
        /**
        * Text query dialog
        */
        CPhoneTextQuery* iTextQuery;
        
        /**
        * DTMF number list query
        */
        CPhoneDtmfNumberListQuery* iDtmfNumberListQuery;
        
        CPhoneListQueryDialog* iListQuery;        

        /**
        * ETrue when the query is asynchronously deleted
        */
        TBool iQueryDeleted;
        
        /**
        * Call UI reconnect query.
        */
        CCaUiReconnectQuery* iCallUiReconnectQuery;
        
        // Eikon environment delivered from CPhoneViewController
        CEikonEnv& iEikEnv;
        
        // Query dialog's text. Stored here for async operations.
        RBuf iQueryNoteText;
        // Prompt for queries. Stored here for async operations.
        RBuf iQueryPrompt;
        
        // Query dialog's text. Stored here for async operations.
        // Needed if query has been deleted before content get 
        RBuf iQueryPreviousText;
        
        // Command for queries. Stored here for async operations.
        TInt iCommandId;
        
        // Waitnote text
        RBuf iWaitNoteText;
            
        // Global confirmation query.
        CAknGlobalConfirmationQuery* iGlobalConfirmationQuery;
 
 		// Currently active avkon query type
 		TInt iActiveAvkonQuery;
 	    
 	    /**
        * Status of VideoCallSetupFailedQuery
        */
        TBool iVideoCallSetupFailedQuery;
        
        /**
        * Status of transfer acceptance query.
        */
        TBool iTransferAcceptanceQuery;
        
        // Displaying a Text Query is a blocking synchronous call and we want to give
        // run time for other Phone stuff so CIdle object is used to let us do that.
        // Owned.
        CIdle *iTextQueryDisplayer;
                
        // Asynchronous call for process command
        CIdle *iCommandAsync;
        
        // Contains selected DTMF string from dtmf listquery dialog.
        HBufC* iResultBuffer;   
        
        // Timer.
        CPhoneTimer* iTimer;
        
        /**
         * Global message query.
         * Own.
         */
        CAknGlobalMsgQuery* iGlobalMsgQuery;
        
        /**
         * Parameter query command object.
         */
        TPhoneCmdParamQuery iQueryParam;
        
        /**
        * Global wait note.
        * Own.
        */
        CAknGlobalNote* iGlobalWaitNote;

        /**
         * Flag for check is command already handled.
         */
        TBool iCommandHandled;
        
		/**
         * Flag indicating connected calls.
         */
        TBool iNoConnectedCalls;
        
     };

#endif      // CPHONEQUERYCONTROLLER_H   
            
// End of File
