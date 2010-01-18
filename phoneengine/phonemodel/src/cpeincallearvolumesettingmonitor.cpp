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
* Description:  Contains phone engine base central repository monitor class
*
*/


// INCLUDE FILES
#include "cpeincallearvolumesettingmonitor.h"
#include "mpephonemodelinternal.h"
#include <centralrepository.h>
#include <mpedatastore.h>
#include <talogger.h>
#include <telephonyvariant.hrh>
#include <telincallvolcntrlcrkeys.h>


// -----------------------------------------------------------------------------
// CPEIncallEarVolumeSettingMonitor::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPEIncallEarVolumeSettingMonitor* CPEIncallEarVolumeSettingMonitor::NewL(
        MPEPhoneModelInternal& aModel )
    {
    CPEIncallEarVolumeSettingMonitor* self = new ( ELeave ) CPEIncallEarVolumeSettingMonitor( 
        aModel );
    CleanupStack::PushL( self );
    self->ConstructL(); 
    CleanupStack::Pop( self);
    return ( self );
    }

// Destructor
CPEIncallEarVolumeSettingMonitor::~CPEIncallEarVolumeSettingMonitor()
    {
    // Cancel() is called by base class destructor, which also deletes iRepository
    }

// -----------------------------------------------------------------------------
// CPEIncallEarVolumeSettingMonitor::CPEIncallEarVolumeSettingMonitor
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPEIncallEarVolumeSettingMonitor::CPEIncallEarVolumeSettingMonitor(
        MPEPhoneModelInternal& aModel
        ) : CPECenRepMonitor( KTelIncallEarVolume ),
            iModel( aModel ),
            iUpdateInProgress( EFalse )
    {
    }

// -----------------------------------------------------------------------------
// CPEIncallEarVolumeSettingMonitor::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPEIncallEarVolumeSettingMonitor::ConstructL()
    {
    BaseConstructL( KCRUidInCallVolume );

    TInt volume = KPEDefaultAudioVolume; // Default setting
    Get( volume );   
    TEFLOGSTRING2( 
        KTAREQOUT, 
        "PE CPEIncallEarVolumeSettingMonitor::ConstructL, Incall ear volume = %d ",
        volume );
    // Write audio volume to EngineInfo.
    iModel.DataStore()->SetAudioVolume( volume );
    }

// -----------------------------------------------------------------------------
// CPEIncallEarVolumeSettingMonitor::Set
// -----------------------------------------------------------------------------
//
TInt CPEIncallEarVolumeSettingMonitor::Set(
        TInt aValue )
    {
    TInt errorCode( KErrNone );

    // Check whether this function call was initiated by a central repository
    // change notification.
    if( iUpdateInProgress == EFalse )
        {
        errorCode = iRepository->Set( iMonitorSetting, aValue );
        TEFLOGSTRING3( KTAREQOUT, "PE CPEIncallEarVolumeSettingMonitor::Set \
> CRepository::Set, Incall ear volume = %d, error code: %d", aValue, errorCode );
        }

    return errorCode;
    }

// -----------------------------------------------------------------------------
// CPEIncallEarVolumeSettingMonitor::UpdateL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPEIncallEarVolumeSettingMonitor::UpdateL()
    {
    TInt volume;
    // The headset volume has changed in repository.
    // Retrieve the current volume from repository.
    User::LeaveIfError( Get( volume ) );

    TEFLOGSTRING2( 
        KTAREQOUT,
        "PE: CPEIncallEarVolumeSettingMonitor::UpdateL, Incall ear volume = %d ",
        volume );

    // Update headset volume using an audiohandling routine
    // The routine will try to update the repository as well,
    // ensure this is blocked using an internal flag.
    iUpdateInProgress = ETrue;
    iModel.DataStore()->SetAudioVolumeCommand( volume );
    iModel.HandleInternalMessage( MPEPhoneModel::EPEMessageSetAudioVolume );

    // Reset the flag when synchronization is complete
    iUpdateInProgress = EFalse;

    }
    
// End of file
