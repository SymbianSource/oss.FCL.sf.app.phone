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
#include "cpdivertselectionitem.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CpDivertSelectionItem::CpDivertSelectionItem
// -----------------------------------------------------------------------------
//
CpDivertSelectionItem::CpDivertSelectionItem( 
        QGraphicsItem * parent )
    //:
    //HbDataFormViewItem( /*parent*/ )
    {
    
    }


// -----------------------------------------------------------------------------
// CpDivertSelectionItem::~CpDivertSelectionItem
// -----------------------------------------------------------------------------
//
CpDivertSelectionItem::~CpDivertSelectionItem(  )
    {
    
    }


// -----------------------------------------------------------------------------
// CpDivertSelectionItem::createItem
// -----------------------------------------------------------------------------
//
HbAbstractViewItem * CpDivertSelectionItem::createItem(  )
    {
    SMC_MOCK_METHOD0( HbAbstractViewItem * )
    }


// -----------------------------------------------------------------------------
// CpDivertSelectionItem::canSetModelIndex
// -----------------------------------------------------------------------------
//
bool CpDivertSelectionItem::canSetModelIndex( 
        const QModelIndex & index ) const
    {
    SMC_MOCK_METHOD1( bool, const QModelIndex &, index )
    }


// -----------------------------------------------------------------------------
// CpDivertSelectionItem::restore
// -----------------------------------------------------------------------------
//
void CpDivertSelectionItem::restore(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// CpDivertSelectionItem::createCustomWidget
// -----------------------------------------------------------------------------
//
HbWidget * CpDivertSelectionItem::createCustomWidget(  )
    {
    SMC_MOCK_METHOD0( HbWidget * )
    }


