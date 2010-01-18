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
* Description:  Outlook Two Lined
*
*/


// INCLUDE FILES
#include    "BMBubbleOutlookTwoLined.h"
#include    "BMResourceManager.h"
#include    "BMBubbleHeader.h"
#include    "BMUtils.h"
#include    "BMLayout.h"
#include    "BMCustomManager.h"

#include    <telbubblecustomelement.h>
#include    <telbubbleanim.h>

#include    <eiklabel.h>
#include    <eikimage.h>
#include    <featmgr.h>
#include    <AknsDrawUtils.h>

// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//
CBubbleOutlookTwoLined::CBubbleOutlookTwoLined
    ( CBubbleManager& aBubbleManager )
    : CBubbleOutlook( aBubbleManager )
    {
    }

// Symbian OS constructor can leave.
void CBubbleOutlookTwoLined::ConstructL()
    {
    iHeader = NULL;
    iTextLineNumber = 0;
          
    CBubbleOutlook::ConstructL( );
    ActivateL();
    }
    
// Destructor
CBubbleOutlookTwoLined::~CBubbleOutlookTwoLined()
    {
    }


// ---------------------------------------------------------------------------
// CBubbleOutlookTwoLined::Reset
//
//  
// ---------------------------------------------------------------------------
//
void CBubbleOutlookTwoLined::Reset( )
    {
    CBubbleResourceManager& res = iBubbleManager.ResourceManager();
    res.ReleaseEikImage( iBubble );
    iBubbleManager.CustomManager().ReleaseCustomElement( iSmallCallIndication );
    iBubbleManager.CustomManager().ReleaseCustomElement( iNumberType );
    res.ReleaseEikImage( iTypeIndication1 );
    res.ReleaseEikImage( iTypeIndication2 );
    res.ReleaseEikImage( iCyphOffImage );
    res.ReleaseEikImage( iTnImage );
    res.ReleaseEikLabel( iTextLine1 );
    res.ReleaseEikLabel( iTextLine2 );
    res.ReleaseEikLabel( iTimerCost );
    
    iBubbleManager.CustomManager().ReleaseCustomElement( iBigCallIndicator );
    iBubbleManager.CustomManager().ReleaseCustomElement( iBrandImage );

    iFullText1.Set( NULL, 0 );
    iFullText2.Set( NULL, 0 );
    iTextLineNumber = 0;

    iHeader = NULL;
    }

// ---------------------------------------------------------------------------
// CBubbleOutlookTwoLined::ReadBubbleHeader
//
//  
// ---------------------------------------------------------------------------
//
void CBubbleOutlookTwoLined::ReadBubbleHeader( CBubbleHeader& aHeader )
    {
    iHeader = &aHeader;

    CBubbleManager::TPhoneCallState callState = iHeader->CallState( );

    // set all icons. divided to rectangle and bubble shaped
    switch ( callState )
        {
        case CBubbleManager::EAlertToDisconnected:
        case CBubbleManager::EWaiting:
        case CBubbleManager::EIncoming:
        case CBubbleManager::EOutgoing:
        case CBubbleManager::EAlerting:
            {
            // Set big call indicator icon
            TBool defaultCallIndicator;
            iBigCallIndicator = 
                iBubbleManager.CustomManager().ReserveCustomElement(
                    aHeader, 
                    CTelBubbleCustomElement::EBigCallIndicator,
                    defaultCallIndicator );
            
            // Number type not shown with custom call indicator
            if ( defaultCallIndicator )
                {
                // Set number type icon
                iNumberType = 
                    iBubbleManager.CustomManager().ReserveCustomElement(
                        aHeader, 
                        CTelBubbleCustomElement::ENumberTypeIcon,
                        defaultCallIndicator );
                }
            }
            break;
        case CBubbleManager::EDisconnected:
        case CBubbleManager::EActive:
        case CBubbleManager::EOnHold:
            {
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
            }

            break;
        case CBubbleManager::ENone:
        default:
            break;
        }

    // Set Cyph off image
    BubbleUtils::SetCyphOffImage( *iHeader, iCyphOffImage, iBubbleManager );

    // Get texts and possible timer/cost string.
    // Reserved text buffer is KBubbleLabelMaxLength. 
    // If new text is longer it panics.
    // It is enough if text is cropped to fit (eg. '...1234567890') 
    BubbleUtils::ChooseTextsToTwoLines( 
        *iHeader , 
        iFullText1 , 
        iFullText2 , 
        iTimerCost , 
        iTextLine1,
        iTextLine2,
        iBubbleManager,
        iText1ClipDirection , 
        iText2ClipDirection , 
        iTextLineNumber );

    // Get thumbnail
    if ( !iCallObjectDisplay && iHeader->TnBitmap() != NULL )
        {
        iTnImage = iBubbleManager.ResourceManager().ReserveEikImage();
        if ( iTnImage && iHeader->TnBitmapMask() != NULL )
            {
            iTnImage->SetPicture( 
                iHeader->TnBitmap() , iHeader->TnBitmapMask() );
            }
        else if ( iTnImage )
            {
            iTnImage->SetPicture( iHeader->TnBitmap() );
            }

        }
    
    // Brand image
    iBrandImage = iBubbleManager.CustomManager().ReserveCustomElement(
        aHeader, 
        CTelBubbleCustomElement::EBrandImage );
    }

// ---------------------------------------------------------------------------
// CBubbleOutlookTwoLined::CountComponentControls() const
//
// All the controls are not used at all times. Athough we want to
// keep the drawing order. 
// ---------------------------------------------------------------------------
//
TInt CBubbleOutlookTwoLined::CountComponentControls() const
    {
    TInt amount = 0;

    if ( iNumberType ) 
        {
        amount++;
        }

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

    if ( iTextLine2 ) 
        {
        amount++;
        }

    if ( iTimerCost ) 
        {
        amount++;
        }

    if ( iBrandImage )
        {
        amount++;
        }
    
    if ( iBigCallIndicator )
        {
        amount++;
        }

    return amount;
    }

// ---------------------------------------------------------------------------
// CBubbleOutlookTwoLined::ComponentControl
//
//  
// ---------------------------------------------------------------------------
//
CCoeControl* CBubbleOutlookTwoLined::ComponentControl(TInt aIndex) const
    {
  
    TInt amount = -1;
    
    if ( iNumberType )
        {
        amount++;
        }

    if ( aIndex == amount )
        {
        return iNumberType->Control();
        }

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

    if ( iTextLine2 )
        {
        amount++;
        }

    if ( aIndex == amount )
        {
        return iTextLine2;
        }

    if ( iTimerCost )
        {
        amount++;
        }

    if ( aIndex == amount )
        {
        return iTimerCost;
        }

    if ( iBrandImage )
        {
        amount++;
        }

    if ( aIndex == amount )
        {
        return iBrandImage->Control();
        }

    if ( iBigCallIndicator )
        {
        amount++;
        }

    if ( aIndex == amount )
        {
        return iBigCallIndicator->Control();
        }
    
    return NULL;

    }

// ---------------------------------------------------------------------------
// CBubbleOutlookTwoLined::Draw
// 
//  
// ---------------------------------------------------------------------------
//
void CBubbleOutlookTwoLined::Draw( const TRect& /*aRect*/ ) const
    {
    // Check that proper call header is set
    
    if ( iHeader == NULL ) 
        { 
        return; 
        }
    
    if ( !iHeader->IsUsed( ) ) 
        { 
        return; 
        }
    
    if ( iHeader->IsInConference( ) ) 
        { 
        return; 
        }
    
    // Set texts to fit.
    switch (iTextLineNumber)
        {
        case 0:
            BubbleUtils::ClipToLabel( 
                iFullText1 , iTextLine1 , iText1ClipDirection );
            BubbleUtils::AddTextToEikLabel( iTextLine2 , KNullDesC );
            break;
        case 1:
            BubbleUtils::SetTextInLabel( 
                iFullText1 , iTextLine1 , iText1ClipDirection );
            BubbleUtils::ClipToLabel( 
                iFullText2 , iTextLine2 , iText2ClipDirection );
            break;
        case 2:
            BubbleUtils::ClipToLabel( 
                iFullText1 , iTextLine1 , iText1ClipDirection );
            BubbleUtils::SetTextInLabel( 
                iFullText2 , iTextLine2 , iText2ClipDirection );
            break;
        default:
            BubbleUtils::AddTextToEikLabel( iTextLine1 , KNullDesC );
            BubbleUtils::AddTextToEikLabel( iTextLine2 , KNullDesC );
            break;
        }
    }

// ---------------------------------------------------------------------------
//CBubbleOutlookTwoLined::DrawTimerCostNow
//
//  
// ---------------------------------------------------------------------------
//
void CBubbleOutlookTwoLined::DrawTimerCostNow()
    {
    if ( iHeader == NULL || iTimerCost == NULL ) 
        {
        return;
        }

    if ( iHeader->CallState() != CBubbleManager::EActive  
        || iTimerCost->Text()->Compare( iHeader->TimerCost() ) == 0 ) 
        {
        return;
        }

    BubbleUtils::ClipToLabel( 
        iHeader->TimerCost() , iTimerCost , CBubbleManager::ERight );
    DrawLabelNow( iTimerCost);
    }

// ---------------------------------------------------------------------------
// CBubbleOutlookTwoLined::DrawCLINow
//
//  
// ---------------------------------------------------------------------------
//
void CBubbleOutlookTwoLined::DrawCLINow()
    {
    if ( iHeader == NULL ) 
        {
        return;
        }

    CEikLabel* cli = NULL;

    if ( iTextLineNumber == 1 )
        {
        iFullText2.Set( iHeader->CLI() );
        cli = iTextLine2;
        }
    else
        {
        iFullText1.Set( iHeader->CLI() );
        cli = iTextLine1;
        }

    CBubbleManager::TBubbleLabelString full = iHeader->CLI();
    CBubbleManager::TPhoneClippingDirection clipDir = 
        iHeader->CLIClipDirection();

    // Set texts to fit.
    if ( full.Length() > 0 )
        {
        BubbleUtils::ClipToLabel( full , cli , clipDir );
        BubbleUtils::AddTextToEikLabel( cli , full );
        }
    else
        {
        BubbleUtils::AddTextToEikLabel( cli , KNullDesC );
        }

    // First clear the area and then draw the new text
    DrawLabelNow( cli );
    }

// ---------------------------------------------------------------------------
// CBubbleOutlookTwoLined::DrawBitmaps
//
//  
// ---------------------------------------------------------------------------
//
void CBubbleOutlookTwoLined::DrawBitmaps( CBitmapContext& aGc ) const
    {
    // Check that proper call header is set
    if ( iHeader == NULL ) 
        { 
        return; 
        }

    if ( !iHeader->IsUsed() ) 
        { 
        return; 
        }

    if ( iHeader->IsInConference() ) 
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
// CBubbleOutlookTwoLined::HandleAnimationStartL
//
//  
// ---------------------------------------------------------------------------
//
void CBubbleOutlookTwoLined::HandleAnimationStartL() const
    {
    if ( iBigCallIndicator && iBigCallIndicator->ControlType() == 
         CTelBubbleCustomElement::EBubbleAnimation )
        {
        CTelBubbleAnim* anim = 
            static_cast<CTelBubbleAnim*> ( iBigCallIndicator->Control() );
        anim->StartAnimationL();    
        }
    
    if ( iBrandImage && iBrandImage->ControlType() == 
         CTelBubbleCustomElement::EBubbleAnimation )
        {
        CTelBubbleAnim* anim = 
            static_cast<CTelBubbleAnim*> ( iBrandImage->Control() );
        anim->StartAnimationL();    
        }
    
    if ( iSmallCallIndication && iSmallCallIndication->ControlType() == 
         CTelBubbleCustomElement::EBubbleAnimation )
        {
        CTelBubbleAnim* anim = 
            static_cast<CTelBubbleAnim*> ( iSmallCallIndication->Control() );
        anim->StartAnimationL();    
        }
    
    if( iNumberType && iNumberType->ControlType() == 
         CTelBubbleCustomElement::EBubbleAnimation )
        {
        CTelBubbleAnim* anim = 
            static_cast<CTelBubbleAnim*> ( iNumberType->Control() );
        anim->StartAnimationL();    
        }

    }

// ---------------------------------------------------------------------------
//  CBubbleOutlookTwoLined::DrawCallHeaderText
// ---------------------------------------------------------------------------
//
void CBubbleOutlookTwoLined::DrawCallHeaderText()
    {
    if ( iHeader == NULL || iTextLine1 == NULL ) 
        {
        return;
        }

    if ( iHeader->CallState() != CBubbleManager::EOutgoing )
        {
        return;
        }
    
    CEikLabel* label = NULL;
    iFullText1.Set( iHeader->Text() );
    label = iTextLine1;

    CBubbleManager::TPhoneClippingDirection textDir = 
        iHeader->TextClipDirection();

    BubbleUtils::ClipToLabel( iHeader->Text(), label, textDir );
    DrawLabelNow( label );
    }

// ---------------------------------------------------------------------------
//  CBubbleOutlookTwoLined::DrawCall1Bitmaps
// ---------------------------------------------------------------------------
//
void CBubbleOutlookTwoLined::DrawCall1Bitmaps( CBitmapContext& aGc ) const
    {
    // Now starts actual drawing. 
    aGc.SetBrushColor( AKN_LAF_COLOR( BubbleLayout::LayoutColourWhite() ) );

    if ( !BubbleUtils::DrawMaskedImage( aGc , iBubble ) )
        {
        BubbleUtils::DrawBackgroundRect( aGc, Rect() );
        }
    

    // If it's a conference bubble, don't draw a thumbnail
    if ( iHeader->IsConference( ) ) 
        {
        return;
        }

    // if it's a arriving call on line 2 -> don't show TN
    if ( iCallObjectDisplay || !iTnImage || !( iTnImage->Bitmap( ) != NULL 
         && ( !( ( iHeader->CallState() == CBubbleManager::EIncoming
                || iHeader->CallState() == CBubbleManager::EWaiting )
         && iHeader->CallFlags()&CBubbleManager::ELine2 ) ) ) )
        {
        return;
        }

    // checks if image needs clipping and draws possible thumbnail image.
    if ( iTnImage->Bitmap() != NULL )
        {
        // Draw shadow
        aGc.SetPenColor( iTnImageShadow.Color() );
        aGc.DrawRect ( iTnImageShadow.Rect() );
        aGc.BitBlt( iTnImage->Rect().iTl , iHeader->TnIconBitmap() );
        }    
    }

// ---------------------------------------------------------------------------
//  CBubbleOutlookTwoLined::DrawCall2Bitmaps
// ---------------------------------------------------------------------------
//        
void CBubbleOutlookTwoLined::DrawCall2Bitmaps( CBitmapContext& aGc ) const
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
