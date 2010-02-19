/*
* Copyright (c) 2003 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Inline functions for CCoUtlInterface class.
*
*/



// INCLUDE FILES
#include    <ecom/ecom.h>
#include    <coutlinterface.hrh>

// CONSTANTS
_LIT8( KCoUtlResolverData, "default" );

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CCoUtlInterface::NewL
// ECom is used to create instance.
// -----------------------------------------------------------------------------
//
inline CCoUtlInterface* CCoUtlInterface::NewL()
    {
    const TUid KCoUtlInterfaceDefinitionUid = 
        { KCoUtlInterfaceUid };

    TEComResolverParams params;
    params.SetDataType( KCoUtlResolverData );
    params.SetWildcardMatch( EFalse );

    TAny* ptr = 
        REComSession::CreateImplementationL(
            KCoUtlInterfaceDefinitionUid,
            _FOFF( CCoUtlInterface, iDtor_ID_Key ), 
            params,
            KRomOnlyResolverUid );

    return reinterpret_cast< CCoUtlInterface* >( ptr );
    }

// -----------------------------------------------------------------------------
// CCoUtlInterface::~CCoUtlInterface
// ECom is used to delete instance..
// -----------------------------------------------------------------------------
//
inline CCoUtlInterface::~CCoUtlInterface()
    {
    REComSession::DestroyedImplementation( iDtor_ID_Key );
    }

//  End of File
