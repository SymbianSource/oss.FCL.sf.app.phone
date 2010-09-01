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
* Description:  Application document class.
*
*/


#ifndef CPHONEDOCUMENT_H
#define CPHONEDOCUMENT_H

//  INCLUDES
#include <AknDoc.h>

// FORWARD DECLARATION
class CApaWindowGroupName;

// CLASS DECLARATION

/**
*  Application document class for the PhoneUI.
*
*  @since 4.0
*
*/
class CPhoneDocument : public CAknDocument
    {
      public:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
        CPhoneDocument( CEikApplication& aApp );

        /**
        * Two-phased constructor.
        */
        static CPhoneDocument* NewL( CEikApplication& aApp );
        
        /**
        * Destructor.
        */
        virtual ~CPhoneDocument();

        /**
        * @see CEikDocument::UpdateTaskNameL
        * @since 2.8
        */
        void UpdateTaskNameL( CApaWindowGroupName* aWgName );

    private:

        /**
        * From CAknDocument: Creates an instance of application UI
        */
        CEikAppUi* CreateAppUiL();
 
    };

#endif      // CPHONEDOCUMENT_H
            
// End of File
