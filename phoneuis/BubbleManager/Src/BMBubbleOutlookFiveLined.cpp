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
* Description:  Bubble for call object text display.
*
*/


// INCLUDE FILES
#include    "BMBubbleOutlookFiveLined.h"
#include    "BMResourceManager.h"
#include    "BMBubbleHeader.h"
#include    "BMUtils.h"
#include    "BMLayout.h"
#include    "BMLayout2.h"
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
CBubbleOutlookFiveLined::CBubbleOutlookFiveLined( 
    CBubbleManager& aBubbleManager ) : CBubbleOutlook( aBubbleManager )
    {
    }

// Symbian OS default constructor can leave.
void CBubbleOutlookFiveLined::ConstructL()
    {
    iHeader = NULL;
    iTextLineNumber = 0;
          
    CBubbleOutlook::ConstructL( );
    ActivateL();
    }

// Destructor
CBubbleOutlookFiveLined::~CBubbleOutlookFiveLined()
    {
    }


// ---------------------------------------------------------------------------
// CBubbleOutlookFiveLined::Reset
//
//  
// ---------------------------------------------------------------------------
//
void CBubbleOutlookFiveLined::Reset( )
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
    res.ReleaseEikLabel( iTextLine3 );
    res.ReleaseEikLabel( iTextLine4 );
    res.ReleaseEikLabel( iTextLine5 );
    
    iBubbleManager.CustomManager().ReleaseCustomElement( iBigCallIndicator );
    
    iFullText1.Set( NULL, 0 );
    iFullText2.Set( NULL, 0 );
    iFullText3.Set( NULL, 0 );
    iFullText4.Set( NULL, 0 );
    iFullText5.Set( NULL, 0 );
    iTextLineNumber = 0;

    iHeader = NULL;
    }

// ---------------------------------------------------------------------------
// CBubbleOutlookFiveLined::ReadBubbleHeader
//
//  
// ---------------------------------------------------------------------------
//
void CBubbleOutlookFiveLined::ReadBubbleHeader( CBubbleHeader& aHeader )
    {
    iHeader = &aHeader;
    
    CBubbleManager::TPhoneCallState callState = iHeader->CallState( );
    
    switch ( callState )
        {
        case CBubbleManager::EIncoming:
        case CBubbleManager::EOutgoing:
        case CBubbleManager::EAlerting:
        case CBubbleManager::EAlertToDisconnected:
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
            break;
            
        default:
            break;
        }

    BubbleUtils::SetCyphOffImage( *iHeader, iCyphOffImage, iBubbleManager );
      
    BubbleUtils::ChooseTextsToFiveLines( 
        *iHeader, 
        iFullText1, 
        iFullText2,
        iFullText3, 
        iTextLine1,
        iTextLine2,
        iTextLine3,
        iTextLine4,
        iTextLine5,
        iBubbleManager,
        iText1ClipDirection, 
        iText2ClipDirection,
        iText3ClipDirection, 
        iTextLineNumber,
        iThreeLinedLayout );
    
    }

// ---------------------------------------------------------------------------
// CBubbleOutlookFiveLined::CountComponentControls
//
// All the controls are not used at all times. Athough we want to
// keep the drawing order. 
// ---------------------------------------------------------------------------
//
TInt CBubbleOutlookFiveLined::CountComponentControls() const
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

    if ( iTextLine3 ) 
        {
        amount++;
        }
    
    if ( iTextLine4 ) 
        {
        amount++;
        }

    if ( iTextLine5 ) 
        {
        amount++;
        }
        
    if ( iTimerCost ) 
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
// CBubbleOutlookFiveLined::ComponentControl
//
//  
// ---------------------------------------------------------------------------
//
CCoeControl* CBubbleOutlookFiveLined::ComponentControl(TInt aIndex) const
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

    if ( iTextLine3 )
        {
        amount++;
        }

    if ( aIndex == amount )
        {
        return iTextLine3;
        }
        
    if ( iTextLine4 )
        {
        amount++;
        }        
        
    if ( aIndex == amount )
        {
        return iTextLine4;
        }
    
    if ( iTextLine5 )
        {
        amount++;
        }        
        
    if ( aIndex == amount )
        {
        return iTextLine5;
        }        

    if ( iTimerCost )
        {
        amount++;
        }

    if ( aIndex == amount )
        {
        return iTimerCost;
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
// CBubbleOutlookFiveLined::Draw
// 
//  
// ---------------------------------------------------------------------------
//
void CBubbleOutlookFiveLined::Draw( const TRect& /*aRect*/ ) const
    {
    CGraphicsContext& gc = SystemGc();
    
    // Check that proper call header is set
    if ( iHeader == NULL ) 
        { 
        return; 
        }
    
    if ( !iHeader->IsUsed( ) ) 
        { 
        return; 
        }
    
    TUint32 callFlag = iHeader->CallFlags( );
	CBubbleManager::TPhoneCallState callState = iHeader->CallState( );		
    
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
            BubbleUtils::ClipToLabel( 
                iFullText3 , iTextLine3 , iText2ClipDirection );                
            break;
        case 2:
            BubbleUtils::ClipToLabel( 
                iFullText1 , iTextLine1 , iText1ClipDirection );
            BubbleUtils::SetTextInLabel( 
                iFullText2 , iTextLine2 , iText2ClipDirection );
            break;
        case 3:
            BubbleUtils::ClipToLabel( 
                iFullText1 , iTextLine1 , iText1ClipDirection );
            BubbleUtils::ClipToLabel( 
                iFullText2 , iTextLine2 , iText2ClipDirection );
            BubbleUtils::SetTextInLabel( 
                iFullText3 , iTextLine3 , iText3ClipDirection );                
            break;            
        default:
            BubbleUtils::AddTextToEikLabel( iTextLine1 , KNullDesC );
            BubbleUtils::AddTextToEikLabel( iTextLine2 , KNullDesC );
            BubbleUtils::AddTextToEikLabel( iTextLine3 , KNullDesC );
            break;
        }

    }

// ---------------------------------------------------------------------------
//CBubbleOutlookFiveLined::DrawTimerCostNow
//
//  
// ---------------------------------------------------------------------------
//
void CBubbleOutlookFiveLined::DrawTimerCostNow()
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
    
    BubbleUtils::ClipToLabel( iHeader->TimerCost() , iTimerCost , 
        CBubbleManager::ERight );
    DrawLabelNow( iTimerCost);
    }

// ---------------------------------------------------------------------------
// CBubbleOutlookFiveLined::DrawCLINow
//
//  
// ---------------------------------------------------------------------------
//
void CBubbleOutlookFiveLined::DrawCLINow()
    {
    if ( iHeader == NULL ) 
        {
        return;
        }
    
    CEikLabel* cli = NULL;
    
    if ( iTextLineNumber == 1 )
        {
        iFullText3.Set( iHeader->CLI() );
        cli = iTextLine3;
        }
    else
        {
        iFullText1.Set( iHeader->CLI() );
        cli = iTextLine1;
        }
    
    CBubbleManager::TBubbleLabelString full = iHeader->CLI();
    CBubbleManager::TPhoneClippingDirection clipDir 
        = iHeader->CLIClipDirection();
    
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
// CBubbleOutlookFiveLined::DrawBitmaps
//
//  
// ---------------------------------------------------------------------------
//
void CBubbleOutlookFiveLined::DrawBitmaps( CBitmapContext& aGc ) const
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
    
    // Now starts actual drawing.
    aGc.SetBrushColor( AKN_LAF_COLOR( BubbleLayout::LayoutColourWhite() ) );

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

// ---------------------------------------------------------------------------
// CBubbleOutlookFiveLined::HandleAnimationStartL
//
//  
// ---------------------------------------------------------------------------
//
void CBubbleOutlookFiveLined::HandleAnimationStartL() const
    {
    if ( iBigCallIndicator && iBigCallIndicator->ControlType() == 
         CTelBubbleCustomElement::EBubbleAnimation )
        {
        CTelBubbleAnim* anim = 
            static_cast<CTelBubbleAnim*> ( iBigCallIndicator->Control() );
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
//  CBubbleOutlookFiveLined::DrawCallHeaderText
// ---------------------------------------------------------------------------
//
void CBubbleOutlookFiveLined::DrawCallHeaderText()
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
    
    
    
//  End of File  
