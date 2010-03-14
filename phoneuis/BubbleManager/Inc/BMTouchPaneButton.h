/*
* Copyright (c) 2006-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Button for touch pane.
*
*/


#ifndef BMTOUCHPANEBUTTON_H_
#define BMTOUCHPANEBUTTON_H_

#include <aknbutton.h>

class MBubbleTouchPaneIconProvider;

/**
 *  Contains touch button data.
 *  
 *  @code
 *   CBubbleTouchPaneButton* button = CBubbleTouchPaneButton::NewL( 
 *                                     iIconProvider);
 *   CleanupStack::PushL( button );
 *   button->ConstructFromResouceL( reader ); 
 *       
 *   CleanupStack::Pop( button );
 *   CleanupStack::PopAndDestroy(); // reader
 *       
 *   button->SetContainerWindowL( *this );
 *   button->SetObserver( this );    
 *   button->ActivateL();
 *  @endcode
 *
 *  @lib bubblemanager.lib
 *  @since S60 S60 v5.0
 */

NONSHARABLE_CLASS( CBubbleTouchPaneButton ) : public CAknButton
    {
    public:
        /**
         * Two-phased constructor.
         * @param aIconProvider the pointer to the icon provider
         * @param aFlags The flags for the button
         */
        static CBubbleTouchPaneButton* NewL
                    ( MBubbleTouchPaneIconProvider* aIconProvider,
                      const TInt aFlags = 0 ); 
        
       

        /**
        * Destructor.
        */
        ~CBubbleTouchPaneButton();
    
    public:
        /**
         * Returns command id of the button
         *
         * @since S60 ?S60_version
         * @param aPreviousState ?description
         */
        TInt CommandId( TBool aPreviousState = EFalse ) const;
        
        /**
         * Checks does the button has the given command. 
         *
         * @since S60 v5.0
         * @param aCommand the command 
         * @return The ETrue if command can be found from iCommands array
         */
        TBool HasState( TInt aCommand ) const;
        
        /**
         * Constructs controls from a resource file.
         *
         * @param aReader The resource reader, with which to access the 
         *      control's resource values.
         */      
        void ConstructFromResouceL( TResourceReader& aReader );
        
        /**
         * Sets the current state of the button.
         *
         * @since S60 v5.0
         * @param aCommand the command
         * @param aDrawNow ETrue to redraw the button.
         */
        void SetState( TInt aCommand, TBool aDrawNow );
        
        /**
         * Sets button dimmed.
         *
         * @since S60 v5.0
         * @param ?arg1 ?description
         */
        void SetDimmed( TBool aDimmed );
        
        /**
         * Handles a change to the control's resources.
         *
         * @param aType is a message UID value.
         */
        void HandleResourceChange( TInt aType );
        
        /**
         * Sets layout text and icon rectanble of the button. 
         *
         * @param aLayoutText the layout text
         * @param aIconRect the rectangle of the button
         */
        void SetLayout( const TAknLayoutText aLayoutText,
                        const TRect aIconRect);
        
        /**
         * Sets correct graphics frame for button. 
         *
         * @param frameId the button which is pressed
         */
         TAknsItemID SelectPressedButton( TAknsItemID frameId )const; 
    
        /**
         * Sets correct graphics frame for button. 
         *
         * @param frameId the button which is dimmed
         */
         TAknsItemID SelectDimmedButton( TAknsItemID frameId )const; 
    protected:
        
        /**
         * C++ constructor for the one state button.
         *
         * @param aIconProvider the pointer to the icon provider
         * @param aFlags The flags for the button
         */
        CBubbleTouchPaneButton( 
                   MBubbleTouchPaneIconProvider* aIconProvider,
                   const TInt aFlags );
        
    protected: 
        //from base class CCoeControl
        /**
         * From CCoeControl. 
         * Draws the control. Called by window server.
        */
        void Draw(const TRect& aRect) const;
        
    private:
        /**
         * Symbian 2nd phase constructor.
         */
        void ConstructL();
        
        /**
         * Loads icons using the given icon provider
         * 
         * @param aIconProvider the icon provider
         */
        void LoadCustomIcons( MBubbleTouchPaneIconProvider& aIconProvider );
              
        /**
         * Draw text and icon according to the layout.
         * 
         * @param aGc the window graphics context
         */ 
        void LayoutIconAndText( CWindowGc& aGc ) const;
        
        /**
         * Gets the correct text color.
         * 
         * @param aPenColor the RGB colour value
         */
        void GetTextColors( TRgb& aPenColor ) const; 
        
        /**
         * Selects the correct text color.
         */
        void SelectTextColor(); 
        
    private: // data
        /**
         * Array of the button commands.
         */
        RArray<TInt> iCommands;
       
        /**
         * Touch pane icon provider.
         * Not own.
         */
        MBubbleTouchPaneIconProvider* iIconProvider;
        
        /**
         * Layout text
         */
        TAknLayoutText iLayoutText;
        
        /**
         * Rectangle of the icon of the button.
         */
        TRect iIconRect;
    };

#endif /*BMTOUCHPANEBUTTON_H_*/
