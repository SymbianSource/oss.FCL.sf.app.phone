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
* Description:  Implementation of the video call operation
*
*/


// INCLUDE FILES
#include "edcontactorheaders.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CEDContactorVideocallOperation::CEDContactorVideocallOperation()
// -----------------------------------------------------------------------------
//
CEDContactorVideocallOperation::CEDContactorVideocallOperation(const TDesC& aParam) : CEDContactorOperation(aParam)
    {
    // No implementation required
    }

// -----------------------------------------------------------------------------
// CEDContactorVideocallOperation::~CEDContactorVideocallOperation()
// -----------------------------------------------------------------------------
//
CEDContactorVideocallOperation::~CEDContactorVideocallOperation()
    {
    }

// -----------------------------------------------------------------------------
// CEDContactorVideocallOperation::NewLC()
// -----------------------------------------------------------------------------
//
CEDContactorVideocallOperation* CEDContactorVideocallOperation::NewLC(const TDesC& aParam)
    {
    CEDContactorVideocallOperation* self = new (ELeave)CEDContactorVideocallOperation(aParam);
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

// -----------------------------------------------------------------------------
// CEDContactorVideocallOperation::NewL()
// -----------------------------------------------------------------------------
//
CEDContactorVideocallOperation* CEDContactorVideocallOperation::NewL(const TDesC& aParam)
    {
    CEDContactorVideocallOperation* self=
            CEDContactorVideocallOperation::NewLC(aParam);
    CleanupStack::Pop(); // self;
    return self;
    }

// -----------------------------------------------------------------------------
// CEDContactorVideocallOperation::ConstructL()
// -----------------------------------------------------------------------------
//
void CEDContactorVideocallOperation::ConstructL()
    {
    
    }

// -----------------------------------------------------------------------------
// CEDContactorVideocallOperation::ExecuteLD()
// -----------------------------------------------------------------------------
//
void CEDContactorVideocallOperation::ExecuteLD()
    {
    CleanupStack::PushL(this);
    CEDPhoneCall::ExecuteL(iParam, CEDPhoneCall::ECCACallTypeVideo, ETrue );
    CleanupStack::PopAndDestroy(this);
    }
// End of File
