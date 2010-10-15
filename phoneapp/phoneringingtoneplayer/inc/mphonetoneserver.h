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
* Description:  Interface class for phone ringing tone server.
*
*/


#ifndef MPHONETONESERVER_H
#define MPHONETONESERVER_H

// CLASS DECLARATION
class MPhoneToneServer
    {
public:
 
    virtual void RunDown() const = 0;
 
    };

#endif // MPHONETONESERVER_H

// End of file
