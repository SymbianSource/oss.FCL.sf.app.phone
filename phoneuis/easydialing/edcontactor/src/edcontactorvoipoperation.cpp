/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Implementation of the voip operation
*
*/


// INCLUDE FILES
#include "edcontactorheaders.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CEDContactorVOIPOperation::CEDContactorVOIPOperation()
// -----------------------------------------------------------------------------
//
CEDContactorVOIPOperation::CEDContactorVOIPOperation(const TDesC& aParam, TUint32 aServiceId) : CEDContactorOperation(aParam), iServiceId(aServiceId)
    {
    // No implementation required
    }

// -----------------------------------------------------------------------------
// CEDContactorVOIPOperation::~CEDContactorVOIPOperation()
// -----------------------------------------------------------------------------
//
CEDContactorVOIPOperation::~CEDContactorVOIPOperation()
    {
    }

// -----------------------------------------------------------------------------
// CEDContactorVOIPOperation::NewLC()
// -----------------------------------------------------------------------------
//
CEDContactorVOIPOperation* CEDContactorVOIPOperation::NewLC(const TDesC& aParam, TUint32 aServiceId)
    {
    CEDContactorVOIPOperation* self = new (ELeave)CEDContactorVOIPOperation(aParam, aServiceId);
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

// -----------------------------------------------------------------------------
// CEDContactorVOIPOperation::NewL()
// -----------------------------------------------------------------------------
//
CEDContactorVOIPOperation* CEDContactorVOIPOperation::NewL(const TDesC& aParam, TUint32 aServiceId)
    {
    CEDContactorVOIPOperation* self=CEDContactorVOIPOperation::NewLC(aParam, aServiceId);
    CleanupStack::Pop(); // self;
    return self;
    }

// -----------------------------------------------------------------------------
// CEDContactorVOIPOperation::ConstructL()
// -----------------------------------------------------------------------------
//
void CEDContactorVOIPOperation::ConstructL()
    {

    }

// -----------------------------------------------------------------------------
// CEDContactorVOIPOperation::ExecuteLD()
// -----------------------------------------------------------------------------
//
void CEDContactorVOIPOperation::ExecuteLD()
    {
    CleanupStack::PushL(this);
    CEDPhoneCall::ExecuteL(iParam, CEDPhoneCall::ECCACallTypeVoIP, EFalse,
                           iServiceId);
    CleanupStack::PopAndDestroy(this);
    }
// End of File
