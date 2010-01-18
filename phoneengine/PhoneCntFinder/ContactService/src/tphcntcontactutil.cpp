/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Contact utilitiest class.
*
*/


#include "tphcntcontactutil.h"

// ---------------------------------------------------------------------------
// Checks are name fields same in both contacts.
// ---------------------------------------------------------------------------
//
TBool TPhCntContactUtil::AreFirstAndLastAndCompanyNameSame( 
            const CPhCntContact& aFirstContact,
            const CPhCntContact& aSecondContact )
    {
    // Returns ETrue if first name, last name and company name are same
    // in both contacts.
    return KErrNone == aFirstContact.FirstName().Compare( 
                        aSecondContact.FirstName() )  &&
           KErrNone == aFirstContact.LastName().Compare( 
                        aSecondContact.LastName() ) &&
           KErrNone == aFirstContact.CompanyName().Compare( 
                        aSecondContact.CompanyName() );
    }


