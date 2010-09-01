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
* Description:
*     A dynamic state class for security mode.
*
*/

#ifndef CPHONESECURITYMODEOBSERVER_H
#define CPHONESECURITYMODEOBSERVER_H

#include "mphonesecuritymodeobserver.h"
#include "mphonesecuritymessagehandler.h"
#include "mphonesecuritymodechangeobserver.h"
#include <e32base.h>
#include <pevirtualengine.h>
#include <startupdomainpskeys.h>
#include <coreapplicationuisdomainpskeys.h>
#include <e32cmn.h>

// Forward Declarations
class MPEEngineInfo;

/**
 * Dynamic state class for security mode.
 * 
 */
NONSHARABLE_CLASS( CPhoneSecurityModeObserver ) : public CBase,
	public MPhoneSecurityModeObserver,
	public MPhoneSecurityMessageHandler
	{
public:
	
	/**
	 * Default static two-phased constructor.
	 * 
	 * @return Pointer to new instance.
	 */
	static CPhoneSecurityModeObserver* NewL();
	
	/**
	 * Default static two-phased constructor.
	 * 
	 * @return Pointer to new instance.
	 */
	static CPhoneSecurityModeObserver* NewLC();
	
	/**
	 * Destructor.
	 */
	~CPhoneSecurityModeObserver();
	
	/**
	 * Sets pointer to phone engine info interface.
	 * 
	 * @param aEngineInfo Pointer to engine info interface.
	 */
	void SetPhoneEngineInfo( MPEEngineInfo* aEngineInfo );
	
	// From MPhoneSecurityModeState
	
	/**
	 * Register state observer.
	 * 
	 * @param aObserver Reference to state observer instance.
	 */
	IMPORT_C void RegisterStateObserver( MPhoneSecurityModeChangeObserver& aObserver );
	
	/**
	 * Remove state observer
	 * 
	 * @param aObserver Reference to state observer instance.
	 */
	IMPORT_C void RemoveStateObserver( MPhoneSecurityModeChangeObserver& aObserver );
	
	/**
	 * Checks if autolock is enabled.
	 * 
	 * @return ETrue if enabled. EFalse otherwise.
	 */
	IMPORT_C TBool IsAutolockEnabled();
	
	/**
	 * Checks if security mode is enabled.
	 * 
	 * @return ETrue if enabled. EFalse otherwise.
	 */
	IMPORT_C TBool IsSecurityMode();
	
	/**
	 * Initializes state instance. This should be called when dynamic security
	 * mode observers are ready.
	 */
	IMPORT_C void Initialize();
	
	/**
	 * Handles phone engine message. Dynamic state object listens phone engine
	 * message to determinate sim states.
	 * 
	 * @param aMessage Message enumeration.
	 * @param aCallId Call identifier.
	 */
	void HandlePhoneEngineMessageL( const TInt aMessage, TInt aCallId );
	
	/**
	 * Handle property change. Dynamic state object listens P&S key changes
	 * to determinate autolock and sim security states.
	 * 
	 * @param aCategory Unique identifer category reference.
	 * @param aKey Key as unsigned integer.
	 * @param aValue Key value as integer.
	 */
	void HandlePropertyChangedL(const TUid& aCategory,
	     const TUint aKey,
	     const TInt aValue );
	
private:
	
	/**
	 * Default C++ constructor.
	 */
	CPhoneSecurityModeObserver();
	
	/**
	 * Defines iSecurityMode with corresponding logic implementation.
	 */
	void DefineSecurityModeState();
	
	/**
	 * Handles security mode state change. Basicly calls observer functions.
	 */
	void HandleStateChange();
	
	/**
	 * Checks if sim security status is enabled.
	 * 
	 * @return ETrue if enabled.
	 */
	TBool IsSimSecurityStatus();
	
	/**
	 * Checks if sim security status is accepted, not in security mode.
	 * 
	 * @return ETrue if not in security mode.
	 */
	TBool IsEngineSimStateAccepted();
	
	/**
	 * Checks if simless features are supported.
	 * 
	 * @return ETrue if features are supported.
	 */
	TBool AreSimlessFeaturesSupported();
	
private:
	
    // Phone sim security state. Used to define dialer security mode and
    // usability restrictions dynamically.
    TPSSimSecurityStatus iSimState;
    
    // Telephony engine sim state. Used to define dialer security mode and
    // usability restrictions dynamically.
    TPESimState iEngineCurrentSimState;
    
    // Some special cases requires that we know previous sim state to 
    // calculate security mode.
    TPESimState iEnginePreviousSimState;
            
    // Phone device lock state. Used to define dialer security mode and
    // usability restrictions dynamically.
    TPSAutolockStatus iDeviceLockState;
    
    // Security mode status flag. This is calculated from iSimState and
    // iDeviceLockState
    TBool iSecurityMode;
    
    /**
     * Pointer array for observers. This dynamic security mode state object
     * doesn't own the observer instances. 
     */
    RPointerArray< MPhoneSecurityModeChangeObserver > iObserverArray;
    
    /**
     * Pointer to Phone Engine info instance. This object doens't own the
     * engine info instance.
     */
    MPEEngineInfo* iEngineInfo;
    
    /**
     * Flag to determinate if this security mode state object has been fully
     * initialized.
     */
    TBool iInitialized;
	
	};

#endif
