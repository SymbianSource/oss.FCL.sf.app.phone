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
* Description:  Image Manager
*
*/


// INCLUDE FILES
#include    "BMBubbleImageManager.h"
#include    "BMBubbleSkinBitmap.h"
#include    "BubbleManagerPaths.h"
#include    "BMPanic.h"
#include    "BMUtils.h"
#include    "BMLayout.h"
#include    "BMBubbleIconIndex.h"

#include    <eikimage.h> 
#include    <AknsUtils.h>
#include    <aknconsts.h>
#include    <AknIconUtils.h>

// CONSTANTS
// 1-piece backgrounds
const TInt KBMLastCachedBitmap1 = EQgn_graf_call_first_one_disconn_mask;
// 9-piece backgrounds
const TInt KBMLastCachedBitmap2 = EQgn_indi_call_voip_disconn_mask; 
const TInt KBMLastAvkonBitmap = EQgn_prop_nrtyp_assistant_mask; 

// ========================= MEMBER FUNCTIONS ================================

// ---------------------------------------------------------------------------
// CBubbleImageManager::CBubbleImageManager()
// Default constructor
// 
// ---------------------------------------------------------------------------
//
CBubbleImageManager::CBubbleImageManager( TBool aFrameIcons ) : 
    iOrder( CompareSkinnedData ), iFrameIcons( aFrameIcons )
    {    
    }

// ---------------------------------------------------------------------------
// CBubbleImageManager::NewL
// Symbian OS two phased constructor
// 
// ---------------------------------------------------------------------------
//
CBubbleImageManager* CBubbleImageManager::NewL( TBool aFrameIcons )
    {
    CBubbleImageManager* self = new( ELeave ) CBubbleImageManager( 
        aFrameIcons );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();    // self
    return self;
    }


// ---------------------------------------------------------------------------
// CBubbleImageManager::ConstructL
// Symbian OS two phased constructor
// 
// ---------------------------------------------------------------------------
//
void CBubbleImageManager::ConstructL()
    {
    LoadCachedBitmapsL();
    
    iBitmapInitializer = CIdle::NewL( CActive::EPriorityIdle );
    }

// ---------------------------------------------------------------------------
// CBubbleImageManager::~CBubbleImageManager()
// Destructor
// 
// ---------------------------------------------------------------------------
//
CBubbleImageManager::~CBubbleImageManager()
    {
    iSkinnedBitmaps.ResetAndDestroy();
    
    if ( iBitmapInitializer ) 
        {
        iBitmapInitializer->Cancel();                
        }
    delete iBitmapInitializer;
    }

// ---------------------------------------------------------------------------
// CBubbleImageManager::SetBitmapToImage
// ---------------------------------------------------------------------------
//
void CBubbleImageManager::SetBitmapToImage( CEikImage*& aImage , 
                                            const TBMIcons& aPicture )
    {
    // Check the paramater is valid.
    if ( aImage == NULL )
        {
        return;
        }
    SetBitmapToImage( *aImage , aPicture, (TBMIcons) KErrNotFound );
    }

// ---------------------------------------------------------------------------
// CBubbleImageManager::SetBitmapToImage
// ---------------------------------------------------------------------------
//
void CBubbleImageManager::SetBitmapToImage( CEikImage*& aImage , 
                                            const TBMIcons& aPicture, 
                                            const TBMIcons& aPictureMask 
                                           )
    {
    // Check that paramaters are valid and bitmaps exist.
    if ( aImage == NULL )
        {
        return;
        }
    SetBitmapToImage( *aImage , aPicture, aPictureMask );
    }

// ---------------------------------------------------------------------------
// CBubbleImageManager::SetBitmapToImage
// ---------------------------------------------------------------------------
//
void CBubbleImageManager::SetBitmapToImage( CEikImage& aImage , 
                                            const TBMIcons& aPicture, 
                                            const TBMIcons& aPictureMask 
                                           ) 
    {
    // Check parameter range and load the bitmaps.
    if ( !GetSkin( aImage, aPicture, aPictureMask ) )
        {
        // Failed, make image not visible
        aImage.SetPicture( NULL , NULL );
        aImage.MakeVisible( EFalse );
        return;
        }    
    }

// ---------------------------------------------------------------------------
// CBubbleImageManager::MapEnumToMbmFile
// ---------------------------------------------------------------------------
//
void CBubbleImageManager::MapEnumToMbmFile( const TBMIcons aBmIcon,
                                            TInt& aMbm,
                                            TDes& aFileName ) const
    {   
    aMbm = KBMIconIndex[aBmIcon].iconIndex;
    switch ( KBMIconIndex[aBmIcon].fileIndex )
        {
        case ECallstatusIconfile:
            aFileName = KBMMbmZDrive;
            aFileName.Append( KDC_APP_BITMAP_DIR );
            aFileName.Append( KBMCallStatusBitmapFile );
            aFileName.ZeroTerminate();
            break;
        case EBubblemanagerIconfile:
            BubbleUtils::BubbleIconFileName( aFileName );
            break;
        case EAvkonIconfile:
            aFileName = AknIconUtils::AvkonIconFileName();
            break;
        default:
            Panic( EBMPanicImages );
        }
    }


// ---------------------------------------------------------------------------
// CBubbleImageManager::MapEnumToSkin
// ---------------------------------------------------------------------------
//
void CBubbleImageManager::MapEnumToSkin( const TBMIcons aBmIcon, 
                                         TAknsItemID& aItemId, 
                                         TInt& aColorTableIndex ) const
    {    
    switch ( aBmIcon )
        {
        case EQgn_indi_call_active:
            aItemId = KAknsIIDQgnIndiCallActive;
            break;
        case EQgn_indi_call_active_cyph_off:
            aItemId = KAknsIIDQgnIndiCallActiveCyphOff;
            break;
        case EQgn_indi_call_disconn:
            aItemId = KAknsIIDQgnIndiCallDisconn;
            break;
        case EQgn_indi_call_disconn_cyph_off:
            aItemId = KAknsIIDQgnIndiCallDisconnCyphOff;
            break;
        case EQgn_indi_call_held:
            aItemId = KAknsIIDQgnIndiCallHeld;
            break;
        case EQgn_indi_call_held_cyph_off:
            aItemId = KAknsIIDQgnIndiCallHeldCyphOff;
            break;
        case EQgn_indi_call_active_2:
            aItemId = KAknsIIDQgnIndiCallActive2;
            break;    
        case EQgn_indi_call_video_1:
            aItemId = KAknsIIDQgnIndiCallVideo1;
            break;
        case EQgn_indi_call_video_callsta_1:
            aItemId = KAknsIIDQgnIndiCallVideoCallsta1;
            break;
        case EQgn_indi_call_video_disconn:
            aItemId = KAknsIIDQgnIndiCallVideoDisconn;
            break;
        case EQgn_indi_call_active_emergency:
            aItemId = KAknsIIDQgnIndiCallActiveEmergency;
            break;
        case EQgn_graf_call_first_one_active:
            aItemId = KAknsIIDQgnGrafCallFirstOneActive;
            break;
        case EQgn_graf_call_first_one_disconn:
            aItemId = KAknsIIDQgnGrafCallFirstOneDisconn;
            break;
        case EQgn_graf_call_first_one_held:
            aItemId = KAknsIIDQgnGrafCallFirstOneHeld;
            break;
        case EQgn_graf_call_rec_big:
            aItemId = KAknsIIDQgnGrafCallRecBig;
            break;
        case EQgn_graf_call_rec_big_disconn:
            aItemId = KAknsIIDQgnGrafCallRecBigDisconn;
            break;
        case EQgn_graf_call_rec_big_right:
            aItemId = KAknsIIDQgnGrafCallRecBigRight;
            break;
        case EQgn_graf_call_rec_big_right_disconn:
            aItemId = KAknsIIDQgnGrafCallRecBigRightDisconn;
            break;
        case EQgn_indi_call_waiting_1:
            aItemId = KAknsIIDQgnIndiCallWaiting1;
            break;
        case EQgn_indi_call_waiting_cyph_off_1:
            aItemId = KAknsIIDQgnIndiCallWaitingCyphOff;
            break;
        case EQgn_indi_call_waiting_disconn:
            aItemId = KAknsIIDQgnIndiCallWaitingDisconn;
            break;
        case EQgn_indi_call_waiting_disconn_cyph_off:
            aItemId = KAknsIIDQgnIndiCallWaitingDisconnCyphOff;
            break;
        case EQgn_graf_call_first_one_active_emergency:
            aItemId = KAknsIIDQgnGrafCallFirstOneActiveEmergency;
            break;  
        case EQgn_graf_call_two_active_emergency:
            aItemId = KAknsIIDQgnGrafCallTwoActiveEmergency;
            break;
        case EQgn_graf_call_conf_five:
            aItemId = KAknsIIDQgnGrafCallConfFive;
            break;
        case EQgn_graf_call_conf_four:
            aItemId = KAknsIIDQgnGrafCallConfFour;
            break;
        case EQgn_graf_call_conf_three:
            aItemId = KAknsIIDQgnGrafCallConfThree;
            break;
        case EQgn_graf_call_conf_two:
            aItemId = KAknsIIDQgnGrafCallConfTwo;
            break;
        case EQgn_graf_call_first_three_active:
            aItemId = KAknsIIDQgnGrafCallFirstThreeActive;
            break;
        case EQgn_graf_call_first_three_disconn:
            aItemId = KAknsIIDQgnGrafCallFirstThreeDisconn;
            break;
        case EQgn_graf_call_first_three_held:
            aItemId = KAknsIIDQgnGrafCallFirstThreeHeld;
            break;
        case EQgn_graf_call_first_two_active:
            aItemId = KAknsIIDQgnGrafCallFirstTwoActive;
            break;
        case EQgn_graf_call_first_two_disconn:
            aItemId = KAknsIIDQgnGrafCallFirstTwoDisconn;
            break;
        case EQgn_graf_call_first_two_held:
            aItemId = KAknsIIDQgnGrafCallFirstTwoHeld;
            break;
        case EQgn_graf_call_first_wait_active:
            aItemId = KAknsIIDQgnGrafCallFirstWaitActive;
            break;
        case EQgn_graf_call_first_wait_disconn:
            aItemId = KAknsIIDQgnGrafCallFirstWaitDisconn;
            break;
        case EQgn_graf_call_hidden_held:
            aItemId = KAknsIIDQgnGrafCallHiddenHeld;
            break;
        case EQgn_graf_call_rec_big_left:
            aItemId = KAknsIIDQgnGrafCallRecBigLeft;
            break;
        case EQgn_graf_call_rec_big_left_disconn:
            aItemId = KAknsIIDQgnGrafCallRecBigLeftDisconn;
            break;
        case EQgn_graf_call_rec_small_left:
            aItemId = KAknsIIDQgnGrafCallRecSmallLeft;
            break;
        case EQgn_graf_call_rec_small_right:
            aItemId = KAknsIIDQgnGrafCallRecSmallRight;
            break;
        case EQgn_graf_call_rec_small_right_disconn:
            aItemId = KAknsIIDQgnGrafCallRecSmallRightDisconn;
            break;
        case EQgn_graf_call_rec_bigger:
            aItemId = KAknsIIDQgnGrafCallRecBigger;
            break;
        case EQgn_graf_call_second_three_active:
            aItemId = KAknsIIDQgnGrafCallSecondThreeActive;
            break;
        case EQgn_graf_call_second_three_disconn:
            aItemId = KAknsIIDQgnGrafCallSecondThreeDisconn;
            break;
        case EQgn_graf_call_second_three_held:
            aItemId = KAknsIIDQgnGrafCallSecondThreeHeld;
            break;
        case EQgn_graf_call_second_two_active:
            aItemId = KAknsIIDQgnGrafCallSecondTwoActive;
            break;
        case EQgn_graf_call_second_two_disconn:
            aItemId = KAknsIIDQgnGrafCallSecondTwoDisconn;
            break;
        case EQgn_graf_call_second_two_held:
            aItemId = KAknsIIDQgnGrafCallSecondTwoHeld;
            break;
        case EQgn_indi_call_active_conf:
            aItemId = KAknsIIDQgnIndiCallActiveConf;
            break;
        case EQgn_indi_call_active_conf_cyph_off:
            aItemId = KAknsIIDQgnIndiCallActiveConfCyphOff;
            break;
        case EQgn_indi_call_active_cyph_off_2:
            aItemId = KAknsIIDQgnIndiCallActiveCyphOff2;
            break;
        case EQgn_indi_call_disconn_conf:
            aItemId = KAknsIIDQgnIndiCallDisconnConf;
            break;
        case EQgn_indi_call_disconn_conf_cyph_off:
            aItemId = KAknsIIDQgnIndiCallDisconnConfCyphOff;
            break;
        case EQgn_indi_call_held_conf:
            aItemId = KAknsIIDQgnIndiCallHeldConf;
            break;
        case EQgn_indi_call_held_conf_cyph_off:
            aItemId = KAknsIIDQgnIndiCallHeldConfCyphOff;
            break;
        case EQgn_indi_call_muted:
            aItemId = KAknsIIDQgnIndiCallMuted;
            break;
        case EQgn_indi_call_waiting_2:
            aItemId = KAknsIIDQgnIndiCallWaiting2;
            break;
        case EQgn_indi_call_waiting_cyph_off_2:
            aItemId = KAknsIIDQgnIndiCallWaitingCyphOff2;
            break;
        case EQgn_indi_call_video_2:
            aItemId = KAknsIIDQgnIndiCallVideo2;
            break;
        case EQgn_indi_call_video_callsta_2:
            aItemId = KAknsIIDQgnIndiCallVideoCallsta2;
            break;            
        case EQgn_indi_call_video_callsta_disconn:
            aItemId = KAknsIIDQgnIndiCallVideoCallstaDisconn; 
            break;
        case EQgn_indi_call_voip_active:
            aItemId = KAknsIIDQgnIndiCallVoipActive;
            break;
        case EQgn_indi_call_voip_active_2:
            aItemId = KAknsIIDQgnIndiCallVoipActive2;
            break;    
        case EQgn_indi_call_voip_disconn:
            aItemId = KAknsIIDQgnIndiCallVoipDisconn;
            break;
        case EQgn_indi_call_voip_held:
            aItemId = KAknsIIDQgnIndiCallVoipHeld;
            break;
        case EQgn_indi_call_voip_waiting_1:
            aItemId = KAknsIIDQgnIndiCallVoipWaiting1;
            break;
        case EQgn_indi_call_voip_waiting_2:
            aItemId = KAknsIIDQgnIndiCallVoipWaiting2;
            break;
        case EQgn_indi_call_voip_callsta_disconn:
            aItemId = KAknsIIDQgnIndiCallVoipCallstaDisconn;
            break; 
        case EQgn_graf_call_image_1:
            aItemId = KAknsIIDQgnGrafCallImage1;
            break;
        case EQgn_indi_call4_image_mask_prt:
            aItemId = KAknsIIDNone;
            break;
        case EQgn_indi_call4_image_mask_lsc:
            aItemId = KAknsIIDNone;
            break;

// Colour changes
        case EQgn_indi_call_line2:
            aItemId = KAknsIIDQgnIndiCallLine2;
            aColorTableIndex = EAknsCIQsnIconColorsCG11;
            break;
        case EQgn_indi_call_cyphering_off:
            aItemId = KAknsIIDQgnIndiCallCypheringOff;
            aColorTableIndex = EAknsCIQsnIconColorsCG11;
            break;
        case EQgn_indi_call_diverted:
            aItemId = KAknsIIDQgnIndiCallDiverted;
            aColorTableIndex = EAknsCIQsnIconColorsCG11;
            break;

        // Not skinned
        case EQgn_prop_nrtyp_home_div:
            aItemId = KAknsIIDQgnPropNrtypHomeDiv;
            break;
        case EQgn_prop_nrtyp_mobile_div:
            aItemId = KAknsIIDQgnPropNrtypMobileDiv;
            break;
        case EQgn_prop_nrtyp_phone_div:
            aItemId = KAknsIIDQgnPropNrtypPhoneDiv;
            break;
        case EQgn_prop_nrtyp_work_div:
            aItemId = KAknsIIDQgnPropNrtypWorkDiv;
            break;

            // From avkon.mbm
        case EQgn_prop_nrtyp_fax:
            aItemId = KAknsIIDQgnPropNrtypFax;
            break;
        case EQgn_prop_nrtyp_home:
            aItemId = KAknsIIDQgnPropNrtypPhone;
            break;
        case EQgn_prop_nrtyp_mobile:
            aItemId = KAknsIIDQgnPropNrtypMobile;
            break;
        case EQgn_prop_nrtyp_pager:
            aItemId = KAknsIIDQgnPropNrtypPager;
            break;
        case EQgn_prop_nrtyp_phone:
            aItemId = KAknsIIDQgnPropNrtypPhone;
            break;
        case EQgn_prop_nrtyp_work:
            aItemId = KAknsIIDQgnPropNrtypPhone;
            break;
        case EQgn_prop_nrtyp_voip:
            aItemId = KAknsIIDQgnPropNrtypVoip;
            break;
        case EQgn_prop_nrtyp_car:
            aItemId = KAknsIIDQgnPropNrtypCar;
            break;            
        case EQgn_prop_nrtyp_assistant:
            aItemId = KAknsIIDQgnPropNrtypAssistant;
            break;            
        default:
            break;
        }
    }

// ---------------------------------------------------------------------------
// CBubbleImageManager::ClearSkinsCache
// 
// ---------------------------------------------------------------------------
//
void CBubbleImageManager::ClearSkinsCache()
    {
    iSkinnedBitmaps.ResetAndDestroy();
    
    TRAP_IGNORE( LoadCachedBitmapsL() );
    
    PrepareCachedBitmaps();
    }
    

// ---------------------------------------------------------------------------
// CBubbleImageManager::CompareSkinnedData
// ---------------------------------------------------------------------------
//
TInt CBubbleImageManager::CompareSkinnedData( 
    const CBubbleSkinBitmap& aSkinnedBitmap1, 
    const CBubbleSkinBitmap& aSkinnedBitmap2 )
    {
    if ( aSkinnedBitmap1.BitmapIndex() > aSkinnedBitmap2.BitmapIndex() )
        {
        return 1;
        }
    if ( aSkinnedBitmap1.BitmapIndex() < aSkinnedBitmap2.BitmapIndex() )
        {
        return -1;
        }
    return 0;    
    }

// ---------------------------------------------------------------------------
// CBubbleImageManager::GetSkin
// ---------------------------------------------------------------------------
//
TBool CBubbleImageManager::GetSkin( CEikImage&      aImage, 
                                    const TBMIcons  aBmIcon, 
                                    const TBMIcons  aBmIconMask )
    {
    if ( aBmIcon < 0 || aBmIcon > KBMLastAvkonBitmap )
        {        
        return EFalse;
        }

    CBubbleSkinBitmap skinnedBitmap;
    skinnedBitmap.SetBitmapIndex( aBmIcon );
    skinnedBitmap.SetSkinnedBitmap( NULL );
    skinnedBitmap.SetSkinnedMask( NULL );
    TInt index = iSkinnedBitmaps.FindInOrder( &skinnedBitmap, iOrder );
    
    CFbsBitmap* bitmap = NULL;
    CFbsBitmap* mask = NULL;
    if ( index != KErrNotFound )
        {
        bitmap = iSkinnedBitmaps[ index ]->SkinnedBitmap();
        mask = iSkinnedBitmaps[ index ]->SkinnedMask();
        }
                                                     
    if ( bitmap == NULL )
        {
        TRAP_IGNORE(
            LoadAndAppendSkinL( aBmIcon, aBmIconMask, bitmap, mask ) );
        }

    if ( bitmap )
        {
        aImage.SetPicture( bitmap, mask ); // Mask might be NULL
        aImage.SetPictureOwnedExternally( ( aBmIcon <= LastCachedIcon() ) ); // cached bitmaps are owned here
        return ETrue;
        }
    
    return EFalse;
    }

// ---------------------------------------------------------------------------
// CBubbleImageManager::LoadAndAppendSkinL
// ---------------------------------------------------------------------------
//
void CBubbleImageManager::LoadAndAppendSkinL( const TBMIcons aBmIcon,
                                              const TBMIcons aBmIconMask,
                                              CFbsBitmap*&   aBitmap, 
                                              CFbsBitmap*&   aMask )

    {
    TInt mbmBitmapIndex = 0;
    TInt mbmMaskIndex = KErrNotFound;
    TInt colorTableIndex( 0 );
    TAknsItemID skinItemId( KAknsIIDNone );    
    MapEnumToMbmFile( aBmIcon , mbmBitmapIndex , iMbmFileBuffer );    
    
    if ( mbmBitmapIndex == KErrNotFound )
        {        
        User::Leave( KErrNotFound );
        }
    
    if ( aBmIconMask >= 0 && aBmIconMask <= KBMLastAvkonBitmap )
        {        
        MapEnumToMbmFile( aBmIconMask , mbmMaskIndex , iMbmFileBuffer );
        }
    MapEnumToSkin( aBmIcon, skinItemId, colorTableIndex );    
    MAknsSkinInstance* skin = AknsUtils::SkinInstance();

    if( colorTableIndex != 0 )
        {
        AknsUtils::CreateColorIconLC( skin, skinItemId, 
                                      KAknsIIDQsnIconColors, 
                                      colorTableIndex, 
                                      aBitmap, aMask, 
                                      iMbmFileBuffer, 
                                      mbmBitmapIndex, mbmMaskIndex, 
                                      KRgbBlack );
        }
    else
        {
        if ( mbmMaskIndex != KErrNotFound )
            {
            AknsUtils::CreateIconLC( skin, skinItemId, 
                                 aBitmap, aMask, iMbmFileBuffer, 
                                 mbmBitmapIndex, mbmMaskIndex );
            }
        else
            {
            AknsUtils::CreateIconLC( skin, skinItemId, 
                                 aBitmap, iMbmFileBuffer, mbmBitmapIndex );
            }
        }

    
    if ( aBmIcon <= LastCachedIcon() )
        {
        CBubbleSkinBitmap* skinnedBitmap = new (ELeave) CBubbleSkinBitmap();
        CleanupStack::PushL( skinnedBitmap );
        skinnedBitmap->SetBitmapIndex( aBmIcon );
        //skinnedBitmap->SetBitmapSkinItemID( skinItemId );
        skinnedBitmap->SetSkinnedBitmap( aBitmap );
        skinnedBitmap->SetSkinnedMask( aMask );
        User::LeaveIfError( 
            iSkinnedBitmaps.InsertInOrder( skinnedBitmap, iOrder ) );                
        CleanupStack::Pop( skinnedBitmap ); // Ownership was given to iSkinnedBitmaps
        }


    if ( colorTableIndex != 0 )
        {
        CleanupStack::Pop( 2 ); // aBitmap, aMask
        }
    else
        {        
        if ( mbmMaskIndex != KErrNotFound )
            {
            CleanupStack::Pop(); // aMask
            }
        CleanupStack::Pop(); // aBitmap
        }    
    }

// ---------------------------------------------------------------------------
// CBubbleImageManager::DoInitializeBitmaps
// ---------------------------------------------------------------------------
//    
TBool CBubbleImageManager::DoInitializeBitmaps()
    {
    // Bubble rect for single call or number entry
    TAknWindowLineLayout bottomBubble = 
    BubbleLayout::popup_call_audio_in_window( 1 );
    TAknLayoutRect bottomBubbleRect;
    bottomBubbleRect.LayoutRect( BubbleLayout::MainPaneRect(), 
                                 bottomBubble );
    
    TInt iconCount = iSkinnedBitmaps.Count();
    if ( iBitmapInitIndex < iconCount )
        {
        CBubbleSkinBitmap* skinnedBitmap = iSkinnedBitmaps[ iBitmapInitIndex ];    
        
        switch ( skinnedBitmap->BitmapIndex() )
            {
            // Small call status icon
            case EQgn_indi_call_active:
            case EQgn_indi_call_active_2:
            case EQgn_indi_call_disconn:
            case EQgn_indi_call_held:
            case EQgn_indi_call_video_callsta_1:
            case EQgn_indi_call_video_callsta_2:
            case EQgn_indi_call_video_callsta_disconn:
            case EQgn_indi_call_voip_active:
            case EQgn_indi_call_voip_active_2:
            case EQgn_indi_call_voip_held:
            case EQgn_indi_call_voip_callsta_disconn:
                {
                TAknWindowLineLayout smallCallStatusIcon = 
                    BubbleLayout::popup_call_audio_first_window_1_elements_1();
                TAknLayoutRect smallCallStatusIconRect;
                smallCallStatusIconRect.LayoutRect( bottomBubbleRect.Rect(),
                                                    smallCallStatusIcon );                                        
                                             
                AknIconUtils::SetSize( skinnedBitmap->SkinnedBitmap(), 
                                       smallCallStatusIconRect.Rect().Size() );                                             
                break;    
                }
            
            // Big call status icon
            case EQgn_indi_call_waiting_1:
            case EQgn_indi_call_waiting_2:
            case EQgn_indi_call_waiting_disconn:
            case EQgn_indi_call_video_1:
            case EQgn_indi_call_video_2:
            case EQgn_indi_call_video_disconn:
            case EQgn_indi_call_voip_waiting_1:
            case EQgn_indi_call_voip_waiting_2:
            case EQgn_indi_call_voip_disconn:
                {
                TAknWindowLineLayout bigCallStatusIcon = 
                    BubbleLayout::popup_call_audio_in_window_1_elements_1();
                TAknLayoutRect bigCallStatusIconRect;
                bigCallStatusIconRect.LayoutRect( bottomBubbleRect.Rect(),
                                                  bigCallStatusIcon );                                        
                                             
                AknIconUtils::SetSize( skinnedBitmap->SkinnedBitmap(), 
                                       bigCallStatusIconRect.Rect().Size() ); 
                break;    
                }
            
            // Bubble graphics
            case EQgn_graf_call_rec_big: 
            case EQgn_graf_call_rec_big_disconn:
            case EQgn_graf_call_first_one_active:
            case EQgn_graf_call_first_one_disconn:
            case EQgn_graf_call_first_one_held:
                {
                AknIconUtils::SetSize( skinnedBitmap->SkinnedBitmap(), 
                                       bottomBubbleRect.Rect().Size(),
                                       EAspectRatioNotPreserved );  
                break;    
                }
            default:
                // do nothing
                break;
            }
        
        iBitmapInitIndex++;
        return ETrue;
        }
    else
        {
        return EFalse;    
        }        
    }

// ---------------------------------------------------------------------------
// CBubbleImageManager::InitializeBitmap
// ---------------------------------------------------------------------------
// 
TInt CBubbleImageManager::InitializeBitmaps( TAny* aThis )
    {
    return static_cast<CBubbleImageManager*>( aThis )->DoInitializeBitmaps();
    }

// ---------------------------------------------------------------------------
// CBubbleImageManager::PrepareCachedBitmaps
// ---------------------------------------------------------------------------
//    
void CBubbleImageManager::PrepareCachedBitmaps()
    {
    iBitmapInitIndex = 0;
    iBitmapInitializer->Cancel();
    iBitmapInitializer->Start( TCallBack( InitializeBitmaps, this ) );            
    }

// ---------------------------------------------------------------------------
// CBubbleImageManager::LoadCachedBitmapsL
// ---------------------------------------------------------------------------
//
void CBubbleImageManager::LoadCachedBitmapsL()
    {
    // Load essential bitmaps right away
    CFbsBitmap* newBitmap = NULL;
    CFbsBitmap* newMask = NULL;
    TInt lastIcon = LastCachedIcon();
    for ( TInt i = 0 ; i < lastIcon ; i++ )
        {
        LoadAndAppendSkinL( (TBMIcons) i, // icon
                            (TBMIcons) (i+1), // mask
                            newBitmap, 
                            newMask );
        newBitmap = NULL;
        newMask = NULL;
        }    
    }

// ---------------------------------------------------------------------------
// CBubbleImageManager::LastCachedIcon
// ---------------------------------------------------------------------------
//    
TInt CBubbleImageManager::LastCachedIcon()
    {
    if ( iFrameIcons )    
        {
        return KBMLastCachedBitmap2;    
        }
    else
        {
        return KBMLastCachedBitmap1;    
        }        
    }

// End of File
