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
*
*/

#ifndef PHONENOTECONTROLLER_H
#define PHONENOTECONTROLLER_H

#include <QObject>
#include <QString>
#include "tphonecmdparamnote.h"
#include "phoneaction.h"

// FORWARD DECLARATION
class TPhoneCommandParam;
class TPhoneCmdParamQuery;
class HbDeviceMessageBox;
class HbDeviceProgressDialog;
class HbProgressDialog;
class HbMessageBox;
class HbAction;
class QSignalMapper;
class QTimer;


class PhoneNoteController : public QObject
{
    Q_OBJECT
    
public:

    /*!
        \fn PhoneNoteController (QObject *parent)
    */
    PhoneNoteController(QObject *parent = 0);
        
    /*!
        Destructor
     */
    virtual ~PhoneNoteController ();
    
    /*!
        \fn void showGlobalNote ()
        
        This method shows global note.

    */
    void showGlobalNote(TPhoneCommandParam *commandParam);

    /*!
        \fn void showNote ()
        
        This method shows a note.

    */
    void showNote(TPhoneCommandParam *commandParam);
    
    /*!
        \fn void showNote ()
        
        This method shows a query.

    */
    void showQuery(TPhoneCommandParam *commandParam);
    
    /*!
        \fn void removeDtmfNote ()
        
        This method removes dtmf note.

    */
    void removeDtmfNote();
    
    /*!
        \fn void removeNote()
        
        This method removes notes.

    */
    void removeNote();
    
    /*!
        \fn void removeQuery()
        
        This method removes queries.

    */
    void removeQuery();
    
    /*!
        \fn void removeGlobalWaitNote()
        
        This method removes global wait note.

    */
    void removeGlobalWaitNote();
    
signals:

    /*!
        \fn void command(int command)
        
        This signal is emitted when note action is handled.
    */
    void command (int command);
    
private slots:

    /*!
        \fn void destroyDialog ()
        
        Slot is called when global note is closed.
    
    */
    void destroyDialog();

    /*!
        \fn void removeMappings ()
        
        Slot is called when note or query is closed.
    
    */
    void removeMappings();
    
    /*!
        \fn void queryCancelled ()
        
        Slot is called when query is canceled.
    
    */
    void queryCancelled();
  
    /*!
        \fn void queryTimeout ()
        
        Slot is called when query timeout expires.
    
    */
    void queryTimeout();
    
private:

    /*!
        \fn QString globalNoteText()
        
        This method returns text for global note.
    */
    QString globalNoteText(TPhoneCommandParam *commandParam);
    
    
    /*!
        \fn void showDtmfNote()
        
        This method shows dtmf sending note.
    */
    void showDtmfNote(TPhoneCmdParamNote* noteParam);
    
    /*!
        \fn void showDefaultQuery()
        
        This method shows default query.
    */
    void showDefaultQuery(TPhoneCmdParamQuery* params);

    /*!
        \fn void showDefaultQuery()
        
        This method shows global wait note.
    */
    void showGlobalWaitNote(TPhoneCmdParamQuery* params);
    
private:

    QTimer *m_timer;
    QList<HbDeviceMessageBox *> m_messageBoxList;
    HbDeviceProgressDialog *m_progressDialog;
    HbProgressDialog *m_dtmfNote;
    HbMessageBox *m_queryNote;
    QSignalMapper *m_signalMapper;
    QList<HbAction*> m_actions;
    int m_queryCanceledCommand;
    int m_timeoutCommand;
    
};

#endif // PHONENOTECONTROLLER_H
