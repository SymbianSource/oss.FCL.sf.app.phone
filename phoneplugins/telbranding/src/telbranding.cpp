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
#include <telbubbleextensionobserver.h>
#include <telbubbleextensiondata.h>
#include <telbubblecustomelement.h>
#include <utf.h>

#include "telbranding.h"	// Ctelbranding
#include "telbrandingsettingshandler.h"
#include "telbrandingserverhandler.h"
#include "telbrandingbubble.h"

//  Member Functions

// ---------------------------------------------------------
// CTelBranding::NewL
// ---------------------------------------------------------
//
CTelBranding* CTelBranding::NewL()
    {
    CTelBranding* self = new (ELeave) CTelBranding();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------
// CTelBranding::CTelBranding
// ---------------------------------------------------------
//
CTelBranding::CTelBranding()
    {
    }

// ---------------------------------------------------------
// CTelBranding::ConstructL
// ---------------------------------------------------------
//
void CTelBranding::ConstructL()
    {
    iSettings = CTelBrandingSettingsHandler::NewL();
    }

// ---------------------------------------------------------
// CTelBranding:::~CTelBranding
// ---------------------------------------------------------
//
CTelBranding::~CTelBranding()
    {
    delete iSettings;
    iBubbleArray.ResetAndDestroy();
    iBubbleArray.Close();
    }

// ---------------------------------------------------------
// CTelBranding::InitializeL
// ---------------------------------------------------------
//
void CTelBranding::InitializeL( MTelBubbleExtensionObserver& aBubbles )
    {
    iObserver = &aBubbles;
    }

// ---------------------------------------------------------
// CTelBranding::StartCustomizedBubble
// ---------------------------------------------------------
//
void CTelBranding::StartCustomizedBubble(
    MTelBubbleExtensionData& aCallData  )
    {
    TRAP_IGNORE( LoadBrandingL( aCallData ) );
    }

// ---------------------------------------------------------
// CTelBranding::StopCustomizedBubble
// ---------------------------------------------------------
//
void CTelBranding::StopCustomizedBubble(
    MTelBubbleExtensionData& aCallData  )
    {
    for( TInt i = 0; i < iBubbleArray.Count(); i++ )
        {
        if( iBubbleArray[i]->BubbleId() == aCallData.BubbleId() )
            {
            iBubbleArray[i]->DetechElements( *iObserver );

            delete iBubbleArray[i];
            iBubbleArray.Remove(i);
            i--;
            }
        }
    }

// ---------------------------------------------------------
// CTelBranding::BubbleUpdating
// ---------------------------------------------------------
//
void CTelBranding::BubbleUpdating()
    {
    // Do nothing
    }

// ---------------------------------------------------------
// CTelBranding::LoadBrandingL
// ---------------------------------------------------------
//
void CTelBranding::LoadBrandingL( MTelBubbleExtensionData& aCallData )
    {
    HBufC* brandid = iSettings->BrandIdLC( aCallData.ServiceId() );
    HBufC8* brandid8 = CnvUtfConverter::ConvertFromUnicodeToUtf8L( *brandid );
    CleanupStack::PopAndDestroy( brandid );
    CleanupStack::PushL( brandid8 );
    
    CTelBrandingServerHandler* bserver = 
        CTelBrandingServerHandler::NewLC( *brandid8 );
    
    CTelBrandingBubble* bubble = 
        CTelBrandingBubble::NewLC( aCallData, *bserver );
        
    iBubbleArray.AppendL( bubble );
    
    CleanupStack::Pop( bubble );
    CleanupStack::PopAndDestroy( bserver );
    CleanupStack::PopAndDestroy( brandid8 );
    
    bubble->AttachElements( *iObserver );
    }

// end of file
