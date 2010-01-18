/*
* Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Container control for touch controls.
*
*/


#ifndef C_BUBBLECTOUCHPANE_H
#define C_BUBBLECTOUCHPANE_H

#include <coecntrl.h>
#include "BMTouchPaneInterface.h"

class CBubbleTouchPaneButton;
class CAknButton;
class TResourceReader;

/**
 *  Container control for touch buttons.
 *
 *
 *  @lib bubblemanager.lib
 *  @since S60 v5.0
 */
class CBubbleTouchPane : public CCoeControl,
                         public MBubbleTouchPaneInterface,
                         public MCoeControlObserver
    {
    public:
    
        /**
        * Two phase constructor.
        * @return New instance
        */
        static CBubbleTouchPane* NewL();
        
        /**
        * Destructor.
        */
        virtual ~CBubbleTouchPane();
        
        /**
        * To be called when skin is changed and icons
        * should be reloaded from skin server.
        */
        void HandleResourceChange( TInt aType );
        
        /**
        * To be called when BubbleManager is within start and end 
        * changes block. TouchPane doesn't draw itself during
        * update.
        * @param aUpdating ETrue when ui update is in progress
        *                  otherwise EFalse.
        */
        void SetUpdating( TBool aUpdating ); 

    protected: // From MBubbleTouchPaneInterface.
    
        /**
        * From MBubbleTouchPaneInterface.
        */    
        TInt SetButtonSet( TInt aResourceId );
        
        /**
        * From MBubbleTouchPaneInterface.
        */
        void SetButtonState( TInt aCommand );
        
        /**
        * From MBubbleTouchPaneInterface.
        */
        void SetButtonDimmed( TInt aCommand, TBool aDimmed );
        
        /**
        * From MBubbleTouchPaneInterface.
        */
        TInt NumberOfButtonsInPane()  const;
        
        /**
        * From MBubbleTouchPaneInterface.
        */
        TInt ButtonCommandId( TInt aButtonIndex ) const;
        
        /**
        * From MBubbleTouchPaneInterface.
        */
        TInt ReplaceButton( TInt aButtonIndex, TInt aResourceId );
        
        /**
        * From MBubbleTouchPaneInterface.
        */
        TBool ButtonHasState( TInt aButtonIndex, TInt aCommand ) const;
        
        /**
        * From MBubbleTouchPaneInterface.
        */
        void SetIconProvider(
            MBubbleTouchPaneIconProvider* aIconProvider );

    protected: // From CCoeControl
    
        /**
        * From CCoeControl.
        */
        void SizeChanged();
        
        /**
        * From CCoeControl.
        */
        TInt CountComponentControls() const;
        
        /**
        * From CCoeControl.
        */
        CCoeControl* ComponentControl( TInt aIndex ) const;
        
        /**
        * From CCoeControl.
        */
        void HandlePointerEventL( const TPointerEvent& aPointerEvent );

    protected: // MCoeControlObserver
    
        /**
        * From MCoeControlObserver.
        */
        void HandleControlEventL(CCoeControl *aControl, TCoeEvent aEventType);    

    private:
    
        /**
        * Handles callback.
        */
        static TInt IdleCallback( TAny* aThis );
        
        /**
        * Resets and destroyes touch pane buttons.
        */
        void DoDeleteButtonsInIdle();
        
        /**
        * Creates buttons.
        */
        void LoadButtonsFromResourceL( TResourceReader& aReader );
        
        /**
        * Sets used button resource.
        */
        void SetButtonSetL( TInt aResourceId );
        
        /**
        * Replaces button with other.
        */
        void ReplaceButtonL( TInt aButtonIndex, TInt aResourceId );
        
    private:

        /**
        * C++ default constructor.
        */
        CBubbleTouchPane();
        
        /**
        * Symbian OS default constructor.
        */
        void ConstructL();
        
        
    private: // data
    
        // Array of buttons.
        RPointerArray<CBubbleTouchPaneButton> iButtons;
        
        // Owned.
        CIdle* iIdleProcessor;
        
        // Async delete information.
        TBool iDeleteButtonsAsync;
        
        // Button set id.
        TInt iCurrentButtonSetId;
        
        // Array of buttons to be deleted.
        RPointerArray<CBubbleTouchPaneButton> iButtonsToBeDeleted;
        
        // Icon provider.
        MBubbleTouchPaneIconProvider* iIconProvider;
        
        // UI updating status flag.
        TBool iUpdating;
    };

#endif // C_BUBBLECTOUCHPANE_H
