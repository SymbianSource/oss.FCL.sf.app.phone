/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This class loads contact model phone number parser DLL.
*
*/


//  INCLUDES
#include "CPhoneCntNumberParser.h"
#include <cntdb.h>
#include <ecom.h>

#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <cntphonenumparser.h>
#endif

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CCntNumberParser::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCntNumberParser* CCntNumberParser::NewL()
    {
    CCntNumberParser* self = new (ELeave) CCntNumberParser();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// Destructor
CCntNumberParser::~CCntNumberParser()
    {
    delete iParser;
    REComSession::FinalClose();
    }

// -----------------------------------------------------------------------------
// CCntNumberParser::CCntNumberParser
// C++ constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CContactPhoneNumberParser* 
    CCntNumberParser::CntPhoneNumberParser() const
    {
    return iParser;
    }

// -----------------------------------------------------------------------------
// CCntNumberParser::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CCntNumberParser::ConstructL()
    {
    RImplInfoPtrArray   implInfoArray;
    CleanupResetAndDestroyPushL( implInfoArray );
    REComSession::ListImplementationsL( KUidEcomCntPhoneNumberParserInterface, 
                                        implInfoArray );
    // Load the first implementation found for KUidEcomCntPhoneNumberParserInterface 
    const TInt count = implInfoArray.Count();
    __ASSERT_ALWAYS( count > 0, User::Leave( KErrNotFound ) );
    const TUid firstImplementationFound = implInfoArray[0]->ImplementationUid();
    iParser = reinterpret_cast<CContactPhoneNumberParser*> 
        ( CContactEcomPhoneNumberParser::NewL( firstImplementationFound ) );
    CleanupStack::PopAndDestroy( &implInfoArray );
    }

// -----------------------------------------------------------------------------
// CCntNumberParser::CCntNumberParser
// C++ constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CCntNumberParser::CCntNumberParser() 
    {
    }

// End of file
