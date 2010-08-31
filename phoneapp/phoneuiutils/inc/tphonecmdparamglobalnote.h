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
* Description: 
*   See class description.
*
*/


#ifndef __TPHONECMDPARAMGLOBALNOTE_H
#define __TPHONECMDPARAMGLOBALNOTE_H

//  INCLUDES

#include <w32std.h>
//#include <AknGlobalNote.h> // for CAknGlobalNote

#include "tphonecommandparam.h"

// DATA TYPES
enum PhoneNotificationType{
    EPhoneNotificationDialog = 0, // notify of a state change that does not require user input.
    EPhoneMessageBoxInformation,
    EPhoneMessageBoxWarning
};
// CLASS DECLARATION

/**
*  A parameter class for global note information.
*/
class TPhoneCmdParamGlobalNote : public TPhoneUICommandParam
    {   
    public:  
        
        /**
        * C++ default constructor.
        */
        IMPORT_C TPhoneCmdParamGlobalNote();

    public: 
        
        /**
        * Sets the global note type.
        * @param aType is the global note type
        */
        IMPORT_C void SetType( PhoneNotificationType aType );

        /**
        * Sets the text resource ID for the global note
        * @param aResourceId is the global note resource ID
        */
        IMPORT_C void SetTextResourceId( TInt aTextResourceId );

        /**
        * Sets the text for the global note.
        * @param aText is the global note text
        */
        IMPORT_C void SetText( const TDesC& aText );

       /**
        * Sets timeout for a note.
        * @param    aTimeout    Timeout in milliseconds. If timeout == 0 then 
        * default timeout is used. If timeout < 0 the note is not closed 
        * automatically.
        */
        IMPORT_C void SetTimeout( TInt aTimeout );
        
        /**
        * Sets notification dialog flag.
        * @param aNotificationDialog True if notification dialog 
        *        should be used.
        */
        IMPORT_C void SetNotificationDialog( TBool aNotificationDialog );
        
        /**
        * Sets cause code.
        * @param aCauseCode Cause code.
        */
        IMPORT_C void SetCauseCode( TInt aCauseCode );
        
        /**
        * Returns the global note type
        * @return Returns the type
        */
        IMPORT_C PhoneNotificationType Type() const;

        /**
        * Returns the global note text resource ID.
        * @return Returns the global note text resource ID.
        */
        IMPORT_C TInt TextResourceId() const;

        /**
        * Returns the global note text
        * @return Returns the text
        */
        IMPORT_C const TDesC& Text() const;

        /**
         * Returns the timeout value.
         * @return   Timeout value.
         */
        IMPORT_C TInt Timeout() const;
        
        /**
         * Returns the notification dialog flag.
         * @return Notification dialog flag.
         */
        IMPORT_C TBool NotificationDialog() const;
        
        /**
        * Returns cause code.
        * @return Cause code.
        */
        IMPORT_C TInt CauseCode() const;
    
    private:    
        
        /**
        * Global note type
        */
        PhoneNotificationType iType;
        
        /**
        * Global note text resource ID
        */
        TInt iTextResourceId;

        /**
        * Global note text
        */
        TPtrC iText;
        
        /**
        * Timeout for a note.
        */
        TInt iTimeout;
        
        /**
        * Notification Dialog flag.
        */
        TBool iNotificationDialog;
        
        /**
        * Cause code. 
        */
        TInt iCauseCode; 
    };

#endif // __TPHONECMDPARAMGLOBALNOTE_H
            
// End of File
