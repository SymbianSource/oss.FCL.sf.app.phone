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
* Description:  Session class for phone ringingtone server.
*
*/

#ifndef CPHONETONESERVERSESSION_H
#define CPHONETONESERVERSESSION_H

#include <e32base.h>

class MPhoneToneServer;
class CPhoneToneServerPlaybackController;

NONSHARABLE_CLASS( CPhoneToneServerSession ) : public CSession2
    {
public:

    /**
    * Two-phased constructor.
    *
    * @param aServer Interface for server.
    * @return new instance.
    */
    static CPhoneToneServerSession* NewL( const MPhoneToneServer& aServer );
    
    ~CPhoneToneServerSession();

protected:

    /**
    * From CSession2, it is called whenever request is processed.
    * @param aMessage It is the request.
    */
    void ServiceL( const RMessage2& aMessage );

private:

    /**
    * C++ default constructor.
    *
    * @param aServer Reference to server interface
    */
    CPhoneToneServerSession( const MPhoneToneServer& aServer );
    
    void ConstructL();
    

private:
    
    const MPhoneToneServer& iServer;
    
    //Owned.
    CPhoneToneServerPlaybackController* iController;
    };

#endif  //CPHONETONESERVERSESSION_H

// End of file
