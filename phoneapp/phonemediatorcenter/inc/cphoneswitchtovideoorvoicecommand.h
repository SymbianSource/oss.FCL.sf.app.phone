/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: 
*     Command for switching to video/voice at Video Telephony shutdown.
*
*/


#ifndef CPHONESWITCHTOVIDEOORVOICECOMMAND_H
#define CPHONESWITCHTOVIDEOORVOICECOMMAND_H

// INCLUDES
#include <e32base.h>
#include "mphoneshutdowncommand.h"
#include "mphoneenginemessagesender.h"

// FORWARD DECLARATIONS

// CLASS DECLARATION

class CPhoneSwitchToVideoOrVoiceCommand : public CBase, 
    public MPhoneShutdownCommand
    {
    public:  

        /**
         * Destructor.
         */
        virtual ~CPhoneSwitchToVideoOrVoiceCommand();
        
        /**
        * Two-phased constructor.
        */
        IMPORT_C static CPhoneSwitchToVideoOrVoiceCommand* NewL( 
            MPhoneEngineMessageSender& aEngineMessageSender );
        
        /**
        * Executes this Command.
        */
        virtual void ExecuteLD();
        

    private:

        /**
        * C++ default constructor.
        */
        CPhoneSwitchToVideoOrVoiceCommand( 
            MPhoneEngineMessageSender& aEngineMessageSender );
        
        /**
         * By default EPOC constructor is private.
         */
        void ConstructL();
        
        
    private:

        /**
        * For sending message to Phone Engine.
        */    
        MPhoneEngineMessageSender& iEngineMessageSender;
                     
    };
    
#endif // CPHONESWITCHTOVIDEOORVOICECOMMAND_H

// End of File
