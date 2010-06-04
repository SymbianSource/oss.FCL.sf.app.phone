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
 * Description:  Starts and stops audio streams.
 *
 */

#include <tmsfactory.h>
#include <tmscall.h>
#include <tmsstream.h>
#include "tmshandler.h"
#include "csplogger.h"
#include "csptimer.h"
#include "csppanic.pan"

/**
 * Timeout initial value.
 */
const TInt KTimeoutInitial = 200000; // 0.2s

/**
 * Double the timeout for every retry.
 */
const TInt KTimeoutMultiplier = 2;

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Static constructor
// ---------------------------------------------------------------------------
//
TmsHandler* TmsHandler::NewL()
    {
    TmsHandler* self = TmsHandler::NewLC();
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------------------------
// Static constructor
// ---------------------------------------------------------------------------
//
TmsHandler* TmsHandler::NewLC()
    {
    TmsHandler* self = new (ELeave) TmsHandler();
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
TmsHandler::~TmsHandler()
    {
    if (iTimer)
        {
        iTimer->CancelNotify();
        delete iTimer;
        }
    if (iTmsUplink && iTmsCall)
        {
        // CloseUplink();
        iTmsCall->DeleteStream(iTmsUplink);
        }
    if (iTmsDnlink && iTmsCall)
        {
        // CloseDownlink();
        iTmsCall->DeleteStream(iTmsDnlink);
        }
    if (iFactory && iTmsCall)
        {
        iFactory->DeleteCall(iTmsCall);
        }
    if (iFactory && iTmsMicSource)
        {
        iFactory->DeleteSource(iTmsMicSource);
        }
    if (iFactory && iTmsModemSource)
        {
        iFactory->DeleteSource(iTmsModemSource);
        }
    if (iFactory && iTmsSpeakerSink)
        {
        iFactory->DeleteSink(iTmsSpeakerSink);
        }
    if (iFactory && iTmsModemSink)
        {
        iFactory->DeleteSink(iTmsModemSink);
        }

    delete iFactory;

    }

// ---------------------------------------------------------------------------
// From class MCSPAudioStream
// Activates mic and speaker.
// ---------------------------------------------------------------------------
//
void TmsHandler::StartStreams()
    {
    CSPLOGSTRING(CSPINT, "TmsHandler::StartStreams");

    StartMicAndSpeaker();
    }

// ---------------------------------------------------------------------------
// From class MCSPAudioStream
// Deactivates mic and speaker if the streams are active or they are
// activating.
// ---------------------------------------------------------------------------
//
void TmsHandler::StopStreams()
    {
    CSPLOGSTRING(CSPINT, "TmsHandler::StopStreams");
    gint status(TMS_RESULT_SUCCESS);
    if (iTimer && IsMicAndSpeakerStarted())
        {
        CSPLOGSTRING(CSPINT, "TmsHandler::StopStreams Stopping");
        iTimer->CancelNotify();
        iTimeout = KTimeoutInitial;
        status = iTmsUplink->Stop();
        iUplinkStarted = FALSE;
        status |= iTmsDnlink->Stop();
        iDnlinkStarted = FALSE;

        if (status != TMS_RESULT_SUCCESS)
            {
            status = TMS_RESULT_GENERAL_ERROR;
            }
        }
    CSPLOGSTRING2(CSPINT, "TmsHandler::StopStreams status %d", status);
    }

// ---------------------------------------------------------------------------
// From class MCSPTimerObserver
// Notify from CSPTimer that timeout passed. Try to start mic and
// speaker again.
// ---------------------------------------------------------------------------
//
void TmsHandler::TimerEvent()
    {
    CSPLOGSTRING(CSPINT, "TmsHandler.TimerEvent" );
    iTimeout *= KTimeoutMultiplier;
    StartMicAndSpeaker();
    }

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
TmsHandler::TmsHandler() :
    iTimeout(KTimeoutInitial)
    {
    }

// ---------------------------------------------------------------------------
// Second phase constructor
// ---------------------------------------------------------------------------
//
void TmsHandler::ConstructL()
    {
    CSPLOGSTRING(CSPINT, "TmsHandler::ConstructL");
    iTimer = CSPTimer::NewL();

    if (CreateTMSCallControl())
        {
        User::LeaveIfError(TMS_RESULT_UNINITIALIZED_OBJECT);
        }
    }

// ---------------------------------------------------------------------------
// TmsHandler::CreateTMSCallControl()
// ---------------------------------------------------------------------------
//
gint TmsHandler::CreateTMSCallControl()
    {
    TMSVer* v = NULL;
    TInt status;
    status = TMSFactory::CreateFactory(iFactory, *v);

    __ASSERT_ALWAYS(iFactory, Panic( ECSPPanicBadHandle));

    status = iFactory->CreateCall(TMS_CALL_CS, iTmsCall, 0);

    status |= CreateUplink();
    status |= CreateDownlink();
    status |= CreateMicSource();
    status |= AddMicSourceToStream();
    status |= CreateModemSink();
    status |= AddModemSinkToStream();
    status |= CreateModemSource();
    status |= AddModemSourceToStream();
    status |= CreateSpeakerSink();
    status |= AddSpeakerSinkToStream();
    status |= OpenDownlink();

    return status;
    }

// ---------------------------------------------------------------------------
// Resets timer
// ---------------------------------------------------------------------------
//
void TmsHandler::AudioStreamsStarted()
    {
    CSPLOGSTRING(CSPINT, "TmsHandler::AudioStreamsStarted" );
    iTimeout = KTimeoutInitial;
    iTimer->CancelNotify();
    }

// ---------------------------------------------------------------------------
// Starts timer
// ---------------------------------------------------------------------------
//
void TmsHandler::StartTimer()
    {
    CSPLOGSTRING(CSPINT, "TmsHandler::StartTimer" );
    iTimer->NotifyAfter(iTimeout, *this);
    }

// ---------------------------------------------------------------------------
// Starts mic and speaker
// ---------------------------------------------------------------------------
//
void TmsHandler::StartMicAndSpeaker()
    {
    // if speaker and mic is active then activation does not cause any actions.
    gint status(TMS_RESULT_SUCCESS);

    if (iTmsUplink)
        {
        status = iTmsUplink->Start();
        }
    CSPLOGSTRING2( CSPINT, "TmsHandler::StartMicAndSpeaker status %d", status );
    }

// ---------------------------------------------------------------------------
// Indicated if mic and speaker are started or starting up.
// ---------------------------------------------------------------------------
//
TBool TmsHandler::IsMicAndSpeakerStarted()
    {
    return (iUplinkStarted || iDnlinkStarted);
    }

// ----------------------------------------------------------------------------
// CreateUplink
// ----------------------------------------------------------------------------
//
gint TmsHandler::CreateUplink()
    {
    CSPLOGSTRING(CSPINT, "TmsHandler::CreateUplink");
    gint status(TMS_RESULT_SUCCESS);
    if (iTmsCall)
        {
        status = iTmsCall->CreateStream(TMS_STREAM_UPLINK, iTmsUplink);
        }
    return status;
    }

// ----------------------------------------------------------------------------
// CreateDownlink
// ----------------------------------------------------------------------------
//
gint TmsHandler::CreateDownlink()
    {
    CSPLOGSTRING(CSPINT, "TmsHandler::CreateDownlink");
    gint status(TMS_RESULT_SUCCESS);

    if (iTmsCall)
        {
        status = iTmsCall->CreateStream(TMS_STREAM_DOWNLINK, iTmsDnlink);
        }
    return status;
    }

// ----------------------------------------------------------------------------
// OpenUplink
// ----------------------------------------------------------------------------
//
gint TmsHandler::OpenUplink()
    {
    CSPLOGSTRING(CSPINT, "TmsHandler::OpenUplink");
    gint status = TMS_RESULT_SUCCESS;

    if (iTmsUplink)
        {
        status = iTmsUplink->AddObserver(*this, NULL);
        if (status == TMS_RESULT_SUCCESS)
            {
            status = iTmsUplink->Init();
            }
        }
    return status;
    }

// ----------------------------------------------------------------------------
// OpenDownlink
// ----------------------------------------------------------------------------
//
gint TmsHandler::OpenDownlink()
    {
    CSPLOGSTRING(CSPINT, "TmsHandler::OpenDownlink");
    gint status = TMS_RESULT_SUCCESS;

    if (iTmsDnlink)
        {
        status = iTmsDnlink->AddObserver(*this, NULL);
        if (status == TMS_RESULT_SUCCESS)
            {
            status = iTmsDnlink->Init();
            }
        }
    return status;
    }

// ----------------------------------------------------------------------------
// CreateModemSource
// ----------------------------------------------------------------------------
//
gint TmsHandler::CreateModemSource()
    {
    gint status(TMS_RESULT_SUCCESS);

    if (iFactory && !iTmsModemSource)
        {
        status = iFactory->CreateSource(TMS_SOURCE_MODEM, iTmsModemSource);
        }
    return status;
    }

// ----------------------------------------------------------------------------
// AddModemSourceToStream
// ----------------------------------------------------------------------------
//
gint TmsHandler::AddModemSourceToStream()
    {
    gint status(TMS_RESULT_SUCCESS);
    if (iTmsDnlink && iTmsModemSource)
        {
        status = iTmsDnlink->AddSource(iTmsModemSource);
        }
    return status;
    }

// ----------------------------------------------------------------------------
// CreateModemSink
// ----------------------------------------------------------------------------
//
gint TmsHandler::CreateModemSink()
    {
    gint status(TMS_RESULT_SUCCESS);

    if (iFactory && !iTmsModemSink)
        {
        status = iFactory->CreateSink(TMS_SINK_MODEM, iTmsModemSink);
        }
    return status;
    }

// ----------------------------------------------------------------------------
// AddModemSinkToStream
// ----------------------------------------------------------------------------
//
gint TmsHandler::AddModemSinkToStream()
    {
    gint status(TMS_RESULT_SUCCESS);
    if (iTmsUplink && iTmsModemSink)
        {
        status = iTmsUplink->AddSink(iTmsModemSink);
        }
    return status;
    }

// ----------------------------------------------------------------------------
// CreateMicSource
// ----------------------------------------------------------------------------
//
gint TmsHandler::CreateMicSource()
    {
    gint status(TMS_RESULT_SUCCESS);

    if (iFactory && !iTmsMicSource)
        {
        status = iFactory->CreateSource(TMS_SOURCE_MIC, iTmsMicSource);
        }
    return status;
    }

// ----------------------------------------------------------------------------
// AddMicSourceToStream
// ----------------------------------------------------------------------------
//
gint TmsHandler::AddMicSourceToStream()
    {
    gint status(TMS_RESULT_SUCCESS);
    if (iTmsUplink && iTmsMicSource)
        {
        status = iTmsUplink->AddSource(iTmsMicSource);
        }
    return status;
    }

// ----------------------------------------------------------------------------
// CreateSpeakerSink
// ----------------------------------------------------------------------------
//
gint TmsHandler::CreateSpeakerSink()
    {
    gint status(TMS_RESULT_SUCCESS);

    if (iFactory && !iTmsSpeakerSink)
        {
        status = iFactory->CreateSink(TMS_SINK_SPEAKER, iTmsSpeakerSink);
        }
    return status;
    }

// ----------------------------------------------------------------------------
// AddSpeakerSinkToStream
// ----------------------------------------------------------------------------
//
gint TmsHandler::AddSpeakerSinkToStream()
    {
    gint status(TMS_RESULT_SUCCESS);
    if (iTmsDnlink && iTmsSpeakerSink)
        {
        status = iTmsDnlink->AddSink(iTmsSpeakerSink);
        }
    return status;
    }

// TMS CALLBACKS

// ----------------------------------------------------------------------------
// TmsHandler::TMSStreamEvent
// ----------------------------------------------------------------------------
//
void TmsHandler::TMSStreamEvent(const TMSStream& stream, TMSSignalEvent event)
    {
    CSPLOGSTRING2(CSPINT, "TmsHandler::TMSStreamEvent status %d", event.reason);

    TMSStreamType strmType = const_cast<TMSStream&>(stream).GetStreamType();

    if (strmType == TMS_STREAM_UPLINK &&
            event.type == TMS_EVENT_STREAM_STATE_CHANGED)
        {
        switch (event.curr_state)
            {
            case TMS_STREAM_INITIALIZED:
                //notify stream ready state
                break;
            case TMS_STREAM_UNINITIALIZED:
                //notify initialization error
                break;
            case TMS_STREAM_PAUSED:
                break;
            case TMS_STREAM_STARTED:
                iUplinkStarted = TRUE;
                iTmsDnlink->Start();
                break;
            default:
                break;
            }
        }
    else if (strmType == TMS_STREAM_DOWNLINK &&
            event.type == TMS_EVENT_STREAM_STATE_CHANGED)
        {
        switch (event.curr_state)
            {
            case TMS_STREAM_INITIALIZED:
                {
                if ((iTmsCall->GetCallType() == TMS_CALL_CS)
                        && (!iUplinkStarted))
                    {
                    OpenUplink();
                    }
                //notify stream ready state
                }
                break;
            case TMS_STREAM_UNINITIALIZED:
                //notify initialization error
                break;
            case TMS_STREAM_PAUSED:
                break;
            case TMS_STREAM_STARTED:
                iDnlinkStarted = TRUE;
                break;
            default:
                break;
            }
        }
    }

//  End of File
