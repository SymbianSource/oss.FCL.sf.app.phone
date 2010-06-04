/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
*   See class description.
*
*/


#ifndef TPHONECMDPARAMRECONNECTQUERY_H
#define TPHONECMDPARAMRECONNECTQUERY_H

//  INCLUDES

#include <w32std.h>
#include "tphonecommandparam.h"

// DATA TYPES

// FORWARD DECLARATION
class MCaUiReconnectQueryObserver;
class CPhCntMatcher;

// CLASS DECLARATION

/**
*  A parameter class for displaying reconnect query.
*/
class TPhoneCmdParamReconnectQuery : public TPhoneUICommandParam
    {   
    public:  
        
        /**
        * C++ default constructor.
        */
        IMPORT_C TPhoneCmdParamReconnectQuery( MCaUiReconnectQueryObserver& aObserver );

    public: 
        
        /**
        * Sets the contact matcher.
        * @param aPhCntMatcher pointer to contact matcher. NULL value allowed.
        */
        IMPORT_C void SetContactMatcher( CPhCntMatcher* aPhCntMatcher );

        /**
        * Sets the phone number.
        * @param aPhoneNumber phone number
        */
        IMPORT_C void SetPhoneNumber( const TPtrC aPhoneNumber );

        /**
        * Sets the video call option to visible or hidden in reconnect query.
        * @param aIncludeVideoCallOption EFalse to hide video call option, ETrue to show it.
        */
        IMPORT_C void SetIncludeVideoOption( const TBool aIncludeVideoCallOption );
       
        /**
        * Returns the reference to reconnect query observer.
        * @return Returns the reference to reconnect query observer.
        */
        IMPORT_C MCaUiReconnectQueryObserver& ReconnectQueryObserver() const;

        /**
        * Returns the contact matcher pointer.
        * @return Returns the contact matcher pointer.
        */
        IMPORT_C CPhCntMatcher* ContactMatcher() const;

        /**
        * Returns the phone number.
        * @return Returns the phone number.
        */
        IMPORT_C const TPtrC PhoneNumber() const;

        /**
        * Returns the video call option value.
        * @return Returns the video call option value.
        */
        IMPORT_C TBool IncludeVideoCallOption() const;

    private:    
        
        /**
        * Reconnect query observer
        */
        MCaUiReconnectQueryObserver& iObserver;

        /**
        * Contact matcher
        */
        CPhCntMatcher* iContactMatcher;

        /**
        * Phone number
        */
        TPtrC iPhoneNumber;

        /**
        * Include video call option
        */
        TBool iIncludeVideoCallOption;
    };

#endif // TPHONECMDPARAMRECONNECTQUERY_H
            
// End of File
