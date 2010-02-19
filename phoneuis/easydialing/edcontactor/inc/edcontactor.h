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
* Description:  Class for launching the different communication methods
*
*/


#ifndef EDCONTACTORSERVICE_H
#define EDCONTACTORSERVICE_H

// INCLUDES
#include <e32std.h>
#include <VPbkFieldTypeSelectorFactory.h>
// CLASS DECLARATION

/**
 * Class for launching communication methods
 *
 *  @code
 *   ?good_class_usage_example(s)
 *  @endcode
 *
 *  @lib
 *  @since S60 v5.0
 */
NONSHARABLE_CLASS(CEDContactor) : CBase
    {
public:
    /**
     * Starts a communication method.
     * 
     * @param aCommMethod The communication method.
     * @param aParam Parameter for the communication, for example phone number or e-mail address.
     * @param aName Name of the contact which the parameter is from, for example for the sms/mms editor's to-field (optional)
     * @param aForcedService Should the service be forced or not, relevant in voice call set-up.
     * @param aServiceID The service ID for the VoIP call comm method, has no effect and can be left out for other comm methods
     */
    static void ExecuteServiceL(VPbkFieldTypeSelectorFactory::TVPbkContactActionTypeSelector aCommMethod, const TDesC& aParam, 
            const TDesC& aName = KNullDesC, TBool aForcedService = EFalse, TUint32 aServiceId = 0);

    };

#endif // EDCONTACTORSERVICE_H
// End of File
