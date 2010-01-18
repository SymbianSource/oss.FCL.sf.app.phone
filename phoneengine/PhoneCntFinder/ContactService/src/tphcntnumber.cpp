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
* Description:  Phone number
*
*/


#include "tphcntnumber.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
TPhCntNumber::TPhCntNumber( 
    const TDesC& aNumber,
    MPhCntMatch::TNumberType aNumberType,
    TInt aSpeedDialPosition ) :
    iNumber( aNumber ),
    iNumberType( aNumberType ),
    iSpeedDialPosition( aSpeedDialPosition )
    {
    }
    
// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
TPhCntNumber::TPhCntNumber() : 
    iNumber( KNullDesC() ),
    iNumberType( MPhCntMatch::ENone ),
    iSpeedDialPosition()
    {
    }

// ---------------------------------------------------------------------------
// Getter
// ---------------------------------------------------------------------------
//
const TDesC& TPhCntNumber::Number() const
    {
    return iNumber;
    }

// ---------------------------------------------------------------------------
// Getter
// ---------------------------------------------------------------------------
//
MPhCntMatch::TNumberType TPhCntNumber::Type() const
    {
    return iNumberType;
    }

// ---------------------------------------------------------------------------
// Getter
// ---------------------------------------------------------------------------
//
TInt TPhCntNumber::Position() const
    {
    return iSpeedDialPosition;
    }

// ---------------------------------------------------------------------------
// Setter
// ---------------------------------------------------------------------------
//    
void TPhCntNumber::Set( 
    const TDesC& aNumber, 
    MPhCntMatch::TNumberType aNumberType,
    TInt aSpeedDialPosition )
    {
    iNumber.Set( aNumber );
    iNumberType = aNumberType;
    iSpeedDialPosition = aSpeedDialPosition; 
    }

