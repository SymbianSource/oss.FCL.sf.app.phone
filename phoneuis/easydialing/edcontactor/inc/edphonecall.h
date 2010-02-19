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
* Description:  Launching Calls. 
*
*/


#ifndef __EDPHONECALL_H__
#define __EDPHONECALL_H__

#include <e32base.h>                             

// CLASS DECLARATION
class CAiwDialDataExt;


/**
 * Helper class for launching calls (voice, voip & video)
 *
 *  @code
 *   ?good_class_usage_example(s)
 *  @endcode
 *
 *  @lib
 *  @since S60 v5.0
 */
class CEDPhoneCall : public CBase
	{
public:
    
    /**
     * Type of call
     */
    enum TEDPhoneCallType { ECCACallTypeVoice, ECCACallTypeVoIP, ECCACallTypeVideo };

	/**
	 * Establish a call by using a phone number (or voip address)
	 *
	 * @param TDesC8& Phone number
	 * @param aCallType Is this a cellular Voice, VoIP, or Video call
	 * @param aForceCallType Whether to execute call type as forced one. 
     *                       Currently only voice call type can be variated with this.
	 *
	 * @return void 
	 */
	static void ExecuteL( const TDesC& aPhoneNumber, TEDPhoneCallType aCallType, 
	        TBool aForcedCallType, 
	        TUint32 aServiceId = 0);

private:

	/**
	 * Establish a call by using a phone number
	 *
	 * @param TDesC8& Phone number
	 * @param TBool Is this a VoIP call
	 *
	 * @return void 
	 */
	static void DoAIWCallL( CAiwDialDataExt& aDialData);
	};

#endif // __EDPHONECALL_H__
// End of File
