/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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


#ifndef __TPHONECMDPARAMTRANSEFFECT_H
#define __TPHONECMDPARAMTRANSEFFECT_H

//  INCLUDES

#include <w32std.h>
#include <aknnotedialog.h> 
#include "tphonecommandparam.h"

// DATA TYPES

/**
* Enumerates effect types
*
* EPhoneTransitionEffectStart - TBD
* EPhoneTransitionEffectStop - TBD 
*/
enum TPhoneTransEffectType
    {
    EPhoneTransEffectNone,
    // These 3 effect types can be used only when dialer is
    // opened/closed when some other app than phone is visible 
    // on the foreground/in the background.
    // These cannot be used for internal transitions (=call ui<->dialer).
    EPhoneTransEffectDialerOpen,
    EPhoneTransEffectDialerClose,
    EPhoneTransEffectDialerCreate,
    // For applying effect to external app launch done from dialer.
    // Effects triggered by App FW don't work properly.
    EPhoneTransEffectAppStartFromDialer,
    EPhoneTransEffectStop
    };

// CLASS DECLARATION

/**
*  A parameter class for transition effect information.
*/
class TPhoneCmdParamTransEffect : public TPhoneUICommandParam
    {   

    public: 
           
        /**
        * C++ default constructor.
        */
        IMPORT_C TPhoneCmdParamTransEffect();

    public: 
        
        /**
        * Sets the effect type
        * @param aType is the effect type
        */
        IMPORT_C void SetType( TPhoneTransEffectType aType );

        /**
        * Sets application uid associated with the effect.
        * @param    aAppUid, Id of app.
        * @return   None.
        */
        IMPORT_C void SetAppUid( const TUid& aAppUid );

        /**
        * Returns the effect type
        * @return Returns the type
        */
        IMPORT_C TPhoneTransEffectType Type() const;

        /**
        * Returns App uid associated with the effect.
        * @param    None.
        * @return   iAppUid.
        */
        IMPORT_C TUid AppUid() const;

    private:    
        
        /**
        * Effect type
        */
        TPhoneTransEffectType iType;
        
        /**
        * App UID
        */
        TUid iAppUid;

    };

#endif // __TPHONECMDPARAMTRANSEFFECT_H   
            
// End of File
