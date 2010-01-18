/*
* Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  NumberListQuery for DTMF strings.
*
*/



#include    "cphonedtmfnumberlistquery.h"
#include    "mphonequeryobserver.h"
#include    "phoneui.hrh"
#include    "phonelogger.h"
#include    <mphcntmatch.h>
#include    <StringLoader.h>
#include    <phoneui.rsg>


const TInt KPhoneManualDTMFString = R_QTN_ENTER_MANUALLY_QUERY_ITEM;
const TInt KPhoneFindDTMFString = R_QTN_FIND_QUERY_ITEM;

// ======== MEMBER FUNCTIONS ========


// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CPhoneDtmfNumberListQuery::CPhoneDtmfNumberListQuery(
    MEikCommandObserver& aCommandObserver,
    MPhoneQueryObserver& aQueryObserver,
    HBufC*& aSelectedDtmfNumber ):
      CAknListQueryDialog( &iIndex ),
      iCommandObserver( aCommandObserver ),
      iQueryObserver ( aQueryObserver ),
      iSelectedDtmfNumber ( aSelectedDtmfNumber ),
      iArray( 2 )
    {
    }
// ---------------------------------------------------------------------------
// CPhoneDtmfNumberListQuery::NewL
// ---------------------------------------------------------------------------
//
CPhoneDtmfNumberListQuery* CPhoneDtmfNumberListQuery::NewL( MEikCommandObserver& aCommandObserver,
                                                           MPhoneQueryObserver& aQueryObserver,
                                                           HBufC*& aSelectedDtmfNumber  )
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneDtmfNumberListQuery::NewL()");
    CPhoneDtmfNumberListQuery* self = 
                new ( ELeave ) CPhoneDtmfNumberListQuery( aCommandObserver, 
                                                          aQueryObserver,
                                                          aSelectedDtmfNumber );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }
    
// ---------------------------------------------------------------------------
// ConstructL
// ---------------------------------------------------------------------------
//
void CPhoneDtmfNumberListQuery::ConstructL()
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneDtmfNumberListQuery::ConstructL()");
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CPhoneDtmfNumberListQuery::~CPhoneDtmfNumberListQuery()
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneDtmfNumberListQuery::~CPhoneDtmfNumberListQuery()");
    iArray.Reset();
    iQueryObserver.SetQueryDeletedFlag( ETrue );
    }

// ---------------------------------------------------------------------------
// CPhoneDtmfNumberListQuery::PostLayoutDynInitL
// ---------------------------------------------------------------------------
//
void CPhoneDtmfNumberListQuery::PostLayoutDynInitL()
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneDtmfNumberListQuery::PostLayoutDynInitL()");
    HBufC* manual = StringLoader::LoadLC( KPhoneManualDTMFString );
    HBufC* find = StringLoader::LoadLC( KPhoneFindDTMFString );
    iArray.AppendL( *find );
    iArray.AppendL( *manual );
    SetItemTextArray( &iArray );
    SetOwnershipType( ELbmDoesNotOwnItemArray ); 
    CleanupStack::PopAndDestroy( 2, manual ); //find, manual  
    Layout();
    DrawDeferred(); 
    }
    
// ---------------------------------------------------------------------------
// CPhoneDtmfNumberListQuery::OfferKeyEventL
// ---------------------------------------------------------------------------
//
TKeyResponse CPhoneDtmfNumberListQuery::OfferKeyEventL( 
    const TKeyEvent& aKeyEvent,
    TEventCode aType )
    {
    if ( aType == EEventKey &&
        ( aKeyEvent.iScanCode == EStdKeyNo || aKeyEvent.iCode == EKeyNo ) )
        {
        OkToExitL( EPhoneCmdEnd );
        }
    else if ( aKeyEvent.iScanCode == EStdKeyYes )
        {
        return EKeyWasConsumed;        
        }        
                
    return CAknListQueryDialog::OfferKeyEventL( aKeyEvent, aType );     
    } 
        
// ---------------------------------------------------------------------------
// CPhoneDtmfNumberListQuery::AppendStringToArrayL
// ---------------------------------------------------------------------------
//    
void CPhoneDtmfNumberListQuery::AppendStringToArrayL( TDesC& aString )
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneDtmfNumberListQuery::AppendStringToArrayL()");
    iArray.AppendL( aString );
    }

// ---------------------------------------------------------------------------
// CPhoneDtmfNumberListQuery::OkToExitL
//
// If softkey is select, result buffer can be filled with DTMF string.
// Selection can be 'Find' then single item fetch is started.
// 
// Otherwise forward to base class. 
//
// ---------------------------------------------------------------------------
//

TBool CPhoneDtmfNumberListQuery::OkToExitL( 
        TInt aButtonId )
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneDtmfNumberListQuery::OkToExitL()");
    TInt index = 0;
    TInt arraycount = 0;
    TBool retVal = ETrue;
    if ( aButtonId == EAknSoftkeySelect || aButtonId == EEikBidOk )
        {
        
        index = ListBox()->CurrentItemIndex();
        arraycount = ListBox()->Model()->
                    MatchableTextArray()->MdcaCount();
        
        // 'Enter manually' selected.  
        //  Last in the list.         
        if ( index == ( arraycount-1 ) )
            {
            // Forward pressed CBA to the framework so that we can handle it
            // in our active state
            iQueryObserver.ReportQueryCompleteAsync( EPhoneInCallCmdDtmfManualQuery ); 
            }

        // Fill result buffer with DTMF string.
        else if ( index <= ( arraycount-3 ) )     
            {
            TPtrC ptr = ListBox()->Model()->
                    MatchableTextArray()->MdcaPoint( index );
                    
            // Caller is responsible to delete object
            iSelectedDtmfNumber = HBufC::NewL( ptr.Length() );
            iSelectedDtmfNumber->Des().Append( ptr );           
            //Let the command observer handle this case.
            iQueryObserver.ReportQueryCompleteAsync( EPhoneCmdDtmfOk );           
            }
           
        //  Make single item fetch if 'Find' selected.  
        // 'Find' item is second last.
        else if ( index == ( arraycount-2 ) )
            {
             // Forward pressed CBA to the framework so that we can handle it
            // in our active state
            iQueryObserver.ReportQueryCompleteAsync( EPhoneInCallCmdDtmfListViewSearch );
            }
        }
        
    else if ( aButtonId == EPhoneCmdEnd )  
        {
        iQueryObserver.ReportQueryCompleteAsync( EPhoneCmdEnd ); 
        }        
    
    return retVal;
    }
    
//  End of File  
