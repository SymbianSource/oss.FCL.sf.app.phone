/*
* Copyright (c) 2010-2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Client interface for phone ringing tone server.
*
*/


#ifndef RPHONETONECLIENT_H
#define RPHONETONECLIENT_H

// INCLUDES
#include <e32base.h>

// FORWARD DECLARATIONS
class TPhoneCommandParam;

// CLASS DECLARATION

class RPhoneToneClient : public RSessionBase
    {
public:

    IMPORT_C RPhoneToneClient();

    IMPORT_C ~RPhoneToneClient();

    /**
    * Starts playing the ringing tone.
    * Method plays tone according to values of parameter
    * @param aCommandParam contais ringingtone file name, volume etc.
    */    
    IMPORT_C void PlayRingingToneL( TPhoneCommandParam *aCommandParam );

    /**
    * Stops playing the ringing tone.
    * Method does not do anything if ringing tone is not playing.
    */
    IMPORT_C void StopPlayingL();

    /**
    * Establishes a connection with Server.
    * @return Result code indicating the success of operation.
    */
    IMPORT_C TInt Connect();
    
    };

#endif // RPHONETONECLIENT_H

// End of file
