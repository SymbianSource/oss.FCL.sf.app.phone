/** Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of the License "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:
*
*/
#include <QDebug>
#include <smcmockclassincludes.h>
#include <hbinstantfeedback.h>

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// HbInstantFeedback::HbInstantFeedback
// -----------------------------------------------------------------------------
//
HbInstantFeedback::HbInstantFeedback(  )
    //:
    //HbAbstractFeedback( /**/ )
    {
    
    }


// -----------------------------------------------------------------------------
// HbInstantFeedback::HbInstantFeedback
// -----------------------------------------------------------------------------
//
HbInstantFeedback::HbInstantFeedback( 
        HbFeedback::InstantEffect /*effect*/ )
    //:
    //HbAbstractFeedback( /*effect*/ )
    {
    
    }


// -----------------------------------------------------------------------------
// HbInstantFeedback::~HbInstantFeedback
// -----------------------------------------------------------------------------
//
HbInstantFeedback::~HbInstantFeedback(  )
    {
    
    }


// -----------------------------------------------------------------------------
// HbInstantFeedback::play
// -----------------------------------------------------------------------------
//
void HbInstantFeedback::play(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// HbInstantFeedback::play
// -----------------------------------------------------------------------------
//
void HbInstantFeedback::play( 
        HbFeedback::InstantEffect effect )
    {
    SMC_MOCK_METHOD1( void, HbFeedback::InstantEffect, effect )
    }


// -----------------------------------------------------------------------------
// HbInstantFeedback::type
// -----------------------------------------------------------------------------
//
/*HbFeedback::Type HbInstantFeedback::type(  ) const
    {
    SMC_MOCK_METHOD0( HbFeedback::Type )
    }
*/

// -----------------------------------------------------------------------------
// HbInstantFeedback::isValid
// -----------------------------------------------------------------------------
//
bool HbInstantFeedback::isValid(  ) const
    {
    SMC_MOCK_METHOD0( bool )
    }


// -----------------------------------------------------------------------------
// HbInstantFeedback::instantEffect
// -----------------------------------------------------------------------------
//
HbFeedback::InstantEffect HbInstantFeedback::instantEffect(  ) const
    {
    SMC_MOCK_METHOD0( HbFeedback::InstantEffect )
    }


// -----------------------------------------------------------------------------
// HbInstantFeedback::setInstantEffect
// -----------------------------------------------------------------------------
//
void HbInstantFeedback::setInstantEffect( 
        HbFeedback::InstantEffect effect )
    {
    SMC_MOCK_METHOD1( void, HbFeedback::InstantEffect, effect )
    }


// -----------------------------------------------------------------------------
// HbInstantFeedback::operator=
// -----------------------------------------------------------------------------
//
HbInstantFeedback & HbInstantFeedback::operator=( 
        const HbInstantFeedback & feedback )
    {
    SMC_MOCK_METHOD1( HbInstantFeedback &, const HbInstantFeedback &, feedback )
    }


// -----------------------------------------------------------------------------
// HbInstantFeedback::operator==
// -----------------------------------------------------------------------------
//
bool HbInstantFeedback::operator==( 
        const HbInstantFeedback & feedback ) const
    {
    SMC_MOCK_METHOD1( bool, const HbInstantFeedback &, feedback )
    }


// -----------------------------------------------------------------------------
// HbInstantFeedback::operator!=
// -----------------------------------------------------------------------------
//
bool HbInstantFeedback::operator!=( 
        const HbInstantFeedback & feedback ) const
    {
    SMC_MOCK_METHOD1( bool, const HbInstantFeedback &, feedback )
    }


