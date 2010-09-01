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
#include    "BMBubbleManager.h" //for enumerations
#include    "BMBubbleOutlookBottomText.h"
#include    "BMBubbleImageManager.h"
#include    "BMResourceManager.h"
#include    "BMBubbleHeader.h"
#include    "BMUtils.h"
#include    "BMLayout.h"
#include    "BMLayout2.h"
#include    "telbubblecustomelement.h"

#include    <eiklabel.h> 
#include    <eikimage.h>
#include    <eikenv.h>
#include    <AknsUtils.h>
#include    <AknUtils.h>
#include    <AknBidiTextUtils.h>
#include    <AknsDrawUtils.h>

// ========================= MEMBER FUNCTIONS ================================

// ---------------------------------------------------------------------------
// CBubbleOutlookBottomText::CBubbleOutlookBottomText
//
//  
// ---------------------------------------------------------------------------
//
CBubbleOutlookBottomText::CBubbleOutlookBottomText( 
    CBubbleManager& aBubbleManager ) : CBubbleOutlookFiveLined( aBubbleManager )
    {
    }

// ---------------------------------------------------------------------------
// CBubbleOutlookBottomText::ConstructL
// 
// ---------------------------------------------------------------------------
//
void CBubbleOutlookBottomText::ConstructL()
    { 
    CBubbleOutlookFiveLined::ConstructL();
    }

// ---------------------------------------------------------------------------
// CBubbleOutlookBottomText::~CBubbleOutlookBottomText
//
//  
// ---------------------------------------------------------------------------
//
CBubbleOutlookBottomText::~CBubbleOutlookBottomText()
    {
    Reset();
    }

// ---------------------------------------------------------------------------
// CBubbleOutlookBottomText::ReadBubbleHeader
//
//  
// ---------------------------------------------------------------------------
//
void CBubbleOutlookBottomText::ReadBubbleHeader( CBubbleHeader& aHeader )
    {
    iHeader = &aHeader;
    
    iBubble = iBubbleManager.ResourceManager().ReserveEikImage( ETrue );        
    
    iThreeLinedLayout = ( iHeader->CNAP().Length() > 0 );
    
    CBubbleOutlookFiveLined::ReadBubbleHeader( aHeader );
    }

// ---------------------------------------------------------------------------
// CBubbleOutlookBottomText::SizeChanged
// called by framework when the view size is changed
//  
// ---------------------------------------------------------------------------
//
void CBubbleOutlookBottomText::SizeChanged()
    {
    if( !iHeader )
        {
        return;
        }

    AknsUtils::RegisterControlPosition( this );
    
    TRAPD( err, DoCall2LayoutL() );
    if ( err )    
        {
        iBubble->SetPicture( NULL, NULL );
                
        MAknsSkinInstance* skin = AknsUtils::SkinInstance();
        AknsDrawUtils::PrepareFrame( skin,
                                     iOuterRect,
                                     iInnerRect,
                                     iFrameId,
                                     KAknsIIDDefault );    
        }        
    }

// ---------------------------------------------------------------------------
// CBubbleOutlookBottomText::PositionChanged
// ---------------------------------------------------------------------------
//
void CBubbleOutlookBottomText::PositionChanged()
    {
    AknsUtils::RegisterControlPosition( this );
    }

// ---------------------------------------------------------------------------
// CBubbleOutlookBottomText::DoCall2Layout
// ---------------------------------------------------------------------------
//
void CBubbleOutlookBottomText::DoCall2LayoutL()
    {
    iBubble->SetPictureOwnedExternally( EFalse );
    iBubble->SetPicture( NULL, NULL );
    
    const TRect rect = Rect();
    CBubbleManager::TPhoneCallState callState = iHeader->CallState();
    switch ( callState )
        {
        case CBubbleManager::EIncoming:
        case CBubbleManager::EOutgoing:
        case CBubbleManager::EAlerting:
        case CBubbleManager::EAlertToDisconnected:
            {
            ///////////////////////////////////////////////////////////////////
            // Incoming call layout for single call. Call object text.
            ///////////////////////////////////////////////////////////////////
            
            TInt variety = iThreeLinedLayout ? 4 : 3;
            
            TAknLayoutRect bubbleRect;
            bubbleRect.LayoutRect( 
                rect, 
                BubbleLayout2::popup_call2_audio_in_background( variety ) );
            
            TRect frameRect( TPoint(0,0), bubbleRect.Rect().Size() );
            TRect outerRect;
            TRect innerRect;
            BubbleLayout2::RectFrameInnerOuterRects( frameRect, 
                                                     outerRect, 
                                                     innerRect);
            
            iFrameId = ( callState == 
                         CBubbleManager::EAlertToDisconnected ) ?
                         KAknsIIDQsnFrCall2RectDisconn : 
                         KAknsIIDQsnFrCall2Rect;
            
            iOuterRect = outerRect;
            iOuterRect.Move( Rect().iTl );
            iInnerRect = innerRect;
            iInnerRect.Move( Rect().iTl );                
            
            
            // Call indicator
            BubbleUtils::LayoutCustomElement(
                iBigCallIndicator, 
                rect, 
                BubbleLayout2::popup_call2_audio_out_call_waiting_icon(1) );
                
            BubbleUtils::LayoutControl( 
                iCyphOffImage, 
                rect,
                BubbleLayout2::popup_call2_audio_in_ciphering_icon(variety) );                                
            
            BubbleUtils::LayoutCustomElement(
                iNumberType, 
                rect, 
                BubbleLayout2::popup_call2_audio_in_call_number_type_icon(variety) );

            BubbleUtils::LayoutLabel( 
                iTextLine1, 
                rect, 
                BubbleLayout2::popup_call2_audio_in_call_text_1(variety));

            BubbleUtils::LayoutLabel( 
                iTextLine2, 
                rect, 
                BubbleLayout2::popup_call2_audio_in_call_text_2(variety));
            
            
            if ( iThreeLinedLayout )
                {
                // Textual cli 1
                BubbleUtils::LayoutLabel( 
                    iTextLine3, 
                    rect, 
                    BubbleLayout2::popup_call2_audio_in_call_text_3(2));
            
                // Textual cli 2
                BubbleUtils::LayoutLabel( 
                    iTextLine4, 
                    rect, 
                    BubbleLayout2::popup_call2_audio_in_cli_textual_2(1));

                // Textual cli 3
                BubbleUtils::LayoutLabel( 
                    iTextLine5, 
                    rect, 
                    BubbleLayout2::popup_call2_audio_in_cli_textual_3(1));
                
            
                CEikLabel* labels[] = { iTextLine4, iTextLine5 };
                SetCallObjectTextToLabelsL( iHeader->CallObjectText(),
                                            2, labels );                    
                }
            else
                {
                // Textual cli 1
                BubbleUtils::LayoutLabel( 
                    iTextLine3, 
                    rect, 
                    BubbleLayout2::popup_call2_audio_in_cli_textual_1(0));
            
                // Textual cli 2
                BubbleUtils::LayoutLabel( 
                    iTextLine4, 
                    rect, 
                    BubbleLayout2::popup_call2_audio_in_cli_textual_2(0));

                // Textual cli 3
                BubbleUtils::LayoutLabel( 
                    iTextLine5, 
                    rect, 
                    BubbleLayout2::popup_call2_audio_in_cli_textual_3(0));
                
            
                CEikLabel* labels[] = { iTextLine3, iTextLine4, iTextLine5 };
                SetCallObjectTextToLabelsL( iHeader->CallObjectText(),
                                            3, labels );                
                }
            
            // Create call object bitmaps
            iBubble->SetRect( bubbleRect.Rect() );                
            
            BubbleUtils::PrepareBubbleImageL( 
                iFrameId,
                outerRect,
                innerRect,
                iBubble );
            
            break;
            }
        default:
            break;
        }
    }

// ---------------------------------------------------------------------------
// CBubbleOutlookBottomText::SetCliTextToLabelsL
// ---------------------------------------------------------------------------
//    
void CBubbleOutlookBottomText::SetCallObjectTextToLabelsL(
    const TDesC& aText,
    TInt aLabelCount,
    CEikLabel* aLabels[] )
    {
    if ( aLabelCount == 0 )
        {
        return;    
        }
    
    CEikLabel* label1 = aLabels[0];
    if ( !label1 )
        {
        return;            
        }
        
    CArrayFix<TPtrC>* wrappedText = 
        new(ELeave) CArrayFixFlat<TPtrC>(aLabelCount);
    CleanupStack::PushL( wrappedText );
    
    CArrayFix<TInt>* lineWidths = 
        new(ELeave) CArrayFixFlat<TInt>(aLabelCount);
    CleanupStack::PushL( lineWidths );
    
    for ( TInt index(0); index < aLabelCount; index++ )
        {
        CEikLabel* label = aLabels[index];
        if ( label )         
            {
            lineWidths->InsertL( index, label->Size().iWidth );
            }
        }
    
    HBufC* buffer = HBufC::NewL( aText.Length() + 
        ( KAknBidiExtraSpacePerLine * aLabelCount ) );
    CleanupStack::PushL( buffer );
    *buffer = aText;
    TPtr text( buffer->Des() );
    
    AknBidiTextUtils::ConvertToVisualAndWrapToArrayL( 
        text,
        *lineWidths,
        *label1->Font(),
        *wrappedText,
        ETrue );
    
    // set wrapped text to labels
    TInt lineCount = wrappedText->Count();
    for ( TInt index(0); index < aLabelCount && index < lineCount ; index++ )
        {
        CEikLabel* label = aLabels[index];
        TPtrC& line = wrappedText->At(index);
         
        if ( line.Length() && label )         
            {
            label->SetTextL( line );    
            }
        }

    CleanupStack::PopAndDestroy( buffer );
    CleanupStack::PopAndDestroy( lineWidths );
    CleanupStack::PopAndDestroy( wrappedText );
    }

// End of File
