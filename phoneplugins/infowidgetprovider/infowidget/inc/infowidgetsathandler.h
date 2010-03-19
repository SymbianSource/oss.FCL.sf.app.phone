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

class InfoWidgetSatHandler: public QObject 
    {
    Q_OBJECT
    
public:    
    InfoWidgetSatHandler(QObject *parent = NULL);
    ~InfoWidgetSatHandler();    

    const QString& satDisplayText() const; 
    void setSatDisplayText(const QString& displayText);

    void logCurrentInfo(); 

signals: 
    void handleMessage( int message );
    void handleError( int operation, int errorCode );

    
private: 
    QString m_displayText; 
    }; 

#endif /* INFOWIDGETSATHANDLER_H_ */


