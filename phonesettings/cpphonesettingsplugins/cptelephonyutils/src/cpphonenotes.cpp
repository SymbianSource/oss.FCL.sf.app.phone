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
#include <QStandardItemModel>
#include <QItemSelectionModel>
#include <hbstringutil.h>
#include <hbextendedlocale.h>

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
     m_isNoteShowingOngoing(false)
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
    QObject* note(NULL);
    foreach (note, *m_notesQueue) {
        delete note;
    }
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
  CpPhoneNotes::showBasicServiceList.
 */
void CpPhoneNotes::showBasicServiceList(
    const QString &title, 
    const QList<unsigned char> &basicServiceGroupIds)
{
    DPRINT << ": IN";
    
    Q_ASSERT(title != "");
    Q_ASSERT(0 < basicServiceGroupIds.count());
    
    QScopedPointer<HbDialog> serviceListPopup(new HbDialog());
    serviceListPopup->setDismissPolicy(HbDialog::NoDismiss);
    serviceListPopup->setTimeout(HbPopup::NoTimeout);
    
    QScopedPointer<HbLabel> heading(
        new HbLabel(title, serviceListPopup.data()));
    heading->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    serviceListPopup->setHeadingWidget(heading.take());
    
    QScopedPointer<HbListView> serviceList(
        new HbListView(serviceListPopup.data()));
    QScopedPointer<QStandardItemModel> serviceListModel(
        new QStandardItemModel(serviceList.data()));
    for (int i = 0; i < basicServiceGroupIds.count(); i++) {
        BasicServiceGroups groupId = 
            static_cast<BasicServiceGroups>(basicServiceGroupIds.at(i));
        QString groupName = basicServiceGroupName(groupId);
        QScopedPointer<QStandardItem> listItem(new QStandardItem(groupName));
        serviceListModel->appendRow(listItem.take());
    }
    serviceList->setModel(serviceListModel.take());
    serviceList->setSelectionMode(HbAbstractItemView::NoSelection);
    serviceListPopup->setContentWidget(serviceList.take());
    
    HbAction *backAction = 
        new HbAction(hbTrId("Back"), serviceListPopup.data());
    serviceListPopup->setPrimaryAction(backAction);
    serviceListPopup->exec();
    
    DPRINT << ": OUT";
}

/*!
  CpPhoneNotes::showBasicServiceCallDivertList.
 */
void CpPhoneNotes::showBasicServiceCallDivertList(
    const QString &title,
    const QList<PSCallDivertingStatus*> &divertStatuses,
    int &selectionIndex,
    CallDivertType divertType,
    CallDivertType &divertDetailType)
{
    DPRINT << ": IN";
    
    Q_ASSERT(title != "");
    Q_ASSERT(0 < divertStatuses.size());
    
    HbDialog *serviceListPopup = new HbDialog();
    serviceListPopup->setDismissPolicy(HbDialog::NoDismiss);
    serviceListPopup->setTimeout(HbPopup::NoTimeout);
    
    HbLabel *heading = new HbLabel(title, serviceListPopup);
    heading->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    serviceListPopup->setHeadingWidget(heading);
    
    HbListView *serviceList = new HbListView(serviceListPopup);
    QStandardItemModel *serviceListModel = new QStandardItemModel(serviceList);
    for (int i = 0; i < divertStatuses.size(); i++) {
        PSCallDivertingStatus *divertStatus = divertStatuses.at(i);
        QString groupName = 
            basicServiceGroupName( 
                convertEtelMobileServiceCode(divertStatus->iServiceGroup) );
        QStandardItem *listItem = new QStandardItem(groupName);
        serviceListModel->appendRow(listItem);
    }
    serviceList->setModel(serviceListModel);
    serviceList->setSelectionMode(HbAbstractItemView::SingleSelection);
    if (0 != serviceListModel->rowCount()) {
        QModelIndex firstItem = serviceList->nextIndex(QModelIndex()); 
        serviceList->setCurrentIndex(firstItem, QItemSelectionModel::Select);
    }
    serviceListPopup->setContentWidget(serviceList);
    
    HbAction *backAction = new HbAction(hbTrId("Back"), serviceListPopup);
    HbAction *numberQueryAction = new HbAction(hbTrId("Number"), serviceListPopup);
    HbAction *detailsAction = new HbAction(hbTrId("Details"), serviceListPopup);
    serviceListPopup->setSecondaryAction(backAction);
    
    switch (divertType) {
        case NoCallDivert:
            break;
        case CallDivertWithNumber:
            serviceListPopup->setPrimaryAction(numberQueryAction);
            break;
        case CallDivertWithNumberAndTimeout:
            serviceListPopup->setPrimaryAction(detailsAction);
            break;
        default:
            DPRINT << ", DEFAULT";
            Q_ASSERT(false);
            break;
    }
    
    HbAction *userAction = serviceListPopup->exec();
    if (userAction == numberQueryAction) {
        divertDetailType = CallDivertWithNumber;
    } else if (userAction == detailsAction) {
        divertDetailType = CallDivertWithNumberAndTimeout;
    } else if (userAction == backAction) {
        divertDetailType = NoCallDivert;
    } else {
        DPRINT << ", WEIRD ACTION";
        Q_ASSERT(false);
    }
    
    selectionIndex = serviceList->currentIndex().row();
    delete serviceListPopup;
    
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
              //  QObject::disconnect(
                //    note, SIGNAL(cancelled()),
                  //  this, SLOT(ProgresNoteCanceled()));
                static_cast<HbDeviceProgressDialog *>(note)->cancel();
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
            note->deleteLater();
        }
    }
    
    DPRINT << ": OUT";
}

/*!
  CpPhoneNotes::basicServiceGroupName.
  Resolves basic service group name by group identifier.
 */
QString CpPhoneNotes::basicServiceGroupName(BasicServiceGroups basicServiceGroupId) const
{
    DPRINT << ": IN";
    
    QString string = "";
    switch (basicServiceGroupId) {
        case AllTeleAndBearer:
            string = hbTrId("All services");
            break;
        case AllTele:
            string = hbTrId("Voice, fax and messages");
            break;
        case Telephony:
            string = hbTrId("Voice calls");
            break;
        case AllDataTele:
            string = hbTrId("Fax and messages");
            break;
        case Fax:
            string = hbTrId("Fax");
            break;
        case Sms:
            string = hbTrId("Messages");
            break;
        case AllTeleExcSms:
            string = hbTrId("Voice calls and fax");
            break;
        case AllBearer:
            if (Tools::videoSupported()) {
                string = hbTrId("Data and video services");
            } else {
                string = hbTrId("Data services");
            }
            break;
        case AllAsync:
            string = hbTrId("Asynchronous services");
            break;
        case AllSync:
            string = hbTrId("Synchronous services");
            break;
        case SyncData:
            string = hbTrId("Synchronous data services");
            break;
        case AsyncData:
            string = hbTrId("Asynchronous data services");
            break;
        case PacketData:
            string = hbTrId("Packet data");
            break;
        case PadAccess:
            string = hbTrId("PAD access");
            break;
        case 30:
            string = hbTrId("Video calls");
            break;
        case AltTele:
            string = hbTrId("Alternate line services");
            break;
        default:
            DPRINT << ", DEFAULT";
            break;
    }
    
    DPRINT << ": OUT";
    return string;
}

/*!
  CpPhoneNotes::convertEtelMobileServiceCode.
*/
BasicServiceGroups CpPhoneNotes::convertEtelMobileServiceCode(int serviceCode) const
{
    DPRINT << "serviceCode: " << serviceCode;
    return Tools::convertEtelMobileServiceCode(serviceCode);
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
    
    HbMessageBox *divertInfo = 
            new HbMessageBox(HbMessageBox::MessageTypeInformation);
    divertInfo->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    
    // TODO: Orbit layout support is insufficient currently and all text
    // is not shown.
    QString content = "";
    content.append(hbTrId("To number: "));
    content.append(formatPhoneNumber(divertStatus.iNumber));
    if (0 < divertStatus.iTimeout) {
        content.append(hbTrId(" Delay time: "));
        content.append(QString::number(divertStatus.iTimeout));
        content.append(hbTrId(" seconds"));
    }
    divertInfo->setText(content);
    HbAction *backAction = new HbAction(hbTrId("Back"), divertInfo);
    divertInfo->setPrimaryAction(backAction);
    divertInfo->exec();
    delete divertInfo;
    
    DPRINT << ": OUT";
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
            static_cast<HbDeviceMessageBox *>(note)->exec();
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
  CpPhoneNotes::handleProgressNoteCanceled().
 */
void CpPhoneNotes::handleProgressNoteCanceled()
{
    DPRINT << ": IN";
    
    emit progressNoteCanceled();
    
    DPRINT << ": OUT";
}

// End of File.
