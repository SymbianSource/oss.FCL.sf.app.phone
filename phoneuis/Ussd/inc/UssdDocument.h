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
* Description:  Declares document for application.
*
*/


#ifndef USSDDOCUMENT_H
#define USSDDOCUMENT_H

// INCLUDES
#include <AknDoc.h>


// FORWARD DECLARATIONS
class  CEikAppUi;


// CLASS DECLARATION

/**
*  CUssdDocument application class.
*
*  @since 1.0
*/
class CUssdDocument : public CAknDocument
    {
    public: // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CUssdDocument* NewL( CEikApplication& aApp );

        /**
        * Destructor.
        */
        virtual ~CUssdDocument();

    private:

        /**
        * C++ constructor.
        */
        CUssdDocument( CEikApplication& aApp );

    private:

        /**
        * From CEikDocument, create CUssdAppUi "App UI" object.
        */
        CEikAppUi* CreateAppUiL();
    };

#endif

// End of File

