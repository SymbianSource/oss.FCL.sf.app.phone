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
* Description:  Qwerty Mode Monitor.
*
*/


// INCLUDE FILES
#include <bautils.h>
#include <AvkonInternalCRKeys.h>
#include <e32property.h>
#include <PSVariables.h>
#include <bldvariant.hrh>

#include "cphoneqwertymodemonitor.h"
#include "mphoneqwertymodeobserver.h"
#include "phonelogger.h"
#include "cphonepubsubproxy.h"


// ============================ MEMBER FUNCTIONS ===============================

// C++ constructor can NOT contain any code, that
// might leave.
//
CPhoneQwertyModeMonitor::CPhoneQwertyModeMonitor()
    {
    }

// ---------------------------------------------------------
// CPhoneQwertyModeMonitor::ConstructL
// ---------------------------------------------------------
//
void CPhoneQwertyModeMonitor::ConstructL()
    {
    // Current Call state.
    iQwertyMode = GetQwertyMode();

// <-- QT PHONE START-->
/*    CPhonePubSubProxy::Instance()->NotifyChangeL(
        KCRUidAvkon, 
        KAknQwertyInputModeActive,
        this );*/
     

     iKeyboard = GetKeyboard();
    
#ifdef RD_INTELLIGENT_TEXT_INPUT
/*    CPhonePubSubProxy::Instance()->NotifyChangeL(
        KCRUidAvkon, 
        KAknKeyBoardLayout,
        this );*/
#endif
// <-- QT PHONE END-->
    }

// -----------------------------------------------------------------------------
// CPhoneQwertyModeMonitor::NewL
// -----------------------------------------------------------------------------
//
CPhoneQwertyModeMonitor* CPhoneQwertyModeMonitor::NewL()
    {
    CPhoneQwertyModeMonitor* self = 
        new (ELeave) CPhoneQwertyModeMonitor();
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    
    return self;
    }

// Destructor
CPhoneQwertyModeMonitor::~CPhoneQwertyModeMonitor()
    {
    iObserverArray.Reset();
    }

// -----------------------------------------------------------------------------
// CPhoneQwertyModeMonitor::AddObserverL
// -----------------------------------------------------------------------------
//
void CPhoneQwertyModeMonitor::AddObserverL(
    MPhoneQwertyModeObserver& aObserver )
    {
    if ( iObserverArray.Find( &aObserver ) != KErrNone )
        {
        User::LeaveIfError( iObserverArray.Append( &aObserver ) );
        }
    }

// -----------------------------------------------------------------------------
// CPhoneQwertyModeMonitor::RemoveObserver
// -----------------------------------------------------------------------------
//
void CPhoneQwertyModeMonitor::RemoveObserver(
    MPhoneQwertyModeObserver& aObserver )
    {
    TInt index;
    if ( iObserverArray.FindInAddressOrder( &aObserver, index ) == KErrNone )
        {
        iObserverArray.Remove( index );
        }
    }
    
// -----------------------------------------------------------
// CPhoneQwertyModeMonitor::HandlePropertyChangedL
// -----------------------------------------------------------
//
void CPhoneQwertyModeMonitor::HandlePropertyChangedL( 
        const TUid& aCategory,
        const TUint aKey,
        const TInt aValue )
    {
    __LOGMETHODSTARTEND(EPhoneControl, "CPhoneQwertyModeMonitor::HandlePropertyChangedL( ) ");
    if ( aCategory == KCRUidAvkon )
        {
        if ( aKey == KAknQwertyInputModeActive )
            {
            iQwertyMode = aValue;

            for ( TInt i = 0; i < iObserverArray.Count(); i++ ) 
                {
                iObserverArray[ i ]->HandleQwertyModeChange( 
                    iQwertyMode );
                }           
            }
#ifdef RD_INTELLIGENT_TEXT_INPUT        
        else if ( aKey == KAknKeyBoardLayout )
            {
            for ( TInt i = 0; i < iObserverArray.Count(); i++ ) 
                {
                iKeyboard = aValue;
                iObserverArray[ i ]->HandleKeyboardLayoutChange();
                }
            }
#endif    
        }
    }

// -----------------------------------------------------------------------------
// CPhoneQwertyModeMonitor::QwertyMode
// -----------------------------------------------------------------------------
//
TInt CPhoneQwertyModeMonitor::QwertyMode() const
    {
    return iQwertyMode;
    }
  
// -----------------------------------------------------------------------------
// CPhoneQwertyModeMonitor::Keyboard
// -----------------------------------------------------------------------------
//
TInt CPhoneQwertyModeMonitor::Keyboard() const
    {
    return iKeyboard;
    }
    
// -----------------------------------------------------------------------------
// CPhoneQwertyModeMonitor::GetQwertyMode
// -----------------------------------------------------------------------------
//
TInt CPhoneQwertyModeMonitor::GetQwertyMode() const
    {
    // Get call state.
    /*TInt qwertyMode( CPhonePubSubProxy::Instance()->Value(
        KCRUidAvkon,
        KAknQwertyInputModeActive ) );*/
    TInt qwertyMode(0);
    
    return qwertyMode;
    }

// -----------------------------------------------------------------------------
// CPhoneQwertyModeMonitor::GetQwertyMode
// -----------------------------------------------------------------------------
//
TInt CPhoneQwertyModeMonitor::GetKeyboard() const
    {
    // Get call state.
// <-- QT PHONE START-->    
/*    TInt keyboard( CPhonePubSubProxy::Instance()->Value(
        KCRUidAvkon,
        KAknKeyBoardLayout ) );*/
    TInt keyboard(0);
// <-- QT PHONE END-->    
    
    return keyboard;
    }

//  End of File

