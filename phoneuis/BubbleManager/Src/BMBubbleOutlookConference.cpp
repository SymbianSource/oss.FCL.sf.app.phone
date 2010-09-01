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
* Description:  Outlook Conference
*
*/


// INCLUDE FILES
#include    "BMBubbleManager.h" //for enumerations
#include    "BMBubbleOutlookConference.h"
#include    "BMBubbleImageManager.h"
#include    "BMResourceManager.h"
#include    "BMConfHeader.h"
#include    "BMLayout.h"
#include    "BMUtils.h"
#include    "BMPanic.h"
#include    "BMLayout2.h"
#include    "BMLayout4.h"

#include    <eiklabel.h>  
#include    <eikimage.h>
#include    <eikenv.h>
#include    <AknsUtils.h>
#include    <AknsDrawUtils.h>

// ========================= MEMBER FUNCTIONS ================================

CBubbleOutlookConference::CBubbleOutlookConference( 
    CBubbleManager& aBubbleManager ) 
    : CBubbleOutlook( aBubbleManager )
    {
    }

// ---------------------------------------------------------------------------
// CBubbleOutlookConference::ConstructL
// ---------------------------------------------------------------------------
//

void CBubbleOutlookConference::ConstructL( const TUint8& aCallAmount )
    {
    iPaneAmount = aCallAmount;
    iHeader = NULL;

    // create panes for each call
    CBubbleConfPane* pane;
    iConfPanes = new ( ELeave ) CArrayPtrFlat<CBubbleConfPane>( iPaneAmount );    
    iConfPanes->SetReserveL( iPaneAmount );

    for ( TUint8 i = 0 ; i < iPaneAmount ; i++ )
        {
        pane = new( ELeave ) CBubbleConfPane;
        CleanupStack::PushL( pane );
        pane->SetContainerWindowL( *this );
        pane->ConstructL( iBubbleManager.IsTouchCallHandling() );
        iConfPanes->InsertL( i , pane );
        CleanupStack::Pop( pane );
        }

    // Create space for headers 
    iCalls = new( ELeave ) CArrayPtrFlat<CBubbleCallHeader>( iPaneAmount );
    iCalls->SetReserveL( iPaneAmount );


    CBubbleOutlook::ConstructL();
    ActivateL();
    }

// Destructor
CBubbleOutlookConference::~CBubbleOutlookConference()
    {

    if ( iCalls && iConfPanes )
        {
        Reset();
        }

    if ( iCalls )
        {
        iCalls->Reset( );
        delete iCalls;
        }
 
    if ( iConfPanes )
        {
        iConfPanes->ResetAndDestroy() ;
        delete iConfPanes;
        }
    
    iHeader = NULL;
    }


// ---------------------------------------------------------------------------
// CBubbleOutlookConference::Reset
//
//  
// ---------------------------------------------------------------------------
//
void CBubbleOutlookConference::Reset()
    {
    iBubbleManager.ResourceManager().ReleaseEikImage( iBubble );
    iBubbleManager.ResourceManager().ReleaseEikLabel( iTimerCost );
    for ( TUint8 i = 0 ; i < iPaneAmount ; i++ )
        {
        iBubbleManager.ResourceManager().ReleaseEikImage( 
            iConfPanes->At( i )->iCallIndication );
        iBubbleManager.ResourceManager().ReleaseEikImage( 
            iConfPanes->At( i )->iCyphOffImage );
        iBubbleManager.ResourceManager().ReleaseEikLabel( 
            iConfPanes->At( i )->iTextLine );
        iConfPanes->At( i )->Reset( );
        }

    iCalls->Delete( 0 , iCalls->Count() );
    
    iHeader = NULL;
    }


// ---------------------------------------------------------------------------
// CBubbleOutlookConference::ReadBubbleHeader
//
//  
// ---------------------------------------------------------------------------
//
void CBubbleOutlookConference::ReadBubbleHeader( CBubbleHeader& aHeader )
    {
    if ( !aHeader.IsUsed() || !aHeader.IsConference() )
        {
        return;
        }

    iHeader = static_cast< CBubbleConfHeader* >( &aHeader ); 

    // get call's info. Function returns calls in drawing order.
    iHeader->GetRows( *iCalls );
    __ASSERT_ALWAYS( iCalls->Count( ) == iPaneAmount, 
                                    Panic( EBMPanicInvalidNumberOfHeaders ) );


    iBubble = iBubbleManager.ResourceManager().ReserveEikImage( ETrue );

    if ( !iCallObjectDisplay )
        {
        // Get bubble graphics
        GetCall1BubbleBitmaps();    
        }

    // Get call indicator for each pane.
    TUint32 flags = 0;  
    CBubbleManager::TPhoneCallState state;

    TInt indicationPicture = KErrNotFound;
    TInt indicationPictureMask =KErrNotFound;

    for ( TUint8 i = 0 ; i < iPaneAmount ; i++ )
        {
        flags = iCalls->At( i )->CallFlags( );
        state = iCalls->At( i )->CallState( );
        CEikImage*& image = iConfPanes->At( i )->CallIndicationHandle( );

        indicationPicture = KErrNotFound;
        indicationPictureMask = KErrNotFound;

        if ( state == CBubbleManager::EDisconnected )
            {
            indicationPicture = EQgn_indi_call_disconn_conf;
            indicationPictureMask = EQgn_indi_call_disconn_conf_mask;
            }
        else if ( state == CBubbleManager::EActive ) 
            {
            indicationPicture = EQgn_indi_call_active_conf;
            indicationPictureMask = EQgn_indi_call_active_conf_mask;
            }
        else 
            {
            indicationPicture = EQgn_indi_call_held_conf;
            indicationPictureMask = EQgn_indi_call_held_conf_mask;
            }

        if ( indicationPicture != KErrNotFound )
            {
            image = iBubbleManager.ResourceManager().ReserveEikImage();
            if ( indicationPictureMask == KErrNotFound )
                {
                iBubbleManager.ImageManager().SetBitmapToImage( 
                    image, (TBMIcons) indicationPicture );
                }
            else
                {
                iBubbleManager.ImageManager().SetBitmapToImage( 
                    image, 
                    (TBMIcons) indicationPicture , 
                    (TBMIcons) indicationPictureMask);
                }
            }

        // Cyph off
        if ( flags&CBubbleManager::ENoCiphering )
            {
            iConfPanes->At( i )->iCyphOffImage = 
                iBubbleManager.ResourceManager().ReserveEikImage();
            iBubbleManager.ImageManager().SetBitmapToImage( 
                    iConfPanes->At( i )->iCyphOffImage, 
                    EQgn_indi_call_cyphering_off,
                    EQgn_indi_call_cyphering_off_mask );
            }

        // set CLI to pane
        if ( ( iCalls->At( i )->ParticipantListCLI() == 
              CBubbleManager::EParticipantListCNAP ) &&
             iCalls->At( i )->CNAP().Length() )
            {
            // Phonenumber
            iConfPanes->At( i )->iTextLine = 
                iBubbleManager.ResourceManager().ReserveEikLabel();
            iConfPanes->At( i )->SetText( iCalls->At( i )->CNAP(), 
                                          iCalls->At( i )->CNAPClipDirection( ) );    
            }
        else            
            {
            // CLI text
            iConfPanes->At( i )->iTextLine = 
                iBubbleManager.ResourceManager().ReserveEikLabel();
            iConfPanes->At( i )->SetText( iCalls->At( i )->CLI(), 
                                          iCalls->At( i )->CLIClipDirection( ) );
            }
                                      
        iConfPanes->At( i )->iCallObjectDisplay = iCallObjectDisplay;
        }

    // get highlight
    for ( TUint8 t = 0 ; t < iConfPanes->Count() ; t++ )
        {
        iConfPanes->At( t )->SetHighlight( EFalse );
        }

    if ( iHeader->Highlight() != 0 )
        {
        iConfPanes->At( iHeader->Highlight() - 1 )->SetHighlight( ETrue );
        }
    
    // place timer/cost text to label
    if ( !iBubbleManager.IsTouchCallHandling() )
        {
        iTimerCost = iBubbleManager.ResourceManager().ReserveEikLabel();
        BubbleUtils::ClipToLabel( 
            iHeader->TimerCost( ) , iTimerCost , CBubbleManager::ERight );
            
        }
    }

// ---------------------------------------------------------------------------
// CBubbleOutlookConference::SizeChanged
// called by framwork when the view size is changed
// Works with all the conference situations.
// ---------------------------------------------------------------------------
//
void CBubbleOutlookConference::SizeChanged()
    {
    if( !iHeader )
        {
        return;
        }
        
    AknsUtils::RegisterControlPosition( this );

    if ( iBubbleManager.IsTouchCallHandling() )
        {
        TRAPD( err, DoCall4LayoutL() );
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
    else if ( !iCallObjectDisplay )
        {
        DoCall1Layout();    
        }
    else
        {
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

    }

// ---------------------------------------------------------------------------
// CBubbleOutlookConference::CountComponentControls
//
//  
// ---------------------------------------------------------------------------
//
TInt CBubbleOutlookConference::CountComponentControls() const
    {
    if ( iTimerCost )
        {
        return 1 + iConfPanes->Count( );
        }
    return iConfPanes->Count( );
    }

// ---------------------------------------------------------------------------
// CBubbleOutlookConference::ComponentControl
//
//  
// ---------------------------------------------------------------------------
//
CCoeControl* CBubbleOutlookConference::ComponentControl(TInt aIndex) const
    {
    if ( iTimerCost )
        {
        if ( aIndex < iConfPanes->Count( ) ) 
            {
            return iConfPanes->At( aIndex );
            }
        else if ( aIndex == iConfPanes->Count( ) ) 
            {
            return iTimerCost;
            }
        else 
            {
            return NULL;
            }
        }
    return iConfPanes->At( aIndex );
    }

// ---------------------------------------------------------------------------
// CBubbleOutlookConference::Draw
// 
//  
// ---------------------------------------------------------------------------
//
void CBubbleOutlookConference::Draw( const TRect& /*aRect*/ ) const
    {
    }

// ---------------------------------------------------------------------------
// CBubbleOutlookConference::DrawTimerCostNow
//
//  
// ---------------------------------------------------------------------------
//
void CBubbleOutlookConference::DrawTimerCostNow()
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
    DrawLabelNow( iTimerCost );
    }

// ---------------------------------------------------------------------------
// CBubbleOutlookConference::DrawCLINow
//
//  
// ---------------------------------------------------------------------------
//
void CBubbleOutlookConference::DrawCLINow()
    {
    }

// ---------------------------------------------------------------------------
// CBubbleOutlookConference::DrawRowNow
//
//  
// ---------------------------------------------------------------------------
//
void CBubbleOutlookConference::DrawRowNow( CBubbleManager::TRowNumber aRow )
    {
    if ( iHeader == NULL || aRow == 0 )
        {
        return;
        }

    // get highlight
    for ( TUint8 t = 0 ; t < iConfPanes->Count() ; t++ )
        {
        iConfPanes->At( t )->SetHighlight( EFalse );
        }

    if ( iHeader->Highlight() != 0 )
        {
        iConfPanes->At( iHeader->Highlight() - 1 )->SetHighlight( ETrue );
        }

    DrawLabelNow( iConfPanes->At( aRow - 1 ) );
    }

// ---------------------------------------------------------------------------
// CBubbleOutlookConference::DrawBitmaps
//
//  
// ---------------------------------------------------------------------------
//
void CBubbleOutlookConference::DrawBitmaps( CBitmapContext& aGc ) const
    {
    // Check that proper conference header is set
    if ( iHeader == NULL )
        {
        return;
        }

    if ( !iHeader->IsUsed() || !iHeader->IsConference( ) ) 
        {
        return;
        }

    aGc.SetBrushColor( AKN_LAF_COLOR( BubbleLayout::LayoutColourWhite() ) );
    
    if ( !iCallObjectDisplay )
        {
        BubbleUtils::DrawMaskedImage( aGc , iBubble );
        }
    else
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
    }

// ---------------------------------------------------------------------------
// CBubbleOutlookConference::HandleAnimationStartL
//
//  
// ---------------------------------------------------------------------------
//
void CBubbleOutlookConference::HandleAnimationStartL() const
    {
    }

// ---------------------------------------------------------------------------
//  CBubbleOutlookConference::DrawCallHeaderText
// ---------------------------------------------------------------------------
//
void CBubbleOutlookConference::DrawCallHeaderText()
    {    
    }
    
// ---------------------------------------------------------------------------
// CBubbleOutlookConference::DoCall1Layout
// ---------------------------------------------------------------------------
//    
void CBubbleOutlookConference::DoCall1Layout()
    {
    // Deprecated
    }

// ---------------------------------------------------------------------------
// CBubbleOutlookConference::DoCall2Layout
// ---------------------------------------------------------------------------
//
void CBubbleOutlookConference::DoCall2LayoutL()
    {
    iBubble->SetPictureOwnedExternally( EFalse );
    iBubble->SetPicture( NULL, NULL );
    
    TAknLayoutRect bubbleRect;
    bubbleRect.LayoutRect( 
        Rect(), 
        BubbleLayout2::popup_call2_conf_pane_background() );
            
    TRect frameRect( TPoint(0,0), bubbleRect.Rect().Size() );
    TRect outerRect;
    TRect innerRect;
    BubbleLayout2::BubbleFrameInnerOuterRects( frameRect, 
                                               outerRect, 
                                               innerRect );

    iFrameId = KAknsIIDQsnFrCall2BubbleFirst;
    TAknsItemID coMaskFrameId = KAknsIIDQsnFrCall2BubbleFirstCoMask;
            
    iOuterRect = outerRect;
    iOuterRect.Move( Rect().iTl );
    iInnerRect = innerRect;
    iInnerRect.Move( Rect().iTl );
            
    // CLI list and panes
    TAknLayoutRect cliList;
    cliList.LayoutRect( Rect(), 
        BubbleLayout2::popup_call2_conf_cli_list() );
    TRect cliListRect = cliList.Rect();
    
    for ( TUint8 paneIndex = 0; paneIndex < iPaneAmount; paneIndex++ )
        {
        TAknLayoutRect singlePane;
        singlePane.LayoutRect( cliListRect, 
            BubbleLayout2::popup_call2_conf_single_list_graphic_pane( 
            paneIndex ) );
        TRect singlePaneRect = singlePane.Rect();
        iConfPanes->At( paneIndex )->SetRect( singlePaneRect );
        }   
    
    // Timer
    BubbleUtils::LayoutLabel( 
        iTimerCost, 
        Rect(),
        BubbleLayout2::popup_call2_conf_window_call_timer_text() );
    
    // Create call object bitmaps
    iBubble->SetRect( bubbleRect.Rect() );                
    
    BubbleUtils::PrepareBubbleImageL( 
        iFrameId,
        outerRect,
        innerRect,
        iBubble );
    
    if ( iHeader->CallObjectImage() )
        {
        // Scale image according to bubble size
        TSize imageSize( Rect().Size() );
        imageSize.iWidth -= imageSize.iWidth / 2;
        
        BubbleUtils::PrepareCallObjectImageL(
            *iHeader,
            imageSize );
        
        TAknLayoutRect cliRect;
        cliRect.LayoutRect( 
            frameRect, 
            BubbleLayout2::popup_call2_bubble_pane_graphics_cli() );
                    
        // Combine image to bubble image
        BubbleUtils::PrepareCallObjectToBubbleImageL(
            iHeader->CallObjectImage(),
            iHeader->CallObjectImageMask(),
            cliRect.Rect(),
            coMaskFrameId, // gradient image mask
            outerRect,
            innerRect,
            iBubble );                                        
        }    
    }
    
// ---------------------------------------------------------------------------
// CBubbleOutlookConference::DoCall4LayoutL
// ---------------------------------------------------------------------------
//
void CBubbleOutlookConference::DoCall4LayoutL()
    {
    iBubble->SetPictureOwnedExternally( EFalse );
    iBubble->SetPicture( NULL, NULL );
    
    TAknLayoutRect bubbleRect;
    bubbleRect.LayoutRect( 
        Rect(), 
        BubbleLayout4::popup_call4_conf_pane_background() );
            
    TRect frameRect( TPoint(0,0), bubbleRect.Rect().Size() );
    TRect outerRect;
    TRect innerRect;
    BubbleLayout2::BubbleFrameInnerOuterRects( frameRect, 
                                               outerRect, 
                                               innerRect );

    iFrameId = KAknsIIDQsnFrCall2BubbleFirst;
    TAknsItemID coMaskFrameId = KAknsIIDQsnFrCall2BubbleFirstCoMask;
            
    iOuterRect = outerRect;
    iOuterRect.Move( Rect().iTl );
    iInnerRect = innerRect;
    iInnerRect.Move( Rect().iTl );
            
    // CLI list and panes
    TAknLayoutRect cliList;
    cliList.LayoutRect( Rect(), 
        BubbleLayout4::popup_call4_conf_cli_list() );
    TRect cliListRect = cliList.Rect();
    
    for ( TUint8 paneIndex = 0; paneIndex < iPaneAmount; paneIndex++ )
        {
        TAknLayoutRect singlePane;
        singlePane.LayoutRect( cliListRect, 
            BubbleLayout4::popup_call4_conf_single_list_graphic_pane( 
            paneIndex ) );
        TRect singlePaneRect = singlePane.Rect();
        iConfPanes->At( paneIndex )->SetRect( singlePaneRect );
        }   
    
    // Create call object bitmaps
    iBubble->SetRect( bubbleRect.Rect() );                
    
    BubbleUtils::PrepareBubbleImageL( 
        iFrameId,
        outerRect,
        innerRect,
        iBubble );
    
    if ( iHeader->CallObjectImage() )
        {
        // Scale image according to bubble size
        TSize imageSize( Rect().Size() );
        imageSize.iWidth -= imageSize.iWidth / 2;
        
        BubbleUtils::PrepareCallObjectImageL(
            *iHeader,
            imageSize );
        
        TAknLayoutRect cliRect;
        cliRect.LayoutRect( 
            frameRect, 
            BubbleLayout2::popup_call2_bubble_pane_graphics_cli() );
                    
        // Combine image to bubble image
        BubbleUtils::PrepareCallObjectToBubbleImageL(
            iHeader->CallObjectImage(),
            iHeader->CallObjectImageMask(),
            cliRect.Rect(),
            coMaskFrameId, // gradient image mask
            outerRect,
            innerRect,
            iBubble );                                        
        }
    }    

// ---------------------------------------------------------------------------
// CBubbleOutlookConference::GetCall1BubbleBitmaps
// ---------------------------------------------------------------------------
//    
void CBubbleOutlookConference::GetCall1BubbleBitmaps()
    {
    switch ( iPaneAmount )
        {
        case 2:
            iBubbleManager.ImageManager().SetBitmapToImage( 
                    iBubble , 
                    EQgn_graf_call_conf_two , 
                    EQgn_graf_call_conf_two_mask );
            break;
        case 3:
            iBubbleManager.ImageManager().SetBitmapToImage( 
                    iBubble , 
                    EQgn_graf_call_conf_three , 
                    EQgn_graf_call_conf_three_mask );
            break;
        case 4:
            iBubbleManager.ImageManager().SetBitmapToImage( 
                    iBubble , 
                    EQgn_graf_call_conf_four , 
                    EQgn_graf_call_conf_four_mask );
            break;
        case 5:
            iBubbleManager.ImageManager().SetBitmapToImage( 
                    iBubble , 
                    EQgn_graf_call_conf_five , 
                    EQgn_graf_call_conf_five_mask );
            break;
        default:
            Panic( EBMPanicTooManyCallsInConference );
            break;
        }
    }    

// ---------------------------------------------------------------------------
// CBubbleOutlookConference::HandlePointerEventL
// ---------------------------------------------------------------------------
//    
void CBubbleOutlookConference::HandlePointerEventL
    ( const TPointerEvent& aPointerEvent )           
    {
    if ( !iHeader->Highlight() || 
         aPointerEvent.iType != TPointerEvent::EButton1Up )
        return;
    
    for ( TInt i=0; i < iPaneAmount; i++ )
        {
        if ( iConfPanes->At( i )->Rect().Contains(
                 aPointerEvent.iPosition ) )
            {
            TInt hightlight = i+1;
            if ( iHeader->Highlight() != hightlight )
                {
                iConfPanes->At( iHeader->Highlight() - 1 )->SetHighlight( EFalse );
                iConfPanes->At( iHeader->Highlight() - 1 )->DrawDeferred();
                iHeader->SetHighlight( hightlight );
                iConfPanes->At( i )->SetHighlight( ETrue );
                iConfPanes->At( i )->DrawDeferred();    
                }
            }
        }
    }        

// End of File
