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
#include <AknGlobalNote.h> // for CAknGlobalNote
#include "tphonecommandparam.h"

// DATA TYPES

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
        IMPORT_C void SetType( TAknGlobalNoteType aType );

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
        * Sets the global note tone.
        * @param aTone is the note tone
        */
        IMPORT_C void SetTone( TInt aTone );
       
        /**
        * Sets the global note wait status.
        * @param aStatus Wait for completed or not
        */
        IMPORT_C void SetWaitForReady( TBool aStatus );
        
        /**
        * Sets the global note softkeys.
        * @param aTone is the note softkeys
        */
        IMPORT_C void SetSoftkeys( TInt aSoftkeys );

        
        /**
        * Returns the global note type
        * @return Returns the type
        */
        IMPORT_C TAknGlobalNoteType Type() const;

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
        * Returns the global note tone.
        * @return Returns the global note tone
        */
        IMPORT_C TInt Tone() const;
        
        /**
        * Gets the global note wait status
        * @return  ETrue if wait for completed
        */
        IMPORT_C TBool WaitForReady() const;
        
        /**
        * Returns the global note softkeys.
        * @return Returns the global note softkeys
        */
        IMPORT_C TInt Softkeys() const;      
 
    private:    
        
        /**
        * Global note type
        */
        TAknGlobalNoteType iType;

        /**
        * Global note text resource ID
        */
        TInt iTextResourceId;

        /**
        * Global note text
        */
        TPtrC iText;

        /**
        * Global note tone
        */
        TInt iTone;
        
        /**
        * Wait for ready status
        */
        TBool iWaitforReady;
        
        /**
        * Global note softkeys
        */
        TInt iSoftkeys;

    };

#endif // __TPHONECMDPARAMGLOBALNOTE_H
            
// End of File