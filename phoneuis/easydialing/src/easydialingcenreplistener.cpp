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
* Description:  Easy dialing cenrep listener.
*
*/

// INCLUDE FILES
#include <centralrepository.h>
#include <cenrepnotifyhandler.h>
#include "easydialingcenreplistener.h"
#include "measydialingcenreplistenerobserver.h"
#include "easydialingcrkeys.h"

// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CEasyDialingCenrepListener::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CEasyDialingCenrepListener* CEasyDialingCenrepListener::NewL(MEasyDialingCenrepListenerObserver* aObserver)
    {   
    CEasyDialingCenrepListener* self = new (ELeave) CEasyDialingCenrepListener(aObserver);
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }
    
// -----------------------------------------------------------------------------
// CEasyDialingCenrepListener::~CEasyDialingCenrepListener
// Destructor
// -----------------------------------------------------------------------------
//
CEasyDialingCenrepListener::~CEasyDialingCenrepListener()
    {
    if (iNotifyHandler)
        {
        iNotifyHandler->StopListening();
        delete iNotifyHandler;
        iNotifyHandler = NULL;
        }

    delete iClient;
    }

// -----------------------------------------------------------------------------
// CEasyDialingCenrepListener::CEasyDialingCenrepListener
// C++ default constructor.
// -----------------------------------------------------------------------------
//
CEasyDialingCenrepListener::CEasyDialingCenrepListener(MEasyDialingCenrepListenerObserver* aObserver)
        : iObserver(aObserver),
        iEasyDialingSettingValue(0),
        iContactThumbnailsSettingsValue(1)
        
    {
    }

// -----------------------------------------------------------------------------
// CEasyDialingCenrepListener::ConstructL
// Symbian 2nd phase constructor.
// -----------------------------------------------------------------------------
//
void CEasyDialingCenrepListener::ConstructL()
    {
    iClient = CRepository::NewL(KCRUidEasyDialSettings);
    
    User::LeaveIfError(iClient->Get( KEasyDialing, iEasyDialingSettingValue ));
    User::LeaveIfError(iClient->Get( KEasyDialingContactThumbnails, iContactThumbnailsSettingsValue ));
    
    iNotifyHandler = CCenRepNotifyHandler::NewL(*this, *iClient );
    iNotifyHandler->StartListeningL();
    }

// -----------------------------------------------------------------------------
// CEasyDialingCenrepListener::KeyValueL
// Returns key value
// -----------------------------------------------------------------------------
//
TInt CEasyDialingCenrepListener::Value() const
    {
    return iEasyDialingSettingValue;
    }

// -----------------------------------------------------------------------------
// CEasyDialingCenrepListener::SetEasyDialingSettingsValue
// Sets key value.
// -----------------------------------------------------------------------------
//
void CEasyDialingCenrepListener::SetEasyDialingSettingsValue( const TInt aValue )
    {
    iClient->Set( KEasyDialing, aValue );
    }

// -----------------------------------------------------------------------------
// CEasyDialingCenrepListener::HandleNotifyInt
// -----------------------------------------------------------------------------
//
void CEasyDialingCenrepListener::HandleNotifyGeneric( TUint32 aId )
    {
    TInt value(0);
    
    iClient->Get( aId, value );
    
    if( aId == KEasyDialingContactThumbnails)
        {
        if( value!= iContactThumbnailsSettingsValue )
            {
            iContactThumbnailsSettingsValue = value;
            iObserver->EasyDialingContactThumbnailsSettingsChanged( iContactThumbnailsSettingsValue );
            }
        }
    
    else if ( aId == KEasyDialing )
        {
        if ( value != iEasyDialingSettingValue )
            {
            iEasyDialingSettingValue = value;
            iObserver->EasyDialingSettingsChanged( iEasyDialingSettingValue );
            }        
        }
    }
//  End of File  
