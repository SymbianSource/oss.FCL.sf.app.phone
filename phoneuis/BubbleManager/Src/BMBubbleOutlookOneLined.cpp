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
* Description:  Implementation of CBubbleOutlookOneLined class.
*
*/


// INCLUDE FILES
#include    "BMBubbleOutlookOneLined.h"
#include    "BMResourceManager.h"
#include    "BMBubbleHeader.h"
#include    "BMUtils.h"
#include    "BMCustomManager.h"
#include    "telbubbleanim.h"
#include    "BMLayout.h"

#include    <eiklabel.h>
#include    <eikimage.h>
#include    <featmgr.h>     
#include    <AknsDrawUtils.h>

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------------------------
// CBubbleOutlookOneLined::CBubbleOutlookOneLined( )
//
//  
// ---------------------------------------------------------------------------
//
CBubbleOutlookOneLined::CBubbleOutlookOneLined( 
                                   CBubbleManager& aBubbleManager ) 
: CBubbleOutlook( aBubbleManager )                               
    {
    }

// ---------------------------------------------------------------------------
// CBubbleOutlookOneLined::ConstructL
// 
// ---------------------------------------------------------------------------
//
void CBubbleOutlookOneLined::ConstructL()
    {
    
    iHeader = NULL;
    CBubbleOutlook::ConstructL( );
    ActivateL();
    }

// ---------------------------------------------------------------------------
// CBubbleOutlookOneLined::~CBubbleOutlookOneLined()
//
//  
// ---------------------------------------------------------------------------
//
CBubbleOutlookOneLined::~CBubbleOutlookOneLined()
    {
    }


// ---------------------------------------------------------------------------
// CBubbleOutlookOneLined::Reset
//
//  
// ---------------------------------------------------------------------------
//
void CBubbleOutlookOneLined::Reset( )
    {

    iBubbleManager.ResourceManager().ReleaseEikImage( iBubble );
    iBubbleManager.CustomManager().ReleaseCustomElement( iSmallCallIndication );
    iBubbleManager.ResourceManager().ReleaseEikImage( iTypeIndication1 );
    iBubbleManager.ResourceManager().ReleaseEikImage( iTypeIndication2 );
    iBubbleManager.ResourceManager().ReleaseEikImage( iCyphOffImage );
    iBubbleManager.ResourceManager().ReleaseEikLabel( iTextLine1 );

    iFullText1.Set( NULL , 0 );
    iHeader = NULL;
    }


// ---------------------------------------------------------------------------
// CBubbleOutlookOneLined::ReadBubbleHeader
//
//  
// ---------------------------------------------------------------------------
//
void CBubbleOutlookOneLined::ReadBubbleHeader( CBubbleHeader& aHeader )
    {
    iHeader = &aHeader;
    
    CBubbleManager::TPhoneCallState callState = iHeader->CallState( );

    // Set small call indication icon
    TInt defaultCallIndicator;
    iSmallCallIndication = 
        iBubbleManager.CustomManager().ReserveCustomElement(
            aHeader, 
            CTelBubbleCustomElement::ESmallCallIndicator,
            defaultCallIndicator );
    
    // Number type not shown with custom call indicator
    if ( defaultCallIndicator )
        {
        // Set the additional icons
        BubbleUtils::SetCallTypePane( 
            *iHeader , 
            iTypeIndication1, 
            iTypeIndication2, 
            iBubbleManager );
        }

    // Set Cyph off image
    BubbleUtils::SetCyphOffImage( *iHeader, iCyphOffImage, iBubbleManager );
    
    
    
    // Get texts and possible timer/cost string.
    // Reserved text buffer is KBubbleLabelMaxLength. If new text 
    // is longer it panics.
    // It is enough if text is cropped to fit (eg. '...1234567890') 
   
    switch ( callState )
        {
        
        case CBubbleManager::EOnHold:
        case CBubbleManager::EDisconnected:
        case CBubbleManager::EActive:
        case CBubbleManager::EWaiting:
        case CBubbleManager::EOutgoing:
        case CBubbleManager::EAlertToDisconnected:
        case CBubbleManager::EAlerting:
            iTextLine1 = iBubbleManager.ResourceManager().ReserveEikLabel();
            iFullText1.Set( iHeader->CLI() );
            iText1ClipDirection = iHeader->CLIClipDirection( );
            break;
        case CBubbleManager::EIncoming:
        case CBubbleManager::ENone:
        default:
            iFullText1.Set( NULL , 0 );
            break;
        }
    }
    

// ---------------------------------------------------------------------------
// CBubbleOutlookOneLined::CountComponentControls() const
//
//  
// ---------------------------------------------------------------------------
//
TInt CBubbleOutlookOneLined::CountComponentControls() const
    {
    TInt amount = 0;
    if ( iSmallCallIndication )
        {
        amount++;
        }

    if ( iTypeIndication1 ) 
        {
        amount++;
        }

    if ( iTypeIndication2 ) 
        {
        amount++;
        }

    if ( iCyphOffImage ) 
        {
        amount++;
        }

    if ( iTextLine1 ) 
        {
        amount++;
        }
    return amount;
    }

// ---------------------------------------------------------------------------
// CBubbleOutlookOneLined::ComponentControl
//
//  
// ---------------------------------------------------------------------------
//
CCoeControl* CBubbleOutlookOneLined::ComponentControl( TInt aIndex ) const
    {
    TInt amount = -1;
    if ( iSmallCallIndication )
        {
        amount++;
        }

    if ( aIndex == amount )
        {
        return iSmallCallIndication->Control();
        }

    if ( iTypeIndication1 )
        {
        amount++;
        }

    if ( aIndex == amount )
        {
        return iTypeIndication1;
        }

    if ( iTypeIndication2 )
        {
        amount++;
        }

    if ( aIndex == amount )
        {
        return iTypeIndication2;
        }

    if ( iCyphOffImage )
        {
        amount++;
        }

    if ( aIndex == amount )
        {
        return iCyphOffImage;
        }

    if ( iTextLine1 ) 
        {
        amount++;
        }

    if ( aIndex == amount )
        {
        return iTextLine1;
        }

    return NULL;

    }


// ---------------------------------------------------------------------------
// CBubbleOutlookOneLined::Draw
// 
//  
// ---------------------------------------------------------------------------
//
void CBubbleOutlookOneLined::Draw( const TRect& /*aRect*/ ) const
    {
    
    // Check that proper call header is set
    if ( iHeader == NULL ) 
        { 
        return; 
        }
    
    if ( !iHeader->IsUsed( ) || iHeader->IsInConference( ) ) 
        { 
        return; 
        }
    
    
    // Set texts to fit.
    BubbleUtils::ClipToLabel( iFullText1 , iTextLine1 , iText1ClipDirection );  

    }

// ---------------------------------------------------------------------------
// CBubbleOutlookOneLined::DrawTimerCostNow
//
//  
// ---------------------------------------------------------------------------
//
void CBubbleOutlookOneLined::DrawTimerCostNow()
    {
    }

// ---------------------------------------------------------------------------
// CBubbleOutlookOneLined::DrawCLINow
//
//  
// ---------------------------------------------------------------------------
//
void CBubbleOutlookOneLined::DrawCLINow()
    {
    if ( iHeader == NULL ) 
        {
        return;
        }
    
    iFullText1.Set( iHeader->CLI() );
    
    CBubbleManager::TBubbleLabelString full = iHeader->CLI();
    CBubbleManager::TPhoneClippingDirection clipDir = 
        iHeader->CLIClipDirection();
    
    // Set texts to fit.
    if ( full.Length() > 0 )
        {
        BubbleUtils::ClipToLabel( full , iTextLine1 , clipDir );
        BubbleUtils::AddTextToEikLabel( iTextLine1 , full );
        }
    else
        {
        BubbleUtils::AddTextToEikLabel( iTextLine1 , KNullDesC );
        }
    
    
    // First clear the area and then draw the new text
    DrawLabelNow( iTextLine1 );
    }

// ---------------------------------------------------------------------------
// CBubbleOutlookOneLined::DrawBitmaps
//
//  
// ---------------------------------------------------------------------------
//
void CBubbleOutlookOneLined::DrawBitmaps( CBitmapContext& aGc ) const
    {
    // Check that proper call header is set
    if ( iHeader == NULL ) 
        { 
        return; 
        }
    
    if ( !iHeader->IsUsed( ) || iHeader->IsInConference( ) ) 
        { 
        return; 
        }

    if ( !iCallObjectDisplay )
        {
        DrawCall1Bitmaps( aGc );    
        }
    else
        {
        DrawCall2Bitmaps( aGc );    
        }        
    }

// ---------------------------------------------------------------------------
// CBubbleOutlookOneLined::HandleAnimationStartL
//
//  
// ---------------------------------------------------------------------------
//
void CBubbleOutlookOneLined::HandleAnimationStartL() const
    {
    if( iSmallCallIndication && 
        iSmallCallIndication->ControlType() == CTelBubbleCustomElement::EBubbleAnimation )
        {
        CTelBubbleAnim* anim = 
            static_cast<CTelBubbleAnim*> ( iSmallCallIndication->Control() );
        anim->StartAnimationL(); 
        }

    }

// ---------------------------------------------------------------------------
//  CBubbleOutlookOneLined::ReleaseBitmaps
// ---------------------------------------------------------------------------
//
void CBubbleOutlookOneLined::ReleaseBitmaps()
    {

    }

// ---------------------------------------------------------------------------
//  CBubbleOutlookOneLined::RebuildBitmaps
// ---------------------------------------------------------------------------
//
void CBubbleOutlookOneLined::RebuildBitmaps()
    {

    }

// ---------------------------------------------------------------------------
//  CBubbleOutlookOneLined::DrawCallHeaderText
// ---------------------------------------------------------------------------
//
void CBubbleOutlookOneLined::DrawCallHeaderText()
    {
    }

// ---------------------------------------------------------------------------
//  CBubbleOutlookTwoLined::DrawCall1Bitmaps
// ---------------------------------------------------------------------------
//
void CBubbleOutlookOneLined::DrawCall1Bitmaps( CBitmapContext& aGc ) const
    {

    aGc.SetBrushColor( AKN_LAF_COLOR( BubbleLayout::LayoutColourWhite() ) );
    if ( !BubbleUtils::DrawMaskedImage( aGc , iBubble ) )
        {
        BubbleUtils::DrawBackgroundRect( aGc, Rect() );
        }
    }

// ---------------------------------------------------------------------------
//  CBubbleOutlookTwoLined::DrawCall2Bitmaps
// ---------------------------------------------------------------------------
//        
void CBubbleOutlookOneLined::DrawCall2Bitmaps( CBitmapContext& aGc ) const
    {
    if ( iBubble->Bitmap() )
        {
        BubbleUtils::DrawMaskedImage( aGc , iBubble );    
        }
    else
        {
        // Draw bubble frame
        MAknsSkinInstance* skin = AknsUtils::SkinInstance();
        if ( !AknsDrawUtils::DrawFrame( skin,
                                        (CWindowGc&) aGc,  
                                        iOuterRect,
                                        iInnerRect,
                                        iFrameId,
                                        KAknsIIDDefault ) )
            {
            BubbleUtils::DrawBackgroundRect( aGc, Rect() );
            }        
        }            
    }

//  End of File  
