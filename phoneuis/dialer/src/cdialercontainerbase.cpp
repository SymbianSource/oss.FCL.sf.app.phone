/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Base for containers
*
*/


// INCLUDE FILES
#include <eikapp.h>
#include <gulicon.h>
#include <AknUtils.h>
#include <aknbutton.h>
#include <AknsUtils.h>
#include <AknsDrawUtils.h>
#include <aknlayout2def.h>
#include <AknsSkinInstance.h>
#include <AknsControlContext.h>
#include <aknlayoutscalable_apps.cdl.h>

#include "cdialercontainerbase.h"
#include "dialercommon.h"
#include "dialertrace.h"

// Constructor
CDialerContainerBase::CDialerContainerBase( 
    CCoeControl& aContainer )
    : iParentControl( aContainer )
    {
    // empty
    }


// Destructor
CDialerContainerBase::~CDialerContainerBase()
    {
    AknsUtils::DeregisterControlPosition( this );

    }

// ---------------------------------------------------------------------------
// CDialerContainerBase::ConstructL
// Symbian OS two phased constructor
// 
// ---------------------------------------------------------------------------
//
void CDialerContainerBase::BaseConstructL( )
    {    
    DIALER_PRINT("ContainerBase::BaseConstructL<");
    SetContainerWindowL( iParentControl );
    SetParent( &iParentControl );

    // Define variety according to features available etc.
    SetVariety();

    DIALER_PRINT("ContainerBase::BaseConstructL>");    
	}

// ---------------------------------------------------------------------------
// CDialerContainerBase::MopSupplyObject
//
// ---------------------------------------------------------------------------
//
TTypeUid::Ptr CDialerContainerBase::MopSupplyObject( TTypeUid aId )
    {
    return CCoeControl::MopSupplyObject( aId );
    }

// ---------------------------------------------------------------------------
// CDialerContainerBase::SizeChanged
//  
// ---------------------------------------------------------------------------
//
void CDialerContainerBase::SizeChanged( )
    {
    SetVariety();
    SetLayout();
    }

// ---------------------------------------------------------------------------
// CDialerContainerBase::PositionChanged
//  
// ---------------------------------------------------------------------------
//
void CDialerContainerBase::PositionChanged()
    {
    AknsUtils::RegisterControlPosition( this );
    }

// ---------------------------------------------------------------------------
// CDialerContainerBase::HandleResourceChange
//  
// ---------------------------------------------------------------------------
//
void CDialerContainerBase::HandleResourceChange( TInt aType )
    {
    // Call implementation class SizeChanged()
    SizeChanged();
    CCoeControl::HandleResourceChange( aType );
    }
    
// End of File
