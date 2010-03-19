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
#include <QDebug>
#include <smcmockclassincludes.h>
#include <cpsettingformentryitemdata.h>

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CpSettingFormEntryItemData::CpSettingFormEntryItemData
// -----------------------------------------------------------------------------
//
CpSettingFormEntryItemData::CpSettingFormEntryItemData( 
        const HbDataFormModelItem * parent )
    //:
    //CpSettingFormItemData( /*parent*/ )
    {
    
    }


// -----------------------------------------------------------------------------
// CpSettingFormEntryItemData::CpSettingFormEntryItemData
// -----------------------------------------------------------------------------
//
CpSettingFormEntryItemData::CpSettingFormEntryItemData( 
        CpItemDataHelper & itemDataHelper,
        const QString & text,
        const QString & description,
        const HbIcon & icon,
        const HbDataFormModelItem * parent )
    //:
    //CpSettingFormItemData( /*itemDataHelper, text, description, icon, parent*/ )
    {
    
    }


// -----------------------------------------------------------------------------
// CpSettingFormEntryItemData::CpSettingFormEntryItemData
// -----------------------------------------------------------------------------
//
CpSettingFormEntryItemData::CpSettingFormEntryItemData( 
        HbDataForm * dataForm,
        const QString & text,
        const QString & description,
        const HbIcon & icon,
        const HbDataFormModelItem * parent )
    //:
    //CpSettingFormItemData( /*dataForm, text, description, icon, parent*/ )
    {
    
    }


// -----------------------------------------------------------------------------
// CpSettingFormEntryItemData::~CpSettingFormEntryItemData
// -----------------------------------------------------------------------------
//
CpSettingFormEntryItemData::~CpSettingFormEntryItemData(  )
    {
    
    }


// -----------------------------------------------------------------------------
// CpSettingFormEntryItemData::text
// -----------------------------------------------------------------------------
//
QString CpSettingFormEntryItemData::text(  ) const
    {
    SMC_MOCK_METHOD0( QString )
    }


// -----------------------------------------------------------------------------
// CpSettingFormEntryItemData::setText
// -----------------------------------------------------------------------------
//
void CpSettingFormEntryItemData::setText( 
        const QString & text )
    {
    SMC_MOCK_METHOD1( void, const QString &, text )
    }


// -----------------------------------------------------------------------------
// CpSettingFormEntryItemData::description
// -----------------------------------------------------------------------------
//
QString CpSettingFormEntryItemData::description(  ) const
    {
    SMC_MOCK_METHOD0( QString )
    }


// -----------------------------------------------------------------------------
// CpSettingFormEntryItemData::setDescription
// -----------------------------------------------------------------------------
//
void CpSettingFormEntryItemData::setDescription( 
        const QString & description )
    {
    SMC_MOCK_METHOD1( void, const QString &, description )
    }


// -----------------------------------------------------------------------------
// CpSettingFormEntryItemData::entryItemIcon
// -----------------------------------------------------------------------------
//
HbIcon CpSettingFormEntryItemData::entryItemIcon(  ) const
    {
//    SMC_MOCK_METHOD0( HbIcon )
    }


// -----------------------------------------------------------------------------
// CpSettingFormEntryItemData::setEntryItemIcon
// -----------------------------------------------------------------------------
//
void CpSettingFormEntryItemData::setEntryItemIcon( 
        const HbIcon & icon )
    {
//    SMC_MOCK_METHOD1( void, const HbIcon &, icon )
    }


