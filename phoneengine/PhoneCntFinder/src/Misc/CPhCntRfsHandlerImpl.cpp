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
* Description:  Implementation of restore factory settings.
*
*/


// INCLUDE FILES
#include    "CPhCntRfsHandlerImpl.h"
#include    <rfsHandler.h>

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhCntRfsHandlerImpl::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPhCntRfsHandlerImpl* CPhCntRfsHandlerImpl::NewL()
    {
    CPhCntRfsHandlerImpl* self = 
        new (ELeave) CPhCntRfsHandlerImpl;
    
    return self;
    }
   
// -----------------------------------------------------------------------------
// CPhCntRfsHandlerImpl::~CPhCntRfsHandlerImpl
// -----------------------------------------------------------------------------
//
CPhCntRfsHandlerImpl::~CPhCntRfsHandlerImpl()
    {
    delete iRfsHandler;
    }

// ---------------------------------------------------------
// CPhCntRfsHandlerImpl::ProcessCommandL
// ---------------------------------------------------------
//
void CPhCntRfsHandlerImpl::ProcessCommandL( 
        TRfsCommand aCommand )
    {
    TRfsType type = ERfsDeep; // By default, let's assume deep.

    switch ( aCommand )
        {
        case ERfsCmdActivateNormal:
            type = ERfsNormal;
            // Fall through..

        case ERfsCmdActivateDeep:
            // Create instance of rfs handler
            if ( !iRfsHandler )
                {
                iRfsHandler = new ( ELeave ) CRfsHandler;
                }
        
            // Activate 
            iRfsHandler->ActivateRfsL( type );
            break;

        case ERfsCmdCancel:
            {
            if ( iRfsHandler )
                {
                iRfsHandler->Cancel();
                }
            }
            break;

        default:
            // Do nothing.
            break;
        }    
    }

//  End of File  

