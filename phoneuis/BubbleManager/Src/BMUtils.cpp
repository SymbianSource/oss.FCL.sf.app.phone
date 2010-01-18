/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Utils
*
*/


// INCLUDE FILES
#include    "BMUtils.h"             // definition
#include    "BMBubbleHeader.h"      // CBubbleHeader
#include    "BMBubbleImageManager.h"// CBubbleImageManager
#include    "BMResourceManager.h"   // Resource pool
#include    "BMPanic.h"             // Panic
#include    "BMLayout.h"
#include    "BMLayout2.h"
#include    "BubbleManagerPaths.h"
#include    "BMCallObjectManager.h"
#include    "BMCallObjectUtils.h"
#include    "telbubblecustomelement.h"

#include    <eikimage.h>            // CEikImage
#include    <eiklabel.h>            // CEikLabel
#include    <bldvariant.hrh>
#include    <AknsUtils.h>
#include    <AknBidiTextUtils.h>
#include    <featmgr.h>
#include    <aknlayoutscalable_apps.cdl.h>
#include    <AknLayout2ScalableDef.h>
#include    <applayout.cdl.h>
#include    <layoutmetadata.cdl.h>

#include    <AknsDrawUtils.h>
#include	<akniconconfig.h>

// ========================= MEMBER FUNCTIONS ================================

// ---------------------------------------------------------------------------
//  BubbleUtils::SetCallTypePane
//  Selects item for call type pane (small bubbles and connected big bubbles)
// ---------------------------------------------------------------------------
//
TBool BubbleUtils::SetCallTypePane( const CBubbleHeader& aHeader, 
                                      CEikImage*& aTypeImage1, 
                                      CEikImage*& aTypeImage2, 
                                      CBubbleManager& aBubbleManager )
    {
    CBubbleManager::TPhoneCallState callState = aHeader.CallState( );
    TUint32 callFlags = aHeader.CallFlags( ); 
    
    TInt picture1 = KErrNotFound;
    TInt picture2 = KErrNotFound;
    TInt picture1mask = KErrNotFound;
    TInt picture2mask = KErrNotFound;

    switch ( callState )
        {
        case CBubbleManager::EWaiting:
        case CBubbleManager::EIncoming:
        case CBubbleManager::EOutgoing:
        case CBubbleManager::EAlertToDisconnected:
        case CBubbleManager::EAlerting:
        case CBubbleManager::EDisconnected:
        case CBubbleManager::EActive:
        case CBubbleManager::EOnHold:
            if ( callFlags&CBubbleManager::ELine2 )
                {
                picture1 = EQgn_indi_call_line2;
                picture1mask = EQgn_indi_call_line2_mask;
                }
            
            break;
        case CBubbleManager::ENone:
        default:
            picture1 = KErrNotFound;
            picture2 = KErrNotFound;
            break;
        }

    if ( picture1 == KErrNotFound )
        {
        return EFalse;
        }

    __ASSERT_DEBUG( aTypeImage1 == NULL , 
        Panic( EBMPanicErrorInResourceManager ) );

    aTypeImage1 = aBubbleManager.ResourceManager().ReserveEikImage();

    __ASSERT_DEBUG( aTypeImage1 != NULL , 
        Panic( EBMPanicErrorInResourceManager ) );

    if ( aTypeImage1 == NULL )
        {
        return EFalse;
        }

    aBubbleManager.ImageManager().SetBitmapToImage( aTypeImage1 , 
        (TBMIcons)picture1,(TBMIcons)picture1mask );

    if ( picture2 != KErrNotFound )
        {
        __ASSERT_DEBUG( aTypeImage2 == NULL , 
            Panic( EBMPanicErrorInResourceManager ) );

        aTypeImage2 = aBubbleManager.ResourceManager().ReserveEikImage();

        __ASSERT_DEBUG( aTypeImage2 != NULL , 
            Panic( EBMPanicErrorInResourceManager ) );

        aBubbleManager.ImageManager().SetBitmapToImage( aTypeImage2 , 
            (TBMIcons)picture2 , (TBMIcons)picture2mask );

        }
    return ETrue;
    }

// ---------------------------------------------------------------------------
//  BubbleUtils::SetCyphOffImage
// ---------------------------------------------------------------------------
// 
TBool BubbleUtils::SetCyphOffImage( const CBubbleHeader& aHeader, 
                                      CEikImage*& aCypfOffImage, 
                                      CBubbleManager& aBubbleManager )
    {
    TUint32 callFlags = aHeader.CallFlags(); 
    if ( callFlags&CBubbleManager::ENoCiphering )
        {
        aCypfOffImage = aBubbleManager.ResourceManager().ReserveEikImage();
        __ASSERT_DEBUG( aCypfOffImage != NULL , 
            Panic( EBMPanicErrorInResourceManager ) );

        aBubbleManager.ImageManager().SetBitmapToImage( aCypfOffImage , 
            EQgn_indi_call_cyphering_off,
            EQgn_indi_call_cyphering_off_mask );
        return ETrue;
        }
    return EFalse;
    }

// ---------------------------------------------------------------------------
//  BubbleUtils::ChooseTextsToTwoLines
// ---------------------------------------------------------------------------
// 
void  BubbleUtils::ChooseTextsToTwoLines( const CBubbleHeader& aHeader,
            TPtrC& aFirstLineText,
            TPtrC& aSecondLineText,
            CEikLabel*& aTimerCost,
            CEikLabel*& aTextLine1,
            CEikLabel*& aTextLine2,
            CBubbleManager& aBubbleManager,
            CBubbleManager::TPhoneClippingDirection& aFirstLineDir,
            CBubbleManager::TPhoneClippingDirection& aSecondLineDir,
            TUint8& aTextLineNumber )
    {
    CBubbleManager::TPhoneCallState callState = aHeader.CallState( );
    TUint32 callFlags = aHeader.CallFlags( ); 

    __ASSERT_DEBUG( aTextLine1 == NULL , 
            Panic( EBMPanicErrorInResourceManager ) );
    __ASSERT_DEBUG( aTextLine2 == NULL , 
            Panic( EBMPanicErrorInResourceManager ) );
    __ASSERT_DEBUG( aTimerCost == NULL , 
            Panic( EBMPanicErrorInResourceManager ) );

    switch ( callState )
        {
        case CBubbleManager::EWaiting:        // 1.cli 2.text
        case CBubbleManager::EIncoming:        // 1.cli 2.text
            // if there is arriving data or fax call and there is
            // cli -> there is something in text field, swap texts
            aTextLine1 = aBubbleManager.ResourceManager().ReserveEikLabel();
            aTextLine2 = aBubbleManager.ResourceManager().ReserveEikLabel();
        
            aFirstLineText.Set( aHeader.CLI() );
            aFirstLineDir = aHeader.CLIClipDirection( );
            aSecondLineText.Set( aHeader.Text() );
            aSecondLineDir = aHeader.TextClipDirection( );
            aTextLineNumber = 2;    // Bubble text line number
            break;
        case CBubbleManager::EAlertToDisconnected:
        case CBubbleManager::EDisconnected:
        case CBubbleManager::EOnHold:
        case CBubbleManager::EAlerting:         // 1.cli 2.text
            aTextLine1 = aBubbleManager.ResourceManager().ReserveEikLabel();
            aTextLine2 = aBubbleManager.ResourceManager().ReserveEikLabel();
            aFirstLineText.Set( aHeader.CLI() );
            aFirstLineDir = aHeader.CLIClipDirection( );
            aSecondLineText.Set( aHeader.Text() );
            aSecondLineDir = aHeader.TextClipDirection( );
            aTextLineNumber = 2;    // Bubble text line number
            break;
        case CBubbleManager::EActive:            // 1.cli 2.timer
            aTextLine1 = aBubbleManager.ResourceManager().ReserveEikLabel();
            aTimerCost = aBubbleManager.ResourceManager().ReserveEikLabel();
            aFirstLineText.Set( aHeader.CLI() );
            aFirstLineDir = aHeader.CLIClipDirection( );
            AddTextToEikLabel( *aTimerCost , aHeader.TimerCost( ) );
            aSecondLineText.Set( NULL , 0 );
            aTextLineNumber = 0;    // no text is used
            break;
        case CBubbleManager::EOutgoing:    // 2.cli 1.text
            aTextLine1 = aBubbleManager.ResourceManager().ReserveEikLabel();
            aTextLine2 = aBubbleManager.ResourceManager().ReserveEikLabel();
            aSecondLineText.Set( aHeader.CLI() );
            aSecondLineDir = aHeader.CLIClipDirection( );
            aFirstLineText.Set( aHeader.Text() );
            aFirstLineDir = aHeader.TextClipDirection( );
            aTextLineNumber = 1;    // Bubble text line number
            break;
        case CBubbleManager::ENone: // flow through
        default:
            aFirstLineText.Set( NULL , 0 );
            aSecondLineText.Set( NULL , 0 );
            aTextLineNumber = 0;    // no text is used
            break;
        }

    __ASSERT_DEBUG( aTextLine1 != NULL , 
            Panic( EBMPanicErrorInResourceManager ) );
    __ASSERT_DEBUG( aTextLine2 != NULL || aTimerCost != NULL , 
            Panic( EBMPanicErrorInResourceManager ) );
    
    }

// ---------------------------------------------------------------------------
//  BubbleUtils::ChooseTextsToTwoLines
// ---------------------------------------------------------------------------
// 
void  BubbleUtils::ChooseTextsToThreeLines( const CBubbleHeader& aHeader,
            TPtrC& aFirstLineText,
            TPtrC& aSecondLineText,
            TPtrC& aThirdLineText,
            CEikLabel*& aTimerCost,
            CEikLabel*& aTextLine1,
            CEikLabel*& aTextLine2,
            CEikLabel*& aTextLine3,
            CBubbleManager& aBubbleManager,
            CBubbleManager::TPhoneClippingDirection& aFirstLineDir,
            CBubbleManager::TPhoneClippingDirection& aSecondLineDir,
            CBubbleManager::TPhoneClippingDirection& aThirdLineDir,
            TUint8& aTextLineNumber )
    {
    CBubbleManager::TPhoneCallState callState = aHeader.CallState( );
    TUint8 callFlags = aHeader.CallFlags( );

    __ASSERT_DEBUG( aTextLine1 == NULL , 
            Panic( EBMPanicErrorInResourceManager ) );
    __ASSERT_DEBUG( aTextLine2 == NULL , 
            Panic( EBMPanicErrorInResourceManager ) );
    __ASSERT_DEBUG( aTimerCost == NULL , 
            Panic( EBMPanicErrorInResourceManager ) );

    switch ( callState )
        {
        case CBubbleManager::EWaiting:        // 1.cli 2.cnap 3.text
        case CBubbleManager::EIncoming:        // 1.cli 2.cnap 3.text
            // if there is arriving data or fax call and there is
            // cli -> there is something in text field, swap texts
            aTextLine1 = aBubbleManager.ResourceManager().ReserveEikLabel();
            aTextLine2 = aBubbleManager.ResourceManager().ReserveEikLabel();
            aTextLine3 = aBubbleManager.ResourceManager().ReserveEikLabel();

            aFirstLineText.Set( aHeader.CLI() );
            aFirstLineDir = aHeader.CLIClipDirection( );
            aSecondLineText.Set( aHeader.CNAP() );
            aSecondLineDir = aHeader.CNAPClipDirection( ); 
            aThirdLineText.Set( aHeader.Text() );
            aThirdLineDir = aHeader.TextClipDirection( );
            aTextLineNumber = 3;    // Bubble text line number
            break;
        case CBubbleManager::EActive:            // 1.cli 2.cnap, 3.timer
            aTextLine1 = aBubbleManager.ResourceManager().ReserveEikLabel();
            aTextLine2 = aBubbleManager.ResourceManager().ReserveEikLabel();
            aTextLine3 = aBubbleManager.ResourceManager().ReserveEikLabel();
            aTimerCost = aBubbleManager.ResourceManager().ReserveEikLabel();
            AddTextToEikLabel( *aTimerCost , aHeader.TimerCost( ) );
            aFirstLineText.Set( aHeader.CLI() );
            aFirstLineDir = aHeader.CLIClipDirection( );
            aSecondLineText.Set( aHeader.CNAP() );
            aSecondLineDir = aHeader.CNAPClipDirection( ); 
            aThirdLineText.Set( NULL, 0 );
            aTextLineNumber = 0;    // no text is used
            break;        
        case CBubbleManager::EDisconnected:
        case CBubbleManager::EOnHold:
        case CBubbleManager::EAlerting:
        case CBubbleManager::EAlertToDisconnected:
            aTextLine1 = aBubbleManager.ResourceManager().ReserveEikLabel();
            aTextLine2 = aBubbleManager.ResourceManager().ReserveEikLabel();
            aTextLine3 = aBubbleManager.ResourceManager().ReserveEikLabel();
            aFirstLineText.Set( aHeader.CLI() );
            aFirstLineDir = aHeader.CLIClipDirection( );
            aSecondLineText.Set( aHeader.CNAP() );
            aSecondLineDir = aHeader.CNAPClipDirection( ); 
            aThirdLineText.Set( aHeader.Text() );
            aThirdLineDir = aHeader.TextClipDirection( );
            aTextLineNumber = 3;    // text is used
            break;
        case CBubbleManager::EOutgoing:
            aTextLine1 = aBubbleManager.ResourceManager().ReserveEikLabel();
            aTextLine2 = aBubbleManager.ResourceManager().ReserveEikLabel();
            aTextLine3 = aBubbleManager.ResourceManager().ReserveEikLabel();
            aFirstLineText.Set( aHeader.Text() );
            aFirstLineDir = aHeader.TextClipDirection( );
            aSecondLineText.Set( aHeader.CLI() );
            aSecondLineDir = aHeader.CLIClipDirection( );
            aThirdLineText.Set( aHeader.CNAP() );
            aThirdLineDir = aHeader.CNAPClipDirection( ); 
            aTextLineNumber = 1;    // Bubble text line number
            break;
        case CBubbleManager::ENone: // flow through
        default:
            aFirstLineText.Set( NULL , 0 );
            aSecondLineText.Set( NULL , 0 );
            aThirdLineText.Set( NULL , 0 );
            aTextLineNumber = 0;    // no text is used
            break;
        }
    
    __ASSERT_DEBUG( aTextLine1 != NULL , 
            Panic( EBMPanicErrorInResourceManager ) );
    __ASSERT_DEBUG( aTextLine2 != NULL , 
            Panic( EBMPanicErrorInResourceManager ) );
    __ASSERT_DEBUG( aTextLine3 != NULL || aTimerCost != NULL , 
            Panic( EBMPanicErrorInResourceManager ) );
    
    }
    
// ---------------------------------------------------------------------------
//  BubbleUtils::ChooseTextsToFiveLines
// ---------------------------------------------------------------------------
//
void BubbleUtils::ChooseTextsToFiveLines( 
    const CBubbleHeader& aHeader,
    TPtrC& aFirstLineText,
    TPtrC& aSecondLineText,
    TPtrC& aThirdLineText,
    CEikLabel*& aTextLine1,
    CEikLabel*& aTextLine2,
    CEikLabel*& aTextLine3,
    CEikLabel*& aTextLine4,
    CEikLabel*& aTextLine5,
    CBubbleManager& aBubbleManager,
    CBubbleManager::TPhoneClippingDirection& aFirstLineDir,
    CBubbleManager::TPhoneClippingDirection& aSecondLineDir,
    CBubbleManager::TPhoneClippingDirection& aThirdLineDir,
    TUint8& aTextLineNumber,
    TBool aThreeLinesOfText )
    {
    CBubbleManager::TPhoneCallState callState = aHeader.CallState( );
    TUint8 callFlags = aHeader.CallFlags( );

    __ASSERT_DEBUG( aTextLine1 == NULL , 
            Panic( EBMPanicErrorInResourceManager ) );
    __ASSERT_DEBUG( aTextLine2 == NULL , 
            Panic( EBMPanicErrorInResourceManager ) );
    __ASSERT_DEBUG( aTextLine3 == NULL , 
            Panic( EBMPanicErrorInResourceManager ) );
    __ASSERT_DEBUG( aTextLine4 == NULL , 
            Panic( EBMPanicErrorInResourceManager ) );
    __ASSERT_DEBUG( aTextLine5 == NULL , 
            Panic( EBMPanicErrorInResourceManager ) );
    
    switch ( callState )
        {
        case CBubbleManager::EIncoming:
            aTextLine1 = aBubbleManager.ResourceManager().ReserveEikLabel();
            aTextLine2 = aBubbleManager.ResourceManager().ReserveEikLabel();
            aTextLine3 = aBubbleManager.ResourceManager().ReserveEikLabel();
            aTextLine4 = aBubbleManager.ResourceManager().ReserveEikLabel();
            aTextLine5 = aBubbleManager.ResourceManager().ReserveEikLabel();

            aFirstLineText.Set( aHeader.CLI() );
            aFirstLineDir = aHeader.CLIClipDirection( );
            
            if ( aThreeLinesOfText )
                {
                aSecondLineText.Set( aHeader.CNAP() );
                aSecondLineDir = aHeader.CNAPClipDirection( );
                aThirdLineText.Set( aHeader.Text() );
                aThirdLineDir = aHeader.TextClipDirection( );
                aTextLineNumber = 3; // Bubble text line number    
                }
            else
                {
                aSecondLineText.Set( aHeader.Text() );
                aSecondLineDir = aHeader.TextClipDirection( );
                aTextLineNumber = 2; // Bubble text line number        
                }
            break;
            
        case CBubbleManager::EOutgoing:
            aTextLine1 = aBubbleManager.ResourceManager().ReserveEikLabel();
            aTextLine2 = aBubbleManager.ResourceManager().ReserveEikLabel();
            aTextLine3 = aBubbleManager.ResourceManager().ReserveEikLabel();
            aTextLine4 = aBubbleManager.ResourceManager().ReserveEikLabel();
            aTextLine5 = aBubbleManager.ResourceManager().ReserveEikLabel();

            if ( aThreeLinesOfText )
                {
                aFirstLineText.Set( aHeader.Text() );
                aFirstLineDir = aHeader.TextClipDirection( );
                aSecondLineText.Set(  aHeader.CLI() );
                aSecondLineDir = aHeader.CLIClipDirection( );
                aThirdLineText.Set( aHeader.CNAP() );
                aThirdLineDir = aHeader.CNAPClipDirection( );
                aTextLineNumber = 1; // Bubble text line number
                }
            else
                {
                aFirstLineText.Set( aHeader.Text() );
                aFirstLineDir = aHeader.TextClipDirection( );
                aSecondLineText.Set(  aHeader.CLI() );
                aSecondLineDir = aHeader.CLIClipDirection( );
                aTextLineNumber = 1; // Bubble text line number
                }                
            
            break;
            
        case CBubbleManager::EAlertToDisconnected:
        case CBubbleManager::EDisconnected:
        case CBubbleManager::EAlerting:         // 1.cli 2.text
            aTextLine1 = aBubbleManager.ResourceManager().ReserveEikLabel();
            aTextLine2 = aBubbleManager.ResourceManager().ReserveEikLabel();
            aTextLine3 = aBubbleManager.ResourceManager().ReserveEikLabel();
            aTextLine4 = aBubbleManager.ResourceManager().ReserveEikLabel();
            aTextLine5 = aBubbleManager.ResourceManager().ReserveEikLabel();
        
            if ( aThreeLinesOfText )
                {
                aFirstLineText.Set( aHeader.CLI() );
                aFirstLineDir = aHeader.CLIClipDirection( );
                aSecondLineText.Set( aHeader.CNAP() );
                aSecondLineDir = aHeader.CNAPClipDirection( );
                aThirdLineText.Set( aHeader.Text() );
                aThirdLineDir = aHeader.TextClipDirection( );
                aTextLineNumber = 3;    // Bubble text line number
                }
            else
                {
                aFirstLineText.Set( aHeader.CLI() );
                aFirstLineDir = aHeader.CLIClipDirection( );
                aSecondLineText.Set( aHeader.Text() );
                aSecondLineDir = aHeader.TextClipDirection( );
                aTextLineNumber = 2;    // Bubble text line number
                }                
            break;            
            
        case CBubbleManager::ENone: // flow through
        default:
            aFirstLineText.Set( NULL , 0 );
            aSecondLineText.Set( NULL , 0 );
            aThirdLineText.Set( NULL , 0 );
            aTextLineNumber = 0;    // no text is used
            break;
        }
    
    __ASSERT_DEBUG( aTextLine1 != NULL , 
            Panic( EBMPanicErrorInResourceManager ) );
    __ASSERT_DEBUG( aTextLine2 != NULL , 
            Panic( EBMPanicErrorInResourceManager ) );
    __ASSERT_DEBUG( aTextLine3 != NULL , 
            Panic( EBMPanicErrorInResourceManager ) );
    __ASSERT_DEBUG( aTextLine4 != NULL , 
            Panic( EBMPanicErrorInResourceManager ) );                        
    __ASSERT_DEBUG( aTextLine5 != NULL , 
            Panic( EBMPanicErrorInResourceManager ) );            
    }

// ---------------------------------------------------------------------------
// BubbleUtils::SetTextInLabel
// ---------------------------------------------------------------------------
//
void BubbleUtils::SetTextInLabel( 
                     const TDesC16& aText, 
                     CEikLabel*& aLabel ,
                     const CBubbleManager::TPhoneClippingDirection& aClipDir )
    {
    __ASSERT_DEBUG( aLabel , Panic( EBMPanicErrorInResourceManager ) );
    if ( aLabel == NULL )
        {
        return;
        }
    SetTextInLabel( aText, *aLabel, aClipDir );
    }

// ---------------------------------------------------------------------------
//  BubbleUtils::SetTextInLabel
//  Gets usable text from given buffer. There might be long and short version.
// ---------------------------------------------------------------------------
//
void BubbleUtils::SetTextInLabel( 
                    const TDesC16& aText, 
                    CEikLabel& aLabel ,
                    const CBubbleManager::TPhoneClippingDirection& aClipDir )
    {
    // Trivial case
    if ( aText.Length() == 0 || aText == KNullDesC )
        {
        AddTextToEikLabel( aLabel , KNullDesC );
        return;
        }

    // Find text separator (/t) if long and short text are both added to the
    // same descriptor.
    TInt separator = KErrNotFound;
    for ( TInt i = 0 ; i < aText.Length() ; i++ )
        {
        if ( aText[i] == KBubbleTextSeparator()[0] )
            {
            separator = i;
            break;
            }
        }

    TPtrC longText( NULL ,0 );
    TPtrC shortText( NULL ,0 );
    TPtrC usedText( NULL , 0 );
    if ( separator != KErrNotFound )
        {
        // The buffer is like 'LongText'+'/t'(+'ShortText').
        longText.Set( aText.Left( separator ) );
        if ( separator != aText.Length()-1 )
            {
            shortText.Set( aText.Right( aText.Length() - separator -1 ) );
            }
        }
    else
        {
        longText.Set( aText );
        }

    // does the long text fit in label?
    if ( aLabel.Font()->TextWidthInPixels( longText ) > aLabel.Size().iWidth )
        {
        //no. Use short text if there is one
        if ( shortText.Length() > 0 )
            {
            usedText.Set( shortText );
            }
        else
            {
            usedText.Set( longText );
            } 
        }
    else
        {
        //yes - so use the long text
        usedText.Set( longText );
        }

    // fit the used text in label - clip if needed
    ClipToLabel( usedText , aLabel , aClipDir );
    }

// ---------------------------------------------------------------------------
// BubbleUtils::ClipToLabel
// ---------------------------------------------------------------------------
//
void BubbleUtils::ClipToLabel( 
                     const TDesC16& aText, 
                     CEikLabel*& aLabel ,
                     const CBubbleManager::TPhoneClippingDirection& aClipDir )
    {
    __ASSERT_DEBUG( aLabel , Panic( EBMPanicErrorInResourceManager ) );
    if ( aLabel == NULL )
        {
        return;
        }
    ClipToLabel( aText, *aLabel, aClipDir );
    }

// ---------------------------------------------------------------------------
// BubbleUtils::ClipToLabel
// ---------------------------------------------------------------------------
//
void BubbleUtils::ClipToLabel( 
                    const TDesC16& aText, 
                    CEikLabel& aLabel ,
                    const CBubbleManager::TPhoneClippingDirection& aClipDir )
    {
    // Trivial case
    if ( aText.Length() == 0  || aText == KNullDesC )
        {
        AddTextToEikLabel( aLabel , KNullDesC );
        return;
        }

    // allocations not leaving
    HBufC* textBuffer = 
        HBufC::New( aText.Length() + KAknBidiExtraSpacePerLine );
    HBufC* visualBuffer = 
        HBufC::New( aText.Length() + KAknBidiExtraSpacePerLine );

    if( visualBuffer && textBuffer && aClipDir == CBubbleManager::ERight )
        {
        *textBuffer = aText;
        TPtr ptr1 = textBuffer->Des();
        TPtr ptr2 = visualBuffer->Des();
        // fit the used text in label - clip if needed
        AknBidiTextUtils::ConvertToVisualAndClip( 
            ptr1,
            ptr2, 
            *aLabel.Font(),
            aLabel.Size().iWidth,
            aLabel.Size().iWidth );
        aLabel.UseLogicalToVisualConversion( EFalse );
        AddTextToEikLabel( aLabel , ptr2 );
        }
    else
        {
        CBubbleManager::TBubbleLabelString usedText = aText;
        AknTextUtils::TClipDirection dir = 
            AknTextUtils::EClipFromBeginning;
        if ( aClipDir == CBubbleManager::ERight )
            {
            dir = AknTextUtils::EClipFromEnd;
            }
        else
            {
            ConvertToArabicIndic( usedText );
            }
        AknTextUtils::ClipToFit( 
            usedText, 
            *aLabel.Font(), 
            aLabel.Size().iWidth, 
            dir );
        AddTextToEikLabel( aLabel, usedText );
        }

    delete textBuffer;
    delete visualBuffer;
    }



// ---------------------------------------------------------------------------
// BubbleUtils::DrawMaskedImage
// ---------------------------------------------------------------------------
//
TBool BubbleUtils::DrawMaskedImage( CBitmapContext& aGc , CEikImage* aImage )
    {
    if ( aImage == NULL )
        {
        return EFalse;
        }
    
    // Bitmap is initialized
    if ( aImage->Bitmap() && aImage->Bitmap()->Handle() )
        {
        DrawMaskedImage( aGc , *aImage );
        return ETrue;
        }
    else
        {
        return EFalse;        
        }           
    }

// ---------------------------------------------------------------------------
// BubbleUtils::DrawMaskedImage
// ---------------------------------------------------------------------------
//
void BubbleUtils::DrawMaskedImage( CBitmapContext& aGc , CEikImage& aImage )
    { 
    TSize imageSize = aImage.MinimumSize();
    if ( aImage.Mask() )
        {
        aGc.BitBltMasked( 
            aImage.Rect().iTl,
            aImage.Bitmap(),
            TRect( TPoint( 0 , 0 ) , 
            TPoint( imageSize.iWidth , imageSize.iHeight ) ),
            aImage.Mask(),
            ETrue );
        }
    else
        {
        aGc.BitBlt
            ( 
            aImage.Rect().iTl, 
            aImage.Bitmap(),
            TRect( TPoint( 0 , 0 ) , 
            TPoint( imageSize.iWidth , imageSize.iHeight ) )
            );
        }
    }

// ---------------------------------------------------------------------------
// BubbleUtils::AddTextToEikLabel
// ---------------------------------------------------------------------------
//
void BubbleUtils::AddTextToEikLabel( CEikLabel& aLabel , const TDesC& aText )
    {
    if ( aText.Length() > KBubbleLabelMaxLength )
        {
        TRAPD ( error, aLabel.SetBufferReserveLengthL( aText.Length() ) );
        if ( error != KErrNone )
            {
            aLabel.SetTextL( aText.Left( KBubbleLabelMaxLength ) );
            return;
            }
        }
    aLabel.SetTextL( aText );    
    }

// ---------------------------------------------------------------------------
// BubbleUtils::AddTextToEikLabel
// ---------------------------------------------------------------------------
//
void BubbleUtils::AddTextToEikLabel( CEikLabel*& aLabel , const TDesC& aText )
    {
    if ( aLabel == NULL )
        {
        return;
        }
    AddTextToEikLabel( *aLabel, aText );
    }

// ---------------------------------------------------------------------------
// BubbleUtils::AddTextToHeader
// ---------------------------------------------------------------------------
//
void BubbleUtils::AddTextToHeader( 
                CBubbleHeader& aHeader,
                const KBubbleUtilsHeaderTextPlace aTextPlace,
                const TDesC& aTextToSet,
                const TInt aMaxLength,
                const CBubbleManager::TPhoneClippingDirection aClipDirection )
    {
    
    TPtrC buffer( NULL , 0 );
    
    if ( aTextToSet.Length() > aMaxLength )
        {
        if ( aClipDirection == CBubbleManager::ERight )
            {
            buffer.Set( aTextToSet.Left( aMaxLength ) );
            }
        else 
            {
            buffer.Set( aTextToSet.Right( aMaxLength ) );
            }
        }
    else
        {
        buffer.Set( aTextToSet );
        }
    
    switch ( aTextPlace )
        {
        case EBubbleHeaderCLI:
            aHeader.SetCLI( buffer, aClipDirection );
            break;
        case EBubbleHeaderLabel:
            aHeader.SetText( buffer, aClipDirection );
            break;
        case EBubbleHeaderTimerCost:
            aHeader.SetTimerCost( buffer );
            break;
        case EBubbleHeaderCNAP:
            aHeader.SetCNAP( buffer, aClipDirection );
            break;
        default:
            Panic( EBMPanicUnhandledSwitchCase );
            break;
            
        }
    
    }

// ---------------------------------------------------------------------------
// BubbleUtils::LayoutControl
// ---------------------------------------------------------------------------
//
void BubbleUtils::LayoutControl( CCoeControl* aControl, 
                                 const TRect& aControlParent, 
                                 const TAknWindowLineLayout& aLayout )
    {
    if ( aControl == NULL )
        {
        return;
        }
    AknLayoutUtils::LayoutControl( aControl, aControlParent, aLayout );
    }

// ---------------------------------------------------------------------------
// BubbleUtils::LayoutBackgroundImage
// ---------------------------------------------------------------------------
//
void BubbleUtils::LayoutBackgroundImage( CEikImage* aImage, 
                                 const TRect& aParent, 
                                 const TAknWindowLineLayout& aLayout )
    {
    if ( aImage == NULL )
        {
        return;
        }

    AknLayoutUtils::LayoutControl( aImage, aParent, aLayout );
    if ( aImage->Bitmap() )
        {
        AknIconUtils::SetSize( const_cast<CFbsBitmap*>( aImage->Bitmap() ), 
                               aImage->Rect().Size(),
                               EAspectRatioNotPreserved );
        }
    }

// ---------------------------------------------------------------------------
// BubbleUtils::LayoutImage
// ---------------------------------------------------------------------------
//
void BubbleUtils::LayoutImage( CEikImage* aImage, 
                               const TRect& aParent, 
                               const TAknWindowLineLayout& aLayout )
    {
    if ( aImage == NULL )
        {
        return;
        }
    AknLayoutUtils::LayoutImage( aImage,  aParent, aLayout );
    }

void BubbleUtils::LayoutImage( CEikImage* aImage, 
                               const TRect& aParent, 
                               const AknLayoutUtils::SAknLayoutControl& aLayout )
    {
    if ( aImage == NULL )
        {
        return;
        }
    AknLayoutUtils::LayoutImage( aImage,  aParent, aLayout );
    }

// ---------------------------------------------------------------------------
// BubbleUtils::LayoutLabel
// ---------------------------------------------------------------------------
//
void BubbleUtils::LayoutLabel( CEikLabel* aLabel, 
                               const TRect& aLabelParent, 
                               const TAknTextLineLayout& aLayout )
    {
    if ( aLabel == NULL )
        {
        return;
        }
    AknLayoutUtils::LayoutLabel( aLabel, aLabelParent, aLayout );

    // Check skinnig colour for the text:
    MAknsSkinInstance* skin = AknsUtils::SkinInstance();
    TRgb skinColor;
    TInt error = 
        AknsUtils::GetCachedColor( 
            skin, 
            skinColor, 
            KAknsIIDQsnTextColors,
            EAknsCIQsnTextColorsCG51 );
    if ( error == KErrNone )
        {
        // Ignore error
        TRAP_IGNORE( aLabel->OverrideColorL( EColorLabelText, skinColor ) );
        }
    }

// ---------------------------------------------------------------------------
// BubbleUtils::LayoutCustomElement
// ---------------------------------------------------------------------------
//
void BubbleUtils::LayoutCustomElement(
    CTelBubbleCustomElement* aElement, 
    const TRect& aParent, 
    const TAknWindowLineLayout& aLayout)
    {
    if ( aElement == NULL )
        {
        return;
        }
    
    if ( aElement->ControlType() == CTelBubbleCustomElement::EBubbleImage )
        {
        BubbleUtils::LayoutImage(
            static_cast<CEikImage*>( aElement->Control() ),
            aParent,
            aLayout );
        }
    else
        {
        BubbleUtils::LayoutControl(
            aElement->Control(),
            aParent,
            aLayout );
        }
    }

// ---------------------------------------------------------------------------
//  BubbleUtils::ConvertToArabicIndic
//  
// ---------------------------------------------------------------------------
//
void BubbleUtils::ConvertToArabicIndic( 
    TDes& aResult, 
    const TDesC& aSource )
    {
    __ASSERT_DEBUG( aResult.MaxLength() >= aSource.Length(), 
        User::Invariant());
    if ( aResult.MaxLength() < aSource.Length() )
        {
        return;
        }

    aResult = aSource;
    ConvertToArabicIndic( aResult );
    }

// ---------------------------------------------------------------------------
//  BubbleUtils::ConvertToArabicIndic
//  
// ---------------------------------------------------------------------------
//
void BubbleUtils::ConvertToArabicIndic( TDes& aDes )
    {
    AknTextUtils::DisplayTextLanguageSpecificNumberConversion( aDes );
    }

// ---------------------------------------------------------------------------
//  BubbleUtils::ConvertToArabicIndic
//  
// ---------------------------------------------------------------------------
//
HBufC* BubbleUtils::ConvertToArabicIndic( const TDesC& aDes )
    {
    HBufC* buffer = HBufC::New( aDes.Length() );
    if ( !buffer )
        {
        return NULL;
        }
    TPtr ptr = buffer->Des();
    ConvertToArabicIndic( ptr , aDes );
    return buffer;
    }

// ---------------------------------------------------------------------------
//  BubbleUtils::SetLayoutForCellTypePane
//  
// ---------------------------------------------------------------------------
//
void BubbleUtils::SetLayoutForCellTypePane( 
            const CBubblePlace::TPhoneBubblePlace& aBubblePlace,
            const TRect& aParent,
            CEikImage* aTypeIndication1,
            CEikImage* aTypeIndication2 )
    {
    CCoeControl* cellTypePane = new CCoeControl;
    if ( !cellTypePane )
        {
        return;
        }

    switch ( aBubblePlace )
        {
        case CBubblePlace::EBottom:
        case CBubblePlace::EBottomCnap:
            LayoutControl( cellTypePane , aParent , 
                BubbleLayout::popup_call_audio_first_window_1_elements_2() );
            break;
        case CBubblePlace::EBottomRightActive:
        case CBubblePlace::EBottomRightHeld:
        case CBubblePlace::EBottomRightCnap:
        case CBubblePlace::EBottomRightActiveTouch:
        case CBubblePlace::EBottomRightActiveCnapTouch:
            LayoutControl( cellTypePane , aParent , 
                BubbleLayout::popup_call_audio_second_window_1_elements_2() );
            break;
        case CBubblePlace::EMiddleActive:
        case CBubblePlace::EMiddleHeld:
        case CBubblePlace::EMiddleCnap:
            LayoutControl( cellTypePane , aParent , 
                BubbleLayout::popup_call_audio_first_window_2_elements_2() );
            break;
        case CBubblePlace::ETopLeft:
        case CBubblePlace::EMiddleHeldTouch:
            LayoutControl( cellTypePane , aParent , 
                BubbleLayout::popup_call_audio_first_window_4_elements_2() );
            break;
        case CBubblePlace::ETopRight:
            LayoutControl( cellTypePane , aParent , 
                BubbleLayout::popup_call_audio_second_window_3_elements_2() );
            break;
        default:
            break;
        }

    LayoutImage( aTypeIndication1 , cellTypePane->Rect() , 
        BubbleLayout::call_type_pane_elements_1() );
    LayoutImage( aTypeIndication2 , cellTypePane->Rect() , 
        BubbleLayout::call_type_pane_elements_2() );

    delete cellTypePane;
    }

// ---------------------------------------------------------------------------
//  BubbleUtils::SetLayoutFofCypheringOffIcon
//  
// ---------------------------------------------------------------------------
//
void BubbleUtils::SetLayoutFofCypheringOffIcon(
    const CBubbleHeader& aHeader,
    const CBubblePlace::TPhoneBubblePlace& aBubblePlace,
    const TRect& aParent,
    CEikImage* aCyphOffImage )
    {
    const CBubbleManager::TPhoneCallState callState = aHeader.CallState();

    const TBool connected = 
        callState == CBubbleManager::EOnHold || 
        callState == CBubbleManager::EDisconnected || 
        callState == CBubbleManager::EActive;

    switch ( aBubblePlace )
        {
        case CBubblePlace::EBottomCnap:
        case CBubblePlace::EBottom:
            if ( connected )
                {
                LayoutImage( aCyphOffImage, aParent, 
                  BubbleLayout::popup_call_audio_first_window_1_elements_3() );
                }
            else
                {
                LayoutImage( aCyphOffImage, aParent, 
                    BubbleLayout::popup_call_audio_in_window_1_elements_15() );
                }
            break;      
        case CBubblePlace::EBottomRightActive:
        case CBubblePlace::EBottomRightHeld:
        case CBubblePlace::EBottomRightCnap:
        case CBubblePlace::EBottomRightActiveTouch:
        case CBubblePlace::EBottomRightActiveCnapTouch:
            if ( connected )
                {
                LayoutImage( aCyphOffImage, aParent, 
                  BubbleLayout::popup_call_audio_second_window_1_elements_3() );
                }
            else
                {
                LayoutImage( aCyphOffImage, aParent, 
                  BubbleLayout::popup_call_audio_out_window_1_elements_15() );
                }
            break;
        case CBubblePlace::EMiddleActive:
        case CBubblePlace::EMiddleHeld:
        case CBubblePlace::EMiddleCnap:
            if ( connected )
                {
                LayoutImage( aCyphOffImage, aParent, 
                  BubbleLayout::popup_call_audio_first_window_2_elements_15() );
                }
            else
                {
                LayoutImage( aCyphOffImage, aParent, 
                    BubbleLayout::popup_call_audio_in_window_2_elements_15() );
                }
            break;
        case CBubblePlace::ETopLeft:
        case CBubblePlace::EMiddleHeldTouch:
            if ( connected )
                {
                LayoutImage( aCyphOffImage, aParent, 
                  BubbleLayout::popup_call_audio_first_window_4_elements_3() );
                }
            else
                {
                LayoutImage( aCyphOffImage, aParent, 
                    BubbleLayout::popup_call_audio_wait_window_3_elements_3() );
                }
            break;
        case CBubblePlace::ETopRight:
            if ( connected )
                {
                LayoutImage( aCyphOffImage, aParent, 
                  BubbleLayout::popup_call_audio_second_window_3_elements_3() );
                }
            else
                {
                LayoutImage( aCyphOffImage, aParent, 
                    BubbleLayout::popup_call_audio_out_window_2_elements_15() );
                }
            break;
        case CBubblePlace::EBottomText:
            if ( connected )
                {
                LayoutImage( aCyphOffImage, aParent, 
                  BubbleLayout2::popup_call2_audio_in_ciphering_icon( 0 ) );
                }
            break;            
        default:
            break;
        }
    }

// ---------------------------------------------------------------------------
//  BubbleUtils::PlaceThumbnail
//  
// ---------------------------------------------------------------------------
//
void BubbleUtils::PlaceThumbnail(
    const CBubblePlace::TPhoneBubblePlace& aBubblePlace,
    CEikImage* aImage , 
    TBubbleLayoutRect& aShadow,
    const TRect& aParentRect,
    CFbsBitmap* aThumbNailBitmap )
    {
    if ( !( aImage && aImage->Bitmap() ) )
        {
        return;
        }
    
    TAknLayoutRect thumbnailLayoutRect;
    TAknLayoutRect shadowLayoutRect;
    TAknWindowLineLayout thumbnailShadowLine;
    
    // Bitmap is resized separately, not via AknLayoutUtils::LayoutImage()
    aImage->SetPicture( NULL );

    // wrong layout data within AppLayout::Incoming_call_pop_up_window_elements_Line_14()
    switch ( aBubblePlace )
        {
        case CBubblePlace::EBottom:
        case CBubblePlace::EBottomCnap:            
            AknLayoutUtils::LayoutImage( aImage, aParentRect, 
                AppLayout::Incoming_call_pop_up_window_elements_Line_12() );
            thumbnailLayoutRect.LayoutRect( aParentRect, 
                AppLayout::Incoming_call_pop_up_window_elements_Line_12() );
            thumbnailShadowLine = 
                AppLayout::Incoming_call_pop_up_window_elements_Line_11();
            shadowLayoutRect.LayoutRect( aParentRect, thumbnailShadowLine );
            break;
        case CBubblePlace::EBottomRightActive:
        case CBubblePlace::EBottomRightHeld:
        case CBubblePlace::EBottomRightCnap:
        case CBubblePlace::EBottomRightActiveTouch:
        case CBubblePlace::EBottomRightActiveCnapTouch:
            AknLayoutUtils::LayoutImage( aImage, aParentRect, 
                AppLayout::Outgoing_call_pop_up_window_elements__held__Line_12() );
            thumbnailLayoutRect.LayoutRect( aParentRect, 
                AppLayout::Outgoing_call_pop_up_window_elements__held__Line_12() );
            thumbnailShadowLine = 
                AppLayout::Outgoing_call_pop_up_window_elements__held__Line_11();
            shadowLayoutRect.LayoutRect( aParentRect, thumbnailShadowLine );
            break;
        case CBubblePlace::EMiddleActive:
        case CBubblePlace::EMiddleHeld:
        case CBubblePlace::EMiddleCnap:
            AknLayoutUtils::LayoutImage( aImage, aParentRect, 
                AppLayout::Incoming_call_pop_up_window_elements__NE__Line_12() );
            thumbnailLayoutRect.LayoutRect( aParentRect, 
                AppLayout::Incoming_call_pop_up_window_elements__NE__Line_12() );
            thumbnailShadowLine = 
                AppLayout::Incoming_call_pop_up_window_elements__NE__Line_11();
            shadowLayoutRect.LayoutRect( aParentRect, thumbnailShadowLine );
            break;
        default:
            break;
        }

    AknIconUtils::SetSize( aThumbNailBitmap, thumbnailLayoutRect.Rect().Size(), 
                           EAspectRatioPreservedAndUnusedSpaceRemoved );
    aImage->SetPicture( aThumbNailBitmap );
    aShadow.SetColor( AKN_LAF_COLOR(221) );
    // aShadow.SetColor( shadowLayoutRect.Color() ); 
    // int leftGap = shadowLayoutRect.Rect().iTl.iX - thumbnailLayoutRect.Rect().iTl.iX;
    // int topGap = shadowLayoutRect.Rect().iTl.iY - thumbnailLayoutRect.Rect().iTl.iY;
    int leftGap = 1;
    int topGap = 1;
    aShadow.SetRect( aImage->Position() + TPoint( leftGap, topGap ), 
                     aImage->MinimumSize() );

    }

// ---------------------------------------------------------------------------
// BubbleUtils::IsCustomImageWidth
// ---------------------------------------------------------------------------
//
TBool BubbleUtils::IsCustomizedImage( const TRect& aParent, const TInt aWidth )
    {
    TInt type = GetThumbnailImageType( aParent, aWidth );
    TBool ret = EFalse;
    if( ( type == 0 ) && ( aWidth != 0 ) ) // Customized
        {
        ret = ETrue;
        }
    return ret;
    }

// ---------------------------------------------------------------------------
// BubbleUtils::LabelExtent
// ---------------------------------------------------------------------------
//
void BubbleUtils::LabelExtent( CEikLabel* aLabel, 
                              const TRect& aParent, 
                              const TInt aWidth )
    {
    if ( aLabel == NULL )
        {
        return;
        }

    if( BubbleUtils::IsCustomizedImage( aParent, aWidth ) )
        {    
        TInt width = aLabel->Size().iWidth - aWidth;
        if( width > 0 )
            {
            TRect rect = aLabel->Rect();

            if( AknLayoutUtils::LayoutMirrored() ) 
                {
                rect.iTl.iX += aWidth;
                }
            rect.SetWidth( width );

            aLabel->SetRect( rect );
            }
        }
    }

// ---------------------------------------------------------------------------
// BubbleUtils::GetThumbnailImageType
// ---------------------------------------------------------------------------
//
TInt BubbleUtils::GetThumbnailImageType( const TRect& /*aParent*/, 
                                         const TInt aWidth )
    {
    /* Scalable UI uses only one image size.
    TAknLayoutRect rectVGAturned;
    rectVGAturned.LayoutRect( aParent, AppLayout::VGA_turned_90() );
    const TInt widthVGAturned = rectVGAturned.Rect().Width();

    TAknLayoutRect rectCIFturned;
    rectCIFturned.LayoutRect( aParent, AppLayout::CIF_turned_90() );
    const TInt widthCIFturned = rectCIFturned.Rect().Width();

    TAknLayoutRect rectVGAnormal;
    rectVGAnormal.LayoutRect( aParent, AppLayout::VGA() );
    const TInt widthVGAnormal = rectVGAnormal.Rect().Width();
    
    TAknLayoutRect rectCIFnormal;
    rectCIFnormal.LayoutRect( aParent, AppLayout::CIF() );
    const TInt widthCIFnormal = rectCIFnormal.Rect().Width();
    
    TAknLayoutRect rectCommmunicator;
    rectCommmunicator.LayoutRect( aParent, 
        AppLayout::Communicator_personal_image() );
    const TInt widthCommmunicator = rectCommmunicator.Rect().Width();

    TInt ret;
    if( aWidth == widthVGAturned )
        {
        ret = 1;
        }
    else if( aWidth == widthCIFturned )
        {
        ret = 2;
        }
    else if( aWidth == widthVGAnormal || aWidth == widthCommmunicator ) 
        {
        ret = 3;
        }
    else if( aWidth == widthCIFnormal )
        {
        ret = 4;
        }
    else // no image.
        {
        ret = 0; 
        }*/

    TInt ret(0); // no thumbnail       
    if ( aWidth > 0 )
        {
        // see First_call_pop_up_window_texts__one_call__Line_1 in
        // Adaptation_Layer_AppLayout_Elaf
        ret = 4;    
        }
    
    return ret;
    }

// ---------------------------------------------------------------------------
// BubbleUtils::BubbleIconFileName
// ---------------------------------------------------------------------------
//
void BubbleUtils::BubbleIconFileName( TDes& aFileName )
    {
    aFileName = KBMMbmZDrive;
    aFileName.Append( KDC_APP_BITMAP_DIR );
    aFileName.Append( KBMBitmapFile );
    aFileName.ZeroTerminate();
    }

// ---------------------------------------------------------------------------
// BubbleUtils::BubbleResourceFileName
// ---------------------------------------------------------------------------
//
void BubbleUtils::BubbleResourceFileName( TDes& aFileName )
    {
    aFileName = KBMMbmZDrive;
    aFileName.Append( KDC_RESOURCE_FILES_DIR );
    aFileName.Append( KBMResourcesFile );
    aFileName.ZeroTerminate();
    }

// ----------------------------------------------------------------------------
// BubbleUtils::::DrawBackgroundRect
// ----------------------------------------------------------------------------
//    
void BubbleUtils::DrawBackgroundRect( CBitmapContext& aGc,
                                      const TRect& aRect )
    {
    TRect bgRect( aRect );
    bgRect.Shrink( aRect.Width() / 48, aRect.Height() / 24 );
    aGc.SetBrushStyle( CGraphicsContext::ESolidBrush ); 
    aGc.SetPenStyle( CGraphicsContext::ENullPen ); 

    bgRect.Move(2,2);
    aGc.SetBrushColor( AKN_LAF_COLOR(221) );
    aGc.DrawRect( bgRect );
    bgRect.Move(-2,-2);
    aGc.SetBrushColor( AKN_LAF_COLOR(0) );
    aGc.SetPenStyle( CGraphicsContext::ESolidPen ); 
    aGc.SetPenColor( AKN_LAF_COLOR(217) );
    aGc.DrawRect( bgRect );

    aGc.SetBrushStyle( CGraphicsContext::ENullBrush );    
    }

// ----------------------------------------------------------------------------
// TBubbleLayoutRect::TBubbleLayoutRect
// ----------------------------------------------------------------------------
//
TBubbleLayoutRect::TBubbleLayoutRect() {}

// ----------------------------------------------------------------------------
// TBubbleLayoutRect::SetRect
// ----------------------------------------------------------------------------
//
void TBubbleLayoutRect::SetRect( const TPoint& aPoint, const TSize& aSize ) 
    {
    iRect.SetRect( aPoint, aSize );
    }

// ----------------------------------------------------------------------------
// TBubbleLayoutRect::Rect
// ----------------------------------------------------------------------------
//
TRect TBubbleLayoutRect::Rect() const
    {
    return iRect;
    }

// ----------------------------------------------------------------------------
// TBubbleLayoutRect::SetColor
// ----------------------------------------------------------------------------
//
void TBubbleLayoutRect::SetColor( const TRgb aColor ) 
    {
    iColor = aColor;
    }

// ----------------------------------------------------------------------------
// TBubbleLayoutRect::Color
// ----------------------------------------------------------------------------
//
TRgb TBubbleLayoutRect::Color() const
    {
    return iColor;
    }

// ---------------------------------------------------------------------------
// BubbleUtils::PrepareBubbleImageL
// ---------------------------------------------------------------------------
//    
void BubbleUtils::PrepareBubbleImageL(
    const TAknsItemID& aFrameID,
    const TRect& aOuterRect,
    const TRect& aInnerRect,    
    CEikImage*& aBubble )
    {
    ///////////////////////////////////////////////////////
    // Create bitmap
    ///////////////////////////////////////////////////////
    CFbsBitmap* bitmap = new(ELeave) CFbsBitmap;
    CleanupStack::PushL( bitmap );
    
    // create bitmap to target size
    AknIconConfig::TPreferredDisplayMode mode;
    AknIconConfig::PreferredDisplayMode( mode, 
                                         AknIconConfig::EImageTypeIcon);
    User::LeaveIfError( bitmap->Create( aOuterRect.Size(), 
                                        mode.iBitmapMode ) );
    
    // create context
    CFbsBitmapDevice* bitmapDev = CFbsBitmapDevice::NewL( bitmap );
    CleanupStack::PushL( bitmapDev );
    CFbsBitGc* bitmapCtx;
    User::LeaveIfError( bitmapDev->CreateContext( bitmapCtx ) );
    CleanupStack::PushL( bitmapCtx ); 
    
    // draw frame
    MAknsSkinInstance* skin = AknsUtils::SkinInstance();
    
    AknsDrawUtils::DrawFrame( skin,
                              *bitmapCtx,
                              aOuterRect,
                              aInnerRect,
                              aFrameID,
                              KAknsIIDDefault,
                              KAknsDrawParamRGBOnly );

    aBubble->SetBitmap( bitmap );
    
    CleanupStack::PopAndDestroy( 2, bitmapDev );
    CleanupStack::Pop( bitmap );
    
    ///////////////////////////////////////////////////////
    // Create mask
    ///////////////////////////////////////////////////////
    CFbsBitmap* mask = new(ELeave) CFbsBitmap;
    CleanupStack::PushL( mask );
    
    // create bitmap to target size
    User::LeaveIfError( mask->Create( aOuterRect.Size(), 
                                      mode.iMaskMode ) );
    
    // create context
    CFbsBitmapDevice* maskDev = CFbsBitmapDevice::NewL( mask );
    CleanupStack::PushL( maskDev );
    CFbsBitGc* maskCtx;
    User::LeaveIfError( maskDev->CreateContext( maskCtx ) );
    CleanupStack::PushL( maskCtx ); 
    
    // draw frame
    AknsDrawUtils::DrawFrame( skin,
                              *maskCtx,
                              aOuterRect,
                              aInnerRect,
                              aFrameID,
                              KAknsIIDDefault,
                              KAknsSDMAlphaOnly );

    aBubble->SetMask( mask );
    
    CleanupStack::PopAndDestroy( 2, maskDev );
    CleanupStack::Pop( mask );
    }

// ---------------------------------------------------------------------------
// BubbleUtils::AddTransparencyToBubbleImageL
// ---------------------------------------------------------------------------
//    
void BubbleUtils::AddTransparencyToBubbleImageL(
    const TAknsItemID& aFrameID,
    const TRect& aOuterRect,
    const TRect& aInnerRect,    
    CEikImage*& aBubble )
    {
    
    const CFbsBitmap* currentMask = aBubble->Mask();
    TSize maskSize( currentMask->SizeInPixels() );
    TRect rect( maskSize );
    
    // create transparency frame
    CFbsBitmap* transparency = new(ELeave) CFbsBitmap;
    CleanupStack::PushL( transparency );
    User::LeaveIfError( transparency->Create( aOuterRect.Size(), 
                                              EGray256 ) );
    CFbsBitmapDevice* transparencyDev = CFbsBitmapDevice::NewL( transparency );
    CleanupStack::PushL( transparencyDev );
    CFbsBitGc* transparencyCtx;
    User::LeaveIfError( transparencyDev->CreateContext( transparencyCtx ) );
    CleanupStack::PushL( transparencyCtx );
    transparencyCtx->SetBrushColor( KRgbDarkGray );
    transparencyCtx->SetBrushStyle( CGraphicsContext::ESolidBrush );
    transparencyCtx->DrawRect( rect );

    CleanupStack::PopAndDestroy( 2, transparencyDev );
            
    // create solid black mask
    CFbsBitmap* solidBlack = new(ELeave) CFbsBitmap;
    CleanupStack::PushL( solidBlack );
    User::LeaveIfError( solidBlack->Create( maskSize, EGray256 ) );
    CFbsBitmapDevice* dev = CFbsBitmapDevice::NewL( solidBlack );
    CleanupStack::PushL( dev );
    CFbsBitGc* gc;
    User::LeaveIfError( dev->CreateContext( gc ) ); 
    CleanupStack::PushL( gc );
    
    gc->SetBrushColor( KRgbBlack );
    gc->SetBrushStyle( CGraphicsContext::ESolidBrush );
    gc->DrawRect( rect );
    
    // blit original mask on the black mask using transparency mask
    gc->BitBltMasked( TPoint(0,0), 
                      currentMask,
                      TRect( currentMask->SizeInPixels() ), 
                      transparency, 
                      ETrue );
 
    CleanupStack::PopAndDestroy( 2, dev);
    
    CleanupStack::Pop(solidBlack);
    aBubble->SetMask( solidBlack );
        
    CleanupStack::PopAndDestroy( transparency );
    delete currentMask;
    }

// ---------------------------------------------------------------------------
// BubbleUtils::PrepareCallObjectToBubbleImageL
// ---------------------------------------------------------------------------
//    
void BubbleUtils::PrepareCallObjectToBubbleImageL(
    const CFbsBitmap* aCOImage,
    const CFbsBitmap* aCOImageMask,
    const TRect& /*aCallObjectRect*/,
    const TAknsItemID& aFrameID,
    const TRect& aOuterRect,
    const TRect& aInnerRect,
    CEikImage*& aBubble,
    TBool aDimmed )
    {
    const CFbsBitmap* bubbleBitmap = aBubble->Bitmap();
    TSize callObjectSize( aCOImage->SizeInPixels() );
    
    // set source rectangle, clip from center if bigger than target
    TRect srcRect( aCOImage->SizeInPixels() );
    TInt clipX = ( callObjectSize.iWidth > aOuterRect.Width() ) ? 
                 ( callObjectSize.iWidth - aOuterRect.Width() ) : 0;
    TInt clipY = ( callObjectSize.iHeight > aOuterRect.Height() ) ? 
                 ( callObjectSize.iHeight - aOuterRect.Height() ) : 0;
    srcRect.Shrink( clipX / 2, clipY / 2 );
    
    // set offset/alignment for image
    TPoint offset(0,0);
    if ( ( callObjectSize.iWidth > aOuterRect.Width() ) )
        {
        // Not usual situation, only if smaller image is under scaling.
        // Set horizontally centered.
        offset.iX = (aOuterRect.Width() - callObjectSize.iWidth) / 2;
        }
    else if ( !Layout_Meta_Data::IsMirrored() )
        {
        // Align image horizontally right
        offset.iX = ( callObjectSize.iWidth < aOuterRect.Width() ) ? 
                    (aOuterRect.Width() - callObjectSize.iWidth )  : 0;
        }
    // vertically centered
    offset.iY = (aOuterRect.Height() - callObjectSize.iHeight) / 2;
    
    // create gradient mask
    CFbsBitmap* gradientMask = new(ELeave) CFbsBitmap;
    CleanupStack::PushL( gradientMask );
    // mask has to be in same size than call object
    User::LeaveIfError( gradientMask->Create( callObjectSize, 
                                              EGray256 ) );
    CFbsBitmapDevice* gradientMaskDev = CFbsBitmapDevice::NewL( gradientMask );
    CleanupStack::PushL( gradientMaskDev );
    CFbsBitGc* gradientMaskCtx;
    User::LeaveIfError( gradientMaskDev->CreateContext( gradientMaskCtx ) );
    CleanupStack::PushL( gradientMaskCtx );
    MAknsSkinInstance* skin = AknsUtils::SkinInstance();
    // initialize mask to black color
    gradientMaskCtx->SetBrushColor( AKN_LAF_COLOR(215) ); // black
    gradientMaskCtx->SetBrushStyle( CGraphicsContext::ESolidBrush );
    gradientMaskCtx->SetPenStyle( CGraphicsContext::ENullPen );
    gradientMaskCtx->DrawRect( callObjectSize );
    // shift origin
    gradientMaskCtx->SetOrigin( -offset ); 
    AknsDrawUtils::DrawFrame( skin,
                              *gradientMaskCtx,
                              aOuterRect,
                              aInnerRect,
                              aFrameID,
                              KAknsIIDDefault,
                              KAknsDrawParamDefault );

    CleanupStack::PopAndDestroy( 2, gradientMaskDev );
    
    if ( aCOImageMask )
        {
        // combine image and gradient mask
        CFbsBitmap* combinedMask = CreateCombinedMaskL(
            gradientMask, aCOImageMask );
        // switch to combined mask
        CleanupStack::PopAndDestroy(gradientMask);
        gradientMask = combinedMask;
        CleanupStack::PushL( gradientMask );            
        }
    
    // blit image to bubble through gradient mask
    CFbsBitmapDevice* bubbleDev = 
        CFbsBitmapDevice::NewL( const_cast<CFbsBitmap*>(bubbleBitmap) );
    CleanupStack::PushL( bubbleDev );
    CFbsBitGc* bubbleGc;
    User::LeaveIfError( bubbleDev->CreateContext( bubbleGc ) ); 
    CleanupStack::PushL( bubbleGc );
    
    
    TPoint blitOffset; 
    blitOffset.iX = ( offset.iX < 0 ) ? 0 : offset.iX;
    blitOffset.iY = ( offset.iY < 0 ) ? 0 : offset.iY;
    
    bubbleGc->SetFaded( aDimmed );
    bubbleGc->BitBltMasked( aOuterRect.iTl + blitOffset, 
                            aCOImage,
                            srcRect, 
                            gradientMask, 
                            ETrue );

    CleanupStack::PopAndDestroy( 3, gradientMask );
    }

// ---------------------------------------------------------------------------
// BubbleUtils::LayoutCallTypeIndicators
// ---------------------------------------------------------------------------
//    
void BubbleUtils::LayoutCallTypeIndicators( 
    const TRect& aParent,
    const TAknWindowLineLayout& aCallTypePane,
    CEikImage*   aTypeIndication1,
    CEikImage*   aTypeIndication2 )    
    {
    if ( !aTypeIndication1 && !aTypeIndication2 )
        {
        return;            
        }
    
    // split pane
    TAknLayoutRect callTypePaneRect;
    callTypePaneRect.LayoutRect( aParent,
                                 aCallTypePane );
        
    TRect paneRect = callTypePaneRect.Rect();
    TInt  paneWidth = paneRect.Width();
    TInt  paneHeight = paneRect.Height();
    TSize indSize( paneWidth/2, paneHeight );
        
    TRect indRect1 = TRect( TPoint( paneRect.iTl ), indSize );
                                
    TRect indRect2 = TRect( TPoint( paneRect.iTl.iX + ( paneWidth / 2 ),
                                    paneRect.iTl.iY ), indSize );                
    
    if ( Layout_Meta_Data::IsMirrored() )
        {
        // switch positions
        TRect tmp = indRect2;
        indRect2 = indRect1;
        indRect1 = tmp;
        }
    
    if ( aTypeIndication1 )
        {
        aTypeIndication1->SetRect( indRect1 );
        }
    
    if ( aTypeIndication2 )        
        {
        if ( aTypeIndication1 )
            {
            aTypeIndication2->SetRect( indRect2 );    
            }
        else
            {
            aTypeIndication2->SetRect( indRect1 );    
            }            
        }
    }

// ---------------------------------------------------------------------------
// BubbleUtils::PrepareCallObjectImageL
// ---------------------------------------------------------------------------
//    
void BubbleUtils::PrepareCallObjectImageL( 
    CBubbleHeader& aHeader,
    const TSize& aSize )
    {
    CFbsBitmap* bitmap = aHeader.CallObjectImage();
     
    if ( bitmap )
        {
        CBubbleHeader::TBMCallObjectImageType imageType = 
            aHeader.CallObjectImageType();
        
        if ( imageType == CBubbleHeader::EThemeImage &&
             AknIconUtils::IsMifIcon( bitmap ) )
            {
            // This does nothing, if icon is already in requested size.
            TInt err =  AknIconUtils::SetSize( 
                            bitmap,
                            aSize,
                            EAspectRatioPreservedSlice );
            
            if ( err != KErrNone ) // out of memory
                {
                aHeader.SetCallObjectImage( NULL );
                aHeader.SetCallObjectImageMask( NULL );
                User::Leave( err );
                }
            }
        else if ( imageType == CBubbleHeader::EGalleryImage &&
                  aHeader.TnBitmap() )
            {
            TSize thumbnailSize = aHeader.TnBitmap()->SizeInPixels();
            
            // Allow one pixel rounding inaccuracy
            if ( thumbnailSize.iWidth  >= (aSize.iWidth - 1) &&
                 thumbnailSize.iHeight >= (aSize.iHeight - 1) )
                {
                aHeader.SwitchToThumbnailImage();    
                }
            }
        }
    }

// ---------------------------------------------------------------------------
// BubbleUtils::CreateCombinedMaskL
// ---------------------------------------------------------------------------
//
CFbsBitmap* BubbleUtils::CreateCombinedMaskL(
    const CFbsBitmap* aPrimaryMask,
    const CFbsBitmap* aSecondaryMask )
    {
    __ASSERT_DEBUG( aPrimaryMask != NULL, 
        Panic( EBMPanicErrorInResourceManager ) );
    __ASSERT_DEBUG( aSecondaryMask != NULL, 
        Panic( EBMPanicErrorInResourceManager ) );
        
    TSize size = aPrimaryMask->SizeInPixels();
    
    // create combined mask
    CFbsBitmap* combinedMask = new(ELeave) CFbsBitmap;
    CleanupStack::PushL( combinedMask );
    User::LeaveIfError( combinedMask->Create( size, 
                                              EGray256 ) );
    
    CFbsBitmapDevice* combinedMaskDev = 
        CFbsBitmapDevice::NewL( combinedMask );
    CleanupStack::PushL( combinedMaskDev );
    CFbsBitGc* combinedMaskCtx;
    User::LeaveIfError( combinedMaskDev->CreateContext( 
        combinedMaskCtx ) );
    CleanupStack::PushL( combinedMaskCtx ); 
        
    // initialize mask to black color
    combinedMaskCtx->SetBrushColor( AKN_LAF_COLOR(215) ); // black
    combinedMaskCtx->SetBrushStyle( CGraphicsContext::ESolidBrush );
    combinedMaskCtx->SetPenStyle( CGraphicsContext::ENullPen );
    combinedMaskCtx->DrawRect( size );
        
    // blit secondary mask through primary mask
    combinedMaskCtx->BitBltMasked( TPoint(0,0), 
                                   aSecondaryMask,
                                   size, 
                                   aPrimaryMask, 
                                   ETrue );                                 
        
    CleanupStack::PopAndDestroy( 2, combinedMaskDev );
    CleanupStack::Pop( combinedMask );
    
    return combinedMask;
    }

// End of File
