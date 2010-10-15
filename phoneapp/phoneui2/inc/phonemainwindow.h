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
* Description:  Main window for PhoneUI
*
*/


#ifndef HBPHONEMAINWINDOW_H_
#define HBPHONEMAINWINDOW_H_

#include <QGraphicsItem>
#include <hbmainwindow.h>

class PhoneUiHouseHold;

class PhoneMainWindow: public HbMainWindow
    {
    Q_OBJECT
public:

    PhoneMainWindow(QWidget* parent = 0);
    ~PhoneMainWindow();
    void ReloadTranslations();
    
signals:
    void focusLost();
    void focusGained();
    
public slots:
    void onFocusLost();
    void onFocusGained();
    
protected:
    void changeEvent(QEvent *event);


private: // data
    PhoneUiHouseHold* mHouseHold;
    

    };



#endif /* HBPHONEMAINVIEW_H_ */
