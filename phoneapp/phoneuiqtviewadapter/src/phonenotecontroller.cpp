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
* Description:  Handles phone notes.
*/

#include "phonenotecontroller.h"
#include "tphonecmdparamglobalnote.h"
#include "tphonecmdparamquery.h"
#include "phoneresourceadapter.h"
#include "qtphonelog.h"
#include <QSignalMapper>
#include <QTimer>
#include <hbdevicemessagebox.h>
#include <hbdeviceprogressdialog.h>
#include <hbmessagebox.h>
#include <hbprogressdialog.h>
#include <hbaction.h>
#include <phoneappcommands.hrh>
  
PhoneNoteController::PhoneNoteController(QObject *parent) : 
    QObject(parent), m_timer(0), m_progressDialog(0), m_dtmfNote(0), 
    m_queryNote(0), m_queryCanceledCommand(-1), m_timeoutCommand(-1)
{
    PHONE_DEBUG("PhoneNoteController::PhoneNoteController");
    m_signalMapper = new QSignalMapper(this);
    connect(m_signalMapper, SIGNAL(mapped(int)), this, SIGNAL(command (int)));
}

PhoneNoteController::~PhoneNoteController()
{
    PHONE_DEBUG("PhoneNoteController::~PhoneNoteController");
    if (m_timer) {
        delete m_timer;
    }
}

void PhoneNoteController::showGlobalNote(TPhoneCommandParam *commandParam)
{
    PHONE_DEBUG("PhoneNoteController::showGlobalNote");
    Q_ASSERT (commandParam->ParamId () == TPhoneCommandParam::EPhoneParamIdGlobalNote);

    TPhoneCmdParamGlobalNote* globalNoteParam = 
        static_cast<TPhoneCmdParamGlobalNote*>( commandParam );
    
        
    HbMessageBox::MessageBoxType type;
    
    switch( globalNoteParam->Type() ) {
    case EAknGlobalInformationNote:
        type = HbMessageBox::MessageTypeInformation;
        break;
    case EAknGlobalWarningNote:
    default:
        type = HbMessageBox::MessageTypeWarning;
        break;
    }
    
    QString noteString = globalNoteText(globalNoteParam);
    
    if (false == noteString.isNull()) {
        bool showNote(true);
        for (int i = 0; i < m_messageBoxList.count(); ++i) {
            // Do not show same note/text several times, e.g when user hits
            // the end button several times we should show only one "not allowed"
            // note.
            if (noteString == m_messageBoxList.at(i)->text()) {
                showNote = false;
                break;
            }
        }
        
        if (showNote) {
            QScopedPointer<HbDeviceMessageBox> messageBox( 
                new HbDeviceMessageBox(noteString, type));
            
            int timeout = globalNoteParam->Timeout();
            if (timeout == 0) {
                messageBox->setTimeout(HbDialog::StandardTimeout);
            } else {
                messageBox->setTimeout(timeout);
            }
            
            HbDeviceMessageBox *messageBoxPtr = messageBox.data();
            m_messageBoxList.append(messageBoxPtr);
            messageBox.take();
            
            if (1 == m_messageBoxList.size()) {
                QObject::connect(messageBoxPtr, SIGNAL(aboutToClose()), 
                                 this, SLOT(destroyDialog()));
                messageBoxPtr->show();
            }
        }
    }
}

void PhoneNoteController::showNote(TPhoneCommandParam *commandParam)
{
    PHONE_DEBUG("PhoneNoteController::showNote");

    TPhoneCmdParamNote* noteParam = static_cast<TPhoneCmdParamNote*>(
            commandParam );
    
    if ( noteParam->Type() == EPhoneNoteDtmfSending ) {
        showDtmfNote(noteParam);
    }
        
}

void PhoneNoteController::showQuery(TPhoneCommandParam *commandParam)
{
    PHONE_DEBUG("PhoneNoteController::showQuery");
    TPhoneCmdParamQuery& params = *static_cast<TPhoneCmdParamQuery*>( commandParam );

    if ( EPhoneQueryDialog == params.QueryType() && 
         params.QueryPrompt().Length() ) {
        showDefaultQuery(&params);
       
    } else if ( EPhoneGlobalWaitNote == params.QueryType() ) {
        showGlobalWaitNote(&params);
        
    }

        
}

void PhoneNoteController::removeDtmfNote()
{
    PHONE_DEBUG("PhoneNoteController::removeDtmfNote"); 
    if (m_dtmfNote) {
        m_dtmfNote->close();
    }
}

void PhoneNoteController::removeNote()
{
    PHONE_DEBUG("PhoneNoteController::removeNote"); 
    removeDtmfNote();
}

void PhoneNoteController::removeQuery()
{
    PHONE_DEBUG("PhoneNoteController::removeQuery"); 
    if (m_queryNote) {
        m_queryNote->close();
    }
}

void PhoneNoteController::removeGlobalWaitNote()
{
    PHONE_DEBUG("PhoneNoteController::removeGlobalWaitNote"); 
    if (m_timer) {
        m_timeoutCommand = -1;
        m_timer->stop();
    }
    
    if (m_progressDialog) {
        m_queryCanceledCommand = -1;
        m_progressDialog->close();
    }
}

void PhoneNoteController::destroyDialog()
{
    PHONE_DEBUG("PhoneNoteController::destroyDialog"); 
    HbDeviceMessageBox *messageBox = m_messageBoxList.takeFirst();
    messageBox->deleteLater();
    messageBox = 0;
    
    if ( 0 < m_messageBoxList.size() ) {
        PHONE_DEBUG("PhoneNoteController::show pending note");
        HbDeviceMessageBox *messageBoxTemp = m_messageBoxList[0];
        QObject::connect(messageBoxTemp, SIGNAL(aboutToClose()), 
                         this, SLOT(destroyDialog()));
        messageBoxTemp->show();
    }
}

void PhoneNoteController::removeMappings()
{
    foreach (HbAction *action, m_actions ) {
        m_signalMapper->removeMappings(action);
    }
    m_actions.clear();
    
    if (m_dtmfNote) {
        m_dtmfNote->deleteLater();
        m_dtmfNote = 0;
    }
    if (m_queryNote) {
        m_queryNote->deleteLater();
        m_queryNote = 0;
    }
    if (m_progressDialog) {
        m_progressDialog->deleteLater();
        m_progressDialog = 0;
    }
}

void PhoneNoteController::queryCancelled()
{
    if (m_timer) {
        m_timer->stop();
    }
    
    if (m_queryCanceledCommand != -1) {
        emit command(m_queryCanceledCommand);
    }
    m_queryCanceledCommand = -1;
    m_timeoutCommand = -1;
}

void PhoneNoteController::queryTimeout()
{
    int sendCommand = m_timeoutCommand;
    if (m_progressDialog) {
        m_queryCanceledCommand = -1;
        m_progressDialog->close();
    }
    if (sendCommand != -1) {        
        emit command(sendCommand);
    }
}

QString PhoneNoteController::globalNoteText(
        TPhoneCommandParam *commandParam)
{
    TPhoneCmdParamGlobalNote* globalNoteParam = 
        static_cast<TPhoneCmdParamGlobalNote*>( commandParam );
    
    QString ret;    
    
    if ( globalNoteParam->TextResourceId() && 
         KErrNone != globalNoteParam->Text().Compare( KNullDesC() ) ) {
         // resource and text exists
         ret = PhoneResourceAdapter::Instance()->convertToString(
                     globalNoteParam->TextResourceId(),
                     QString::fromUtf16(globalNoteParam->Text().Ptr(), 
                                     globalNoteParam->Text().Length()) );         
    } else if ( globalNoteParam->TextResourceId() ) {
        // resource exists
        ret = PhoneResourceAdapter::Instance()->convertToString(
                    globalNoteParam->TextResourceId());

    } else if ( KErrNone != globalNoteParam->Text().Compare( KNullDesC() ) ) {
        // text exists
        ret = QString::fromUtf16(globalNoteParam->Text().Ptr(), 
                globalNoteParam->Text().Length());
    }
    
    return ret;
}

void PhoneNoteController::showDtmfNote(TPhoneCmdParamNote* noteParam)
{    
    if (m_dtmfNote) {
        m_dtmfNote->setText( QString::fromUtf16(noteParam->Text().Ptr(), 
                noteParam->Text().Length()) );
        m_dtmfNote->update();
    } else {
        QList<HbAction*> hbactions = PhoneResourceAdapter::Instance()->
            convertToHbActions(noteParam->ResourceId());

        if (hbactions.count() > 0) {
            m_dtmfNote = new HbProgressDialog(HbProgressDialog::ProgressDialog);
            m_dtmfNote->setText( QString::fromUtf16(noteParam->Text().Ptr(), 
                    noteParam->Text().Length()) ); 
                  
            connect(hbactions.at(0), SIGNAL(triggered()), m_signalMapper, SLOT(map()));

            int count = m_dtmfNote->actions().count();
            for (int i=count;0<i;i--) {
                QAction *action = m_dtmfNote->actions().at(i-1);
                m_dtmfNote->removeAction(action);
                //TODO
                //delete action;
            }
            m_dtmfNote->addAction(hbactions.at(0));
            m_signalMapper->setMapping(hbactions.at(0), hbactions.at(0)->data().toInt());
            
            QObject::connect(m_dtmfNote, SIGNAL(aboutToClose()), 
                             this, SLOT(removeMappings())); 
            
            m_actions.append(hbactions.at(0));
      
            m_dtmfNote->show();
        }
    }
}

void PhoneNoteController::showDefaultQuery(TPhoneCmdParamQuery* params)
{    
    if (!m_queryNote) {
        QList<HbAction*> hbactions = PhoneResourceAdapter::Instance()->
            convertToHbActions(params->QueryResourceId());
        
        if (hbactions.count() > 0) {
            m_queryNote = new HbMessageBox(HbMessageBox::MessageTypeQuestion);
            m_queryNote->setTimeout(HbPopup::NoTimeout);
            m_queryNote->setDismissPolicy(HbPopup::NoDismiss);
            m_queryNote->setText(QString::fromUtf16(params->QueryPrompt().Ptr(), 
                    params->QueryPrompt().Length()));
            
            QObject::connect(m_queryNote, SIGNAL(aboutToClose()), 
                             this, SLOT(removeMappings())); 
                            
            for (int i = 0; i < hbactions.count(); ++i) {
                connect(hbactions.at(i), SIGNAL(triggered()), m_signalMapper, SLOT(map()));
                m_signalMapper->setMapping(hbactions.at(i), hbactions.at(i)->data().toInt());
                m_actions.append(hbactions.at(i));
            }
            
            int count = m_queryNote->actions().count();
            for (int i=count;0<i;i--) {
                QAction *action = m_queryNote->actions().at(i-1);
                m_queryNote->removeAction(action);
                //TODO
                //delete action;
            }
            
            for (int i=0;i<hbactions.count();i++) {
                m_queryNote->addAction(hbactions.at(i));
            }
            
            m_queryNote->show();
        }
    }
}

void PhoneNoteController::showGlobalWaitNote(TPhoneCmdParamQuery* params)
{    
    if (!m_progressDialog) {        
        m_queryCanceledCommand = params->CbaCommandMapping(EAknSoftkeyCancel);

        m_progressDialog = new HbDeviceProgressDialog(HbProgressDialog::WaitDialog);
        
        if (params->QueryPrompt().Length()) {
            m_progressDialog->setText(QString::fromUtf16(params->QueryPrompt().Ptr(), 
                    params->QueryPrompt().Length()));
        } else if (0 != params->DataText()) {
            m_progressDialog->setText(QString::fromUtf16(params->DataText()->Ptr(), 
                    params->DataText()->Length()));
        }
        
        QObject::connect(m_progressDialog, SIGNAL(aboutToClose()), 
                         this, SLOT(removeMappings())); 
        
        QObject::connect(m_progressDialog, SIGNAL(cancelled()), 
                         this, SLOT(queryCancelled())); 
        
        if (params->TimeOut() != 0) {
            if (!m_timer) {
                m_timer = new QTimer();
                m_timer->setSingleShot(true);
                connect(m_timer, SIGNAL(timeout()), SLOT(queryTimeout()));
            }           
            
            m_timeoutCommand = -1;
            int customCommand;
            if (-1 != params->GetCustomCommandForTimeOut(customCommand)) {
                m_timeoutCommand = customCommand;
            }
            
            m_timer->start(params->TimeOut());
        }
        
        m_progressDialog->show();
    }
}


