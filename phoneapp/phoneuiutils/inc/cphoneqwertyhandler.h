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
* Description:  Input method related functionality.
*
*/



#ifndef CPHONEQWERTYHANDLER_H
#define CPHONEQWERTYHANDLER_H

//  INCLUDES
#include <e32base.h>
#include "mphoneqwertymodeobserver.h"
#include "mphonelangsettingobserver.h"
#include <PtiDefs.h>
#include <w32std.h>

// CONSTANTS

// FORWARD DECLARATIONS
class CPhoneLangSettingMonitor;
class CPhoneQwertyModeMonitor;

// CLASS DECLARATION

/**
*  Input method related functionality.
*
*/
class CPhoneQwertyHandler : public CBase, 
                            private MPhoneLangSettingObserver,
                            private MPhoneQwertyModeObserver
    {
    friend class T_CPhoneQwertyHandlerUT;
    
    public:

        /**
        * Two-phased constructor.
        */
        IMPORT_C static CPhoneQwertyHandler* NewL();
        
        /**
        * Destructor.
        */
        IMPORT_C virtual ~CPhoneQwertyHandler();

        /**
        * Checks if qwerty input is used.
        * @return ETrue if in qwerty mode.
        */
        IMPORT_C TBool IsQwertyInput() const;
        
        /**
        * Gets keycode according to current keyboard mapping. 
        */
        IMPORT_C TInt NumericKeyCode( const TKeyEvent& aKeyEvent );

        /**
        * Converts the key code of the event to numeric mode code and removes
        * any modifiers so that editor component will not do any more conversions.
        * Does nothing if the key doesn't map to any numeric mode characters.
        * @return ETrue if key contained any numeric mode character
        */
        IMPORT_C TBool ConvertToNumeric( TKeyEvent& aKeyEvent );

        /**
        * Add qwerty mode observer. 
        */
        IMPORT_C void AddQwertyModeObserverL( MPhoneQwertyModeObserver& aObserver );

    private:
    
        /**
        * @see MIdleLangSettingObserver.
        */
        void HandleInputLanguageSettingChange( TInt aLanguage );
        
        /**
        * @see MIdleQwertyModeObserver.
        */
        void HandleQwertyModeChange( TInt aMode );
        
        /**
        * @see MIdleQwertyModeObserver.
        */
        void HandleKeyboardLayoutChange();

        /**
        * C++ default constructor.
        */
        CPhoneQwertyHandler();
        
        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();
        
        /**
        * LoadNumericKeyBindings
        */
        void LoadNumericKeyBindings( TInt aLanguage, TInt aKeyboard );
        
        /**
         * Check if given character is a number character
         */
        TBool IsNumber( TText aChar ) const;


    private:    // Data
        
        // Input language
        TInt iInputLanguageId;
        
        // Qwerty mode
        TInt iQwertyMode;
        
        // Language setting monitor
        CPhoneLangSettingMonitor* iLangSettingMonitor;
        
        // Qwerty mode monitor
        CPhoneQwertyModeMonitor* iQwertyModeMonitor;
        
        // RArray
        RArray<TPtiNumericKeyBinding> iNumericKeys;
    };

#endif // CPHONEQWERTYHANDLER_H   
            
// End of File
