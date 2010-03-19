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
* Description:  This file contains the header of the phone engine base 
*                central repository monitor class
*
*/


#ifndef CPEINCALLEARVOLUMESETTINGMONITOR_H
#define CPEINCALLEARVOLUMESETTINGMONITOR_H

// INCLUDE FILES
#include "cpecenrepmonitor.h"

// Forward declarations
class MPEPhoneModelInternal;

/**
*   CPEIncallEarVolumeSettingMonitor
*   For set/get of the phone ear volume setting (KTelIncallEarVolume) from 
*   the central repository 
*
*  @lib PhoneEngine.lib
*/
NONSHARABLE_CLASS( CPEIncallEarVolumeSettingMonitor ) : public CPECenRepMonitor
{
public:
    static CPEIncallEarVolumeSettingMonitor* NewL( MPEPhoneModelInternal& aModel );
    ~CPEIncallEarVolumeSettingMonitor();
public:
    TInt Set( TInt aValue );
protected: // From base class
    virtual void UpdateL();
protected:
    CPEIncallEarVolumeSettingMonitor( MPEPhoneModelInternal& aModel );
    void ConstructL();    
private:
    MPEPhoneModelInternal& iModel;
    TBool iUpdateInProgress;
};

#endif // CPEINCALLEARVOLUMESETTINGMONITOR_H
