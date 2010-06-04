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

#ifndef INFOWIDGETSATHANDLER_H_
#define INFOWIDGETSATHANDLER_H_

#include <QObject>
#include <rsatservice.h>


class InfoWidgetSatHandlerPrivate;

class InfoWidgetSatHandler: public QObject
    {
    Q_OBJECT
    
public:    
    InfoWidgetSatHandler(QObject *parent = NULL);
    ~InfoWidgetSatHandler();    
    
    void connect(bool connect);
        
    const QString& satDisplayText() const; 
    void setSatDisplayText(const QString& displayText);

    void logCurrentInfo(); 
    
    void handleIdleModeTxtMessage( int idleResult );
    void handleSatError( int operation, int errorCode );

signals: 
    void handleMessage( int message );
    void handleError( int operation, int errorCode );

    
private: 
    QString m_displayText;
    
    int m_connected;
    
    InfoWidgetSatHandlerPrivate *d_ptr;
    RSatService m_satService;
    
    }; 

#endif /* INFOWIDGETSATHANDLER_H_ */


