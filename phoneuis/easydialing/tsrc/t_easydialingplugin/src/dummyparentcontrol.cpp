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
* Description:  EUnit test file.
*
*/

// INCLUDE FILES
#include "dummyparentcontrol.h"

// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES

// CONSTANTS

// MACROS

// LOCAL CONSTANTS AND MACROS

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

CDummyParentControl* CDummyParentControl::NewL()
    {
    
    CDummyParentControl* self = new (ELeave) CDummyParentControl();
    
    self->ConstructL();
    
    return self;
    }
    

CDummyParentControl::CDummyParentControl() : CCoeControl()
    {
    
    }
    
CDummyParentControl::~CDummyParentControl()
    {
    }
    
void CDummyParentControl::ConstructL()
    {
    CreateWindowL();
    }

