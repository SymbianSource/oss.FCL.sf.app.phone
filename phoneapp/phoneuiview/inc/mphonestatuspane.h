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


#ifndef MPHONESTATUSPANE_H
#define MPHONESTATUSPANE_H

// INCLUDES

// CONSTANTS

// FORWARD DECLARATIONS
class MPhoneStatusPaneObserver;
class CAknTitlePane;

// CLASS DECLARATION

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
class MPhoneStatusPane
    {
    public:

        /**
        * Destructor
        */
        virtual ~MPhoneStatusPane(){};

        /**
        * Returns ETrue if status pane is visible.
        */
        virtual TBool IsVisible() const = 0;

        /**
        * Get the reference of the title pane
        * @return the reference of the title pane
        */
        virtual CAknTitlePane& TitlePane() const = 0;

        /**
        * Add title pane handler, returns reserve id
        * @return reservatoin id
        */
        virtual void AddTitlePaneHandlerL(
            MPhoneStatusPaneObserver& aObserver ) = 0;

        /**
        * Reserve the title pane
        * @param the reserve id for release and get the visibility status
        */
        virtual void ReserveTitlePane(
            MPhoneStatusPaneObserver& aObserver ) = 0;

        /**
        * Release title pane that has been reserved
        * @param aReserveId the reserve id
        */
        virtual void ReleaseTitlePane( 
            MPhoneStatusPaneObserver& aObserver ) = 0;

        /**
        * Remove the handler
        */
        virtual void RemoveTitlePaneHandler( 
            MPhoneStatusPaneObserver& aObserver ) = 0;

        /**
        * Check if the title pane is visible
        * @param aReserveId the reservation id
        * @return ETrue if the title pane is visible
        */
        virtual TBool IsTitlePaneVisible( 
            MPhoneStatusPaneObserver& aObserver ) const = 0;
    };

#endif      // MPhoneStatusPane_H   
            
// End of File
