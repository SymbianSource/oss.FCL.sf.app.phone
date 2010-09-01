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
* Description:  Declares a document for application.
*
*/


#ifndef VMDOCUMENT_H
#define VMDOCUMENT_H

// INCLUDES
#include <eikdoc.h>

// FORWARD DECLARATIONS
class CEikAppUi;

// CLASS DECLARATION

/**
*  CVmDocument application class.
*
*  @lib ?library
*  @since ?Series60_version
*/
class CVmDocument : public CEikDocument
    {
    public:  // Constructors and destructor
        /**
         * Two-phased constructor.
         */
        static CVmDocument* NewL( CEikApplication& aApp );
        
        /**
         * Destructor.
         */
        virtual ~CVmDocument();
 
     private:
        /**
         * Symbian OS default constructor.
         */
        CVmDocument( CEikApplication& aApp ): CEikDocument( aApp ){  }
     
     private:
        /**
         * From CEikDocument, create CVmAppUi "App UI" object.
         */
        CEikAppUi* CreateAppUiL();

    };

#endif  // VMDOCUMENT_H

// End of File  
