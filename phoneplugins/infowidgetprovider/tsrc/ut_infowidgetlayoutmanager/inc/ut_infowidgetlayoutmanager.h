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

#ifndef UT_INFOWIDGETLAYOUTMANAGER_H
#define UT_INFOWIDGETLAYOUTMANAGER_H

#include <QtTest/QtTest>
#include <mockservice.h>

class InfoWidgetLayoutManager;

class UT_InfoWidgetLayoutManager : public QObject, public MockService
{
    Q_OBJECT

public:    
    
    UT_InfoWidgetLayoutManager();
    ~UT_InfoWidgetLayoutManager();
    
private slots:

    void init();
    void cleanup();
    
private:
    
    InfoWidgetLayoutManager *m_layoutManager;

};

#endif // UT_INFOWIDGETLAYOUTMANAGER_H
