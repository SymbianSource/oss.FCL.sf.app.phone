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
* Description: 
*     Describes all possible System events.
*
*/


#ifndef __MPHONESYSTEMEVENTS_H
#define __MPHONESYSTEMEVENTS_H

//  INCLUDES
#include <aknappui.h>

// CLASS DECLARATION

/**
*  All possible system events
*/
class MPhoneSystemEvents
    {

    public:

        /**
        * Destructor
        */
        virtual ~MPhoneSystemEvents(){};

        /**
        * From CAknAppUi, initialise a menupane (dynamic).
        *
        * @param aResourceId It is the resource id for the pane.
        * @param aMenuPane It is the menu pane corresponding to the resource.
        */
        virtual void DynInitMenuPaneL( 
            TInt aResourceId, 
            CEikMenuPane* aMenuPane ) = 0;

        /**
        * From CAknAppUi, initialise a menubar (dynamic).
        *
        * @param aResourceId It is the resource id for the bar.
        * @param aMenuBar It is the menu bar corresponding to the resource.
        */
        virtual void DynInitMenuBarL( 
            TInt aResourceId, 
            CEikMenuBar* aMenuBar ) = 0;

        /**
        * From CEikAppUi. For Idle indicator
        */
        virtual void HandleSystemEventL( const TWsEvent& aEvent ) = 0;

        /**
        * From CAknAppUi, indicates when app ui is on the foreground.
        * @param aForeground It is true if app is on the foreground.
        */
        virtual void HandleForegroundEventL( TBool aForeground ) = 0;

        /**
        * Indicates when the Phone app is in the foreground.
        */
        virtual void HandlePhoneForegroundEventL() = 0;
        
        /**
        * Indicates when the Phone app is is losing focus.
        */
        virtual void HandlePhoneFocusLostEventL() = 0;
        
        /**
        * Indicates when the Idle app is in the foreground.
        */
        virtual void HandleIdleForegroundEventL() = 0;

        /**
        * Indicates when the keylock events
        */
        virtual void HandleKeyLockEnabled( TBool aKeylockEnabled ) = 0;
        
        /**
        * Handle environment changes.
        * @param aChanges environment changes which may be reported by
        *  a change notifier through the RChangeNotifier interface.
        */
        virtual void HandleEnvironmentChangeL( const TInt aChanges ) = 0;

        /**
        * Handles startup of the phone application 
        */
        virtual void HandlePhoneStartupL() = 0;

        /**
        * This function is called when there is property value change.
        * @param aCategory Category of the property
        * @param aKey Property key that is changed
        * @param aValue New property value
        */
        virtual void HandlePropertyChangedL( 
            const TUid& aCategory,
            const TUint aKey,
            const TInt aValue ) = 0;

        /**
        * Handle the change of the setting from Central Repository
        * @param aUid identifing the central repository UID.
        * @param aId central repository ID.
        */
        virtual void HandleCenRepChangeL( 
            const TUid& aUid,
            const TUint aId ) = 0;
    };

#endif      // __MPHONESYSTEMEVENTS_H   
            
// End of File
