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
* Description:  Homescreen widget plugin
*
*/

#include <qserviceinterfacedescriptor.h>
#include <qabstractsecuritysession.h>
#include <qservicecontext.h>

#include "hsdialerwidgetplugin.h"
#include "hsdialerwidget.h"
#include "qtphonelog.h"

#ifdef COVERAGE_MEASUREMENT
#pragma CTC SKIP
#endif //COVERAGE_MEASUREMENT

QObject *HsDialerWidgetPlugin::createInstance(const QServiceInterfaceDescriptor &descriptor,
                                             QServiceContext *context,
                                             QAbstractSecuritySession *session)
{
    PHONE_DEBUG("HsDialerWidgetPlugin::createInstance");
    Q_UNUSED(context);
    Q_UNUSED(session);

    if (descriptor.interfaceName() == QLatin1String("com.nokia.symbian.IHomeScreenWidget")) {
        PHONE_DEBUG("creating HsDialerWidget instance");
        return new HsDialerWidget();
    } else {
        return 0;
    }
}

Q_EXPORT_PLUGIN2(hsdialerwidgetplugin, HsDialerWidgetPlugin)

#ifdef COVERAGE_MEASUREMENT
#pragma CTC ENDSKIP
#endif //COVERAGE_MEASUREMENT
