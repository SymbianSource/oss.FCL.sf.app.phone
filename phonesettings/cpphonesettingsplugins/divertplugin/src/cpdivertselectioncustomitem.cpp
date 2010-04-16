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

#include "cpdivertselectioncustomitem.h"
#include <hblabel.h>
#include <hbcheckbox.h>
#include <qgraphicsgridlayout.h>
#include <cppluginlogging.h>


CpDivertSelectionCustomitem::CpDivertSelectionCustomitem(
        QGraphicsItem *parent /**=0*/,
        Qt::WindowFlags wFlags /**=0*/):
        HbWidget(parent, wFlags),
        layout(NULL),
        checkbox(NULL),
        //numberValueLabel(NULL),
        timeoutLabel(NULL),
        timeoutValueLabel(NULL),
        m_state(Disabled)
{
    DPRINT;

    layout = new QGraphicsGridLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0,0,0,0);
    layout->setRowMaximumHeight(0,0);
    layout->setRowMaximumHeight(1,0);

    checkbox = new HbCheckBox(this);
    checkbox->setBackgroundItem(HbStyle::P_DataItem_background);

    HbStyle::setItemName(checkbox, "dataItem_ContentWidget");

    layout->addItem(checkbox, 0, 0, 1, 2);

    connect(checkbox,SIGNAL(clicked()), this, SIGNAL(clicked()));

    //numberValueLabel = new HbLabel(this);
    //layout->addItem(numberValueLabel, 0, 1);
    //connect(numberValueLabel,SIGNAL(clicked()), this, SIGNAL(clicked()));

    timeoutLabel = new HbLabel(this);
    HbStyle::setItemName(timeoutLabel, "dataItem_ContentWidget");
    timeoutLabel->setAlignment(Qt::AlignRight);
    layout->addItem(timeoutLabel, 1, 0);

    timeoutLabel->hide();
    timeoutValueLabel = new HbLabel(this);
    timeoutValueLabel->setBackgroundItem(HbStyle::P_DataItem_background);
    HbStyle::setItemName(timeoutValueLabel, "dataItem_ContentWidget");
    timeoutValueLabel->hide();
    layout->addItem(timeoutValueLabel, 1, 1);

    setLayout(layout);
}

CpDivertSelectionCustomitem::~CpDivertSelectionCustomitem()
{
    DPRINT;
}

const QString CpDivertSelectionCustomitem::number() const
{
    DPRINT;
    return checkbox->text();
}

void CpDivertSelectionCustomitem::setNumber( const QString& number )
{
    DPRINT << number;
    
    checkbox->setText(number);
}

int CpDivertSelectionCustomitem::timeout() const
{
    DPRINT;
    return timeoutValueLabel->plainText().toInt();
}

void CpDivertSelectionCustomitem::setTimeout( int timeout)
{
    DPRINT << timeout;
    timeoutValueLabel->setPlainText(QString::number(timeout));
}

const QString CpDivertSelectionCustomitem::timeoutText() const
{
    DPRINT;
    return timeoutLabel->plainText();
}

void CpDivertSelectionCustomitem::setTimeoutText( const QString& text )
{
    DPRINT << text;
    timeoutLabel->setPlainText(text);

    if (text != QString("")) {
        timeoutLabel->show();
        timeoutValueLabel->show();

    } else {
        timeoutLabel->hide();
        timeoutValueLabel->hide();
    }
}

int CpDivertSelectionCustomitem::state() const
{
    DPRINT << m_state;
    
    return m_state;
}

void CpDivertSelectionCustomitem::setState(int state)
{
    DPRINT << state;
    
    m_state = state;
    
    updateCheckState();
}

void CpDivertSelectionCustomitem::updateCheckState()
{
    DPRINT << m_state;
    
    switch (m_state) {
        case Enabled:
            checkbox->setCheckState(Qt::Checked);
            break;
        case Deactivated:
            setNumber(QString(""));
            setTimeout(0);
			//intended fall-through
        case Disabled:
            checkbox->setCheckState(Qt::Unchecked);
            break;
        default:
            break;
    }
}
