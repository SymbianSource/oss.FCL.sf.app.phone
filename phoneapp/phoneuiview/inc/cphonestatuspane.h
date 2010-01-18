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
*       Manager for the statuspane, include navi pane and title pane
*
*/


#ifndef CPHONESTATUSPANE_H
#define CPHONESTATUSPANE_H

// INCLUDES
#include <e32base.h>
#include <coemain.h>
#include <aknnavi.h>    // for CAknNavigationControlContainer & 
                        // CAknNavigationDecorator

#include "mphonestatuspane.h"
#include "phoneconstants.h"

// CONSTANTS

// FORWARD DECLARATIONS
class CAknTitlePane;
class CEikStatusPane;
class TPhoneCommandParam;
class CAknIndicatorContainer;

// CLASS DECLARATION

class THandlerTag
    {
    public:
   		MPhoneStatusPaneObserver* iObserver;
    	TInt iLayer;
    };
/**
*  Manager for the statuspane, include navi pane and title pane
*  Usage:
*  Every client in phone application that will change the title pane should
*  use status pane manager, to avoid confliction. 
*  - Always use iStatusPaneManager.TitlePane() to get the reference of status 
*    pane.
*  - in ConstructL() insert a new handler to status pane manager by 
*    AddTitlePaneHandlerL( *this )
*  - Reserve the title pane before you change the title pane by 
*    ReserveTitlePane( *this )
*  - Before changing the status pane, always check if I'm the topmost handler
*    of the manager, by iStatusPaneManager->IsTitlePaneVisible()
*  - release the title pane after the change
*  
*/
class CPhoneStatusPane : 
    public CCoeStatic, 
    public MPhoneStatusPane
    {
    public:  // Constructors and destructor
        
        /**
        * First call initializes the singleton object. Subsequent calls return
        * instance.
        * @return the created instance.
        */
        static CPhoneStatusPane* Instance();

        /**
        * Destructor.
        */
        virtual ~CPhoneStatusPane();

    public: // from MPAStatusPane

        /**
        * Returns ETrue if status pane is visible.
        */
        TBool IsVisible() const;

        /**
        * Get the reference of the title pane
        * @return the reference of the title pane
        */
        CAknTitlePane& TitlePane() const;

        /**
        * Get the reference of the title pane
        * @return the reference of the title pane
        */
        CEikStatusPane& StatusPane() const;

        /**
        * Get the reference of the navi pane
        * @return the reference of the navi pane
        */
        CAknNavigationControlContainer& NaviPane() const;

        /**
        * Get the reference of the navi decorator
        * @return the reference of the navi decorator
        */
        CAknNavigationDecorator& NaviDecorator() const;

        /**
        * Add title pane handler, returns reserve id
        * @return reservatoin id
        */
        void AddTitlePaneHandlerL( MPhoneStatusPaneObserver& aObserver );

        /**
        * Reserve the title pane
        * @param the reserve id for release and get the visibility status
        */
        void ReserveTitlePane( MPhoneStatusPaneObserver& aObserver  );

        /**
        * Release title pane that has been reserved
        * @param aReserveId the reserve id
        */
        void ReleaseTitlePane( MPhoneStatusPaneObserver& aObserver  );

        /**
        * Remove the handler
        */
        void RemoveTitlePaneHandler( MPhoneStatusPaneObserver& aObserver  );

        /**
        * Check if the title pane is visible
        * @param aReserveId the reservation id
        * @return ETrue if the title pane is visible
        */
        TBool IsTitlePaneVisible( 
            MPhoneStatusPaneObserver& aObserver  ) const;

        /**
        * Gets status pane shape.
        * @param aRegion shape of the status pane.
        */
        void GetShape( TRegion& aRegion );

    public:

        /**
        * Update the title pane.
        */
        void UpdateTitlePane();


        void SetTitlePaneContentL( const TDesC& aMessage );

        void SetTitlePanePictureL( TPhoneCommandParam* aCommandParam );
        
        /**
        * Handle update of volume leven in naviPane
        */    
        void SetVolumeLevel( 
            TPhoneCommandParam* aCommandParam );
            
        void ActivateVoiceVolumeControl(
            TPhoneCommandParam* aCommandParam);
        
        void DeactivateVoiceVolumeControl();
        
        TInt DoSwitchVolumeControlL( 
            TPhoneCommandParam* aCommandParam );

        /**
        * Creates title pane containing text.
        *
        * When instance is deleted, then text is removed from title pane.
        *
        * @param aTextResourceId text resorce.
        * @return instance, also put to cleanup stack.
        */
        CBase* CreateTextTitlePaneLC( TInt aTextResourceId );
        
        /**
        * Creates empty indicator pane.
        * @return  the pointer of empty indicator pane.
        */        
        CAknIndicatorContainer* CreateEmptyIndicatorContainerL();

    private:
    
        /**
        * Update the title pane within recovery system
        */
        void DoUpdateTitlePaneL();
        /**
        * Callback funciton for recovery system to update title pane.
        */
        static TInt UpdateTitlePaneCallBackL( TAny* aAny );

        /**
        * Pushes new decorator to navi pane replacing the
        * previous one.
        * @param aNew new to replace the old one.
        */
        void PushL( CAknNavigationDecorator& aNew );

    private:

        /**
        * Two-phased constructor.
        * @return new instance.
        */
        static CPhoneStatusPane* NewL();

        /**
        * C++ default constructor.
        */
        CPhoneStatusPane();

        /**
        * By default Symbian OS constructor is private.
        */
        void ConstructL();

    private:

        /**
        * Get the largest layer in the title pane list
        */
        TInt LargestLayerInTitlePaneList() const;

        /**
        * Get the pointer to the title pane hander by reserve id
        */
        THandlerTag* FindTitlePaneHandler( 
            MPhoneStatusPaneObserver& aObserver ) const;

        /**
        * Get the current active title pane user
        */
        THandlerTag* FindTopmostTitlePaneHandler() const;

        /**
        * Notify handler on activation event
        */
        void NotifyHandlerOnActivation( 
            THandlerTag& aHandlerTag,
            TBool aActive );

        /**
        * Call back function for recovery system to switch the layout to idle
        */
        static TInt RecoverySwitchLayoutToIdleL( TAny* aAny );

        // By default, prohibit copy constructor
        CPhoneStatusPane( const CPhoneStatusPane& );

        // Prohibit assigment operator
        CPhoneStatusPane& operator= ( const CPhoneStatusPane& );
        
        /** 
         * Function for statuspane if there is a operatorpicture 
         */ 
        void RenderBitmapOnTitlePaneL(); 
      
    private:    // Data  

        //Recovery id for switch layout to idle state
        TInt iSwitchLayoutIdleRecoveryId;

        // Recovery id for update title pane
        TInt iUpdateTitlePaneRecoveryId;

        /**
         * Array of PhoneStatusPane observers 
         * Own.
         */
        CArrayFixFlat< THandlerTag > *iTitlePaneList;

        /**
        * Pointer to Eikon status pane.
        * Not own.
        */
        CEikStatusPane* iStatusPane;
        
        /**
         * Pointer the Avkon title pane.
         * Not own.
         */
        CAknTitlePane* iTitlePane;
        
        /**
        * Title pane bitmap.
        * Own.
        */
        CFbsBitmap* iTitlePanePicture;
        
        /**
         *  Title pane bitmap mask
         *  Own.
         */
        CFbsBitmap* iTitlePanePictureMask;
        
        /**
         * ETrue if title pane text is displayed; otherwise, display the bitmap
         */ 
        TBool iTitlePaneTextIsDisplayed;

        /* 
         * The pointer to the navi pane.
         * Not own.
         */
        CAknNavigationControlContainer* iNaviPane;    
        
        /**
         * The pointer to the navi decorator
         * Own.
         */
        CAknNavigationDecorator* iNaviDecorator;
        
        // Title Pane Text
        TBuf< KPhoneTitlePaneTextSize > iTitleText; 
    };

#endif      // CPhoneStatusPane_H   
            
// End of File
