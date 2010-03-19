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

#ifndef INFOWIDGETNETWORKHANDLER_H_
#define INFOWIDGETNETWORKHANDLER_H_


#include <QObject>
#include <nwhandlingengine.h>

class CNWSession; 

class InfoWidgetNetworkHandler : public QObject, public MNWMessageObserver 
    {
    Q_OBJECT
    
public:
    InfoWidgetNetworkHandler(QObject *parent = NULL);
    ~InfoWidgetNetworkHandler();    
    
    QString homeZoneTextTag() const; 
    int homeZoneIndicatorType() const; 
    
    QString mcnName() const; 
    int mcnIndicatorType() const; 

    void logCurrentInfo(); 

public: // From MNWMessageObserver
    void HandleNetworkMessage(const TNWMessages aMessage);
    void HandleNetworkError(const TNWOperation aOperation, TInt aErrorCode);
    
signals: 
    void networkDataChanged();
    void networkError(int operation, int errorCode);
    
private:
    TNWInfo m_nwInfo;
    QScopedPointer<CNWSession> m_nwSession;
    }; 

#endif /* INFOWIDGETNETWORKHANDLER_H_ */


