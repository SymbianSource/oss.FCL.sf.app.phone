/* 
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:
*
*/
#include <QDebug>
#include <smcmockclassincludes.h>
#include <hbabstractitemview.h>

HbScrollAreaPrivate* pointerHbScrollAreaPrivate(NULL);

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// HbAbstractItemView::HbAbstractItemView
// -----------------------------------------------------------------------------
//
HbAbstractItemView::HbAbstractItemView( 
        HbAbstractItemViewPrivate & dd,
        HbAbstractItemContainer * container,
        QGraphicsItem * parent )
    : HbScrollArea( *pointerHbScrollAreaPrivate, parent )
    {
    
    }


// -----------------------------------------------------------------------------
// HbAbstractItemView::~HbAbstractItemView
// -----------------------------------------------------------------------------
//
HbAbstractItemView::~HbAbstractItemView(  )
    {
    
    }


// -----------------------------------------------------------------------------
// HbAbstractItemView::type
// -----------------------------------------------------------------------------
//
/*
int HbAbstractItemView::type(  ) const
    {
    SMC_MOCK_METHOD0( int )
    }
*/

// -----------------------------------------------------------------------------
// HbAbstractItemView::setModel
// -----------------------------------------------------------------------------
//
void HbAbstractItemView::setModel( 
        QAbstractItemModel * model,
        HbAbstractViewItem * prototype )
    {

    }


// -----------------------------------------------------------------------------
// HbAbstractItemView::model
// -----------------------------------------------------------------------------
//
QAbstractItemModel * HbAbstractItemView::model(  ) const
    {
  
    }


// -----------------------------------------------------------------------------
// HbAbstractItemView::setItemPrototype
// -----------------------------------------------------------------------------
//
void HbAbstractItemView::setItemPrototype( 
        HbAbstractViewItem * prototype )
    {

    }


// -----------------------------------------------------------------------------
// HbAbstractItemView::setItemPrototypes
// -----------------------------------------------------------------------------
//
void HbAbstractItemView::setItemPrototypes( 
        const QList<HbAbstractViewItem *> & prototypes )
    {
  
    }


// -----------------------------------------------------------------------------
// HbAbstractItemView::itemPrototypes
// -----------------------------------------------------------------------------
//
QList <HbAbstractViewItem * > HbAbstractItemView::itemPrototypes(  ) const
    {
    
    }


// -----------------------------------------------------------------------------
// HbAbstractItemView::setSelectionModel
// -----------------------------------------------------------------------------
//
void HbAbstractItemView::setSelectionModel( 
        QItemSelectionModel * selectionModel )
    {
  
    }


// -----------------------------------------------------------------------------
// HbAbstractItemView::selectionModel
// -----------------------------------------------------------------------------
//
QItemSelectionModel * HbAbstractItemView::selectionModel(  ) const
    {
  
    }


// -----------------------------------------------------------------------------
// HbAbstractItemView::currentIndex
// -----------------------------------------------------------------------------
//
QModelIndex HbAbstractItemView::currentIndex(  ) const
    {
   
    }


// -----------------------------------------------------------------------------
// HbAbstractItemView::rootIndex
// -----------------------------------------------------------------------------
//
QModelIndex HbAbstractItemView::rootIndex(  ) const
    {
    
    }


// -----------------------------------------------------------------------------
// HbAbstractItemView::setSelectionMode
// -----------------------------------------------------------------------------
//
void HbAbstractItemView::setSelectionMode( 
        SelectionMode newMode,
        bool resetSelection )
    {

    }


// -----------------------------------------------------------------------------
// HbAbstractItemView::selectionMode
// -----------------------------------------------------------------------------
//
HbAbstractItemView::SelectionMode HbAbstractItemView::selectionMode(  ) const
    {

    }


// -----------------------------------------------------------------------------
// HbAbstractItemView::setItemRecycling
// -----------------------------------------------------------------------------
//
void HbAbstractItemView::setItemRecycling( 
        bool enabled )
    {
    SMC_MOCK_METHOD1( void, bool, enabled )
    }


// -----------------------------------------------------------------------------
// HbAbstractItemView::itemRecycling
// -----------------------------------------------------------------------------
//
bool HbAbstractItemView::itemRecycling(  ) const
    {
    SMC_MOCK_METHOD0( bool )
    }


// -----------------------------------------------------------------------------
// HbAbstractItemView::isVisible
// -----------------------------------------------------------------------------
//
bool HbAbstractItemView::isVisible( 
        const QModelIndex & index ) const
    {

    }


// -----------------------------------------------------------------------------
// HbAbstractItemView::isVisible
// -----------------------------------------------------------------------------
//
bool HbAbstractItemView::isVisible( 
        HbAbstractViewItem * item ) const
    {

    }


// -----------------------------------------------------------------------------
// HbAbstractItemView::currentViewItem
// -----------------------------------------------------------------------------
//
HbAbstractViewItem * HbAbstractItemView::currentViewItem(  ) const
    {
    SMC_MOCK_METHOD0( HbAbstractViewItem * )
    }


// -----------------------------------------------------------------------------
// HbAbstractItemView::itemByIndex
// -----------------------------------------------------------------------------
//
HbAbstractViewItem * HbAbstractItemView::itemByIndex( 
        const QModelIndex & index ) const
    {

    }


// -----------------------------------------------------------------------------
// HbAbstractItemView::scrollTo
// -----------------------------------------------------------------------------
//
void HbAbstractItemView::scrollTo( 
        const QModelIndex & index,
        ScrollHint hint )
    {

    }


// -----------------------------------------------------------------------------
// HbAbstractItemView::visibleItems
// -----------------------------------------------------------------------------
//
QList <HbAbstractViewItem * > HbAbstractItemView::visibleItems(  ) const
    {
 
    }


// -----------------------------------------------------------------------------
// HbAbstractItemView::itemAtPosition
// -----------------------------------------------------------------------------
//
HbAbstractViewItem * HbAbstractItemView::itemAtPosition( 
        const QPointF & position ) const
    {
    
    }


// -----------------------------------------------------------------------------
// HbAbstractItemView::indexCount
// -----------------------------------------------------------------------------
//
int HbAbstractItemView::indexCount(  ) const
    {
    SMC_MOCK_METHOD0( int )
    }


// -----------------------------------------------------------------------------
// HbAbstractItemView::indexPosition
// -----------------------------------------------------------------------------
//
int HbAbstractItemView::indexPosition( 
        const QModelIndex & index ) const
    {
   
    }


// -----------------------------------------------------------------------------
// HbAbstractItemView::nextIndex
// -----------------------------------------------------------------------------
//
QModelIndex HbAbstractItemView::nextIndex( 
        const QModelIndex & index ) const
    {

    }


// -----------------------------------------------------------------------------
// HbAbstractItemView::previousIndex
// -----------------------------------------------------------------------------
//
QModelIndex HbAbstractItemView::previousIndex( 
        const QModelIndex & index ) const
    {

    }


// -----------------------------------------------------------------------------
// HbAbstractItemView::layoutName
// -----------------------------------------------------------------------------
//
QString HbAbstractItemView::layoutName(  ) const
    {
 
    }


// -----------------------------------------------------------------------------
// HbAbstractItemView::setLayoutName
// -----------------------------------------------------------------------------
//
void HbAbstractItemView::setLayoutName( 
        const QString & layoutName )
    {
   
    }


// -----------------------------------------------------------------------------
// HbAbstractItemView::setUniformItemSizes
// -----------------------------------------------------------------------------
//
void HbAbstractItemView::setUniformItemSizes( 
        bool enable )
    {
    SMC_MOCK_METHOD1( void, bool, enable )
    }


// -----------------------------------------------------------------------------
// HbAbstractItemView::uniformItemSizes
// -----------------------------------------------------------------------------
//
bool HbAbstractItemView::uniformItemSizes(  ) const
    {
    SMC_MOCK_METHOD0( bool )
    }


// -----------------------------------------------------------------------------
// HbAbstractItemView::setCurrentIndex
// -----------------------------------------------------------------------------
//
void HbAbstractItemView::setCurrentIndex( 
        const QModelIndex & index,
        QItemSelectionModel::SelectionFlags selectionFlag )
    {
    
    }


// -----------------------------------------------------------------------------
// HbAbstractItemView::setRootIndex
// -----------------------------------------------------------------------------
//
void HbAbstractItemView::setRootIndex( 
        const QModelIndex & index )
    {
   
    }


// -----------------------------------------------------------------------------
// HbAbstractItemView::reset
// -----------------------------------------------------------------------------
//
void HbAbstractItemView::reset(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// HbAbstractItemView::selectAll
// -----------------------------------------------------------------------------
//
void HbAbstractItemView::selectAll(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// HbAbstractItemView::clearSelection
// -----------------------------------------------------------------------------
//
void HbAbstractItemView::clearSelection(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// HbAbstractItemView::pressed
// -----------------------------------------------------------------------------
//
void HbAbstractItemView::pressed( 
        const QModelIndex & index )
    {
    
    }


// -----------------------------------------------------------------------------
// HbAbstractItemView::released
// -----------------------------------------------------------------------------
//
void HbAbstractItemView::released( 
        const QModelIndex & index )
    {
    
    }


// -----------------------------------------------------------------------------
// HbAbstractItemView::activated
// -----------------------------------------------------------------------------
//
void HbAbstractItemView::activated( 
        const QModelIndex & index )
    {
    
    }


// -----------------------------------------------------------------------------
// HbAbstractItemView::longPressed
// -----------------------------------------------------------------------------
//
void HbAbstractItemView::longPressed( 
        HbAbstractViewItem * item,
        const QPointF & coords )
    {
    
    }


// -----------------------------------------------------------------------------
// HbAbstractItemView::selectionCommand
// -----------------------------------------------------------------------------
//
QItemSelectionModel::SelectionFlags HbAbstractItemView::selectionCommand( 
        const HbAbstractViewItem *item, 
        const QEvent *event )
    {
    
    }


// -----------------------------------------------------------------------------
// HbAbstractItemView::event
// -----------------------------------------------------------------------------
//
bool HbAbstractItemView::event( 
        QEvent * e )
    {
    
    }

// -----------------------------------------------------------------------------
// HbAbstractItemView::mousePressEvent
// -----------------------------------------------------------------------------
//
void HbAbstractItemView::mousePressEvent( 
        QGraphicsSceneMouseEvent * event )
    {
    
    }


// -----------------------------------------------------------------------------
// HbAbstractItemView::mouseReleaseEvent
// -----------------------------------------------------------------------------
//
void HbAbstractItemView::mouseReleaseEvent( 
        QGraphicsSceneMouseEvent * event )
    {
    
    }


// -----------------------------------------------------------------------------
// HbAbstractItemView::mouseMoveEvent
// -----------------------------------------------------------------------------
//
void HbAbstractItemView::mouseMoveEvent( 
        QGraphicsSceneMouseEvent * event )
    {
    
    }


// -----------------------------------------------------------------------------
// HbAbstractItemView::focusOutEvent
// -----------------------------------------------------------------------------
//
void HbAbstractItemView::focusOutEvent( 
        QFocusEvent * event )
    {
    
    }


// -----------------------------------------------------------------------------
// HbAbstractItemView::upGesture
// -----------------------------------------------------------------------------
//
void HbAbstractItemView::upGesture( 
        int value )
    {
    SMC_MOCK_METHOD1( void, int, value )
    }


// -----------------------------------------------------------------------------
// HbAbstractItemView::downGesture
// -----------------------------------------------------------------------------
//
void HbAbstractItemView::downGesture( 
        int value )
    {
    SMC_MOCK_METHOD1( void, int, value )
    }


// -----------------------------------------------------------------------------
// HbAbstractItemView::leftGesture
// -----------------------------------------------------------------------------
//
void HbAbstractItemView::leftGesture( 
        int value )
    {
    SMC_MOCK_METHOD1( void, int, value )
    }


// -----------------------------------------------------------------------------
// HbAbstractItemView::rightGesture
// -----------------------------------------------------------------------------
//
void HbAbstractItemView::rightGesture( 
        int value )
    {
    SMC_MOCK_METHOD1( void, int, value )
    }


// -----------------------------------------------------------------------------
// HbAbstractItemView::emitActivated
// -----------------------------------------------------------------------------
//
void HbAbstractItemView::emitActivated( 
        const QModelIndex & modelIndex )
    {
   
    }


// -----------------------------------------------------------------------------
// HbAbstractItemView::emitPressed
// -----------------------------------------------------------------------------
//
void HbAbstractItemView::emitPressed( 
        const QModelIndex & modelIndex )
    {
   
    }


// -----------------------------------------------------------------------------
// HbAbstractItemView::emitReleased
// -----------------------------------------------------------------------------
//
void HbAbstractItemView::emitReleased( 
        const QModelIndex & modelIndex )
    {
    
    }


// -----------------------------------------------------------------------------
// HbAbstractItemView::scrollByAmount
// -----------------------------------------------------------------------------
//
bool HbAbstractItemView::scrollByAmount( 
        const QPointF & delta )
    {
   
    }


// -----------------------------------------------------------------------------
// HbAbstractItemView::container
// -----------------------------------------------------------------------------
//
HbAbstractItemContainer * HbAbstractItemView::container(  ) const
    {
    
    }


// -----------------------------------------------------------------------------
// HbAbstractItemView::itemChange
// -----------------------------------------------------------------------------
//
QVariant HbAbstractItemView::itemChange( 
        GraphicsItemChange change,
        const QVariant & value )
    {
    
    }


// -----------------------------------------------------------------------------
// HbAbstractItemView::dataChanged
// -----------------------------------------------------------------------------
//
void HbAbstractItemView::dataChanged( 
        const QModelIndex & topLeft,
        const QModelIndex & bottomRight )
    {
    
    }


// -----------------------------------------------------------------------------
// HbAbstractItemView::orientationChanged
// -----------------------------------------------------------------------------
//
void HbAbstractItemView::orientationChanged( 
        Qt::Orientation newOrientation )
    {
    
    }


// -----------------------------------------------------------------------------
// HbAbstractItemView::orientationAboutToBeChanged
// -----------------------------------------------------------------------------
//
void HbAbstractItemView::orientationAboutToBeChanged(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// HbAbstractItemView::currentIndexChanged
// -----------------------------------------------------------------------------
//
void HbAbstractItemView::currentIndexChanged( 
        const QModelIndex & current,
        const QModelIndex & previous )
    {
    
    }


// -----------------------------------------------------------------------------
// HbAbstractItemView::currentSelectionChanged
// -----------------------------------------------------------------------------
//
void HbAbstractItemView::currentSelectionChanged( 
        const QItemSelection & selected,
        const QItemSelection & deselected )
    {
    
    }


// -----------------------------------------------------------------------------
// HbAbstractItemView::rowsAboutToBeInserted
// -----------------------------------------------------------------------------
//
void HbAbstractItemView::rowsAboutToBeInserted( 
        const QModelIndex & index,
        int start,
        int end )
    {
    
    }


// -----------------------------------------------------------------------------
// HbAbstractItemView::rowsInserted
// -----------------------------------------------------------------------------
//
void HbAbstractItemView::rowsInserted( 
        const QModelIndex & parent,
        int start,
        int end )
    {
    
    }


// -----------------------------------------------------------------------------
// HbAbstractItemView::rowsAboutToBeRemoved
// -----------------------------------------------------------------------------
//
void HbAbstractItemView::rowsAboutToBeRemoved( 
        const QModelIndex & index,
        int start,
        int end )
    {
    
    }


// -----------------------------------------------------------------------------
// HbAbstractItemView::rowsRemoved
// -----------------------------------------------------------------------------
//
void HbAbstractItemView::rowsRemoved( 
        const QModelIndex & parent,
        int start,
        int end )
    {
    
    }


// -----------------------------------------------------------------------------
// HbAbstractItemView::columnsAboutToBeInserted
// -----------------------------------------------------------------------------
//
void HbAbstractItemView::columnsAboutToBeInserted( 
        const QModelIndex & index,
        int start,
        int end )
    {
    
    }


// -----------------------------------------------------------------------------
// HbAbstractItemView::columnsInserted
// -----------------------------------------------------------------------------
//
void HbAbstractItemView::columnsInserted( 
        const QModelIndex & parent,
        int start,
        int end )
    {
    
    }


// -----------------------------------------------------------------------------
// HbAbstractItemView::columnsAboutToBeRemoved
// -----------------------------------------------------------------------------
//
void HbAbstractItemView::columnsAboutToBeRemoved( 
        const QModelIndex & index,
        int start,
        int end )
    {
    
    }


// -----------------------------------------------------------------------------
// HbAbstractItemView::columnsRemoved
// -----------------------------------------------------------------------------
//
void HbAbstractItemView::columnsRemoved( 
        const QModelIndex & parent,
        int start,
        int end )
    {
    
    }


// -----------------------------------------------------------------------------
// HbAbstractItemView::panGesture
// -----------------------------------------------------------------------------
//
void HbAbstractItemView::panGesture( 
        const QPointF & point )
    {
    
    }


// -----------------------------------------------------------------------------
// HbAbstractItemView::longPressGesture
// -----------------------------------------------------------------------------
//
void HbAbstractItemView::longPressGesture( 
        const QPointF & point )
    {
    
    }


