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
* Description:  PhoneUI Text Query.
*
*/


#ifndef CPHONETEXTQUERY_H
#define CPHONETEXTQUERY_H

//  INCLUDES
#include <AknQueryDialog.h>

// CLASS DECLARATION

class CPhoneTextQuery : public CAknTextQueryDialog
    {
    public:

        /**
        * Constructor.
        */
        CPhoneTextQuery( 
            MEikCommandObserver& aCommandObserver, 
            TDes& aDataText,
            TInt aDefaultCbaResourceId,
            TInt aContentCbaResourceId,
            TBool aSendKeyEnabled,
            TInt aEikBidOkCmd = EEikBidOk );  
        
        /**
        * Destructor.
        */
        virtual ~CPhoneTextQuery();   

        /**
        * From CAknTextQueryDialog, handles keyevents.
        */
        TKeyResponse OfferKeyEventL(
            const TKeyEvent& aKeyEvent,
            TEventCode aType );

        /**
        * From CAknTextQueryDialog, dynamic initialization 
        * after layout.
        */
        void PostLayoutDynInitL();   
    
        void GetContent( TDes& aText );
        
      
    private:
      
        /**
        * From CEikdialog   This function is called by the EIKON framework 
        *                   if the user activates a button in the button panel. 
        *                   It is not called if the Cancel button is activated, 
        *                   unless the EEikDialogFlagNotifyEsc flag is set.
        * @param aButtonId  The ID of the button that was activated
        * @return           Should return ETrue if the dialog should exit, and EFalse if it should not.
        */
        TBool OkToExitL( TInt aButtonId );

        /**
        * Updates softkeys.
        */
        void UpdateSoftkeysL();
 
        /**
         * Asks the command id from the softkey (CBA)
         * @return the command id or 0 in case of problems
         */
        TInt FetchCommandFromCba();
     
    private: // Data
        
        MEikCommandObserver& iCommandObserver;
                
        TInt iDefaultCbaResourceId;
        
        TInt iContentCbaResourceId;

        TBool iSendKeyHandlingEnabled;
        
        TInt iEikBidOkCmd;
    };

#endif      // CPHONETEXTQUERY_H   
            
// End of File
