/*!
* Copyright (c) 2009-2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  DTMF API to be used through Qt Highway.
*
*/

#include <xqserviceutil.h>
#include <e32base.h>
#include "dtmfservice.h"
#include "qtphonelog.h"

DTMFService::DTMFService(
    MPECallControlIF &call, 
    MPECallSettersIF &parameters, 
    QObject* parent) 
    : 
    XQServiceProvider(
        QLatin1String("com.nokia.symbian.IDtmfPlay"), parent), 
    m_call(call), 
    m_parameters(parameters)
{
    publishAll();
}

DTMFService::~DTMFService()
{
}

void DTMFService::playDTMFTone(const QChar& keyToPlay)
{
    PHONE_DEBUG2("DTMFService::playDTMFTone keyToPlay:", keyToPlay);
    m_parameters.SetKeyCode(keyToPlay.unicode());
    TRAP_IGNORE( m_call.HandlePlayDTMFL() );
}

void DTMFService::stopDTMFPlay()
{
    PHONE_DEBUG("DTMFService::stopDTMFPlay");
    m_call.HandleEndDTMF(); 
}
