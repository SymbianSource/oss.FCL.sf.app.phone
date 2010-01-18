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
* Description:  Handles Ussd communications to the Phone Server.
*
*/


#ifndef USSDCOMMS_H
#define USSDCOMMS_H


// INCLUDES
#include <coemain.h>
#include <CPhCltUssdInt.h>
#include <PhCltTypes.h>


// CONSTANTS
const TInt KUssdEditorMaxLenght = 182;


// FORWARD DECLARATIONS    
class CPhCltUssd;


// CLASS DECLARATION

/**
*  CUssdComms  
*  Handles Ussd communications to the Phone Server.
*
*  @since 1.0
*/
class CUssdComms : public CBase  
    {
    public: // Constructors and destructor
        
        /**
        * Two-phased constructor
        */
        static CUssdComms* NewL();
     
        /**
        * Destructor.
        */
        ~CUssdComms();


    public: // Member functions

        /**
        * Send created string
        * 
        * @param aString String to be sent.
        * @return Error code
        */
        TInt SendString( const TDesC& aString );

        /**
        * Application to foreground
        * 
        * @return ETrue if editor needs clearing.
        */
        TBool InformAppForeground();

        /**
        * Application to background
        */
        void InformAppBackground();

        /**
        * Tell the reason why app was terminated.
        */
        void InformExitReason( TPhCltUssdAppExitReason aExitReason );


    protected: // Constructor

        // Default constructor
        CUssdComms();

        // 2nd phase constructor
        void ConstructL();


    private: // Data

        // Sending Ussd, Inform ussd
        CPhCltUssdInt* iUssdClientInt; 

        // Ussd application exit reason.
        TPhCltUssdAppExitReason  iExitReason;
    };

#endif // USSDCOMMS_H


// End of File
