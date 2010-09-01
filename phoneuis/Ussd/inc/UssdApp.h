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
* Description:  Declares main application class.
*
*/


#ifndef USSDAPP_H
#define USSDAPP_H

// INCLUDES
#include <aknapp.h>

// CONSTANTS
// UID of the application
const TUid KUidussd = { 0x10005955 };


// CLASS DECLARATION

/**
*  CUssdApp application class.
*  Provides factory to create concrete document object.
* 
*  @since 1.0
*/
class CUssdApp : public CAknApplication
    {
    
    private:

        /**
        * From CApaApplication, creates CUssdDocument document object.
        * @return A pointer to the created document object.
        */
        CApaDocument* CreateDocumentL();
        
        /**
        * From CApaApplication, returns application's UID (KUidussd).
        * @return The value of KUidussd.
        */
        TUid AppDllUid() const;
    };

#endif

// End of File

