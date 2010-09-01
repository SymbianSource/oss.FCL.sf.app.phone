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
* Description:  Implementation of the call operation
*
*/


// INCLUDE FILES
#include "edcontactorheaders.h"


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CEDContactorCallOperation::CEDContactorCallOperation()
// -----------------------------------------------------------------------------
//
CEDContactorCallOperation::CEDContactorCallOperation(const TDesC& aParam, 
        TBool aForcedCallType) : CEDContactorOperation(aParam), 
        iForcedCallType( aForcedCallType )
    {
    }

// -----------------------------------------------------------------------------
// CEDContactorCallOperation::~CEDContactorCallOperation()
// -----------------------------------------------------------------------------
//
CEDContactorCallOperation::~CEDContactorCallOperation()
    {
    }

// -----------------------------------------------------------------------------
// CEDContactorCallOperation::NewLC()
// -----------------------------------------------------------------------------
//
CEDContactorCallOperation* CEDContactorCallOperation::NewLC(const TDesC& aParam,
        TBool aForcedCallType )
    {
    CEDContactorCallOperation* self = 
        new (ELeave)CEDContactorCallOperation(aParam, aForcedCallType);
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

// -----------------------------------------------------------------------------
// CEDContactorCallOperation::NewL()
// -----------------------------------------------------------------------------
//
CEDContactorCallOperation* CEDContactorCallOperation::NewL(const TDesC& aParam,
        TBool aForcedCallType)
    {
    CEDContactorCallOperation* self =
        CEDContactorCallOperation::NewLC(aParam,aForcedCallType);
    CleanupStack::Pop(); // self;
    return self;
    }

// -----------------------------------------------------------------------------
// CEDContactorCallOperation::ConstructL()
// -----------------------------------------------------------------------------
//
void CEDContactorCallOperation::ConstructL()
    {

    }

// -----------------------------------------------------------------------------
// CEDContactorCallOperation::ExecuteLD()
// -----------------------------------------------------------------------------
//
void CEDContactorCallOperation::ExecuteLD()
    {
    CleanupStack::PushL(this);
    CEDPhoneCall::ExecuteL(iParam, CEDPhoneCall::ECCACallTypeVoice, iForcedCallType );
    CleanupStack::PopAndDestroy(this);
    }
// End of File
