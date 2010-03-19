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

#include "infowidgetpreferences.h"
#include "infowidgetlogging.h"


/*!
    InfoWidgetPreferences::InfoWidgetPreferences() 
*/
InfoWidgetPreferences::InfoWidgetPreferences(QObject *parent): 
    QObject(parent)
{
    DPRINT;
}

/*!
    InfoWidgetPreferences::~InfoWidgetPreferences() 
*/
InfoWidgetPreferences::~InfoWidgetPreferences()
{
    DPRINT;
}

/*!
    InfoWidgetPreferences::loadPreferences() 
*/
void InfoWidgetPreferences::loadPreferences()
{
    DPRINT;
}

/*!
    InfoWidgetPreferences::storePreferences() 
*/
void InfoWidgetPreferences::storePreferences()
{
    DPRINT;
}

/*!
    InfoWidgetPreferences::preference() 
*/
QString InfoWidgetPreferences::preference(Option preferenceId) const
{
    DPRINT << ": preference id: " << static_cast<int>(preferenceId); 
    
    QString preferenceString;
    if (m_options.testFlag(preferenceId)) {
        preferenceString = DISPLAY_SETTING_ON; 
    } else {
        preferenceString = DISPLAY_SETTING_OFF;  
    } 
    
    return preferenceString;
}

/*!
    InfoWidgetPreferences::setPreference() 
*/
void InfoWidgetPreferences::setPreference(Option preferenceId, 
    const QString& preferenceString)
{
    DPRINT << ": preference id: " << static_cast<int>(preferenceId);
    DPRINT << ": preference string: " << preferenceString; 
    DPRINT << ": initial options: " << m_options;
    
    if (preferenceString.compare(DISPLAY_SETTING_ON) == 0) {
        m_options |= preferenceId; 
    } else {
        m_options &= ~preferenceId; 
    }
    
    DPRINT << ": modified options: " << m_options;
}

/*!
    InfoWidgetPreferences::visibleItemCount() 
*/
int InfoWidgetPreferences::visibleItemCount() 
{
    DPRINT << ": IN";
    
    int visibleItems = 0;
    if (m_options.testFlag(DisplayHomeZone)){
        visibleItems++; 
    }
    if (m_options.testFlag(DisplayMcn)){
        visibleItems++; 
    }
    if (m_options.testFlag(DisplayActiveLine)){
        visibleItems++; 
    }
    if (m_options.testFlag(DisplaySatText)){
        visibleItems++; 
    }
    
    DPRINT << ": visible item count: " << visibleItems;
    return visibleItems; 
}

// End of File. 

