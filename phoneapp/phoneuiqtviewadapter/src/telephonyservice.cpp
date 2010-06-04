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
* Description: telephony service
*
*/

#include <xqserviceutil.h>
#include "telephonyservice.h"
#include "phoneuiqtviewadapter.h"
#include "tphonecmdparaminteger.h"
#include "qtphonelog.h"

TelephonyService::TelephonyService( PhoneUIQtViewAdapter *viewAdapter, QObject* parent) :
    XQServiceProvider(QLatin1String("com.nokia.services.telephony.uistarter"), parent),
    m_viewAdapter (viewAdapter)
{
    PHONE_DEBUG("TelephonyService::TelephonyService");
    publishAll();
}

TelephonyService::~TelephonyService()
{
    PHONE_DEBUG("TelephonyService::~TelephonyService");
}

void TelephonyService::start(const int serviceId)
{
    PHONE_DEBUG2("TelephonyService::start:", serviceId);
    switch (serviceId) {
        case 0: {
            TPhoneCmdParamInteger uidParam;
            uidParam.SetInteger( KUidPhoneApplication.iUid );
            m_viewAdapter->ExecuteCommandL(EPhoneViewBringAppToForeground, &uidParam);
            break;
        }
        case 1: {
            m_viewAdapter->ExecuteCommandL( EPhoneViewOpenDialer );

            /*TODO: remove this as soon as QT Highway brings in call dialer to foreground*/
            TPhoneCmdParamInteger uidParam;
            uidParam.SetInteger( KUidPhoneApplication.iUid );
            m_viewAdapter->ExecuteCommandL(EPhoneViewBringAppToForeground, &uidParam);
            break;
        }
        default: {
            // do nothing
            break;
        }
    }
}
