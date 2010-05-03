/*
 * Copyright (c) 2007-2010 Nokia Corporation and/or its subsidiary(-ies).
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

class TmsHandler;
/**
 * Handles call adding from calls not done by the plugin.
 *
 */
class CSPAudioHandler : public CBase
    {
public:
    //Constructors and descructor

    /**
     * Two-phased constructing for the monitor.
     *
     * @param aObserver the observer for getting notification
     * @param aLine the line to monitor
     * @param aLineId line identifier
     */
    static CSPAudioHandler* NewL();

    /**
     * C++ default destructor.
     */
    virtual ~CSPAudioHandler();

    /**
     * Start audio streams.
     */
    void Start();

    /**
     * Stop audio streams.
     */
    void Stop();

protected:
    // From CActive
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
    TInt RunError(TInt aError);

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
    CSPAudioHandler();

    /**
     * Constructs the monitor in the second phase.
     */
    void ConstructL();

private:
    // data


    /**
     * Audio streams handler.
     */
    TmsHandler* iTmsHandler;

    /**
     * Call count
     */
    TInt iCallCount;
    };

#endif // CSPAUDIOHANDLER_H
