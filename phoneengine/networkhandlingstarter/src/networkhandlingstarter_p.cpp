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

#include <hbdevicemessagebox.h>
#include <HbAction.h>
#include <CpPluginLauncher.h>
#include <hbinstance.h>
#include <HbView.h>
#include <xqserviceutil.h>

#include "networkhandlingstarter_p.h"
#include "networkhandlingstarterlogging.h"
#include "cnetworklistener.h"

/*!
    Constructor of NetworkHandlingStarterPrivate.
 */
NetworkHandlingStarterPrivate::NetworkHandlingStarterPrivate(QObject *parent) : 
    QObject(parent), m_note(NULL)
{
    DPRINT << ": IN";
    
    QT_TRAP_THROWING(m_networkListener = CNetworkListener::NewL(*this));
    
    DPRINT << ": OUT";
}

/*!
    Destructor of NetworkHandlingStarterPrivate.
 */
NetworkHandlingStarterPrivate::~NetworkHandlingStarterPrivate()
{
    DPRINT << ": IN";
    
    delete m_networkListener;
    if (m_note) {
        delete m_note;
    }
    
    DPRINT << ": OUT";
}
    
/*!
    NetworkHandlingStarterPrivate::ShowNote()
 */
void NetworkHandlingStarterPrivate::ShowNote()
{
    DPRINT << ": IN";
    
    if (m_note) {
        m_note->close();
        delete m_note;
        m_note = NULL;
    }
    m_note = new HbDeviceMessageBox(
        hbTrId("txt_phone_info_network_lost_select_network"), 
        HbMessageBox::MessageTypeQuestion);
    HbAction *primaryAction = new HbAction(hbTrId("txt_common_button_yes"), m_note);
    m_note->setAction(primaryAction, HbDeviceMessageBox::AcceptButtonRole); 
    HbAction *secondaryAction = new HbAction(hbTrId("txt_common_button_no"), m_note);
    m_note->setAction(secondaryAction,HbDeviceMessageBox::RejectButtonRole); 
    QObject::connect(
        primaryAction, SIGNAL(triggered()),
        this, SLOT(LaunchCpNetworkPluginView()));
    m_note->setTimeout(0);
    m_note->show();
    
    DPRINT << ": OUT";
}

/*!
    NetworkHandlingStarterPrivate::RemoveNote()
 */
void NetworkHandlingStarterPrivate::RemoveNote()
{
    DPRINT << ": IN";
    
    if (m_note) {
        m_note->close();
        delete m_note;
        m_note = NULL;
    }
    
    DPRINT << ": OUT";
}

/*!
    NetworkHandlingStarterPrivate::InitaliseCpNetworkPluginView()
 */
void NetworkHandlingStarterPrivate::InitaliseCpNetworkPluginView()
{
    DPRINT << ": IN";
    
    HbMainWindow *mainWnd = MainWindow();
    if (mainWnd) {
        if (CpPluginLauncher::launchCpPluginView(
                "resource\\qt\\plugins\\controlpanel\\cpnetworkplugin.qtplugin")) {
            foreach (HbView *view, mainWnd->views()) {
                if (QString(view->metaObject()->className()) == 
                    QString("CpNetworkPluginView")) {
                    QObject::connect(
                        view, SIGNAL(aboutToClose()), 
                        this, SLOT(ViewDone()));
                    QObject::connect(
                        this, SIGNAL(SearchAvailableNetworks()), 
                        view, SLOT(SearchAvailableNetworks()));
                }
            }
        }
    }
    
    DPRINT << ": OUT";
}

/*!
    NetworkHandlingStarterPrivate::LaunchCpNetworkPluginView()
 */
void NetworkHandlingStarterPrivate::LaunchCpNetworkPluginView()
{
    DPRINT << ": IN";
    
    InitaliseCpNetworkPluginView();
    HbMainWindow *mainWnd = MainWindow();
    if (mainWnd) {
        mainWnd->show();
        XQServiceUtil::toBackground(false);
    }
    emit SearchAvailableNetworks();
    
    DPRINT << ": OUT";
}

/*!
    NetworkHandlingStarterPrivate::ViewDone()
 */
void NetworkHandlingStarterPrivate::ViewDone()
{
    HbMainWindow *mainWnd = MainWindow();
    if (mainWnd) {
        mainWnd->hide();
    }
    XQServiceUtil::toBackground(true);
}

/*!
    NetworkHandlingStarterPrivate::MainWindow()
 */

HbMainWindow * NetworkHandlingStarterPrivate::MainWindow() 
{
    HbMainWindow *window(NULL);
    QList<HbMainWindow*> mainWindows = hbInstance->allMainWindows();
    if (!mainWindows.isEmpty()) {
        window = mainWindows.front();
    }
    return window;
}

// End of File.
