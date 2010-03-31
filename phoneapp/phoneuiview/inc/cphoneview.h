/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Phone UI View
*
*/

#ifndef __CPHONEVIEW_H
#define __CPHONEVIEW_H

//  INCLUDES

#include <coecntrl.h>
#include <coeview.h>

// FORWARD DECLARATIONS
class CAknsLayeredBackgroundControlContext;
class TPhoneCommandParam;
class CEikButtonGroupContainer;
class MPhoneViewObserver;

// CLASS DECLARATION

/**
*   Base class for protocol spesific call header views.
*/
class CPhoneView : public CCoeControl, public MCoeView
    {
    public:  // Constructors and destructor        

        static CPhoneView* NewL( TRect aRect, CEikButtonGroupContainer& aCba );

        virtual ~CPhoneView();

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
        
        /**
        * Sets the bitmap
        * @param aCommandParam a command parameter
        */
        void SetBitmap( TPhoneCommandParam* aCommandParam );

        /**
        * Gets the bitmap
        * @param aCommandParam a command parameter
        */
        void GetBitmap( TPhoneCommandParam* aCommandParam );
       
        /**
        * Sets the security mode flag ETrue or EFalse
        * @param aStatus ETrue for the security mode. EFalse for the 
        * normal mode.
        */ 
        void SetSecurityMode ( TBool aStatus );
        
        /**
        * Gets the previous app flag value.
        */ 
        TBool GetActivatePreviousApp();
        
        /**
        * Sets the fall back flag ETrue or EFalse
        * @param aStatus ETrue if back activates previous app.
        * EFalse if idle is to be opened.
        */ 
        void SetActivatePreviousApp( const TBool aStatus );
        
        /**
        * Sets dialer activate flag to ETrue or EFalse
        * @param aStatus ETrue if next activate view inside 
        * phoneapp is dialer. EFalse if not dialer. 
        */ 
        void SetPhoneAppViewToDialer( const TBool aStatus );
        
        /**
        * Returns dialer activate flag status.
        */ 
        TBool PhoneAppViewToDialer();
        
        /**
        * Enables or disables pointer capture for phoneview 
        * @param aStatus ETrue, pointer capture will be enabled 
        */         
        void CapturePointerEvents( const TBool aCapture );      
        
        /**
         * Sets view observer for view events.
         */
        void SetPhoneViewObserver( MPhoneViewObserver& aObserver );
        
        /**
         * Sets permission to activate dialer view.
         */
        void SetDialerActivation( TBool aActivation );
        
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

        CPhoneView( CEikButtonGroupContainer& aCba );

        void ConstructL( TRect aRect );

    private:

        /**
        * Updates size of the control.
        */
        void UpdateControlRect();

    private:

        // Region type.
        typedef RRegionBuf< KPhoneRegionAmount > RPhoneRegionBuf;

        // Owned: region
        RPhoneRegionBuf iRegion;

        // Owned: background context.
        CAknsLayeredBackgroundControlContext* iBgContext;

        // Pointer to active control.
        CCoeControl* iControl;

        // Owned: Active Idle main bitmap.
        CFbsBitmap* iBitmap;
        
        //Id of the fullscreen AI plugin
        TInt iFsAiId;
        
        //MainPane
        TRect iMainPane;
        
        //Is security mode or not
        TBool iSecurityMode;
        
        // Previous app flag.
        TBool iActivatePreviousApp;
        
        // Dialer activation flag.
        TBool iPhoneAppViewToDialer;
        
        CEikButtonGroupContainer& iCba;
        
        MPhoneViewObserver* iObserver;
        
        TBool iDialerActivation;
        };

#endif // __CPHONEVIEW_H

