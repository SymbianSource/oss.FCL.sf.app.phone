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
* Description:  Implementation of the im operation
*
*/


// INCLUDE FILES

#include "edcontactorheaders.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CEDContactorIMOperation::CEDContactorIMOperation()
// -----------------------------------------------------------------------------
//
CEDContactorIMOperation::CEDContactorIMOperation(const TDesC& aParam) : CEDContactorOperation(aParam)
    {
    iPlugin = NULL;
    }

// -----------------------------------------------------------------------------
// CEDContactorIMOperation::~CEDContactorIMOperation()
// -----------------------------------------------------------------------------
//
CEDContactorIMOperation::~CEDContactorIMOperation()
    {
    delete iPlugin;
    iPlugin = NULL;
    REComSession::FinalClose();   
    delete iSPSettings;
    }

// -----------------------------------------------------------------------------
// CEDContactorIMOperation::NewLC()
// -----------------------------------------------------------------------------
//
CEDContactorIMOperation* CEDContactorIMOperation::NewLC(const TDesC& aParam)
    {
    CEDContactorIMOperation* self = new (ELeave)CEDContactorIMOperation(aParam);
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

// -----------------------------------------------------------------------------
// CEDContactorIMOperation::NewL()
// -----------------------------------------------------------------------------
//
CEDContactorIMOperation* CEDContactorIMOperation::NewL(const TDesC& aParam)
    {
    CEDContactorIMOperation* self=CEDContactorIMOperation::NewLC(aParam);
    CleanupStack::Pop(); // self;
    return self;
    }

// -----------------------------------------------------------------------------
// CEDContactorIMOperation::ConstructL()
// -----------------------------------------------------------------------------
//
void CEDContactorIMOperation::ConstructL()
    {
    iSPSettings = CSPSettings::NewL();
    }

// -----------------------------------------------------------------------------
// CEDContactorIMOperation::ExecuteLD()
// -----------------------------------------------------------------------------
//
void CEDContactorIMOperation::ExecuteLD()
    {
    CleanupStack::PushL(this);
    TPtrC aXspId = iParam;

    TUid dllUid = ResolveEcomUidL( ExtractService(aXspId)  );    
    LoadEcomL( dllUid, aXspId );
    CleanupStack::PopAndDestroy(this);
    }

// --------------------------------------------------------------------------
// CEDContactorIMOperation::ResolveEcomUidL
// --------------------------------------------------------------------------
//
TUid CEDContactorIMOperation::ResolveEcomUidL( const TDesC& aServiceId )
    {    
    // Resolve ECom UID from SP settings
    TInt launchId = KErrNotFound;  
               
    launchId = DoResolveEcomUidL( aServiceId ); 
    TUid launchUid = {launchId };    
    return launchUid;
    }

// --------------------------------------------------------------------------
// CEDContactorIMOperation::DoResolveEcomUidL
// --------------------------------------------------------------------------
//
TInt CEDContactorIMOperation::DoResolveEcomUidL( const TDesC& aServiceId )
    {    
    // Resolve ECom UID from SP settings
    TInt err = KErrNone;
    TInt launchId = KErrNotFound;  
    CDesCArrayFlat* nameArray = NULL;      
    
    RIdArray ids;
    CleanupClosePushL( ids );
    
    nameArray = new (ELeave) CDesCArrayFlat(2);
    CleanupStack::PushL( nameArray );    
    
    err = iSPSettings->FindServiceIdsL( ids );  
    User::LeaveIfError( err );     
    err = iSPSettings->FindServiceNamesL( ids, *nameArray );  
    User::LeaveIfError( err );     
    
    TInt count = nameArray->MdcaCount();
    for ( TInt i(0); i < count; i++)
        {
        // search the mathching service name
        TPtrC p = nameArray->MdcaPoint( i );
        if (!p.CompareF( aServiceId ))
            {
            // We have found the service, now get the plugin id
            CSPProperty* IMLaunchIdProperty = CSPProperty::NewLC();
            err = iSPSettings->FindPropertyL( ids[i], ESubPropertyIMLaunchUid, *IMLaunchIdProperty );
            User::LeaveIfError( err );               
            err = IMLaunchIdProperty->GetValue( launchId );
            User::LeaveIfError( err ); 
            CleanupStack::PopAndDestroy( IMLaunchIdProperty );            
            break;
            }
        }    
    CleanupStack::PopAndDestroy( nameArray );
    CleanupStack::PopAndDestroy( ); // >>> ids
    
    return launchId; 
    }
    
// --------------------------------------------------------------------------
// CEDContactorIMOperation::LoadEcomL
// --------------------------------------------------------------------------
//
void CEDContactorIMOperation::LoadEcomL( TUid aUidImp, const TDesC& aXspId  )
    {         
    TUid destructorId;

    TAny* volatile implementation =
        REComSession::CreateImplementationL( aUidImp,
                                             destructorId );
                                             
    iPlugin = reinterpret_cast< CCmsContactorImPluginBase* >( implementation );    
    iPlugin->SetDestructorId( destructorId );    
    iPlugin->ExecuteL( aXspId );    
    }

// --------------------------------------------------------------------------
// CEDContactorIMOperation::ExtractService
// --------------------------------------------------------------------------
//
TPtrC CEDContactorIMOperation::ExtractService( const TDesC& aXspId )
    {         
    TInt pos = aXspId.Find(KColon);
    if ( pos >= 0)
        {
        // ok input
        return aXspId.Left(pos);
        }
    else
        {
        // return something in illegal input case
        return TPtrC(KNullDesC);
        }
    }
// End of File
