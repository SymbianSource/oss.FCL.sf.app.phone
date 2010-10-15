/** Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of the License "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:
*
*/
#include <QDebug>
#include <smcmockclassincludes.h>
#include <hbdeviceprofile.h>
#include <QGraphicsItem>
#include <HbMainWindow>
#include <qshareddata.h>

// NOTE! The following header include requires
//       INCLUDEPATH += /sf/mw/hb/include/hbcore/private
//#include <hbdeviceprofile_p.h>
class HbDeviceProfilePrivate : public QSharedData {};

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// HbDeviceProfile::HbDeviceProfile
// -----------------------------------------------------------------------------
//
HbDeviceProfile::HbDeviceProfile(  )
    {
    //SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// HbDeviceProfile::HbDeviceProfile
// -----------------------------------------------------------------------------
//
HbDeviceProfile::HbDeviceProfile( 
        const HbDeviceProfile & /*other*/ )
    {
    //SMC_MOCK_METHOD1( void, const HbDeviceProfile &, other )
    }


// -----------------------------------------------------------------------------
// HbDeviceProfile::HbDeviceProfile
// -----------------------------------------------------------------------------
//
HbDeviceProfile::HbDeviceProfile( 
        const QString & /*name*/ )
    {
    //SMC_MOCK_METHOD1( void, const QString &, name )
    }


// -----------------------------------------------------------------------------
// HbDeviceProfile::~HbDeviceProfile
// -----------------------------------------------------------------------------
//
HbDeviceProfile::~HbDeviceProfile(  )
    {
    //SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// HbDeviceProfile::=
// -----------------------------------------------------------------------------
//
HbDeviceProfile & HbDeviceProfile::operator=( 
        const HbDeviceProfile & other )
    {
    SMC_MOCK_METHOD1( HbDeviceProfile &, const HbDeviceProfile &, other )
    }


// -----------------------------------------------------------------------------
// HbDeviceProfile::isNull
// -----------------------------------------------------------------------------
//
bool HbDeviceProfile::isNull(  ) const
    {
    SMC_MOCK_METHOD0( bool )
    }


// -----------------------------------------------------------------------------
// HbDeviceProfile::name
// -----------------------------------------------------------------------------
//
QString HbDeviceProfile::name(  ) const
    {
    SMC_MOCK_METHOD0( QString )
    }


// -----------------------------------------------------------------------------
// HbDeviceProfile::logicalSize
// -----------------------------------------------------------------------------
//
QSize HbDeviceProfile::logicalSize(  ) const
    {
    SMC_MOCK_METHOD0( QSize )
    }


// -----------------------------------------------------------------------------
// HbDeviceProfile::physicalSize
// -----------------------------------------------------------------------------
//
QSizeF HbDeviceProfile::physicalSize(  ) const
    {
    SMC_MOCK_METHOD0( QSizeF )
    }


// -----------------------------------------------------------------------------
// HbDeviceProfile::alternateProfileName
// -----------------------------------------------------------------------------
//
QString HbDeviceProfile::alternateProfileName(  ) const
    {
    SMC_MOCK_METHOD0( QString )
    }


// -----------------------------------------------------------------------------
// HbDeviceProfile::touch
// -----------------------------------------------------------------------------
//
bool HbDeviceProfile::touch(  ) const
    {
    SMC_MOCK_METHOD0( bool )
    }


// -----------------------------------------------------------------------------
// HbDeviceProfile::orientation
// -----------------------------------------------------------------------------
//
Qt::Orientation HbDeviceProfile::orientation(  ) const
    {
    SMC_MOCK_METHOD0( Qt::Orientation )
    }


// -----------------------------------------------------------------------------
// HbDeviceProfile::orientationAngle
// -----------------------------------------------------------------------------
//
qreal HbDeviceProfile::orientationAngle(  ) const
    {
    SMC_MOCK_METHOD0( qreal )
    }


// -----------------------------------------------------------------------------
// HbDeviceProfile::unitValue
// -----------------------------------------------------------------------------
//
qreal HbDeviceProfile::unitValue(  ) const
    {
    SMC_MOCK_METHOD0( qreal )
    }


// -----------------------------------------------------------------------------
// HbDeviceProfile::ppmValue
// -----------------------------------------------------------------------------
//
qreal HbDeviceProfile::ppmValue(  ) const
    {
    SMC_MOCK_METHOD0( qreal )
    }


// -----------------------------------------------------------------------------
// HbDeviceProfile::current
// -----------------------------------------------------------------------------
//
HbDeviceProfile HbDeviceProfile::current(  )
    {
    SMC_MOCK_METHOD0( HbDeviceProfile )
    }


// -----------------------------------------------------------------------------
// HbDeviceProfile::profile
// -----------------------------------------------------------------------------
//
HbDeviceProfile HbDeviceProfile::profile( 
        const HbMainWindow * window )
    {
    SMC_MOCK_METHOD1( HbDeviceProfile, const HbMainWindow *, window )
    }


// -----------------------------------------------------------------------------
// HbDeviceProfile::profile
// -----------------------------------------------------------------------------
//
HbDeviceProfile HbDeviceProfile::profile( 
        const QGraphicsItem * item )
    {
    SMC_MOCK_METHOD1( HbDeviceProfile, const QGraphicsItem *, item )
    }


// -----------------------------------------------------------------------------
// HbDeviceProfile::profileNames
// -----------------------------------------------------------------------------
//
QStringList HbDeviceProfile::profileNames(  )
    {
    SMC_MOCK_METHOD0( QStringList )
    }


