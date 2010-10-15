/** Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of the License "Symbian Foundation License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.symbianfoundation.org/legal/sfl-v10.html".
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
#include <hbdataformmodel.h>

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// HbDataFormModel::HbDataFormModel
// -----------------------------------------------------------------------------
//
HbDataFormModel::HbDataFormModel( 
        QObject * parent )
    //:
    //QAbstractItemModel( /*parent*/ )
	:d_ptr(NULL)
    {
    
    }


// -----------------------------------------------------------------------------
// HbDataFormModel::~HbDataFormModel
// -----------------------------------------------------------------------------
//
HbDataFormModel::~HbDataFormModel(  )
    {
    
    }


// -----------------------------------------------------------------------------
// HbDataFormModel::appendDataFormPage
// -----------------------------------------------------------------------------
//
HbDataFormModelItem * HbDataFormModel::appendDataFormPage( 
        const QString & label )
    {
    SMC_MOCK_METHOD1( HbDataFormModelItem *, const QString &, label )
    }


// -----------------------------------------------------------------------------
// HbDataFormModel::appendDataFormGroup
// -----------------------------------------------------------------------------
//
HbDataFormModelItem * HbDataFormModel::appendDataFormGroup( 
        const QString & label,
        HbDataFormModelItem * parent )
    {
    SMC_MOCK_METHOD2( HbDataFormModelItem *, const QString &, label, 
        HbDataFormModelItem *, parent )
    }


// -----------------------------------------------------------------------------
// HbDataFormModel::appendDataFormGroupPage
// -----------------------------------------------------------------------------
//
HbDataFormModelItem * HbDataFormModel::appendDataFormGroupPage( 
        const QString & label,
        HbDataFormModelItem * parent )
    {
    SMC_MOCK_METHOD2( HbDataFormModelItem *, const QString &, label, 
        HbDataFormModelItem *, parent )
    }


// -----------------------------------------------------------------------------
// HbDataFormModel::appendDataFormItem
// -----------------------------------------------------------------------------
//
HbDataFormModelItem * HbDataFormModel::appendDataFormItem( 
        HbDataFormModelItem::DataItemType itemType,
        const QString & label,
        HbDataFormModelItem * parent )
    {
    SMC_MOCK_METHOD3( HbDataFormModelItem *, HbDataFormModelItem::DataItemType, itemType, 
        const QString &, label, 
        HbDataFormModelItem *, parent )
    }


// -----------------------------------------------------------------------------
// HbDataFormModel::appendDataFormItem
// -----------------------------------------------------------------------------
//
void HbDataFormModel::appendDataFormItem( 
        HbDataFormModelItem * data,
        HbDataFormModelItem * parent )
    {
    SMC_MOCK_METHOD2( void, HbDataFormModelItem *, data, 
        HbDataFormModelItem *, parent )
    }


// -----------------------------------------------------------------------------
// HbDataFormModel::insertDataFormPage
// -----------------------------------------------------------------------------
//
HbDataFormModelItem * HbDataFormModel::insertDataFormPage( 
        int index,
        const QString & label )
    {
    SMC_MOCK_METHOD2( HbDataFormModelItem *, int, index, 
        const QString &, label )
    }


// -----------------------------------------------------------------------------
// HbDataFormModel::insertDataFormGroup
// -----------------------------------------------------------------------------
//
HbDataFormModelItem * HbDataFormModel::insertDataFormGroup( 
        int index,
        const QString & label,
        HbDataFormModelItem * parent )
    {
    SMC_MOCK_METHOD3( HbDataFormModelItem *, int, index, 
        const QString &, label, 
        HbDataFormModelItem *, parent )
    }


// -----------------------------------------------------------------------------
// HbDataFormModel::insertDataFormGroupPage
// -----------------------------------------------------------------------------
//
HbDataFormModelItem * HbDataFormModel::insertDataFormGroupPage( 
        int index,
        const QString & label,
        HbDataFormModelItem * parent )
    {
    SMC_MOCK_METHOD3( HbDataFormModelItem *, int, index, 
        const QString &, label, 
        HbDataFormModelItem *, parent )
    }


// -----------------------------------------------------------------------------
// HbDataFormModel::insertDataFormItem
// -----------------------------------------------------------------------------
//
HbDataFormModelItem * HbDataFormModel::insertDataFormItem( 
        int index,
        HbDataFormModelItem::DataItemType itemType,
        const QString & label,
        HbDataFormModelItem * parent )
    {
    SMC_MOCK_METHOD4( HbDataFormModelItem *, int, index, 
        HbDataFormModelItem::DataItemType, itemType, 
        const QString &, label, 
        HbDataFormModelItem *, parent )
    }


// -----------------------------------------------------------------------------
// HbDataFormModel::insertDataFormItem
// -----------------------------------------------------------------------------
//
void HbDataFormModel::insertDataFormItem( 
        int index,
        HbDataFormModelItem * data,
        HbDataFormModelItem * parent )
    {
    SMC_MOCK_METHOD3( void, int, index, 
        HbDataFormModelItem *, data, 
        HbDataFormModelItem *, parent )
    }


// -----------------------------------------------------------------------------
// HbDataFormModel::removeItem
// -----------------------------------------------------------------------------
//
bool HbDataFormModel::removeItem( 
        const QModelIndex & index )
    {
    SMC_MOCK_METHOD1( bool, const QModelIndex &, index )
    }


// -----------------------------------------------------------------------------
// HbDataFormModel::removeItem
// -----------------------------------------------------------------------------
//
bool HbDataFormModel::removeItem( 
        HbDataFormModelItem * item )
    {
    SMC_MOCK_METHOD1( bool, HbDataFormModelItem *, item )
    }


// -----------------------------------------------------------------------------
// HbDataFormModel::childCount
// -----------------------------------------------------------------------------
//
int HbDataFormModel::childCount( 
        const QModelIndex & parent ) const
    {
    SMC_MOCK_METHOD1( int, const QModelIndex &, parent )
    }


// -----------------------------------------------------------------------------
// HbDataFormModel::itemFromIndex
// -----------------------------------------------------------------------------
//
HbDataFormModelItem * HbDataFormModel::itemFromIndex( 
        const QModelIndex & index ) const
    {
    SMC_MOCK_METHOD1( HbDataFormModelItem *, const QModelIndex &, index )
    }


// -----------------------------------------------------------------------------
// HbDataFormModel::indexFromItem
// -----------------------------------------------------------------------------
//
QModelIndex HbDataFormModel::indexFromItem( 
        const HbDataFormModelItem * item ) const
    {
    SMC_MOCK_METHOD1( QModelIndex, const HbDataFormModelItem *, item )
    }


// -----------------------------------------------------------------------------
// HbDataFormModel::item
// -----------------------------------------------------------------------------
//
HbDataFormModelItem * HbDataFormModel::item( 
        int row,
        const QModelIndex & index ) const
    {
    SMC_MOCK_METHOD2( HbDataFormModelItem *, int, row, 
        const QModelIndex &, index )
    }


// -----------------------------------------------------------------------------
// HbDataFormModel::invisibleRootItem
// -----------------------------------------------------------------------------
//
HbDataFormModelItem * HbDataFormModel::invisibleRootItem(  ) const
    {
    SMC_MOCK_METHOD0( HbDataFormModelItem * )
    }


// -----------------------------------------------------------------------------
// HbDataFormModel::clear
// -----------------------------------------------------------------------------
//
void HbDataFormModel::clear(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// HbDataFormModel::removeRows
// -----------------------------------------------------------------------------
//
bool HbDataFormModel::removeRows( 
        int row,
        int count,
        const QModelIndex & index )
    {
    SMC_MOCK_METHOD3( bool, int, row, 
        int, count, 
        const QModelIndex &, index )
    }


// -----------------------------------------------------------------------------
// HbDataFormModel::parent
// -----------------------------------------------------------------------------
//
QModelIndex HbDataFormModel::parent( 
        const QModelIndex & index ) const
    {
    SMC_MOCK_METHOD1( QModelIndex, const QModelIndex &, index )
    }


// -----------------------------------------------------------------------------
// HbDataFormModel::index
// -----------------------------------------------------------------------------
//
QModelIndex HbDataFormModel::index( 
        int row,
        int column,
        const QModelIndex & index ) const
    {
    SMC_MOCK_METHOD3( QModelIndex, int, row, 
        int, column, 
        const QModelIndex &, index )
    }


// -----------------------------------------------------------------------------
// HbDataFormModel::data
// -----------------------------------------------------------------------------
//
QVariant HbDataFormModel::data( 
        const QModelIndex & index,
        int role ) const
    {
    SMC_MOCK_METHOD2( QVariant, const QModelIndex &, index, 
        int, role )
    }


// -----------------------------------------------------------------------------
// HbDataFormModel::setData
// -----------------------------------------------------------------------------
//
bool HbDataFormModel::setData( 
        const QModelIndex & index,
        const QVariant & value,
        int role )
    {
    SMC_MOCK_METHOD3( bool, const QModelIndex &, index, 
        const QVariant &, value, 
        int, role )
    }


// -----------------------------------------------------------------------------
// HbDataFormModel::hasChildren
// -----------------------------------------------------------------------------
//
bool HbDataFormModel::hasChildren( 
        const QModelIndex & index ) const
    {
    SMC_MOCK_METHOD1( bool, const QModelIndex &, index )
    }


// -----------------------------------------------------------------------------
// HbDataFormModel::rowCount
// -----------------------------------------------------------------------------
//
int HbDataFormModel::rowCount( 
        const QModelIndex & index ) const
    {
    SMC_MOCK_METHOD1( int, const QModelIndex &, index )
    }


// -----------------------------------------------------------------------------
// HbDataFormModel::columnCount
// -----------------------------------------------------------------------------
//
int HbDataFormModel::columnCount( 
        const QModelIndex & index ) const
    {
    SMC_MOCK_METHOD1( int, const QModelIndex &, index )
    }


// -----------------------------------------------------------------------------
// HbDataFormModel::canFetchMore
// -----------------------------------------------------------------------------
//
bool HbDataFormModel::canFetchMore( 
        const QModelIndex & parent ) const
    {
    SMC_MOCK_METHOD1( bool, const QModelIndex &, parent )
    }


// -----------------------------------------------------------------------------
// HbDataFormModel::flags
// -----------------------------------------------------------------------------
//
Qt::ItemFlags HbDataFormModel::flags( 
        const QModelIndex & index ) const
    {
    SMC_MOCK_METHOD1( Qt::ItemFlags, const QModelIndex &, index )
    }


