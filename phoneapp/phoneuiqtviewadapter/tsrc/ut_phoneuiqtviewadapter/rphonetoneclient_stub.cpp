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
* Description:
*
*/

// INCLUDE FILES

bool m_EPhoneViewMuteRingToneOnAnswer_called;
bool m_EPhoneViewStopRingTone_called;
bool m_EPhoneViewMuteRingTone_called;
bool m_EPhoneViewPlayRingTone_called;


// INCLUDE FILES
#include <e32base.h>
#include <e32std.h>
#include <e32svr.h>

#include "rphonetoneclient.h"
#include "tphonecmdparamringtone.h"



// -----------------------------------------------------------------------------
// RPhoneToneClient::RPhoneToneClient
// -----------------------------------------------------------------------------
//
EXPORT_C RPhoneToneClient::RPhoneToneClient() 
    {
    }

// -----------------------------------------------------------------------------
// RPhoneToneClient::~RPhoneToneClient
// -----------------------------------------------------------------------------
//
EXPORT_C RPhoneToneClient::~RPhoneToneClient()
    {
    m_EPhoneViewMuteRingToneOnAnswer_called = false;
    m_EPhoneViewStopRingTone_called = false;
    m_EPhoneViewMuteRingTone_called = false;
    m_EPhoneViewPlayRingTone_called = false;

    }

// -----------------------------------------------------------------------------
// RPhoneToneClient::Connect
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RPhoneToneClient::Connect()
    {
		return KErrNone;
    }


// -----------------------------------------------------------------------------
// RPhoneToneClient::PlayRingingToneL
// -----------------------------------------------------------------------------
//
EXPORT_C void RPhoneToneClient::PlayRingingToneL( TPhoneCommandParam *aCommandParam )
    {
    if ( aCommandParam->ParamId() == TPhoneCommandParam::EPhoneParamIdRingTone )
        {
        m_EPhoneViewPlayRingTone_called = true;
        } 
    }

// -----------------------------------------------------------------------------
// RPhoneToneClient::StopPlaying
// -----------------------------------------------------------------------------
//
EXPORT_C void RPhoneToneClient::StopPlayingL()
    {
     m_EPhoneViewStopRingTone_called = true;
    }

// End of file
