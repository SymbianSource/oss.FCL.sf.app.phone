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
* Description:  This file contains the interface for key sequence recognition.
*
*/

#ifndef MPEKEYSEQUENCERECOGNITIONIF_H
#define MPEKEYSEQUENCERECOGNITIONIF_H

#include <e32base.h>

// CLASS DECLARATION

/**
*  Defines interface for key sequence recognition and execution. 
* 
*  @lib phoneengine.lib
*  @since S60_10.1
*/
NONSHARABLE_CLASS( MPEKeySequenceRecognitionIF ) 
{
    public:    //New functions

        /**
         * Executes provided key sequence if the sequence is recognized.
         * @param   aSequence   The key sequence.
         * @return  ETrue if the key sequence was executed, EFalse otherwise.
         */
        virtual TBool ExecuteKeySequenceL(const TDesC16& aSequence) = 0;   
    };

#endif // MPEKEYSEQUENCERECOGNITIONIF_H

// End of File
