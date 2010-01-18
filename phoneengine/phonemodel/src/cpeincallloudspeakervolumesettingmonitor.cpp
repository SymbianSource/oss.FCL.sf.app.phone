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
#include "cpeincallloudspeakervolumesettingmonitor.h"
#include "mpephonemodelinternal.h"
#include <centralrepository.h>
#include <mpedatastore.h>
#include <talogger.h>
#include <telephonyvariant.hrh>
#include <telincallvolcntrlcrkeys.h>


// -----------------------------------------------------------------------------
// CPEIncallLoudspeakerVolumeSettingMonitor::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPEIncallLoudspeakerVolumeSettingMonitor* CPEIncallLoudspeakerVolumeSettingMonitor::NewL(
        MPEPhoneModelInternal& aModel
        )
    {
    CPEIncallLoudspeakerVolumeSettingMonitor* me = 
        new ( ELeave ) CPEIncallLoudspeakerVolumeSettingMonitor( aModel );
    CleanupStack::PushL( me );
    me->ConstructL(); 
    CleanupStack::Pop( me );
    return ( me );
    }

// Destructor
CPEIncallLoudspeakerVolumeSettingMonitor::~CPEIncallLoudspeakerVolumeSettingMonitor()
    {
    // Cancel() is called by base class destructor, which also deletes iRepository
    }

// -----------------------------------------------------------------------------
// CPEIncallLoudspeakerVolumeSettingMonitor::CPEIncallLoudspeakerVolumeSettingMonitor
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPEIncallLoudspeakerVolumeSettingMonitor::CPEIncallLoudspeakerVolumeSettingMonitor(
        MPEPhoneModelInternal& aModel
        ) : CPECenRepMonitor( KTelIncallLoudspeakerVolume ),
            iModel( aModel ),
            iUpdateInProgress( EFalse )
    {
    }

// -----------------------------------------------------------------------------
// CPEIncallLoudspeakerVolumeSettingMonitor::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPEIncallLoudspeakerVolumeSettingMonitor::ConstructL()
    {
    BaseConstructL( KCRUidInCallVolume );

    TInt volume = KPEDefaultAudioVolume; // Default setting
    Get( volume );   
    TEFLOGSTRING2( 
        KTAREQOUT, 
        "PE CPEIncallLoudspeakerVolumeSettingMonitor::ConstructL, Incall loudspeaker volume = %d ",
        volume );
    //In boot, cannot write volume value to datastore. It override HeadSet mode 
    // volume ( default value ).
    }

// -----------------------------------------------------------------------------
// CPEIncallLoudspeakerVolumeSettingMonitor::Set
// -----------------------------------------------------------------------------
//
TInt CPEIncallLoudspeakerVolumeSettingMonitor::Set(
        TInt aValue
        )
    {
    TInt errorCode( KErrNone );

    // Check whether this function call was initiated by a central repository
    // change notification.
    if( iUpdateInProgress == EFalse )
        {
        errorCode = iRepository->Set( iMonitorSetting, aValue );
        TEFLOGSTRING3( KTAREQOUT, "PE CPEIncallLoudspeakerVolumeSettingMonitor::Set \
> CRepository::Set, Incall loudspeaker volume = %d, error code: %d", aValue, errorCode );
        }

    return errorCode;
    }

// -----------------------------------------------------------------------------
// CPEIncallLoudspeakerVolumeSettingMonitor::UpdateL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPEIncallLoudspeakerVolumeSettingMonitor::UpdateL()
    {
    TInt volume;
    // The loudspeaker volume has changed in repository.
    // Retrieve the current volume from repository.
    User::LeaveIfError( Get( volume ) );

    TEFLOGSTRING2( 
        KTAREQOUT,
        "PE CPEIncallLoudspeakerVolumeSettingMonitor::UpdateL, Incall loudspeaker volume = %d ",
        volume );

    // Update loudspeaker volume using an audiohandling routine
    // The routine will try to update the repository as well,
    // ensure this is blocked using an internal flag.
    iUpdateInProgress = ETrue;
    iModel.DataStore()->SetAudioVolumeCommand( volume );
    iModel.HandleInternalMessage( MPEPhoneModel::EPEMessageSetAudioVolume );

    // Reset the flag when synchronization is complete
    iUpdateInProgress = EFalse;
    }

// End of file
