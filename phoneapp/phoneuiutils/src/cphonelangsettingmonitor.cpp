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
* Description:  Monitor for language settings.
*
*/



// INCLUDE FILES
#include <bautils.h>
#include <AknFepInternalCRKeys.h>
#include "cphonelangsettingmonitor.h"
#include "mphonelangsettingobserver.h"
#include "phonelogger.h"
#include "cphonecenrepproxy.h"
#include "mphonecenrepobserver.h"

// CONSTANTS

// FORWARD DECLARATIONS

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhoneLangSettingMonitor::CPhoneLangSettingMonitor
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPhoneLangSettingMonitor::CPhoneLangSettingMonitor()
    {
    }

// -----------------------------------------------------------------------------
// CPhoneLangSettingMonitor::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPhoneLangSettingMonitor::ConstructL()
    {
    TInt language(1); // english
    TInt err = KErrNone;
    
    if ( err == KErrNone )
        {
        iInputLanguageSetting = language;    
        }             
            
    // Start listen changes in setting and image path
    CPhoneCenRepProxy::Instance()->NotifyChangeL(
        KCRUidAknFep, 
        KAknFepInputTxtLang,
        this );       
    }

// -----------------------------------------------------------------------------
// CPhoneLangSettingMonitor::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPhoneLangSettingMonitor* CPhoneLangSettingMonitor::NewL()
    {
    CPhoneLangSettingMonitor* self = 
        new (ELeave) CPhoneLangSettingMonitor();

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }
    
// Destructor
CPhoneLangSettingMonitor::~CPhoneLangSettingMonitor()
    {
    iObserverArray.ResetAndDestroy();
    }

// -----------------------------------------------------------------------------
// CPhoneLangSettingMonitor::AddObserverL
// -----------------------------------------------------------------------------
//
void CPhoneLangSettingMonitor::AddObserverL(
    MPhoneLangSettingObserver& aObserver )
    {
    if ( iObserverArray.Find( &aObserver ) != KErrNone )
        {
        User::LeaveIfError( iObserverArray.Append( &aObserver ) );
        }
    }

// -----------------------------------------------------------------------------
// CPhoneLangSettingMonitor::RemoveObserver
// -----------------------------------------------------------------------------
//
void CPhoneLangSettingMonitor::RemoveObserver(
    MPhoneLangSettingObserver& aObserver )
    {
    TInt index;
    if ( iObserverArray.FindInAddressOrder( &aObserver, index ) == KErrNone )
        {
        iObserverArray.Remove( index );
        }
    }
// -----------------------------------------------------------------------------
// CPhoneLangSettingMonitor::HandleCenRepChangeL
// -----------------------------------------------------------------------------
//
void CPhoneLangSettingMonitor::HandleCenRepChangeL( 
    const TUid& aUid,
    const TUint aId )
    {
    __LOGMETHODSTARTEND(EPhoneControl, 
        "CPhoneLangSettingMonitor::HandleCenRepChangeL( )");
        
    if ( aUid == KCRUidAknFep && aId == KAknFepInputTxtLang )
        {
        TInt language(1); // english
        TInt err( CPhoneCenRepProxy::Instance()->GetInt(
            KCRUidAknFep,
            KAknFepInputTxtLang,
            language ));
 
        if ( err == KErrNone && iInputLanguageSetting != language )
            {           
            iInputLanguageSetting = language;
            
            // Notify change to the observers.
            for ( TInt i = 0; i < iObserverArray.Count(); i++ ) 
                {
                iObserverArray[i]->HandleInputLanguageSettingChange( 
                    iInputLanguageSetting );
                }    
            }   
        }      
    }
    
// -----------------------------------------------------------------------------
// CPhoneLangSettingMonitor::Language
// -----------------------------------------------------------------------------
//
TInt CPhoneLangSettingMonitor::InputLanguage() const
    {
    return iInputLanguageSetting;
    }



//  End of File  
