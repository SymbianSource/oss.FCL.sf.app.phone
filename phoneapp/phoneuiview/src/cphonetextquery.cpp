/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Implementation of CPhoneTextQuery class.
*
*/


// INCLUDE FILES
#include "cphonetextquery.h"
#include "phoneui.hrh"
#include "phonelogger.h"
#include "phoneconstants.h"

// ================= MEMBER FUNCTIONS =======================
// C++ default constructor can NOT contain any code, that
// might leave.
//
CPhoneTextQuery::CPhoneTextQuery( 
    MEikCommandObserver& aCommandObserver, 
    TDes& aDataText,
    TInt aDefaultCbaResourceId,
    TInt aContentCbaResourceId,
    TBool aSendKeyEnabled,
    TInt aEikBidOkCmd ) :
    CAknTextQueryDialog( aDataText, CAknQueryDialog::ENoTone ),
    iCommandObserver( aCommandObserver ),
    iDefaultCbaResourceId( aDefaultCbaResourceId ),
    iContentCbaResourceId( aContentCbaResourceId ),
    iSendKeyHandlingEnabled( aSendKeyEnabled ),
    iEikBidOkCmd( aEikBidOkCmd )
    {
    }

// ---------------------------------------------------------
// Destructor
// ---------------------------------------------------------
// 
CPhoneTextQuery::~CPhoneTextQuery()
    {
    }

// ---------------------------------------------------------
// CPhoneTextQuery::OfferKeyEventL
// ---------------------------------------------------------
//
TKeyResponse CPhoneTextQuery::OfferKeyEventL( 
    const TKeyEvent& aKeyEvent,
    TEventCode aType )
    {
    TKeyResponse response ( EKeyWasConsumed );
    // Send or Ok key down event
    if ( aKeyEvent.iScanCode == EStdKeyYes && aType == EEventKeyDown )  
        {
        // Send key enabled
        if ( iSendKeyHandlingEnabled )
             {    
             OkToExitL( EPhoneInCallCmdNewCallCall );
             }
        }
    else if ( aType == EEventKey && 
            ( aKeyEvent.iScanCode == EStdKeyNo || aKeyEvent.iCode == EKeyNo ) )
		{
		OkToExitL( EPhoneCmdEnd );
		}
    else if ( aKeyEvent.iCode == EKeyEnter && aType == EEventKey )
        {
        TInt commandID = FetchCommandFromCba();
        if ( commandID )
            {
            TryExitL( commandID );
            }
        } 
	else
		{
        UpdateSoftkeysL();		
	    response = CAknTextQueryDialog::OfferKeyEventL( aKeyEvent, aType );		
		}
    return response;
    }

// ---------------------------------------------------------
// CPhoneTextQuery::OkToExitL
// ---------------------------------------------------------
//
TBool CPhoneTextQuery::OkToExitL( TInt aCommand )
    {
    // Let the command observer process the command
    if ( aCommand == EEikBidOk )
        {
        iCommandObserver.ProcessCommandL( iEikBidOkCmd );
        }
    else
        {
        iCommandObserver.ProcessCommandL( aCommand );
        }        

    return ETrue;
    }

// ---------------------------------------------------------
// CPhoneTextQuery::PostLayoutDynInitL
// ---------------------------------------------------------
//
void CPhoneTextQuery::PostLayoutDynInitL()
    {
    CAknQueryControl* control =
        static_cast< CAknQueryControl* >( Control( EGeneralQuery ) );    
    
    control->SetTextEntryLength( KPhoneNumberEntryBufferSize );
    CAknTextQueryDialog::PostLayoutDynInitL();
    UpdateSoftkeysL();
    }

// ---------------------------------------------------------
// CPhoneTextQuery::UpdateSoftkeysL
// ---------------------------------------------------------
//
void CPhoneTextQuery::UpdateSoftkeysL()
    {
    CAknQueryControl* control =
        static_cast< CAknQueryControl* >( Control( EGeneralQuery ) );
        
    CEikButtonGroupContainer& buttonGroup = ButtonGroupContainer();

    if ( control->GetTextLength() )
        {
        // There is text in cba; we can update softkeys to ok - cancel
        buttonGroup.SetCommandSetL( iContentCbaResourceId );
        MakeLeftSoftkeyVisible( ETrue );
        }
    else 
        {
        // There is no text in cba; update softkeys to find - cancel
        buttonGroup.SetCommandSetL(  iDefaultCbaResourceId );
        MakeLeftSoftkeyVisible( ETrue );
        }

    buttonGroup.DrawDeferred();    
    }
    
// ---------------------------------------------------------
// CPhoneTextQuery::GetContent
// ---------------------------------------------------------
//
void CPhoneTextQuery::GetContent( TDes& aText )
    {
    CAknQueryControl* control =
        static_cast< CAknQueryControl* >( Control( EGeneralQuery ) );

    control->GetText( aText );        
    }

// ---------------------------------------------------------
// CPhoneTextQuery::FetchCommandFromCba
// ---------------------------------------------------------
//
TInt CPhoneTextQuery::FetchCommandFromCba()
    {
    TInt commandID = 0;
    CEikCba* cba = NULL;
    cba = MopGetObject( cba );

    if ( cba )
        {
        commandID = cba->CommandId( 0 );
        }
    
    return commandID;
    }

// End of File
