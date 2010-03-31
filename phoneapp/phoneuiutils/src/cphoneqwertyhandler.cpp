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
* Description: Implementation of CPhoneQwertyHandler class.
*
*/


// INCLUDE FILES
#include    "cphoneqwertyhandler.h"
#include    "cphonelangsettingmonitor.h"
#include    "cphoneqwertymodemonitor.h"
#include    <PtiEngine.h>
#include    <w32std.h>

// CONSTANTS

// FORWARD DECLARATIONS

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhoneQwertyHandler::CPhoneQwertyHandler
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPhoneQwertyHandler::CPhoneQwertyHandler()
    {
    }

// -----------------------------------------------------------------------------
// CPhoneQwertyHandler::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPhoneQwertyHandler::ConstructL()
    {
    // Language setting monitor
    iLangSettingMonitor = CPhoneLangSettingMonitor::NewL();
    iLangSettingMonitor->AddObserverL( *this );
    
    // Qwerty mode
    iQwertyModeMonitor = CPhoneQwertyModeMonitor::NewL();
    iQwertyModeMonitor->AddObserverL( *this );
    
    // Read current values
    iInputLanguageId = iLangSettingMonitor->InputLanguage();
    iQwertyMode = iQwertyModeMonitor->QwertyMode();
    
    if ( iQwertyMode )
        {
        LoadNumericKeyBindings( iInputLanguageId, iQwertyModeMonitor->Keyboard() );            
        }
    }

// -----------------------------------------------------------------------------
// CPhoneQwertyHandler::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CPhoneQwertyHandler* CPhoneQwertyHandler::NewL()
    {
    CPhoneQwertyHandler* self = 
        new (ELeave) CPhoneQwertyHandler();

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }
    
// Destructor
EXPORT_C CPhoneQwertyHandler::~CPhoneQwertyHandler()
    {
	iLangSettingMonitor->RemoveObserver( *this );
    delete iLangSettingMonitor;
    iLangSettingMonitor = NULL;
    iQwertyModeMonitor->RemoveObserver( *this );
    delete iQwertyModeMonitor;
    iQwertyModeMonitor = NULL;
    iNumericKeys.Close();
    }
    
// -----------------------------------------------------------------------------
// CPhoneQwertyHandler::IsQwertyInput
// -----------------------------------------------------------------------------
//    
EXPORT_C TBool CPhoneQwertyHandler::IsQwertyInput() const
    {
    return iQwertyMode > 0 ? ETrue : EFalse;    
    }

// -----------------------------------------------------------------------------
// CPhoneQwertyHandler::HandleLanguageSettingChange
// -----------------------------------------------------------------------------
//    
void CPhoneQwertyHandler::HandleInputLanguageSettingChange( TInt aLanguage )
    {
    iInputLanguageId = aLanguage;
    if ( iQwertyMode )
        {
        LoadNumericKeyBindings( iInputLanguageId, iQwertyModeMonitor->Keyboard() );
        }
    }

// -----------------------------------------------------------------------------
// CPhoneQwertyHandler::HandleQwertyModeChange
// Loads keybinding with keyboard EPtiKeyboardNone if no keyboeard has been   
// defined
// -----------------------------------------------------------------------------
//    
void CPhoneQwertyHandler::HandleQwertyModeChange( TInt aMode )
    {
    iQwertyMode = aMode;
#ifndef RD_INTELLIGENT_TEXT_INPUT    
    if ( iQwertyMode && !iNumericKeys.Count() )
        {
        LoadNumericKeyBindings( iInputLanguageId, iQwertyModeMonitor->Keyboard() );            
        }
#endif    
    }

// -----------------------------------------------------------------------------
// CPhoneQwertyHandler::HandleKeyboardLayoutChange
// This event comes after HandleQwertyModeChange
// -----------------------------------------------------------------------------
//
void CPhoneQwertyHandler::HandleKeyboardLayoutChange()
    {
    
    LoadNumericKeyBindings( iInputLanguageId, iQwertyModeMonitor->Keyboard() );            
   
    }

// -----------------------------------------------------------------------------
// CPhoneQwertyHandler::LoadNumericKeyBindings
// -----------------------------------------------------------------------------
//    
void CPhoneQwertyHandler::LoadNumericKeyBindings( TInt aLanguage, TInt aKeyboard )
    {
    iNumericKeys.Reset();
    
#ifdef RD_INTELLIGENT_TEXT_INPUT    
    TPtiKeyboardType keyboard = static_cast<TPtiKeyboardType>( aKeyboard );
    TRAPD( err, 
        {
        CPtiEngine* ptiEngine = CPtiEngine::NewL();
        CleanupStack::PushL( ptiEngine );
        
        ptiEngine->GetNumericModeKeysForQwertyL( aLanguage, 
                                                 iNumericKeys,
                                                 keyboard );         
        CleanupStack::PopAndDestroy( ptiEngine );                                                 
        } ); // TRAP
#else
    TRAPD( err, 
        {
        CPtiEngine* ptiEngine = CPtiEngine::NewL();
        CleanupStack::PushL( ptiEngine );
        ptiEngine->GetNumericModeKeysForQwertyL( aLanguage, 
                                                 iNumericKeys );         
        CleanupStack::PopAndDestroy( ptiEngine );                                                 
        } ); // TRAP
#endif    
        
    if ( err )        
        {
        iNumericKeys.Reset();
        iQwertyMode = 0; // To default mode
        }
    else
        {
        // remove keys that are not remapped
        TInt numericKeysCount = iNumericKeys.Count();
        while ( numericKeysCount-- )
            {
            TPtiNumericKeyBinding numKeyBind = iNumericKeys[numericKeysCount];
            
            // This is PTI bug? Should not be in numeric keys list.
            if ( numKeyBind.iKey == EPtiKeyQwertySpace ) 
                 {
                 iNumericKeys.Remove( numericKeysCount );                    
                 }
            }    
        }
    }

// -----------------------------------------------------------------------------
// CPhoneQwertyHandler::NumericKeyCode
// -----------------------------------------------------------------------------
//    
EXPORT_C TInt CPhoneQwertyHandler::NumericKeyCode( const TKeyEvent& aKeyEvent )
    {
  
    // Check shift state
    TBool shiftActive(EFalse);
    shiftActive = aKeyEvent.iModifiers & EModifierLeftShift ||
                  aKeyEvent.iModifiers & EModifierRightShift; //||
    
    TInt numericKeysCount = iNumericKeys.Count();
    
    while ( numericKeysCount-- )
        {
        TPtiNumericKeyBinding numKeyBind = iNumericKeys[numericKeysCount];
            
        TBool shiftRequired = ( numKeyBind.iCase ==EPtiCaseUpper ) ||
                              ( numKeyBind.iCase ==EPtiCaseChrUpper );
            
        if ( numKeyBind.iKey == aKeyEvent.iScanCode &&
             (shiftRequired == shiftActive  )   )
            {
            return numKeyBind.iChar;
            }
        }
        
    return EKeyNull;         
    }           

// -----------------------------------------------------------------------------
// CPhoneQwertyHandler::AddQwertyModeObserverL
// -----------------------------------------------------------------------------
//
EXPORT_C void CPhoneQwertyHandler::AddQwertyModeObserverL( 
        MPhoneQwertyModeObserver& aObserver )
    {
    iQwertyModeMonitor->AddObserverL( aObserver );
    
    // Observer need to be notified with a current qwerty status so that
    // status dependent objects would get correct initial value.
    aObserver.HandleQwertyModeChange( iQwertyMode );
    }

//  End of File  
