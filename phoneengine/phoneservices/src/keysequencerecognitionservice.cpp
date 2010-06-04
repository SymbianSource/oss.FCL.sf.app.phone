/*!
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
* Description:  Provides key sequence recognition and execution services.
*
*/

#include <QDebug>
#include <xqserviceutil.h>
#include <e32base.h>
#include "keysequencerecognitionservice.h"
#include "mpekeysequencerecognitionif.h"

/*!
  KeySequenceRecognitionService::KeySequenceRecognitionService.
 */
KeySequenceRecognitionService::KeySequenceRecognitionService(
    MPEKeySequenceRecognitionIF &keySequenceRecognizer,
    QObject* parent) 
    : 
    XQServiceProvider(
        QLatin1String("com.nokia.symbian.IDtmfPlay"), parent),
        m_keySequenceRecognizer(keySequenceRecognizer)
{
    publishAll();
}


/*!
  KeySequenceRecognitionService::~KeySequenceRecognitionService.
 */
KeySequenceRecognitionService::~KeySequenceRecognitionService()
{
}


/*!
  KeySequenceRecognitionService::executeKeySequence.
 */
bool KeySequenceRecognitionService::executeKeySequence(
    const QString &keySequence)
{
    TPtrC16 keySequencePtr(
        reinterpret_cast<const TUint16*>(keySequence.utf16()));
    bool sequenceProcessed = false;
    TRAPD(leaveCode, sequenceProcessed = static_cast<bool>(
        m_keySequenceRecognizer.ExecuteKeySequenceL(keySequencePtr)));
    sequenceProcessed = (KErrNone == leaveCode) ? sequenceProcessed : false;
    return sequenceProcessed;
}
