/*
* Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Fetch items from Contact
*
*/


#ifndef C_CPHONESINGLEITEMFETCH_H
#define C_CPHONESINGLEITEMFETCH_H

#include <e32base.h>
#include "mphoneviewobserver.h"

class CPhoneViewController;
class CPhoneContactController;
class CPhCntSingleItemFetch;
class CPhoneStatusPane;

/**
 *  Items Fetch from Contact
 *
 *
 *  @lib PhoneUIView
 *  @since S60 5.0
 */
class CPhoneSingleItemFetch : public CActive, public MPhoneViewObserver
    {

public:

   /**
    * Two-phased constructor.
    */
    IMPORT_C static CPhoneSingleItemFetch* NewL( 
        CEikonEnv& aEikEnv,
        CPhoneViewController& aViewController,
        CPhoneContactController& aContactController,
        CPhoneStatusPane& aStatusPane );

   /**
    * Destructor.
    */
     virtual ~CPhoneSingleItemFetch();

    /**
     * Open single item fetch dialog
     * @param aCommandParam command parameter holding data for
     * populating the needed query dialog.
     */
     void OpenSingleItemFetchDialogL( TPhoneCommandParam* aCommandParam ); 
    
    /**
     * Delete SingleItemFetch
     */
     void Delete();
     
    /**
     * See MPhoneViewObserver.h
     */
     void PhoneViewActivatedL();
     

protected: // from CActive

    /**
     * See CActive documentation.
     */  
     void RunL();

    /**
     * See CActive documentation.
     * @param aError symbian error code
     */
     TInt RunError( TInt aError );

    /**
     * See CActive documentation.
     */    
     void DoCancel();
     
private:

    /**
     * C++ default constructor.
     */
     CPhoneSingleItemFetch( 
        CEikonEnv& aEikEnv,
        CPhoneViewController& aViewController,
        CPhoneContactController& aContactController,
        CPhoneStatusPane& aStatusPane );

private: // data

    
    /**
    * Eikon environment delivered from CPhoneViewController
    */
    CEikonEnv& iEikEnv;
    
    /**
    * Reference to owner
    */
    CPhoneViewController& iViewController;
    
    /**
    * Pointer to CPhoneContactController
    */
    CPhoneContactController& iContactController;
    
    /**
    * Pointer to CPhoneStatusPane
    */
    CPhoneStatusPane& iStatusPane;
    
    /**
    * Single Item Fetch
    */
    CPhCntSingleItemFetch* iSingleItemFetch;
    
    /**
    * ResourceId
    */
    TInt iResourceId;
    
    /**
    * ButtonId
    */
    TInt iButtonId;
    
    /**
     * True when singleitem fetch have been completed.
     */
    TBool iFetchPerformed;
    };

#endif // C_CPHONESINGLEITEMFETCH_H
