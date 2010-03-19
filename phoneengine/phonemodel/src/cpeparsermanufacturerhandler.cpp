/*
* Copyright (c) 2006-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Battery state monitoring class methods implementation
*
*/
 

//  ==================== INCLUDE FILES  ====================
#include "cpeparsermanufacturerhandler.h"
#include "mpephonemodelinternal.h"
#include <mpecallhandling.h>
#include <mpedatastore.h>
#include <sysutil.h>
#include <talogger.h>


// CONSTANTS
const TInt KPESwVersionLineCount = 3;
const TInt KPEVersionLineFeed = '\n';
const TInt KPESalesModelNameMaxLength = 20; 
_LIT( KPESalesModelFileName, "Z:\\resource\\versions\\model.txt");

// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//
CPEParserManufacturerHandler::CPEParserManufacturerHandler( 
        MPEPhoneModelInternal& aModel,
        MPECallHandling& aCallHandling,
        RFs& aFsSession )
        : iModel( aModel ),
            iCallHandling( aCallHandling ),
            iFsSession( aFsSession )
    {
    TEFLOGSTRING( KTAOBJECT, "PE CPEParserManufacturerHandler::CPEParserManufacturerHandler()" );
    }

// Destructor
CPEParserManufacturerHandler::~CPEParserManufacturerHandler()
    {    
    TEFLOGSTRING( KTAOBJECT, "PE CPEParserManufacturerHandler::~CPEParserManufacturerHandler()" );
    }

// -----------------------------------------------------------------------------
// CPhoneManufacturerHandler::ProcessCommandL
// -----------------------------------------------------------------------------
//
void CPEParserManufacturerHandler::ProcessCommandL( 
        TUint aCommand )       // aCommand command enumeration.
    {
    TEFLOGSTRING( KTAINT, "PE CPEParserManufacturerHandler::ProcessCommandL" );

    switch ( aCommand )
        {
        case EShowVersion: // *#0000#
            {
            TEFLOGSTRING( KTAINT, "PE CPEParserManufacturerHandler::ProcessCommandL, EShowVersion" );
            
            TPEPhoneIdentityParameters phoneIdentity;
            phoneIdentity = iModel.DataStore()->PhoneIdentityParameters();
            if ( GetSwVersionAndPhoneModelL( phoneIdentity.iRevision ) == KErrNone )
                {
                iModel.DataStore()->SetPhoneIdentityParameters( phoneIdentity );
                iModel.SendMessage( MEngineMonitor::EPEMessageShowVersion );
                }
            else
                {
                iModel.SendMessage( MEngineMonitor::EPEMessageError );
                TEFLOGSTRING( KTAERROR, 
                    "PE CPEGSMPARSERMANUFACTURERHANDLER::PROCESSCOMMANDL: FAILED TO RETRIEVE SW VERSION" );
                }
            break;
            }
        case EBadPinChange:    
            {
            iModel.SendMessage( MEngineMonitor::EPEMessageBadPin1Change );
            break;
            }
        case EBadPin2Change:
            {
            iModel.SendMessage( MEngineMonitor::EPEMessageBadPin2Change );
            break;
            }
        case EBadPinUnblock:    
            {
            iModel.SendMessage( MEngineMonitor::EPEMessageBadPin1Unblock );
            break;
            }
        case EBadPin2Unblock:
            {
            iModel.SendMessage( MEngineMonitor::EPEMessageBadPin2Unblock );
            break;
            }
        case EActivateRfsNormal:
            {
            iModel.SendMessage( MEngineMonitor::EPEMessageActivateRfsNormal );
            break;
            }
        case EActivateRfsDeep:    
            {
            iModel.SendMessage( MEngineMonitor::EPEMessageActivateRfsDeep );
            break;
            }
        case EActivateWarranty:
            //lint -fallthrough
        case ELifeTimer:
            {
            iModel.SendMessage( MEngineMonitor::EPEMessageActivateWarrantyMode );
            break;
            }
        case EShowBtAddress: // *#2820#
            {
            iModel.SendMessage( MEngineMonitor::EPEMessageShowBTDeviceAddress );
            break;
            }
        case EBTLoopback: // *#9990#
            {
            iModel.SendMessage( MEngineMonitor::EPEMessageShowBTLoopback );
            break;
            }
        case EShowWlanMac: // *#62209526#
            {
            iModel.SendMessage( MEngineMonitor::EPEMessageShowWlanMacAddress );
            break;
            }
        case EBTDebugMode: // *#2873#
            {
            iModel.SendMessage( MEngineMonitor::EPEMessageBTDebugMode );
            break;
            }
        default:
            {
            TEFLOGSTRING( KTAERROR, "PE CPEGSMPARSERMANUFACTURERHANDLER::PROCESSCOMMANDL: UNEXPECTED COMMAND" );
            break;
            }
        }
    }

// -----------------------------------------------------------------------------
// CPEParserManufacturerHandler::ProcessDebugL
// -----------------------------------------------------------------------------
//
void CPEParserManufacturerHandler::ProcessDebugL(
        const TDesC& aCode
        )
    {
    TEFLOGSTRING( KTAINT, "PE CPEParserManufacturerHandler::ProcessDebugL" );
    
    if ( aCode.Length() )
        {
        TInt cmd = KErrNotFound;
        TLex( aCode.Left( 1 ) ).Val( cmd ); // first character.

        TInt param = KErrNotFound;
        TLex( aCode.Mid( 1 ) ).Val( param ); // rest

        switch ( cmd )
            {
            case EPhoneCmdDebugPrint:
                {
                //Copied from S60 Telephony. To be migrated to CoTe
                TEFLOGSTRING( KTAERROR, "PE CPEGSMPARSERMANUFACTURERHANDLER::PROCESSDEBUGL EPHONECMDDEBUGPRINT" );
                }
                break;

            case EPhoneCmdHeapFailure:
                {
#ifdef PHONE_HEAP_FAILURE_SUPPORT
                if ( param <= 0 )
                    {
                    // Reset alloc failure
                    User::__DbgSetAllocFail(
                        RHeap::EUser,
                        RHeap::ENone,
                        1 );
                    }
                else
                    {
                    // Set heap failure
                    User::__DbgSetAllocFail(
                        RHeap::EUser,
                        RHeap::ERandom,
                        param );
                    }
#endif // PHONE_HEAP_FAILURE_SUPPORT
                } 
                break;

            default:
                break;
            }
        }
    }

// -----------------------------------------------------------------------------
// CPEParserManufacturerHandler::GetSwVersionAndPhoneModelL
// Gets sw version from SysUtil::GetSwVersion and loads phone model information
// from file. This information is set to aSwVersion.
// -----------------------------------------------------------------------------
//
TInt CPEParserManufacturerHandler::GetSwVersionAndPhoneModelL( 
    TDes& aSwVersion )
    {
    TEFLOGSTRING( KTAINT, "PE CPEParserManufacturerHandler::GetSwVersionAndPhoneModelL" );
    
    TInt errorCode = SysUtil::GetSWVersion( aSwVersion ) ;
    
    TEFLOGSTRING2( KTAINT,
        "PE CPEParserManufacturerHandler::GetSwVersionAndPhoneModelL > SysUtil::GetSWVersion, error code: %d",
        errorCode );

    if ( errorCode == KErrNone )
        {
        // Remove needless lines
        RemoveLinesAfterThreeLines( aSwVersion );
        // Add phone model
        AppendPhoneModelL( aSwVersion );
        }
        
    return errorCode;  
    }

// -----------------------------------------------------------------------------
// CPEParserManufacturerHandler::RemoveNeedlessLinesL
// Removes lines after 3 end-of-line (\n) chars.
// -----------------------------------------------------------------------------
//
void CPEParserManufacturerHandler::RemoveLinesAfterThreeLines( 
    TDes& aSwVersion ) 
    {
    TEFLOGSTRING( KTAINT, "PE CPEParserManufacturerHandler::RemoveLinesAfterThreeLines" );
    
    TInt lineFeedCount = 0;
    
    const TInt swVersionLength( aSwVersion.Length() );
    for( TInt i = 0; i < swVersionLength ; i++ )
        {
        if( aSwVersion[i] == KPEVersionLineFeed )
            {
            lineFeedCount++;
            if( lineFeedCount == KPESwVersionLineCount )
                {
                const TInt charsToDelete( aSwVersion.Length() - i );
                
                // Dont delete the last line feed characted, thats why
                // i + 1.
                aSwVersion.Delete( i + 1, charsToDelete );
                return;
                }
            }
        }
    }
    
// -----------------------------------------------------------------------------
// CPEParserManufacturerHandler::AppendPhoneModelL
// Reads phone model info from disk and appends it to aSwVersion.
// -----------------------------------------------------------------------------
//
void CPEParserManufacturerHandler::AppendPhoneModelL(   
    TDes& aSwVersion ) 
    {   
    RFile file;
    User::LeaveIfError( file.Open( iFsSession, 
                                   KPESalesModelFileName, 
                                   EFileShareReadersOnly | EFileRead ) );
    CleanupClosePushL( file );
    
    HBufC* model = HBufC::NewLC( KPESalesModelNameMaxLength );
    TPtr ptr = model->Des();

    // Read the data from file.
    TFileText reader;
    reader.Set( file );
    User::LeaveIfError( reader.Read( ptr ) );

    // Append the phone model to aSwVersion
    aSwVersion.Append( *model );
    
    CleanupStack::PopAndDestroy( 2 ); // model, file
    }

// End of File
