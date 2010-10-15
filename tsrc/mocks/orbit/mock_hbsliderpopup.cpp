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
#include "hbsliderpopup.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// HbSliderPopup::HbSliderPopup
// -----------------------------------------------------------------------------
//
HbSliderPopup::HbSliderPopup( 
        QGraphicsItem * parent )
    : HbDialog( parent )
    {
    
    }


// -----------------------------------------------------------------------------
// HbSliderPopup::HbSliderPopup
// -----------------------------------------------------------------------------
//
HbSliderPopup::HbSliderPopup( 
        Qt::Orientation orientation,
        QGraphicsItem * parent )
    //:
    //HbDialog( /*orientation, parent*/ )
    {
    
    }


// -----------------------------------------------------------------------------
// HbSliderPopup::~HbSliderPopup
// -----------------------------------------------------------------------------
//
HbSliderPopup::~HbSliderPopup(  )
    {
    
    }


// -----------------------------------------------------------------------------
// HbSliderPopup::invertedAppearance
// -----------------------------------------------------------------------------
//
bool HbSliderPopup::invertedAppearance(  ) const
    {
    SMC_MOCK_METHOD0( bool )
    }


// -----------------------------------------------------------------------------
// HbSliderPopup::setInvertedAppearance
// -----------------------------------------------------------------------------
//
void HbSliderPopup::setInvertedAppearance( 
        bool inverted )
    {
    SMC_MOCK_METHOD1( void, bool, inverted )
    }


// -----------------------------------------------------------------------------
// HbSliderPopup::maximum
// -----------------------------------------------------------------------------
//
int HbSliderPopup::maximum(  ) const
    {
    SMC_MOCK_METHOD0( int )
    }


// -----------------------------------------------------------------------------
// HbSliderPopup::setMaximum
// -----------------------------------------------------------------------------
//
void HbSliderPopup::setMaximum( 
        int max )
    {
    SMC_MOCK_METHOD1( void, int, max )
    }


// -----------------------------------------------------------------------------
// HbSliderPopup::minimum
// -----------------------------------------------------------------------------
//
int HbSliderPopup::minimum(  ) const
    {
    SMC_MOCK_METHOD0( int )
    }


// -----------------------------------------------------------------------------
// HbSliderPopup::setMinimum
// -----------------------------------------------------------------------------
//
void HbSliderPopup::setMinimum( 
        int min )
    {
    SMC_MOCK_METHOD1( void, int, min )
    }


// -----------------------------------------------------------------------------
// HbSliderPopup::setRange
// -----------------------------------------------------------------------------
//
void HbSliderPopup::setRange( 
        int min,
        int max )
    {
    SMC_MOCK_METHOD2( void, int, min, 
        int, max )
    }


// -----------------------------------------------------------------------------
// HbSliderPopup::orientation
// -----------------------------------------------------------------------------
//
Qt::Orientation HbSliderPopup::orientation(  ) const
    {
    SMC_MOCK_METHOD0( Qt::Orientation )
    }


// -----------------------------------------------------------------------------
// HbSliderPopup::pageStep
// -----------------------------------------------------------------------------
//
int HbSliderPopup::pageStep(  ) const
    {
    SMC_MOCK_METHOD0( int )
    }


// -----------------------------------------------------------------------------
// HbSliderPopup::setPageStep
// -----------------------------------------------------------------------------
//
void HbSliderPopup::setPageStep( 
        int step )
    {
    SMC_MOCK_METHOD1( void, int, step )
    }


// -----------------------------------------------------------------------------
// HbSliderPopup::singleStep
// -----------------------------------------------------------------------------
//
int HbSliderPopup::singleStep(  ) const
    {
    SMC_MOCK_METHOD0( int )
    }


// -----------------------------------------------------------------------------
// HbSliderPopup::setSingleStep
// -----------------------------------------------------------------------------
//
void HbSliderPopup::setSingleStep( 
        int step )
    {
    SMC_MOCK_METHOD1( void, int, step )
    }


// -----------------------------------------------------------------------------
// HbSliderPopup::sliderPosition
// -----------------------------------------------------------------------------
//
int HbSliderPopup::sliderPosition(  ) const
    {
    SMC_MOCK_METHOD0( int )
    }


// -----------------------------------------------------------------------------
// HbSliderPopup::setSliderPosition
// -----------------------------------------------------------------------------
//
void HbSliderPopup::setSliderPosition( 
        int pos )
    {
    SMC_MOCK_METHOD1( void, int, pos )
    }


// -----------------------------------------------------------------------------
// HbSliderPopup::hasTracking
// -----------------------------------------------------------------------------
//
bool HbSliderPopup::hasTracking(  ) const
    {
    SMC_MOCK_METHOD0( bool )
    }


// -----------------------------------------------------------------------------
// HbSliderPopup::setTracking
// -----------------------------------------------------------------------------
//
void HbSliderPopup::setTracking( 
        bool enable )
    {
    SMC_MOCK_METHOD1( void, bool, enable )
    }


// -----------------------------------------------------------------------------
// HbSliderPopup::tickPosition
// -----------------------------------------------------------------------------
//
Hb::SliderTickPositions HbSliderPopup::tickPosition(  ) const
    {
    SMC_MOCK_METHOD0( Hb::SliderTickPositions )
    }


// -----------------------------------------------------------------------------
// HbSliderPopup::setTickPosition
// -----------------------------------------------------------------------------
//
void HbSliderPopup::setTickPosition( 
        Hb::SliderTickPositions position )
    {
    SMC_MOCK_METHOD1( void, /*Hb::SliderTickPositions*/int, position )
    }


// -----------------------------------------------------------------------------
// HbSliderPopup::snappingMode
// -----------------------------------------------------------------------------
//
HbSlider::SnappingMode HbSliderPopup::snappingMode(  ) const
    {
    SMC_MOCK_METHOD0( HbSlider::SnappingMode )
    }


// -----------------------------------------------------------------------------
// HbSliderPopup::setSnappingMode
// -----------------------------------------------------------------------------
//
void HbSliderPopup::setSnappingMode( 
        HbSlider::SnappingMode mode )
    {
    SMC_MOCK_METHOD1( void, HbSlider::SnappingMode, mode )
    }


// -----------------------------------------------------------------------------
// HbSliderPopup::icon
// -----------------------------------------------------------------------------
//
HbIcon HbSliderPopup::icon( 
        HbSlider::SliderElement element ) const
    {
    SMC_MOCK_METHOD1( HbIcon, HbSlider::SliderElement, element )
    }


// -----------------------------------------------------------------------------
// HbSliderPopup::setIcon
// -----------------------------------------------------------------------------
//
void HbSliderPopup::setIcon( 
        HbSlider::SliderElement element,
        const HbIcon & icon )
    {
    SMC_MOCK_METHOD2( void, HbSlider::SliderElement, element, 
        const HbIcon &, icon )
    }


// -----------------------------------------------------------------------------
// HbSliderPopup::setElementIcon
// -----------------------------------------------------------------------------
//
void HbSliderPopup::setElementIcon( 
        HbSlider::SliderElement element,
        const HbIcon & icon )
    {
    SMC_MOCK_METHOD2( void, HbSlider::SliderElement, element, 
        const HbIcon &, icon )
    }


// -----------------------------------------------------------------------------
// HbSliderPopup::elementIcon
// -----------------------------------------------------------------------------
//
HbIcon HbSliderPopup::elementIcon( 
        HbSlider::SliderElement element ) const
    {
    SMC_MOCK_METHOD1( HbIcon, HbSlider::SliderElement, element )
    }


// -----------------------------------------------------------------------------
// HbSliderPopup::elements
// -----------------------------------------------------------------------------
//
QList <HbSlider::SliderElement > HbSliderPopup::elements(  ) const
    {
    SMC_MOCK_METHOD0( QList <HbSlider::SliderElement > )
    }


// -----------------------------------------------------------------------------
// HbSliderPopup::setElements
// -----------------------------------------------------------------------------
//
void HbSliderPopup::setElements( 
        const QList<HbSlider::SliderElement> & elements )
    {
    SMC_MOCK_METHOD1( void, const QList<HbSlider::SliderElement> &, elements )
    }


// -----------------------------------------------------------------------------
// HbSliderPopup::elementIcons
// -----------------------------------------------------------------------------
//
typedef QMap<QString, QVariant> qmapDefine;
QMap <QString , QVariant > HbSliderPopup::elementIcons(  ) const
    {
    SMC_MOCK_METHOD0( qmapDefine )
    }


// -----------------------------------------------------------------------------
// HbSliderPopup::setElementIcons
// -----------------------------------------------------------------------------
//
void HbSliderPopup::setElementIcons( 
        const QMap <QString, QVariant> &elements )
    {
    SMC_MOCK_METHOD1( void, const qmapDefine, elements )
    }


// -----------------------------------------------------------------------------
// HbSliderPopup::setSliderElements
// -----------------------------------------------------------------------------
//
void HbSliderPopup::setSliderElements( 
        const QList<QVariant> & elements )
    {
    SMC_MOCK_METHOD1( void, const QList<QVariant> &, elements )
    }


// -----------------------------------------------------------------------------
// HbSliderPopup::sliderElements
// -----------------------------------------------------------------------------
//
QList <QVariant > HbSliderPopup::sliderElements(  ) const
    {
    SMC_MOCK_METHOD0( QList <QVariant > )
    }


// -----------------------------------------------------------------------------
// HbSliderPopup::majorTickInterval
// -----------------------------------------------------------------------------
//
int HbSliderPopup::majorTickInterval(  ) const
    {
    SMC_MOCK_METHOD0( int )
    }


// -----------------------------------------------------------------------------
// HbSliderPopup::setMajorTickInterval
// -----------------------------------------------------------------------------
//
void HbSliderPopup::setMajorTickInterval( 
        int interval )
    {
    SMC_MOCK_METHOD1( void, int, interval )
    }


// -----------------------------------------------------------------------------
// HbSliderPopup::minorTickInterval
// -----------------------------------------------------------------------------
//
int HbSliderPopup::minorTickInterval(  ) const
    {
    SMC_MOCK_METHOD0( int )
    }


// -----------------------------------------------------------------------------
// HbSliderPopup::setMinorTickInterval
// -----------------------------------------------------------------------------
//
void HbSliderPopup::setMinorTickInterval( 
        int interval )
    {
    SMC_MOCK_METHOD1( void, int, interval )
    }


// -----------------------------------------------------------------------------
// HbSliderPopup::majorTickLabels
// -----------------------------------------------------------------------------
//
QStringList HbSliderPopup::majorTickLabels(  ) const
    {
    SMC_MOCK_METHOD0( QStringList )
    }


// -----------------------------------------------------------------------------
// HbSliderPopup::setMajorTickLabels
// -----------------------------------------------------------------------------
//
void HbSliderPopup::setMajorTickLabels( 
        const QStringList & majorTickLabels )
    {
    SMC_MOCK_METHOD1( void, const QStringList &, majorTickLabels )
    }


// -----------------------------------------------------------------------------
// HbSliderPopup::minorTickLabels
// -----------------------------------------------------------------------------
//
QStringList HbSliderPopup::minorTickLabels(  ) const
    {
    SMC_MOCK_METHOD0( QStringList )
    }


// -----------------------------------------------------------------------------
// HbSliderPopup::setMinorTickLabels
// -----------------------------------------------------------------------------
//
void HbSliderPopup::setMinorTickLabels( 
        const QStringList & minorTickLabels )
    {
    SMC_MOCK_METHOD1( void, const QStringList &, minorTickLabels )
    }


// -----------------------------------------------------------------------------
// HbSliderPopup::value
// -----------------------------------------------------------------------------
//
int HbSliderPopup::value(  ) const
    {
    SMC_MOCK_METHOD0( int )
    }


// -----------------------------------------------------------------------------
// HbSliderPopup::text
// -----------------------------------------------------------------------------
//
QString HbSliderPopup::text(  ) const
    {
    SMC_MOCK_METHOD0( QString )
    }


// -----------------------------------------------------------------------------
// HbSliderPopup::setToolTipAlignment
// -----------------------------------------------------------------------------
//
void HbSliderPopup::setToolTipAlignment( 
        Qt::Alignment a)
    {
    SMC_MOCK_METHOD1( void, /*Qt::Alignment*/int, a )
    }


// -----------------------------------------------------------------------------
// HbSliderPopup::toolTipAlignment
// -----------------------------------------------------------------------------
//
Qt::Alignment HbSliderPopup::toolTipAlignment(  ) const
    {
    SMC_MOCK_METHOD0( Qt::Alignment )
    }


// -----------------------------------------------------------------------------
// HbSliderPopup::setToolTipVisible
// -----------------------------------------------------------------------------
//
void HbSliderPopup::setToolTipVisible( bool visible )
    {
    SMC_MOCK_METHOD1( void, bool, visible )
    }


// -----------------------------------------------------------------------------
// HbSliderPopup::isToolTipVisible
// -----------------------------------------------------------------------------
//
bool HbSliderPopup::isToolTipVisible(  )
    {
    SMC_MOCK_METHOD0( bool )
    }


// -----------------------------------------------------------------------------
// HbSliderPopup::setTrackFilled
// -----------------------------------------------------------------------------
//
void HbSliderPopup::setTrackFilled( 
        bool trackVisible )
    {
    SMC_MOCK_METHOD1( void, bool, trackVisible )
    }


// -----------------------------------------------------------------------------
// HbSliderPopup::isTrackFilled
// -----------------------------------------------------------------------------
//
bool HbSliderPopup::isTrackFilled(  ) const
    {
    SMC_MOCK_METHOD0( bool )
    }


// -----------------------------------------------------------------------------
// HbSliderPopup::primitive
// -----------------------------------------------------------------------------
//
QGraphicsItem * HbSliderPopup::primitive( 
        HbStyle::Primitive primitive ) const
    {
    SMC_MOCK_METHOD1( QGraphicsItem *, HbStyle::Primitive, primitive )
    }


// -----------------------------------------------------------------------------
// HbSliderPopup::setValue
// -----------------------------------------------------------------------------
//
void HbSliderPopup::setValue( 
        int value )
    {
    SMC_MOCK_METHOD1( void, int, value )
    }


// -----------------------------------------------------------------------------
// HbSliderPopup::setText
// -----------------------------------------------------------------------------
//
void HbSliderPopup::setText( 
        const QString & text )
    {
    SMC_MOCK_METHOD1( void, const QString &, text )
    }


// -----------------------------------------------------------------------------
// HbSliderPopup::setOrientation
// -----------------------------------------------------------------------------
//
void HbSliderPopup::setOrientation( 
        Qt::Orientation orientation )
    {
    SMC_MOCK_METHOD1( void, Qt::Orientation, orientation )
    }


// -----------------------------------------------------------------------------
// HbSliderPopup::updatePrimitives
// -----------------------------------------------------------------------------
//
void HbSliderPopup::updatePrimitives(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// HbSliderPopup::valueChanged
// -----------------------------------------------------------------------------
//
void HbSliderPopup::valueChanged( 
        int value )
    {
    SMC_MOCK_METHOD1( void, int, value )
    }


// -----------------------------------------------------------------------------
// HbSliderPopup::sliderPressed
// -----------------------------------------------------------------------------
//
void HbSliderPopup::sliderPressed(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// HbSliderPopup::sliderReleased
// -----------------------------------------------------------------------------
//
void HbSliderPopup::sliderReleased(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// HbSliderPopup::sliderMoved
// -----------------------------------------------------------------------------
//
void HbSliderPopup::sliderMoved( 
        int value )
    {
    SMC_MOCK_METHOD1( void, int, value )
    }


// -----------------------------------------------------------------------------
// HbSliderPopup::rangeChanged
// -----------------------------------------------------------------------------
//
void HbSliderPopup::rangeChanged( 
        int min,
        int max )
    {
    SMC_MOCK_METHOD2( void, int, min, 
        int, max )
    }


// -----------------------------------------------------------------------------
// HbSliderPopup::iconPressed
// -----------------------------------------------------------------------------
//
void HbSliderPopup::iconPressed(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// HbSliderPopup::iconReleased
// -----------------------------------------------------------------------------
//
void HbSliderPopup::iconReleased(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// HbSliderPopup::iconClicked
// -----------------------------------------------------------------------------
//
void HbSliderPopup::iconClicked(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// HbSliderPopup::textClicked
// -----------------------------------------------------------------------------
//
void HbSliderPopup::textClicked(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// HbSliderPopup::HbSliderPopup
// -----------------------------------------------------------------------------
//
HbSliderPopup::HbSliderPopup( 
        HbSliderPopupPrivate & dd,
        QGraphicsItem * parent )
    //:
    //HbDialog( /*dd, parent*/ )
    {
    
    }


