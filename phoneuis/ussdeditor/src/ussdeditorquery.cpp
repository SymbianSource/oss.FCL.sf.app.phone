/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
*
*/

// INCLUDE FILES
#include <hbinputeditorinterface.h>
#include <hbaction.h>
#include <hbinputstandardfilters.h>
#include <hbinputfilter.h> 
#include <hbinputstate.h> 
#include <hbinputvkbhost.h>
#include <hbapplication.h>
#include <cphcltussdint.h>

#include "tflogger.h"
#include "ussdcomms.h"
#include "ussdeditorquery.h"

// CONSTANTS
// The maximum number of editor lines method NumberOfEditorLines can return.
const TInt KUssdMaxNumberOfEditorLines = 16;
// The maximum length of editor lines.
const TInt KUssdEditorMaxLength = 182;

// ============================ MEMBER FUNCTIONS ===============================
// -----------------------------------------------------------------------------
// UssdEditorQuery::UssdEditorQuery
// Constructor.
// -----------------------------------------------------------------------------
//
UssdEditorQuery::UssdEditorQuery(CUssdComms &ussd, QGraphicsItem *parent)
    :HbInputDialog(parent), mComms(ussd)
{
    TFLOGSTRING("USSDEDITOR: UssdEditorQuery::UssdEditorQuery IN")
    setPromptText(hbTrId("Reply"));

    if (lineEdit()){
        // Set max length and rows
        lineEdit()->setMaxLength(KUssdEditorMaxLength);
        lineEdit()->setMaxRows(KUssdMaxNumberOfEditorLines);
        lineEdit()->setText(QString());

        // 0-9, *, +, #
        HbEditorInterface interface(lineEdit());
        interface.setFilter(HbPhoneNumberFilter::instance());
        // TODO: cannot open keypad
        interface.vkbHost()->openKeypad();

        mComms.appStarting();
        // Disable Ok key by default
        primaryAction()->setEnabled(false);

        bool ret(false);
        ret = connect(primaryAction(), SIGNAL(triggered(bool)),
                      this, SLOT(sendUssdString()));
        TFLOGSTRING2("USSDEDITOR: UssdEditorQuery::UssdEditorQuery \
            connect send %d", ret);  
        
        ret = connect(lineEdit(), SIGNAL(textChanged(QString)),
                      this, SLOT(updateButtonVisible(QString)));
        TFLOGSTRING2("USSDEDITOR: UssdEditorQuery::UssdEditorQuery \
            connect ok button %d", ret);
        
        // Connect cancel
        ret = connect(secondaryAction(), SIGNAL(triggered(bool)),
                      this, SLOT(cancelUssdString()));
        TFLOGSTRING2("USSDEDITOR: UssdEditorQuery::UssdEditorQuery \
            connect send %d", ret);
    }
    TFLOGSTRING("USSDEDITOR: UssdEditorQuery::UssdEditorQuery OUT")
}

// -----------------------------------------------------------------------------
// UssdEditorQuery::~UssdEditorQuery
// ~UssdEditorQuery.
// -----------------------------------------------------------------------------
//
UssdEditorQuery::~UssdEditorQuery()
{
    TFLOGSTRING("USSDEDITOR: UssdEditorQuery::~UssdEditorQuery IN-OUT")
}

// -----------------------------------------------------------------------------
// UssdEditorQuery::sendUssdString
// sendUssdString.
// -----------------------------------------------------------------------------
//
void UssdEditorQuery::sendUssdString()
{
    TFLOGSTRING("USSDEDITOR: UssdEditorQuery::sendUssdString IN"); 
    if (lineEdit() && 
        0 < lineEdit()->text().length() &&
        KUssdEditorMaxLength >= lineEdit()->text().length()) {
        int ret = mComms.send(lineEdit()->text());
        TFLOGSTRING2("USSDEDITOR: UssdEditorQuery::sendUssdString %d", ret);
        mComms.informExitReason(static_cast<int>(EPhCltSendCompleted));
    }
    TFLOGSTRING("USSDEDITOR: UssdEditorQuery::sendUssdString OUT");
    HbApplication::quit();
}

// -----------------------------------------------------------------------------
// UssdEditorQuery::cancelUssdString
// cancelUssdString.
// -----------------------------------------------------------------------------
//
void UssdEditorQuery::cancelUssdString()
{
    TFLOGSTRING("USSDEDITOR: UssdEditorQuery::cancelUssdString IN-OUT"); 
    mComms.informExitReason((EPhCltUserExit));
    HbApplication::quit();
}

// -----------------------------------------------------------------------------
// UssdEditorQuery::updateButtonVisible
// updateButtonVisible.
// -----------------------------------------------------------------------------
//
void UssdEditorQuery::updateButtonVisible( const QString &text )
{
    TFLOGSTRING("USSDEDITOR: UssdEditorQuery::updateButtonVisible IN"); 
    if (text.length() > 0) {
        primaryAction()->setEnabled(true);
    } else {
        primaryAction()->setEnabled(false);
    }
    TFLOGSTRING("USSDEDITOR: UssdEditorQuery::updateOkButton OUT");
}

// End of file
