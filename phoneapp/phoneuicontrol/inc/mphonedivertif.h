/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
*     Interface for manipulating divert information.
*
*/


#ifndef MPHONEDIVERTIF_H
#define MPHONEDIVERTIF_H

//  INCLUDES
#include <e32def.h>

// CLASS DECLARATION

/**
*  Handle all possible messages from Phone Engine
*/
class MPhoneDivertIF
    {
    public:
        
        /**
        * Destructor.
        */
        virtual ~MPhoneDivertIF() {};

        /**
        * Setter for divert indication showing in bubble.
        * @param aDivertIndication ETrue to show divert indication,
        *        EFalse to not. Usually setting EFalse isn't necessary
        *        as it's a default value in bubble creation.
        */
        virtual void SetDivertIndication(
            const TBool aDivertIndication ) = 0;            
    };

#endif      // MPHONEDIVERTIF_H   
            
// End of File
