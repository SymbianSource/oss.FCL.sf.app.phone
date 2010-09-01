/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Controller for DTMF mode of Dialer
*
*/

#ifndef CPHONEDTMFDIALERCONTROLLER_H
#define CPHONEDTMFDIALERCONTROLLER_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include "cphonedialercontroller.h"

// CLASS DECLARATION

/**
 *  CPhoneDtmfDialerController
 *  
 *  Provides data for the DTMF mode of the dialer.
 */
NONSHARABLE_CLASS( CPhoneDtmfDialerController ) : public CPhoneDialerController
    {
public: // Constructors and destructor

    /**
     * Destructor.
     */
    ~CPhoneDtmfDialerController();

    /**
     * Two-phased constructor.
     */
    static CPhoneDtmfDialerController* NewL( 
            CPhoneBubbleWrapper* aBubbleWrapper,
            CCoeEnv& aCoeEnv );

    /**
     * Two-phased constructor.
     */
    static CPhoneDtmfDialerController* NewLC( 
            CPhoneBubbleWrapper* aBubbleWrapper,
            CCoeEnv& aCoeEnv );

private:

    /**
     * Constructor for performing 1st stage construction
     */
    CPhoneDtmfDialerController( 
            CPhoneBubbleWrapper* aBubbleWrapper,
            CCoeEnv& aCoeEnv );

    /**
     * Symbian default constructor for performing 2nd stage construction
     */
    void ConstructL();

public: // from CPhoneDialerController
    
    /**
     * @see MPhoneDialerController
     */
    const TDesC& NumberEntryPromptTextL();
    
    /**
     * @see MPhoneDialerController
     */
    TInt GetButtonData( TButtonIndex aIndex, RPointerArray<CButtonData>& aData ) const;
    
    /**
     * @see MPhoneDialerController
     */
    TInt ButtonState( TButtonIndex aIndex ) const;
    
    /**
     * @see MPhoneDialerController
     */
    TBool ButtonDimmed( TButtonIndex aIndex ) const;
    
    /**
     * @see MPhoneDialerController
     */
    TBool EasyDialingAllowed() const;
    
private: // data
    
    HBufC* iPromptText; // OWN
    };

#endif // CPHONEDTMFDIALERCONTROLLER_H
