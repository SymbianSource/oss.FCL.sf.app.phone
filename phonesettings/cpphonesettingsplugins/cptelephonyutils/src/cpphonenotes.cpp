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
#include "cpphonenotes.h"
#include "cpplugincommon.h"
#include "cppluginlogging.h"
#include <hbdevicemessagebox.h>
#include <HbDeviceProgressDialog>
#include <hblistview.h>
#include <hbdialog.h>
#include <hblabel.h>
#include <hbaction.h>
#include <hbmessagebox.h>
#include <hbnotificationdialog.h>
#include <QStandardItemModel>
#include <QItemSelectionModel>
#include <hbstringutil.h>
#include <hbextendedlocale.h>
#include <HbInputDialog>
#include <HbEditorInterface>
#include <hbinputdef.h>
#include <hbinputstandardfilters.h>

/*!
  CpPhoneNotes::instance.
 */
CpPhoneNotes* CpPhoneNotes::instance()
{
    DPRINT << ": IN";
  
    static CpPhoneNotes theInstance;
    
    DPRINT << ", instance address: " << reinterpret_cast<int>(&theInstance);
    return &theInstance;
}
 
/*!
  CpPhoneNotes::CpPhoneNotes.
 */
CpPhoneNotes::CpPhoneNotes(): 
     QObject(NULL), 
     m_notesQueue(NULL),
     m_isNoteShowingOngoing(false),
     m_passwordDialog(NULL),
     m_passwordValidator(NULL)
    {
    DPRINT << ": IN";

    m_notesQueue = new QQueue<QObject*>();
    m_cpSettingsWrapper = new CpSettingsWrapper;
    
    DPRINT << ": OUT";
    }

/*!
  CpPhoneNotes::~CpPhoneNotes.
 */
CpPhoneNotes::~CpPhoneNotes()
{
    DPRINT << ": IN";

    delete m_cpSettingsWrapper;
    qDeleteAll(*m_notesQueue);
    delete m_notesQueue;
        
    
    DPRINT << ": OUT";
}

/*!
  CpPhoneNotes::showGlobalProgressNote.
 */
void CpPhoneNotes::showGlobalProgressNote(
    int &noteId, const QString& text)
{
    DPRINT << ": IN";
    
    HbDeviceProgressDialog *note = 
        new HbDeviceProgressDialog(HbProgressDialog::WaitDialog, this);
    note->setText(text);
    
    if(hbTrId("txt_common_info_requesting") == text){
        QAction *action = new QAction(hbTrId("txt_common_button_hide"), note);
        note->setAction(action, HbDeviceProgressDialog::CancelButtonRole );
    }
    
    noteId = reinterpret_cast<int>(note);
    DPRINT << ", NOTEID: " << noteId;
    QObject::connect(
        note, SIGNAL(aboutToClose()),
        this, SLOT(activeNoteAboutToClose()));
    QObject::connect(
        note, SIGNAL(cancelled()),
        this, SLOT(handleProgressNoteCanceled()));
    m_notesQueue->enqueue(note);    
    launchNextNoteIfReady();
    
    DPRINT << ": OUT";
}
    
/*!
  CpPhoneNotes::showGlobalNote.
 */
void CpPhoneNotes::showGlobalNote(int &noteId, const QString& text, 
    HbMessageBox::MessageBoxType messageBoxType)
{
    DPRINT << ": IN";

    HbDeviceMessageBox *note
        = new HbDeviceMessageBox(text, messageBoxType, this);
    if (messageBoxType == HbMessageBox::MessageTypeQuestion ||
        messageBoxType == HbMessageBox::MessageTypeInformation) {
        note->setTimeout(HbPopup::ConfirmationNoteTimeout);
    }
    else {
        note->setTimeout(0);
    }
    noteId = reinterpret_cast<int>(note);
    DPRINT << ", NOTEID: " << noteId;
    
    QObject::connect(
        note, SIGNAL(aboutToClose()),
        this, SLOT(activeNoteAboutToClose()));
    
    m_notesQueue->enqueue(note);
    launchNextNoteIfReady();
    
    DPRINT << ": OUT";
}


/*!
  CpPhoneNotes::cancelNote.
 */
void CpPhoneNotes::cancelNote(int noteId)
{
    DPRINT << ": IN";
    
    if (!m_notesQueue->isEmpty()) {
        QObject *note = m_notesQueue->head();
        if(note == reinterpret_cast<QObject *>(noteId)) {
            int index = m_notesQueue->indexOf(reinterpret_cast<QObject *>(noteId));
            Q_ASSERT(-1 < index);
            QObject *note = m_notesQueue->at(index);
            DPRINT << ": NOTEID: " << noteId;
            if (qobject_cast<HbDeviceProgressDialog *>(note)) {
                static_cast<HbDeviceProgressDialog *>(note)->close();
            } else if (qobject_cast<HbDeviceMessageBox *>(note)) {
                static_cast<HbDeviceMessageBox *>(note)->close();
            } else {
                DPRINT << ", UNKNOWN NOTE";
                Q_ASSERT(false);
            }
        }
        else {
            DPRINT << ": remove from queue, noteId: " << noteId;
            m_notesQueue->removeOne(reinterpret_cast<QObject *>(noteId));
        }
    }
    
    DPRINT << ": OUT";
}

/*!
  CpPhoneNotes::noteShowing.
 */
bool CpPhoneNotes::noteShowing()
{
    return !m_notesQueue->isEmpty();
}

/*!
  CpPhoneNotes::showGlobalErrorNote.
 */
void CpPhoneNotes::showGlobalErrorNote(int &noteId, int errorcode)
{
    DPRINT << ": IN";
    
    QString errorText = "";
    Tools::errorCodeTextMapping(errorcode, errorText);

    HbDeviceMessageBox *note 
        = new HbDeviceMessageBox(errorText, HbMessageBox::MessageTypeWarning, this);
    note->setTimeout(0);
    noteId = reinterpret_cast<int>(note);
    DPRINT << ", NOTEID: " << noteId;
    QObject::connect(
        note, SIGNAL(aboutToClose()),
        this, SLOT(activeNoteAboutToClose()));
    m_notesQueue->enqueue(note);
    launchNextNoteIfReady();
    
    DPRINT << ": OUT";
} 

/*!
  CpPhoneNotes::showCallDivertDetails.
 */
void CpPhoneNotes::showCallDivertDetails(
    const PSCallDivertingStatus &divertStatus)
{
    DPRINT << ": IN";
    
    QScopedPointer<HbMessageBox> divertInfoScopedPointer(
        new HbMessageBox(HbMessageBox::MessageTypeInformation));
    divertInfoScopedPointer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    
    // TODO: Orbit layout support is insufficient currently and all text
    // is not shown.
    QString content = "";
    content.append(hbTrId("txt_phone_info_number"));
    content.append(formatPhoneNumber(divertStatus.iNumber));
    if (0 < divertStatus.iTimeout) {
        content.append(hbTrId("txt_phone_setlabel_delay"));
        content.append(QString::number(divertStatus.iTimeout));
        content.append(hbTrId(" seconds"));
    }
    divertInfoScopedPointer->setText(content);
    HbAction *backAction = new HbAction(
        hbTrId("txt_common_button_back"), 
        divertInfoScopedPointer.data());
    divertInfoScopedPointer->addAction(backAction);
    
    HbMessageBox *divertInfo = divertInfoScopedPointer.take();
    QObject::connect(
            divertInfo, SIGNAL(finished(HbAction*)), 
            divertInfo, SLOT(deleteLater()));
    divertInfo->show();
    
    DPRINT << ": OUT";
}

/*!
  CpPhoneNotes::showPasswordQueryDialog.
 */
void CpPhoneNotes::showPasswordQueryDialog(
    const QString &title, 
    const QValidator &validator,
    int maxPasswordLength)
{
    DPRINT << ": IN";
    
    QScopedPointer<HbInputDialog> passwordDialog(new HbInputDialog());
    
    // configure editor so that only digits can be inputted
    passwordDialog->setPromptText(title);
    passwordDialog->setEchoMode(HbLineEdit::Password);
    passwordDialog->setInputMethodHints(Qt::ImhDigitsOnly);
    passwordDialog->actions().at(0)->setEnabled(false);
    
    HbLineEdit *hbLineEdit = passwordDialog->lineEdit();
    hbLineEdit->setMaxLength(maxPasswordLength);
    
    HbEditorInterface editorInterface(hbLineEdit);
    editorInterface.setMode(HbInputModeNumeric);
    editorInterface.setInputConstraints(HbEditorConstraintFixedInputMode);
    editorInterface.setFilter(HbDigitsOnlyFilter::instance());
    
    m_passwordValidator = &validator;
    
    connect(
        hbLineEdit, SIGNAL(contentsChanged()), 
        this, SLOT(passwordTextChanged()));
    
    passwordDialog->open(this, SLOT(finishedPasswordQueryDialog(HbAction*)));
	if(m_passwordDialog) {
		m_passwordDialog->deleteLater();
		m_passwordDialog = NULL;
	}
    m_passwordDialog = passwordDialog.take();
    m_passwordDialog->setParent(this);
    DPRINT << ": OUT";
}

/*!
  CpPhoneNotes::finishedPasswordQueryDialog.
 */
void CpPhoneNotes::finishedPasswordQueryDialog(HbAction* action)
{
    bool ok;
    QString password;
    if(m_passwordDialog) {
        if (action == m_passwordDialog->actions().at(1)) {
            ok = false;
        } else {
            ok = true;
            password = m_passwordDialog->value().toString();
        }
        
        disconnect(
            m_passwordDialog->lineEdit(), SIGNAL(contentsChanged()), 
            this, SLOT(passwordTextChanged()));
        
        m_passwordDialog->deleteLater();
        m_passwordDialog = NULL;
        m_passwordValidator = NULL;
        
        emit passwordQueryCompleted(password, ok);
    }
}

/*!
  CpPhoneNotes::formatPhoneNumber.
  Formats phone number according to locale specific rules.
 */
QString CpPhoneNotes::formatPhoneNumber(QString number) const
{
    DPRINT << ": IN";
    
    QString formattedNumber = number;
    
    if (m_cpSettingsWrapper->numberGroupingSupported() == true) {
        // TODO: utilize HbNumberGrouping API when available
    }
    
    // TODO: digit conversion e.g. into arabic-indic
//    HbExtendedLocale locale = HbExtendedLocale::system();
//    HbStringUtil::convertDigitsTo(formattedNumber, ArabicIndicDigit);
    
    DPRINT << ": OUT";

    return formattedNumber;
}

/*!
  CpPhoneNotes::launchNextNoteIfReady.
 */
void CpPhoneNotes::launchNextNoteIfReady()
{
    DPRINT << ": IN";
    
    if (m_notesQueue->isEmpty()) {
        DPRINT << ", QUEUE EMPTY";
        return;
    }
    if (!m_isNoteShowingOngoing) {
        m_isNoteShowingOngoing = true;
        // note is left in the queue so that it can be cancelled at request
        QObject *note = m_notesQueue->head();
        DPRINT << ", note: " << reinterpret_cast<int>(note);
        if (qobject_cast<HbDeviceProgressDialog *>(note)) {
            DPRINT << ", show HbDeviceProgressDialog";
            static_cast<HbDeviceProgressDialog *>(note)->show();
        } else if (qobject_cast<HbDeviceMessageBox *>(note)) {
            DPRINT << ", show HbDeviceMessageBox";    
            static_cast<HbDeviceMessageBox *>(note)->show();
        } else {
            DPRINT << ", UNKNOWN NOTE";
            Q_ASSERT(false);
        }
    } else {
        DPRINT << ", BUSY";
    }
    
    DPRINT << ": OUT";
}

/*!
  CpPhoneNotes::activeNoteAboutToClose.
 */
void CpPhoneNotes::activeNoteAboutToClose()
{
    DPRINT << ": IN";
    
    if (m_isNoteShowingOngoing) {
        m_isNoteShowingOngoing = false;
        QObject* note(NULL);
        if (!m_notesQueue->isEmpty()) {
            note = m_notesQueue->dequeue();
        }
        if(note) {
            launchNextNoteIfReady();
            note->disconnect(this);
            DPRINT << ", delete note: " << reinterpret_cast<int>(note);
            HbDeviceProgressDialog *pNote = 
                qobject_cast<HbDeviceProgressDialog *>(note);
            note->deleteLater();
        }
    }
    
    DPRINT << ": OUT";
}

/*!
  CpPhoneNotes::handleProgressNoteCanceled().
 */
void CpPhoneNotes::handleProgressNoteCanceled()
{
    DPRINT << ": IN";
    
    emit progressNoteCanceled();
    
    DPRINT << ": OUT";
}

/*!
  CpPhoneNotes::passwordTextChanged().
 */
void CpPhoneNotes::passwordTextChanged()
{
    DPRINT << ": IN";
    Q_ASSERT(m_passwordDialog && m_passwordValidator);
    
    HbLineEdit *hbLineEdit = m_passwordDialog->lineEdit();
    int position = 0;
    QString password = hbLineEdit->text();
    bool isPasswordValid = 
        (QValidator::Acceptable == m_passwordValidator->validate(
            password, position));
    m_passwordDialog->actions().at(0)->setEnabled(isPasswordValid);
        
    DPRINT << ": OUT";
}


/*!
  CpPhoneNotes::showNotificationDialog.
 */
void CpPhoneNotes::showNotificationDialog(const QString& text)
{
    DPRINT << ": IN";
    HbNotificationDialog *notifDialog = new HbNotificationDialog();
    notifDialog->setDismissPolicy(HbPopup::TapAnywhere);
    notifDialog->setAttribute(Qt::WA_DeleteOnClose, true);
    notifDialog->setText(text);
    notifDialog->show();

    DPRINT << ": OUT";
}

// End of File.
