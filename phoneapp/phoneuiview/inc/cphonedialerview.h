/*
* Copyright (c) 2008 - 2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Phone UI Dialer View
*
*/

#ifndef __CPHONEDIALERVIEW_H
#define __CPHONEDIALERVIEW_H

//  INCLUDES

#include <coecntrl.h>
#include <coeview.h>

// FORWARD DECLARATIONS
class CAknsLayeredBackgroundControlContext;
class TPhoneCommandParam;

// CLASS DECLARATION

/**
*   View for dialer control
*/
class CPhoneDialerView : public CCoeControl, public MCoeView
    {
    public:  // Constructors and destructor        

        static CPhoneDialerView *NewL( TRect aRect );

        virtual ~CPhoneDialerView();

    public: // Functions from base classes

        /**
        * Returns a TVwsViewId uniquely identifying the view.
        * @return Represents a view in the server
        */
        TVwsViewId ViewId() const;

        void ActivateL();

        /**
        * From CCoeControl; notifier for resource changes
        * @param aType : Type of resource change
        */
        void HandleResourceChange( TInt aType );

        /**
        * From CCoeControl
        * @param aVisible : ETrue if view is to be shown
        *                   EFalse if view is to be hidden
        */
        void MakeVisible( TBool aVisible );

    public: // New functions

        /**
        * Sets the control as component.
        * @param aControl It is the new component.
        */
        void SetControl( CCoeControl* aControl );
        
        /**
        * Returns the control in the view.
        * @return aControl The control.
        */
        CCoeControl* Control() const;
        
    public:  // Functions from base classes
        
        /**
        * From CCoeControl
        */
        void SetRect( const TRect &aRect );        

    protected:  // Functions from base classes
        
        /**
        * From CCoeControl
        */
        void HandlePointerEventL(
            const TPointerEvent& aPointerEvent );

        /**
        * From CCoeControl
        */
        CCoeControl* ComponentControl( TInt aIndex ) const;

        /**
        * From CCoeControl
        */
        TInt CountComponentControls() const;

        /**
        * From CCoeControl
        */
        void Draw( const TRect& aRect ) const;

        /**
        * From CCoeControl, informs that the size is changed to update
        * the sizes of component controls appropriately.
        */
        void SizeChanged();

        /**
        * From CCoeControl, informs that the position has been changed.
        */
        void PositionChanged();

        /**
        * From CCoeControl, MopSupplyObject
        */
        TTypeUid::Ptr MopSupplyObject( TTypeUid aId );

        /**
        * From CCoeControl
        */        
        void FocusChanged( TDrawNow aDrawNow );
        
    private: // Functions from base classes

        /**
        * Takes any action required on activation of the view. 
        * Reimplemented versions should read the message text
        * @param aCustomMessage message for any message type 
        *                       identified by aCustomMessageId
        * @param aCustomMessageId message ID that the view understands
        * @param aPrevViewId The id of the previously active view 
        *                    so that the view may switch back to its caller
        *                    at some later stage
        */
        void ViewActivatedL( 
            const TVwsViewId& aPrevViewId, 
            TUid aCustomMessageId, 
            const TDesC8& aCustomMessage );

        /**
        * Takes any action required on deactivation of the view e.g. to 
        * make the controls owned by the view non-visible
        * (thereby saving the window server uneccessary work).
        */
        void ViewDeactivated();

    private:

        CPhoneDialerView();

        void ConstructL( TRect aRect );

    private:

        /**
        * Updates size of the control.
        */
        void UpdateControlRect();

    private:

        // Region type.
        typedef RRegionBuf< KPhoneRegionAmount > RPhoneRegionBuf;

        // Control rectangle
        TRect iControlRect;
        
        // Owned: region
        RPhoneRegionBuf iRegion;
        
        // Pointer to active control.
        CCoeControl* iControl;

        // Owned: background context.
        CAknsLayeredBackgroundControlContext* iBgContext;

        // Top left coordinate.
        TPoint iTopLeftCoord;
        
        // Owned: Status area use status.
        TBool iStatusAreaUsed;
        
        //Id of the fullscreen AI plugin
        TInt iFsAiId;
        
        //MainPane
        TRect iMainPane;
  
        };

#endif // __CPHONEDIALERVIEW_H

