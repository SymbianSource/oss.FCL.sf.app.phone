/*
* Copyright (c)  Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Implementation of TPhCntContactSelectionStrategy class.
*
*/

#include <MVPbkContactLinkArray.h>
#include <CVPbkContactStoreUriArray.h>
#include <MVPbkContactLink.h>
#include <MVPbkContactStore.h>
#include <MVPbkContactStoreProperties.h>

#include "tphcntcontactselectionstrategy.h"

const TInt KFirstContactInList = 0;

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
TPhCntContactSelectionStrategy::TPhCntContactSelectionStrategy( )
    {
    iSeveralMatchesAllowed = TPhCntContactSelectionStrategy::EAllowSingleMatch;
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void TPhCntContactSelectionStrategy::SetContactSelectionStrategy( 
        TAllowSeveralMatches aSeveralMatchesAllowed )
    {
    iSeveralMatchesAllowed = aSeveralMatchesAllowed;
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
TInt TPhCntContactSelectionStrategy::ApplyStrategy( 
    const MVPbkContactLinkArray& aContactLinkArray )
    {
    iContactLinkArray = &aContactLinkArray;
        
    if ( iSeveralMatchesAllowed == TPhCntContactSelectionStrategy::EAllowSingleMatch )
        {
        return ApplySingleMatchStrategy( );
        }
    else
        {
        return ApplySeveralMatchesStrategy( );
        }    
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
TInt TPhCntContactSelectionStrategy::ApplyAdditonalStoreStrategy( 
    const MVPbkContactLinkArray& aContactLinkArray,
    const CVPbkContactStoreUriArray& aAdditionalStoreUriArray )
    {
    iContactLinkArray = &aContactLinkArray;    
    
    TInt ret( KNoContact );
    TInt contactsFoundFromAdditionalStore(0); 
    
    for ( TInt i = 0; i < aContactLinkArray.Count(); i++ )
        {
        TVPbkContactStoreUriPtr uri = GetContactStoreUri( i );
    
        if ( aAdditionalStoreUriArray.IsIncluded( uri ) )
            {
            contactsFoundFromAdditionalStore++;
            ret = i;
            }            
        }
    
    if ( contactsFoundFromAdditionalStore > 1 )
        {
        // Multiple matches from additional stores -> no match.
        ret = KManyContacts;
        }
            
    return ret;    
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
TVPbkContactStoreUriPtr TPhCntContactSelectionStrategy::GetContactStoreUri( 
        TInt aContactLinkArrayIndex )
    {
    return iContactLinkArray->At( aContactLinkArrayIndex ).ContactStore().StoreProperties().Uri();
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
TInt TPhCntContactSelectionStrategy::ApplySingleMatchStrategy( )
    {
    if ( iContactLinkArray->Count() == 1 )
        {        
        return KFirstContactInList;
        }
    else
        {
        return KNoContact;
        }
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
TInt TPhCntContactSelectionStrategy::ApplySeveralMatchesStrategy( )
    {
    if ( iContactLinkArray->Count() >= 1 )
        {
        return KFirstContactInList;
        }
    else
        {
        return KNoContact;
        }
    }
