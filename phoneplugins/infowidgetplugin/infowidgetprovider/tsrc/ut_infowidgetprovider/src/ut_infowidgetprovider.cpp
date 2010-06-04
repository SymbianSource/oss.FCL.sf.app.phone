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
#include "ut_infowidgetprovider.h"
#include "infowidgetprovider.h"
#include "qtestmains60.h"

#include <qserviceinterfacedescriptor.h>
#include <qabstractsecuritysession.h>
#include <qservicecontext.h>
#include <QString>

Q_EXTERN_C const char * qt_plugin_query_verification_data();
Q_EXTERN_C ::QObject *  qt_plugin_instance();

class MyQServiceInterfaceDescriptor : public QServiceInterfaceDescriptor
{}; 

class MyQServiceContext : public QServiceContext
{
public: 
    virtual void notify( ContextType type, const QVariant& variant){
    Q_UNUSED(type)
    Q_UNUSED(variant)
    };
}; 

class MyQAbstractSecuritySession : public QAbstractSecuritySession
{
public: 
    virtual bool isAllowed(const QStringList& capabilityList){
    Q_UNUSED(capabilityList)
    return true;
    };
}; 

QString KValidInfoWidgetInterfaceName("com.nokia.IHomeScreenWidget"); 
QString KInvalidInfoWidgetInterfaceName("com");


/*!
  UT_InfoWidgetProvider::UT_InfoWidgetProvider
 */
UT_InfoWidgetProvider::UT_InfoWidgetProvider() 
{
    const char * verificationData = qt_plugin_query_verification_data();
}


/*!
  UT_InfoWidgetProvider::~UT_InfoWidgetProvider
 */
UT_InfoWidgetProvider::~UT_InfoWidgetProvider()
{

}


/*!
  UT_InfoWidgetProvider::init
 */
void UT_InfoWidgetProvider::init()
{
    initialize();

    QVERIFY(verify());
}


/*!
  UT_InfoWidgetProvider::cleanup
 */
void UT_InfoWidgetProvider::cleanup()
{
    reset();
    
    delete (InfoWidgetProvider*)qt_plugin_instance();
}


void UT_InfoWidgetProvider::t_createInstance()
    {
    MyQServiceInterfaceDescriptor myServiceInterfaceDescriptor; 
    MyQServiceContext myQServiceContext; 
    MyQAbstractSecuritySession myAbstractSecuritySession; 
    
    QObject *instanceValue = NULL;
    InfoWidgetProvider* p = (InfoWidgetProvider*)qt_plugin_instance();
    // Test: instance creation with valid interface name
    expect("QtMobility::QServiceInterfaceDescriptor::interfaceName").returns(KValidInfoWidgetInterfaceName);
    expect("InfoWidget::InfoWidget"); 
    instanceValue = p->createInstance(myServiceInterfaceDescriptor,
                                             &myQServiceContext,
                                             &myAbstractSecuritySession);
    Q_ASSERT(instanceValue); 
    
    // Test: instance creation with invalid interface name
    expect("QtMobility::QServiceInterfaceDescriptor::interfaceName").returns(KInvalidInfoWidgetInterfaceName);
    instanceValue = p->createInstance(myServiceInterfaceDescriptor,
                                             &myQServiceContext,
                                             &myAbstractSecuritySession);
    Q_ASSERT(instanceValue == NULL); 

}

QTEST_MAIN_S60(UT_InfoWidgetProvider)
