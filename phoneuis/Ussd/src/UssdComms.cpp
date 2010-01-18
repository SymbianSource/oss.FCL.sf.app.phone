/*
* Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Handles Ussd communications to the Phone Server.
*
*/



// INCLUDE FILES
#include "UssdComms.h"
#include <CPhCltUssd.h>


// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CUssdComms::CUssdComms
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CUssdComms::CUssdComms()
:   iExitReason( EPhCltExitReasonUnknown )
    {
    }

// -----------------------------------------------------------------------------
// CUssdComms::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CUssdComms* CUssdComms::NewL()
    {
    CUssdComms* self = new( ELeave ) CUssdComms();    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();    // self
    return self;
    }


// -----------------------------------------------------------------------------
// CUssdComms::ConstructL
// Symbian 2nd phase constructor can leave.
// 
// -----------------------------------------------------------------------------
//
void CUssdComms::ConstructL()
    {
    iUssdClientInt = CPhCltUssdInt::NewL();
    User::LeaveIfError( iUssdClientInt->AppStarting() );
    }


// -----------------------------------------------------------------------------
// CUssdComms::~CUssdComms
// Destructor.
// 
// -----------------------------------------------------------------------------
//
CUssdComms::~CUssdComms()
    {
    //disconnect from server. No need to check SubSessionHandles.

    // Inform the exit reason.
    // If send completed, then the USSD session is not canceled in server side,
    // to ensure reply possibility to possible network request.
    
    if( iUssdClientInt )
        {
        iUssdClientInt->AppTerminating( iExitReason ); 
        }
    delete iUssdClientInt;  
    }


// -----------------------------------------------------------------------------
// CUssdAppUi::SendString
// Sends given string to network via phone client.
//
// -----------------------------------------------------------------------------
TInt CUssdComms::SendString( const TDesC& aString )
    {
    return iUssdClientInt->SendUssd( aString );
    }

// -----------------------------------------------------------------------------
// CUssdAppUi::InformAppForeground
// Informs the server that app is on foreground.
//
// -----------------------------------------------------------------------------
TBool CUssdComms::InformAppForeground()
    {
    return iUssdClientInt->AppToForeground();
    }

// -----------------------------------------------------------------------------
// CUssdAppUi::InformAppBackground
// Informs the Server that app in on background
//
// -----------------------------------------------------------------------------
void CUssdComms::InformAppBackground()
    {
    iUssdClientInt->AppToBackground();
    }


// -----------------------------------------------------------------------------
// CUssdAppUi::InformExitReason
// 
// Inform the reason why app was terminated.
// -----------------------------------------------------------------------------
void CUssdComms::InformExitReason( TPhCltUssdAppExitReason aExitReason )
    {
    iExitReason = aExitReason;
    }


// End of File
