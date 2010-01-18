/*
* Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
#include "telbrandingserverhandler.h"	// CTelBrandingServerHandler
#include <mbsaccess.h>
#include <mbselement.h>
#include <cbsfactory.h>
#include <cbsbitmap.h>
#include <barsread.h>

_LIT8( KApplicationId, "xsp" );
_LIT8( KDefaultBrandId, "xsp" );

//  Member Functions

// ---------------------------------------------------------
// CTelBrandingServerHandler::NewL
// ---------------------------------------------------------
//
CTelBrandingServerHandler* CTelBrandingServerHandler::NewLC( 
    const TDesC8& aBrandId )
    {
    CTelBrandingServerHandler* self = 
        new (ELeave) CTelBrandingServerHandler();
    
    CleanupStack::PushL( self );
    self->ConstructL( aBrandId );
    return self;
    }

// ---------------------------------------------------------
// CTelBrandingServerHandler::CTelBrandingServerHandler
// ---------------------------------------------------------
//
CTelBrandingServerHandler::CTelBrandingServerHandler() :
    iAccess( NULL ),
    iFactory( NULL )
    {
    }

// ---------------------------------------------------------
// CTelBrandingServerHandler::ConstructL
// ---------------------------------------------------------
//
void CTelBrandingServerHandler::ConstructL( const TDesC8& aBrandId )
    {
    TInt languageId( ELangInternationalEnglish );
    
    // connect to branding server
    iFactory = CBSFactory::NewL( 
        KDefaultBrandId,
        KApplicationId );

    
    // Create access to Branding server
    iAccess = iFactory->CreateAccessL( aBrandId, 
        TLanguage( languageId ) );
    }

// ---------------------------------------------------------
// CTelBrandingServerHandler::GetBrandIconL
// ---------------------------------------------------------
//
void CTelBrandingServerHandler::GetBrandIconL( 
    const TDesC8& aImageId,
    CFbsBitmap*& aBitmap,
    CFbsBitmap*& aBitmask ) const
    {
    // Fetch brand image
    iAccess->GetBitmapL( aImageId, aBitmap, aBitmask );
    }

// ---------------------------------------------------------
// CTelBrandingServerHandler:::~CTelBrandingServerHandler
// ---------------------------------------------------------
//
CTelBrandingServerHandler::~CTelBrandingServerHandler()
    {
    if( iAccess )
        {
        iAccess->Close();
        }

    delete iFactory;
    }



// end of file
