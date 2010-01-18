/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Voice mailbox handler.
*
*/


// INCLUDE FILES
#include    <eikenv.h>
#include    <e32property.h>
#include    <featmgr.h>
#include    <exterror.h>
#include    <NetworkHandlingDomainPSKeys.h> 
#include    <telvmbxsettingscrkeys.h>
#include    <vmnumber.h>
#include    <voicemailboxdomaincrkeys.h>

#include    "cphonecenrepproxy.h"
#include    "cphonevmbxhandler.h"
#include    "phoneconstants.h"
#include    "phonelogger.h"
#include    "phoneui.pan"
// CONSTANTS
 
const TInt KVideoMbxVariabilityFeatureOn = 2;

#ifdef _DEBUG
const TInt KPhoneSpeedDialMaxLocation = 9;
#endif // _DEBUG

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhoneVmbxHandler::CPhoneVmbxHandler
// -----------------------------------------------------------------------------
//
CPhoneVmbxHandler::CPhoneVmbxHandler()
    {
    iCsVideoTelFeature = FeatureManager::FeatureSupported
                                                  ( KFeatureIdCsVideoTelephony ); 
    }

// -----------------------------------------------------------------------------
// CPhoneVmbxHandler::NewL
// -----------------------------------------------------------------------------
//
CPhoneVmbxHandler* CPhoneVmbxHandler::NewL() 
    {
    CPhoneVmbxHandler* self = 
        new (ELeave) CPhoneVmbxHandler();
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    
    return self;
    }

// -----------------------------------------------------------------------------
// CPhoneVmbxHandler::~CPhoneVmbxHandler
// -----------------------------------------------------------------------------
//
CPhoneVmbxHandler::~CPhoneVmbxHandler()
    {
    if ( iVmbx )
        {
        iVmbx->Close();
        delete iVmbx;
        }
    }
 
 // -----------------------------------------------------------------------------
// CPhoneVmbxHandler::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPhoneVmbxHandler::ConstructL()
    {
    }
 
// -----------------------------------------------------------------------------
// CPhoneVmbxHandler::IsStraightCallToVideoMailBox()
// -----------------------------------------------------------------------------
//
TBool CPhoneVmbxHandler::IsStraightCallToVideoMailBox() const
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneVmbxHandler::IsStraightCallToVideoMailBox()" );
    
    TInt callStraightToVideoMailBox = KErrNone;
    TInt err = CPhoneCenRepProxy::Instance()->GetInt(         
        KCRUidTelVideoMailbox, 
        KTelVideoMbxKey, 
        callStraightToVideoMailBox );
                
    if ( err == KErrNone )
        {
        return ( callStraightToVideoMailBox == KVideoMbxVariabilityFeatureOn );
        }
            
    __PHONELOG1( EBasic, 
                 EPhoneUIView,
                 "CPhoneVmbxHandler::IsStraightCallToVideoMailBox -> err = (%d)", 
                 err );
                         
    return EFalse;
    }
    
// -----------------------------------------------------------------------------
// CPhoneVmbxHandler::IsVideoMailBoxConfigured()
// -----------------------------------------------------------------------------
//
TBool CPhoneVmbxHandler::IsVideoMailBoxConfigured() const
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneVmbxHandler::IsVideoMailBoxConfigured()" );
    
    TInt videoMbxConfigured = KErrNone;
    TInt err = CPhoneCenRepProxy::Instance()->GetInt(         
        KCRUidVideoMailbox, 
        KVideoMbxSupport, 
        videoMbxConfigured );
        
    if ( err == KErrNone )
        {
        return videoMbxConfigured;
        }

    __PHONELOG1( EBasic, 
                 EPhoneUIView,
                 "CPhoneVmbxHandler::IsVideoMailBoxConfigured -> err = (%d)", 
                 err );
        
    return EFalse;
    }    
 
 
// -----------------------------------------------------------------------------
// CPhoneVmbxHandler::IsVideoMbxLocationAndConfigured
// -----------------------------------------------------------------------------
//
TBool CPhoneVmbxHandler::IsVideoMbxLocationAndConfigured( 
        TInt aSpdLocation ) const
    {
    return ( aSpdLocation == KPhoneVideoMbxLocation && 
                    IsVideoMailBoxConfigured() );
    }

// -----------------------------------------------------------------------------    
// CPhoneVmbxHandler::GetStraightSpeedDialNumber
// -----------------------------------------------------------------------------
//
TInt CPhoneVmbxHandler::GetStraightSpeedDialNumber( 
    TInt aSpdLocation, 
    TDes& aNumber )
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneVmbxHandler::GetStraightSpeedDialNumber()" );
    
    __ASSERT_DEBUG( ( aSpdLocation >=KPhoneVoiceMailBoxLocation && 
        aSpdLocation <= KPhoneSpeedDialMaxLocation), 
        Panic( EPhoneViewIndexOutOfBounds ) );
      
    TInt retVal( KErrNone );
    
    TBool straightCallToVideoMbx = IsStraightCallToVideoMailBox();
    
    /******************************************************
    * -Separated Voice / Video 
    * - Key '1' to VoiceMbx and Key '2' to VideoMbx,
    *   if Video mailbox configured.
    ******************************************************/
    if ( IsCsVideoTelFeature() &&
              straightCallToVideoMbx && 
            ( IsVideoMbxLocationAndConfigured( aSpdLocation ) || 
                aSpdLocation == KPhoneVoiceMailBoxLocation ) )
        {
        
        TRAPD( error, CreateVmbxL() );
        if ( !error )
            {
            if ( aSpdLocation == KPhoneVoiceMailBoxLocation )
                {
                // Get to voice mailbox number
                retVal = LaunchMbx( EVmbx, aNumber );
                }
            else  
                {
                // Get to video mailbox number
                retVal = LaunchMbx( EVmbxVideo, aNumber );            
                }
            }
         else
            {
            retVal = error;        
            }   
        }
    __PHONELOG1( EBasic, 
             EPhoneUIView,"CPhoneVmbxHandler::GetStraightSpeedDialNumber -> retval = (%d)", 
             retVal );
                  
    return retVal;
    }

// -----------------------------------------------------------------------------
// CPhoneVmbxHandler::IsCsVideoTelFeature
// -----------------------------------------------------------------------------
//
TBool CPhoneVmbxHandler::IsCsVideoTelFeature()
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneVmbxHandler::IsCsVideoTelFeature()" );
    return iCsVideoTelFeature; 
    }
// -----------------------------------------------------------------------------
// CPhoneVmbxHandler::LaunchMbx
// -----------------------------------------------------------------------------
//
TInt CPhoneVmbxHandler::LaunchMbx( TInt aMailboxType, TDes& aNumber )
    {
    TInt retVal( KErrNone );
    iMailboxType = aMailboxType;
    
    switch ( aMailboxType )
        {
        case EVmbx:
            {
            TRAPD( error, retVal = CSVmbxL( aNumber ) );
            if ( error )
                {
                retVal = error;
                }
            }
            break; 
        case EVmbxIP:
            {
            TRAPD( error,retVal = IPVmbxL( aNumber ) );
            if ( error )
                {
                retVal = error;
                }            
            } 
            break;
        case EVmbxVideo:
            {
            TRAPD( error, retVal = VideoVmbxL( aNumber ) );
            if ( error )
                {
                retVal = error;
                }
            }
            break;
            
        default:
            break;
        }
 
    __PHONELOG1( EBasic, 
                 EPhoneUIView,"CPhoneVmbxHandler::LaunchMbx -> retval = (%d)", 
                 retVal );
    return retVal;
    }

// -----------------------------------------------------------------------------
// CPhoneVmbxHandler::CSVmbxL
// -----------------------------------------------------------------------------
//
TInt CPhoneVmbxHandler::CSVmbxL( TDes& aNumber )
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneVmbxHandler::CSVmbxL()" );
    CreateVmbxL();
    TInt err ( KErrNone );
    err = iVmbx->GetVmbxNumber( aNumber );
    
    //check error code
    if ( err != KErrNone && err != KErrNotFound )
        {
        User::Leave( err );
        }

    //If not assigned
    if ( err == KErrNotFound )
        {
        aNumber.Zero();
        if ( !( iVmbx->QueryNumberL( EVmbxNotDefinedQuery, aNumber ) ) )
            {
            err = KErrNotFound;
            }
        else
            {
            if ( aNumber.Length() != 0 )
                {
                err = KErrNone;
                }
            else
                {
                err = KErrNotFound;
                }
            }
        }
    return err;
    }
// -----------------------------------------------------------------------------
// CPhoneVmbxHandler::IPVmbxL
// -----------------------------------------------------------------------------
//
TInt CPhoneVmbxHandler::IPVmbxL( TDes& aNumber )   
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneVmbxHandler::IPVmbxL()" );
    CreateVmbxL();

    TInt err = iVmbx->GetVmbxAddress( aNumber, iServiceId );

    return err; 
    }
// -----------------------------------------------------------------------------
// CPhoneVmbxHandler::VideoVmbxL
// -----------------------------------------------------------------------------
//
TInt CPhoneVmbxHandler::VideoVmbxL( TDes& aNumber )   
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneVmbxHandler::VideoVmbxL()" );
    CreateVmbxL();
    TInt err = iVmbx->GetVideoMbxNumber( aNumber );
    //check error code
    if ( err != KErrNone && err != KErrNotFound )
        {
        User::Leave( err );
        }
    //If not assigned
    if ( err == KErrNotFound )
        {
        aNumber.Zero();
        if ( !( iVmbx->QueryVideoMbxNumberL( EVmbxNotDefinedQuery, aNumber ) ) )
            {
            err = KErrNotFound;
            }
        else
            {
            if ( aNumber.Length() != 0 )
                {
                err = KErrNone;
                }
            else
                {
                err = KErrNotFound;
                }
            }
        }
    
    __PHONELOG1( EBasic, 
                 EPhoneUIView,"CPhoneVmbxHandler::VideoVmbxL -> err = (%d)", 
                 err );    
    return err; 
    } 
    
    
// -----------------------------------------------------------------------------
// CPhoneVmbxHandler::NetworkMode
// -----------------------------------------------------------------------------
//
TInt CPhoneVmbxHandler::NetworkMode()
    
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneVmbxHandler::NetworkMode()" );
    TInt networkMode = ENWNetworkModeUnknown;
    
    RProperty::Get( KPSUidNetworkInfo, 
                    KNWTelephonyNetworkMode,
                    networkMode );
    __PHONELOG1( EBasic, 
                 EPhoneUIView,"CPhoneVmbxHandler::NetworkMode -> NWMode = (%d)", 
                 networkMode );                    
    return networkMode; 
    } 
  
// ---------------------------------------------------------
// CPhoneVmbxHandler::CreateVmbxL
// ---------------------------------------------------------
//
void CPhoneVmbxHandler::CreateVmbxL()
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneVmbxHandler::CreateVmbxL()" );    
    if ( !iVmbx )
        {
        RVmbxNumber* vmbx = new ( ELeave ) RVmbxNumber;
        CleanupStack::PushL( vmbx );
        User::LeaveIfError( vmbx->Open() );
        CleanupStack::Pop( vmbx );
        iVmbx = vmbx;
        }
    }

// ---------------------------------------------------------
// CPhoneVmbxHandler::GetMailboxType
// ---------------------------------------------------------
//
TInt CPhoneVmbxHandler::GetMailboxType()
    {
    return iMailboxType;
    }

// ---------------------------------------------------------
// CPhoneVmbxHandler::SelectMailboxTypeL
// ---------------------------------------------------------
//
TInt CPhoneVmbxHandler::SelectMailboxTypeL( TUint& aServiceId )
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneVmbxHandler::SelectMailboxTypeL()" );
    CreateVmbxL();
    
    TInt err( KErrNotFound );
    TInt err2( KErrNotFound );
    TInt err3( KErrNotFound );
    TInt type( EVmbxNone );
    
    TBuf<KPhoneNumberEntryBufferSize> number;
    number.Zero(); // empty the string
    // Voice mbx number
    err = iVmbx->GetVmbxNumber( number );

    // Video mbx number
    if( IsCsVideoTelFeature() )
        {
        err2 = iVmbx->GetVideoMbxNumber( number );
        }

    if ( iVmbx->IsIpVoiceMailboxServices() )
        {
        err3 = KErrNone;
        }

    if ( ( err == KErrNotFound || err == KErrNotSupported ) &&
         ( err2 == KErrNotFound || err2 == KErrNotSupported ) &&
         ( err3 == KErrNotFound || err3 == KErrNotSupported ) )
        {
        type = iVmbx->SelectTypeL( EVmbxDefine );
        }
    else 
        {
        type =iVmbx->SelectTypeL( EVmbxCall, iServiceId );
        aServiceId = iServiceId;
        } 
    
    return type;
    }
   
// ---------------------------------------------------------
// CPhoneVmbxHandler::CancelVmbx
// ---------------------------------------------------------
//
void CPhoneVmbxHandler::CancelVmbx()
    {
    if ( iVmbx )
        {
        iVmbx->Close();
        delete iVmbx;
        iVmbx = NULL;
        }    
    }

//  End of File  

