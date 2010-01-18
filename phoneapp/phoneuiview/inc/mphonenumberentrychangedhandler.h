/*
* Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Number entry state changed handler interface.
*
*/

#ifndef MPHONENUMBERENTRYCHANGEDHANDLER_H
#define MPHONENUMBERENTRYCHANGEDHANDLER_H

#include "tphonecommandparam.h"

/**
* Defines Number Entry Changed Handler 
*  
* The implemenation of this interface defines the functionality that needs
* to be done after Phone Number Entry has notified that the entry state  
* has changed.
* 
*  @lib phoneuiview.lib
*  @since S60 5.0 
*/
class MPhoneNumberEntryChangedHandler  
    {
    public:
       
        /**
        * The implementing class contains the functionality about what is done
        * when number entry state has changed
        */
        virtual void HandleNumberEntryChanged() = 0;
        
        /**
         * Sets call back to an observer that wants to be notified about 
         * the change in the content of the Phone Number Entry. 
         */
        virtual void SetNumberEntryChangedCallBack( 
                            TPhoneCommandParam* aCommandParam ) = 0;  
    };

#endif /*MPHONENUMBERENTRYCHANGEDHANDLER_H*/

