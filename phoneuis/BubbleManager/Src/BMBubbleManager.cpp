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
* Description:  Manager for the whole subsystem
*
*/


// INCLUDE FILES

#include    "BMBubbleManager.h"
#include    "BMBubbleImageManager.h"
#include    "BMResourceManager.h"
#include    "BMCustomManager.h"
#include    "BMBubbleOutlook.h"
#include    "BMBubbleOutlookMiddle.h"
#include    "BMBubbleOutlookConference.h"
#include    "BMBubbleOutlookNE.h"
#include    "BMCallHeader.h"
#include    "BMConfHeader.h"
#include    "BMPanic.h"
#include    "BMUtils.h"
#include    "BMLayout.h"
#include    "BMBubbleImage.h"
#include    "BubbleManagerPaths.h"
#include    "BMCallObjectManager.h"
#include    "BMLayout2.h"
#include    "BMLayout3.h"
#include    "BMLayout4.h"
#include    "BMLayout6.h"
#include    "BMCallObjectUtils.h"
#include    "BMVideoController.h"
#include    "BMTouchPane.h"
#include    "BMTouchPaneInterface.h"
#include    "BMMainPaneControl.h"
#include    <eikimage.h>    // for muted-image
#include    <coemain.h>
#include    <PbkFields.hrh> // for field id
#include    <ConeResLoader.h> 
#include    <AknsUtils.h>
#include    <NumberGrouping.h>
#include    <AknDef.h>
#include    <layoutmetadata.cdl.h>

#include    <featmgr.h> 
#include    <telephonyvariant.hrh>

#include <centralrepository.h>
#include    <NumberGroupingCRKeys.h>
#ifdef RD_UI_TRANSITION_EFFECTS_PHASE2
#include <gfxtranseffect/gfxtranseffect.h> 
#include <akntransitionutils.h>
#endif

#include    "BMBubbleDebug.h"

// ========================= MEMBER FUNCTIONS ================================

// ---------------------------------------------------------------------------
// CBubbleManager::NewL
// Symbian OS two phased constructor
// 
// ---------------------------------------------------------------------------
//
EXPORT_C CBubbleManager* CBubbleManager::NewL( const CCoeControl& aContainer, 
                                               const TRect& aRect )
    {
    CBubbleManager* self = new( ELeave )CBubbleManager;    
    CleanupStack::PushL( self );
    self->ConstructL( aContainer , aRect, EFalse );
    CleanupStack::Pop();    // self
    return self;
    }

// ---------------------------------------------------------------------------
// CBubbleManager::NewL
// Symbian OS two phased constructor
// 
// ---------------------------------------------------------------------------
//
EXPORT_C CBubbleManager* CBubbleManager::NewL( const CCoeControl& aContainer, 
                                               const TRect& aRect,
                                               TBool aBigNEInIdle )
    {
    CBubbleManager* self = new( ELeave )CBubbleManager;    
    CleanupStack::PushL( self );
    self->ConstructL( aContainer , aRect, aBigNEInIdle );
    CleanupStack::Pop();    // self
    return self;
    }

// ---------------------------------------------------------------------------
// CBubbleManager::ConstructL
// Symbian OS two phased constructor
// 
// ---------------------------------------------------------------------------
//

void CBubbleManager::ConstructL( 
    const CCoeControl& aContainer, 
    const TRect& aRect,
    TBool aBigNEInIdle )
    {    
    // Call object display
    if ( FeatureManager::FeatureSupported( KFeatureIdCallImagetext ) )
        {
        iConfigFlags |= EBMCallObjectDisplay;    
        }
    
    // Bigger number entry window in idle    
    if ( aBigNEInIdle )
        {
        iConfigFlags |= EBMBigNEInIdleState;    
        }
   
     // Touch CallHandling.
    if ( FeatureManager::FeatureSupported( KFeatureIdTouchCallHandling ) )
        {
        iConfigFlags |= EBMTouchCallhandling;    
        }
               
    // set window
    SetContainerWindowL( aContainer );
    SetParent( const_cast<CCoeControl*>(&aContainer) );
    iParentControl = &aContainer;
    
    // load resource file for animation and number entry component.
    RConeResourceLoader resourceLoader( *iCoeEnv ); 
    TFileName fileName; 
    BubbleUtils::BubbleResourceFileName( fileName ); 
    resourceLoader.OpenL(fileName); 
    CleanupClosePushL( resourceLoader );
    
    // create image manager
    iImageManager = CBubbleImageManager::NewL(
        iConfigFlags & EBMCallObjectDisplay );

    // create resource pool
    iResourceManager = CBubbleResourceManager::NewL( *this );
    
    // create custom manager
    iCustomManager = CBubbleCustomManager::NewL(*this);
    
    // muted image
    iIsMuted = EFalse;
    iMutedImage = new( ELeave ) CBubbleImage;
    iMutedImage->SetContainerWindowL( *this );
    iMutedImage->SetParent( this );
    iMutedImage->SetPictureOwnedExternally( ETrue );
    iMutedImage->MakeVisible( iIsMuted );
    
    // create array of call headers
    CBubbleCallHeader* header;
    iCallHeaders = new( ELeave ) CArrayPtrFlat<CBubbleCallHeader>( 1 );
    iCallHeaders->SetReserveL( KBubbleCallMaxAmount );
    for ( TUint8 i = 0 ; i < KBubbleCallMaxAmount ; i++ )
        {
        header = CBubbleCallHeader::NewL( i );
        iCallHeaders->InsertL( i , header );    // can't leave
        }
    
    // set reserve for active headers
    iActiveHeaders = new ( ELeave ) CArrayPtrFlat<CBubbleHeader>( 1 );
    // 1 is for conference
    iActiveHeaders->SetReserveL( KBubbleCallMaxAmount + 1 ); 
        
    // create conference header
    iConfHeader = CBubbleConfHeader::NewL( KBubbleConferenceId );
    
    // create number entry
    iNumberEntry = new ( ELeave ) CBubbleOutlookNumberEntry( *this, 
                                                             *iMutedImage );
    iNumberEntry->SetCallObjectDisplay( 
        iConfigFlags & EBMCallObjectDisplay );
    iNumberEntry->SetContainerWindowL( *this );
    iNumberEntry->SetParent( this );
    iNumberEntry->ConstructL();
    iNumberEntry->MakeVisible( EFalse ); //add
    
    // create places and their contents ( outlooks )
    TInt maxAmount = (iConfigFlags & EBMTouchCallhandling) ?
        KBubblePlaceMaxAmountTouch : KBubblePlaceMaxAmount;
    CBubblePlace* place;
    iBubblePlaces = new ( ELeave ) CArrayPtrFlat<CBubblePlace>( 1 );
    iBubblePlaces->SetReserveL( maxAmount );
    for ( TUint8 e = 0 ; e < maxAmount ; e++  )
        {
        place = new ( ELeave ) CBubblePlace;
        CleanupStack::PushL( place );
        place->SetContainerWindowL( *this );
        place->SetParent( this );
        place->ConstructL( CBubblePlace::TPhoneBubblePlace ( e ) , *this );
        place->GetBubbleOutlook().SetCallObjectDisplay( 
            iConfigFlags & EBMCallObjectDisplay );
        iBubblePlaces->InsertL( e , place );
        CleanupStack::Pop(); //place
        }
    
    // reserve space for active places
    iActivePlaces = 
        new (ELeave) CArrayPtrFlat<CBubblePlace>( KBubblePlaceMaxVisible );
    iActivePlaces->SetReserveL( KBubblePlaceMaxVisible );
    
    // reserve space for label text
    iLabelBuf = HBufC::NewMaxL ( KBubbleLabelMaxLength );

    // call object manager
    if ( iConfigFlags & EBMCallObjectDisplay )
        {
        iCallObjectManager = CBubbleCallObjectManager::NewL( *this );
        
        // Main pane control is not set as component control,
        // it just controls the drawing of the call image to
        // the main pane.
        iMainPaneControl = CBubbleMainPaneControl::NewL(
                *this,
                *iCallObjectManager );
        iMainPaneControl->ActivateL();
        iMainPaneControl->SetContainerWindowL( *this );
        iMainPaneControl->MakeVisible( EFalse );
        }
        
    // Video ringtone
    iVideoController = CBubbleVideoController::NewL( *this );
    
    // Touch pane
    if ( iConfigFlags & EBMTouchCallhandling )
        {
        iTouchPane = CBubbleTouchPane::NewL();
        iTouchPane->SetContainerWindowL( *this );
        iTouchPane->SetParent( this );
        iTouchPane->MakeVisible( EFalse );
        iTouchPane->ActivateL();
        }   
    
    SetRect( aRect );
    ActivateL();
    iResourceManager->ActivateL();
    iIsReadyToDraw = 0;

    // Resource file isn't needed anymore so it can be closed.
    CleanupStack::PopAndDestroy(); // resourceLoader   
    
    TInt pngErr( KErrNone );
    TInt pngSupported( 0 );
    CRepository* repository = CRepository::NewL( KCRUidNumberGrouping );
    pngErr = repository->Get( KNumberGrouping, pngSupported );
    delete repository;

    if ( KErrNone == pngErr && pngSupported != 0 )
        {
        iNumberGrouping = CPNGNumberGrouping::NewL(KBubbleLabelMaxLength);
        }
#ifdef RD_UI_TRANSITION_EFFECTS_PHASE2
    // Register the number entry popup in the transition server
    GfxTransEffect::Register( iNumberEntry, KGfxNumberEntryPopupUid, EFalse );

    CAknTransitionUtils::SetData( (TInt) this, 0 );
#endif
    }

// Destructor
EXPORT_C CBubbleManager::~CBubbleManager()
    {
#ifdef RD_UI_TRANSITION_EFFECTS_PHASE2
    CAknTransitionUtils::RemoveData( (TInt) this );
    GfxTransEffect::Deregister( iNumberEntry );
#endif
    AknsUtils::DeregisterControlPosition( this );
    
    delete iMutedImage;
    
    if ( iActiveHeaders )
        {
        iActiveHeaders->Reset( );
        delete iActiveHeaders;
        }

    delete iLabelBuf;
    
    if ( iActivePlaces )
        {
        iActivePlaces->Reset( );
        delete iActivePlaces;
        }
    
    if ( iBubblePlaces )
        {
        iBubblePlaces->ResetAndDestroy() ;
        delete iBubblePlaces;
        }
    
    delete iNumberEntry;
    
    delete iConfHeader;
    
    
    if ( iCallHeaders )
        {
        iCallHeaders->ResetAndDestroy() ;
        delete iCallHeaders;
        }

    delete iResourceManager;
    
    delete iImageManager;
    
    delete iCustomManager;
    
    delete iNumberGrouping;

    delete iCallObjectManager;
    
    delete iMainPaneControl;

    delete iVideoController;
    
    delete iTouchPane;
    }

// ---------------------------------------------------------------------------
// CBubbleManager::SizeChanged()
// called by framework when the view size is changed
//  
// ---------------------------------------------------------------------------
//
void CBubbleManager::SizeChanged()
    {
    AknsUtils::RegisterControlPosition( this );
    
    if ( iConfigFlags & EBMTouchCallhandling )
        {
        if ( iTouchPane->IsVisible() )
            {
            TInt variety = 0;
            // Popups
            TAknLayoutRect windowsPane;        
            windowsPane.LayoutRect( 
                Rect(), 
                BubbleLayout6::call6_windows_pane(variety));

            iMainPaneControl->SetRect( windowsPane.Rect() );
                
            TAknLayoutRect bubblePane;        
            bubblePane.LayoutRect( 
                Rect(), 
                BubbleLayout4::call4_windows_pane(variety));

            DoLayoutCall4Bubbles( bubblePane.Rect() );    
            }
        else
            {
            iMainPaneControl->SetRect( Rect() );
            
            DoLayoutCall4Bubbles( Rect() );    
            }            
            
                    
        // Muted icon
        AknLayoutUtils::LayoutImage( 
            iMutedImage, Rect(), 
            BubbleLayout::main_pane_elements_muted( EFalse ) );    
        
        // Touch pane

        TInt variety = 0;
        AknLayoutUtils::LayoutControl( 
            iTouchPane, 
            Rect(), 
            BubbleLayout6::call6_button_grp_pane(variety) );
        
        }
    else if ( iConfigFlags & EBMCallObjectDisplay )
        {
        // call object layout
        iMainPaneControl->SetRect( Rect() );
        DoLayoutCall2Bubbles();    
        }        
    else
        {
        // old thumbnail layout
        DoLayoutCall1Bubbles();    
        }
    
    // Layout number entry
    TInt neIndex;
    switch ( iNumberEntry->Place() )
        {
        case CBubbleOutlookNumberEntry::ENEBottomRight:
            neIndex = 1;
            break;
        case CBubbleOutlookNumberEntry::ENEIdleState:
            neIndex = 4;
            break;
        default: // CBubbleOutlookNumberEntry::ENEBottom
            neIndex = 0;
            break;                        
        }

    AknLayoutUtils::LayoutControl( 
        iNumberEntry, Rect( ), 
        BubbleLayout::popup_number_entry_window( neIndex ) );        
    
    // Layout muted icon
    if ( !(iConfigFlags & EBMTouchCallhandling) )
        {
        AknLayoutUtils::LayoutImage( 
            iMutedImage, Rect(), 
            BubbleLayout::main_pane_elements_muted( iNumberEntry->IsUsed() ) );        
        }
    }

// ---------------------------------------------------------------------------
// CBubbleManager::DoLayoutCall1Bubbles
// ---------------------------------------------------------------------------
//
void CBubbleManager::DoLayoutCall1Bubbles()
    {
    CBubblePlace* tempBubblePlace = NULL;
    TUint8 placeCount( TUint8( iBubblePlaces->Count( ) ) );
    TAknWindowLineLayout placeLayout;
    
    for ( TUint8 index = 0 ; index < placeCount ; index++  ) 
        {
        tempBubblePlace = iBubblePlaces->At( index );
        CBubblePlace::TPhoneBubblePlace place = 
            tempBubblePlace->GetBubblePlace();
                    
        switch ( place )
            {
            case CBubblePlace::EBottom:
                placeLayout = BubbleLayout::popup_call_audio_in_window(1);    
                break;
                
            case CBubblePlace::EBottomRightActive:
                placeLayout = BubbleLayout::popup_call_audio_second_window(1);
                break;
                
            case CBubblePlace::EBottomRightHeld:
                placeLayout = BubbleLayout::popup_call_audio_second_window(2);
                break;
                
            case CBubblePlace::EMiddleActive:
                placeLayout = BubbleLayout::popup_call_audio_first_window(3);
                break;
                
            case CBubblePlace::EMiddleHeld:
                if ( iBubblePlaces->At( CBubblePlace::EBottomCnap )->IsUsed() )
                    {
                    placeLayout = BubbleLayout::popup_call_audio_first_window(7);
                    }
                else
                    {
                    placeLayout = BubbleLayout::popup_call_audio_first_window(2);
                    }
                break;
                
            case CBubblePlace::ETopLeft:
                placeLayout = BubbleLayout::popup_call_audio_first_window(4);
                break;
                
            case CBubblePlace::ETopRight:
                placeLayout = BubbleLayout::popup_call_audio_second_window(3);
                break;
                
            case CBubblePlace::EHide:
                {
                TInt v = AknLayoutUtils::ScalableLayoutInterfaceAvailable() ?
                         5 : 6;
                placeLayout = BubbleLayout::popup_call_audio_first_window(v);
                }
                break;
                
            case CBubblePlace::E2InConf:
                placeLayout = BubbleLayout::popup_call_audio_conf_window(1);
                break;
                
            case CBubblePlace::E3InConf:
                placeLayout = BubbleLayout::popup_call_audio_conf_window(2);
                break;
                
            case CBubblePlace::E4InConf:
                placeLayout = BubbleLayout::popup_call_audio_conf_window(3);
                break;
                
            case CBubblePlace::E5InConf:
                placeLayout = BubbleLayout::popup_call_audio_conf_window(4);
                break;
                
            case CBubblePlace::EBottomCnap:
                placeLayout = BubbleLayout::popup_call_audio_in_window(3);
                break;
                
            case CBubblePlace::EMiddleCnap:
                placeLayout = BubbleLayout::popup_call_audio_in_window(4);
                break;
                
            case CBubblePlace::EVideo:
                placeLayout = BubbleLayout3::popup_call3_audio_in_window(0);
                break;

            case CBubblePlace::EVideoCnap:
                placeLayout = BubbleLayout3::popup_call3_audio_in_window(0);
                break;                                                 

            case CBubblePlace::EBottomText: // not active
            case CBubblePlace::EBottomImage: // not active
            case CBubblePlace::EBottomRightCnap: // not active
            case CBubblePlace::EBottomImageCnap: // not active
                placeLayout = BubbleLayout::popup_call_audio_in_window(1);
                break;                 
                
            default:
                Panic( EBMPanicPlaceEnumerationDoesNotExist );
                break;
            }
        
        AknLayoutUtils::LayoutControl( tempBubblePlace , Rect() , placeLayout );
        }    
    }
    
// ---------------------------------------------------------------------------
// CBubbleManager::DoLayoutCall2Bubbles
// ---------------------------------------------------------------------------
//
void CBubbleManager::DoLayoutCall2Bubbles()
    {
    CBubblePlace* tempBubblePlace = NULL;
    TUint8 placeCount( TUint8( iBubblePlaces->Count( ) ) );
    TAknWindowLineLayout placeLayout;
    
    for ( TUint8 index = 0 ; index < placeCount ; index++  ) 
        {
        tempBubblePlace = iBubblePlaces->At( index );
        CBubblePlace::TPhoneBubblePlace place = 
            tempBubblePlace->GetBubblePlace();
        
        switch ( place )
            {
            case CBubblePlace::EBottom:
                placeLayout = BubbleLayout2::popup_call2_audio_in_window(0);
                break;
                
            case CBubblePlace::EBottomRightActive:
                placeLayout = BubbleLayout2::popup_call2_audio_second_window(0);
                break;
                
            case CBubblePlace::EBottomRightHeld:
                placeLayout = BubbleLayout2::popup_call2_audio_second_window(1);    
                break;
                
            case CBubblePlace::EMiddleActive:
                placeLayout = BubbleLayout2::popup_call2_audio_first_window(2);    
                break;
                
            case CBubblePlace::EMiddleHeld:
                if ( iBubblePlaces->At( CBubblePlace::EBottomRightCnap )->IsUsed() )
                    {
                    placeLayout = BubbleLayout2::popup_call2_audio_first_window(18);
                    }
                else
                    {
                    placeLayout = BubbleLayout2::popup_call2_audio_first_window(1);    
                    }                    
                break;
                
            case CBubblePlace::ETopLeft:
                placeLayout = BubbleLayout2::popup_call2_audio_first_window(3);    
                break;
                
            case CBubblePlace::ETopRight:
                placeLayout = BubbleLayout2::popup_call2_audio_second_window(2);    
                break;
                
            case CBubblePlace::EHide:
                placeLayout = BubbleLayout2::popup_call2_audio_first_window(4);    
                break;
                
            case CBubblePlace::E2InConf:
                placeLayout = BubbleLayout2::popup_call2_audio_conf_window(0);    
                break;
                
            case CBubblePlace::E3InConf:
                placeLayout = BubbleLayout2::popup_call2_audio_conf_window(1);       
                break;
                
            case CBubblePlace::E4InConf:
                placeLayout = BubbleLayout2::popup_call2_audio_conf_window(2);        
                break;
                
            case CBubblePlace::E5InConf:
                placeLayout = BubbleLayout2::popup_call2_audio_conf_window(3);        
                break;
                
            case CBubblePlace::EBottomCnap:
                placeLayout = BubbleLayout2::popup_call2_audio_in_window(2);    
                break;
                
            case CBubblePlace::EMiddleCnap:
                placeLayout = BubbleLayout2::popup_call2_audio_in_window(3);    
                break;
                
            case CBubblePlace::EBottomText:
                placeLayout = BubbleLayout2::popup_call2_audio_in_window(8);
                break;                

            case CBubblePlace::EBottomImage:
                placeLayout = BubbleLayout2::popup_call2_audio_first_window(0);
                break;                
                
            case CBubblePlace::EVideo:
                placeLayout = BubbleLayout3::popup_call3_audio_in_window(0);
                break;

            case CBubblePlace::EVideoCnap:
                placeLayout = BubbleLayout3::popup_call3_audio_in_window(0);
                break;

            case CBubblePlace::EBottomRightCnap:
                placeLayout = BubbleLayout2::popup_call2_audio_wait_window(9);    
                break;
                
            case CBubblePlace::EBottomImageCnap:
                placeLayout = BubbleLayout2::popup_call2_audio_first_window(0);    
                break;                
                
            default:
                Panic( EBMPanicPlaceEnumerationDoesNotExist );
                break;
            }
       
        AknLayoutUtils::LayoutControl( tempBubblePlace , Rect() , placeLayout );
        }    
    }    

// ---------------------------------------------------------------------------
// CBubbleManager::DoLayoutCall4Bubbles
// ---------------------------------------------------------------------------
//
void CBubbleManager::DoLayoutCall4Bubbles( const TRect& aWindowPane )
    {
    CBubblePlace* tempBubblePlace = NULL;
    TUint8 placeCount( TUint8( iBubblePlaces->Count( ) ) );
    TAknWindowLineLayout placeLayout;
    TBool initCall = InitializingCall();
    
    for ( TUint8 index = 0 ; index < placeCount ; index++  ) 
        {
        tempBubblePlace = iBubblePlaces->At( index );
        CBubblePlace::TPhoneBubblePlace place = 
            tempBubblePlace->GetBubblePlace();
        
        switch ( place )
            {
            case CBubblePlace::EBottom:
            case CBubblePlace::EBottomCnap:
                if ( initCall )
                    {
                    // in / out
                    placeLayout = 
                        BubbleLayout4::popup_call4_audio_in_window(4);
                    }
                else
                    {
                    // incall
                    placeLayout =
                        BubbleLayout6::popup_call6_audio_first_window(0);    
                    }
                
                break;
                
            // in / out with text
            case CBubblePlace::EBottomText:
                placeLayout = BubbleLayout2::popup_call2_audio_in_window(8);
                break;                

            // in with video                
            case CBubblePlace::EVideo:
            case CBubblePlace::EVideoCnap:
                placeLayout = BubbleLayout3::popup_call3_audio_in_window(0);
                break;

            // first
            case CBubblePlace::EBottomImage:
            case CBubblePlace::EBottomImageCnap:
                placeLayout = BubbleLayout6::popup_call6_audio_first_window(0);
                break;                

            // two calls - active
            case CBubblePlace::EBottomRightActiveTouch:
            case CBubblePlace::EBottomRightActiveCnapTouch:                
                placeLayout = BubbleLayout4::popup_call4_audio_second_window(0);
                break;
            // two calls - active                
            case CBubblePlace::EMiddleHeldTouch:                
                placeLayout = BubbleLayout4::popup_call4_audio_first_window(1);          
                break;                
            
            // three calls - held
            case CBubblePlace::ETopRight:
                placeLayout = BubbleLayout4::popup_call4_audio_second_window(1);    
                break;
            // two or three calls - active
            case CBubblePlace::ETopLeft:
                placeLayout = BubbleLayout4::popup_call4_audio_first_window(2);    
                break;
            // two or three calls - waiting    
            case CBubblePlace::EBottomRightActive:
            case CBubblePlace::EBottomRightCnap:
            case CBubblePlace::EBottomRightHeld:
                placeLayout = BubbleLayout4::popup_call4_audio_wait_window(0);    
                break;
            
            // conference
            case CBubblePlace::E2InConf:
                placeLayout = BubbleLayout4::popup_call4_audio_conference_window(3);    
                break;
                
            case CBubblePlace::E3InConf:
                placeLayout = BubbleLayout4::popup_call4_audio_conference_window(2);       
                break;
                
            case CBubblePlace::E4InConf:
                placeLayout = BubbleLayout4::popup_call4_audio_conference_window(1);        
                break;
                
            case CBubblePlace::E5InConf:
                placeLayout = BubbleLayout4::popup_call4_audio_conference_window(0);        
                break;

            // not used in touch
            case CBubblePlace::EMiddleActive:
            case CBubblePlace::EMiddleHeld:
            case CBubblePlace::EMiddleCnap:
            case CBubblePlace::EHide:
                continue;
                
            default:
                Panic( EBMPanicPlaceEnumerationDoesNotExist );
                break;
            }
       
        AknLayoutUtils::LayoutControl( tempBubblePlace , aWindowPane , placeLayout );
        }    
    }    


// ---------------------------------------------------------------------------
// CBubbleManager::PositionChanged
// ---------------------------------------------------------------------------
//
void CBubbleManager::PositionChanged()
    {
    AknsUtils::RegisterControlPosition( this );
    }

// ---------------------------------------------------------------------------
// CBubbleManager::CountComponentControls() const
//
//  Controls: NumberEntry, MutedImage and ActivePlaces
// ---------------------------------------------------------------------------
//
TInt CBubbleManager::CountComponentControls() const
    {
    if ( ( iConfigFlags & EBMTouchCallhandling ) && 
         ( iTouchPane->IsVisible() ) )
        {
        return 3 + iActivePlaces->Count(); // ne + muted + touchpane + places
        }
    else
        {
        return 2 + iActivePlaces->Count(); // ne + muted + places
        }       
    }

// ---------------------------------------------------------------------------
// CBubbleManager::ComponentControl(TInt aIndex) const
//
//  First give active places. Then possible NE, muted-image and AnimStarter
// ---------------------------------------------------------------------------
//
CCoeControl* CBubbleManager::ComponentControl( TInt aIndex ) const
    {
    TInt activePlacesCount = iActivePlaces->Count();

    // If too high value, return NULL
    if ( aIndex >= activePlacesCount + 3 )
        {
        return NULL;
        }

    if ( aIndex < activePlacesCount )
        {
        return iActivePlaces->At( aIndex  );
        }

    if ( aIndex == activePlacesCount )
        {
        return iNumberEntry;
        }

    if ( iConfigFlags & EBMTouchCallhandling )
        {
        if ( aIndex == ( activePlacesCount + 1 ) )
            {
            return iMutedImage;    
            }

        return iTouchPane;
        }
    else
        {
        return iMutedImage;    
        }
    }

// ---------------------------------------------------------------------------
// CBubbleManager::Draw
// ---------------------------------------------------------------------------
//
void CBubbleManager::Draw( const TRect& aRect ) const
    {
    // clear the backround area.
    CWindowGc& gc = SystemGc();
    gc.SetBrushColor( AKN_LAF_COLOR( BubbleLayout::LayoutColourWhite() ) );

    // Draw just the bitmaps
    if ( iMainPaneControl &&
         iMainPaneControl->IsVisible() )
        {
        // main pane image
        iMainPaneControl->DrawBitmaps( aRect );
        }
    
    for ( TInt i = 0 ; i < iActivePlaces->Count() ; i++ )
        {
        iActivePlaces->At(i)->GetBubbleOutlook().DrawBitmaps( gc );
        }
    
    if ( iNumberEntry->IsUsed() )
        {
        iNumberEntry->DrawBitmaps(  gc );
        }
    }
    
// ---------------------------------------------------------------------------
//  CBubbleManager::MakeVisible
//  
// 
// ---------------------------------------------------------------------------
//
void CBubbleManager::MakeVisible( TBool aVisible )
    {
    CCoeControl::MakeVisible( aVisible );
    
    if ( aVisible )    
        {
        StartAnimations();    
        }        
    }

// ---------------------------------------------------------------------------
// CBubbleManager::StartChanges
//  
//  Reset all places so there won't be anything wrong left.
// ---------------------------------------------------------------------------
//
EXPORT_C void CBubbleManager::StartChanges( )
    {
    BM_TRACE_( "[BUBBLEMANAGER] CBubbleManager::StartChanges" );
    iIsReadyToDraw++;
    if ( iIsReadyToDraw > 1 ) // is this called already?
        {
        return;
        }
    
    // clear all used areas
    CBubblePlace* tempBubblePlace = NULL;
    TInt placeCount = iBubblePlaces->Count();
    for ( TUint8 index = 0 ; index < placeCount ; index++  ) 
        {
        tempBubblePlace = iBubblePlaces->At( index );
        if ( tempBubblePlace->IsUsed() )
            {
            Window().Invalidate( tempBubblePlace->Rect() );
            tempBubblePlace->SetIsUsed( EFalse );
            }
        }
    
    iActivePlaces->Delete( 0 , iActivePlaces->Count() ); // do not compress!!

    if ( iTouchPane )
        {
        iTouchPane->SetUpdating( ETrue );                
        }
    }

// ---------------------------------------------------------------------------
// CBubbleManager::EndChanges
//  
//  Check the situation and decide which outlook to take. 
// ---------------------------------------------------------------------------
//
EXPORT_C void CBubbleManager::EndChanges( )
    {
    BM_TRACE_( "[BUBBLEMANAGER] CBubbleManager::EndChanges" );
     
    __ASSERT_ALWAYS( 
        iIsReadyToDraw > 0 , Panic( EBMPanicErrorInStartAndEndChanges ) );

    iIsReadyToDraw--;
    if ( iIsReadyToDraw != 0 )
        {
        return;
        }

    if ( iConfHeader->IsExpanded() )
        {
        BuildExpandedConference();
        }
    else if ( iNumberEntry->IsUsed() && iNumberEntry->IsVisible() )
        {
        BuildBubblesWithNE();
        }
    else if ( ( iConfigFlags & EBMTouchCallhandling ) )
        {
        BuildBubblesForTouch();    
        }
    else
        {
        BuildBubbles();
        }

    // Stop video if playing and video bubble is not active.
    if ( ( iVideoFlags & EBMVideoPlayingVideo ) &&
         !( iVideoFlags & EBMVideoBubbleActive ) )
        {
        StopAndDeleteVideoPlayer();
        }                

    // Touch pane        
    if ( iConfigFlags & EBMTouchCallhandling )
        {
        if ( iTouchPaneSetVisible &&        // Client sets
             iActiveHeaders->Count() > 0 && // Incall situation                   
             !iNumberEntry->IsVisible() )   
            {
            iTouchPane->MakeVisible( ETrue );
            }
        else
            {
            iTouchPane->MakeVisible( EFalse );    
            }    
        }

    if ( ( iConfigFlags & EBMCallObjectDisplay ) && 
         iActiveHeaders->Count() > 0 &&
         !( iVideoFlags & EBMVideoBubbleActive ) )
        {
        // Image is created only for incoming & outgoing
        // calls, check is inside method.
        BuildMainPaneImage();
        }
    
    SizeChanged();
    
    // Invalidate the new areas.
    for ( TInt i = 0 ; i < iActivePlaces->Count() ; i++ )
        {
        iActivePlaces->At(i)->GetBubbleOutlook().DrawDeferred();
        }
    if ( iNumberEntry->IsUsed() )
        {
        iNumberEntry->DrawDeferred();
        iNumberEntry->GetEditor()->DrawDeferred();
        }
        
    if ( iTouchPane )
        {
        iTouchPane->SetUpdating( EFalse );
        }
            
#ifdef RD_UI_TRANSITION_EFFECTS_PHASE2
    TBool calledBegin = (TBool) CAknTransitionUtils::GetData( ( TInt ) this );
    // Only draw parent control if no number entry exists, the number entry is
    // used or if begin has not been called.
    if ( iNumberEntry->IsUsed() || !calledBegin )
        {
        // Do redraw. Do not use DrawDeferred() here!
        iParentControl->DrawNow();
        }
#else
    // Do redraw. Do not use DrawDeferred() here!
    iParentControl->DrawNow();
#endif
    // Start animations. 
    StartAnimations();
    
    // Start video if video bubble is active.
    if ( ( iVideoFlags & EBMVideoBubbleActive ) &&
         !( iVideoFlags & EBMVideoPlayingVideo ) )
        {
        CreateVideoPlayerAndPlay();
        }
#ifdef RD_UI_TRANSITION_EFFECTS_PHASE2
    if ( calledBegin )
        {
        // Initialize transition effect
        CAknTransitionUtils::SetAllParents( iNumberEntry );
        GfxTransEffect::SetDemarcation( iNumberEntry, iNumberEntry->Rect() );
        GfxTransEffect::End( iNumberEntry );

        // Reset begin call reminder
        CAknTransitionUtils::SetData( (TInt) this, 0 );
        }
    // SetFocus here instead of in SetNumberEntryVisible
    if ( iNumberEntry->IsVisible() )
        {
        iNumberEntry->SetFocus( ETrue );
        }
#endif
    return;
    }


// ---------------------------------------------------------------------------
//  CBubbleManager::SetActivePlace
//  
//  This helps the EndChanges() to make bubble places active.
// ---------------------------------------------------------------------------
//
void CBubbleManager::SetActivePlace( 
    const CBubblePlace::TPhoneBubblePlace& aPlace , 
    CBubbleHeader& aHeader )
    {
    CBubblePlace* place = iBubblePlaces->At( aPlace );
    place->SetIsUsed( ETrue );
    CBubbleOutlook& bubbleOutlook = place->GetBubbleOutlook( );
    bubbleOutlook.ReadBubbleHeader( aHeader );
    iActivePlaces->InsertL( iActivePlaces->Count( ), place ); // Can't leave
    }


void CBubbleManager::BuildExpandedConference()
    {
    iNumberEntry->SetPlace( CBubbleOutlookNumberEntry::ENENone );
    switch ( iConfHeader->RowCount() )
        {
        // Cases mean number of calls in conference.
        case 2:
            SetActivePlace( CBubblePlace::E2InConf , *iConfHeader );
            break;
        case 3:
            SetActivePlace( CBubblePlace::E3InConf , *iConfHeader );
            break;
        case 4:
            SetActivePlace( CBubblePlace::E4InConf , *iConfHeader );
            break;
        case 5:
            SetActivePlace( CBubblePlace::E5InConf , *iConfHeader );
            break;
        default:
            Panic( EBMPanicPlaceEnumerationDoesNotExist );
            break;
        }
    return;
    }


// ---------------------------------------------------------------------------
//  CBubbleManager::FindNextDrawableHeader
//  
// 
// ---------------------------------------------------------------------------
//
void CBubbleManager::FindNextDrawableHeader( TUint8& aIndex )
    {
    while( iActiveHeaders->At( aIndex )->IsInConference() ) 
        {
        aIndex++;
        }
    }

// ---------------------------------------------------------------------------
//  CBubbleManager::ShownHeaderCount
// ---------------------------------------------------------------------------
//
EXPORT_C TUint8 CBubbleManager::ShownHeaderCount() const
    {
    // We have to find headers that are really shown. 
    // That means headers which are 
    // used and are NOT in conference. 
    TUint8 bubbleAmount = 0;
    TUint activeCount = iActiveHeaders->Count();
    for (TUint8 i = 0 ; i < activeCount ; i++ )
        {
        if ( !iActiveHeaders->At(i)->IsInConference() )
            {
            bubbleAmount++;
            }
        }
    return bubbleAmount;
    }

// ---------------------------------------------------------------------------
//  CBubbleManager::BuildBubbles
//  
// 
// ---------------------------------------------------------------------------
//
void CBubbleManager::BuildBubbles()
    { 
    const TUint8 bubbleAmount = ShownHeaderCount();
    iVideoFlags &= ~EBMVideoBubbleActive;
    
    // Without number entry.
    switch ( bubbleAmount )
        {
        case 0:
            break;
        case 1:
            {
            TUint8 index = 0;
            FindNextDrawableHeader( index );
            CBubbleHeader* header = iActiveHeaders->At( index );
            TPhoneCallState callState = header->CallState();
            TBool callObjectText( 
                  ( iConfigFlags & EBMCallObjectDisplay ) && 
                  ( header->CallObjectText().Length() > 0) );
            TBool callObjectImage( 
                  ( iConfigFlags & EBMCallObjectDisplay ) && 
                  header->CallObjectImage() );                  
            
            TInt cnapLength = header->CNAP().Length();

            // Video
            if ( callState == EIncoming && 
                 iVideoFlags & EBMVideoSetAsRingTone )
                {
                if ( cnapLength ) // 3-lines
                    {
                    SetActivePlace( CBubblePlace::EVideoCnap, 
                                    *header );
                                    iVideoFlags |= EBMVideoBubbleActive;
                    }
                else
                    {
                    SetActivePlace( CBubblePlace::EVideo, 
                                    *header );
                                    iVideoFlags |= EBMVideoBubbleActive;    
                    }                            
                }
            // Caller text                    
            else if ( callObjectText && ( callState == EIncoming || 
                                          callState == EOutgoing ||
                                          callState == EAlerting ||
                                          callState == EAlertToDisconnected ) )
                {
                SetActivePlace( CBubblePlace::EBottomText,
                                *header );                            
                }
            // Caller image
            else if ( callObjectImage && ( callState == EActive ||
                                           callState == EOnHold ||
                                           callState == EDisconnected ) )
                {
                CBubbleHeader::TBMCallObjectImageType imageType = 
                    header->CallObjectImageType();

                // Theme image -> use normal bubble                        
                if ( imageType == CBubbleHeader::EThemeImage ||
                     imageType == CBubbleHeader::ETextBasedImage )
                    {
                    if ( cnapLength ) // 3-lines
                        {
                        SetActivePlace( CBubblePlace::EBottomCnap,
                                        *header );    
                        }
                    else
                        {
                        SetActivePlace( CBubblePlace::EBottom,
                                        *header );    
                        }                                
                    }
                // Caller image -> use bigger bubble                                                    
                else
                    {
                    TSize imageSize = header->CallObjectImage()->SizeInPixels();
                    TSize tinySize = CallObjectImageIncallSize( ETrue );
                    TBool tiny = imageSize.iHeight < tinySize.iHeight;
                    iBottomImageVariety = tiny ? 0 : 4;
                    
                    if ( cnapLength ) // 3-lines
                        {
                        CBubblePlace::TPhoneBubblePlace place = tiny ?
                            CBubblePlace::EBottomCnap : CBubblePlace::EBottomImageCnap;
                        
                        SetActivePlace( place,
                                        *header );                        
                        }
                    else 
                        {
                        CBubblePlace::TPhoneBubblePlace place = tiny ?
                            CBubblePlace::EBottom : CBubblePlace::EBottomImage;
                        
                        SetActivePlace( place,
                                        *header );                        
                        }                           
                    }
                }
            // Basic
            else
                {
                if ( cnapLength ) // 3-lines
                    {
                    SetActivePlace( CBubblePlace::EBottomCnap,
                                    *header );
                    }
                else
                    {
                    SetActivePlace( CBubblePlace::EBottom ,
                                    *header );
                    }    
                }
            break;
            }
        case 2:
            {
            // first call: middle held/active
            TUint8 index = 0;
            FindNextDrawableHeader( index );
            TPhoneCallState callState = 
                iActiveHeaders->At( index )->CallState();
            TInt cnapLength = iActiveHeaders->At( index )->CNAP().Length();                

            // second call: bottom held/active
            TUint8 index2 = TUint8(index + 1);
            FindNextDrawableHeader( index2 );
            TInt cnapLength2 = iActiveHeaders->At( index2 )->CNAP().Length();
            TPhoneCallState callState2 = 
                iActiveHeaders->At( index2 )->CallState();

            
            CBubblePlace::TPhoneBubblePlace place;
            
            if ( callState == EOnHold  )
                {
                place = CBubblePlace::EMiddleHeld;
                }
            else if ( cnapLength  ) // and not held
                {
                place = CBubblePlace::EMiddleCnap;    
                }
            else
                {
                place = CBubblePlace::EMiddleActive;
                }                
            
            CBubblePlace::TPhoneBubblePlace place2 = 
                CBubblePlace::EBottomRightActive;
            
            if ( callState2 == EOnHold )
                {
                place2 = CBubblePlace::EBottomRightHeld;
                }
            else if ( cnapLength2 ) // and not held
                {
                if ( cnapLength == 0 ||
                     callState2 == EOutgoing ||
                     callState2 == EAlerting || 
                     callState2 == EAlertToDisconnected ||
                     callState2 == EWaiting ||
                     callState2 == EActive )                
                    {
                    // bottom shows 3-lines
                    place = CBubblePlace::EMiddleHeld;
                    place2 = CBubblePlace::EBottomRightCnap;
                    }
                else
                    {
                    place2 = CBubblePlace::EBottomRightHeld;
                    }                    
                }
            else
                {
                place2 = CBubblePlace::EBottomRightActive;    
                }                
            
            // set drawing order
            if ( callState2 > callState )
                {
                SetActivePlace( place , *iActiveHeaders->At( index ) );
                SetActivePlace( place2 , *iActiveHeaders->At( index2 ) );
                }
            else
                {
                SetActivePlace( place2 , *iActiveHeaders->At( index2 ) );
                SetActivePlace( place , *iActiveHeaders->At( index ) );
                }                    
            }
            break;
        case 3:
            {
            TUint8 index = 0;
            FindNextDrawableHeader( index );
            
            TUint8 index2 = TUint8( index + 1 );
            FindNextDrawableHeader( index2 );
            
            if ( iActiveHeaders->At( index2 )->CallState( ) 
                > iActiveHeaders->At( index )->CallState( ) )
                {
                SetActivePlace( 
                    CBubblePlace::ETopLeft , *iActiveHeaders->At( index ) );
                SetActivePlace( 
                    CBubblePlace::ETopRight , *iActiveHeaders->At( index2 ) );
                }
            else
                {
                SetActivePlace( 
                    CBubblePlace::ETopRight , *iActiveHeaders->At( index2 ) );
                SetActivePlace( 
                    CBubblePlace::ETopLeft , *iActiveHeaders->At( index ) );
                }
            
            
            index = TUint8( index2 + 1 );
            FindNextDrawableHeader( index );
            SetActivePlace( 
                CBubblePlace::EBottom , *iActiveHeaders->At( index ) );
            break;
            }
        default:
            Panic( EBMPanicInvalidNumberOfHeaders );
            break;
        }
    }
    
// ---------------------------------------------------------------------------
//  CBubbleManager::BuildBubblesWithNE
//  
// 
// ---------------------------------------------------------------------------
//
void CBubbleManager::BuildBubblesWithNE()
    { 
    const TUint8 bubbleAmount = ShownHeaderCount();
    iVideoFlags &= ~EBMVideoBubbleActive;
    
    switch ( bubbleAmount )
        {
        case 0:
            if ( iConfigFlags & EBMBigNEInIdleState )
                {
                iNumberEntry->SetPlace(
                    CBubbleOutlookNumberEntry::ENEIdleState );
                }
            else
                {
                iNumberEntry->SetPlace(
                    CBubbleOutlookNumberEntry::ENEBottom );
                }                
            break;
        case 1:
            {
            // NE is placed according to it's activation time compared 
            // to call's activation.
            TUint8 index = 0;
            FindNextDrawableHeader( index );
            TInt cnapLength = iActiveHeaders->At( index )->CNAP().Length();
            TPhoneCallState callState = 
                iActiveHeaders->At( index )->CallState();

            CBubbleOutlook& bubbleOutlook = 
                iBubblePlaces->
                    At( CBubblePlace::EMiddleHeld )->GetBubbleOutlook( );
            CBubbleOutlookMiddle& bubbleMiddle =  
                static_cast< CBubbleOutlookMiddle& >( bubbleOutlook ) ;
            bubbleMiddle.SetIsUnder( ETrue );

            if ( cnapLength )
                {
                SetActivePlace( CBubblePlace::EMiddleCnap ,
                                *iActiveHeaders->At( index ) );
                }
            else
                {
                SetActivePlace( CBubblePlace::EMiddleHeld ,
                                *iActiveHeaders->At( index ) );
                }
            iNumberEntry->SetPlace( CBubbleOutlookNumberEntry::ENEBottomRight );
            break;
            }
        case 2:
            {
            // NE goes top left. Calls go like second and waiting 
            // call when there are three calls.
            iNumberEntry->SetPlace( CBubbleOutlookNumberEntry::ENEBottom );
                        
            // first call: middle held/active
            TUint8 index = 0;
            FindNextDrawableHeader( index );
            CBubblePlace::TPhoneBubblePlace place = CBubblePlace::ETopLeft;
            
            // second call: bottom held/active
            TUint8 index2 = TUint8(index + 1);
            FindNextDrawableHeader( index2 );
            CBubblePlace::TPhoneBubblePlace place2 = CBubblePlace::ETopRight;
                        
            // set drawing order
            if ( iActiveHeaders->At( index2 )->CallState( ) 
                > iActiveHeaders->At( index )->CallState( ) )
                {
                SetActivePlace( place , *iActiveHeaders->At( index ) );
                SetActivePlace( place2 , *iActiveHeaders->At( index2 ) );
                }
            else
                {
                SetActivePlace( place2 , *iActiveHeaders->At( index2 ) );
                SetActivePlace( place , *iActiveHeaders->At( index ) );
                }                    
            
            break;
            }
        case 3:
            {
            iNumberEntry->SetPlace( CBubbleOutlookNumberEntry::ENEBottom );
            
            TUint8 index = 0;
            FindNextDrawableHeader( index );
            
            TUint8 index2 = TUint8( index + 1 );
            FindNextDrawableHeader( index2 );
                        
            if ( iActiveHeaders->At( index2 )->CallState( ) 
                > iActiveHeaders->At( index )->CallState( ) )
                {
                SetActivePlace( 
                    CBubblePlace::EHide , *iActiveHeaders->At( index ) );
                SetActivePlace( 
                    CBubblePlace::ETopLeft , *iActiveHeaders->At( index2 ) );
                }
            else
                {
                SetActivePlace( 
                    CBubblePlace::EHide , *iActiveHeaders->At( index2 ) );
                SetActivePlace( 
                    CBubblePlace::ETopLeft , *iActiveHeaders->At( index ) );
                }
            
            index = TUint8( index2 + 1 );
            FindNextDrawableHeader( index );
            SetActivePlace( 
                CBubblePlace::ETopRight , *iActiveHeaders->At( index ) );
            break;
            }
        default:
            Panic( EBMPanicInvalidNumberOfHeaders );
            break;
        }
    return;           
    }

// ---------------------------------------------------------------------------
//  CBubbleManager::BuildBubblesForTouch
//  
// 
// ---------------------------------------------------------------------------
//
void CBubbleManager::BuildBubblesForTouch()
    {
    const TUint8 bubbleAmount = ShownHeaderCount();
    iVideoFlags &= ~EBMVideoBubbleActive;
    
    // Without number entry.
    switch ( bubbleAmount )
        {
        case 0:
            break;
        case 1:
            {
            TUint8 index = 0;
            FindNextDrawableHeader( index );
            CBubbleHeader* header = iActiveHeaders->At( index );
            TPhoneCallState callState = header->CallState();
            TBool callObjectText( 
                  ( iConfigFlags & EBMCallObjectDisplay ) && 
                  ( header->CallObjectText().Length() > 0) );
            TBool callObjectImage( 
                  ( iConfigFlags & EBMCallObjectDisplay ) && 
                  header->CallObjectImage() );                  
            
            TInt cnapLength = header->CNAP().Length();

            // Video
            if ( iVideoFlags & EBMVideoSetAsRingTone &&
                 ( callState == EIncoming || callState == EAlertToDisconnected ) )
                {
                if ( cnapLength ) // 3-lines
                    {
                    SetActivePlace( CBubblePlace::EVideoCnap, 
                                    *header );
                                    iVideoFlags |= EBMVideoBubbleActive;
                    }
                else
                    {
                    SetActivePlace( CBubblePlace::EVideo, 
                                    *header );
                                    iVideoFlags |= EBMVideoBubbleActive;    
                    }                            
                }
            // Caller text                    
            else if ( callObjectText && ( callState == EIncoming || 
                                          callState == EOutgoing ||
                                          callState == EAlerting ||
                                          callState == EAlertToDisconnected ) )
                {
                SetActivePlace( CBubblePlace::EBottomText,
                                *header );                            
                }
            // Incall
            else if ( ( callState == EActive ||
                        callState == EOnHold ||
                        callState == EDisconnected ) )                
                {
                CBubbleHeader::TBMCallObjectImageType imageType = 
                    header->CallObjectImageType();
                
                if ( callObjectImage && 
                     ( imageType == CBubbleHeader::EGalleryImage ) )
                    {
                    TSize imageSize = header->CallObjectImage()->SizeInPixels();
                    TSize tinySize = CallObjectImageIncallSize( ETrue );
                    TBool tiny = imageSize.iHeight < tinySize.iHeight;
                    iBottomImageVariety = tiny ? 0 : 4;
                    
                    CBubblePlace::TPhoneBubblePlace place;
                    place = cnapLength ? CBubblePlace::EBottomImageCnap :
                                         CBubblePlace::EBottomImage;
                    
                    SetActivePlace( place,
                                    *header );                        
                    }
                else
                    {
                    CBubblePlace::TPhoneBubblePlace place =
                        cnapLength ? CBubblePlace::EBottomCnap :
                                     CBubblePlace::EBottom;
                    
                    SetActivePlace( place,
                                    *header );
                    }
                }
            // Initializing call
            else
                {
                if ( cnapLength ) // 3-lines
                    {
                    SetActivePlace( CBubblePlace::EBottomCnap,
                                    *header );
                    }
                else
                    {
                    SetActivePlace( CBubblePlace::EBottom ,
                                    *header );
                    }    
                }                
            break;
            }
        case 2:
            {
            // first call: middle held/active
            TUint8 index = 0;
            FindNextDrawableHeader( index );
            TPhoneCallState callState = 
                iActiveHeaders->At( index )->CallState();
            TInt cnapLength = iActiveHeaders->At( index )->CNAP().Length();                

            // second call: bottom held/active
            TUint8 index2 = TUint8(index + 1);
            FindNextDrawableHeader( index2 );
            TInt cnapLength2 = iActiveHeaders->At( index2 )->CNAP().Length();
            TPhoneCallState callState2 = 
                iActiveHeaders->At( index2 )->CallState();
            TPhoneCallState previousState = 
                iActiveHeaders->At( index )->PreviousCallState();                
            TPhoneCallState previousState2 = 
                iActiveHeaders->At( index2 )->PreviousCallState();                

            CBubblePlace::TPhoneBubblePlace place;
            CBubblePlace::TPhoneBubblePlace place2;
            
            if ( !InitializingCall() ) // Incall
                {
                if ( ( callState == EActive  ) ||
                     ( callState == EDisconnected &&
                       previousState == EActive ) )
                    {
                    // First call shown on bottom
                    place = cnapLength ? 
                            CBubblePlace::EBottomRightActiveCnapTouch :
                            CBubblePlace::EBottomRightActiveTouch;
                    place2 = CBubblePlace::EMiddleHeldTouch;    
                    }
                else if ( ( callState2 == EActive )  ||
                          ( callState2 == EDisconnected &&
                            previousState2 == EActive ) )
                    {
                    // Second call is shown on bottom
                    place = CBubblePlace::EMiddleHeldTouch;
                    place2 = cnapLength2 ? 
                             CBubblePlace::EBottomRightActiveCnapTouch :
                             CBubblePlace::EBottomRightActiveTouch;    
                    }
                else 
                    {
                    // The call that was shown in the held call position
                    // keeps that position.
                    if ( previousState == EOnHold )
                        {
                        place = CBubblePlace::EMiddleHeldTouch;
                        place2 = cnapLength2 ? 
                                 CBubblePlace::EBottomRightActiveCnapTouch :
                                 CBubblePlace::EBottomRightActiveTouch;    
                        }
                    else
                        {
                        place = cnapLength ? 
                                CBubblePlace::EBottomRightActiveCnapTouch :
                                CBubblePlace::EBottomRightActiveTouch;
                        place2 = CBubblePlace::EMiddleHeldTouch;    
                        }
                    }
                }
            else // Initializing call
                {
                // the call that is in initializing state is shown in
                // primary position.
                place = CBubblePlace::ETopLeft;
                place2 =  cnapLength2 ? CBubblePlace::EBottomRightCnap :
                                        CBubblePlace::EBottomRightActive;
                }                
            
            // set drawing order
            if ( callState2 > callState )
                {
                SetActivePlace( place , *iActiveHeaders->At( index ) );
                SetActivePlace( place2 , *iActiveHeaders->At( index2 ) );
                }
            else
                {
                SetActivePlace( place2 , *iActiveHeaders->At( index2 ) );
                SetActivePlace( place , *iActiveHeaders->At( index ) );
                }                    
            }
            break;
        case 3:
            {
            TUint8 index = 0;
            FindNextDrawableHeader( index );
            
            TUint8 index2 = TUint8( index + 1 );
            FindNextDrawableHeader( index2 );
            
            if ( iActiveHeaders->At( index2 )->CallState( ) 
                > iActiveHeaders->At( index )->CallState( ) )
                {
                SetActivePlace( 
                    CBubblePlace::ETopRight , *iActiveHeaders->At( index ) );
                SetActivePlace( 
                    CBubblePlace::ETopLeft , *iActiveHeaders->At( index2 ) );
                }
            else
                {
                SetActivePlace( 
                    CBubblePlace::ETopRight , *iActiveHeaders->At( index2 ) );
                SetActivePlace( 
                    CBubblePlace::ETopLeft , *iActiveHeaders->At( index ) );
                }
            
            
            index = TUint8( index2 + 1 );
            FindNextDrawableHeader( index );
            SetActivePlace( 
                CBubblePlace::EBottomRightActive , *iActiveHeaders->At( index ) );
            break;
            }
        default:
            Panic( EBMPanicInvalidNumberOfHeaders );
            break;
        }    
    }
    
// ---------------------------------------------------------------------------
//  CBubbleManager::SetPhoneMuted
//  
// 
// ---------------------------------------------------------------------------
//
EXPORT_C void CBubbleManager::SetPhoneMuted( const TBool& aIsMuted )
    {
    __ASSERT_ALWAYS( 
        iIsReadyToDraw > 0, Panic( EBMPanicErrorInStartAndEndChanges ) );

    if ( aIsMuted && iMutedImage->Bitmap() == NULL )
        {
        iImageManager->SetBitmapToImage( iMutedImage , 
                                     EQgn_indi_call_muted , 
                                     EQgn_indi_call_muted_mask );
        }

    iIsMuted = aIsMuted;
    iMutedImage->MakeVisible( iIsMuted );
    if ( !iIsMuted )
        {
        iMutedImage->DrawDeferred();
        }
    }

// ---------------------------------------------------------------------------
//  CBubbleManager::CreateCallHeader
//  
// 
// ---------------------------------------------------------------------------
//
EXPORT_C CBubbleManager::TBubbleId CBubbleManager::CreateCallHeader( )
    {
    __ASSERT_ALWAYS( 
        iIsReadyToDraw > 0, Panic( EBMPanicErrorInStartAndEndChanges ) );
    
    // find unused call header for use.
    
    TUint8 index = 0;
    while ( iCallHeaders->At( index )->IsUsed( ) )
        {
        index++;
        __ASSERT_ALWAYS( index < iCallHeaders->Count( ), 
                         Panic( EBMPanicInvalidNumberOfHeaders ) );
        }

    CBubbleCallHeader* header = iCallHeaders->At( index );
    
    header->SetIsUsed( ETrue );
    iActiveHeaders->InsertL( iActiveHeaders->Count( ), header );// Can't leave
    
    return header->BubbleId( );
    }
    
// ---------------------------------------------------------------------------
//  CBubbleManager::RemoveCallHeader
//  
// 
// ---------------------------------------------------------------------------
//
EXPORT_C void CBubbleManager::RemoveCallHeader( const TBubbleId& aBubbleId )
    {
    __ASSERT_ALWAYS( 
        iIsReadyToDraw > 0, Panic( EBMPanicErrorInStartAndEndChanges ) );
    
    // find header
    CBubbleHeader* header = NULL;
    __ASSERT_ALWAYS( FindActiveHeader( aBubbleId , header ) ,  
                     Panic( EBMPanicBubbleIdIsNotInUse ) );
    
    if ( header->IsConference() )
        {
        __ASSERT_DEBUG( 
            EFalse , Panic( EBMPanicReferredHeaderIsConferenceHeader ) );
        return;
        }
    
    if ( header->IsInConference() )
        {
        __ASSERT_DEBUG( 
            EFalse , Panic( EBMPanicReferredCallHeaderIsInConferenceCall ) );
        return;
        }
    
    header->Reset();
    
    
    // find the active header from array
    TUint8 index = 0;
    while ( iActiveHeaders->At( index )->BubbleId( ) != aBubbleId )
        {
        index++;
        }
        
    iActiveHeaders->Delete( index );
    
    iCustomManager->RemoveCustomElements( aBubbleId );
    
    // remove main pane call object
    if ( iMainPaneControl && 
         iMainPaneControl->BubbleId() == aBubbleId )
        {
        iMainPaneControl->Reset();
        }
        
    if ( ( iConfigFlags & EBMCallObjectDisplay ) && iCallObjectManager )
        {
        iCallObjectManager->CancelCallObjectLoading( aBubbleId );            
        }
    }


// ---------------------------------------------------------------------------
//  CBubbleManager::SetState
//  
//  for conf also (active/on hold/ none)
// ---------------------------------------------------------------------------
//
EXPORT_C void CBubbleManager::SetState( const TBubbleId& aBubbleId, 
                                        const TPhoneCallState& aState )
    {    
    
    __ASSERT_ALWAYS( 
        iIsReadyToDraw > 0, Panic( EBMPanicErrorInStartAndEndChanges ) );
    
    // find header
    CBubbleHeader* header = NULL;
    __ASSERT_ALWAYS( FindActiveHeader( aBubbleId , header ) ,  
                     Panic( EBMPanicBubbleIdIsNotInUse ) );
    
    TPhoneCallState oldstate = header->CallState();
    header->SetCallState( aState );

    TUint activeCount = iActiveHeaders->Count();
    
    //check if we have to change the bubble order
    if ( (oldstate == EAlerting || oldstate == EOutgoing 
        || oldstate == EWaiting || oldstate == EIncoming ) 
        && aState == EActive 
        && iActiveHeaders->At( activeCount - 1 )->BubbleId() == aBubbleId)
        {
        //so we know that the connected call was the last
        if ( activeCount > 1 )
            {
            //we know that there is another call
            TPhoneCallState previouscallstate = 
                iActiveHeaders->At( activeCount - 2 )->CallState();
            
            if ( previouscallstate == EAlerting 
                || previouscallstate == EOutgoing 
                || previouscallstate == EWaiting 
                || previouscallstate == EIncoming 
                || previouscallstate == EAlertToDisconnected )
                {
                //we know that it's too about to get connection
                
                // now we must change the bubble order so active bubble
                // don't stay under rectangular bubble.
                
                CBubbleHeader* tempBubble = 
                    iActiveHeaders->At( activeCount-2 );
                iActiveHeaders->At( activeCount-2 ) = 
                                    iActiveHeaders->At(activeCount-1 );
                iActiveHeaders->At( activeCount-1 ) = tempBubble;
                }
            
            }
        
        }

    // Call collision exception cases:

    // Case: AlertToDisconnected + Waiting
    // We have change the drawing order:
    if ( activeCount == 2 )
        {
        if ( iActiveHeaders->At(0)->CallState() == EAlertToDisconnected
            && iActiveHeaders->At(1)->CallState() == EWaiting )
            {
            // now we must change the bubble order so waiting bubble
            // don't stay under alerttodisconnected bubble.
            CBubbleHeader* tempBubble = iActiveHeaders->At(0);
            iActiveHeaders->At(0) = iActiveHeaders->At(1);
            iActiveHeaders->At(1) = tempBubble;
            }
        }
    
    }


// ---------------------------------------------------------------------------
//  CBubbleManager::SetLabel
//  
//  for conf also 
// ---------------------------------------------------------------------------
//
EXPORT_C void CBubbleManager::SetLabel( 
    const TBubbleId& aBubbleId, 
    const TDesC& aText, 
    const TPhoneClippingDirection& aClipDirection)
    {
    __ASSERT_ALWAYS( 
        iIsReadyToDraw > 0, Panic( EBMPanicErrorInStartAndEndChanges ) );
    
    // find header
    CBubbleHeader* header = NULL;
    __ASSERT_ALWAYS( FindActiveHeader( aBubbleId , header ) ,
        Panic( EBMPanicBubbleIdIsNotInUse ) );
    
    BubbleUtils::AddTextToHeader( *header, 
                                  BubbleUtils::EBubbleHeaderLabel , 
                                  aText , 
                                  KBubbleLabelMaxLength ,
                                  aClipDirection ); 
    }

// ---------------------------------------------------------------------------
//  CBubbleManager::SetLabel
//  
//  for conf also 
// ---------------------------------------------------------------------------
//
EXPORT_C void CBubbleManager::SetLabel( 
    const TBubbleId& aBubbleId, 
    const TDesC& aLongText, 
    const TDesC& aShortText, 
    const TPhoneClippingDirection& aClipDirection)
    {
    __ASSERT_ALWAYS( 
        iIsReadyToDraw > 0, Panic( EBMPanicErrorInStartAndEndChanges ) );
    
    // find header
    CBubbleHeader* header = NULL;
    __ASSERT_ALWAYS( FindActiveHeader( aBubbleId , header ) ,  
        Panic( EBMPanicBubbleIdIsNotInUse ) );
    
    // set both text to single buffer. text are cut from the end.
    TInt shortLength = aShortText.Length();
    TInt longLength = aLongText.Length();
    TInt textLength = longLength + shortLength + 1;

    TPtr16 textPtr = iLabelBuf->Des();
    textPtr.Zero();
    if ( textPtr.MaxLength() < textLength )
        {        
        TRAPD ( err, iLabelBuf = iLabelBuf->ReAllocL( textLength ) );
        textPtr.Set( iLabelBuf->Des() );
        textPtr.Zero();
        if ( longLength > 0 )
            {
            if ( err != KErrNone )
                {
                longLength = textPtr.MaxLength() - shortLength - 1;
                textPtr.Append( aLongText.Left( longLength ) );
                }
            else
                {
                textPtr.Append( aLongText );
                }
            }
        else
            {
            textPtr.Append( KNullDesC );
            }
        }
    else
        {
        if ( longLength > 0 )
            {
            textPtr.Append( aLongText );
            }
        else
            {
            textPtr.Append( KNullDesC );
            }
        }

    textPtr.Append( KBubbleTextSeparator );

    if ( shortLength > 0 )
        {
        textPtr.Append( aShortText );
        }
    else
        {
        textPtr.Append( KNullDesC );
        }

    header->SetText( textPtr , aClipDirection );
    }


// ---------------------------------------------------------------------------
//  CBubbleManager::SetCLI
//  
//  for conf also 
// ---------------------------------------------------------------------------
//
EXPORT_C void CBubbleManager::SetCLI( 
    const TBubbleId& aBubbleId, 
    const TDesC& aCLIText, 
    const TPhoneClippingDirection& aClipDirection  )
    {
    __ASSERT_ALWAYS( 
        iIsReadyToDraw > 0, Panic( EBMPanicErrorInStartAndEndChanges ) );
    
    // find header
    CBubbleHeader* header = NULL;
    __ASSERT_ALWAYS( FindActiveHeader( aBubbleId , header ) ,  
        Panic( EBMPanicBubbleIdIsNotInUse ) );
    

    // We have a number
    if ( aClipDirection == ELeft )
        {
        BubbleUtils::AddTextToHeader( *header, 
                                  BubbleUtils::EBubbleHeaderCLI , 
                                  NumberGroup( aCLIText ), 
                                  KBubbleLabelMaxLength ,
                                  aClipDirection ); 
        }
    else
        {
        BubbleUtils::AddTextToHeader( *header, 
                                  BubbleUtils::EBubbleHeaderCLI , 
                                  aCLIText , 
                                  KBubbleLabelMaxLength ,
                                  aClipDirection ); 
        }
    }

// ---------------------------------------------------------------------------
//  CBubbleManager::UpdateCLI
//  
//  for conf also 
// ---------------------------------------------------------------------------
//
EXPORT_C void CBubbleManager::UpdateCLI( 
    const TBubbleId& aBubbleId, 
    const TDesC& aCLIText, 
    const TPhoneClippingDirection& aClipDirection )
    {
    // find header
    CBubbleHeader* header = NULL;
    __ASSERT_ALWAYS( FindActiveHeader( aBubbleId , header ) ,  
                     Panic( EBMPanicBubbleIdIsNotInUse ) );
    
    BubbleUtils::AddTextToHeader( *header, 
                                  BubbleUtils::EBubbleHeaderCLI , 
                                  aCLIText , 
                                  KBubbleLabelMaxLength ,
                                  aClipDirection );
    
    
    // Tell active bubbles to update their CLI if it's visible.
    CBubbleOutlook* outlook = NULL;
    TInt placeCount = iActivePlaces->Count();
    for ( TUint8 index = 0 ; index < placeCount ; index++ )
        {
        outlook = &iActivePlaces->At( index )->GetBubbleOutlook( );
        outlook->DrawCLINow();
        }
    
    }

// ---------------------------------------------------------------------------
//  CBubbleManager::SetCallTime
//  
//  for conf also 
// ---------------------------------------------------------------------------
//
EXPORT_C void CBubbleManager::SetCallTime( const TBubbleId& aBubbleId, 
                                           const TDesC& aCallTime )
    {
    __ASSERT_ALWAYS( 
        iIsReadyToDraw > 0, Panic( EBMPanicErrorInStartAndEndChanges ) );
    
    // find header
    CBubbleHeader* header = NULL;
    __ASSERT_ALWAYS( FindActiveHeader( aBubbleId , header ) ,  
        Panic( EBMPanicBubbleIdIsNotInUse ) );
    

    BubbleUtils::AddTextToHeader( *header, 
                                  BubbleUtils::EBubbleHeaderTimerCost , 
                                  aCallTime , 
                                  KBubbleLabelMaxLength ,
                                  CBubbleManager::ERight );

    }

// ---------------------------------------------------------------------------
//  CBubbleManager::UpdateCallTime
//  
//  for conf also 
// ---------------------------------------------------------------------------
//
EXPORT_C void CBubbleManager::UpdateCallTime( const TBubbleId& aBubbleId, 
                                              const TDesC& aCallTime )
    {
    // find header
    CBubbleHeader* header = NULL;
    __ASSERT_ALWAYS( FindActiveHeader( aBubbleId , header ) ,  
                     Panic( EBMPanicBubbleIdIsNotInUse ) );
    
    BubbleUtils::AddTextToHeader( *header, 
                                  BubbleUtils::EBubbleHeaderTimerCost , 
                                  aCallTime , 
                                  KBubbleLabelMaxLength ,
                                  CBubbleManager::ERight );
    
    // Tell active bubbles to update their timer/cost text if it's visible.
    CBubbleOutlook* outlook = NULL;
    TInt placeCount = iActivePlaces->Count();
    for ( TUint8 index = 0 ; index < placeCount ; index++ )
        {
        outlook = &iActivePlaces->At( index )->GetBubbleOutlook( );
        outlook->DrawTimerCostNow();
        }
    
    }

// ---------------------------------------------------------------------------
//  CBubbleManager::UpdateCallHeaderText
//  
//  for conf also 
// ---------------------------------------------------------------------------
//
EXPORT_C void CBubbleManager::UpdateCallHeaderText( const TBubbleId& aBubbleId, 
                                            const TDesC& aLongText, 
                                            const TDesC& aShortText, 
                                            const TPhoneClippingDirection& aClipDirection )
    {
    // find header
    CBubbleHeader* header = NULL;
    __ASSERT_ALWAYS( FindActiveHeader( aBubbleId , header ) ,  
                     Panic( EBMPanicBubbleIdIsNotInUse ) );
    
    iIsReadyToDraw++;
    SetLabel( aBubbleId, aLongText, aShortText, aClipDirection);
    iIsReadyToDraw--;

    // Tell active bubbles to update call header text if it's visible.
    CBubbleOutlook* outlook = NULL;
    TInt placeCount = iActivePlaces->Count();
    for ( TUint8 index = 0 ; index < placeCount ; index++ )
        {
        outlook = &iActivePlaces->At( index )->GetBubbleOutlook( );        
        outlook->DrawCallHeaderText();
        }
    
    }

// ---------------------------------------------------------------------------
//  CBubbleManager::SetThumbnail
//  
// 
// ---------------------------------------------------------------------------
//
EXPORT_C void CBubbleManager::SetThumbnail( const TBubbleId& aBubbleId, 
                                            CFbsBitmap* aTnBitmap, 
                                            CFbsBitmap* aTnBitmapMask,
                                            TBool aDataOwnershipTransferred )
    {
    if ( !(iConfigFlags & EBMCallObjectDisplay) )
        {
        __ASSERT_ALWAYS( 
            iIsReadyToDraw > 0, Panic( EBMPanicErrorInStartAndEndChanges ) );
        }
    
    // find header
    CBubbleHeader* header = NULL;
    __ASSERT_ALWAYS( FindActiveHeader( aBubbleId , header ) ,  
                     Panic( EBMPanicBubbleIdIsNotInUse ) );
    
    header->SetTnBitmap( aTnBitmap );
    header->SetTnBitmapMask( aTnBitmapMask );
    header->SetTnDataOwnership( aDataOwnershipTransferred );
    }

// ---------------------------------------------------------------------------
//  CBubbleManager::SetCallFlags
//  
//  for conf also 
// ---------------------------------------------------------------------------
//
EXPORT_C void CBubbleManager::SetCallFlags( const TBubbleId& aBubbleId, 
                                            const TUint32& aFlags ) 
    {
    __ASSERT_ALWAYS( 
        iIsReadyToDraw > 0, Panic( EBMPanicErrorInStartAndEndChanges ) );
    
    // find header
    CBubbleHeader* header = NULL;
    __ASSERT_ALWAYS( FindActiveHeader( aBubbleId , header ) ,  
                     Panic( EBMPanicBubbleIdIsNotInUse ) );
    
    header->SetCallFlags( aFlags );
    }

// ---------------------------------------------------------------------------
//  CBubbleManager::SetCallFlag
//  
//  for conf also 
// ---------------------------------------------------------------------------
//
EXPORT_C void CBubbleManager::SetCallFlag( const TBubbleId& aBubbleId, 
                                           const TPhoneCallTypeFlags& aFlag, 
                                           const TBool& aSet )
    {
    __ASSERT_ALWAYS( 
        iIsReadyToDraw > 0, Panic( EBMPanicErrorInStartAndEndChanges ) );
    
    // find header
    CBubbleHeader* header = NULL;
    __ASSERT_ALWAYS( FindActiveHeader( aBubbleId , header ) ,  
                                       Panic( EBMPanicBubbleIdIsNotInUse ) );
    
    if ( aSet )
        {
        header->SetCallFlag( aFlag );
        }
    else 
        {
        header->RemoveCallFlag( aFlag );
        } 
    }

// ---------------------------------------------------------------------------
//  CBubbleManager::SetNumberType
//  
//  
// ---------------------------------------------------------------------------
//
EXPORT_C void CBubbleManager::SetNumberType( const TBubbleId& aBubbleId, 
                                             const TPhoneNumberType& aNumType )
    {
    __ASSERT_ALWAYS( 
        iIsReadyToDraw > 0, Panic( EBMPanicErrorInStartAndEndChanges ) );
    
    // find header
    CBubbleHeader* header = NULL;
    __ASSERT_ALWAYS( FindActiveHeader( aBubbleId , header ) ,  
                     Panic( EBMPanicBubbleIdIsNotInUse ) );
    
    header->SetNumberType( aNumType );
    }


// ---------------------------------------------------------------------------
//  CBubbleManager::SetPbkNumberType
//  
//  
// ---------------------------------------------------------------------------
//
EXPORT_C void CBubbleManager::SetPbkNumberType( const TBubbleId& aBubbleId, 
                                                const TInt aNumType )
    {
    TPhoneNumberType numType = ENotSet;
    switch ( aNumType )
        {  
        case EPbkFieldIdPhoneNumberMobile:
            numType = EMobile;
            break;
        case EPbkFieldIdPhoneNumberStandard:
        case EPbkFieldIdPhoneNumberHome:
        case EPbkFieldIdPhoneNumberWork:
            numType = EPhone;
            break;
        case EPbkFieldIdFaxNumber:
            numType = EFaxNumber;
            break;
        case EPbkFieldIdPagerNumber:
            numType = EPager;
            break;
        case EPbkFieldIdCarNumber:
            numType = ECar;
            break;            
        case EPbkFieldIdAssistantNumber:
            numType = EAssistant;
            break;            
        case EPbkFieldIdNone:       // flow through
        default:
            numType = ENotSet;
            break;
        }
    SetNumberType( aBubbleId , numType );
    }

// ---------------------------------------------------------------------------
//  CBubbleManager::SetPhCntNumberType
//  
//  
// ---------------------------------------------------------------------------
//
EXPORT_C void CBubbleManager::SetPhCntNumberType( 
    const TBubbleId& aBubbleId,
    const MPhCntMatch::TNumberType aNumberType )    
    {
#ifdef RD_VIRTUAL_PHONEBOOK
    TPhoneNumberType numType = ENotSet;
    switch( aNumberType ) 
        {
        case MPhCntMatch::EMobileNumber:
            numType = EMobile;
            break;

        case MPhCntMatch::EVoipNumber:            
        case MPhCntMatch::EStandardNumber:
            numType = EPhone;
            break;
            
        case MPhCntMatch::EFaxNumber:
            numType = EFaxNumber;
            break;
            
        case MPhCntMatch::EPagerNumber:
            numType = EPager;
            break;
        case MPhCntMatch::EAssistantNumber:
        	numType = EAssistant;
        	break;
        case MPhCntMatch::ECarNumber:
        	numType = ECar;
        	break;
        default:
            break;
        }
    SetNumberType( aBubbleId, numType );
#else
    (void) aBubbleId;
    (void) aNumberType;	    
#endif // RD_VIRTUAL_PHONEBOOK
    }

// ---------------------------------------------------------------------------
//  CBubbleManager::CreateConference
//  
//  For conference calls only
// ---------------------------------------------------------------------------
//
EXPORT_C CBubbleManager::TBubbleId CBubbleManager::CreateConference( 
                                                const TBubbleId& aBubble1, 
                                                const TBubbleId& aBubble2 )
    {
    __ASSERT_ALWAYS( 
        iIsReadyToDraw > 0, Panic( EBMPanicErrorInStartAndEndChanges ) );
    __ASSERT_ALWAYS( iActiveHeaders->Count() < KBubblePlaceMaxVisible, 
                     Panic( EBMPanicInvalidNumberOfHeaders ) );
    __ASSERT_DEBUG( !iConfHeader->IsUsed() , Panic( EBMPanicConferenceIsUsed ) );
    
    iConfHeader->SetIsUsed( ETrue );
    
    AddRowToConference( aBubble1 );
    AddRowToConference( aBubble2 );
    
    // Put conference header to first so it will be drawn always on top.
    iActiveHeaders->InsertL( 0 , iConfHeader ); //Can't leave
    
    return iConfHeader->BubbleId( );    
    }

// ---------------------------------------------------------------------------
//  CBubbleManager::RemoveConference
//  
//  For conference calls only
// ---------------------------------------------------------------------------
//
EXPORT_C void CBubbleManager::RemoveConference( )
    {
    __ASSERT_ALWAYS( 
        iIsReadyToDraw > 0, Panic( EBMPanicErrorInStartAndEndChanges ) );
    __ASSERT_ALWAYS( iConfHeader->IsUsed( ), Panic( EBMPanicConferenceNotUsed ) );
    
    iConfHeader->Reset();
    
    
    // find the active element
    TUint8 index = 0;
    __ASSERT_ALWAYS( 
        iActiveHeaders->Count( ) > 0, Panic( EBMPanicInvalidNumberOfHeaders ) );
    while ( iActiveHeaders->At( index )->BubbleId( ) != KBubbleConferenceId )
        {
        index++;
        __ASSERT_ALWAYS( index != iActiveHeaders->Count( ), 
            Panic( EBMPanicInvalidNumberOfHeaders ) );
        }
    
    iActiveHeaders->Delete( index );
    }

// ---------------------------------------------------------------------------
//  CBubbleManager::AddRowToConference
//  
//  For conference calls only
// ---------------------------------------------------------------------------
//
EXPORT_C void CBubbleManager::AddRowToConference( const TBubbleId& aBubbleId )
    {
    __ASSERT_ALWAYS( 
        iIsReadyToDraw > 0, Panic( EBMPanicErrorInStartAndEndChanges ) );
    
    // find header
    CBubbleHeader* header = NULL;
    __ASSERT_ALWAYS( FindActiveHeader( aBubbleId , header ) ,  
                     Panic( EBMPanicBubbleIdIsNotInUse ) );
    
    __ASSERT_ALWAYS( header->IsUsed(), Panic( EBMPanicBubbleIdIsNotInUse ) );
    __ASSERT_ALWAYS( !header->IsConference(), 
                     Panic( EBMPanicReferredHeaderIsConferenceHeader ) );
    __ASSERT_ALWAYS( !header->IsInConference(), 
                     Panic( EBMPanicReferredCallHeaderIsInConferenceCall ) );
    
    iConfHeader->AddRow( static_cast<CBubbleCallHeader&>( *header ) ); // type cast downwards
    }

// ---------------------------------------------------------------------------
//  CBubbleManager::RemoveRowFromConference
//  
//  For conference calls only
// ---------------------------------------------------------------------------
//
EXPORT_C void CBubbleManager::RemoveRowFromConference( const TBubbleId& aBubbleId )
    {
    __ASSERT_ALWAYS( 
        iIsReadyToDraw > 0, Panic( EBMPanicErrorInStartAndEndChanges ) );
    
    iConfHeader->RemoveRow( aBubbleId );
    }

// ---------------------------------------------------------------------------
//  CBubbleManager::ConferenceRowCount
//  
//  For conference calls only
// ---------------------------------------------------------------------------
//
EXPORT_C TUint8 CBubbleManager::ConferenceRowCount( ) const
    {
    return iConfHeader->RowCount();
    }

// ---------------------------------------------------------------------------
//  CBubbleManager::SetExpandedConferenceCallHeader
//  
//  For conference calls only
// ---------------------------------------------------------------------------
//
EXPORT_C void CBubbleManager::SetExpandedConferenceCallHeader( 
    const TBool& aIsExpanded )
    {
    iConfHeader->SetIsExpanded( aIsExpanded );
    }

// ---------------------------------------------------------------------------
//  CBubbleManager::IsConferenceExpanded
//  
//  For conference calls only
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CBubbleManager::IsConferenceExpanded( ) const
    {
    return iConfHeader->IsExpanded();
    }

// ---------------------------------------------------------------------------
//  CBubbleManager::SetSelectionInConference
//  
//  For conference calls only
// ---------------------------------------------------------------------------
//
EXPORT_C void CBubbleManager::SetSelectionInConference( 
    const TRowNumber& aRowNumber )
    {
    CBubbleManager::TRowNumber oldrow = iConfHeader->Highlight();
    iConfHeader->SetHighlight( aRowNumber );
    CBubbleManager::TRowNumber newrow = iConfHeader->Highlight();
    
    UpdateConferenceRows( oldrow , newrow );
    }

// ---------------------------------------------------------------------------
//  CBubbleManager::SetSelectionIdInConference
//  
//  For conference calls only
// ---------------------------------------------------------------------------
//
EXPORT_C void CBubbleManager::SetSelectionIdInConference( 
    const TBubbleId& aBubbleId )
    {
    CBubbleManager::TRowNumber oldrow = iConfHeader->Highlight();
    iConfHeader->SetHighlightId( aBubbleId );
    CBubbleManager::TRowNumber newrow = iConfHeader->Highlight();
    
    UpdateConferenceRows( oldrow , newrow );
    }
// ---------------------------------------------------------------------------
//  CBubbleManager::SelectionInConference
//  
//  For conference calls only
// ---------------------------------------------------------------------------
//
EXPORT_C CBubbleManager::TRowNumber 
    CBubbleManager::SelectionInConference() const
    {
    return iConfHeader->Highlight();
    }

// ---------------------------------------------------------------------------
//  CBubbleManager::SelectionIdInConference
//  
//  For conference calls only
// ---------------------------------------------------------------------------
//
EXPORT_C CBubbleManager::TBubbleId 
    CBubbleManager::SelectionIdInConference() const
    {
    return iConfHeader->HighlightId();
    }

// ---------------------------------------------------------------------------
//  CBubbleManager::MoveHighlightOneUpInConference
//  
//  For conference calls only
// ---------------------------------------------------------------------------
//
EXPORT_C void CBubbleManager::MoveHighlightOneUpInConference()
    {
    CBubbleManager::TRowNumber oldrow = iConfHeader->Highlight();
    iConfHeader->MoveHighlightOneUp();
    CBubbleManager::TRowNumber newrow = iConfHeader->Highlight();
    
    UpdateConferenceRows( oldrow , newrow );
    }

// ---------------------------------------------------------------------------
//  CBubbleManager::MoveHighlightOneDownInConference
//  
//  For conference calls only
// ---------------------------------------------------------------------------
//
EXPORT_C void CBubbleManager::MoveHighlightOneDownInConference()
    {
    CBubbleManager::TRowNumber oldrow = iConfHeader->Highlight();
    iConfHeader->MoveHighlightOneDown();
    CBubbleManager::TRowNumber newrow = iConfHeader->Highlight();
    
    UpdateConferenceRows( oldrow , newrow );
    }



// ---------------------------------------------------------------------------
//  CBubbleManager::CreateNumberEntry
//  
//  For number entry
// ---------------------------------------------------------------------------
//
EXPORT_C void CBubbleManager::CreateNumberEntry()
    {
    __ASSERT_ALWAYS( 
        iIsReadyToDraw > 0, Panic( EBMPanicErrorInStartAndEndChanges ) );
    __ASSERT_ALWAYS( !iNumberEntry->IsUsed(), Panic( EBMPanicNumberEntryInUse ) );

#ifdef RD_UI_TRANSITION_EFFECTS_PHASE2
    if ( !ShownHeaderCount() ) // Only effects in number entry
        {
        // Capture transition begin state
        GfxTransEffect::Begin( iNumberEntry, KGfxControlAppearAction );
        // Set reminder that Begin has been called.
        CAknTransitionUtils::SetData( (TInt) this, (TAny*) 1 );
        }
#endif //NOT_RD_UI_TRANSITION_EFFECTS_PHASE2
    
    iNumberEntry->SetIsUsed( ETrue );
    iNumberEntry->MakeVisible( ETrue );
    }

// ---------------------------------------------------------------------------
//  CBubbleManager::GetNumberEntry
//  
//  For number entry
// ---------------------------------------------------------------------------
//
EXPORT_C CCoeControl* CBubbleManager::GetNumberEntry( ) const
    {
    return iNumberEntry->GetEditor();
    }

// ---------------------------------------------------------------------------
//  CBubbleManager::IsNumberEntryUsed
//  
//  For number entry
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CBubbleManager::IsNumberEntryUsed( ) const
    {
    return iNumberEntry->IsUsed();
    }

// ---------------------------------------------------------------------------
//  CBubbleManager::SetNumberEntryVisible
//  
//  For number entry
// ---------------------------------------------------------------------------
//
EXPORT_C void CBubbleManager::SetNumberEntryVisible( const TBool& aVisibility )
    {
    __ASSERT_ALWAYS( 
        iIsReadyToDraw > 0, Panic( EBMPanicErrorInStartAndEndChanges ) );

    iNumberEntry->SetPlace( CBubbleOutlookNumberEntry::ENENone );
    iNumberEntry->MakeVisible( aVisibility );
    CCoeControl* editor = iNumberEntry->GetEditor();
    if ( !aVisibility )
        {
        editor->SetFocus( EFalse );
        }
    else
        {
// Don't set focus now ( wait until the screen is redirected )
// We don't want the cursor to be shown until the transition is finished
#ifndef RD_UI_TRANSITION_EFFECTS_PHASE2
        editor->SetFocus( ETrue );
#endif
        }
    }

// ---------------------------------------------------------------------------
//  CBubbleManager::SetTextToNumberEntry
//  
//  For number entry
// ---------------------------------------------------------------------------
//
EXPORT_C void CBubbleManager::SetTextToNumberEntry( const TDesC& aDesC )
    {
    iNumberEntry->SetText( aDesC );
    }

// ---------------------------------------------------------------------------
//  CBubbleManager::GetTextFromNumberEntry
//  
//  For number entry
// ---------------------------------------------------------------------------
//
EXPORT_C void CBubbleManager::GetTextFromNumberEntry( TDes& aDesC )
    {
    iNumberEntry->GetText( aDesC );
    }

// ---------------------------------------------------------------------------
//  CBubbleManager::RemoveNumberEntry
//  
//  For number entry
// ---------------------------------------------------------------------------
//
EXPORT_C void CBubbleManager::RemoveNumberEntry( )
    {
    __ASSERT_ALWAYS( 
        iIsReadyToDraw > 0, Panic( EBMPanicErrorInStartAndEndChanges ) );
    __ASSERT_ALWAYS( 
        iNumberEntry->IsUsed(), Panic( EBMPanicNumberEntryNotInUse ) );
    
#ifdef RD_UI_TRANSITION_EFFECTS_PHASE2
    if ( !ShownHeaderCount() ) // Only effects in number entry
        {
        // Capture transition begin state
        GfxTransEffect::Begin( iNumberEntry, KGfxControlDisappearAction );
        // Set reminder that Begin has been called.
        CAknTransitionUtils::SetData( (TInt) this, (TAny*) 1 );
        }
#endif

    iNumberEntry->Reset();
    iNumberEntry->MakeVisible( EFalse );
    }


// ---------------------------------------------------------------------------
//  CBubbleManager::FindHeader
//  
// 
// ---------------------------------------------------------------------------
//
TBool CBubbleManager::FindHeader( 
    const TBubbleId& aBubbleID, 
    CBubbleHeader*& aHeader ) const
    {
    TUint count( TUint8( iCallHeaders->Count() ) );
    CBubbleHeader* tempHeader = NULL;
    for ( TUint8 i = 0 ; i < count ; i++ )
        {
        tempHeader = iCallHeaders->At( i );
        if ( aBubbleID == tempHeader->BubbleId() )
            {
            aHeader = tempHeader;
            return ETrue;
            }
        }
    aHeader = NULL;
    return EFalse;
    }

// ---------------------------------------------------------------------------
//  CBubbleManager::FindActiveHeader
//  
// 
// ---------------------------------------------------------------------------
//
TBool CBubbleManager::FindActiveHeader( 
    const TBubbleId& aBubbleID, 
    CBubbleHeader*& aHeader ) const
    {
    TUint count( TUint8( iActiveHeaders->Count() ) );
    CBubbleHeader* tempHeader = NULL;
    for ( TUint8 i = 0 ; i < count ; i++ )
        {
        tempHeader = iActiveHeaders->At( i );
        if ( aBubbleID == tempHeader->BubbleId() )
            {
            aHeader = tempHeader;
            return ETrue;
            }
        }
    aHeader = NULL;
    return EFalse;
    }

// ---------------------------------------------------------------------------
//  CBubbleManager::UpdateConferenceRows
//  
// 
// ---------------------------------------------------------------------------
//
void CBubbleManager::UpdateConferenceRows( CBubbleManager::TRowNumber aRow1, 
                                          CBubbleManager::TRowNumber aRow2 )
    {
    // if the rows are the same or conf is not expanded - nothing to do.
    if ( aRow1 == aRow2 || !iConfHeader->IsExpanded() )
        {
        return;
        }
    
    // if we are surrounded by start-/endchanges, we don't have to
    // worry about drawing.
    if ( iIsReadyToDraw > 0 )
        {
        return;
        }
    
    // find current control:
    
    // There should be only one active place: the expanded conference place
    if ( iActivePlaces->Count() != 1 )
        {
        __ASSERT_DEBUG( EFalse, User::Invariant() );
        return;
        }
    
    CBubbleOutlookConference* control = 
        static_cast< CBubbleOutlookConference* >
            ( &iActivePlaces->At(0)->GetBubbleOutlook() );
    
    // make sure it's what we want
    if ( control == NULL )
        {
        __ASSERT_DEBUG( EFalse, User::Invariant() );
        return;
        }
    
    
    // Draw row1 first:
    control->DrawRowNow( aRow1 );
    
    // Then second row:
    control->DrawRowNow( aRow2 );
    }


// ---------------------------------------------------------------------------
//  CBubbleManager::FocusChanged
//  
// 
// ---------------------------------------------------------------------------
//
void CBubbleManager::FocusChanged( TDrawNow aDrawNow )
    {
    if ( iNumberEntry->IsUsed() && iNumberEntry->IsVisible() )
        {
        iNumberEntry->SetFocus( IsFocused(), aDrawNow );
        }
    }

// ---------------------------------------------------------------------------
//  CBubbleManager::ImageManager
//  
// 
// ---------------------------------------------------------------------------
//
CBubbleImageManager& CBubbleManager::ImageManager()
    {
    return *iImageManager;
    }

// ---------------------------------------------------------------------------
//  CBubbleManager::ResourceManager
//  
// 
// ---------------------------------------------------------------------------
//
CBubbleResourceManager& CBubbleManager::ResourceManager()
    {
    return *iResourceManager;
    }

// ---------------------------------------------------------------------------
//  CBubbleManager::CustomManager
//  
// 
// ---------------------------------------------------------------------------
//
CBubbleCustomManager& CBubbleManager::CustomManager()
    {
    return *iCustomManager;
    }
    
// ---------------------------------------------------------------------------
//  CBubbleManager::HandleResourceChange
//  
// 
// ---------------------------------------------------------------------------
//
void CBubbleManager::HandleResourceChange(TInt aType)
    {
    if ( aType ==  KEikMessageUnfadeWindows )
        {
        if ( iIsReadyToDraw == 0 )
            {
            if ( iActivePlaces->Count() > 0 )
                {
                iParentControl->DrawNow();
                }
            }
        }

    if ( aType == KAknsMessageSkinChange || 
         aType == KEikDynamicLayoutVariantSwitch )
        {
        StartChanges();
        
        if ( aType == KAknsMessageSkinChange )
            {
            // Remove muted image. It will be reloaded when 
            // needed for the next time.
            iMutedImage->SetPicture( NULL , NULL );
            iMutedImage->MakeVisible( EFalse );

            // Release all bitmaps from the animations
            for ( TInt i = 0; i < iBubblePlaces->Count() ; i++ )
                {
                iBubblePlaces->At( i )->GetBubbleOutlook().ReleaseBitmaps();
                }

            // Number entry
            iNumberEntry->ReleaseBitmaps();

            // Clear all the skinned images from own cache
            iImageManager->ClearSkinsCache();

            // Set the image back if it was there...
            SetPhoneMuted( iIsMuted );

            // Recreate all bitmaps from the animations
            for ( TInt a = 0; a < iBubblePlaces->Count() ; a++ )
                {
                iBubblePlaces->At( a )->GetBubbleOutlook().RebuildBitmaps();
                }
            
            // Number entry
            iNumberEntry->RebuildBitmaps();
            
            // Touch pane
            if ( iTouchPane )
                {
                iTouchPane->HandleResourceChange( aType );                    
                }
            
            // Update preloaded theme image
            if ( iCallObjectManager && 
                 iCallObjectManager->CallThemeImageIcon() )
                {
                iMainPaneControl->Reset();
                
                iCallObjectManager->ReleaseCallThemeImage();
                iCallObjectManager->LoadCallThemeImage( 
                        *iImageManager, 
                        MainPaneImageRect().Size() );
                }
            }
        else if ( aType == KEikDynamicLayoutVariantSwitch )
            {
            iImageManager->PrepareCachedBitmaps();
            }            
        
        // Call object
        if ( aType == KEikDynamicLayoutVariantSwitch )
            {
            if ( iCallObjectManager && 
                 iCallObjectManager->CallThemeImageIcon() )
                {
                iCallObjectManager->ResizeCallThemeImage( MainPaneImageRect().Size() );
                }
                        
            if ( iMainPaneControl &&
                 iMainPaneControl->IsUsed() )
                {
                ReloadCallObjectImage();
                }
            }
        
        EndChanges();

        // Relayout video
        if ( iVideoFlags & EBMVideoPlayingVideo )
            {
            iVideoController->HandleLayoutChange();    
            }

        return;
        }

    CCoeControl::HandleResourceChange(aType);
    }

// ---------------------------------------------------------------------------
//  CBubbleManager::SetCNAP
//  
// 
// ---------------------------------------------------------------------------
//
EXPORT_C void CBubbleManager::SetCNAP( 
    const TBubbleId& aBubbleId, 
    const TDesC& aCNAPText, 
    const TPhoneClippingDirection& aClipDirection )
    {
    __ASSERT_ALWAYS( 
        iIsReadyToDraw > 0, Panic( EBMPanicErrorInStartAndEndChanges ) );

    // find header
    CBubbleHeader* header = NULL;
    __ASSERT_ALWAYS( FindActiveHeader( aBubbleId , header ) ,  
                     Panic( EBMPanicBubbleIdIsNotInUse ) );

    if ( aCNAPText.Length() == 0 )
        {
        return;
        }

    // We have a number        
    if ( aClipDirection == ELeft )
        {
        BubbleUtils::AddTextToHeader( 
            *header, 
            BubbleUtils::EBubbleHeaderCNAP, 
            NumberGroup( aCNAPText ), 
            KBubbleLabelMaxLength ,
            aClipDirection );
        }
    else
        {
        BubbleUtils::AddTextToHeader(
            *header, 
            BubbleUtils::EBubbleHeaderCNAP, 
            aCNAPText, 
            KBubbleLabelMaxLength ,
            aClipDirection );    
        }            
    }

// ---------------------------------------------------------------------------
//  CBubbleManager::NumberGroup
//  
// 
// ---------------------------------------------------------------------------
//
const TDesC& CBubbleManager::NumberGroup( const TDesC& aNumber )
    {
    if ( iNumberGrouping )
        {
        iNumberGrouping->Set( aNumber );
        return iNumberGrouping->FormattedNumber();
        }
    else
        {
        return aNumber;
        }
    }
    
// ---------------------------------------------------------------------------
//  CBubbleManager::SetCallObjectImage
//  
// 
// ---------------------------------------------------------------------------
//
EXPORT_C void CBubbleManager::SetCallObjectImage( 
    const TBubbleId& aBubbleId,
    const TDesC& aImageFileName )
    {
    __ASSERT_ALWAYS( iCallObjectManager, 
                     Panic( EBMPanicFeatureNotSupported ) );
        
    CBubbleHeader* header = NULL;
    __ASSERT_ALWAYS( FindActiveHeader( aBubbleId , header ) ,  
                     Panic( EBMPanicBubbleIdIsNotInUse ) );
    
    if ( !header->CallObjectImage() )
        {
        if ( !header->IsConference() )        
            {
            // load image from file
            TBool threeLines( header->CNAP().Length() > 0 );
            CBubbleCallObjectManager::TImageLoadingParams params;
            params.iPreferredSize = MainPaneImageRect().Size();
            params.iTinyImageSize = MainPaneImageRect( ETrue, 
                                                       threeLines ).Size();
            params.iDisplayMode = Window().DisplayMode();
            params.iThumbnailSize = CallObjectImageIncallSize( ETrue );
            iCallObjectManager->LoadImageFromFile( aBubbleId,
                                                   aImageFileName,
                                                   params );
            }
        else // conference
            {
            TAknLayoutRect bubbleRect;
            bubbleRect.LayoutRect( Rect(),
                BubbleLayout2::popup_call2_audio_conf_window(3) );
            
            TSize imageSize( 
                CallObjectImageIncallSize().iWidth,
                bubbleRect.Rect().Size().iHeight );
            
            // load image from file
            CBubbleCallObjectManager::TImageLoadingParams params;
            params.iPreferredSize = imageSize;
            params.iDisplayMode = Window().DisplayMode();
            params.iTinyImageSize = TSize(0,0);
            params.iThumbnailSize = TSize(0,0);
            iCallObjectManager->LoadImageFromFile( aBubbleId,
                                                   aImageFileName,
                                                   params );    
            }            

        header->SetCallObjectFileName( aImageFileName.Alloc() );
        header->SetCallObjectImageType(
           CBubbleHeader::EGalleryImage );
        }
    
    return;            
    }

// ---------------------------------------------------------------------------
//  CBubbleManager::SetCallObjectImage
//  
// 
// ---------------------------------------------------------------------------
//    
EXPORT_C void CBubbleManager::SetCallObjectImage( const TBubbleId& aBubbleId,
                                                  CFbsBitmap* aCOBitmap, 
                                                  CFbsBitmap* aCOBitmapMask,
                                                  TBool aDataOwnershipTrasferred )
    {
    __ASSERT_ALWAYS( ( iConfigFlags & EBMCallObjectDisplay ), 
                     Panic( EBMPanicFeatureNotSupported ) );
        
    __ASSERT_ALWAYS( 
        iIsReadyToDraw > 0, Panic( EBMPanicErrorInStartAndEndChanges ) );
    
    // find header
    CBubbleHeader* header = NULL;
    __ASSERT_ALWAYS( FindActiveHeader( aBubbleId , header ) ,  
                     Panic( EBMPanicBubbleIdIsNotInUse ) );
    
    header->SetCallObjectImage( aCOBitmap );
    header->SetCallObjectImageMask( aCOBitmapMask );
    header->SetCallObjectImageDataOwnership( aDataOwnershipTrasferred );
    iCallObjectChanged = ETrue;    
    }

// ---------------------------------------------------------------------------
//  CBubbleManager::SetCallObjectTheme
//  
// 
// ---------------------------------------------------------------------------
//
EXPORT_C void CBubbleManager::SetCallObjectFromTheme( 
    const TBubbleId& aBubbleId )
    {
    CBubbleHeader* header = NULL;
    __ASSERT_ALWAYS( FindActiveHeader( aBubbleId , header ) ,  
                     Panic( EBMPanicBubbleIdIsNotInUse ) );
    
    if ( !header->CallObjectImage()  )
        {
        CEikImage* themeImage = new CEikImage;
        if ( themeImage != NULL )
            {
            iImageManager->SetBitmapToImage( themeImage,
                                             EQgn_graf_call_image_1,
                                             EQgn_graf_call_image_1_mask );
            SetCallObjectImage( 
                aBubbleId,
                const_cast<CFbsBitmap*> ( themeImage->Bitmap() ),
                const_cast<CFbsBitmap*> ( themeImage->Mask() ),
                ETrue );
    
            themeImage->SetPictureOwnedExternally( ETrue );
            delete themeImage;
            header->SetCallObjectImageType( CBubbleHeader::EThemeImage );                
            }
        }
    }
    
// ---------------------------------------------------------------------------
//  CBubbleManager::SetCallObjectText
//  
// 
// ---------------------------------------------------------------------------
//    
EXPORT_C void CBubbleManager::SetCallObjectText( const TBubbleId& aBubbleId, 
                                                 const TDesC& aCOText )
    {
    __ASSERT_ALWAYS( iCallObjectManager, 
                     Panic( EBMPanicFeatureNotSupported ) );
        
    __ASSERT_ALWAYS( 
        iIsReadyToDraw > 0, Panic( EBMPanicErrorInStartAndEndChanges ) );
    
    // find header
    CBubbleHeader* header = NULL;
    __ASSERT_ALWAYS( FindActiveHeader( aBubbleId , header ) ,  
                     Panic( EBMPanicBubbleIdIsNotInUse ) );
    
    if ( !header->CallObjectText().Length() &&
         !header->IsConference() )
        {
        HBufC* text = aCOText.Alloc();    
        header->SetCallObjectText( text ); // takes ownership
        iCallObjectChanged = ETrue;
    
        CBubbleCallObjectManager::TImageLoadingParams params;
        params.iPreferredSize = CallObjectImageIncallSize();
        params.iTinyImageSize = TSize(0,0);
        params.iThumbnailSize = TSize(0,0);
        params.iDisplayMode = Window().DisplayMode();
        iCallObjectManager->LoadImageFromText( aBubbleId,
                                               aCOText,
                                               params );
        header->SetCallObjectImageType(
           CBubbleHeader::ETextBasedImage );                                               
        }
    }

// ---------------------------------------------------------------------------
//  CBubbleManager::ChangeEditorMode
//  
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CBubbleManager::ChangeEditorMode( TBool aDefaultMode )
    {
    return iNumberEntry->ChangeEditorMode( aDefaultMode );
    }
    
// ---------------------------------------------------------------------------
//  CBubbleManager::GetEditorMode
//  
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CBubbleManager::GetEditorMode() const
    {
    return iNumberEntry->GetEditorMode();
    }    
    
// ---------------------------------------------------------------------------
//  CBubbleManager::ResetEditorToDefaultValues
//  
// 
// ---------------------------------------------------------------------------
//
EXPORT_C void CBubbleManager::ResetEditorToDefaultValues()
    {
    iNumberEntry->ResetEditorToDefaultValues();
    }        
                                                   
// ---------------------------------------------------------------------------
//  CBubbleManager::DeactivatepPopupDisplay
//  
// 
// ---------------------------------------------------------------------------
//
TBool CBubbleManager::FindHeader( const TPhoneCallState& aCallState,
                                  CBubbleHeader*& aHeader ) const
    {
    // lookup outgoing and incoming call headers
    TInt activeHeaderCount( iActiveHeaders->Count() );
    for ( TInt i(0); i < activeHeaderCount; i++ )
        {
        CBubbleHeader* header = iActiveHeaders->At( i );
        TPhoneCallState callState( header->CallState() );    
        if ( callState == aCallState )
            {
            aHeader = header;
            return ETrue;
            }
        }

    return EFalse;
    }


// ---------------------------------------------------------------------------
//  CBubbleManager::BuildMainPaneCallObjectL
//  
// 
// ---------------------------------------------------------------------------
//     
void CBubbleManager::BuildMainPaneImage()
    {
    BM_TRACE_( "[BUBBLEMANAGER] CBubbleManager::BuildMainPaneImage" );
    
    CBubbleHeader* incomingHeader = NULL;
    CBubbleHeader* outgoingHeader = NULL;
    CBubbleHeader* activeHeader = NULL;
    CBubbleHeader* holdHeader = NULL;
    CBubbleHeader* activeConfHeader = NULL;
    
    // get incoming and outgoing call headers
    TInt activeHeaderCount( iActiveHeaders->Count() );
    for ( TInt i(0); i < activeHeaderCount; i++ )
        {
        CBubbleHeader* header = iActiveHeaders->At( i );
        TPhoneCallState callState( header->CallState() );    
        if ( callState == EIncoming || callState == EWaiting )
            {
            incomingHeader = header;
            activeHeader = NULL;
            holdHeader = NULL;
            activeConfHeader = NULL;
            break;
            }
        else if ( callState == EOutgoing || callState == EAlerting )
            {
            outgoingHeader = header;
            activeHeader = NULL;
            holdHeader = NULL;
            activeConfHeader = NULL;
            break;
            }
        else if ( callState == EActive )
            {
            holdHeader = NULL;
            if( header->IsConference() )
                {
                // active conference founded, no image to show
                activeConfHeader = header;
                activeHeader = NULL;
                }
            else if ( !activeConfHeader )
                {
                // no conference, single active
                activeHeader = header;
                }
            }
        else if ( callState == EOnHold )
            {
            if( !activeHeader && !activeConfHeader )
                {                
                // none active headers, hold need to handle
                holdHeader = header;
                }
            }
        BM_TRACE_1( "[BUBBLEMANAGER] CBubbleManager::BuildMainPaneImage - callState = %d", callState );
        }
    
   if ( holdHeader && iMainPaneControl->IsUsed() )
        {
        // Keep image in mainpane displayed
        return;
        }
    else if ( activeConfHeader || ( !incomingHeader && !outgoingHeader && !activeHeader && !holdHeader ) )
        {
        // there are conference call or no incoming, outgoing, active, hold calls , no image
        // in mainpane is displayed
        iMainPaneControl->Reset();
        return;
        }
    
    CBubbleHeader* header = incomingHeader;
    if ( outgoingHeader )
        {
        header = outgoingHeader;
        }
    else if ( activeHeader )
        {
        // single active
        header = activeHeader;
        iCallObjectChanged = ETrue;
        }
    else if ( holdHeader )
        {
        // single hold and no image yet
        header = holdHeader;
        iCallObjectChanged = ETrue;
        }
                
    if ( header->IsInConference() || header->CallObjectText().Length() )
        {
        // Conference item or Text is not shown in mainpane.
        return;            
        }

    if ( !iMainPaneControl->IsUsed() || iCallObjectChanged )
        {
        iMainPaneControl->Reset();
        iMainPaneControl->ReadBubbleHeader( *header );
        iCallObjectChanged = EFalse;    
        }
    }

// ---------------------------------------------------------------------------
//  CBubbleManager::ReloadCallObjectImage
//  
// 
// ---------------------------------------------------------------------------
//
void CBubbleManager::ReloadCallObjectImage()
    {
    const TBubbleId bubble = iMainPaneControl->BubbleId();

    iMainPaneControl->Reset();
    
    CBubbleHeader* header = NULL;
    FindActiveHeader( bubble , header );

    if ( header && header->CallObjectFileName().Length() )        
        {
        TBool threeLines( header->CNAP().Length() > 0 );
        
        CBubbleCallObjectManager::TImageLoadingParams params;
        params.iPreferredSize = MainPaneImageRect().Size();
        params.iTinyImageSize = MainPaneImageRect( ETrue, threeLines ).Size();
        params.iThumbnailSize = CallObjectImageIncallSize( ETrue );
        params.iDisplayMode = Window().DisplayMode();    

        iCallObjectManager->CancelCallObjectLoading( bubble );
        
        iCallObjectManager->LoadImageFromFile( 
            bubble,
            header->CallObjectFileName(),
            params );
        
        header->SetCallObjectImage( NULL );
        }
    }

// ---------------------------------------------------------------------------
//  CBubbleManager::CallIsDroppedByEndKey
//  
// 
// ---------------------------------------------------------------------------
//
TBool CBubbleManager::IsCallDroppedByEndKey( 
    const TPhoneCallState& aState ) const
    {
    TUint activeCallCount( iActiveHeaders->Count() );
    
    if ( activeCallCount == 1 )
        {
        // just one call, it is always dropped by end key
        return ETrue;            
        }
        
    // Dropping order in multicall state
    // 1. Disconnected call
    // 2. Initialising or alerting call
    // 3. Active call
    // 4. Held call
    // 5. Waiting call
    
    if ( aState == EWaiting )
        {
        // never dropped by end key
        return EFalse;                
        }
    
    // go through ongoing calls and check if there is a call with
    // higher dropping priority
    TBool dropped( ETrue );
    for ( TInt i(0); i < activeCallCount; i++ )
        {
        TPhoneCallState callState = iActiveHeaders->At( i )->CallState();
        
        if ( aState == EOnHold && ( callState == EDisconnected ||
             callState == EOutgoing || callState == EAlerting ||
             callState == EActive ) )
            {
            dropped = EFalse;
            break;    
            }
        else if ( aState == EActive && ( callState == EDisconnected ||
             callState == EOutgoing || callState == EAlerting ) )
            {
            dropped = EFalse;
            break;        
            }
        else if ( ( aState == EOutgoing || aState == EAlerting ) &&
            ( callState == EDisconnected ) )
            {
            dropped = EFalse;
            break;            
            }
        else
            {
            // do nothing    
            }
        } // for
    
    return dropped;       
    }    

// ---------------------------------------------------------------------------
//  CBubbleManager::ActiveCallCount
//  
// 
// ---------------------------------------------------------------------------
//    
TUint8 CBubbleManager::ActiveCallCount() const
    {
    return iActiveHeaders->Count();    
    }

// ---------------------------------------------------------------------------
//  CBubbleManager::MainPaneImageRect
//  
// 
// ---------------------------------------------------------------------------
//
const TRect CBubbleManager::MainPaneImageRect( 
    TBool aTinyImage,
    TBool aThreeLinesShown ) const
    {
    TRect imageRect; // whole main
    
    if ( aTinyImage )
        {
        TInt variety = aThreeLinesShown ? 1 : 0;
        
        if ( iConfigFlags & EBMTouchCallhandling )
            {
            // requires call2_image_placing_area to LAF data.
            TAknLayoutRect callPane;
            callPane.LayoutRect(
                Rect(), 
                BubbleLayout4::call4_windows_pane(2) );    
            
            TAknLayoutRect image;
            image.LayoutRect(
                callPane.Rect(), 
                BubbleLayout2::call2_image_placing_area( variety ) );                
            imageRect = image.Rect();                
            }
        else
            {
            TAknLayoutRect image;
            image.LayoutRect(
                Rect(), 
                BubbleLayout2::call2_image_placing_area( variety ) );    
            imageRect = image.Rect();    
            }
        }
    else
        {
        if ( iConfigFlags & EBMTouchCallhandling )
            {
            TAknLayoutRect image;
            image.LayoutRect(
                Rect(), 
                BubbleLayout4::call4_image_pane( 2 ) );    
            imageRect = image.Rect();                
            }
        else
            {
            // mainpane
            imageRect = Rect();    
            }            
        }        
    
    return imageRect;
    }

// ---------------------------------------------------------------------------
//  CBubbleManager::PrepareIcons
//  
// 
// ---------------------------------------------------------------------------
//    
EXPORT_C void CBubbleManager::PrepareIcons()
    {
    iImageManager->PrepareCachedBitmaps();
    }

// ---------------------------------------------------------------------------
//  CBubbleManager::PreloadCallThemeImage
//  
// 
// ---------------------------------------------------------------------------
//
EXPORT_C void CBubbleManager::PreloadCallThemeImage( TBool aPreload )
    {
    __ASSERT_ALWAYS( ( iConfigFlags & EBMCallObjectDisplay ), 
                       Panic( EBMPanicFeatureNotSupported ) );
    StartChanges();
    iMainPaneControl->Reset();
    if ( aPreload )
        {
        iCallObjectManager->LoadCallThemeImage( 
                *iImageManager,
                MainPaneImageRect().Size() ); 
        }
    else
        {
        iCallObjectManager->ReleaseCallThemeImage();
        }        
    EndChanges();
    }

// ---------------------------------------------------------------------------
//  CBubbleManager::StartAnimations
//  
// 
// ---------------------------------------------------------------------------
//    
void CBubbleManager::StartAnimations()
    {
    if ( !IsVisible() )
        {
        // Animation will be started when this control becomes visible.
        return;            
        }
    
    for ( TInt i = 0 ; i < iActivePlaces->Count() ; i++ )
        {
        TRAP_IGNORE( 
            iActivePlaces->At(i)->GetBubbleOutlook().HandleAnimationStartL() );
        }    
    }

// ---------------------------------------------------------------------------
//  CBubbleManager::HandleBackgroundImageChange
//  
// 
// ---------------------------------------------------------------------------
//    
EXPORT_C void CBubbleManager::HandleBackgroundImageChange()
    {
    // Restart animations to update background frames.
    StartAnimations();    
    }

// ---------------------------------------------------------------------------
//  CBubbleManager::SetVideoRingTone
//  
// 
// ---------------------------------------------------------------------------
// 
EXPORT_C void CBubbleManager::SetVideoRingTone(
    const TDesC& aFileName,
    TBubbleVideoPlayMode aPlayMode,
    TInt aVolumeLevel,
    TBool aUseArbitraryScaling,
    MBubbleVideoPlaybackObserver* aObserver )
    {
    iVideoController->StopAndDeletePlayer();
    iVideoController->PrepareToPlayVideo( aFileName,
                                          aPlayMode,
                                          aVolumeLevel,
                                          aUseArbitraryScaling,
                                          aObserver );
    
    iVideoFlags |=  EBMVideoSetAsRingTone;
    
    if ( iCallObjectManager )
        {
        iCallObjectManager->Suspend();            
        }    
    }

// ---------------------------------------------------------------------------
//  CBubbleManager::StopVideoRingTone
//  
// 
// ---------------------------------------------------------------------------
//
EXPORT_C void CBubbleManager::StopVideoRingTone()
    {
    iVideoController->StopPlaying();                
    }

// ---------------------------------------------------------------------------
//  CBubbleManager::MuteVideoRingTone
//  
// 
// ---------------------------------------------------------------------------
//        
EXPORT_C void CBubbleManager::MuteVideoRingTone()
    {
    iVideoController->MutePlaying();                
    }

// ---------------------------------------------------------------------------
//  CBubbleManager::StopVideoRingTone
//  
// 
// ---------------------------------------------------------------------------
//        
EXPORT_C void CBubbleManager::CancelVideoRingTone()
    {
    StopAndDeleteVideoPlayer();
    }
    
// ---------------------------------------------------------------------------
//  CBubbleManager::CreateVideoPlayerAndPlay
//  
// 
// ---------------------------------------------------------------------------
//
void CBubbleManager::CreateVideoPlayerAndPlay()
    { 
    iVideoController->CreatePlayerAndPlay();
    iVideoFlags |= EBMVideoPlayingVideo;
    }
    
// ---------------------------------------------------------------------------
//  CBubbleManager::StopAndDeleteVideoPlayer
//  
// 
// ---------------------------------------------------------------------------
//
void CBubbleManager::StopAndDeleteVideoPlayer()
    {
    iVideoController->StopAndDeletePlayer();
    iVideoFlags &= ~EBMVideoPlayingVideo;
    iVideoFlags &= ~EBMVideoSetAsRingTone;
    if ( iCallObjectManager )
        {
        iCallObjectManager->Resume();            
        }
    }
    
// ---------------------------------------------------------------------------
//  CBubbleManager::VideoController
//  
// 
// ---------------------------------------------------------------------------
//
CBubbleVideoController& CBubbleManager::VideoController() const
    {
    return *iVideoController;    
    }

// ---------------------------------------------------------------------------
//  CBubbleManager::CallState
//  
// 
// ---------------------------------------------------------------------------
//
CBubbleManager::TPhoneCallState CBubbleManager::CallState( 
    const TBubbleId& aBubbleId )
    {
    CBubbleHeader* header = NULL;
    __ASSERT_ALWAYS( FindActiveHeader( aBubbleId , header ) ,  
                     Panic( EBMPanicBubbleIdIsNotInUse ) );
    return header->CallState();                         
    }
    

// ---------------------------------------------------------------------------
//  CBubbleManager::CallObjectImageIncallSize
//  
// 
// ---------------------------------------------------------------------------
//
TSize CBubbleManager::CallObjectImageIncallSize( TBool aBigBubble )
    {
    TAknLayoutRect bubbleRect;

    if ( iConfigFlags & EBMTouchCallhandling )
        {
        TInt variety = aBigBubble ? 4 : 0;
        
        bubbleRect.LayoutRect( Rect(),
            BubbleLayout4::popup_call4_audio_first_window(variety) );
        }
    else
        {
        if ( aBigBubble )    
            {
            bubbleRect.LayoutRect( Rect(),
                BubbleLayout2::popup_call2_audio_first_window(0) );
            }
        else
            {
            bubbleRect.LayoutRect( Rect(),
               BubbleLayout2::popup_call2_audio_in_window(0) );    
            }
        }
            
    // image size is half from bubble size
    TSize imageSize( bubbleRect.Rect().Size() );
    // Opaque part in bubble approx. 45%.
    imageSize.iWidth -= (45 * imageSize.iWidth) / 100;
    return imageSize;  
    }

// ---------------------------------------------------------------------------
//  CBubbleManager::SetParticipantListCLI
//  
// 
// ---------------------------------------------------------------------------
//    
EXPORT_C void CBubbleManager::SetParticipantListCLI(
    const TBubbleId& aBubbleId,
    TBubbleParticipantListCLI aParticipantCLI )
    {
    CBubbleHeader* header = NULL;
    __ASSERT_ALWAYS( FindActiveHeader( aBubbleId , header ) ,  
                     Panic( EBMPanicBubbleIdIsNotInUse ) );
    header->SetParticipantListCLI( aParticipantCLI );
    }

// ---------------------------------------------------------------------------
//  CBubbleManager::TouchPane
//  
// 
// ---------------------------------------------------------------------------
//
EXPORT_C MBubbleTouchPaneInterface* CBubbleManager::TouchPane()
    {
    return iTouchPane;    
    }

// ---------------------------------------------------------------------------
//  CBubbleManager::SetTouchPaneVisible
//  
// 
// ---------------------------------------------------------------------------
//    
EXPORT_C void CBubbleManager::SetTouchPaneVisible( TBool aVisible )
    {
    iTouchPaneSetVisible = aVisible;   
    }

// ---------------------------------------------------------------------------
//  CBubbleManager::InitializingCall
//  
// 
// ---------------------------------------------------------------------------
//    
TBool CBubbleManager::InitializingCall() const
    {
    TBool initializingCall = EFalse;
    
    TInt activeHeaderCount( iActiveHeaders->Count() );
    for ( TInt i(0); i < activeHeaderCount; i++ )
        {
        CBubbleHeader* header = iActiveHeaders->At( i );
        TPhoneCallState callState( header->CallState() );    
        if ( callState == EIncoming || callState == EWaiting ||
             callState == EAlerting || callState == EOutgoing ||
             callState == EAlertToDisconnected )
            {
            initializingCall = ETrue;
            break;
            }
        }
        
    return initializingCall;
    }

// ---------------------------------------------------------------------------
//  CBubbleManager::IsTouchCallHandling
//  
// 
// ---------------------------------------------------------------------------
//    
TBool CBubbleManager::IsTouchCallHandling() const
    {
    return (iConfigFlags & EBMTouchCallhandling);    
    }

// ---------------------------------------------------------------------------
//  CBubbleManager::AddCustomElement
//  
// 
// ---------------------------------------------------------------------------
//
EXPORT_C void CBubbleManager::AddCustomElement( 
    const TBubbleId& aBubbleId,
    CTelBubbleCustomElement* aElement,
    TInt aPriority )
    {
    if ( aElement->ElementType() == CTelBubbleCustomElement::ECallImage &&
         aBubbleId == iMainPaneControl->BubbleId() )
        {
        // reset control to get it replaced with this custom element
        iMainPaneControl->Reset();
        }
    
    iCustomManager->AddCustomElement( aBubbleId, aElement, aPriority );
    }
        
// ---------------------------------------------------------------------------
//  CBubbleManager::RemoveCustomElement
//  
// 
// ---------------------------------------------------------------------------
//
EXPORT_C void CBubbleManager::RemoveCustomElement( 
    const TBubbleId& aBubbleId,
    CTelBubbleCustomElement* aElement )
    {
    if ( aElement->ElementType() == CTelBubbleCustomElement::ECallImage &&
         aBubbleId == iMainPaneControl->BubbleId() )
        {
        // aElement is being used; release it, before removing
        // it from custom manager.
        iMainPaneControl->Reset();
        }
    
    iCustomManager->RemoveCustomElement( aBubbleId, aElement );
    }

// End of File
