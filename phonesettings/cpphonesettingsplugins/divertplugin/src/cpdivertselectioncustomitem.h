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

#ifndef CPDIVERTSELECTIONCUSTOMITEM_H
#define CPDIVERTSELECTIONCUSTOMITEM_H

#include <hbwidget.h>

class QGraphicsGridLayout;
class HbCheckBox;
class HbLabel;

class CpDivertSelectionCustomitem : public HbWidget
{
    Q_OBJECT
    Q_PROPERTY(QString number READ number WRITE setNumber)
    Q_PROPERTY(QString timeoutText READ timeoutText WRITE setTimeoutText)
    Q_PROPERTY(int timeout READ timeout WRITE setTimeout)
    Q_PROPERTY(int state READ state WRITE setState)

public:    
    enum State {
        /** Divert is registered and activated.*/
        Enabled,
        /** Divert is registered but inactive.*/
        Deactivated,
        /** Divert is not registered.*/
        Disabled
    };

public:
    CpDivertSelectionCustomitem(
            QGraphicsItem *parent=0,
            Qt::WindowFlags wFlags=0);

    ~CpDivertSelectionCustomitem();

    const QString number() const;

    void setNumber( const QString& number );

    int timeout() const;

    void setTimeout( int timeout);

    const QString timeoutText() const;

    void setTimeoutText( const QString& text );
    
    int state() const;

    void setState( int state);
    
signals:
    void clicked();

private:
    void removeItemFromLayout( QGraphicsWidget* item );
    void updateCheckState();

private:
    QGraphicsGridLayout *layout;
    HbCheckBox *checkbox;
    HbLabel *timeoutLabel;
    HbLabel *timeoutValueLabel;
    int m_state;
};

#endif // CPDIVERTSELECTIONCUSTOMITEM_H
