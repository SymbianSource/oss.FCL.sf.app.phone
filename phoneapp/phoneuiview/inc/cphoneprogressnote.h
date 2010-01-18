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
* Description:  Progress note handler
*
*/


#ifndef CPHONEPROGRESSNOTE_H
#define CPHONEPROGRESSNOTE_H

//  INCLUDES
#include    <AknProgressDialog.h>

// FORWARD DECLARATIONS

class CPhoneProgressNote : public CAknProgressDialog
    {
    public:  // Constructors and destructor

        /**
        * Constructor.
        *
        * @param aFinalValue final value.
        * @param anIncrement increment.
        * @param anInterval interval after which note is updated.
        * @param aSelfPtr pointer to itself.
        * @param aCommandHandler command handler.
        */
        CPhoneProgressNote( 
                TInt aFinalValue,
                TInt anIncrement,
                TInt anInterval,
                CEikDialog** aSelfPtr,
                MEikCommandObserver* aCommandObserver ); 
                
        /**
        * Constructor.
        *
        * @param aSelfPtr pointer to itself.
        * @param aCommandHandler command handler.
        */
        CPhoneProgressNote(
            CEikDialog** aSelfPtr,
            MEikCommandObserver* aCommandObserver  ); 
        
        /**
        * Destructor.
        */
        virtual ~CPhoneProgressNote();  
        
    private:  // From CAknNoteDialog

        TKeyResponse OfferKeyEventL( 
            const TKeyEvent& aKeyEvent,
            TEventCode aType );

        TBool OkToExitL( TInt aCommand );

    private:  // Data

        MEikCommandObserver* iCommandObserver;
    };
#endif      // CPhoneProgressNote_H
            
// End of File
