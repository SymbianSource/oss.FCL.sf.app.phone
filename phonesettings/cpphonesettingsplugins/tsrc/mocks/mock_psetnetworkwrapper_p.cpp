/* 
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:
*
*/
#include <rmmcustomapi.h>
#include <PsetContainer.h>
#include <PsetNetwork.h>
#include <smcmockclassincludes.h>
#include "psetnetworkwrapper_p.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// PSetNetworkWrapperPrivate::PSetNetworkWrapperPrivate
// -----------------------------------------------------------------------------
//
PSetNetworkWrapperPrivate::PSetNetworkWrapperPrivate( 
        PSetNetworkWrapper & owner,
        CPsetContainer & psetContainer )
    : m_owner(owner)
    {
    Q_UNUSED(psetContainer)
    }


// -----------------------------------------------------------------------------
// PSetNetworkWrapperPrivate::~PSetNetworkWrapperPrivate
// -----------------------------------------------------------------------------
//
PSetNetworkWrapperPrivate::~PSetNetworkWrapperPrivate(  )
    {
    
    }


// -----------------------------------------------------------------------------
// PSetNetworkWrapperPrivate::getAvailableNetworks
// -----------------------------------------------------------------------------
//
void PSetNetworkWrapperPrivate::getAvailableNetworks(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// PSetNetworkWrapperPrivate::getNetworkSelectionMode
// -----------------------------------------------------------------------------
//
PSetNetworkWrapper::ErrorCode PSetNetworkWrapperPrivate::getNetworkSelectionMode( 
        PSetNetworkWrapper::NetworkSelectionMode & mode ) const
    {
    SMC_MOCK_METHOD1( PSetNetworkWrapper::ErrorCode, PSetNetworkWrapper::NetworkSelectionMode &, mode )
    }


// -----------------------------------------------------------------------------
// PSetNetworkWrapperPrivate::selectNetwork
// -----------------------------------------------------------------------------
//
void PSetNetworkWrapperPrivate::selectNetwork( 
        const PSetNetworkWrapper::NetworkInfo & networkInfo )
    {
    SMC_MOCK_METHOD1( void, const PSetNetworkWrapper::NetworkInfo &, networkInfo )
    }


// -----------------------------------------------------------------------------
// PSetNetworkWrapperPrivate::cancelRequest
// -----------------------------------------------------------------------------
//
void PSetNetworkWrapperPrivate::cancelRequest(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// PSetNetworkWrapperPrivate::getNetworkAccessMode
// -----------------------------------------------------------------------------
//
void PSetNetworkWrapperPrivate::getNetworkAccessMode(  ) const
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// PSetNetworkWrapperPrivate::setNetworkAccessMode
// -----------------------------------------------------------------------------
//
void PSetNetworkWrapperPrivate::setNetworkAccessMode( 
        PSetNetworkWrapper::NetworkAccessMode mode )
    {
    SMC_MOCK_METHOD1( void, PSetNetworkWrapper::NetworkAccessMode, mode )
    }


// -----------------------------------------------------------------------------
// PSetNetworkWrapperPrivate::HandleNetworkInfoReceivedL
// -----------------------------------------------------------------------------
//
void PSetNetworkWrapperPrivate::HandleNetworkInfoReceivedL( 
        const CNetworkInfoArray * aInfoArray,
        const TInt aResult )
    {
    SMC_MOCK_METHOD2( void, const CNetworkInfoArray *, aInfoArray, 
        const TInt, aResult )
    }


// -----------------------------------------------------------------------------
// PSetNetworkWrapperPrivate::HandleCurrentNetworkInfoL
// -----------------------------------------------------------------------------
//
void PSetNetworkWrapperPrivate::HandleCurrentNetworkInfoL( 
        const MPsetNetworkSelect::TCurrentNetworkInfo & aCurrentInfo,
        const TInt aResult )
    {
    SMC_MOCK_METHOD2( void, const MPsetNetworkSelect::TCurrentNetworkInfo &, aCurrentInfo, 
        const TInt, aResult )
    }


// -----------------------------------------------------------------------------
// PSetNetworkWrapperPrivate::HandleNetworkChangedL
// -----------------------------------------------------------------------------
//
void PSetNetworkWrapperPrivate::HandleNetworkChangedL( 
        const MPsetNetworkSelect::TNetworkInfo & aCurrentInfo,
        const MPsetNetworkSelect::TCurrentNetworkStatus aStatus,
        const TInt aResult )
    {
    SMC_MOCK_METHOD3( void, const MPsetNetworkSelect::TNetworkInfo &, aCurrentInfo, 
        const MPsetNetworkSelect::TCurrentNetworkStatus, aStatus, 
        const TInt, aResult )
    }


// -----------------------------------------------------------------------------
// PSetNetworkWrapperPrivate::HandleNetworkChangedL
// -----------------------------------------------------------------------------
//
void PSetNetworkWrapperPrivate::HandleNetworkChangedL( 
        const MPsetNetworkSelect::TNetworkInfo & aCurrentInfo,
        const RMobilePhone::TMobilePhoneRegistrationStatus & aStatus,
        const TInt aResult )
    {
    SMC_MOCK_METHOD3( void, const MPsetNetworkSelect::TNetworkInfo &, aCurrentInfo, 
        const RMobilePhone::TMobilePhoneRegistrationStatus &, aStatus, 
        const TInt, aResult )
    }


// -----------------------------------------------------------------------------
// PSetNetworkWrapperPrivate::HandleSearchingNetworksL
// -----------------------------------------------------------------------------
//
void PSetNetworkWrapperPrivate::HandleSearchingNetworksL( 
        MPsetNetworkInfoObserver::TServiceRequest aRequest )
    {
    SMC_MOCK_METHOD1( void, MPsetNetworkInfoObserver::TServiceRequest, aRequest )
    }


// -----------------------------------------------------------------------------
// PSetNetworkWrapperPrivate::HandleRequestingSelectedNetworkL
// -----------------------------------------------------------------------------
//
void PSetNetworkWrapperPrivate::HandleRequestingSelectedNetworkL( 
        TBool aOngoing )
    {
    SMC_MOCK_METHOD1( void, TBool, aOngoing )
    }


// -----------------------------------------------------------------------------
// PSetNetworkWrapperPrivate::HandleCallActivatedL
// -----------------------------------------------------------------------------
//
void PSetNetworkWrapperPrivate::HandleCallActivatedL(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// PSetNetworkWrapperPrivate::HandleNetworkErrorL
// -----------------------------------------------------------------------------
//
void PSetNetworkWrapperPrivate::HandleNetworkErrorL( 
        const MPsetNetworkInfoObserver::TServiceRequest aRequest,
        const TInt aError )
    {
    SMC_MOCK_METHOD2( void, const MPsetNetworkInfoObserver::TServiceRequest, aRequest, 
        const TInt, aError )
    }


// -----------------------------------------------------------------------------
// PSetNetworkWrapperPrivate::HandleNetworkSystemModeEventsL
// -----------------------------------------------------------------------------
//
void PSetNetworkWrapperPrivate::HandleNetworkSystemModeEventsL( 
        const MPsetNetworkModeObserver::TServiceRequest aRequest,
        const TUint32 aNetworkModeCaps )
    {
    SMC_MOCK_METHOD2( void, const MPsetNetworkModeObserver::TServiceRequest, aRequest, 
        const TUint32, aNetworkModeCaps )
    }


// -----------------------------------------------------------------------------
// PSetNetworkWrapperPrivate::HandleNetworkErrorL
// -----------------------------------------------------------------------------
//
void PSetNetworkWrapperPrivate::HandleNetworkErrorL( 
        const MPsetNetworkModeObserver::TServiceRequest aRequest,
        const TInt aError )
    {
    SMC_MOCK_METHOD2( void, const MPsetNetworkModeObserver::TServiceRequest, aRequest, 
        const TInt, aError )
    }


