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


#include "infowidgetsathandler.h"
#include "infowidgetlogging.h"


/*!
 */
InfoWidgetSatHandler::InfoWidgetSatHandler(QObject *parent):
    QObject(parent)
{
    DPRINT << ": IN";
    
    // ToDo: remove when SAT implemented / releasing 
    DPRINT << ": setting DUMMY DATA to sat display text";
    m_displayText = "SAT DISPLAY TEXT"; 
    
    DPRINT << ": OUT";
}

/*!
 */
InfoWidgetSatHandler::~InfoWidgetSatHandler()
{
    DPRINT << ": IN";
    DPRINT << ": OUT";
}    

/*!
 */
void InfoWidgetSatHandler::logCurrentInfo()
{
    DPRINT << ": IN"; 
    DPRINT << ": OUT";
    
} 

/*!
 */
const QString& InfoWidgetSatHandler::satDisplayText() const
{
    DPRINT << ": text: " << m_displayText; 
    return m_displayText;
}

/*!
 */
void InfoWidgetSatHandler::setSatDisplayText(const QString& displayText)
{
    DPRINT << ": display text: " << displayText;
    m_displayText = displayText;
}

// End of File. 

