/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Definition of CDialerKeyPadButton class.
*
*/


#ifndef C_CDIALERKEYPADBUTTON_H
#define C_CDIALERKEYPADBUTTON_H

#include <e32base.h>
#include <aknbutton.h>

// CLASS DECLARATION

/**
*  Contains keypad button data.
*
*  @lib dialer.lib
*  @since S60 v5.0
*/
NONSHARABLE_CLASS( CDialerKeyPadButton ) : public CBase
    {
    public:
    
        CDialerKeyPadButton();

        ~CDialerKeyPadButton();
    
    public:
        CCoeControl* Control() const { return iButton; };
        TInt ScanCode() const;
        TInt KeyCode() const;
        void CreateButtonL( const TInt aScanCode,
                            const TInt aKeyCode, 
                            const TMifDialer aButtonIcon, 
                            const TMifDialer aButtonIconMask );
        void SetIconSize( TSize& aSize );                    
        void HandleResourceChange( TInt aType );
        void MapDialerIconToSkinIcon( const TInt aDialerIcon, 
                                      TAknsItemID& aItemId ) const;  
        void EnableAudioFeedback( const TBool aEnable );

    private:
        void UpdateIconL();
        
    private: // data
    
        CAknButton*  iButton;
        
        TInt iScanCode;
        TInt iKeyCode;
        TMifDialer iButtonIcon;
        TMifDialer iButtonIconMask;
    };

#endif // C_CDIALERKEYPADBUTTON_H
