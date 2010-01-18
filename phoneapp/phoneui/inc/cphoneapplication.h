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
* Description:  Provide a basis for applications working 
                 within the framework provided by Avkon.
*
*/


#ifndef CPHONEAPPLICATION_H
#define CPHONEAPPLICATION_H

//  INCLUDES
#include <eikenv.h>
#include <eikapp.h>
#include <eikappui.h>
#include <aknapp.h>
#include <eikdoc.h>

#include "phoneconstants.h"

// CONSTANTS

// CLASS DECLARATION
class CPhoneLibraryContainer;

/**
*  Application class for the PhoneUI.
*
*  @since 4.0
*/
class CPhoneApplication : public CAknApplication
    {
    public:
 
        /**
        * From CEikApplication, creates an instance of the document for 
        * application.
        *
        * @return Returns the created instance.
        */
        CApaDocument* CreateDocumentL();

        /**
        * From CEikApplication, returns the uid of the application.
        *
        * @return Returns the uid.
        */
        TUid AppDllUid() const;

        /**
        * destructor
        */
        ~CPhoneApplication();

    protected:

        /**
        * From CAknApplication, called before document construction.
        * Creates factories.
        */
        void PreDocConstructL();

    private:
        /**
        * From CAknApplication Returns the application resource file name.
        */
        TFileName ResourceFileName() const;

        // Library handle container
        CPhoneLibraryContainer*      iLibraryContainer;
    };

#endif      // CPHONEAPPLICATION_H   
            
// End of File
