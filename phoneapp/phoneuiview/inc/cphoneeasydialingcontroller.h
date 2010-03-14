/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Class handles Easy dialing specific commands
*
*/

#ifndef CPHONEEASYDIALINGCONTROLLER_H
#define CPHONEEASYDIALINGCONTROLLER_H

//INCLUDES
#include <e32base.h>
#include "phoneviewcommanddefinitions.h"

//FORWARD
class CDialer;
class TPhoneCommandParam;
class CDialingExtensionInterface;

/**
 * Class takes care of Easy dialing
 *
 * @lib phoneuicontrol
 * @since S60 v9.2
 */
NONSHARABLE_CLASS( CPhoneEasyDialingController ) : public CBase
    {
    public:
    
        /**
        * Creates new instance of CPhoneEasyDialingController
        * @param aDialer: reference to dialer
        * @return CPhoneEasyDialingController* object  
        */
        static CPhoneEasyDialingController* NewL(
                CDialer& aDialer );

        /**
        * Destructor.
        */
        virtual ~CPhoneEasyDialingController();
        
        void ExecuteCommandL(
            TPhoneViewCommandId aCmdId,
            TPhoneCommandParam* aCommandParam );

        TPhoneViewResponseId HandleCommandL( TPhoneViewCommandId aCmdId );
       
        TBool InitializeEasyDialingMenuL( TPhoneCommandParam* aCommandParam );

    private:
    
        /**
        * C++ default constructor.
        */
        CPhoneEasyDialingController(CDialer& aDialer);
        
    private:
        // Dialer. Not owned
        CDialer& iDialer;
        
        // Easydialing in interface. Not owned.
        CDialingExtensionInterface* iEasyDialing;
    };

#endif //CPHONEEASYDIALINGCONTROLLER_H
