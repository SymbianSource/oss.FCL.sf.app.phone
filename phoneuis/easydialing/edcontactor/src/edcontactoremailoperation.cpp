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
* Description:  Implementation of the email operation
*
*/


// INCLUDE FILES
#include "edcontactorheaders.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CEDContactorEmailOperation::CEDContactorEmailOperation()
// -----------------------------------------------------------------------------
//
CEDContactorEmailOperation::CEDContactorEmailOperation(const TDesC& aParam, const TDesC& aName) : 
    CEDContactorOperation(aParam, aName)
    {
    }

// -----------------------------------------------------------------------------
// CEDContactorEmailOperation::~CEDContactorEmailOperation()
// -----------------------------------------------------------------------------
//
CEDContactorEmailOperation::~CEDContactorEmailOperation()
    {
    // No implementation required
    }

// -----------------------------------------------------------------------------
// CEDContactorEmailOperation::NewLC()
// -----------------------------------------------------------------------------
//
CEDContactorEmailOperation* CEDContactorEmailOperation::NewLC(const TDesC& aParam, const TDesC& aName)
    {
    CEDContactorEmailOperation* self = new (ELeave)CEDContactorEmailOperation(aParam, aName);
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

// -----------------------------------------------------------------------------
// CEDContactorEmailOperation::NewL()
// -----------------------------------------------------------------------------
//
CEDContactorEmailOperation* CEDContactorEmailOperation::NewL(const TDesC& aParam, const TDesC& aName)
    {
    CEDContactorEmailOperation* self=CEDContactorEmailOperation::NewLC(aParam, aName);
    CleanupStack::Pop(); // self;
    return self;
    }

// -----------------------------------------------------------------------------
// CEDContactorEmailOperation::ConstructL()
// -----------------------------------------------------------------------------
//
void CEDContactorEmailOperation::ConstructL()
    {

    }

// -----------------------------------------------------------------------------
// CEDContactorEmailOperation::ExecuteLD()
// -----------------------------------------------------------------------------
//
void CEDContactorEmailOperation::ExecuteLD()
    {
    CleanupStack::PushL(this);
    CEDMsgEditors::ExecuteL(KSenduiMtmSmtpUid, iParam,
                iName);
    CleanupStack::PopAndDestroy(this);
    }
// Enf of File
