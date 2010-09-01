/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Implementation of Profile information retrieval.
*
*/


// INCLUDE FILES
#include    "CPhCntProfileEngineImpl.h"
#include    <MProfile.h>

// CONSTANTS
_LIT( KPhCntProfilePanic, "PhCntProfile" );

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhCntProfileEngineImpl::NewL
// -----------------------------------------------------------------------------
//
CPhCntProfileEngineImpl* CPhCntProfileEngineImpl::NewL()
    {
    CPhCntProfileEngineImpl* self = 
        new ( ELeave ) CPhCntProfileEngineImpl;
    
    return self;
    }

// -----------------------------------------------------------------------------
// CPhCntProfileEngineImpl::~CPhCntProfileEngineImpl
// -----------------------------------------------------------------------------
//      
CPhCntProfileEngineImpl::~CPhCntProfileEngineImpl()
    {
    if ( iProfile )
        {
        iProfile->Release();
        iProfile = NULL;
        }

    if ( iEngine )
        {
        iEngine->Release();
        iEngine = NULL;
        }
    }

// -----------------------------------------------------------------------------
// CPhCntProfileEngineImpl::RefreshL
// -----------------------------------------------------------------------------
//
void CPhCntProfileEngineImpl::RefreshL()
    {
    if ( !iEngine )
        {
        iEngine = CreateProfileEngineL();
        }

    MProfile* profile = iEngine->ActiveProfileL();

    if ( iProfile )
        {
        iProfile->Release();
        iProfile = NULL;
        }
    iProfile = profile;
    }

// -----------------------------------------------------------------------------
// CPhCntProfileEngineImpl::HasProfile
// -----------------------------------------------------------------------------
//
TBool CPhCntProfileEngineImpl::HasProfile() const
    {
    return iProfile != NULL;
    }

// -----------------------------------------------------------------------------
// CPhCntProfileEngineImpl::Profile
// -----------------------------------------------------------------------------
//
MProfile& CPhCntProfileEngineImpl::Profile()
    {
    __ASSERT_ALWAYS( iProfile, User::Panic( KPhCntProfilePanic, 0 ) );
    return *iProfile;
    }

// -----------------------------------------------------------------------------
// CPhCntProfileEngineImpl::SetActiveProfileL
// -----------------------------------------------------------------------------
//
void CPhCntProfileEngineImpl::SetActiveProfileL( const TInt aProfileId )
    {
    if ( !iEngine )
        {
        iEngine = CreateProfileEngineL();
        }

    iEngine->SetActiveProfileL( aProfileId );
    }

// -----------------------------------------------------------------------------
// CPhCntProfileEngineImpl::IsFeatureSupported
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CPhCntProfileEngineImpl::IsFeatureSupported(
        TProfileFeatureId aFeatureId ) const
    {
    return iEngine->IsFeatureSupported( aFeatureId );
    }

//  End of File  
