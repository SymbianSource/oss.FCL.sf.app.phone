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
* Description:  DTMF API to be used through Qt Highway.
*
*/

#ifndef DTMFSERVICE_H
#define DTMFSERVICE_H

#include <QObject>
#include <QString>
#include <xqserviceprovider.h>
#include "mpecallcontrolif.h"
#include "mpecallsettersif.h"

class DTMFService : public XQServiceProvider
{
    Q_OBJECT
public:
    DTMFService(
        MPECallControlIF &call, 
        MPECallSettersIF &parameters, 
        QObject *parent = 0);
    ~DTMFService();
    
public slots:
    /*!
        \fn playDTMFTone(const QChar& keyToPlay)
        
        This method plays DTMF tone matching given character.
        If character doesn't match any supported DTMF tone
        it is ignored.
        
        Usage example:
        XQServiceRequest snd("com.nokia.symbian.IDtmfPlay","playDTMFTone(QChar)");
        snd << "5";
    */
    void playDTMFTone(const QChar& keyToPlay);

    /*!
        \fn stopDTMFPlay()
        
        This method stops playing DTMF tone if it is currently played.
    */
    void stopDTMFPlay();
    
    
private:
    MPECallControlIF &m_call;
    MPECallSettersIF &m_parameters;    
};

#endif // DTMFSERVICE_H