/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Sets brand according service id.
*
*/


//  Include Files  
#include "telbrandingsettingshandler.h"	// CTelBrandingSettingHandler
#include <spsettings.h>
#include <spproperty.h>

//  Member Functions

// ---------------------------------------------------------
// CTelBrandingSettingsHandler::NewL
// ---------------------------------------------------------
//
CTelBrandingSettingsHandler* CTelBrandingSettingsHandler::NewL()
    {
    CTelBrandingSettingsHandler* self = 
        new (ELeave) CTelBrandingSettingsHandler();
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------
// CTelBrandingSettingsHandler::CTelBrandingSettingsHandler
// ---------------------------------------------------------
//
CTelBrandingSettingsHandler::CTelBrandingSettingsHandler()
    {
    }

// ---------------------------------------------------------
// CTelBrandingSettingsHandler::ConstructL
// ---------------------------------------------------------
//
void CTelBrandingSettingsHandler::ConstructL()
    {

    }

// ---------------------------------------------------------
// CTelBrandingSettingsHandler::BrandId
// ---------------------------------------------------------
//
HBufC* CTelBrandingSettingsHandler::BrandIdLC( TUint32 aServiceId ) const
    {
    HBufC* ret = HBufC::NewLC( KSPMaxDesLength );
    TPtr ptr( ret->Des() );
    CSPSettings* settings = CSPSettings::NewLC();
    CSPProperty* property = CSPProperty::NewLC();
    
    User::LeaveIfError( settings->FindPropertyL( aServiceId, 
        EPropertyBrandId, *property) );
    
    User::LeaveIfError( property->GetValue( ptr ) );
    
    CleanupStack::PopAndDestroy( property );
    CleanupStack::PopAndDestroy( settings );
    
    return ret;
    }

// ---------------------------------------------------------
// CTelBrandingSettingsHandler:::~CTelBrandingSettingsHandler
// ---------------------------------------------------------
//
CTelBrandingSettingsHandler::~CTelBrandingSettingsHandler()
    {

    }



// end of file
