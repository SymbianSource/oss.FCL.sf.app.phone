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
*     Panic Function
*
*/


#ifndef BMPANIC_H
#define BMPANIC_H

//  INCLUDES
#include <e32std.h>

// DATA TYPES
enum TBMPanic // panics
    {
    EBMPanicConferenceIsUsed = 6000,
    EBMPanicConferenceNotUsed,
    EBMPanicReferredHeaderIsConferenceHeader,
    EBMPanicReferredCallHeaderIsInConferenceCall,
    EBMPanicTooManyCallsInConference,
    EBMPanicConferenceCallEmpty,
    EBMPanicInvalidConfRowNumber,

    EBMPanicInvalidNumberOfHeaders,
    EBMPanicErrorInStartAndEndChanges,
    EBMPanicPlaceEnumerationDoesNotExist,

    EBMPanicBubbleIdDoesNotExist,
    EBMPanicBubbleIdIsNotInUse,

    EBMPanicNumberEntryNotInUse,
    EBMPanicNumberEntryInUse,
    EBMPanicInvalidNumberEntry,

    EBMPanicLAF,
    EBMPanicImages,
    EBMPanicUnhandledSwitchCase,
    EBMPanicTooLongText,

    EBMPanicFeatureNotSupported,

    EBMPanicErrorInResourceManager,
    
    EBMPanicCallObjectManager,
    
    EBMPanicVideoPlayer,
        
    EBMPanicTouchPane,
    
    EBMPanicCustomization
    };

// FUNCTION PROTOTYPES

/**
* Panic function
* @since 1.0
* @param aPanic Panic enumeration
*/
GLREF_C void Panic( TBMPanic aPanic );

#endif

// End of File
