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
* Description:  DTMF Sending Note. This note handles keypresses unlike
*								 CPhoneNote.
*
*/

/*
*/

#ifndef CPHONEDTMFSENDINGNOTE_H
#define CPHONEDTMFSENDINGNOTE_H


//  INCLUDES
#include "cphonenote.h"

// FORWARD DECLARATIONS

class CPhoneDtmfSendingNote : public CPhoneNote
    {
    public:  // Constructors and destructor

        /**
         * Constructor.
         * 
         * @param    aSelfPtr            Pointer to self. 
         *                               This pointer is set to NULL when the note is destroyed by the framework.
         * @param    aCommandObserver    Observer to receive command events from the note.
         */
        CPhoneDtmfSendingNote( CEikDialog** aSelfPtr, MEikCommandObserver& aCommandObserver );  
        
        /**
        * Destructor.
        */
        virtual ~CPhoneDtmfSendingNote();  
        
        
    protected:

        TKeyResponse OfferKeyEventL( 
            const TKeyEvent& aKeyEvent,
            TEventCode aType );

    };

#endif // CPHONEDTMFSENDINGNOTE_H   
            
// End of File
