/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Service to show dialogs for user to select contact data.
*
*/


#ifndef CPHCNTCONTACTDATASELECTION_H
#define CPHCNTCONTACTDATASELECTION_H

#include <e32base.h>
#include "mphcntmatch.h"

NONSHARABLE_CLASS( CPhCntSelectedData ) : public CBase
    {
public:

    /**
     * Number selected by user.
     */
    virtual const TDesC& Data() const = 0;
    
    /**
     * Phonebook's link, which points to the selected
     * data.
     */
    virtual const TDesC8& FieldLink() const = 0;
    
    /**
     * Number type
     */
    virtual MPhCntMatch::TNumberType NumberType() const = 0;
   
    };

/**
 * Observer for contact selection.
 *
 * @lib PhoneCntFinder.lib
 * @since S60  v3.2
 */
class MPhCntSelectionObserver 
    {
public:
    
    /**
     * Callback to indicate that selection is done. 
     *
     * @param aContactData Selected data. May be NULL if error occured. Ownership is not transferred.
     * @param aErrorCode Possible system wide error code if error occured.
     */
    virtual void SelectionDone( CPhCntSelectedData* aContactData, TInt aErrorCode ) = 0;
    };

/**
 *  Interface for showing dialogs for selectiong contact data.
 *
 *  @lib PhoneCntFinder
 *  @since S60 v3.2
 */
NONSHARABLE_CLASS( CPhCntContactDataSelection ) : public CBase
    {

public:

    /**
	 * Phone number filtering
	 * 
	 * ECallPhoneNumber		- CS
     * ECallVoip,          	- PS
     * ECallVideoNumber,    - Video 
	 */
	enum TCallType
        {
        ECallPhoneNumber, 
        ECallVoip, 
        ECallVideoNumber 
        };

    /**
     * Determines if request is active.
     *
     * @since S60 v3.2
     * @return ETrue - Request is pending
     *         EFalse - No requests active.
     */
    virtual TBool IsActive() const = 0;
    
    /**
     * Cancels any outstanding request.
     * 
     * @post IsActive() == EFalse
     *
     * @since S60 v3.2
     */
    virtual void Cancel() = 0;

    /**
     * Shows a dialog for user to select a phonenumber from specified contact. 
     * The contact is specified by the contact link. Observer is notified
     * when the user has made the selection or canceled the dialog.
     *
     * @pre IsActive() == EFalse, leaves with error code KErrInUse.
     * Otherwise may leave with system wide errorcode.
     *
     * @since S60 v3.2
     * @param aContactLink Link to the contact, which number is to be selected.
     * @param aCallType Type of number, which will be shown to be selected.
     * @param aObserver Observer, which is notified when selection is done.
     */
     virtual void SelectPhoneNumberForCallL(
        const TDesC8& aContactLink,
        const TCallType& aCallType,
        MPhCntSelectionObserver& aObserver ) = 0;

    };

#endif // CPHCNTCONTACTDATASELECTION_H
