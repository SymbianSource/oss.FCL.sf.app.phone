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
#include <xqservicerequest.h>

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// XQServiceRequest::XQServiceRequest
// -----------------------------------------------------------------------------
//
XQServiceRequest::XQServiceRequest(  )
    //:
    //QObject( /**/ )
    {
    
    }


// -----------------------------------------------------------------------------
// XQServiceRequest::XQServiceRequest
// -----------------------------------------------------------------------------
//
XQServiceRequest::XQServiceRequest( 
        const QString & fullServiceName,
        const QString & message,
        const bool & synchronous )
    //:
    //QObject( /*fullServiceName, message, synchronous*/ )
    {
    
    }


// -----------------------------------------------------------------------------
// XQServiceRequest::XQServiceRequest
// -----------------------------------------------------------------------------
//
XQServiceRequest::XQServiceRequest( 
        const XQServiceRequest & orig )
    //:
    //QObject( /*orig*/ )
    {
    
    }


// -----------------------------------------------------------------------------
// XQServiceRequest::XQServiceRequest
// -----------------------------------------------------------------------------
//
XQServiceRequest::XQServiceRequest( 
        const XQAiwInterfaceDescriptor & descriptor,
        const QString & message,
        const bool & synchronous )
    //:
    //QObject( /*descriptor, message, synchronous*/ )
    {
    
    }


// -----------------------------------------------------------------------------
// XQServiceRequest::~XQServiceRequest
// -----------------------------------------------------------------------------
//
XQServiceRequest::~XQServiceRequest(  )
    {
    
    }


// -----------------------------------------------------------------------------
// XQServiceRequest::send
// -----------------------------------------------------------------------------
//
bool XQServiceRequest::send(  )
    {
    SMC_MOCK_METHOD0( bool )
    }


// -----------------------------------------------------------------------------
// XQServiceRequest::send
// -----------------------------------------------------------------------------
//
bool XQServiceRequest::send( 
        QVariant & retValue )
    {
    SMC_MOCK_METHOD1( bool, QVariant &, retValue )
    }


// -----------------------------------------------------------------------------
// XQServiceRequest::isNull
// -----------------------------------------------------------------------------
//
bool XQServiceRequest::isNull(  ) const
    {
    SMC_MOCK_METHOD0( bool )
    }


// -----------------------------------------------------------------------------
// XQServiceRequest::setSynchronous
// -----------------------------------------------------------------------------
//
void XQServiceRequest::setSynchronous( 
        const bool & synchronous )
    {
    SMC_MOCK_METHOD1( void, const bool &, synchronous )
    }


// -----------------------------------------------------------------------------
// XQServiceRequest::isSynchronous
// -----------------------------------------------------------------------------
//
bool XQServiceRequest::isSynchronous(  ) const
    {
    SMC_MOCK_METHOD0( bool )
    }


// -----------------------------------------------------------------------------
// XQServiceRequest::setService
// -----------------------------------------------------------------------------
//
void XQServiceRequest::setService( 
        const QString & fullServiceName )
    {
    SMC_MOCK_METHOD1( void, const QString &, fullServiceName )
    }


// -----------------------------------------------------------------------------
// XQServiceRequest::service
// -----------------------------------------------------------------------------
//
QString XQServiceRequest::service(  ) const
    {
    SMC_MOCK_METHOD0( QString )
    }


// -----------------------------------------------------------------------------
// XQServiceRequest::setMessage
// -----------------------------------------------------------------------------
//
void XQServiceRequest::setMessage( 
        const QString & message )
    {
    SMC_MOCK_METHOD1( void, const QString &, message )
    }


// -----------------------------------------------------------------------------
// XQServiceRequest::message
// -----------------------------------------------------------------------------
//
QString XQServiceRequest::message(  ) const
    {
    SMC_MOCK_METHOD0( QString )
    }


// -----------------------------------------------------------------------------
// XQServiceRequest::arguments
// -----------------------------------------------------------------------------
//
const QList <QVariant > & XQServiceRequest::arguments(  ) const
    {
    SMC_MOCK_METHOD0( const QList <QVariant > & )
    }


// -----------------------------------------------------------------------------
// XQServiceRequest::setArguments
// -----------------------------------------------------------------------------
//
void XQServiceRequest::setArguments( 
        const QList<QVariant> & arguments )
    {
    SMC_MOCK_METHOD1( void, const QList<QVariant> &, arguments )
    }


// -----------------------------------------------------------------------------
// XQServiceRequest::latestError
// -----------------------------------------------------------------------------
//
int XQServiceRequest::latestError(  )
    {
    SMC_MOCK_METHOD0( int )
    }


// -----------------------------------------------------------------------------
// XQServiceRequest::setInfo
// -----------------------------------------------------------------------------
//
void XQServiceRequest::setInfo( 
        const XQRequestInfo & reqInfo )
    {
    SMC_MOCK_METHOD1( void, const XQRequestInfo &, reqInfo )
    }


// -----------------------------------------------------------------------------
// XQServiceRequest::info
// -----------------------------------------------------------------------------
//
XQRequestInfo XQServiceRequest::info(  ) const
    {
    SMC_MOCK_METHOD0( XQRequestInfo )
    }


// -----------------------------------------------------------------------------
// XQServiceRequest::=
// -----------------------------------------------------------------------------
//
XQServiceRequest & XQServiceRequest::operator=( 
        const XQServiceRequest & orig )
    {
    SMC_MOCK_METHOD1( XQServiceRequest &, const XQServiceRequest &, orig )
    }


// -----------------------------------------------------------------------------
// XQServiceRequest::serializeArguments
// -----------------------------------------------------------------------------
//
QByteArray XQServiceRequest::serializeArguments( 
        const XQServiceRequest & actione )
    {
    SMC_MOCK_METHOD1( QByteArray, const XQServiceRequest &, actione )
    }


// -----------------------------------------------------------------------------
// XQServiceRequest::deserializeArguments
// -----------------------------------------------------------------------------
//
void XQServiceRequest::deserializeArguments( 
        XQServiceRequest & actione,
        const QByteArray & data )
    {
    SMC_MOCK_METHOD2( void, XQServiceRequest &, actione, 
        const QByteArray &, data )
    }


// -----------------------------------------------------------------------------
// XQServiceRequest::serialize
// -----------------------------------------------------------------------------
//
template <typename Stream > void XQServiceRequest::serialize( 
        Stream & stream ) const
    {
    SMC_MOCK_METHOD1( template <typename Stream > void, Stream &, stream )
    }


// -----------------------------------------------------------------------------
// XQServiceRequest::deserialize
// -----------------------------------------------------------------------------
//
template <typename Stream > void XQServiceRequest::deserialize( 
        Stream & stream )
    {
    SMC_MOCK_METHOD1( template <typename Stream > void, Stream &, stream )
    }


// -----------------------------------------------------------------------------
// XQServiceRequest::requestCompleted
// -----------------------------------------------------------------------------
//
/*void XQServiceRequest::requestCompleted( 
        const QVariant & value )
    {
    SMC_MOCK_METHOD1( void, const QVariant &, value )
    }*/


// -----------------------------------------------------------------------------
// XQServiceRequest::requestError
// -----------------------------------------------------------------------------
//
/*void XQServiceRequest::requestError( 
        int err )
    {
    SMC_MOCK_METHOD1( void, int, err )
    }*/

// -----------------------------------------------------------------------------
// XQServiceRequest::addArg
// -----------------------------------------------------------------------------
//
void XQServiceRequest::addArg(
        const QVariant& v)
    {
    SMC_MOCK_METHOD1( void, const QVariant&, v )
    }

// -----------------------------------------------------------------------------
// XQServiceRequest::handleSharableFileArgs
// -----------------------------------------------------------------------------
//
bool XQServiceRequest::handleSharableFileArgs()
    {
    SMC_MOCK_METHOD0( bool );
    }


