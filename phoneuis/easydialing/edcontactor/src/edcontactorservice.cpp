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
* Description:  This is a class for starting services
*
*/

#include <e32std.h>

#include "edcontactorserviceheaders.h"

_LIT(KContactorResourceFile, "\\resource\\edcontactor.rsc");


// ================= MEMBER FUNCTIONS =======================

// --------------------------------------------------------------------------
// CEDContactorService::ExecuteServiceL
// --------------------------------------------------------------------------
//
EXPORT_C CEDContactorService* CEDContactorService::NewL( MEDContactorObserver* aObserver )
    {
    CEDContactorService* self = new (ELeave) CEDContactorService(aObserver);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// --------------------------------------------------------------------------
// Destructor
// --------------------------------------------------------------------------
//
CEDContactorService::~CEDContactorService()
    {
    delete iOperator;
    
    CCoeEnv* coe = CCoeEnv::Static();
    if( coe )
        {
        coe->DeleteResourceFile( iResourceFileOffset );
        }
    }

// --------------------------------------------------------------------------
// Constructor
// --------------------------------------------------------------------------
//
CEDContactorService::CEDContactorService( MEDContactorObserver* aObserver ) :
iObserver(aObserver)
    {
    }

// --------------------------------------------------------------------------
// CEDContactorService::ConstructL
// --------------------------------------------------------------------------
//
void CEDContactorService::ConstructL()
    {
    CCoeEnv* coe = CCoeEnv::Static();    
    if( coe )
        {
        TFileName dllFileName;
        Dll::FileName( dllFileName );
        
        TParse parse;
        User::LeaveIfError(parse.Set(KContactorResourceFile, &dllFileName, NULL));
        TFileName resourceFileName(parse.FullName());
         
        iResourceFileOffset = coe->AddResourceFileL( resourceFileName );
        }
    
    iOperator = CEDContactorServiceOperator::NewL( iObserver );
    }

// --------------------------------------------------------------------------
// CEDContactorService::ExecuteServiceL
// --------------------------------------------------------------------------
//
EXPORT_C void CEDContactorService::ExecuteServiceL(
    const TCSParameter& aParameter)
    {
    LOGSTRING("CEDContactorService::ExecuteServiceL(): enter");

    iCommandIsBeingHandled = ETrue;

    // All leaves are trapped in op class.
    iOperator->Execute(aParameter);

    iCommandIsBeingHandled = EFalse;

    LOGSTRING("CEDContactorService::ExecuteServiceL(): exit");
    }

// --------------------------------------------------------------------------
// CEDContactorService::CancelService
// --------------------------------------------------------------------------
//
EXPORT_C void CEDContactorService::CancelService()
    {
    if ( iOperator )
        {
        iOperator->Cancel();
        }
    }

// --------------------------------------------------------------------------
// CEDContactorService::IsBusy
// --------------------------------------------------------------------------
//
EXPORT_C TBool CEDContactorService::IsBusy() const
    {
    return iCommandIsBeingHandled;
    }

// End of file
