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
#ifndef CPPHONENOTES_H
#define CPPHONENOTES_H

#include <QObject>
#include <QString>
#include <QQueue>
#include <hbmessagebox.h>
#include <psetcalldivertingwrapper.h>
#include "cptelephonyutilsdefs.h"

class CpSettingsWrapper;
class HbInputDialog;
class QValidator;

using namespace CpTelephonyUtils;

class CPTELEPHONYUTILS_EXPORT CpPhoneNotes: public QObject
{
    Q_OBJECT
    
public:

    static CpPhoneNotes* instance();
    
private: 
    
    CpPhoneNotes();
    
    ~CpPhoneNotes();

    Q_DISABLE_COPY(CpPhoneNotes)
    
signals:
    
    /**
     This signal is emitted when the device progress dialog is cancelled by 
     user pressing the "cancel" button or by the application itself.
     @param     noteId      On return contains unique note identifier.
     */
    void progressNoteCanceled();
    
    /**
     This signal is emitted when the user coplete password query.
     @param password            On return contains user given password.
     @param ok                  True if the user pressed OK, false if the user 
                                pressed Cancel.
     */
    void passwordQueryCompleted(
        QString password,
        bool okPressed);
    
public slots: // Slots: 
    
    /**
     Shows global progress note. Note showing may be delayed if other note 
     showing is ongoing. 
     @param     noteId      On return contains unique note identifier.
     @param     text        Text to show on a note.
     */
    void showGlobalProgressNote(int &noteId, const QString& text);
    
    /**
     Shows global note. Note showing may be delayed if other note 
     showing is ongoing. 
     @param     noteId      On return contains unique note identifier.
     @param     text        Text to show on a note.
     @param     noteType    Note type.
     */
    void showGlobalNote(int &noteId, const QString& text, 
        HbMessageBox::MessageBoxType noteType);
    
    /**
     Shows global error note. Note showing may be delayed if other note 
     showing is ongoing. 
     @param     noteId      On return contains unique note identifier.
     @param     errorCode   Code specifying error situation. 
     */
    void showGlobalErrorNote(int &noteId, int errorCode);
    
    /**
     Shows the basic service list.
     @param     title                   Heading to be used for the list.
     @param     basicServiceGroupIds    Basic services to be listed. 
     */
    void showBasicServiceList(
        const QString &title, 
        const QList<unsigned char> &basicServiceGroupIds);
    
    /**
     Shows detailed call divert information about the selected divert service
     group.
     @param     divertStatus    Divert status information.
     */
    void showCallDivertDetails(
        const PSCallDivertingStatus &divertStatus);
    
    /**
     Shows password query dialog with OK & Cancel buttons and returns user
     given password unless user has canceled query. Only valid password is
     accepted.
     @param title               Title for the query dialog.
     @param validator           Password validator.
     @param maxPasswordLength   Maximum length for the password.
     */
    void showPasswordQueryDialog(
        const QString &title,
        const QValidator &validator,
        int maxPasswordLength);
    
    /**
     Cancels specified note.
     @param     noteId      Note identifier.
     */
    void cancelNote(int noteId);
    
    /**
     Retuns true if note is currently shown.
     */
    bool noteShowing();
    
    /**
     Slot for HbDialog finished singal.
     */
    void finishedPasswordQueryDialog(HbAction* action);
    
private:
    
    /**
     Resolves basic service group name by identifier.
     @param     basicServiceGroupId     Group identifier.
     @return    Group name.
     */
    QString basicServiceGroupName(BasicServiceGroups basicServiceGroupId) const;
    
    /**
     Formats phone number according to locale specific rules.
     @param     number          Unformatted phone number.
     @return    Formatted number.
     */
    QString formatPhoneNumber(QString number) const;
    
    /**
     Launches next note in the queue if not busy with showing other 
     note currently.
     */
    void launchNextNoteIfReady();

private slots:
    
    /**
     Handler method for notes' about to close signal.
     */
    void activeNoteAboutToClose();
    
    /**
     Handler method for notes' canceled signal.
     */
    void handleProgressNoteCanceled();
    
    /**
     Used for dynamic enable/disable of password dialog's OK button according
     to validity of the currently inputted password.
     */
    void passwordTextChanged();
    
private: // Data: 

    /**
      Cenrep settings wrapper.
     */
     CpSettingsWrapper *m_cpSettingsWrapper;

    /**
      Notes waiting to be shown.
     */
    QQueue<QObject*> *m_notesQueue;
    
    /**
      Indicates whether note controller is busy with some note showing.
     */
    bool m_isNoteShowingOngoing;
    
    /**
      Password query dialog. Own.
     */
    HbInputDialog *m_passwordDialog;

    /**
      Password validator. Not own.
     */
    const QValidator *m_passwordValidator;
};
#endif // CPPHONENOTES_H
