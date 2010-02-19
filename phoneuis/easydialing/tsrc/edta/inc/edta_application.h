/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Easy dialing test application.
*
*/

#ifndef __EDTA_APPLICATION_H__
#define __EDTA_APPLICATION_H__

// INCLUDES
#include <aknapp.h>

// CLASS DECLARATION

/**
* CEdtaApplication application class.
* Provides factory to create concrete document object.
* An instance of CEdtaApplication is the application part of the
* AVKON application framework for the Edta example application.
*/
class CEdtaApplication : public CAknApplication
    {
    public: // Functions from base classes

        /**
        * From CApaApplication, AppDllUid.
        * @return Application's UID (KUidEdtaApp).
        */
        TUid AppDllUid() const;

    protected: // Functions from base classes

        /**
        * From CApaApplication, CreateDocumentL.
        * Creates document object. The returned
        * pointer in not owned by the CEdtaApplication object.
        * @return A pointer to the created document object.
        */
        CApaDocument* CreateDocumentL();
    };

#endif // __EDTA_APPLICATION_H__

// End of File

