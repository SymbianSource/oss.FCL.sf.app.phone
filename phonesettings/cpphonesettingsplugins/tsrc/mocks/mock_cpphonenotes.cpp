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
#include <QDebug>
#include <QString>
#include <smcmockclassincludes.h>
#include "cpphonenotes.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CpPhoneNotes::CpPhoneNotes
// -----------------------------------------------------------------------------
//
CpPhoneNotes::CpPhoneNotes( ): QObject(NULL) 
    {
    
    }

// -----------------------------------------------------------------------------
// CpPhoneNotes::instance
// -----------------------------------------------------------------------------
//
CpPhoneNotes * CpPhoneNotes::instance(  )
    {
    static CpPhoneNotes theInstance;
    return &theInstance; 
    }
// -----------------------------------------------------------------------------
// CpPhoneNotes::~CpPhoneNotes
// -----------------------------------------------------------------------------
//
CpPhoneNotes::~CpPhoneNotes(  )
    {

    }



// -----------------------------------------------------------------------------
// CpPhoneNotes::showGlobalProgressNote
// -----------------------------------------------------------------------------
//
void CpPhoneNotes::showGlobalProgressNote( int & noteId, const QString & text )
    {
    SMC_MOCK_METHOD2( void, int &, noteId, const QString &, text )
    }


// -----------------------------------------------------------------------------
// CpPhoneNotes::showGlobalNote
// -----------------------------------------------------------------------------
//
void CpPhoneNotes::showGlobalNote( 
        int & noteId,
        const QString & text,
        HbMessageBox::MessageBoxType noteType )
    {
    SMC_MOCK_METHOD3( void, int &, noteId, 
        const QString &, text, 
        HbMessageBox::MessageBoxType, noteType )
    }


// -----------------------------------------------------------------------------
// CpPhoneNotes::showGlobalErrorNote
// -----------------------------------------------------------------------------
//
void CpPhoneNotes::showGlobalErrorNote( int & noteId, int errorCode )
    {
    SMC_MOCK_METHOD2( void, int &, noteId, int, errorCode )
    }

// -----------------------------------------------------------------------------
// CpPhoneNotes::showBasicServiceList
// -----------------------------------------------------------------------------
//
void CpPhoneNotes::showBasicServiceList(
    const QString &title, 
    const QList<unsigned char> &basicServiceGroupIds)
{
    SMC_MOCK_METHOD2( void, const QString &, title, 
        const QList<unsigned char> &, basicServiceGroupIds )
}

// -----------------------------------------------------------------------------
// CpPhoneNotes::showCallDivertDetails
// -----------------------------------------------------------------------------
//
void CpPhoneNotes::showCallDivertDetails( 
        const PSCallDivertingStatus & divertStatus )
    {
    SMC_MOCK_METHOD1( void, const PSCallDivertingStatus &, divertStatus )
    }


// -----------------------------------------------------------------------------
// CpPhoneNotes::showPasswordQueryDialog
// -----------------------------------------------------------------------------
//
void CpPhoneNotes::showPasswordQueryDialog(
    const QString &title, 
    const QValidator &validator,
    int maxPasswordLength,
    QString &password, 
    bool &ok)
{
    SMC_MOCK_METHOD5( void, 
        const QString &, title, 
        const QValidator &, validator,
        int, maxPasswordLength,
        QString &, password,
        bool &, ok)
}


// -----------------------------------------------------------------------------
// CpPhoneNotes::cancelNote
// -----------------------------------------------------------------------------
//
void CpPhoneNotes::cancelNote( int noteId )
    {
    SMC_MOCK_METHOD1( void, int, noteId )
    }


// -----------------------------------------------------------------------------
// CpPhoneNotes::noteShowing
// -----------------------------------------------------------------------------
//
bool CpPhoneNotes::noteShowing()
{
    SMC_MOCK_METHOD0( bool )
}


// -----------------------------------------------------------------------------
// CpPhoneNotes::activeNoteAboutToClose
// -----------------------------------------------------------------------------
//
void CpPhoneNotes::activeNoteAboutToClose()
{
    SMC_MOCK_METHOD0( void )
}


// -----------------------------------------------------------------------------
// CpPhoneNotes::handleProgressNoteCanceled
// -----------------------------------------------------------------------------
//
void CpPhoneNotes::handleProgressNoteCanceled()
{
    SMC_MOCK_METHOD0( void )
}


// -----------------------------------------------------------------------------
// CpPhoneNotes::passwordTextChanged
// -----------------------------------------------------------------------------
//

void CpPhoneNotes::passwordTextChanged()
{
    SMC_MOCK_METHOD0( void )
}
