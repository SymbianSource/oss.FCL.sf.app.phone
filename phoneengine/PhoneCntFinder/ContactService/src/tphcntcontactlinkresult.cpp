/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Result of getting user selected contact link.
*
*/


#include <AiwGenericParam.h>
#include "tphcntcontactlinkresult.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
TPhCntContactLinkResult::TPhCntContactLinkResult( 
    HBufC8*& aLink ) : 
    iLink( aLink )
    {
    
    }

// ---------------------------------------------------------------------------
// From class MPhCntServiceResult
// Takes the contact link descriptor from AIW result.
// ---------------------------------------------------------------------------
//
void TPhCntContactLinkResult::ParseResult( 
    CAiwGenericParamList& aResult )
    {
    TInt index = 0;
    const TAiwGenericParam* param =
        aResult.FindFirst( index, EGenericParamContactLinkArray );
    if ( param )
        {
        delete iLink;
        iLink = NULL;
        iLink = param->Value().AsData().Alloc();
        }
    }

