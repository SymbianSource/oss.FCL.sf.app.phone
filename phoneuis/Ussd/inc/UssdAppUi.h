/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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


#ifndef USSDAPPUI_H
#define USSDAPPUI_H


// INCLUDES
#include <aknappui.h>
#include <bldvariant.hrh> 

// FORWARD DECLARATIONS
class CUssdContainer;
class CUssdComms;



// CLASS DECLARATION

/**
*  Application UI class.
*  Provides support for the following features:
*  - EIKON control architecture
* 
*  @since 1.0
*/
class CUssdAppUi : public CAknAppUi
    {

    public: // Constructors and destructor

        /**
        * Symbian OS default constructor.
        */      
        void ConstructL();

        /**
        * Destructor.
        */      
        ~CUssdAppUi();


    public: // New functions

        /**
        * Set manu pane items.
        * To be used only if help is defined, otherwise does nothing.
        *
        * @param aResourceId The resource ID.
        * @param aMenuPane The menu pane.
        */
        void DynInitMenuPaneL( TInt aResourceId,CEikMenuPane* aMenuPane );

        /**
        * Set's left seftkey visible.
        * To be used only when help not defined. Otherwise does nothing.
        *
        * @param aVisible ETrue if set visible.
        */
        void SetSoftkeySendVisibleL( TBool aVisible = ETrue );

        
    public: // Functions from base classes
        /**
        * From CEikAppUi, takes care of command handling.
        * @param aCommand command to be handled
        */
        void HandleCommandL( TInt aCommand );

        /**
        * From CEikAppUi, handles resource changes.
        * @param aType type of event.
        */
        virtual void HandleResourceChangeL( TInt aType );

    private:

        // From CAknAppUi
        void HandleForegroundEventL(TBool aForeground);


    private: //Data

        // Object for sneding the string.
        CUssdComms* iComms;

        // Main "view".
        CUssdContainer* iAppContainer;

        // Current softkeys. Used only when help is not defined.
        TInt iCbaResource;

        // Is control added to stack.
        TBool iIsAddedToStack;
    };

#endif

// End of File
