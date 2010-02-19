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
* Description:  Implementation of the base class of contactor operations
*
*/


// INCLUDE FILES
#include "edcontactorheaders.h"


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CEDContactorOperation::CEDContactorOperation()
// -----------------------------------------------------------------------------
//
CEDContactorOperation::CEDContactorOperation(const TDesC& aParam, const TDesC& aName) : iParam(aParam), iName(aName)
    {
    }

// -----------------------------------------------------------------------------
// CEDContactorOperation::~CEDContactorOperation()
// -----------------------------------------------------------------------------
//
CEDContactorOperation::~CEDContactorOperation()
    {
    // No implementation required
    }
// End of File
