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
* Description:  Declares main application class for the application.
*
*/


#ifndef VMAPP_H
#define VMAPP_H

// INCLUDES
#include <aknapp.h>

// CONSTANTS
// UID of the application
const TUid KUidvm = { 0x100058F5 };

// CLASS DECLARATION

/**
*  CVmApp application class.
*  Provides factory to create concrete document object.
*
*  @lib ?library
*  @since ?Series60_version
*/
class CVmApp : public CAknApplication
    {
    private:
        /**
         * From CApaApplication, creates CVmDocument document object.
         * @return A pointer to the created document object.
         */
        CApaDocument* CreateDocumentL();
    
        /**
         * From CApaApplication, returns application's UID (KUidvm).
         * @return The value of KUidvm.
         */
        TUid AppDllUid() const;
    };

#endif  // VMAPP_H

// End of File  
