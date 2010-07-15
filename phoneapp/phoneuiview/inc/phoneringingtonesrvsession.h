/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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


#ifndef __PHONERINGINGTONESRVSESSION_H
#define __PHONERINGINGTONESRVSESSION_H

// FORWARD DECLARATIONS
class MPhoneRingingToneServer;

// CLASS DECLARATION
class CPhoneRingingToneServerSession : public CSession2
    {
public:
    /**
    * Two-phased constructor.
    * @param aServer It is the main server object.
    * @return new instance.
    */
    static CPhoneRingingToneServerSession* NewL( MPhoneRingingToneServer& aServer );

    /**
    * Destructor.
    */
    ~CPhoneRingingToneServerSession();

protected:
    /**
    * C++ default constructor.
    */
    CPhoneRingingToneServerSession( MPhoneRingingToneServer& aServer );
    
    /**
    * Symbian 2nd phase constructor.
    */
    void ConstructL();

    /**
    * From CSession2, it is called whenever request is processed.
    * @param aMessage It is the request.
    */
    void ServiceL( const RMessage2& aMessage );

private:
    // Server.
    MPhoneRingingToneServer& iServer;

    TInt iStartSession;
    };

#endif
