/*
* Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Declarations for class CSPAudioHandler
*
*/


#ifndef CSPAUDIOHANDLER_H
#define CSPAUDIOHANDLER_H

#include <e32base.h>
#include <sounddevice.h>

#include "mcsppubsubobserver.h"
#include "mcspcenrepobserver.h"


class CSPPubSubListener;
class CSPCenRepListener;
class CSPAudioStreams;

/**
* Handles call adding from calls not done by the plugin.
*
*/
class CSPAudioHandler: public CBase,
                       public MCSPPubSubObserver,
                       public MCSPCenRepObserver,
                       public MDevSoundObserver
    {
    
    public: //Constructors and descructor    
    
        /**
        * Two-phased constructing for the monitor.
        *
        * @param aObserver the observer for getting notification
        * @param aLine the line to monitor
        * @param aLineId line identifier
        */
        static CSPAudioHandler* NewL(  );

        /**
        * C++ default destructor.
        */
        virtual ~CSPAudioHandler( );

        /**
        * Start audio streams.  
        */
        void Start();

        /**
        * Stop audio streams. 
        */
        void Stop();

        /**
        * From base class MCSPPubSubObserver.
        * Handler for changed event.
        * @param aUid uid of setting 
        * @param aKey id of setting
        * @param aStatus status of completed AO operation
        */
        virtual void HandleNotifyPSL( const TUid aUid, const TInt& aKey, 
            const TRequestStatus& aStatus );
 
        /**
        * From base class MCSPCenRepObserver. 
        * Handler for changed event.
        * @param aUid uid of setting 
        * @param aVal value
        */
        virtual void CSPAudioHandler::HandleNotifyCenRepL( 
                const TUid aUid, 
                const TUint32 aKey,
                TInt aVal );

    protected: //From DevSound
        
        /**
        * Handles DevDound initialization completion event.
        * @param  aError.  KErrNone if successful. Other values are possible
        * indicating a problem initializing CMMFDevSound object.
        */
        void InitializeComplete( TInt aError );  
        
        /**
        * Handles CMMFDevSound object's data request event.
        * @param aBuffer. Buffer to be filled
        */
        void BufferToBeFilled( CMMFBuffer* aBuffer ); 
        
        /**
        * Handles play completion or cancel event.
        * @param  aError. The status of playback
        */
        void PlayError( TInt aError ); 
        
        /**
        * Not Supported
        */
        void ToneFinished( TInt aError );         
        
        /**
        * Not supported.
        */
        void BufferToBeEmptied( CMMFBuffer* aBuffer ); 
        
        /**
        * Not supported.
        */ 
        void RecordError( TInt aError );
         
        /**
        * Not supported.
        */ 
        void ConvertError( TInt aError ); 
        
        /**
        * Not supported.
        */ 
        void DeviceMessage( TUid aMessageType, const TDesC8& aMsg );  
    
    protected: // From CActive
        /**
        * From CActive
        * RunL
        */
        void RunL();
        
        /**
        * From CActive
        * Catches errors if RunL leaves
        * @param aError error code
        * @return error code
        */
        TInt RunError( TInt aError );
        
        /**
        * From CActive
        * Cancels the monitor
        */
        void DoCancel();

    private:
        /**
        * C++ default constructor
        * @param aObserver the observer for status change (incoming call)
        * @param aLine the line associated with the call
        * @param aLineId line identifier
        */
        CSPAudioHandler(  );
                                          
        /**
        * Constructs the monitor in the second phase.
        */
        void ConstructL();
        
    private: // data

        /**
        * Mute listening from Publish&Subscribe.
        */        
        CSPPubSubListener* iMuteListener;

        /**
        * Incall loudspeaker listening from Central Repository.
        */        
        CSPCenRepListener* iIncallLoudspeakerVolumeListener;

        /**
        * Incall ear volume listening from Central Repository.
        */        
        CSPCenRepListener* iIncallEarVolumeListener;
        
        /**
        * Audio streams handler.
        */
        CSPAudioStreams* iAudioStreams;
        
        /** 
        * Call count
        */
        TInt iCallCount;
    };
    
#endif // CSPAUDIOHANDLER_H
