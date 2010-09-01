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

#ifndef __EDTA_DOCUMENT_H__
#define __EDTA_DOCUMENT_H__

// INCLUDES
#include <akndoc.h>

// FORWARD DECLARATIONS
class CEdtaAppUi;
class CEikApplication;


// CLASS DECLARATION

/**
* CEdtaDocument application class.
* An instance of class CEdtaDocument is the Document part of the
* AVKON application framework for the Edta example application.
*/
class CEdtaDocument : public CAknDocument
    {
    public: // Constructors and destructor

        /**
        * NewL.
        * Two-phased constructor.
        * Construct a CEdtaDocument for the AVKON application aApp
        * using two phase construction, and return a pointer
        * to the created object.
        * @param aApp Application creating this document.
        * @return A pointer to the created instance of CEdtaDocument.
        */
        static CEdtaDocument* NewL( CEikApplication& aApp );

        /**
        * NewLC.
        * Two-phased constructor.
        * Construct a CEdtaDocument for the AVKON application aApp
        * using two phase construction, and return a pointer
        * to the created object.
        * @param aApp Application creating this document.
        * @return A pointer to the created instance of CEdtaDocument.
        */
        static CEdtaDocument* NewLC( CEikApplication& aApp );

        /**
        * ~CEdtaDocument
        * Virtual Destructor.
        */
        virtual ~CEdtaDocument();

    public: // Functions from base classes

        /**
        * CreateAppUiL
        * From CEikDocument, CreateAppUiL.
        * Create a CEdtaAppUi object and return a pointer to it.
        * The object returned is owned by the Uikon framework.
        * @return Pointer to created instance of AppUi.
        */
        CEikAppUi* CreateAppUiL();

    private: // Constructors

        /**
        * ConstructL
        * 2nd phase constructor.
        */
        void ConstructL();

        /**
        * CEdtaDocument.
        * C++ default constructor.
        * @param aApp Application creating this document.
        */
        CEdtaDocument( CEikApplication& aApp );

    };

#endif // __EDTA_DOCUMENT_H__

// End of File

