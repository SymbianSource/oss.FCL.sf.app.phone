/*!
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
* Description:  Phone Services manages all Phone Engine's Qt Highway services.
*
*/

#include "phoneservices.h"
#include "dialservice.h"
#include "dtmfservice.h"
#include "mpekeysequencerecognitionif.h"
#include "keysequencerecognitionservice.h"
#include "qtphonelog.h"

// Depricated APIs
#include "dialservicedepricated.h"
#include "dtmfservicedepricated.h"
#include "keysequencerecognitionservicedepricated.h"

PhoneServices::PhoneServices(
    MPECallControlIF &call, 
    MPECallSettersIF &parameters, 
    MPEKeySequenceRecognitionIF &keySequenceRecognition,
    QObject* parent) 
    : 
    QObject(parent),
    dialService(0),
    dtmfService(0),
    m_keySequenceRecognitionService(0)
    // Depricated
    , dialServiceDepricated(0)
    , dtmfServiceDepricated(0)
    , m_keySequenceRecognitionServiceDepricated(0)
{
    PHONE_DEBUG("PhoneServices::PhoneServices");
    dialService.reset(new DialService(call, parameters));
    dtmfService.reset(new DTMFService(call, parameters));
    m_keySequenceRecognitionService.reset( 
        new KeySequenceRecognitionService(keySequenceRecognition));

    // Depricated
    dialServiceDepricated.reset(new DialServiceDepricated(call, parameters));
    dtmfServiceDepricated.reset(new DTMFServiceDepricated(call, parameters));
    m_keySequenceRecognitionServiceDepricated.reset( 
        new KeySequenceRecognitionServiceDepricated(keySequenceRecognition));
}

PhoneServices::~PhoneServices()
{
}
