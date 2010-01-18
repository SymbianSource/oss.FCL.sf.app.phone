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
* Description:  ListQueryDialog
*
*/


// INCLUDES

#include "cphonelistquerydialog.h"
#include "mphonequeryobserver.h"
#include "phoneui.hrh"


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CPhoneListQueryDialog::CPhoneListQueryDialog(
    MEikCommandObserver& aCommandObserver,
    MPhoneQueryObserver& aQueryObserver, 
    const RArray<TInt>& aCommands ) :
    CAknListQueryDialog( &iIndex ), 
    iCommandObserver( aCommandObserver ),
    iQueryObserver ( aQueryObserver ) 
    {
    for ( TInt i( 0 ); i < aCommands.Count(); i++ )
        {
        iCommands.Append( aCommands[i] );
        }
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CPhoneListQueryDialog::~CPhoneListQueryDialog()
    {
    iCommands.Close(); 
    iQueryObserver.SetQueryDeletedFlag( ETrue );
    }
    
// ---------------------------------------------------------------------------
// CPhoneListQueryDialog::OfferKeyEventL
// ---------------------------------------------------------------------------
//
TKeyResponse CPhoneListQueryDialog::OfferKeyEventL( const TKeyEvent& aKeyEvent,
                                                          TEventCode aType )
    {
    TKeyResponse ret = EKeyWasNotConsumed;
    if ( aType == EEventKey &&
        ( aKeyEvent.iScanCode == EStdKeyNo || aKeyEvent.iCode == EKeyNo ) )
        {
        OkToExitL( EPhoneCmdWaitNoteOk );
        }       
    else if ( aKeyEvent.iScanCode == EStdKeyYes )
        {
        ret =  EKeyWasConsumed;        
        }    
    else 
        {
        ret = CAknListQueryDialog::OfferKeyEventL( aKeyEvent, aType );              
        }
    
    return ret;
  
    }    
// ---------------------------------------------------------------------------
// CPhoneListQueryDialog::PostLayoutDynInitL
// 
// 
// ---------------------------------------------------------------------------
//
void CPhoneListQueryDialog::PostLayoutDynInitL()
    {
    CAknListQueryDialog::PostLayoutDynInitL();
    }

// ---------------------------------------------------------------------------
// CPhoneListQueryDialog::OkToExitL
// ---------------------------------------------------------------------------
//
TBool CPhoneListQueryDialog::OkToExitL( TInt aKeyCode )
    {    
    if ( aKeyCode == EEikBidOk )
        { 
        if( !ListBox() )
            {
            User::Leave( KErrNotFound );
            }
        
        TInt itemIndex = ListBox()->CurrentItemIndex();
        
        if( itemIndex >= iCommands.Count() || itemIndex < 0 )
            {
            User::Leave( KErrNotFound );
            }
        
        iQueryObserver.ReportQueryCompleteAsync( iCommands [itemIndex] );        
        }    
    else if ( aKeyCode == EPhoneCmdWaitNoteOk )  
        {
        iQueryObserver.ReportQueryCompleteAsync( EPhoneCmdWaitNoteOk ); 
        }
    
    return ETrue;
    }
    
//  End of File  
