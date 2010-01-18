/*
* Copyright (c) 2004-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Implementation of the CPhoneApplicationExit class.
*
*/



// INCLUDE FILES
#include    "cphoneapplicationexit.h"
#include    "cphonecenrepproxy.h"
#include    "cphoneviewcontroller.h"
#include    "telephonyvariant.hrh"
#include    "telprivatecrkeys.h"
#include    "phonelogger.h"

#include    <w32std.h>
#include    <e32hal.h>
#include    <apgwgnam.h>
#include    <apgtask.h>
#include    <apgcli.h>
#include    <avkon.hrh>
#include    <telephonyvariant.hrh>
#include    <gfxtranseffect/gfxtranseffect.h>     
#include    <akntranseffect.h>      

// CONSTANTS

// MODULE DATA STRUCTURES

/**
* Abstract base class to handle rules when certain application should be
* closed.
* @since 2.6
*/
class CPhoneApplicationExit::CElement : public CBase
    {
    public: // New functions

        /**
        * Checks if this application should be accepted to be closed.
        * @param aUid uid of the application.
        * @return ETrue if should be closed, otherwise EFalse.
        */
        virtual TBool AcceptL( const TUid& aUid ) const = 0;
    };

/**
* Element to handle any application and low memory cases.
* @since 2.6
*/
class CPhoneApplicationExit::CElementBasic : 
    public CPhoneApplicationExit::CElement
    {
    public: // Constructors

        /**
        * Enumerates types.
        */
        enum TTypeOfElement
            {
            // Any application.
            EExitAnyApp,
            // Only when low memory.
            EExitWhenLowMemory
            };

        /**
        * Constructor.
        * @param aType type of element.
        */
        CElementBasic( TTypeOfElement aType );

    public: // New functions

        /**
        * Checks if system is low in memory.
        * @return ETrue if system is low in memory.
        */
        static TBool IsLowMemory();

    public: // Functions from base classes

        /**
        * @see CPhoneApplicationExit::CElement::AcceptL.
        */ 
        virtual TBool AcceptL( const TUid& aUid ) const;

    private:

        // Type of element.
        TTypeOfElement iType;

    };

/**
* Element to handle applications UID of which are included in UID range.
* @since 2.6
*/
class CPhoneApplicationExit::CElementUid :
    public CPhoneApplicationExit::CElement
    {
    public: // Constructors

        /**
        * Constructor.
        * @param aStart start of range.
        * @param aEnd end of range.
        */
        CElementUid( const TUid& aStart, const TUid& aEnd );

    public: // Functions from base classes

        /**
        * @see CPhoneApplicationExit::CElement::AcceptL.
        */ 
        virtual TBool AcceptL( const TUid& aUid ) const;

    private:

        // Start of the range.
        TUid iStart;

        // End of the range.
        TUid iEnd;

    };

/**
* Element to handle applications on specific drive.
* @since 2.6
*/
class CPhoneApplicationExit::CElementDrive :
    public CPhoneApplicationExit::CElement
    {
    public: // Constructors

        /**
        * Constructor.
        * @param aDrive drive.
        */
        CElementDrive( const TChar& aDrive );

    public: // Functions from base classes

        /**
        * @see CPhoneApplicationExit::CElement::AcceptL.
        */ 
        virtual TBool AcceptL( const TUid& aUid ) const;

    private:

        /**
        * Checks if application has been installed on specified drive.
        * @param aUid application UID.
        * @return ETrue if application has been installed on specified drive.
        */
        TBool CheckApplicationDriveL( 
            const TUid& aUid ) const;

    private:

        // Drive.
        TChar iDrive; 
        
    };

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhoneApplicationExit::CPhoneApplicationExit
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPhoneApplicationExit::CPhoneApplicationExit(
    CPhoneViewController* aViewController,
    RWsSession& aWsSession,
    const TInt aUikonWgId ) :
    iViewController( aViewController ),
    iWsSession( aWsSession ),
    iUikonWgId( aUikonWgId )
    {
    }

// -----------------------------------------------------------------------------
// CPhoneApplicationExit::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPhoneApplicationExit* CPhoneApplicationExit::NewL(
    CPhoneViewController* aViewController,
    RWsSession& aWsSession,
    const TInt aUikonWgId )
    {
    CPhoneApplicationExit* self = 
        new ( ELeave ) CPhoneApplicationExit(
            aViewController,
            aWsSession,
            aUikonWgId );
    
    return self;
    }

// Destructor
CPhoneApplicationExit::~CPhoneApplicationExit()
    {
    iCloseList.ResetAndDestroy();
    iCloseList.Close();

    iDontCloseList.ResetAndDestroy();
    iDontCloseList.Close();
    }

// -----------------------------------------------------------------------------
// CPhoneApplicationExit::ExitApplication
// -----------------------------------------------------------------------------
//
TInt CPhoneApplicationExit::ExitApplication()
    {
    TRAPD( err, ExitApplicationL() );
#ifdef __PHENG_DEBUG_INFO__
    if ( err != KErrNone )
        {
        __PHONELOG1( EBasic, EPhoneUIView, "CPhoneApplicationExit::ExitApplication err=%d", err );
        }
#endif
    return err;
    }

// -----------------------------------------------------------------------------
// CPhoneApplicationExit::ExitApplicationL
// -----------------------------------------------------------------------------
//
void CPhoneApplicationExit::ExitApplicationL()
    {
    __LOGMETHODSTARTEND( EPhoneUIView, "CPhoneApplicationExit::ExitApplicationL()" );
    TInt foregroundWindowGroupId = iWsSession.GetFocusWindowGroup();
    if ( foregroundWindowGroupId == iUikonWgId )
        {
        foregroundWindowGroupId = 
            iViewController->ForegroundApplicationWindowGroupId();
        }
    if ( foregroundWindowGroupId == 
        iViewController->ApplicationWindowGroupId() )
        {
        // No need to send event to itself.
        __PHONELOG( EBasic, EPhoneUIView, "CPhoneApplicationExit::ExitApplicationL Phone is foreground application so don't close it" );
        if ( IsApplicationClosingEnabled() )
            {
            ReadListsL();
            }
        return;
        }
    __PHONELOG1( EBasic, EPhoneUIView, "CPhoneApplicationExit::ExitApplicationL foregroundWindowGroupId=%d", foregroundWindowGroupId );
    
    CApaWindowGroupName* wgName = 
        CApaWindowGroupName::NewLC(
            iWsSession, 
            foregroundWindowGroupId );

    if ( IsApplicationClosingEnabled() )
        {
        __PHONELOG1( EBasic, EPhoneUIView, "CPhoneApplicationExit::ExitApplicationL Application closing is enabled wgName->AppUid().iUid=%d", wgName->AppUid().iUid );

        if ( CheckApplicationClosingL( wgName->AppUid() ) )
            {
            // Transition effects end key handling
            // This call makes it possible to show application exit effect
            // whenever exiting application with the end key
            GfxTransEffect::BeginFullScreen( 
                         AknTransEffect::EApplicationExit, 
                         TRect(), 
                         AknTransEffect::EParameterType, 
                         AknTransEffect::GfxTransParam( wgName->AppUid(), 
                         AknTransEffect::TParameter::EActivateExplicitCancel | 
                         AknTransEffect::TParameter::EEndCheck ) );
 
            __PHONELOG( EBasic, EPhoneUIView, "CPhoneApplicationExit::ExitApplicationL Send exit event" );
            TWsEvent event;
            event.SetType( KAknUidValueEndKeyCloseEvent );
            event.SetTimeNow();

            iWsSession.SendEventToWindowGroup( 
                foregroundWindowGroupId, 
                event );
            }
        }
    else
        {
        __PHONELOG( EBasic, EPhoneUIView, "CPhoneApplicationExit::ExitApplicationL Application closing is NOT enabled" );

        // If the application is not system and system has low memory, then
        // stop the application.
        if ( !wgName->IsSystem() && CElementBasic::IsLowMemory() )
            {
            __PHONELOG( EBasic, EPhoneUIView, "CPhoneApplicationExit::ExitApplicationL Close non-system app because of low memory" );
            // Close application
            TApaTask task( iWsSession );
            task.SetWgId( foregroundWindowGroupId );
            task.EndTask();
            }
        }

    CleanupStack::PopAndDestroy( wgName );
    }

// -----------------------------------------------------------------------------
// CPhoneApplicationExit::CheckApplicationClosingL
// -----------------------------------------------------------------------------
//
TBool CPhoneApplicationExit::CheckApplicationClosingL( 
    const TUid& aUid )
    {
    ReadListsL();

    return 
        IsIncludedToCloseListL( aUid ) && !IsIncludedToDontCloseListL( aUid );
    }

// -----------------------------------------------------------------------------
// CPhoneApplicationExit::IsIncludedToCloseListL
// -----------------------------------------------------------------------------
//
TBool CPhoneApplicationExit::IsIncludedToCloseListL( 
    const TUid& aUid ) const
    {
    __PHONELOG( EBasic, EPhoneUIView, "CPhoneApplicationExit::IsIncludedToCloseList()" );

    TBool result = 
        CheckListL( 
            iCloseList, 
            aUid );

    __PHONELOG1( EBasic, EPhoneUIView, "CPhoneApplicationExit::IsIncludedToCloseList() result=%d", result );

    return result;
    }

// -----------------------------------------------------------------------------
// CPhoneApplicationExit::IsIncludedToDontCloseListL
// -----------------------------------------------------------------------------
//
TBool CPhoneApplicationExit::IsIncludedToDontCloseListL( 
    const TUid& aUid ) const
    {
    __PHONELOG( EBasic, EPhoneUIView, "CPhoneApplicationExit::IsIncludedToDontCloseList()" );

    TBool result = 
        CheckListL( 
            iDontCloseList, 
            aUid );

    __PHONELOG1( EBasic, EPhoneUIView, "CPhoneApplicationExit::IsIncludedToDontCloseList() result=%d", result );

    return result;
    }

// -----------------------------------------------------------------------------
// CPhoneApplicationExit::CheckListL
// -----------------------------------------------------------------------------
//
TBool CPhoneApplicationExit::CheckListL( 
    const RPointerArray< CElement >& aTarget, 
    const TUid& aUid ) const
    {
    TBool result = EFalse;

    const TInt count = aTarget.Count();
    
    for ( TInt i = 0; i < count && !result; i++ )
        {
        result = aTarget[ i ]->AcceptL( aUid );
        }
    
    return result;
    }

// -----------------------------------------------------------------------------
// CPhoneApplicationExit::ReadListsL
// -----------------------------------------------------------------------------
//
void CPhoneApplicationExit::ReadListsL()
    {
    if ( !iListsRead )
        {
        iCloseList.ResetAndDestroy();
        iDontCloseList.ResetAndDestroy();

        // Acquire buffer for reading operations.
        HBufC* tempBuffer = HBufC::NewLC( KPhoneTemporaryBufferMaxLength );
        tempBuffer->Des().Zero();
        TPtr ptr( tempBuffer->Des() );

        TInt err = KErrNone;
        // Read "close" list.
        err = CPhoneCenRepProxy::Instance()->GetString(
            KCRUidTelPrivateVariation,
            KTelAppsCloseList,
            ptr );
        if ( err == KErrNone )
            {
            BuildListL( iCloseList, ptr, ETrue );
            }
        if ( err == KErrNotFound )
            {
            // Not defined.
            err = KErrNone;
            }
        User::LeaveIfError( err );

        // Read "don't close" list.
        err = CPhoneCenRepProxy::Instance()->GetString(
            KCRUidTelPrivateVariation,
            KTelAppsKeepList,
            ptr );
        if ( err == KErrNone )
            {
            BuildListL( iDontCloseList, ptr, EFalse );
            }
        if ( err == KErrNotFound )
            {
            // Not defined.
            err = KErrNone;
            }
        User::LeaveIfError( err );

        // By default, applications are closed on OOM.
        if ( !iCloseList.Count() && !iDontCloseList.Count() )
            {
            CElement* element = 
                new ( ELeave ) CElementBasic( 
                    CElementBasic::EExitWhenLowMemory );
            CleanupStack::PushL( element );
            User::LeaveIfError( iCloseList.Append( element ) );
            CleanupStack::Pop( element );
            }

        CleanupStack::PopAndDestroy( tempBuffer );
        iListsRead = ETrue;

        }
    }

// -----------------------------------------------------------------------------
// CPhoneApplicationExit::BuildListL
// -----------------------------------------------------------------------------
//
void CPhoneApplicationExit::BuildListL(
    RPointerArray< CElement >& aTarget,
    const TDesC& aSource,
    TBool aOomAllowed )
    {
    aTarget.ResetAndDestroy();

    TLex lexer( aSource );
    lexer.SkipSpace();

    TUid start;
    TUid end;
    TChar drive;

    TInt length = lexer.Remainder().Length();
    while ( length )
        {
        lexer.SkipSpace();

        CElement* element = NULL;

        if ( ParseString( lexer, KPhoneApplicationAnyApp ) )
            {
            element = 
                new ( ELeave ) CElementBasic( 
                    CElementBasic::EExitAnyApp );
            }
        else if ( ParseString( lexer, KPhoneApplicationOom ) )
            {
            if ( aOomAllowed )
                {
                element = 
                    new ( ELeave ) CElementBasic( 
                        CElementBasic::EExitWhenLowMemory );
                }
            }
        else if ( ParseDrive( lexer, drive ) )
            {
            element = new ( ELeave ) CElementDrive( drive );
            }
        else if ( ParseUid( lexer, start, end ) )
            {
            
            element = new ( ELeave ) CElementUid( start, end );
            }
        else
            {
            // No proceeding, stop.
            length = 0;
            }

        if ( element )
            {
            CleanupStack::PushL( element );
            User::LeaveIfError( aTarget.Append( element ) );
            CleanupStack::Pop( element );
            }

        if ( length )
            {
            const TInt oldLength = length;
            length = lexer.Remainder().Length();

            if ( length == oldLength )
                {
                // No proceeding, stop.
                length = 0;
                }
            }
        }
    }

// -----------------------------------------------------------------------------
// CPhoneApplicationExit::ParseString
// -----------------------------------------------------------------------------
//
TBool CPhoneApplicationExit::ParseString( 
    TLex& aLexer,
    const TDesC& aString )
    {
    TBool result = EFalse;
    TLexMark startMark;
    aLexer.Mark( startMark );
    const TInt length = aString.Length();
    
    TInt i;
    for ( i = 0; 
          i < length && ( aString[ i ] == TUint( aLexer.Peek() ) ); 
          i++ )
        {
        aLexer.Inc();
        }

    result = ( ( i == length ) && CheckEnd( aLexer ) );

    if ( !result )
        {
        aLexer.UnGetToMark( startMark );
        }
    return result;
    }

// -----------------------------------------------------------------------------
// CPhoneApplicationExit::ParseUid
// -----------------------------------------------------------------------------
//
TBool CPhoneApplicationExit::ParseUid( 
    TLex& aLexer, 
    TUid& aUidStart,
    TUid& aUidEnd )
    {
    TBool result = EFalse;
    TLexMark startMark;

    // Read first UID.
    aLexer.Mark( startMark );
    while ( aLexer.Peek().IsHexDigit() )
        {
        aLexer.Inc();
        }
    TPtrC token = aLexer.MarkedToken( startMark );
    if ( token.Length() )
        {
        TUint uid = 0;
        result = ( TLex( token ).Val( uid, EHex ) == KErrNone );
        if ( result )
            {
            aUidStart.iUid = uid;
            aUidEnd.iUid = uid;
            }
        }

    if ( result )
        {
        TLexMark afterUid;
        aLexer.Mark( afterUid );
        aLexer.SkipSpace();

        // Check range separator.
        if ( aLexer.Peek() == KPhoneApplicationRangeSeparator )
            {
            // Now there must be another UID as well.
            result = EFalse; 
            aLexer.Inc(); // skip separator character
            aLexer.SkipSpace();

            // Read second UID.
            TLexMark mark;
            aLexer.Mark( mark );
            while ( aLexer.Peek().IsHexDigit() )
                {
                aLexer.Inc();
                }

            TPtrC token = aLexer.MarkedToken( mark );
            if ( token.Length() )
                {
                TUint uid = 0;
                result = ( TLex( token ).Val( uid, EHex ) == KErrNone );
                if ( result )
                    {
                    result = CheckEnd( aLexer );
                    aUidEnd.iUid = uid;
                    }
                }
            }
        else
            {
            // Only one UID, next must be space.
            aLexer.UnGetToMark( afterUid );
            result = CheckEnd( aLexer );
            }
        }

    if ( !result )
        {
        aLexer.UnGetToMark( startMark );
        }

    return result;
    }

// -----------------------------------------------------------------------------
// CPhoneApplicationExit::ParseDrive
// -----------------------------------------------------------------------------
//
TBool CPhoneApplicationExit::ParseDrive(
    TLex& aLexer, 
    TChar& aChar )
    {
    TBool result = EFalse;
    TChar ch = aLexer.Peek();
    
    if ( ch.IsAlpha() )
        {
        aLexer.Inc();
        if ( aLexer.Peek() == KPhoneApplicationDriveIdentifier )
            {
            aLexer.Inc();
            aChar = ch;
            result = CheckEnd( aLexer );
            if ( !result )
                {
                aLexer.UnGet(); // drive identifier
                aLexer.UnGet(); // drive letter
                }
            }
        else
            {
            aLexer.UnGet(); // drive letter
            }
        }

    return result;
    }

// -----------------------------------------------------------------------------
// CPhoneApplicationExit::CheckEnd
// -----------------------------------------------------------------------------
//
inline TBool CPhoneApplicationExit::CheckEnd( TLex& aLexer )
    {
    return ( aLexer.Eos() || aLexer.Peek().IsSpace() );
    }

// -----------------------------------------------------------------------------
// CPhoneApplicationExit::IsApplicationClosingEnabled
// -----------------------------------------------------------------------------
//
TBool CPhoneApplicationExit::IsApplicationClosingEnabled() const
    {
    return CPhoneCenRepProxy::Instance()->IsTelephonyFeatureSupported( 
        KTelephonyLVFlagCloseApplications );
    }

// Implementation of CPhoneApplicationExit::CElementBasic.

// -----------------------------------------------------------------------------
// CPhoneApplicationExit::CElementBasic::CElementBasic
// -----------------------------------------------------------------------------
//
CPhoneApplicationExit::CElementBasic::CElementBasic( TTypeOfElement aType ) :
    iType( aType )
    {
    }

// -----------------------------------------------------------------------------
// CPhoneApplicationExit::CElementBasic::IsLowMemory
// -----------------------------------------------------------------------------
//
TBool CPhoneApplicationExit::CElementBasic::IsLowMemory()
    {
    // Fetch amount of free memory.
    TMemoryInfoV1Buf memory;
    UserHal::MemoryInfo( memory );
    TInt freeRam = (TInt)( memory().iFreeRamInBytes );
    TInt oomRamLowThreshold = 0;
    /* MIGRATION NOTE - need new method to TelUtils for static get of one
    value.
    CRepository* cenRep = NULL;
    TRAPD( error, cenRep = CRepository::NewL( KCRUidUiklaf ) );
    if ( error == KErrNoMemory )        
        {
        return ETrue;
        }
    else if ( error != KErrNone )
        {
        // Error; don't know OOM status.
        return EFalse;
        }
    cenRep->Get( KUikOOMRamLowThreshold, oomRamLowThreshold );
    delete cenRep;*/
    return ( freeRam < oomRamLowThreshold );    
    }

// -----------------------------------------------------------------------------
// CPhoneApplicationExit::CElementBasic::AcceptL
// -----------------------------------------------------------------------------
//
TBool CPhoneApplicationExit::CElementBasic::AcceptL( 
    const TUid& /*aUid*/ ) const
    {
    TBool result = EFalse;
    switch ( iType )
        {
        case EExitAnyApp:
            result = ETrue;
            break;

        case EExitWhenLowMemory:
            result = IsLowMemory();
            break;

        default:
            break;
        }
    return result;
    }

// Implementation of CPhoneApplicationExit::CElementUid.

// -----------------------------------------------------------------------------
// CPhoneApplicationExit::CElementUid::CElementUid
// -----------------------------------------------------------------------------
//
CPhoneApplicationExit::CElementUid::CElementUid( 
    const TUid& aStart, 
    const TUid& aEnd ) :
    iStart( aStart ), 
    iEnd( aEnd )
    {
    }

// -----------------------------------------------------------------------------
// CPhoneApplicationExit::CElementUid::AcceptL
// -----------------------------------------------------------------------------
//
TBool CPhoneApplicationExit::CElementUid::AcceptL( const TUid& aUid ) const
    {
    return ( iStart.iUid <= aUid.iUid && aUid.iUid <= iEnd.iUid );
    }

// Implementation of CPhoneApplicationExit::CElementDrive.

// -----------------------------------------------------------------------------
// CPhoneApplicationExit::CElementDrive::CElementDrive
// -----------------------------------------------------------------------------
//
CPhoneApplicationExit::CElementDrive::CElementDrive( const TChar& aDrive ) :
    iDrive( aDrive )
    {
    }

// -----------------------------------------------------------------------------
// CPhoneApplicationExit::CElementDrive::AcceptL
// -----------------------------------------------------------------------------
//
TBool CPhoneApplicationExit::CElementDrive::AcceptL( const TUid& aUid ) const
    {
    return CheckApplicationDriveL( aUid );
    }

// -----------------------------------------------------------------------------
// CPhoneApplicationExit::CElementDrive::CheckApplicationDriveL
// -----------------------------------------------------------------------------
//
TBool CPhoneApplicationExit::CElementDrive::CheckApplicationDriveL( 
    const TUid& aUid ) const
    {
    RApaLsSession session;
    User::LeaveIfError( session.Connect() );
    CleanupClosePushL( session );

    TApaAppInfo* appInfo = new ( ELeave ) TApaAppInfo;
    CleanupStack::PushL( appInfo );

    TBool result = EFalse;
    TInt count = KPhoneApplicationRetryCount;
    while ( count )
        {
        TInt err = session.GetAppInfo( *appInfo, aUid );
        
        if ( err == KErrNone )
            {
            count = 0;
            result = ETrue;
            }
        else if ( err == KErrNotFound )
            {
            count = 0;
            result = EFalse;
            }
        else
            {
            User::LeaveIfError( err );

            // GetAppInfo may return positive value indicating that
            // application information is not ready.
            count--;
            User::After( KPhoneApplicationRetryInterval );
            }
        }

    if ( result )
        {
        result = EFalse;
        if ( appInfo->iFullName.Length() )
            {
            // First character is drive
            TChar drive( appInfo->iFullName[ 0 ] );
            drive.UpperCase();

            TChar targetDrive( iDrive );
            targetDrive.UpperCase();

            result = ( drive == targetDrive );
            }
        }

    CleanupStack::PopAndDestroy( appInfo );
    CleanupStack::PopAndDestroy(); // CleanupClosePushL
    return result;
    }

//  End of File
