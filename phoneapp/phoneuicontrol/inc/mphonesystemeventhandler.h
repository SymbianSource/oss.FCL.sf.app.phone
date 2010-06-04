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


#ifndef __MPHONESYSTEMEVENTHANDLER_H
#define __MPHONESYSTEMEVENTHANDLER_H

//  INCLUDES
#include <aknappui.h>

// CLASS DECLARATION

/**
*  All possible system events
*/
class MPhoneSystemEventHandler
    {
    public:

        /**
        * Destructor
        */
        virtual ~MPhoneSystemEventHandler(){};

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
        * Indicates when the Idle app is in the foreground.
        */
        virtual void HandleIdleForegroundEventL() = 0;

        /**
        * Indicates when the Phone app is is losing focus.
        */
        virtual void HandlePhoneFocusLostEventL() = 0;

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
        * Indicates when the keylock events
        */
        virtual void HandleKeyLockEnabled( TBool aKeylockEnabled ) = 0;
        
    };

#endif      // __MPHONESYSTEMEVENTHANDLER_H   
            
// End of File
