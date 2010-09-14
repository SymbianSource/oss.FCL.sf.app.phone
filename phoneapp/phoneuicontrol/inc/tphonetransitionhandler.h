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
* Description: Class handles phoneapp internal/external transitions.
*
*/


#ifndef TPHONETRANSITIONHANDLER_H
#define TPHONETRANSITIONHANDLER_H

//INCLUDES
#include <e32base.h>
#include "mphoneviewcommandhandle.h"
#include "tphonecmdparamtranseffect.h"

class CPhoneState;

/**
 * Class handles phoneapp internal/external transitions.
 *
 * @lib phoneuicontrol
 * @since S60 v9.2
 */
class TPhoneTransitionHandler
    {
    public:
        /**
        * default constructor.
        */
        TPhoneTransitionHandler(
                CPhoneState& aState,
                MPhoneViewCommandHandle& aViewCommandHandle );
        
        /**
        * Informs view to start Transition effect if effect
        * type is feasible for current state.
        * @param aType a transition effect, default none
        * EndTransEffect() must be called when update is done.
        */
        IMPORT_C void BeginTransEffectLC( TPhoneTransEffectType aType = EPhoneTransEffectNone );

        /**
        * Informs view to complete Transition effect
        */
        IMPORT_C void EndTransEffect();
        
        /**
        * Informs view that UI is being updated (call bubble or number editor).
        * EndUiUpdate() must be called when update is done.
        */
        IMPORT_C void BeginUiUpdateLC();
       
        /**
        * Informs view that UI update is completed.
        */
        IMPORT_C void EndUiUpdate();
        
        /**
        * Informs view that UI is being updated (call bubble or number editor).
        * EndUiUpdate() must be called when update is done.
        */
        IMPORT_C void IncomingCallUiUpdateLC();
      
        /**
        * Informs view that UI is being updated (call bubble or number editor).
        * EndUiUpdate() must be called when update is done.
        */
        IMPORT_C void IdleCallUiUpdateLC();
       
        /**
        * Informs view that UI update is completed.
        */
        IMPORT_C void EndUiUpdateAndEffect();
        
    private:
        
        /**
        * TCleanupOperation to call EndUiUpdate(), if leave occurs
        * after BeginUiUpdate().
        */
        static void UiUpdateCleanup(TAny* aThis );

        /**
        * TCleanupOperation to call EndTransEffect, if leave occurs
        * after BeginTransEffect().
        */
        static void EffectCleanup(TAny* aThis );
        
    private:
            
        // Not owned:
        CPhoneState& iState;
        MPhoneViewCommandHandle& iViewCommandHandle;
    };


#endif // TPHONETRANSITIONHANDLER_H
