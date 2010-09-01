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
* Description:  Implementation of phonebook management in PhoneCntFinder.
*
*/


// INCLUDE FILES
#include  "CPhoneCntPbkOwner.h"
#include  "CPhCntContactManager.h"

#include  <e32property.h>

// MACROS
#ifndef __WINS__
#ifdef _DEBUG
#define PHONECNTDEBUG_ON
#endif // _DEBUG
#endif // __WINS__

#ifdef PHONECNTDEBUG_ON
#include    <e32svr.h> // class RDebug
#define PHONECNTDEBUGPRINT( x ) RDebug::Print( (x) );
#else
#define PHONECNTDEBUGPRINT( x )
#endif // PHONECNTDEBUG_ON


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhoneCntPbkOwner::CPhoneCntPbkOwner
// C++ constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPhoneCntPbkOwner::CPhoneCntPbkOwner( ) 
    {
    }

// -----------------------------------------------------------------------------
// CPhoneCntPbkOwner::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPhoneCntPbkOwner::ConstructL()
    {
    iContactManager = CPhCntContactManager::NewL();
    }

// -----------------------------------------------------------------------------
// CPhoneCntPbkOwner::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPhoneCntPbkOwner* CPhoneCntPbkOwner::NewL()
    {
    CPhoneCntPbkOwner* self = new (ELeave) CPhoneCntPbkOwner();
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();

    return self;
    }
    
// -----------------------------------------------------------------------------
// CPhoneCntPbkOwner::~CPhoneCntPbkOwner
// -----------------------------------------------------------------------------
//
CPhoneCntPbkOwner::~CPhoneCntPbkOwner()
    {
    delete iContactManager;
    }

// -----------------------------------------------------------------------------
// CPhoneCntPbkOwner::ContactManager
// -----------------------------------------------------------------------------
CPhCntContactManager* CPhoneCntPbkOwner::ContactManager() const
    {
    return iContactManager;
    }

//  End of File
