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
* Description:  PhoneUI Note
*
*/

/*
*/

#ifndef CPHONEUIDISABLEDNOTE_H
#define CPHONEUIDISABLEDNOTE_H


//  INCLUDES
#include "tphonecmdparamnote.h"
#include "cphonenote.h"

// FORWARD DECLARATIONS
class CAknNoteDialog;

class CPhoneUIDisabledNote : public CPhoneNote
    {
    public:  // Constructors and destructor

        /**
         * Constructor.
         * 
         * @param    aSelfPtr            Pointer to self. 
         *                               This pointer is set to NULL when the note is destroyed by the framework.
         * @param    aCommandObserver    Observer to receive command events from the note.
         */
        CPhoneUIDisabledNote( CEikDialog** aSelfPtr, MEikCommandObserver& aCommandObserver );  
        
        /**
        * Destructor.
        */
        virtual ~CPhoneUIDisabledNote();  

        /**
        * Sets the type of the note.
        */
        void SetNoteType( TPhoneNoteType aNoteType );

        /**
        * Gets the type of the note.
        */
        TPhoneNoteType NoteType();
        
        /**
        * Updates softkeys to note
        * @param    aResourceId     Resource id
        */
        void UpdateSoftkeysL( TInt aResourceId );
        
    protected:

        TKeyResponse OfferKeyEventL( 
            const TKeyEvent& aKeyEvent,
            TEventCode aType );

        TBool OkToExitL( TInt aCommand );

        void HandlePointerEventL( 
            const TPointerEvent& aPointerEvent );

        void SetSizeAndPosition( const TSize& aSize );
        
    protected:  // Data
        
        //The type of note. Not necessarily set.
        //Used in security view
        TPhoneNoteType iNoteType;
    };

#endif // CPHONEUIDISABLEDNOTE_H   
            
// End of File
