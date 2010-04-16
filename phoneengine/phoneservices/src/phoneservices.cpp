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
#include "qtphonelog.h"

PhoneServices::PhoneServices(MPECallControlIF &call, MPECallSettersIF &parameters, QObject* parent) : 
    QObject(parent)
{
    PHONE_DEBUG("PhoneServices::PhoneServices");
    dialService = new DialService(call, parameters, this);
    dtmfService = new DTMFService(call, parameters, this);
}

PhoneServices::~PhoneServices()
{
}
