/*
* Copyright (c) 2010-2010 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef MPHONESECURITYMESSAGEHANDLER_H
#define MPHONESECURITYMESSAGEHANDLER_H

class TUid;

class MPhoneSecurityMessageHandler
	{
	public:
	/**
	 * Handles phone engine message. Dynamic state object listens phone engine
	 * message to determinate sim states.
	 * 
	 * @param aMessage Message enumeration.
	 * @param aCallId Call identifier.
	 */
	virtual void HandlePhoneEngineMessageL( const TInt aMessage, TInt aCallId ) = 0;
	
	/**
	 * Handle property change. Dynamic state object listens P&S key changes
	 * to determinate autolock and sim security states.
	 * 
	 * @param aCategory Unique identifer category reference.
	 * @param aKey Key as unsigned integer.
	 * @param aValue Key value as integer.
	 */
	virtual void HandlePropertyChangedL(const TUid& aCategory,
	     const TUint aKey,
	     const TInt aValue ) = 0;
	};

#endif
