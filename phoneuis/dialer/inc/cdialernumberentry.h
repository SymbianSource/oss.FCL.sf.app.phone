/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CDialerNumberEntry class handles viewing
*                and positioning of Number Entry and offers API
*                to access NE contents.
*
*/



#ifndef CDIALERNUMBERENTRY_H
#define CDIALERNUMBERENTRY_H

//  INCLUDES
#include <coecntrl.h>
#include <coemain.h>
#include <AknPhoneNumberEditor.h>

#include "cdialercontainerbase.h"
#include "dialer.hrh"

// CONSTANTS

// FORWARD DECLARATIONS
class CAknPhoneNumberEditor;
class CAknButton;
class CAknsFrameBackgroundControlContext;
class MNumberEntryObserver;
class CEikLabel;
class CDialingExtensionInterface;

// Number entry varietys
enum TDialerNEVariety
    {
    KDialerNELayoutLargeFont,
    KDialerNELayoutSmallFont
    };

// CLASS DECLARATION

/**
*  CDialerNumberEntry control class
*
*  @lib dialer.lib
*  @since S60 v5.0
*/
NONSHARABLE_CLASS(CDialerNumberEntry) : 
    public CDialerContainerBase
    {
    public:  // Constructors and destructor
        
        /**
        * Two phase constructor
        * @param aContainer Parent container
        * @return New instance of CDialerNumberEntry
        */
        static CDialerNumberEntry* NewL( 
            const CCoeControl& aContainer );
        
        /**
        * Destructor.
        */
        virtual ~CDialerNumberEntry();

    public: // New functions

        /**
        * Return text length in number entry
        * @return text length
        */
        TInt TextLength();

        /**
        * Return number entry as CCoeControl
        * @return CCoeControl pointer of CAknPhoneNumberEditor
        */
        CCoeControl* GetNumberEntry( ) const;

        /**
        * Create and show number entry
        */
        void CreateNumberEntry();

        /**
        * Toggles editor mode from number->text->number->..
        * @param aDefaultMode if EFalse, set mode to number,
        *        else toggle mode.
        */
        TInt ChangeEditorMode( TBool aDefaultMode );

        /**
        * Return current editor mode
        * @return editor mode.
        */
        TInt GetEditorMode() const;

        /**
        * Set text to number entry
        * @param aDesc Text to set.
        */
        void SetTextToNumberEntry( const TDesC& aDesC );

        /**
        * Returns TPtrC pointed to editor text.
        * @return   Pointer to editor text.
        */
        TPtrC Text() const;
        
        /**
        * Get tect from number entry.
        * @param aDes Text is returned here.
        */
        void GetTextFromNumberEntry( TDes& aDes );

        /**
        * Reset editor to default values.
        */
        void ResetEditorToDefaultValues();

        /**
        * Set number entry's prompt text.
        * @param aPromptText.
        */        
        void SetNumberEntryPromptTextL( const TDesC& aPromptText );
        

        /**
        * Handles command targeted to this container.
        * @param aCommand Command.
        */
        TBool HandleCommandL( TDialerCommandId aCommand);

        /**
        * Set number entry observer.
        */
        void SetNumberEntryObserver( MNumberEntryObserver& aObserver );

        /**
        * Clear editor flags to default values.
        */
        void ClearEditorFlags();

        void SetEasyDialingPlugin( CDialingExtensionInterface* iEasyDialer );
        
        void SetOperationMode( TDialerOperationMode aMode );
        
        TBool Validate( const TDesC& aString );
        
    protected:
        
        /**
        * Callback function for CIdle.
        */
        static TInt SetLateFocus( TAny* aThis );
        
        /**
        * Sets focus to number entry. 
        */
        void DoSetFocus( TBool aFocus, TDrawNow aDrawNow );

    private:  // Functions from MCoeControlObserver
        /**
        * @see MCoeControlObserver
        */
        void HandleControlEventL( CCoeControl* aControl,TCoeEvent aEventType );

    private:  // Functions from CCoeControl

        /**
        * @see CCoeControl
        */
        TKeyResponse OfferKeyEventL( const TKeyEvent& aKeyEvent, 
                                     TEventCode aType );

        /**
        * @see CCoeControl
        */
        TInt CountComponentControls() const;
        
        /**
        * @see CCoeControl
        */
        CCoeControl* ComponentControl( TInt aIndex ) const;
        
        /**
        * @see CCoeControl
        */
        void Draw( const TRect& /*aRect */) const;
        
        /**
        * @see CCoeControl
        */
        void HandleResourceChange( TInt aType );        

        /**
        * @see CCoeControl
        */
        void HandlePointerEventL( const TPointerEvent& aPointerEvent );

    public:        
        /**
        * @see CCoeControl
        */
        void SetFocus( TBool aFocus, 
                       TDrawNow aDrawNow=ENoDrawNow );
					   
		/**
        * @see CCoeControl
        */			   
        void MakeVisible( TBool aVisible );
   
    private: // From CDialerContainerBase


        /**
        * @see CDialerContainerBase
        */
        void SetVariety( );
        
        /**
        * @see CDialerContainerBase
        */
        void SetLayout( );
        
        /**
        * @see CDialerContainerBase
        */                                         
        TTypeUid::Ptr MopSupplyObject( TTypeUid aId );                                         
        
        
        /**
        * Get inner and outer rectangles for rectangle frame.
        * @param aFrameRect Frame rectangle.
        * @param aOuterRect Outer rectangle (return).
        * @param aInnerRect Inner rectangle (return).
        */ 
        void RectFrameInnerOuterRects( const TRect& aFrameRect,
                                              TRect& aOuterRect,
                                              TRect& aInnerRect );        
    private:

        /**
        * 2nd phase constructor.
        */
        void ConstructL();
            
        /**
        * Constructor
        * @param aContainer Parent container
        * @param aCommandHandler
        */
        CDialerNumberEntry(
            const CCoeControl& aContainer/*,
            MDialerCommandHandler& aCommandHandler */);

        /**
        * Construct number editor from layout.
        */
        void ConstructEditorFromResourceL( TResourceReader& aReader );

        /**
        * Update formats in number entry from layout
        */
        void UpdateNumberEntryFormats();
        
        /**
        * Calculates number entry layout & format for given number entry variety
        * @param aFormat -format to be filled
        * @param aNEVariety -determines the format layot
        */
        void CalculateLayout( CAknPhoneNumberEditor::TFormat& aFormat,
						      TInt aNEVariety );
        /**
        * Start virtual keyboard.
        */
        void StartVirtualKeyBoard();

        /**
        * Checks the number entry and informs client via observer interface
        * whether the state is changed.
        */
        void InformNumberEntryState(); 
        
        
        void CheckLabelSkinningColor();
        
        /**
         * Checks editor formatting.
         */
        void HandleEditorFormatting(); 
         
    private:    // Data
        
        // Number editor field
        CAknPhoneNumberEditor* iEditor;
        
        //Prompt text label for number entry.
        CEikLabel* iLabel;
        
        // NE Font.
        CFbsFont* iNEFont;
        
        // Amount of formats for updating.
        TInt iFormatCount;
         
        // Skin control context for the editor (9-piece)
        CAknsFrameBackgroundControlContext* iFrameContext;

        TRect iOuterRect; 
        
        TRect iInnerRect;
        
        MNumberEntryObserver* iObserver;
        
        TBool iNumberContents;
        
        TBuf<KDialerPhoneNumberEntryBufferSize> iPreviousNumberEntryContent;

        // NOT OWN
        CDialingExtensionInterface* iEasyDialer;
        
        TDialerOperationMode iOperationMode;
        
        // NOT OWNED.
        CEikAppUi* iAppUi;
        
        CIdle* iLateFocuser;

    };

#endif      // CDIALERNUMBERENTRY_H

// End of File
