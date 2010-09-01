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
* Description:  Factory to create AIW service requests.
*
*/


#include <e32std.h>

#include "TPhCntServiceRequestFactory.h"
#include "CPhCntCreateNewContactRequest.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
TPhCntServiceRequestFactory::TPhCntServiceRequestFactory()
    {
    }

// ---------------------------------------------------------------------------
// Creates request to create new contact.
// ---------------------------------------------------------------------------
//
CPhCntCreateNewContactRequest* 
    TPhCntServiceRequestFactory::CreateCreateNewContactRequestL()
    {
    return CPhCntCreateNewContactRequest::NewL();    
    }

