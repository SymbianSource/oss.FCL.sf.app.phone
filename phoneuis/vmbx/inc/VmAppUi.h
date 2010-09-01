/*
* Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Declares UI class for application.
*
*/


#ifndef VMAPPUI_H
#define VMAPPUI_H

// INCLUDES
#include <aknappui.h>
#include <bldvariant.hrh>
#include "vm.hrh"
#include "VmQueryLauncher.h"

// FORWARD DECLARATIONS
class CVmContainer;
class CAiwServiceHandler;
class CRepository;

/**
* Status of AppUi
*/
enum TVmbxAppUiState
    {
    /** idle state of AppUi */
    EIdle= 0,
    /** state of that AppUi launch to CVmQueryLauncher
    * for querying number */
    ELauncherQueryingNumber
    };

// Constants
const TInt KVmCmdAiwVoiceCall = EVmCmdAiwCmdLast;
const TInt KVmCmdAiwVideoCall = EVmCmdAiwCmdLast+1;
const TInt KVmCmdAiwVoipCall  = EVmCmdAiwCmdLast+2;

// CLASS DECLARATION

/**
*  Application UI class.
*  Provides support for the following features:
*  - UIKON control architecture
*
*  @lib ?library
*  @since ?Series60_version
*/
class CVmAppUi : public CAknAppUi, public MVmQueryLauncher
    {
    public:  // Constructors and destructor

        /**
        * Symbian OS default constructor.
        */
        void ConstructL();

        /**
        * Destructor.
        */
        ~CVmAppUi();

    public: // New functions

        /**
        * Loads CBA buttons for MSK
        * @return void
        */
        void DrawCbaButtonsL();

        /**
        * Get current AppUi state
        * @return TVmbxAppUiState, state of AppUi
        */
        TVmbxAppUiState GetAppUiState();

    private: // New functions

        /**
        * Adds an item in menu.
        *
        * @param aMenuPane menu pane
        * @param aText item text
        * @param aCommandId menu command id
        */
        void AddMenuItemL( CEikMenuPane* aMenuPane, const TDesC& aText,
                                                         TInt aCommandId );
        /**
        * Defines whether voice mail is started from general settings
        * @return TBool, ETrue if started from GS else EFalse
        */
        TBool IsStartedFromGSL();

        /**
         * Set in MenuPane cmomand's item specific property.
         * @since S60 v5.2
         * @param aMenuPane curent munu pane
         * @param aCommandId command Id
         */
        void SetItemSpecificL( CEikMenuPane* aMenuPane, TInt aCommandId );

    private: // Functions from base classes

        /**
        * From MEikMenuObserver, dynamically initialises menu pane.
        */
        void DynInitMenuPaneL( TInt aResourceId, CEikMenuPane* aMenuPane );

        /**
        * From CCoeAppUi, gets a list of help contexts for a particular
        * application user interface.
        */
        CArrayFix<TCoeHelpContext>* HelpContextL() const;

        /**
        * From CEikAppUi, takes care of command handling.
        * @param aCommand command to be handled
        */
        void HandleCommandL( TInt aCommand );

        /**
        * From CEikAppUi, Handles changes in keyboard focus when an application
        * switches to,  or from, the foreground.
        * @param aForeground ETrue if the application is in the foreground,
        *                     otherwise EFalse.
        */
        void HandleForegroundEventL( TBool aForeground );

       /**
         * From MVmQueryLauncher,Callback when query number completed
         *
         */
        void NumberQueryComplete();

        /**
         * see VmContainer::IsIpVoiceMailboxServices
         */
        TBool IsIpVoiceMailboxServices();

    private: // Data

        //AppUi container
        CVmContainer* iAppContainer;

        //menupane text strings
        HBufC* iTextShowDetails;  // owned
        HBufC* iTextClearIcon; // owned
        HBufC* iTextCallVoiceMail;  // owned
        HBufC* iTextDefineNumber;  // owned
        HBufC* iTextDefineVideoNumber;  // owned
        HBufC* iTextChangeNumber;  // owned
        HBufC* iTextHelp;  // owned
        HBufC* iTextExit;  // owned
        HBufC* iTextActivate; // owned
        HBufC* iTextCallIntVoiceMail;  // owned
        HBufC* iTextCancel; // owned
        HBufC* iTextCheckStatus; // owned
        HBufC* iTextCheckMessage; // owned
        HBufC* iTextCallVideoMail;  // owned

        //type of vmbx
        TInt iType;

        //title of the popupList
        HBufC* iTextRingingTime; // owned

        //CVmQueryLauncher object for asking vmbx number
        CVmQueryLauncher* iQueryLauncher;  // owned

        // AIW
        CAiwServiceHandler* iServiceHandler; // owned

        TBool iFeatureManagerInitialized;

        // Boolean defining whether voice mail is started from
        // general settings or not
        TBool iFromGS;

        // state of AppUi
        TVmbxAppUiState iAppUiState;
    };

#endif  // VMAPPUI_H

// End of File
