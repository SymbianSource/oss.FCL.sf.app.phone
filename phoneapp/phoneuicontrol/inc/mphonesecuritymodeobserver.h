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
*     An interface for dynamic security mode state class.
*
*/

#ifndef MPHONESECURITYMODEOBSERVER_H
#define MPHONESECURITYMODEOBSERVER_H

#include <e32def.h>

// Forward declarations
class MPhoneSecurityModeChangeObserver;

/**
 * Interface for dynamic security mode state class.
 */
class MPhoneSecurityModeObserver
	{
public :
	
	/**
	 * Checks if autolock is enabled.
	 * 
	 * @return ETrue if autolock is enabled. EFalse otherwise.
	 */
	virtual TBool IsAutolockEnabled() = 0;
	
	/**
	 * Checks if security mode is enabled.
	 * 
	 * @return ETrue if enabled, otherwise EFalse.
	 */
	virtual TBool IsSecurityMode() = 0;
	
	/**
	 * Registers security mode state observer to observer state changes.
	 *  
	 * @param aObserver Reference to state observer instance..
	 */
	virtual void RegisterStateObserver( MPhoneSecurityModeChangeObserver& aObserver ) = 0;
	
	/** 
	 * Removes registered state observer.
	 * 
	 * @param aObserver Reference to observer instance.
	 */
	virtual void RemoveStateObserver( MPhoneSecurityModeChangeObserver& aObserver ) = 0;
	
	/**
	 * Initializes dynamic security mode state observer.
	 */
	virtual void Initialize() = 0;
	};

#endif

// End of file
