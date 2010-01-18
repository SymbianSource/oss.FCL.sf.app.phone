/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Publish & Subscribe property change observer implementation
*
*/



// INCLUDE FILES
#include "VmDetDlgPropertyObserver.h"
#include "VoiceMailboxAppPrivateCRKeys.h"
#include "VmDetailsDialog.h"

#include <e32property.h>


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CVmDetDlgPropertyObserver::CVmDetDlgPropertyObserver
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CVmDetDlgPropertyObserver::CVmDetDlgPropertyObserver( 
                                CVmDetailsDialog& aDetDialog )
    : CActive( CActive::EPriorityLow ),
      iDetDialog( aDetDialog )
    {
    }

// -----------------------------------------------------------------------------
// CVmDetDlgPropertyObserver::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CVmDetDlgPropertyObserver::ConstructL()
    {
    CActiveScheduler::Add( this );
    }

// -----------------------------------------------------------------------------
// CMGXPropertyObserver::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CVmDetDlgPropertyObserver* CVmDetDlgPropertyObserver::NewL( 
                                CVmDetailsDialog& aDetDialog )
    {
    CVmDetDlgPropertyObserver* self = 
                         new( ELeave ) CVmDetDlgPropertyObserver( aDetDialog );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// ----------------------------------------------------------------------------
// CVmDetDlgPropertyObserver::~CVmDetDlgPropertyObserver
// Destructor.
// ----------------------------------------------------------------------------
//
CVmDetDlgPropertyObserver::~CVmDetDlgPropertyObserver()
    {
    if ( IsActive() )
        {
        Cancel();
        }
    }

// -----------------------------------------------------------------------------
// CVmDetDlgPropertyObserver::ObservePropertyChangeL
// -----------------------------------------------------------------------------
//
void CVmDetDlgPropertyObserver::ObservePropertyChangeL( TUid aCategory, 
                                                                    TUint aKey )
    {
    if ( IsActive() )
        {
        Cancel();
        }
    User::LeaveIfError( iProperty.Attach( aCategory, aKey ) );
    iProperty.Subscribe( iStatus );
    SetActive();
    iKey = aKey;
    }

// -----------------------------------------------------------------------------
// CVmDetDlgPropertyObserver::RunL
// -----------------------------------------------------------------------------
//
void CVmDetDlgPropertyObserver::RunL()
    {
    TFileName fileName( KNullDesC );

    if ( iKey == KVmSummaryInfoIsSet )
        {
        User::LeaveIfError( iProperty.Get( fileName ) );
        }

    iDetDialog.HandlePropertyNotificationEventL( fileName );
    }

// -----------------------------------------------------------------------------
// CVmDetDlgPropertyObserver::DoCancel
// -----------------------------------------------------------------------------
//
void CVmDetDlgPropertyObserver::DoCancel()
    {
    iProperty.Cancel();
    }

// -----------------------------------------------------------------------------
// CVmDetDlgPropertyObserver::RunError
// -----------------------------------------------------------------------------
//
TInt CVmDetDlgPropertyObserver::RunError( TInt /*aError*/ )
    {
    // Return no error to prevent ui note from being displayed,
    // nothing else can be done.
    return KErrNone;
    }

//  End of File
