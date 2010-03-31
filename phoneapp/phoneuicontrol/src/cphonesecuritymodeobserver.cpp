/*
* Copyright (c) 2005-2010 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: Implementation of CPhoneSecurityModeState class.
*
*/

#include "cphonesecuritymodeobserver.h"
#include "mphonesecuritymodechangeobserver.h"
#include "cphonelogger.h"
#include "CPhonePubSubProxy.h"

#include <mpeengineinfo.h>
#include <featmgr.h>
#include <e32cmn.h>

// -----------------------------------------------------------
// CPhoneSecurityModeObserver::NewL
// -----------------------------------------------------------
CPhoneSecurityModeObserver* CPhoneSecurityModeObserver::NewL()
	{
	return new( ELeave )CPhoneSecurityModeObserver;
	}

// -----------------------------------------------------------
// CPhoneSecurityModeObserver::NewLC
// -----------------------------------------------------------
CPhoneSecurityModeObserver* CPhoneSecurityModeObserver::NewLC()
	{
	CPhoneSecurityModeObserver* self = new( ELeave )CPhoneSecurityModeObserver;
	CleanupStack::PushL( self );
	return self;
	}

// -----------------------------------------------------------
// CPhoneSecurityModeObserver::~CPhoneSecurityModeObserver
// -----------------------------------------------------------
CPhoneSecurityModeObserver::~CPhoneSecurityModeObserver()
	{
	iObserverArray.Reset();
	iObserverArray.Close();
	}

// -----------------------------------------------------------
// CPhoneSecurityModeObserver::SetPhoneEngineInfo
// -----------------------------------------------------------
void CPhoneSecurityModeObserver::SetPhoneEngineInfo( MPEEngineInfo* aEngineInfo )
	{
	iEngineInfo = aEngineInfo;
	}

// -----------------------------------------------------------
// CPhoneSecurityModeObserver::RegisterStateObserver
// -----------------------------------------------------------
void CPhoneSecurityModeObserver::RegisterStateObserver( MPhoneSecurityModeChangeObserver& aObserver )
	{
	iObserverArray.Append( &aObserver );
	if ( iInitialized )
		{
		// Notify observer with registration call, if state instance is initialized.
		aObserver.HandleSecurityModeChanged( iSecurityMode );
		}
	}
	
// -----------------------------------------------------------
// CPhoneSecurityModeObserver::RemoveStateObserver
// -----------------------------------------------------------
void CPhoneSecurityModeObserver::RemoveStateObserver( MPhoneSecurityModeChangeObserver& aObserver )
	{
	TInt index = iObserverArray.Find( &aObserver );
	if ( KErrNotFound != index )
		{
		iObserverArray.Remove( index );
		}
	}

// -----------------------------------------------------------
// CPhoneSecurityModeObserver::IsAutolockEnabled
// -----------------------------------------------------------
TBool CPhoneSecurityModeObserver::IsAutolockEnabled()
	{
	// Autolock is enabled in uninitialized state also.
	return ( iDeviceLockState > EAutolockOff
		|| EAutolockStatusUninitialized == iDeviceLockState );
	}

// -----------------------------------------------------------
// CPhoneSecurityModeObserver::IsSecurityMode
// -----------------------------------------------------------
TBool CPhoneSecurityModeObserver::IsSecurityMode()
	{
	return iSecurityMode;
	}

// -----------------------------------------------------------
// CPhoneSecurityModeObserver::HandlePhoneEngineMessageL
// -----------------------------------------------------------
void CPhoneSecurityModeObserver::HandlePhoneEngineMessageL( const TInt aMessage,
	TInt /*aCallId*/ )
	{
	TBool previousSecurityMode = iSecurityMode;
	switch( aMessage )
		{
		case MEngineMonitor::EPEMessageSIMStateChanged:
			{
			__PHONELOG( EBasic,
				EPhoneControl,
				"CPhoneSecurityModeObserver::HandlePhoneEngineMessageL() " );
	
			// Register new sim states.
			iEnginePreviousSimState = iEngineCurrentSimState;
			if ( iEngineInfo )
				{
				iEngineCurrentSimState = iEngineInfo->SimState();
				DefineSecurityModeState();
				}
			}
		break;
		}
		
	// Notify observers for state change.
	if ( previousSecurityMode != iSecurityMode )
		{
		HandleStateChange();
		}
	}

// -----------------------------------------------------------
// CPhoneSecurityModeObserver::HandlePropertyChangedL
// -----------------------------------------------------------
void CPhoneSecurityModeObserver::HandlePropertyChangedL(const TUid& aCategory,
	const TUint aKey,
	const TInt aValue )
	{
	__PHONELOG( EBasic,
		EPhoneControl,
		"CPhoneSecurityModeObserver::HandlePropertyChangedL() " );
	
	// Register sim security and autolock value changes.
	TBool previousSecurityMode = iSecurityMode;
	if ( KPSUidStartup == aCategory && KStartupSimSecurityStatus == aKey )
		{
		iSimState = ( TPSSimSecurityStatus )aValue;
		DefineSecurityModeState();
		}
	else if ( KPSUidCoreApplicationUIs == aCategory 
	    && KCoreAppUIsAutolockStatus == aKey )
		{
		iDeviceLockState = ( TPSAutolockStatus )aValue;
		DefineSecurityModeState();
		}
		
	// Notify observer for state change.
	if ( previousSecurityMode != iSecurityMode )
		{
		HandleStateChange();
		}
	}

// -----------------------------------------------------------
// CPhoneSecurityModeObserver::Initialize
// -----------------------------------------------------------
void CPhoneSecurityModeObserver::Initialize()
	{
	__PHONELOG1( EBasic,
		EPhoneControl,
		"CPhoneSecurityModeObserver::Initialize() %d ", iSecurityMode );
	if ( !iInitialized )
		{
		if ( !iEngineInfo )
			{
			// Do not initialize if engine info data interface hasn't been set.
			return;
			}
			
		// Get current values for sim states and autolock value.
		iEngineCurrentSimState = iEngineInfo->SimState();
		iSimState = ( TPSSimSecurityStatus )CPhonePubSubProxy::Instance()->Value(
			KPSUidStartup,
			KStartupSimSecurityStatus );
		iDeviceLockState = ( TPSAutolockStatus )CPhonePubSubProxy::Instance()->Value(
			KPSUidCoreApplicationUIs,
			KCoreAppUIsAutolockStatus );
		iInitialized = ETrue;
		DefineSecurityModeState();
		HandleStateChange();
		}
		
	}

// -----------------------------------------------------------
// CPhoneSecurityModeObserver::CPhoneSecurityModeObserver
// -----------------------------------------------------------
CPhoneSecurityModeObserver::CPhoneSecurityModeObserver() :
	iSimState( ESimSecurityStatusUninitialized ),
	iEngineCurrentSimState( EPESimStatusUninitialized ),
	iEnginePreviousSimState(EPESimStatusUninitialized ),
	iDeviceLockState( EAutolockStatusUninitialized )
	{
	
	}

// -----------------------------------------------------------
// CPhoneSecurityModeObserver::DefineSecurityModeState
// -----------------------------------------------------------
void CPhoneSecurityModeObserver::DefineSecurityModeState()
	{
	// Device lock 
	if ( IsAutolockEnabled() )
		{
		iSecurityMode = ETrue;
		}
	
	// Sim security status
	else if ( IsSimSecurityStatus() )
		{
		iSecurityMode = ETrue;
		}
		
	// Phone engine sim state
	else if ( !IsEngineSimStateAccepted() )
		{
		iSecurityMode = ETrue;
		}
	
	else
		{
		iSecurityMode = EFalse;
		}
	
	
	__PHONELOG1( EBasic,
		EPhoneControl,
		"CPhoneSecurityModeObserver::DefineSecurityModeState(), iEngineCurrentSimState( %d )",
		iEngineCurrentSimState );
		
	__PHONELOG1( EBasic,
		EPhoneControl,
		"CPhoneSecurityModeObserver::DefineSecurityModeState(), iEnginePreviousSimState( %d )",
		iEnginePreviousSimState );
		
	__PHONELOG1( EBasic,
		EPhoneControl,
		"CPhoneSecurityModeObserver::DefineSecurityModeState(), iSimState( %d )",
		iSimState );
		
	__PHONELOG1( EBasic,
		EPhoneControl,
		"CPhoneSecurityModeObserver::DefineSecurityModeState(), iDeviceLockState( %d )",
		iDeviceLockState );
		
	__PHONELOG1( EBasic,
		EPhoneControl,
		"CPhoneSecurityModeObserver::DefineSecurityModeState(), iSecurityMode( %d )",
		iSecurityMode );
	}

// -----------------------------------------------------------
// CPhoneSecurityModeObserver::HandleStateChange
// -----------------------------------------------------------
void CPhoneSecurityModeObserver::HandleStateChange()
	{
	__PHONELOG2( EBasic,
		EPhoneControl,
		"CPhoneSecurityModeObserver::HandleStateChange() %d, %d observers ", iSecurityMode, iObserverArray.Count() );
	if ( iInitialized )
		{
		for( TInt i( 0 ); i < iObserverArray.Count(); i++ )
			{
			MPhoneSecurityModeChangeObserver& observer = *( iObserverArray[ i ] );
			observer.HandleSecurityModeChanged( iSecurityMode );
			}
		}
	}

// -----------------------------------------------------------
// CPhoneSecurityModeObserver::HandleStateChange
// -----------------------------------------------------------
TBool CPhoneSecurityModeObserver::IsSimSecurityStatus()
	{
	TBool securityStatus( EFalse );
	if ( ESimRejected == iSimState )
		{
		securityStatus = ETrue;
		}
	else if ( ESimUnaccepted == iSimState )
		{
		securityStatus = ETrue;
		}
	else if ( ESimInvalid == iSimState )
		{
		securityStatus = ETrue;
		}
	return securityStatus;
	}
	
// -----------------------------------------------------------
// CPhoneSecurityModeObserver::HandleStateChange
// -----------------------------------------------------------
TBool CPhoneSecurityModeObserver::IsEngineSimStateAccepted()
	{
	TBool accepted( ETrue );
	if ( ( EPESimNotPresent == iEngineCurrentSimState )
		&& ( ( EPESimUsable == iEnginePreviousSimState )
		|| ( EPESimReadable == iEnginePreviousSimState )
		|| ( EPESimNotReady == iEnginePreviousSimState ) ) )
		{
		accepted = EFalse;
		}
	else if ( EPESimNotSupported == iEngineCurrentSimState )
		{
		accepted = EFalse;
		}
	else if ( EPESimNotPresent == iEngineCurrentSimState &&
		!AreSimlessFeaturesSupported() )
		{
		accepted = EFalse;
		}
	return accepted;
	}
	
// -----------------------------------------------------------
// CPhoneSecurityModeObserver::HandleStateChange
// -----------------------------------------------------------
TBool CPhoneSecurityModeObserver::AreSimlessFeaturesSupported()
	{
	return FeatureManager::FeatureSupported( KFeatureIdOfflineMode )
		&&FeatureManager::FeatureSupported( KFeatureIdFfSimlessOfflineSupport );
	}
	

// END OF FILE
