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
*               THIS IS DEPRICATED API - DO NOT USE!
*               (use KeySequenceRecognitionService instead)
*
*/

#ifndef KEYSEQUENCERECOGNITIONSERVICEDEPRICATED_H
#define KEYSEQUENCERECOGNITIONSERVICEDEPRICATED_H

#include <QObject>
#include <QString>
#include <xqserviceprovider.h>

class MPEKeySequenceRecognitionIF;

class KeySequenceRecognitionServiceDepricated : public XQServiceProvider
{
    Q_OBJECT

public:

    KeySequenceRecognitionServiceDepricated(
        MPEKeySequenceRecognitionIF &keySequenceRecognizer,
        QObject *parent = 0);
    
    ~KeySequenceRecognitionServiceDepricated();
    
public slots:
    
    /*!
        \fn executeKeySequence(const QString& keySequence)
        
        Client can use this method to execute key sequence recognition.
        Key sequence may contain for example product codes etc. that
        may cause lower layers to do any specific things.

        If sequence isn't recognized this method doesn't do anything.
        If sequence is recognized then task related to the code is executed.
        \param  keySequence          Key sequence to execute.
        \return True if key sequence was processed, false otherwise.
        
        Usage example:
        XQServiceRequest request(
            "com.nokia.services.telephony",
            "executeKeySequence(QString)",
            true);
        request << "*#0000#";
        QVariant keySequenceProcessed;
        bool requestOk = request.send(keySequenceProcessed);
    */
    bool executeKeySequence(const QString &keySequence);
    
private:
    
    MPEKeySequenceRecognitionIF &m_keySequenceRecognizer;
};

#endif // KEYSEQUENCERECOGNITIONSERVICEDEPRICATED_H
