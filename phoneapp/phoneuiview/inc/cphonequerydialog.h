/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  PhoneUI Query Dialog
*
*/

/*
*/

#ifndef CPHONEQUERYDIALOG_H
#define CPHONEQUERYDIALOG_H


//  INCLUDES
#include <AknQueryDialog.h>

// FORWARD DECLARATIONS

class CPhoneQueryDialog : public CAknQueryDialog
    {
    public:  // Constructors and destructor

        /**
        * Constructor.
        */
        CPhoneQueryDialog( const TTone& aTone );
        
        static CPhoneQueryDialog* NewL( const TTone& aTone = ENoTone );  
        
        /**
        * Destructor.
        */
        virtual ~CPhoneQueryDialog();  

        
    protected:

        TKeyResponse OfferKeyEventL( 
            const TKeyEvent& aKeyEvent,
            TEventCode aType );
    };

#endif // CPHONEQUERYDIALOG_H   
            
// End of File
