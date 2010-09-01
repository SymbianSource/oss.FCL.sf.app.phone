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
* Description:  Implementation of unieditor (SMS/MMS) operation
*
*/


// INCLUDE FILES
#include "edcontactorheaders.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CEDContactorUniEditorOperation::CEDContactorUniEditorOperation()
// -----------------------------------------------------------------------------
//
CEDContactorUniEditorOperation::CEDContactorUniEditorOperation(const TDesC& aParam, const TDesC& aName) : 
    CEDContactorOperation(aParam, aName)
    {
    // No implementation required
    }

// -----------------------------------------------------------------------------
// CEDContactorUniEditorOperation::~CEDContactorUniEditorOperation()
// -----------------------------------------------------------------------------
//
CEDContactorUniEditorOperation::~CEDContactorUniEditorOperation()
    {
    }

// -----------------------------------------------------------------------------
// CEDContactorUniEditorOperation::NewLC()
// -----------------------------------------------------------------------------
//
CEDContactorUniEditorOperation* CEDContactorUniEditorOperation::NewLC(const TDesC& aParam, const TDesC& aName)
    {
    CEDContactorUniEditorOperation* self = new (ELeave)CEDContactorUniEditorOperation(aParam, aName);
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

// -----------------------------------------------------------------------------
// CEDContactorUniEditorOperation::NewL()
// -----------------------------------------------------------------------------
//
CEDContactorUniEditorOperation* CEDContactorUniEditorOperation::NewL(const TDesC& aParam, const TDesC& aName)
    {
    CEDContactorUniEditorOperation* self =
            CEDContactorUniEditorOperation::NewLC( aParam, aName );
    CleanupStack::Pop(); // self;
    return self;
    }

// -----------------------------------------------------------------------------
// CEDContactorUniEditorOperation::ConstructL()
// -----------------------------------------------------------------------------
//
void CEDContactorUniEditorOperation::ConstructL()
    {

    }

// -----------------------------------------------------------------------------
// CEDContactorUniEditorOperation::ExecuteLD()
// -----------------------------------------------------------------------------
//
void CEDContactorUniEditorOperation::ExecuteLD()
    {
    CleanupStack::PushL(this);
    CEDMsgEditors::ExecuteL( KSenduiMtmUniMessageUid, iParam, iName );
    CleanupStack::PopAndDestroy(this);
    }
// End of File
