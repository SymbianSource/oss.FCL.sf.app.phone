/*
 * Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
 * Description: Starts and stops audio streams.
 *
 */

#ifndef TMSHandler_H
#define TMSHandler_H

#include <e32base.h>
#include "mcsptimerobserver.h"
#include <tms.h>
#include <tmsstreamobsrvr.h>

namespace TMS {
//FORWARD DECLARATIONS
class TMSFactory;
class TMSCall;
class TMSStream;
class TMSSource;
class TMSSink;
} //namespace TMS

using namespace TMS;

//FORWARD DECLARATIONS
class CSPTimer;

/**
 *  Starts and stops TMS audio streams.
 *  If the activation fails then there is a retry timer which
 *  will try the activation later.
 */
NONSHARABLE_CLASS(TmsHandler) : public CBase,
                                public MCSPTimerObserver,
                                public TMSStreamObserver
    {
public:

    /**
     * Two-phased constructing.
     */
    static TmsHandler* NewL();

    /**
     * Two-phased constructing.
     */
    static TmsHandler* NewLC();

    /**
     * C++ default destructor.
     */
    virtual ~TmsHandler();

    /**
     * Starts audio streams ie inits mic and speaker.
     */
    void StartStreams();

    /**
     * Stops audio streams ie shuts down mic and speaker.
     */
    void StopStreams();

    /**
     * From MCSPTimerObserver.
     * Notify from CSPTimer that timeout passed.
     */
    void TimerEvent();

protected:
    // Functions from base classes

    // From TMSStreamObserver
    void TMSStreamEvent(const TMSStream& stream, TMSSignalEvent event);

private:

    /**
     * C++ default constructor.
     */
    TmsHandler();

    void ConstructL();

    static TInt TimerCallBack(TAny* aThisPtr);

    void AudioStreamsStarted();

    void StartTimer();

    void StartMicAndSpeaker();

    TBool IsMicAndSpeakerStarted();

    gint CreateTMSCallControl();
    gint CreateFactory();
    gint CreateCall();

    gint CreateUplink();
    gint CreateDownlink();
    gint OpenUplink();
    gint OpenDownlink();

    gint CreateModemSource();
    gint AddModemSourceToStream();
    gint CreateModemSink();
    gint AddModemSinkToStream();
    gint CreateMicSource();
    gint AddMicSourceToStream();
    gint CreateSpeakerSink();
    gint AddSpeakerSinkToStream();

private:
    // data

    /**
     * Timer for retrying the activation of streams (mic & speaker) if
     * there was a failure in activation.
     * Own.
     */
    CSPTimer* iTimer;

    /**
     * Timeout value used with timer. Timeout value doubles every retry to
     * activate the streams.
     */
    TInt iTimeout;

    TMSFactory* iFactory;
    TMSCall* iTmsCall;
    TMSCallType iCallType;
    TMSStream* iTmsDnlink;
    TMSStream* iTmsUplink;
    TMSSource* iTmsMicSource;
    TMSSource* iTmsModemSource;
    TMSSink* iTmsSpeakerSink;
    TMSSink* iTmsModemSink;
    TBool iUplinkStarted;
    TBool iDnlinkStarted;
    };

#endif // TMSHANDLER_H
