/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Interface between ringing tone player and ui controls. 
*
*/



#ifndef MPHONEDISPLAYPROVIDER_H
#define MPHONEDISPLAYPROVIDER_H

// CLASS DECLARATION

/**
*  Interface between ringing tone player and ui controls.
*  Ringing tone player uses this interface to get display access for
*  video ringing tone playback. 
*
*  @lib Phone.lib
*  @since Series 60 3.0
*/
class MPhoneDisplayProvider
    {
    public:

        enum TPhoneDisplayType
            {
            EIncomingCallQcifVideo,
            EIncomingCallSubQcifVideo
            };

    public: // New functions

        /**
        * Activates a display. There can be one display active at time.
        * This method may be called also to change display type.
        * @since Series 60 3.0
        * @param aDisplayType Type of the display requested.
        * @param aClientWindow Window for client's use.
        * @param aUncropPane Rectangle for cropping control, relative to screen.
        * @return KErrNone If activation succeeded.                  
        */
        virtual TInt ActivateDisplay( 
            TPhoneDisplayType aDisplayType,
            RWindow& aClientWindow,
            TRect& aUncropPane ) = 0;
        
        /**
        * Deactivates the display active.
        * @since Series 60 3.0
        */
        virtual void DeactivateDisplay() = 0;
    };

#endif      // MPHONEDISPLAYPROVIDER_H   
            
// End of File
