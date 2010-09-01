/*
* Copyright (c) 2008, 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
#include "dialercommon.h"

const TMifDialer KDialerNoIcon = EMbmDialerLastElement;

// CLASS DECLARATION

/**
*  Contains keypad button data.
*
*  @lib dialer.lib
*  @since S60 v5.0
*/
NONSHARABLE_CLASS( CDialerKeyPadButton ) : public CAknButton
    {
    public: // Enumerations

        enum TDialerKeyPadButtonEvent
            {
            /* Reported to the observer when pointer is dragged outside
             * the button's area.
             */
            EEventDraggingOutsideButton = 200
            };
        
    public:
    
        /**
         * @param   aNumberLabel            The number (etc) of the key.
         *                                  Reference must remain valid until reset with SetNumLabel
         * @parem   aPrimaryAlphaLabel      The first row of alphabets of the key.
         *                                  Reference must remain valid until reset with SetPrimaryAlphaLabel
         * @param   aSecondaryAlphaLabel    The second row of alphabets of the key.
         *                                  Reference must remain valid until reset with SetSecondaryAlphaLabel
         * @param   aScanCode               The button scanCode. 
         * @param   aKeyCode                The button keyCode.
         * @param   aButtonIconId           The icon for the normal state.
         * @param   aButtonIconMaskId       The icon mask for the normal state.
         * @param   aFlags                  CAknButton flags
         */
        static CDialerKeyPadButton* NewLC( const TDesC& aNumberLabel,
                                           const TDesC& aPrimaryAlphaLabel,
                                           const TDesC& aSecondaryAlphaLabel,
                                           TInt aScanCode, 
                                           TInt aKeyCode,
                                           TMifDialer aButtonIconId, 
                                           TMifDialer aButtonIconMaskId,
                                           TInt aFlags );  
        
        /**
         * Destructor
         */
        ~CDialerKeyPadButton();
        
    public:

        /**
         * Scan code associated with the key.
         */
        TInt ScanCode() const;
        
        /**
         * Key code associated with the key
         */
        TInt KeyCode() const;
        
        /**
         * Set layout variety to be used
         */
        void SetVariety( TInt aVariety );
        
        /**
         * Set operation mode of the keypad. Operation mode has rules,
         * which labels are shown and how they are laid out.
         */
        void SetOperationMode( TDialerOperationMode aMode );
        
        /**
         * Set number label for the key.
         * @param   aLabel  Reference to new label text. The reference must remain
         *                  valid until reset or this button is deleted.
         */
        void SetNumLabel( const TDesC& aLabel );

        /**
         * Set first row alphabet label.
         * @param   aLabel  Reference to new label text. The reference must remain
         *                  valid until reset or this button is deleted.
         */
        void SetPrimaryAlphaLabel( const TDesC& aLabel );
        
        /**
         * Set second row alphabet label for the key.
         * @param   aLabel  Reference to new label text. The reference must remain
         *                  valid until reset or this button is deleted.
         */
        void SetSecondaryAlphaLabel( const TDesC& aLabel );
        
        /**
         * Enable or disable audio feedback for this button.
         * @param   aEnable ETrue if audio feedback should be enabled.
         *                  EFalse otherwise.
         */
        void EnableAudioFeedback( const TBool aEnable );

    public: //from CAknButton
        
        /**
         * @see CAknButton
         */
        void HandleResourceChange( TInt aType );

    protected: // from CAknButton
        
        /**
         * @see CAknButton
         */
        void Draw(const TRect& aRect) const;
        
        /**
         * @see CAknButton
         */
        void SizeChanged();
        
        /**
         * @see CAknButton
         */
        void HandlePointerEventL( const TPointerEvent& aPointerEvent );
        
    private:

        void UpdateIconL();
        void DrawIconAndText( CWindowGc& aGc ) const;
        void GetTextColors( TRgb& aPenColor, TRgb& aBrushColor ) const; 
        void MapDialerIconToSkinIcon( TInt aDialerIcon, 
                                      TAknsItemID& aItemId ) const; 
        void SetIconLayout( const TRect& aRect );
        
    private: // constructors    
        
        /**
         * C++ constructor
         * 
         * @param aNumberLabel          The number (etc) of the key.
         * @param aPrimaryAlphaLabel    The first row of alphabets of the key.
         * @param aSecondaryAlphaLabel  The second row of alphabets of the key.
         * @param aScanCode             The button scanCode. 
         * @param aKeyCode              The button keyCode.
         * @param aButtonIconId         The icon for the normal state.
         * @param aButtonIconMaskId     The mask icon for the normal state.
         * @param   aFlags              CAknButton flags
         */ 
        CDialerKeyPadButton( const TDesC& aNumberLabel,
                             const TDesC& aPrimaryAlphaLabel,
                             const TDesC& aSecondaryAlphaLabel,
                             TInt aScanCode, 
                             TInt aKeyCode,
                             TMifDialer aButtonIconId, 
                             TMifDialer aButtonIconMaskId,
                             TInt aFlags );
        
        /**
         * Symbian 2nd phase constructor.
         *
         * @param aFlags Flags of the button.
         */
        void ConstructL( TInt aFlags );
        
    private: // data

        /** Scan code of the key */
        TInt iScanCode;
        
        /** Key code of the key */
        TInt iKeyCode;
        
        /** ID of the icon bitmap */
        TMifDialer iButtonIconId;
        
        /** ID of the icon mask */
        TMifDialer iButtonIconMaskId;
        
        /** Layout variety to use */
        TInt iVariety;
        
        /** Operation mode of the keypad */
        TDialerOperationMode iOperationMode;
        
        /** Layout for number */
        TAknLayoutText iNumberLayout;
        
        /** Layout for primary row of alphabets */
        TAknLayoutText iPrimaryAlphaLayout;
        
        /** Layout for secondar row of alphabets */
        TAknLayoutText iSecondaryAlphaLayout;
        
        /** Label containing the number or hash or asterisk */
        TPtrC iNumberLabel;
        
        /** Label containing first row of alphabets */
        TPtrC iPrimaryAlphaLabel;  
        
        /** Label containing second row of alphabets */
        TPtrC iSecondaryAlphaLabel;
        
        /** Rect inside the button where icon is drawn (if available) */
        TRect iIconRect;
        
        /** Own variable for counting drag events, cannot use the one in base class */
        TInt iDragEventCounter;
    };

#endif // C_CDIALERKEYPADBUTTON_H
