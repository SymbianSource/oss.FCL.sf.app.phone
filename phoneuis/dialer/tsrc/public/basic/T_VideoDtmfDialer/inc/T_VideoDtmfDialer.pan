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
* Description: Inline implementation of video dtmf dialer. 
*
*/

#ifndef T_VIDEODTMFDIALER_PAN
#define T_VIDEODTMFDIALER_PAN

/** T_VideoDtmfDialer application panic codes */
enum TT_VideoDtmfDialerPanics 
    {
    ET_VideoDtmfDialerBasicUi = 1
    // add further panics here
    };

inline void Panic(TT_VideoDtmfDialerPanics aReason)
    {
	  _LIT(applicationName,"T_VideoDtmfDialer");
    User::Panic(applicationName, aReason);
    }

#endif // T_VIDEODTMFDIALER_PAN
