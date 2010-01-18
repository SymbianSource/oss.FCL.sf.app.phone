/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: 
*     Handles number entry component
*
*/


#ifndef BUBBLEOUTLOOKNE_H
#define BUBBLEOUTLOOKNE_H

// INCLUDES
#include    <calslbs.h>
#include    "BMBubbleOutlook.h"

// FORWARD DECLARATIONS
class CEikLabel; 
class CEikImage;
class CBubbleImageManager;
class CBubbleHeader;
class CAknPhoneNumberEditor;
class CAknsBasicBackgroundControlContext;
class CEikStatusPane;
class CAknsFrameBackgroundControlContext;

// CLASS DECLARATION

/**
*  CBubbleOutlookNumberEntry container class
*
*  Handles number entry component
*
*  @lib bubblemanager
*  @since 1.0
*/
class CBubbleOutlookNumberEntry : public CBubbleOutlook, 
                                  public MCoeControlObserver
    {
    public:
        
        // Place on the screen
        enum TPhoneNumberEntryPlace
            {
            ENENone,
            ENEBottom,         // NE + multiple calls
            ENEBottomRight,    // NE + single call
            ENEIdleState       // NE in idle state
            };
        
    public: // Constructors and destructor
        
        /**
        * Default constructor.
        * @param aBubbleManager Main container
        * @param aMutedImage Muted image pointer. This needs refresh when row
        *                    amount is changed.
        */
        CBubbleOutlookNumberEntry( CBubbleManager& aBubbleManager,
                                   const CCoeControl& aMutedImage );

        /**
        * Symbian OS 2nd phase constructor.
        */
        void ConstructL();
        
        /**
        * Destructor.
        */
        virtual ~CBubbleOutlookNumberEntry();

    public: // New functions

        /**
        * Set number entry in use.
        * @param aIsUsed ETrue if put to use.
        */
        void SetIsUsed( const TBool& aIsUsed );

        /**
        * Is number entry in use.
        * @return ETrue if in use.
        */
        TBool IsUsed() const;

        /**
        * Set number entry place.
        * @param aPlace Palce enumeration.
        */
        void SetPlace( const TPhoneNumberEntryPlace& aPlace );

        /**
        * Get number entry place
        * @return Place enumeration.
        */
        TPhoneNumberEntryPlace Place() const;

        /**
        * Get editor component.
        * @return The editor.
        */
        CCoeControl* GetEditor( ) const;

        /**
        * Set text in number entry.
        * @param aDesC Text to in.
        */
        void SetText( const TDesC& aDesC );
        
        /**
        * Get number entry text string.
        * @param aDesC Descriptor where the text is set to.
        */
        void GetText( TDes& aDesC ); 
        
        /**
        * Set number entry's editor mode.
        * @since 3.0
        * @param aMode for number editor input mode.
        * @return Input mode of the editor.
        */
        TInt ChangeEditorMode( TBool aDefaultMode = EFalse );
        
        /**
        * Get number entry's editor mode.
        * @since 3.0
        * @return Input mode of the editor.
        */
        TInt GetEditorMode() const;
        
        /**
        * Resets number entry editor to default values.
        * @since 3.0
        */
        void ResetEditorToDefaultValues();
        
    public: // Functions from base classes
        
        /**
        * From CBubbleOutlook
        */
        void Reset( );
        
        /**
        * From CBubbleOutlook
        */
        void ReadBubbleHeader( CBubbleHeader& aHeader );
        
        /**
        * From CBubbleOutlook
        */
        void DrawTimerCostNow();
        
        /**
        * From CBubbleOutlook
        */
        void DrawCLINow();

        /**
        * From CBubbleOutlook
        */
        void DrawBitmaps( CBitmapContext& aGc ) const;

        /**
        * From CBubbleOutlook.
        */
        virtual void HandleAnimationStartL() const;

        /**
        * From CBubbleOutlook.
        */
        virtual void ReleaseBitmaps();

        /**
        * From CBubbleOutlook.
        */
        virtual void RebuildBitmaps();

         /**
        * Update call header text and redraw it.
        */
        virtual void DrawCallHeaderText();

    private: // Functions from base classes
          
        /**
        * From CCoeControl
        */
        void SizeChanged();
        
        /**
        * From CCoeControl
        */
        TInt CountComponentControls() const;
        
        /**
        * From CCoeControl
        */
        CCoeControl* ComponentControl( TInt aIndex ) const;
        
        /**
        * From MCoeControlObserver
        */
        void HandleControlEventL( CCoeControl* aControl,TCoeEvent aEventType );
       
        /**
        * From CCoeControl
        */
        TKeyResponse OfferKeyEventL( const TKeyEvent& aKeyEvent, 
                                     TEventCode aType );
          
        /**
        * From CCoeControl
        */
        void FocusChanged( TDrawNow aDrawNow );

        /**
        * From CCoeControl
        */
        TTypeUid::Ptr MopSupplyObject( TTypeUid aId );

        /**
        * From CCoeControl
        */
        virtual void Draw( const TRect& aRect ) const;

    private: // New functions

        /**
        * Construct and layout the editor.
        * @param aReader a resource reader.
        * @since Series60_2.8
        * @return None.
        */
        void ConstructEditorFromResourceAndLayoutL( TResourceReader& aReader );

        /**
        * Get number entry rect.
        * @param aIndex a layout index.
        * @param aColor number entry text color value.
        * @return the rect of the number entry.
        * @since Series60_2.8
        */
        TRect NumberEntryRect( TInt aIndex, TRgb& aColor );

        /**
        * Update existing formats.
        */
        void UpdateEditorFormats( TPhoneNumberEntryPlace aPlace );

        /**
        * Update phone number editor skin color.
        */
        void UpdateEditorColor( const TRgb aColor );
        
        /**
        * SetPlace for 1-piece graphics
        */
        void SetPlace1( const TPhoneNumberEntryPlace& aPlace );
        
        /**
        * SetPlace for 9-piece graphics
        */
        void SetPlace2( const TPhoneNumberEntryPlace& aPlace );
        
        /**
        * SizeChanged for 1-piece graphics
        */
        void SizeChanged1();
        
        /**
        * SizeChanged for 9-piece graphics
        */
        void SizeChanged2();
             
        /**
         * Updates editor
         * 
         */
        void UpdateAndDrawEditor();
        
    private: //data
        
        // The editor component
        CAknPhoneNumberEditor* iEditor;
        
        // Muted image
        const CCoeControl& iMutedImage;

        // Current place
        TPhoneNumberEntryPlace iPlace;
        // Is NE used
        TBool iIsUsed;
        
        // Bubble graphics
        CEikImage* iBubble;

        // Last event
        TInt iLastEvent;
        // Was NE focused before
        TInt iWasFocused;

        // Skin control context for the editor (1-piece)
        CAknsBasicBackgroundControlContext* iSkinContext;
        
        // Skin control context for the editor (9-piece)
        CAknsFrameBackgroundControlContext* iFrameContext;

        // Amount of formats for updating.
        TInt iFormatCount;

        // Pointer to status pane.
        CEikStatusPane* iStatusPane; 
        
    };

#endif // BUBBLEOUTLOOKNE_H

// End of File
