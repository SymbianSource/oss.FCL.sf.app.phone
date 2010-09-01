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
* Description:  Easy dialing panic codes and macro.
*
*/

#ifndef EASYDIALINGPANICS_H_
#define EASYDIALINGPANICS_H_

/** Easy dialing panic codes */
enum 
    {
    EEasyDialingPanicInvalidListBoxModelString = 200,
    EEasyDialingPanicNoResults,
    EEasyDialingPanicNoContactSelected,
    EEasyDialingActionNotSupported,
    EEasyDialingNoFontFound,
    // add further panics here
    };


/** Easy dialing string for panic function */
_LIT( KEasyDialingPanicString, "Easy dialing" );


/**
 * Panic function for easy dialing module. 
 * 
 */
inline void EasyDialingPanic( TInt aReason )
    {
    User::Panic( KEasyDialingPanicString, aReason );
    }


#endif /* EASYDIALINGPANICS_H_ */
