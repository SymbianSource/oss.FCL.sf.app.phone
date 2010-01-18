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
* Description:  Provides interface to restore factory settings.
*
*/


#ifndef CPHCNTRFSHANDLER_H
#define CPHCNTRFSHANDLER_H

// INCLUDES
#include    <e32base.h>

// CLASS DECLARATION

/**
*  It provides interface to restore factory settings.
*
*  @lib PhoneCntFinder
*  @since 1.0
*/
class CPhCntRfsHandler
    : public CBase
    {
    public: // New functions       

        /**
        * It enumerates different types of commands.
        *
        * ERfsCmdActivateNormal - normal rfs
        * ERfsCmdActivateDeep - deep rfs
        * ERfsCmdCancel - cancel whatever you are doing
        */
        enum TRfsCommand
            {
            ERfsCmdActivateNormal,
            ERfsCmdActivateDeep,
            ERfsCmdCancel
            };
        
        /**
        * Perform command.
        *
        * @param aCommand command to be performed.
        */
        virtual void ProcessCommandL( TRfsCommand aCommand ) = 0;

    };

#endif      // CPHCNTRFSHANDLER_H
            
// End of File
