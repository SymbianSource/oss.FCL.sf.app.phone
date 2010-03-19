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

#ifndef UT_CPDIVERTSELECTIONITEM_H
#define UT_CPDIVERTSELECTIONITEM_H

#include <QtTest/QtTest>
#include <mockservice.h>

class CpDivertSelectionItem;
class HbDataForm;

class UT_CpDivertSelectionItem : public QObject, MockService
{
    Q_OBJECT

public:    
    UT_CpDivertSelectionItem();
    ~UT_CpDivertSelectionItem();
    
private slots:

    void init();
    
    void cleanup();
    
    void t_memleak();
    
    void t_createItem();
    void t_canSetModelIndex();
    void t_restore();
    void t_createCustomWidget();
private:
    // Not own
    CpDivertSelectionItem *m_item;
    
    // Own
    HbDataForm *m_form;
    
};

#endif // UT_CPDIVERTSELECTIONITEM_H
