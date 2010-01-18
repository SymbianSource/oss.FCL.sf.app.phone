/*
* Copyright (c) 2005-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Implementation of CPhoneAudioController class.
*
*/


// INCLUDE FILES
#include <aknnavi.h>
#include <aknnavide.h>
#include <StringLoader.h>
#include <avkon.rsg>
#include <phoneui.rsg>
#include <telephonyvariant.hrh>
#include <eikenv.h>
#include <eikappui.h>
#include <AknUtils.h>
#include "cphoneaudiocontroller.h"
#include "cphonerecoverysystem.h"
#include "cphonecenrepproxy.h"
#include "cphonestatuspane.h"
#include "cphonetimer.h"
#include "tphonecommandparam.h"
#include "tphonecmdparamboolean.h"
#include "tphonecmdparaminteger.h"
#include "phoneconstants.h"
#include "phonelogger.h"
#include "phoneui.hrh"

// CONSTANTS
// Defines how long time volume control is shown when volume is changed 
// while device is muted. Duration in microseconds.
const TInt KPhoneDelayVolumeControlToDefaultAfterReleased = 1000000;

// ================= MEMBER FUNCTIONS =======================
// C++ default constructor can NOT contain any code, that
// might leave.
//
CPhoneAudioController::CPhoneAudioController() :
    iIhfVolume( KPhoneDefaultVolume ),
    iEarVolume( KPhoneDefaultVolume )
    {
    }

// ---------------------------------------------------------
// CPhoneAudioController::ConstructL()
// Construction method for CPhoneAudioController.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPhoneAudioController::ConstructL()
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneAudioController::ConstructL()");

    // Start observing foreground events
    CCoeEnv::Static()->AddForegroundObserverL( *this );

    // Create the status pane singleton
    iStatusPane = CPhoneStatusPane::Instance();

    // Create volume controls
    iEarVolumeControl = 
        iStatusPane->NaviPane().CreateVolumeIndicatorL( 
        R_AVKON_NAVI_PANE_EARPIECE_VOLUME_INDICATOR );
    
    iIhfVolumeControl = 
        iStatusPane->NaviPane().CreateVolumeIndicatorL( 
        R_AVKON_NAVI_PANE_VOLUME_INDICATOR );
        
    // To get touch input, pen check in HandleControlEventL()
    iEarVolumeControl->DecoratedControl()->SetObserver( this );
    iIhfVolumeControl->DecoratedControl()->SetObserver( this );            
  
    // Create "muted" navi decorator
    HBufC* mutedText = StringLoader::LoadLC( R_PHONE_INCALL_MUTED_PANE );
    iMutedControl = iStatusPane->NaviPane().CreateMessageLabelL( *mutedText );
    CleanupStack::PopAndDestroy( mutedText );
        
    iNaviPaneUpdateTimer = CPhoneTimer::NewL();

    iActivateRecoveryId = CPhoneRecoverySystem::Instance()->AddL( 
        TCallBack( DoRecoverActivateL, this ),
        CTeleRecoverySystem::EPhonePriorityStandard,
        CTeleRecoverySystem::EPhoneStateIdle );   
    }

// ---------------------------------------------------------
// CPhoneAudioController::NewL()
// Two-phased constructor
// (other items were commented in a header).
// ---------------------------------------------------------
CPhoneAudioController* CPhoneAudioController::NewL()
    {   
    CPhoneAudioController* self = 
        new( ELeave ) CPhoneAudioController();
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }

// ---------------------------------------------------------
// Destructor
// ---------------------------------------------------------
// 
CPhoneAudioController::~CPhoneAudioController()
    {
    CCoeEnv::Static()->RemoveForegroundObserver( *this );
    CPhoneRecoverySystem::Remove( iActivateRecoveryId );
     
    delete iNaviPaneUpdateTimer;   
    delete iMutedControl;
    delete iIhfVolumeControl;
    delete iEarVolumeControl;
    }

// ---------------------------------------------------------
// CPhoneAudioController::ActivateVolumeControlL
// ---------------------------------------------------------
//
void CPhoneAudioController::ActivateVolumeControlL()
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneAudioController::ActivateVolumeControlL()");
    iVolumeControlCount++;
    if ( iVolumeControlCount == 1 ) // First time activation
        {
        // Decide whether decorator should be Ear or ihf decorator
        CAknNavigationDecorator& selectedDecorator = SelectDecoratorL(); 
        PushL( selectedDecorator ); 
        }
    }

// ---------------------------------------------------------
// CPhoneAudioController::DoRecoverUpdateL
//
// Callback function.
// ---------------------------------------------------------
//
TInt CPhoneAudioController::DoRecoverActivateL( TAny* aAny )
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneAudioController::DoRecoverActivateL()");
    static_cast< CPhoneAudioController* >( aAny )->ActivateL();
    return KErrNone;
    }
    
// ---------------------------------------------------------
// CPhoneAudioController::ActivateL
//
// Updates volume control according to current audio data.
// ---------------------------------------------------------
//
void CPhoneAudioController::ActivateL()
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneAudioController::ActivateL()");
    
    if( iVolumeControlCount )
        {
        // Decide whether decorator should be Ear, Ihf or Muted decorator
        CAknNavigationDecorator& selectedDecorator = SelectDecoratorL(); 
        PushL( selectedDecorator );     
        
        if( &selectedDecorator != iMutedControl )
            {            
            TInt volumeLevel = VolumeLevel(); 
            
            SetVolumeLevel( VolumeControl(), volumeLevel );                 
            }      
        }
    }
    
// ---------------------------------------------------------
// CPhoneAudioController::DeactivateVolumeControl
// ---------------------------------------------------------
//
void CPhoneAudioController::DeactivateVolumeControl()
    {
    __LOGMETHODSTARTEND( EPhoneUIView,"CPhoneAudioController::DeactivateVolumeControl()" );
    if ( iOldControl )
        {
        iStatusPane->NaviPane().Pop( iOldControl );
        iOldControl = NULL;
        iVolumeControlCount = 0;
        }
    }
    
// ---------------------------------------------------------
// CPhoneAudioController::HandleVolumeChangeL
//
// Updates incall indicator according to current call state.
// ---------------------------------------------------------
//
void CPhoneAudioController::HandleVolumeChangeL(
    TPhoneCommandParam* aCommandParam )
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneAudioController::HandleVolumeChangeL()");
    if( !iPhoneInForeground )
        {
        // Telephony is in the background so it's not our responsibility
        // to display the volume control.
        __PHONELOG( EBasic, EPhoneUIView,"CPhoneAudioController::HandleVolumeChangeL - phone in the background" ); 
        return;
        }
    
    TPhoneCmdParamInteger* volumeParam =
        static_cast<TPhoneCmdParamInteger*>( aCommandParam );

    if ( iIhfMode )
        {
        iIhfVolume = volumeParam->Integer();
        }
    else
        {
        iEarVolume = volumeParam->Integer();
        }
    
    if( iMuted )
        {
        // Enable timed control transition in muted state    
        iTimedMuteTransferPending = ETrue; 
        }
    else
        {        
        iTimedMuteTransferPending = EFalse; 
        }
     
    // Activate the approriate volume control
    CPhoneRecoverySystem::Instance()->RecoverNow( 
        iActivateRecoveryId, 
        CTeleRecoverySystem::EPhonePriorityStandard );

    if( iTimedMuteTransferPending )    
        {
        iTimedMuteTransferPending = EFalse;     
        
        // Cancel any pending request
        iNaviPaneUpdateTimer->CancelTimer();

        // Activate change to Muted decorator after given delay        
        iNaviPaneUpdateTimer->After(                    
            KPhoneDelayVolumeControlToDefaultAfterReleased,
            TCallBack( DoUpdateNaviPaneToDefault, this ) );           
        }
    }
    
// ---------------------------------------------------------
// CPhoneAudioController::HandleIhfChange
//
// Updates incall indicator according to current call state.
// ---------------------------------------------------------
//
void CPhoneAudioController::HandleIhfChange(
    TPhoneCommandParam* aCommandParam )
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneAudioController::HandleIhfChange()");
    TPhoneCmdParamBoolean* booleanParam =
        static_cast<TPhoneCmdParamBoolean*>( aCommandParam );
    
    iIhfMode = booleanParam->Boolean();    
    
    // Set state transition flag 
    iIhfTransferPending = ETrue; 
    }
    
// ---------------------------------------------------------
// CPhoneAudioController::HandleMuteChange
//
// Updates incall indicator according to current call state.
// ---------------------------------------------------------
//
void CPhoneAudioController::HandleMuteChange(
    TPhoneCommandParam* aCommandParam )
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneAudioController::HandleMuteChange()");
    TPhoneCmdParamBoolean* booleanParam =
        static_cast<TPhoneCmdParamBoolean*>( aCommandParam );

    iMuted = booleanParam->Boolean();
    
    // Decide whether decorator should be ear, ihf or muted decorator
    TRAP_IGNORE( PushL( SelectDecoratorL() ) );
    }
    
// -----------------------------------------------------------------------------
// CPhoneAudioController::PushL
// -----------------------------------------------------------------------------
//
void CPhoneAudioController::PushL( CAknNavigationDecorator& aNew )
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneAudioController::PushL()");
    TInt err = KErrNone;
    if ( !iOldControl )
        {
        __PHONELOG( EBasic, EPhoneUIView,"CPhoneAudioController::iStatusPane->NaviPane().PushL( aNew );" ); 
        iStatusPane->NaviPane().PushL( aNew );
        }
    else if ( &aNew != iOldControl )
        {
        err = iStatusPane->NaviPane().ReplaceL( *iOldControl, aNew );
        }
    // otherwise iOldControl == &aNew.

    if ( err == KErrNone )
        {
        iOldControl = &aNew;
        }
    }

// -----------------------------------------------------------------------------
// CPhoneAudioController::DoUpdateNaviPaneToDefault
// -----------------------------------------------------------------------------
//
TInt CPhoneAudioController::DoUpdateNaviPaneToDefault( TAny* aAny )
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneAudioController::DoUpdateNaviPaneToDefault()");
    CPhoneAudioController* self = static_cast< CPhoneAudioController* >( aAny );
    CPhoneRecoverySystem::Instance()->RecoverNow( 
        self->iActivateRecoveryId, 
        CTeleRecoverySystem::EPhonePriorityStandard );
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CPhoneAudioController::HandleControlEventL
// -----------------------------------------------------------------------------
//    
void CPhoneAudioController::HandleControlEventL( 
    CCoeControl* aControl, 
    TCoeEvent aEventType )
    {
    __LOGMETHODSTARTEND( EPhoneUIView,"CPhoneAudioController::HandleControlEventL()" );
    if ( aEventType == MCoeControlObserver::EEventStateChanged &&
         AknLayoutUtils::PenEnabled() )
        {
        if ( aControl == iEarVolumeControl->DecoratedControl() )
            {
            TInt volume =  static_cast< CAknVolumeControl* > 
                               ( iEarVolumeControl->DecoratedControl() )
                               ->Value();
            
            if ( volume != iEarVolume )
                {
                // Volume level was changed via ui control
                iEarVolume = volume;
                CEikonEnv::Static()->EikAppUi()->HandleCommandL(
                    EPhoneInCallCmdSetVolumeLevel );                        
                }
            }
        else if ( aControl == iIhfVolumeControl->DecoratedControl() )
            {
            TInt volume =  static_cast< CAknVolumeControl* > 
                               ( iIhfVolumeControl->DecoratedControl() )
                               ->Value();
            
            if ( volume != iIhfVolume )
                {
                // Volume level was changed via ui control
                iIhfVolume = volume;
                CEikonEnv::Static()->EikAppUi()->HandleCommandL(
                    EPhoneInCallCmdSetVolumeLevel );                        
                }        
            }            
        }
    }

// -----------------------------------------------------------------------------
// CPhoneAudioController::VolumeLevelFromControl
// -----------------------------------------------------------------------------
//
TInt CPhoneAudioController::VolumeLevelFromControl()
    {       
    __LOGMETHODSTARTEND( EPhoneUIView,"CPhoneAudioController::VolumeLevelFromControl()" );         
    TInt volume = VolumeControl().Value();     
    
    return volume; 
    }
        
// -----------------------------------------------------------------------------
// CPhoneAudioController::VolumeDecorator
// -----------------------------------------------------------------------------
//    
CAknNavigationDecorator& CPhoneAudioController::VolumeDecorator()    
    {
    __LOGMETHODSTARTEND( EPhoneUIView,"CPhoneAudioController::VolumeDecorator()" );                 
    CAknNavigationDecorator* volumeDecorator = NULL;      
    if ( iIhfMode )
        {            
        volumeDecorator = iIhfVolumeControl;
        }
    else
        {
        volumeDecorator = iEarVolumeControl;        
        }
        
    return *volumeDecorator; 
    }

// -----------------------------------------------------------------------------
// CPhoneAudioController::VolumeControl
// -----------------------------------------------------------------------------
//    
CAknVolumeControl& CPhoneAudioController::VolumeControl()      
    {
    __LOGMETHODSTARTEND( EPhoneUIView,"CPhoneAudioController::VolumeControl()" );                     
    CAknVolumeControl* volumeControl = NULL;     
    volumeControl = static_cast< CAknVolumeControl* >( 
        VolumeDecorator().DecoratedControl() ); 

    return *volumeControl; 
    }

// -----------------------------------------------------------------------------
// CPhoneAudioController::VolumeLevel
// -----------------------------------------------------------------------------
//    
TInt CPhoneAudioController::VolumeLevel()    
    {
    __LOGMETHODSTARTEND( EPhoneUIView,"CPhoneAudioController::VolumeLevel()" );                             
    TInt volumeLevel;     
    if ( iIhfMode )
        {            
        volumeLevel = iIhfVolume; 
        }
    else
        {
        volumeLevel = iEarVolume;         
        }        
        
    return volumeLevel;
    }

// -----------------------------------------------------------------------------
// CPhoneAudioController::SelectDecoratorL
// -----------------------------------------------------------------------------
//    
CAknNavigationDecorator& CPhoneAudioController::SelectDecoratorL()
    {
    __LOGMETHODSTARTEND( EPhoneUIView,"CPhoneAudioController::SelectDecoratorL()" );                                     
    CAknNavigationDecorator* newDecorator = NULL;    
    if ( !iMuted )
        {
        __PHONELOG( EBasic, EPhoneUIView,"CPhoneAudioController::SelectDecoratorL() - volume decorator " );     
        newDecorator = &VolumeDecorator();
        }
    else 
        {
        if ( iIhfTransferPending || !iTimedMuteTransferPending )
            {
            __PHONELOG( EBasic, EPhoneUIView,"CPhoneAudioController::SelectDecoratorL() - muted decorator " );         
            newDecorator = iMutedControl;    
            }            
        else 
            {
            __PHONELOG( EBasic, EPhoneUIView,"CPhoneAudioController::SelectDecoratorL() - volume decorator " );         
            newDecorator = &VolumeDecorator();
            
            // In muted state the volume decorator must be first 
            // pushed to container to enable volume popup showing 
            PushL( *newDecorator );             
            }
        }

    // Clear IHF transfer flag. 
    iIhfTransferPending = EFalse;
           
    return *newDecorator;         
    }

// -----------------------------------------------------------------------------
// CPhoneAudioController::SetVolumeLevel
// -----------------------------------------------------------------------------
//    
void CPhoneAudioController::SetVolumeLevel(
    CAknVolumeControl& aVolumeControl, 
    TInt aVolumeLevel)
    {
    __LOGMETHODSTARTEND( EPhoneUIView,"CPhoneAudioController::SetVolumeLevel()" );   
    aVolumeControl.SetValue( aVolumeLevel ); 

    }

// -----------------------------------------------------------------------------
// CPhoneAudioController::HandleGainingForeground
// -----------------------------------------------------------------------------
//    
void CPhoneAudioController::HandleGainingForeground()
    {
    iPhoneInForeground = ETrue;
    }

// -----------------------------------------------------------------------------
// CPhoneAudioController::HandleLosingForeground
// -----------------------------------------------------------------------------
//    
void CPhoneAudioController::HandleLosingForeground()
    {
    iPhoneInForeground = EFalse;
    }

// End of file
