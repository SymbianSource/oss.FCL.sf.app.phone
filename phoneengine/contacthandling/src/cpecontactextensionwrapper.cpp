/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Implementation of CPEContactExtensionWrapper class.
*
*/


#include "cpecontactextensionwrapper.h"


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CPEContactExtensionWrapper::CPEContactExtensionWrapper
// ---------------------------------------------------------------------------
//
CPEContactExtensionWrapper::CPEContactExtensionWrapper( 
        TUid aImplementationUid )
    : 
    iPluginUid( aImplementationUid )
    {

    }


// ---------------------------------------------------------------------------
// CPEContactExtensionWrapper::ConstructL
// ---------------------------------------------------------------------------
//
void CPEContactExtensionWrapper::ConstructL()
    {
    iPlugin = CreateContactExtensionL();
    }


// ---------------------------------------------------------------------------
// CPEContactExtensionWrapper::NewL
// ---------------------------------------------------------------------------
//
CPEContactExtensionWrapper* CPEContactExtensionWrapper::NewL( 
        TUid aImplementationUid )
    {
    CPEContactExtensionWrapper* self = 
        CPEContactExtensionWrapper::NewLC( aImplementationUid );
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CPEContactExtensionWrapper::NewLC
// ---------------------------------------------------------------------------
//
CPEContactExtensionWrapper* CPEContactExtensionWrapper::NewLC( 
        TUid aImplementationUid )
    {
    CPEContactExtensionWrapper* self = 
        new( ELeave ) CPEContactExtensionWrapper( aImplementationUid );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// CPEContactExtensionWrapper::~CPEContactExtensionWrapper
// ---------------------------------------------------------------------------
//
CPEContactExtensionWrapper::~CPEContactExtensionWrapper()
    {
    delete iPlugin;
    }


// ---------------------------------------------------------------------------
// CPEContactExtensionWrapper::MatchByUid
// ---------------------------------------------------------------------------
//
TBool CPEContactExtensionWrapper::MatchByUid( 
        const TUid* aKey, const CPEContactExtensionWrapper& aWrapper )
    {
    if ( aKey )
        {
        return ( aWrapper.Identifier() == *aKey );
        }
    else
        {
        return EFalse;
        }
    }


// ---------------------------------------------------------------------------
// From class CTelMatchingExtension.
// ---------------------------------------------------------------------------
//
void CPEContactExtensionWrapper::InitializeL( 
        TUint aServiceId, const TDesC& aOrigAddress )
    {
    iPlugin->InitializeL( aServiceId, aOrigAddress );
    }


// ---------------------------------------------------------------------------
// From class CTelMatchingExtension.
// ---------------------------------------------------------------------------
//
TInt CPEContactExtensionWrapper::GetAddressForMatching( 
        RBuf& aParsedAddress, TInt& aMeaningfulDigits )
    {
    return iPlugin->GetAddressForMatching( aParsedAddress, aMeaningfulDigits );
    }


// ---------------------------------------------------------------------------
// From class CTelMatchingExtension.
// ---------------------------------------------------------------------------
//
TInt CPEContactExtensionWrapper::GetContactStoreUris( CDesCArray& aStoreUris )
    {
    return iPlugin->GetContactStoreUris( aStoreUris );
    }


// ---------------------------------------------------------------------------
// From class CTelMatchingExtension.
// ---------------------------------------------------------------------------
//
TInt CPEContactExtensionWrapper::GetRemotePartyName( RBuf& aRemotePartyName )
    {
    return iPlugin->GetRemotePartyName( aRemotePartyName );
    }


// ---------------------------------------------------------------------------
// CPEContactExtensionWrapper::CreateContactExtensionL
// Needed for testing with mock technique.
// ---------------------------------------------------------------------------
//
CTelMatchingExtension* 
    CPEContactExtensionWrapper::CreateContactExtensionL() const
    {
    return CTelMatchingExtension::NewL( iPluginUid );
    }


// ---------------------------------------------------------------------------
// CPEContactExtensionWrapper::Identifier
// ---------------------------------------------------------------------------
//
TUid CPEContactExtensionWrapper::Identifier() const
    {
    return iPluginUid;
    }
