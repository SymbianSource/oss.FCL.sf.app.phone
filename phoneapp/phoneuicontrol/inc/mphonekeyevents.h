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
*   See class description.
*
*/


#ifndef MPHONEKEYEVENTS_H
#define MPHONEKEYEVENTS_H


//  INCLUDES
#include <e32def.h>
#include <e32keys.h>
#include <w32std.h>

// CLASS DECLARATION

/**
*  MPhoneKeyEvents:
*  An abstract class whose purpose is to define an interface from the key
*  event handler to the UI controller.
*/

class MPhoneKeyEvents
{
    public: // Message Ids
        
        enum TPhoneKeyEventMessages
            {
            // Processed key-events.  
            // Most classes are expected to act upon processed events only.
            EPhoneKeyShortPress,      // Args: TKeyCode
            EPhoneKeyLongPress,       // Args: TKeyCode
            };

    public:

        /**
        * Destructor
        */
        virtual ~MPhoneKeyEvents(){};

    public: // Message handling functions

        virtual void HandleKeyMessageL(
            TPhoneKeyEventMessages aMessage,
            TKeyCode aCode ) = 0;

        virtual void HandleKeyEventL(
            const TKeyEvent& aKeyEvent,
            TEventCode aEventCode ) = 0;

        virtual void HandleKeyPressDurationL(
            TKeyCode aCode,
            TTimeIntervalMicroSeconds aKeyPressDuration ) = 0;
            
        virtual void HandleLongHashL() = 0;

        virtual TBool IsNoteVisibleL() = 0;

        virtual TBool IsMenuBarVisibleL() const = 0;
        
        virtual void HandleCreateNumberEntryL( const TKeyEvent& aKeyEvent,
                TEventCode aEventCode ) = 0;
        
        virtual void HandleDtmfKeyToneL( const TKeyEvent& aKeyEvent,
                        TEventCode aEventCode ) = 0;
        
        virtual TBool IsAlphanumericSupportedAndCharInput( 
                const TKeyEvent& aKeyEvent ) = 0 ;
};

#endif // __MPHONEKEYEVENTS_H
