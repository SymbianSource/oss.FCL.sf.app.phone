/*!
* Copyright (c) 2009-2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Recognizes the parser messages that needs to be notified to 
*               the world using QtHighway.
*/

#ifndef PARSERRECOGNIZER_H
#define PARSERRECOGNIZER_H

#include <QObject>
#include <xqservicerequest.h>

#ifdef BUILD_PARSERRECOGNIZER
#define PARSERRECOGNIZER_EXPORT Q_DECL_EXPORT
#else
#define PARSERRECOGNIZER_EXPORT Q_DECL_IMPORT
#endif

class PARSERRECOGNIZER_EXPORT ParserRecognizer : public QObject
{
    Q_OBJECT

public:
    ParserRecognizer(QObject *parent = 0);
    ~ParserRecognizer();
    
    /*!
        \fn sendMessage(const int message, const int callId)
        
        This method takes a Phone Engine message and a caller id.
        It recognizes the messages that needs to be sent to the
        QtHighway. If message isn't recognized it doesn't do anything.
    */
    void sendMessage(const int message, const int callId);

private slots:
    void requestCompleted(const QVariant &returnValue);
    void requestError(int error);
    
private:
    XQServiceRequest *m_currentRequest;
};

#endif // PARSERRECOGNIZER_H
