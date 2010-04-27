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
static const TUint32 KNumKeyModifiers( EModifierLeftShift | EModifierRightShift | EModifierShift |
                                       EModifierLeftFunc | EModifierRightFunc | EModifierFunc );


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
    TInt keyCode( EKeyNull );
    
    // It's possible that there are several numeric mode characters in same
    // QWERTY key (for example, '2' and 'w' may be on same button) and there must 
    // be a way to enter each of these. 
    // Select numeric mode key code for the key event with the following logic:
    // 1. If key contains exactly one numeric mode character, return that
    //    regardless of the current modifiers.
    // 2a. If key has two numeric mode characters, then actual numbers are preferred.
    //     Pressing such key without modifiers will produce the number character.
    // 2b. Pressing key with two numeric mode characters together with any modifier
    //     (Fn, Shift, Chr) will produce the secondary numeric mode character.
    // 3. More than two numeric mode characters on one key are not supported.
    //    Such cases shouldn't ever occur, but if they will, then this algorithm
    //    must be changed.
    
    // Check modifier state
    TBool modifierActive = ( aKeyEvent.iModifiers & KNumKeyModifiers );
    
    TInt numBindIdx = iNumericKeys.Count();
    
    while ( numBindIdx-- )
        {
        const TPtiNumericKeyBinding& numKeyBind = iNumericKeys[numBindIdx];
        
        if ( numKeyBind.iKey == aKeyEvent.iScanCode )
            {
            if ( !keyCode )
                {
                // first match for this key
                keyCode = numKeyBind.iChar;
                }
            else
                {
                // Second numeric mode character for this key. Override
                // previous code if it was the number character but some
                // modifier is active or it was not number character and no
                // modifiers are active.
                if ( ( IsNumber(keyCode) && modifierActive ) ||
                     ( !IsNumber(keyCode) && !modifierActive ) )
                    {
                    keyCode = numKeyBind.iChar;
                    }
                }
            }
        }
        
    return keyCode;
    }

// -----------------------------------------------------------------------------
// CPhoneQwertyHandler::ConvertToNumeric
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CPhoneQwertyHandler::ConvertToNumeric( TKeyEvent& aKeyEvent )
    {
    TBool ret( EFalse );
    TInt numericCode = NumericKeyCode( aKeyEvent );
    if ( numericCode )
        {
        aKeyEvent.iCode = numericCode;
        aKeyEvent.iModifiers &= ~KNumKeyModifiers;
        ret = ETrue;
        }
    return ret;
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

// -----------------------------------------------------------------------------
// CPhoneQwertyHandler::IsNumber
// -----------------------------------------------------------------------------
//
TBool CPhoneQwertyHandler::IsNumber( TText aChar ) const
    {
    return TChar( aChar ).IsDigit();
    }


//  End of File  
