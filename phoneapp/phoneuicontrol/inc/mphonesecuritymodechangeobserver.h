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
*     A dynamic security mode state class observer interface.
*
*/

#ifndef MPHONESECURITYMODECHANGEOBSERVER_H
#define MPHONESECURITYMODECHANGEOBSERVER_H

#include <e32def.h>

/**
 * Observer interface to observe dynamic security mode state changes.
 */
class MPhoneSecurityModeChangeObserver
	{
public:
	/**
	 * Handles security mode state change. This is called only when state
	 * actually changes.
	 * 
	 * @param aIsEnabled Defines if security mode is enabled.
	 */
	virtual void HandleSecurityModeChanged( TBool aIsEnabled ) = 0;
	};

#endif

// End of file
