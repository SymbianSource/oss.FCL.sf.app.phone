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
* Description:  Infowidgetplugin service manager 
* registration handler
*
*/

#include <qservicemanager.h>
#include <QCoreApplication>
#include <QFile>

QTM_USE_NAMESPACE

const QString xml = "z:/private/20022F35/import/widgetregistry/20029F28/infowidgetprovider.xml";

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QServiceManager s;

	if (QFile::exists(xml)) {
        s.addService(xml);
	} else {
		s.removeService("infowidgetprovider");
	}
}
