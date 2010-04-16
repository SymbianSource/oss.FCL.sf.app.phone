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


class MyQServiceInterfaceDescriptor : public QServiceInterfaceDescriptor
{}; 

class MyQServiceContext : public QServiceContext
{
public: 
    virtual void notify( ContextType type, const QVariant& variant){};
}; 

class MyQAbstractSecuritySession : public QAbstractSecuritySession
{
public: 
    virtual bool isAllowed(const QStringList& capabilityList){return true;};
}; 

QString KValidInfoWidgetInterfaceName("com.nokia.homescreen.widget"); 
QString KInvalidInfoWidgetInterfaceName("com");


/*!
  UT_InfoWidgetProvider::UT_InfoWidgetProvider
 */
UT_InfoWidgetProvider::UT_InfoWidgetProvider() 
    : 
    m_infoWidgetProvider(0)
{
    
}


/*!
  UT_InfoWidgetProvider::~UT_InfoWidgetProvider
 */
UT_InfoWidgetProvider::~UT_InfoWidgetProvider()
{
    delete m_infoWidgetProvider;
}


/*!
  UT_InfoWidgetProvider::init
 */
void UT_InfoWidgetProvider::init()
{
    initialize();
    
    m_infoWidgetProvider = new InfoWidgetProvider();
    
    QVERIFY(verify());
}


/*!
  UT_InfoWidgetProvider::cleanup
 */
void UT_InfoWidgetProvider::cleanup()
{
    reset();
    
    delete m_infoWidgetProvider;
    m_infoWidgetProvider = 0;
}


void UT_InfoWidgetProvider::t_createInstance()
    {
    MyQServiceInterfaceDescriptor myServiceInterfaceDescriptor; 
    MyQServiceContext myQServiceContext; 
    MyQAbstractSecuritySession myAbstractSecuritySession; 
    
    QObject *instanceValue = NULL; 

    // Test: instance creation with valid interface name
    expect("QtMobility::QServiceInterfaceDescriptor::interfaceName").returns(KValidInfoWidgetInterfaceName);
    expect("InfoWidget::InfoWidget"); 
    instanceValue = m_infoWidgetProvider->createInstance(myServiceInterfaceDescriptor,
                                             &myQServiceContext,
                                             &myAbstractSecuritySession);
    Q_ASSERT(instanceValue); 
    
    // Test: instance creation with invalid interface name
    expect("QtMobility::QServiceInterfaceDescriptor::interfaceName").returns(KInvalidInfoWidgetInterfaceName);
    instanceValue = m_infoWidgetProvider->createInstance(myServiceInterfaceDescriptor,
                                             &myQServiceContext,
                                             &myAbstractSecuritySession);
    Q_ASSERT(instanceValue == NULL); 
    

}


QTEST_MAIN_S60(UT_InfoWidgetProvider)
