/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Implementation of CPhoneCallHeaderTextAnimation class.
*
*/


// INCLUDE FILES
#include "cphonecallheadertextanimation.h"
#include "phoneconstants.h"
#include "phonelogger.h"
#include "cphonetimer.h"

// CONSTANTS
const TUint16 KPhoneCallHeaderAnimationDot = 0x002E;
const TInt KPhoneCallHeaderDotNum = 3;
const TInt KOneSecond = 1000000;

// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CPhoneCallHeaderTextAnimation::CPhoneCallHeaderTextAnimation
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPhoneCallHeaderTextAnimation::CPhoneCallHeaderTextAnimation() : 
    CCoeStatic( KUidCallHeaderTextAnimationSingleton ),
    iTimer( NULL ),
    iDotNum( 0 ),
    iBubbleManager( NULL ),
    iActiveBubble( 0 )
    {
    }

// Destructor
CPhoneCallHeaderTextAnimation::~CPhoneCallHeaderTextAnimation()
    { 
    delete iTimer;
    iTimer = NULL;  
    }

// ---------------------------------------------------------
// CPhoneCallHeaderTextAnimation::InstanceL
// (other items were commented in a header).
// ---------------------------------------------------------
//
CPhoneCallHeaderTextAnimation* CPhoneCallHeaderTextAnimation::InstanceL()
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneCallHeaderTextAnimation::InstanceL");
    CPhoneCallHeaderTextAnimation* instance = static_cast<CPhoneCallHeaderTextAnimation*>(
        CCoeEnv::Static( KUidCallHeaderTextAnimationSingleton ) );
    if ( !instance )
        {
        instance = new ( ELeave ) CPhoneCallHeaderTextAnimation;
        CleanupStack::PushL( instance );
        instance->ConstructL();
        CleanupStack::Pop( instance );
        }
    return instance;
    }

// -----------------------------------------------------------------------------
// CPhoneCallHeaderTextAnimation::CPhoneCallHeaderTextAnimation
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPhoneCallHeaderTextAnimation::ConstructL()
    {
    iTimer = CPhoneTimer::NewL();
    }

// ---------------------------------------------------------
// CPhoneCallHeaderTextAnimation::StartAnimatingVideoCallHeaderL
// ---------------------------------------------------------
//
void CPhoneCallHeaderTextAnimation::StartAnimatingVideoCallHeaderL(
	const CBubbleManager::TBubbleId aBubbleId,    
    const TDesC& aNormalText,
    const TDesC& aShortText,
    CBubbleManager& aBubbleManager )
	{
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneCallHeaderTextAnimation::StartAnimatingVideoCallHeaderL");
	iActiveBubble = aBubbleId;
	iBubbleManager = &aBubbleManager;

	SetBubbleTexts( aNormalText, aShortText );
	
	// Update immediately when we start
	HandleTimeOutL();		
	}

// ---------------------------------------------------------
// CPhoneCallHeaderTextAnimation::RemoveAnimatingVideoCallHeader
// ---------------------------------------------------------
//
void CPhoneCallHeaderTextAnimation::RemoveAnimatingVideoCallHeader()
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneCallHeaderTextAnimation::RemoveAnimatingVideoCallHeader");
    if( iTimer )
        {
        if( iTimer->IsActive() )
            {
            iTimer->CancelTimer();                
            }          
        }
    iDotNum = 0;
    }

// ---------------------------------------------------------
// CPhoneCallHeaderTextAnimation::HandleTimeOutL
// ---------------------------------------------------------
//
void CPhoneCallHeaderTextAnimation::HandleTimeOutL()
    {
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneCallHeaderTextAnimation::HandleTimeOutL");

    iBubbleManager->UpdateCallHeaderText( iActiveBubble, 
                iNormalText, iShortText, CBubbleManager::ERight );    

    if ( iDotNum < KPhoneCallHeaderDotNum ) 
        {
        __PHONELOG( EBasic, EPhoneUIView, 
            "CPhoneCallHeaderTextAnimation::HandleTimeOutL()->iDotNum < KPhoneCallHeaderDotNum" );
        iNormalText.Append( &KPhoneCallHeaderAnimationDot, 1);
        iShortText.Append( &KPhoneCallHeaderAnimationDot, 1);
        iDotNum++;
        }
    else
        {
        __PHONELOG( EBasic, EPhoneUIView, 
            "CPhoneCallHeaderTextAnimation::HandleTimeOutL()->else..." );
        TInt normalLength = iNormalText.Length();
        iNormalText.Delete( normalLength - KPhoneCallHeaderDotNum, KPhoneCallHeaderDotNum);

        TInt shortLength = iShortText.Length();
        iShortText.Delete( shortLength - KPhoneCallHeaderDotNum, KPhoneCallHeaderDotNum);
        
        iDotNum = 0;
        }    	

    // Start timer to update headers periodically
    if ( !iTimer->IsActive() )
        {
        __PHONELOG( EBasic, EPhoneUIView, 
            "CPhoneCallHeaderTextAnimation::HandleTimeOutL()-> Start timer to update headers periodically" );
		iTimer->After( KOneSecond, this );
        }
    }

// ---------------------------------------------------------
// CPhoneCallHeaderTextAnimation::SetBubbleTexts
// ---------------------------------------------------------
//
void CPhoneCallHeaderTextAnimation::SetBubbleTexts( const TDesC& aNormalText, 
	const TDesC& aShortText )
	{
    __LOGMETHODSTARTEND(EPhoneUIView, "CPhoneCallHeaderTextAnimation::SetBubbleTexts");
    __PHONELOG1( 
        EBasic, 
        EPhoneUIView, 
        "CPhoneCallHeaderTextAnimation::SetBubbleTexts aNormalText = %S",
        &aNormalText );
        
    __PHONELOG1( 
        EBasic, 
        EPhoneUIView, 
        "CPhoneCallHeaderTextAnimation::SetBubbleTexts aShortText = %S",
        &aShortText );
        
	if( aNormalText.Length() > KPhoneCallHeaderAnimationTextLength )
		{
		iNormalText = aNormalText.Left( KPhoneCallHeaderAnimationTextLength );	
		}
	else
		{
		iNormalText = aNormalText;			
		}
		
	if( aShortText.Length() > KPhoneCallHeaderAnimationTextLength )
		{
		iShortText = aShortText.Left( KPhoneCallHeaderAnimationTextLength );		
		}
	else
		{
		iShortText = aShortText;			
		}		
	}

// End of File
