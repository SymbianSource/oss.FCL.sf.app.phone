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
*     Panic function
*
*/


#include "BMPanic.h"

#ifdef _DEBUG
    _LIT( KBMPanicCategory, "Bubblemanager" );
#endif    

GLREF_C void Panic( 
#ifdef _DEBUG                  
                TBMPanic aPanic
#else           
                TBMPanic /*aPanic*/
#endif
                )
    {
    // May be used in debug purposes. We do not really want 
    // to kill phone app's thread..
    __ASSERT_DEBUG( EFalse, User::Panic( KBMPanicCategory, aPanic ) );
    }

// End of File
