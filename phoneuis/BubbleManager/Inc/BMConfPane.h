/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
*     Container class for single line in expanded conference call
*
*/


#ifndef CONFPANE_H
#define CONFPANE_H

// INCLUDES
#include    <calslbs.h>
#include    "BMBubbleManager.h"

// FORWARD DECLARATIONS
class CEikLabel;     
class CEikImage;

// CLASS DECLARATION

/**
*  CConfPane container class
*
*  Object represents single line in conference call.
*
*  @lib bubblemanager
*  @since 1.0
*/
class CBubbleConfPane : public CCoeControl
    {
    public: // Constructors and destructor
        
        /**
        * Symbian OS 2nd phase constructor.
        */
        void ConstructL( TBool aTouchCallHandling );
        
        /**
        * Destructor.
        */
        virtual ~CBubbleConfPane();

    public: //member functions
        
        /**
        * Sets data members to initial state.
        */
        void Reset( );
          
        /**
        * Sets is pane highlighted.
        * @param aIsHighlighted ETrue means highlight is on.
        */
        void SetHighlight( const TBool& aIsHighlighted );
        
        /**
        * Returns pointer to Call indication image. Use to set the bitmap 
        * to it.
        * @return The image.
        */
        CEikImage*& CallIndicationHandle( );
        
        /**
        * Set CLI
        * @param aText The text
        * @param aDir Clipping direction
        */
        void SetText( TPtrC aText , 
                      const CBubbleManager::TPhoneClippingDirection& aDir );
        
    private: // Functions from base classes
        
        /**
        * From CCoeControl
        */
        void SizeChanged();

        /**
        * From CCoeControl
        */
        void PositionChanged();

        /**
        * From CCoeControl
        */
        TInt CountComponentControls() const;
        
        /**
        * From CCoeControl 
        */
        CCoeControl* ComponentControl( TInt aIndex ) const;
        
        /**
        * From CCoeControl 
        */
        void Draw( const TRect& aRect ) const;
        
    private:
    
        /**
        * DoCall1Layout
        */
        void DoCall1Layout();

        /**
        * DoCall2Layout
        */            
        void DoCall2Layout();
        
        /**
        * DoCall4Layout
        */            
        void DoCall4Layout();
      
    private: //data
        // Is this pane highlighted
        TBool iIsHighlighted; 
        
        // Phone image.
        CEikImage* iCallIndication; 
        // Cyphering off
        CEikImage* iCyphOffImage;
        
        // Text line; full text is clipped to fit
        mutable CEikLabel* iTextLine; 
        // Contains full text 
        TPtrC iFullText; 
        // Text's clip direction
        CBubbleManager::TPhoneClippingDirection iTextClipDirection; 
        
        // Highlight's Shadow
        TAknLayoutRect iShadow;
        // Highlight
        TAknLayoutRect iHighlight;
        
        // Call object setting
        TBool iCallObjectDisplay;
        
        // Feature flag
        TBool iTouchCallHandling;

    private:
        friend class CBubbleOutlookConference; // Handles texts
        friend class CT_CBubbleConfPane;
    };

#endif // CONFPANE_H

// End of File
