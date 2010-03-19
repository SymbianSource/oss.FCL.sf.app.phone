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
#include <QtDebug>
#include <QSignalMapper>
#include <QTimer>
#include <hbdevicemessagebox.h>
#include <hbdeviceprogressdialog.h>
#include <hbmessagebox.h>
#include <hbprogressnote.h>
#include <hbaction.h>
#include <phoneappcommands.hrh>
  
PhoneNoteController::PhoneNoteController(QObject *parent) : 
    QObject(parent), m_timer(0), m_progressDialog(0), m_dtmfNote(0), 
    m_queryNote(0), m_queryCanceledCommand(-1), m_timeoutCommand(-1)
{
    qDebug() << "PhoneNoteController::PhoneNoteController";
    m_signalMapper = new QSignalMapper(this);
    connect(m_signalMapper, SIGNAL(mapped(int)), this, SIGNAL(command (int)));
}

PhoneNoteController::~PhoneNoteController()
{
    qDebug() << "PhoneNoteController::~PhoneNoteController";
    if (m_timer) {
        delete m_timer;
    }
}

void PhoneNoteController::showGlobalNote(TPhoneCommandParam *commandParam)
{
    qDebug() << "PhoneNoteController::showGlobalNote";
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
        if (globalNoteParam->WaitForReady()) {
            HbDeviceMessageBox messageBox(noteString, type);
            messageBox.setTimeout(HbDialog::StandardTimeout);
            messageBox.exec();
        } else {    
            HbDeviceMessageBox *messageBox = new HbDeviceMessageBox(
                    noteString, type);
            messageBox->setTimeout(HbDialog::StandardTimeout);
            m_messageBoxList.append(messageBox);
            
            if (1 == m_messageBoxList.size()) {
                QObject::connect(messageBox, SIGNAL(aboutToClose()), 
                                 this, SLOT(destroyDialog()));            
                messageBox->show();
            }
        }
    }
}

void PhoneNoteController::showNote(TPhoneCommandParam *commandParam)
{
    qDebug() << "PhoneNoteController::showNote";

    TPhoneCmdParamNote* noteParam = static_cast<TPhoneCmdParamNote*>(
            commandParam );
    
    if ( noteParam->Type() == EPhoneNoteDtmfSending ) {
        showDtmfNote(noteParam);
    }
        
}

void PhoneNoteController::showQuery(TPhoneCommandParam *commandParam)
{
    qDebug() << "PhoneNoteController::showQuery";
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
    qDebug() << "PhoneNoteController::removeDtmfNote"; 
    if (m_dtmfNote) {
        m_dtmfNote->close();
    }
}

void PhoneNoteController::removeNote()
{
    qDebug() << "PhoneNoteController::removeNote"; 
    removeDtmfNote();
}

void PhoneNoteController::removeQuery()
{
    qDebug() << "PhoneNoteController::removeQuery"; 
    if (m_queryNote) {
        m_queryNote->close();
    }
}

void PhoneNoteController::removeGlobalWaitNote()
{
    qDebug() << "PhoneNoteController::removeGlobalWaitNote"; 
    if (m_timer) {
        m_timeoutCommand = -1;
        m_timer->stop();
    }
    
    if (m_progressDialog) {
        m_queryCanceledCommand = -1;
        m_progressDialog->cancel();
    }
}

void PhoneNoteController::destroyDialog()
{
    qDebug() << "PhoneNoteController::destroyDialog"; 
    HbDeviceMessageBox *messageBox = m_messageBoxList.takeFirst();
    messageBox->deleteLater();
    messageBox = 0;
    
    if ( 0 < m_messageBoxList.size() ) {
        qDebug() << "PhoneNoteController::show pending note";
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
        m_progressDialog->cancel();
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
            m_dtmfNote = new HbProgressNote(HbProgressNote::ProgressNote);
            m_dtmfNote->setText( QString::fromUtf16(noteParam->Text().Ptr(), 
                    noteParam->Text().Length()) ); 
                  
            connect(hbactions.at(0), SIGNAL(triggered()), m_signalMapper, SLOT(map()));
            m_dtmfNote->setPrimaryAction(hbactions.at(0));
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
            
            if (hbactions.count() == 1) {
                m_queryNote->setPrimaryAction(hbactions.at(0));
            } else if (hbactions.count() > 1) {
                m_queryNote->setPrimaryAction(hbactions.at(0));
                m_queryNote->setSecondaryAction(hbactions.at(1));
            }
            
            m_queryNote->show();
        }
    }
}

void PhoneNoteController::showGlobalWaitNote(TPhoneCmdParamQuery* params)
{    
    if (!m_progressDialog) {        
        m_queryCanceledCommand = params->CbaCommandMapping(EAknSoftkeyCancel);

        m_progressDialog = new HbDeviceProgressDialog(HbProgressNote::WaitNote);
        
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


