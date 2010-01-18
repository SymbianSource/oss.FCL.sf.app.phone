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
* Description:  PhoneUI ListQueryDialog
*
*/


#ifndef CPHONELISTQUERYDIALOG_H
#define CPHONELISTQUERYDIALOG_H

//  INCLUDES
#include <aknlistquerydialog.h>


// CLASS DECLARATION
class MPhoneQueryObserver;

/**
* This is a dialog, which can be initialized with selected items.
*/
class CPhoneListQueryDialog : public CAknListQueryDialog
    {
    public:  // Constructors and destructor
        
        /**
        * C++ constructor.
        *
        * @param aCommandObserver callback observer
        *                    
        */
        CPhoneListQueryDialog( MEikCommandObserver& aCommandObserver,
                               MPhoneQueryObserver& aQueryObserver,
                               const RArray<TInt>& aCommands );

        /**
        * Destructor.
        */
        virtual ~CPhoneListQueryDialog();
        
        
    private: // From CEikDialog
        
        /**
        * Sets selection indexes to list box.
        */
        void PostLayoutDynInitL();

        /**
        * If the user has pushed Ok, the dialog Stores the selected 
        * values in to the server.
        *
        * @param aKeyCode key code
        * @return ETrue if dialog is closed
        */
        TBool OkToExitL( TInt aKeycode );
        
               
        TKeyResponse OfferKeyEventL( const TKeyEvent& aKeyEvent,
                                           TEventCode aType );

    private: // Data

        // contains selected index
        TInt iIndex;
        
        MEikCommandObserver& iCommandObserver;
        
        MPhoneQueryObserver& iQueryObserver; 
        
        RArray<TInt> iCommands;

    };

#endif      // CPHONELISTQUERYDIALOG_H
            
// End of File
