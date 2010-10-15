
#include "devsoundstub.h"

RPointerArray< CTestFunctionCallData > CMdaAudioPlayerUtility::iStaticTestData;
    
RPointerArray< CTestFunctionCallData > CMdaAudioToneUtility::iStaticTestData;
    
bool CMdaAudioPlayerUtility::iDelayedAudioLoading = EFalse;
    
bool CMdaAudioPlayerUtility::iErrorInAudioLoading = EFalse;
    
bool CMdaAudioToneUtility::iDelayedAudioLoading = EFalse;
    
bool CMdaAudioToneUtility::iErrorInAudioLoading = EFalse;
    
const TTimeIntervalMicroSeconds32 KNormalLoadingTime = TTimeIntervalMicroSeconds32( 1500000 );
const TTimeIntervalMicroSeconds32 KDelayedLoadingTime = TTimeIntervalMicroSeconds32( 4000000 );

const static TInt KGeneratedError = KErrNotFound;


// -----------------------------------------------------------------------------
// CMdaServer::NewL()
// -----------------------------------------------------------------------------
//
CMdaServer* CMdaServer::NewL()
    {
    return new( ELeave )CMdaServer;
    }

// -----------------------------------------------------------------------------
// CMdaServer::~CMdaServer()
// -----------------------------------------------------------------------------
//
CMdaServer::~CMdaServer()
    {
    }

// -----------------------------------------------------------------------------
// CTestFunctionCallData::NewL
// -----------------------------------------------------------------------------
//
CTestFunctionCallData* CTestFunctionCallData::NewL( TInt aFunctionIndex, HBufC* aString, TInt aInt1, TInt aInt2 )
    {
    return new( ELeave )CTestFunctionCallData( aFunctionIndex, aString, aInt1, aInt2, TTimeIntervalMicroSeconds( 0 ) );
    }

// -----------------------------------------------------------------------------
// CTestFunctionCallData::NewL
// -----------------------------------------------------------------------------
//
CTestFunctionCallData* CTestFunctionCallData::NewL( TInt aFunctionIndex, TInt aInt1, TInt aInt2 )
    {
    return new( ELeave )CTestFunctionCallData( aFunctionIndex, NULL, aInt1, aInt2, TTimeIntervalMicroSeconds( 0 ) );
    }

// -----------------------------------------------------------------------------
// CTestFunctionCallData::NewL
// -----------------------------------------------------------------------------
//
CTestFunctionCallData* CTestFunctionCallData::NewL( TInt aFunctionIndex, TInt aInt1 )
    {
    return new( ELeave )CTestFunctionCallData( aFunctionIndex, NULL, aInt1, 0, TTimeIntervalMicroSeconds( 0 ) );
    }

// -----------------------------------------------------------------------------
// CTestFunctionCallData::NewL
// -----------------------------------------------------------------------------
//
CTestFunctionCallData* CTestFunctionCallData::NewL( TInt aFunctionIndex, TInt aInt1, TTimeIntervalMicroSeconds aSeconds )
    {
    return new( ELeave )CTestFunctionCallData( aFunctionIndex, NULL, aInt1, 0, aSeconds );
    }

// -----------------------------------------------------------------------------
// CTestFunctionCallData::NewL
// -----------------------------------------------------------------------------
//
CTestFunctionCallData* CTestFunctionCallData::NewL( TInt aFunctionIndex, TTimeIntervalMicroSeconds aSeconds )
    {
    return new( ELeave )CTestFunctionCallData( aFunctionIndex, NULL, 0, 0, aSeconds );
    }

// -----------------------------------------------------------------------------
// CTestFunctionCallData::NewL
// -----------------------------------------------------------------------------
//
CTestFunctionCallData* CTestFunctionCallData::NewL( TInt aFunctionIndex, HBufC8* aString )
    {
    return new( ELeave )CTestFunctionCallData( aFunctionIndex, aString );
    }

// -----------------------------------------------------------------------------
// CTestFunctionCallData::~CTestFunctionCallData()
// -----------------------------------------------------------------------------
//
CTestFunctionCallData::~CTestFunctionCallData()
    {
    delete iP1;
    delete iP5;
    }

// -----------------------------------------------------------------------------
// CTestFunctionCallData::CTestFunctionCallData
// -----------------------------------------------------------------------------
//
CTestFunctionCallData::CTestFunctionCallData( TInt aFunctionIndex, HBufC* aPtr1, TInt aInt1, TInt aInt2, TTimeIntervalMicroSeconds aSeconds ) :
    iFunctionIndex( aFunctionIndex ),
    iP1( aPtr1 ),
    iP2( aInt1 ),
    iP3( aInt2 ),
    iP4( aSeconds )
    {
    }

// -----------------------------------------------------------------------------
// CTestFunctionCallData::CTestFunctionCallData
// -----------------------------------------------------------------------------
//
CTestFunctionCallData::CTestFunctionCallData( TInt aFunctionIndex, HBufC8* aString ) : iFunctionIndex( aFunctionIndex ),
    iP5( aString )
    {
    }

// -----------------------------------------------------------------------------
// CTestFunctionCallData::DesC1()
// -----------------------------------------------------------------------------
//
HBufC* CTestFunctionCallData::DesC1()
    {
    return iP1;
    }

// -----------------------------------------------------------------------------
// CTestFunctionCallData::DesC2()
// -----------------------------------------------------------------------------
//
HBufC8* CTestFunctionCallData::DesC2()
    {
    return iP5;
    }

// -----------------------------------------------------------------------------
// CTestFunctionCallData::Int1()
// -----------------------------------------------------------------------------
//
TInt CTestFunctionCallData::Int1()
    {
    return iP2;
    }

// -----------------------------------------------------------------------------
// CTestFunctionCallData::Int2()
// -----------------------------------------------------------------------------
//
TInt CTestFunctionCallData::Int2()
    {
    return iP3;
    }

// -----------------------------------------------------------------------------
// CTestFunctionCallData::Seconds()
// -----------------------------------------------------------------------------
//
TTimeIntervalMicroSeconds CTestFunctionCallData::Seconds()
    {
    return iP4;
    }

// -----------------------------------------------------------------------------
// CTestFunctionCallData::FunctionIndex()
// -----------------------------------------------------------------------------
//
TInt CTestFunctionCallData::FunctionIndex()
    {
    return iFunctionIndex;
    }

// -----------------------------------------------------------------------------
// CMdaAudioPlayerUtility::NewFilePlayerL
// -----------------------------------------------------------------------------
//
TBool CTestFunctionCallData::operator ==( CTestFunctionCallData& aData )
    {
    TBool equals( ETrue );
    
    if ( aData.FunctionIndex() != iFunctionIndex )
        {
        equals = EFalse;
        }
    else if ( iP1 && !aData.DesC1() )
        {
         equals = EFalse;
        } 
    else if ( !iP1 && aData.DesC1() )
        {
        equals = EFalse;
        }
    else if ( iP1 && aData.DesC1() && iP1->Des().Compare( *aData.DesC1() ) )
        {
        equals = EFalse;
        }
    else if ( iP5 && !aData.DesC2() )
        {
        equals = EFalse;
        }
    else if (!iP5 && aData.DesC2() )
        {
        equals = EFalse;
        }
    else if ( iP5 && aData.DesC2() && iP5->Des().Compare( *aData.DesC2() ) )
        {
        equals = EFalse;
        }
    else if ( iP2 != aData.Int1() )
        {
        equals = EFalse;
        }
    else if ( iP3 != aData.Int2() )
        {
        equals = EFalse;
        }
    else if ( iP4 != aData.Seconds() )
        {
        equals = EFalse;
        }
    return equals;
    }

// -----------------------------------------------------------------------------
// CMdaAudioPlayerUtility::NewFilePlayerL
// -----------------------------------------------------------------------------
//
CMdaAudioPlayerUtility* CMdaAudioPlayerUtility::NewFilePlayerL(const TDesC& aFileName,
    MMdaAudioPlayerCallback& aCallback,
    TInt aPriority,
    TInt aPref,
    CMdaServer* aServer )
    {
    HBufC* desc = HBufC::NewL( aFileName.Length() );
    *desc = aFileName;
    CleanupStack::PushL( desc );
    CTestFunctionCallData* data = CTestFunctionCallData::NewL( CMdaAudioPlayerUtility::ENewFilePlayerL, desc, aPriority, aPref );
    CleanupStack::Pop( desc );
    CMdaAudioPlayerUtility::iStaticTestData.Append( data );
    CMdaAudioPlayerUtility* self = new( ELeave )CMdaAudioPlayerUtility(  aCallback, aPriority, aPref, aServer );
    self->ConstructL( aFileName );
    return self;
    }
    
// -----------------------------------------------------------------------------
// CMdaAudioPlayerUtility::NewL
// -----------------------------------------------------------------------------
//
CMdaAudioPlayerUtility* CMdaAudioPlayerUtility::NewL( MMdaAudioPlayerCallback& aCallback,
    TInt aPriority,
    TInt aPref )
    {
    CTestFunctionCallData* data = CTestFunctionCallData::NewL( CMdaAudioPlayerUtility::ENewL, aPriority, aPref );
    CMdaAudioPlayerUtility::iStaticTestData.Append( data );
    CMdaAudioPlayerUtility* self = new( ELeave )CMdaAudioPlayerUtility(  aCallback, aPriority, aPref, NULL );
    return self;
    }

// -----------------------------------------------------------------------------
// CMdaAudioPlayerUtility::CMdaAudioPlayerUtility
// -----------------------------------------------------------------------------
//
CMdaAudioPlayerUtility::CMdaAudioPlayerUtility( MMdaAudioPlayerCallback& aObserver, TInt aPriority, TInt aPref, CMdaServer* aServer ) :
    CActive( EPriorityStandard ),
    iObserver( aObserver ),
    iMaxVolume( 10 ),
    iVolume( 5 ),
    iPriority( aPriority ),
    iPref( aPref ),
    iServer( aServer )
    {
    CActiveScheduler::Add( this );
    }

// -----------------------------------------------------------------------------
// CMdaAudioPlayerUtility::~CMdaAudioPlayerUtility()
// -----------------------------------------------------------------------------
//
CMdaAudioPlayerUtility::~CMdaAudioPlayerUtility()
    {
    //iTimer.Cancel();
    Cancel();
    iTimer.Close();
    delete iFile;
    delete iFile8;
    }

// -----------------------------------------------------------------------------
// CMdaAudioPlayerUtility::SetVolume
// -----------------------------------------------------------------------------
//
TInt CMdaAudioPlayerUtility::SetVolume(TInt aVolume)
    {
    CTestFunctionCallData* data = NULL;
    TRAP_IGNORE( data = CTestFunctionCallData::NewL( CMdaAudioPlayerUtility::ESetVolume, aVolume ) );
    CMdaAudioPlayerUtility::iStaticTestData.Append( data );
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdaAudioPlayerUtility::SetRepeats
// -----------------------------------------------------------------------------
//
void CMdaAudioPlayerUtility::SetRepeats(TInt aRepeatNumberOfTimes, const TTimeIntervalMicroSeconds& aTrailingSilence )
    {
    CTestFunctionCallData* data = NULL;
    TRAP_IGNORE( data = CTestFunctionCallData::NewL( CMdaAudioPlayerUtility::ESetRepeats, aRepeatNumberOfTimes, aTrailingSilence ) );
    CMdaAudioPlayerUtility::iStaticTestData.Append( data );
    }

// -----------------------------------------------------------------------------
// CMdaAudioPlayerUtility::SetVolumeRamp
// -----------------------------------------------------------------------------
//
void CMdaAudioPlayerUtility::SetVolumeRamp(const TTimeIntervalMicroSeconds& aRampDuration)
    {
    CTestFunctionCallData* data = NULL;
    TRAP_IGNORE( data = CTestFunctionCallData::NewL( CMdaAudioPlayerUtility::ESetVolumeRamp, aRampDuration ) );
    CMdaAudioPlayerUtility::iStaticTestData.Append( data );
    }

// -----------------------------------------------------------------------------
// CMdaAudioPlayerUtility::MaxVolume()
// -----------------------------------------------------------------------------
//
TInt CMdaAudioPlayerUtility::MaxVolume()
    {
    return iMaxVolume;
    }

// -----------------------------------------------------------------------------
// CMdaAudioPlayerUtility::GetVolume
// -----------------------------------------------------------------------------
//
TInt CMdaAudioPlayerUtility::GetVolume(TInt& aVolume)
    {
    aVolume = iVolume;
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMdaAudioPlayerUtility::OpenDesL
// -----------------------------------------------------------------------------
//
void CMdaAudioPlayerUtility::OpenDesL(const TDesC8& aDescriptor)
    {
    HBufC8* desc = HBufC8::NewL( aDescriptor.Length() );
    *desc = aDescriptor;
    CleanupStack::PushL( desc );
    CTestFunctionCallData* data = CTestFunctionCallData::NewL( 
            CMdaAudioPlayerUtility::EOpenDesL, desc );
    CleanupStack::Pop( desc );
    CMdaAudioPlayerUtility::iStaticTestData.Append( data );
    ConstructL( aDescriptor );
    }

// -----------------------------------------------------------------------------
// CMdaAudioPlayerUtility::DoCancel()
// -----------------------------------------------------------------------------
//
void CMdaAudioPlayerUtility::DoCancel()
    {
    iTimer.Cancel();
    }

// -----------------------------------------------------------------------------
// CMdaAudioPlayerUtility::RunL()
// -----------------------------------------------------------------------------
//
void CMdaAudioPlayerUtility::RunL()
    {
        
    TTimeIntervalMicroSeconds s( 0 );
    if ( CMdaAudioPlayerUtility::iErrorInAudioLoading )
        {
        iObserver.MapcInitComplete( KGeneratedError, s );
        }
    else
        {
        iObserver.MapcInitComplete( KErrNone, s );
        }
    }

// -----------------------------------------------------------------------------
// CMdaAudioPlayerUtility::DelayAudioLoading()
// -----------------------------------------------------------------------------
//
void CMdaAudioPlayerUtility::DelayAudioLoading()
    {
    CMdaAudioPlayerUtility::iDelayedAudioLoading = ETrue;
    }

// -----------------------------------------------------------------------------
// CMdaAudioPlayerUtility::SimulateErrorInAudioLoading()
// -----------------------------------------------------------------------------
//
void CMdaAudioPlayerUtility::SimulateErrorInAudioLoading()
    {
    CMdaAudioPlayerUtility::iErrorInAudioLoading = ETrue;
    }

// -----------------------------------------------------------------------------
// CMdaAudioPlayerUtility::TestData()
// -----------------------------------------------------------------------------
//
RPointerArray< CTestFunctionCallData >& CMdaAudioPlayerUtility::TestData()
    {
    return CMdaAudioPlayerUtility::iStaticTestData;
    }

// -----------------------------------------------------------------------------
// CMdaAudioPlayerUtility::ResetTestData()
// -----------------------------------------------------------------------------
//
void CMdaAudioPlayerUtility::ResetTestData()
    {
    CMdaAudioPlayerUtility::iStaticTestData.ResetAndDestroy();
    CMdaAudioPlayerUtility::iDelayedAudioLoading = EFalse;
    CMdaAudioPlayerUtility::iErrorInAudioLoading = EFalse;
    }

// -----------------------------------------------------------------------------
// CMdaAudioPlayerUtility::ConstructL()
// -----------------------------------------------------------------------------
//
 void CMdaAudioPlayerUtility::ConstructL( const TDesC& aFileName )
    {
    iTimer.CreateLocal();
    
    if ( CMdaAudioPlayerUtility::iDelayedAudioLoading )
        {
        iTimer.After( iStatus, KDelayedLoadingTime );
        }
    else
        {
        iTimer.After( iStatus, KNormalLoadingTime );
        }
    SetActive();
    
    delete iFile;
    iFile = NULL;   
    
    iFile = HBufC::NewL( aFileName.Length() );
    *iFile = aFileName;
    }
    
void CMdaAudioPlayerUtility::ConstructL( const TDesC8& aFileName )
    {
    iTimer.CreateLocal();
    if ( CMdaAudioPlayerUtility::iDelayedAudioLoading )
        {
        iTimer.After( iStatus, KDelayedLoadingTime );
        }
    else
        {
        iTimer.After( iStatus, KNormalLoadingTime );
        }
    SetActive();
    
    delete iFile8;
    iFile8 = NULL;
    
    iFile8 = HBufC8::NewL( aFileName.Length() );
    *iFile8 = aFileName;
    }

// -----------------------------------------------------------------------------
// CMdaAudioPlayerUtility::Play()
// -----------------------------------------------------------------------------
//
void CMdaAudioPlayerUtility::Play()
    {
    CTestFunctionCallData* data( NULL );
    if ( iFile )
        {
        TRAP_IGNORE(  data = CTestFunctionCallData::NewL( CMdaAudioPlayerUtility::EPlay, iFile, 0, 0 ) );
        iFile = NULL;
        }
    else if ( iFile8 )
        {
        TRAP_IGNORE(  data = CTestFunctionCallData::NewL( CMdaAudioPlayerUtility::EPlay, iFile8 ) );
        iFile8 = NULL;
        }
    else
        {
        TRAP_IGNORE( data = CTestFunctionCallData::NewL( CMdaAudioPlayerUtility::EPlay ) );
        } 
    CMdaAudioPlayerUtility::iStaticTestData.Append( data );
    }

// -----------------------------------------------------------------------------
// CMdaAudioPlayerUtility::Stop()
// -----------------------------------------------------------------------------
//
void CMdaAudioPlayerUtility::Stop()
    {
    CTestFunctionCallData* data = CTestFunctionCallData::NewL( CMdaAudioPlayerUtility::EStop );
    CMdaAudioPlayerUtility::iStaticTestData.Append( data );
    }
   

// -----------------------------------------------------------------------------
// CMdaAudioPlayerUtility::Close()
// -----------------------------------------------------------------------------
//
void CMdaAudioPlayerUtility::Close()
    {

    }



//***************************************
/*
**
**
**  CMdaAudioToneUtility
**
**
**
*****************************************/


// -----------------------------------------------------------------------------
// CMdaAudioToneUtility::NewL
// -----------------------------------------------------------------------------
//
/*
CMdaAudioToneUtility* CMdaAudioToneUtility::NewL(MMdaAudioToneObserver& aObserver, CMdaServer* aServer )
    {
    CTestFunctionCallData* data = CTestFunctionCallData::NewL( CMdaAudioToneUtility::ENewL );
    CMdaAudioToneUtility::iStaticTestData.Append( data );
    CMdaAudioToneUtility* self = new( ELeave )CMdaAudioToneUtility( aObserver, 0, 0, aServer );
    return self;
    }
*/
// -----------------------------------------------------------------------------
// CMdaAudioToneUtility::NewL
// -----------------------------------------------------------------------------
//
CMdaAudioToneUtility* CMdaAudioToneUtility::NewL(MMdaAudioToneObserver& aObserver, CMdaServer* aServer,
                                               TInt aPriority, 
                                               TInt aPref )
    {
    CTestFunctionCallData* data = CTestFunctionCallData::NewL( CMdaAudioToneUtility::ENewL, aPriority, aPref );
    CMdaAudioToneUtility::iStaticTestData.Append( data );
    CMdaAudioToneUtility* self = new( ELeave )CMdaAudioToneUtility( aObserver, aPriority, aPref, aServer );
    return self;
    }
    
// -----------------------------------------------------------------------------
// CMdaAudioToneUtility::CMdaAudioToneUtility
// -----------------------------------------------------------------------------
//
CMdaAudioToneUtility::CMdaAudioToneUtility( MMdaAudioToneObserver& aObserver, TInt aPriority, TInt aPref, CMdaServer* aServer ) :
    CActive( EPriorityStandard ),
    iObserver( aObserver ),
    iPriority( aPriority ),
    iPref( aPref ),
    iServer( aServer )
    {
    CActiveScheduler::Add( this );
    }

// -----------------------------------------------------------------------------
// CMdaAudioToneUtility::~CMdaAudioToneUtility()
// -----------------------------------------------------------------------------
//
CMdaAudioToneUtility::~CMdaAudioToneUtility()
    {
    //iTimer.Cancel();
    Cancel();
    iTimer.Close();
    delete iFile8;
    }

// -----------------------------------------------------------------------------
// CMdaAudioToneUtility::MaxVolume()
// -----------------------------------------------------------------------------
//
TInt CMdaAudioToneUtility::MaxVolume()
    {
    return iMaxVolume;
    }

// -----------------------------------------------------------------------------
// CMdaAudioToneUtility::Volume()
// -----------------------------------------------------------------------------
//
TInt CMdaAudioToneUtility::Volume()
    {
    return iVolume;
    }

// -----------------------------------------------------------------------------
// CMdaAudioToneUtility::SetVolume
// -----------------------------------------------------------------------------
//
void CMdaAudioToneUtility::SetVolume( TInt aVolume )
    {
    iVolume = aVolume;
    CTestFunctionCallData* data = NULL;
    TRAP_IGNORE( data = CTestFunctionCallData::NewL( CMdaAudioToneUtility::ESetVolume, aVolume ) );
    CMdaAudioToneUtility::iStaticTestData.Append( data );
    }

// -----------------------------------------------------------------------------
// CMdaAudioToneUtility::SetRepeats
// -----------------------------------------------------------------------------
//
void CMdaAudioToneUtility::SetRepeats( TInt aRepeatNumberOfTimes, const TTimeIntervalMicroSeconds& aTrailingSilence )
    {
    CTestFunctionCallData* data = NULL;
    TRAP_IGNORE( data = CTestFunctionCallData::NewL( CMdaAudioToneUtility::ESetRepeats, aRepeatNumberOfTimes, aTrailingSilence ) );
    CMdaAudioToneUtility::iStaticTestData.Append( data );
    }

// -----------------------------------------------------------------------------
// CMdaAudioToneUtility::SetVolumeRamp
// -----------------------------------------------------------------------------
//
void CMdaAudioToneUtility::SetVolumeRamp( const TTimeIntervalMicroSeconds& aRampDuration )
    {
    CTestFunctionCallData* data = NULL;
    TRAP_IGNORE( data = CTestFunctionCallData::NewL( CMdaAudioToneUtility::ESetVolumeRamp, aRampDuration ) );
    CMdaAudioToneUtility::iStaticTestData.Append( data );   
    }

// -----------------------------------------------------------------------------
// CMdaAudioToneUtility::FixedSequenceCount()
// -----------------------------------------------------------------------------
//
TInt CMdaAudioToneUtility::FixedSequenceCount()
    {
    return 0;
    }

// -----------------------------------------------------------------------------
// CMdaAudioToneUtility::FixedSequenceName
// -----------------------------------------------------------------------------
//
const TDesC& CMdaAudioToneUtility::FixedSequenceName( TInt /*aSequenceNumber*/ )
    {
    return KNullDesC();
    }

// -----------------------------------------------------------------------------
// CMdaAudioToneUtility::PrepareToPlayTone
// -----------------------------------------------------------------------------
//
void CMdaAudioToneUtility::PrepareToPlayTone( TInt /*aFrequency*/, const TTimeIntervalMicroSeconds& /*aDuration*/ )
    {
    }

// -----------------------------------------------------------------------------
// CMdaAudioToneUtility::PrepareToPlayDTMFString
// -----------------------------------------------------------------------------
//
void CMdaAudioToneUtility::PrepareToPlayDTMFString( const TDesC& /*aDTMF*/ )
    {
    }

// -----------------------------------------------------------------------------
// CMdaAudioToneUtility::PrepareToPlayDesSequence
// -----------------------------------------------------------------------------
//
void CMdaAudioToneUtility::PrepareToPlayDesSequence( const TDesC8& aSequence )
    {
    HBufC8* des( NULL );
    TRAP_IGNORE( des = HBufC8::NewL( aSequence.Length() ) );
    TRAP_IGNORE( iFile8 = HBufC8::NewL( aSequence.Length() ) );
        
    CTestFunctionCallData* data( NULL );
    if ( des )
        {
        *des = aSequence;
        } 

    if ( iFile8 )
        {
        *iFile8 = aSequence;
        }

    TRAP_IGNORE( data = CTestFunctionCallData::NewL( 
            CMdaAudioToneUtility::EPrepareToPlayDesSequence, des ) );
    CMdaAudioToneUtility::iStaticTestData.Append( data );   
    }

// -----------------------------------------------------------------------------
// CMdaAudioToneUtility::PrepareToPlayFileSequence
// -----------------------------------------------------------------------------
//
void CMdaAudioToneUtility::PrepareToPlayFileSequence( const TDesC& aFileName )
    {
    HBufC* desc = HBufC::NewL( aFileName.Length() );
    *desc = aFileName;
    CleanupStack::PushL( desc );
    CTestFunctionCallData* data( NULL );
    data = CTestFunctionCallData::NewL(
            CMdaAudioToneUtility::EPrepareToPlayFileSequence, desc, 0, 0 );
    CleanupStack::Pop( desc );
    CMdaAudioToneUtility::iStaticTestData.Append( data );
    ConstructL();
    }

// -----------------------------------------------------------------------------
// CMdaAudioToneUtility::PrepareToPlayFixedSequence
// -----------------------------------------------------------------------------
//
void CMdaAudioToneUtility::PrepareToPlayFixedSequence( TInt /*aSequenceNumber*/ )
    {
    }

// -----------------------------------------------------------------------------
// CMdaAudioToneUtility::CancelPrepare()
// -----------------------------------------------------------------------------
//
void CMdaAudioToneUtility::CancelPrepare()
    {
    CTestFunctionCallData* data = NULL;
    TRAP_IGNORE( data = CTestFunctionCallData::NewL( 
            CMdaAudioToneUtility::ECancelPrepare ) );
    CMdaAudioToneUtility::iStaticTestData.Append( data );
    
    }

// -----------------------------------------------------------------------------
// CMdaAudioToneUtility::Play()
// -----------------------------------------------------------------------------
//
void CMdaAudioToneUtility::Play()
    {
    CTestFunctionCallData* data( NULL );
    TRAP_IGNORE( data = CTestFunctionCallData::NewL( CMdaAudioToneUtility::EPlay ) );
    CMdaAudioToneUtility::iStaticTestData.Append( data );   
    }

// -----------------------------------------------------------------------------
// CMdaAudioToneUtility::State()
// -----------------------------------------------------------------------------
//
TMdaAudioToneUtilityState CMdaAudioToneUtility::State()
     {
    TMdaAudioToneUtilityState retVal;
     if ( IsActive() )
         {
         retVal = EMdaAudioToneUtilityNotReady;
         }
     else
         {
         retVal = EMdaAudioToneUtilityPlaying;
         }
     return retVal;
     }

// -----------------------------------------------------------------------------
// CMdaAudioToneUtility::CancelPlay()
// -----------------------------------------------------------------------------
//
void CMdaAudioToneUtility::CancelPlay()
    {
    CTestFunctionCallData* data = NULL;
    TRAP_IGNORE( data = CTestFunctionCallData::NewL( CMdaAudioToneUtility::ECancelPlay ) );
    CMdaAudioToneUtility::iStaticTestData.Append( data );
    }
// -----------------------------------------------------------------------------
// CMdaAudioToneUtility::DelayAudioLoading()
// -----------------------------------------------------------------------------
//
void CMdaAudioToneUtility::DelayAudioLoading()
    {
    CMdaAudioToneUtility::iDelayedAudioLoading = ETrue;
    }
 
// -----------------------------------------------------------------------------
// CMdaAudioToneUtility::SimulateErrorInAudioLoading()
// -----------------------------------------------------------------------------
//
void CMdaAudioToneUtility::SimulateErrorInAudioLoading()
    {
    CMdaAudioToneUtility::iErrorInAudioLoading = ETrue;
    }

// -----------------------------------------------------------------------------
// CMdaAudioToneUtility::TestData()
// -----------------------------------------------------------------------------
//
RPointerArray< CTestFunctionCallData >& CMdaAudioToneUtility::TestData()
    {
    return CMdaAudioToneUtility::iStaticTestData;
    }

// -----------------------------------------------------------------------------
// CMdaAudioToneUtility::ResetTestData()
// -----------------------------------------------------------------------------
//
void CMdaAudioToneUtility::ResetTestData()
    {
    CMdaAudioToneUtility::iStaticTestData.ResetAndDestroy();
    CMdaAudioToneUtility::iDelayedAudioLoading = EFalse;
    CMdaAudioToneUtility::iErrorInAudioLoading = EFalse;
    }

// -----------------------------------------------------------------------------
// CMdaAudioToneUtility::DoCancel()
// -----------------------------------------------------------------------------
//
void CMdaAudioToneUtility::DoCancel()
    {
    iTimer.Cancel();
    }

// -----------------------------------------------------------------------------
// CMdaAudioToneUtility::RunL()
// -----------------------------------------------------------------------------
//
void CMdaAudioToneUtility::RunL()
    {
    if ( CMdaAudioToneUtility::iErrorInAudioLoading )
        {
        iObserver.MatoPrepareComplete( KGeneratedError );
        }
    else
        {
        iObserver.MatoPrepareComplete( KErrNone );
        }
    }

// -----------------------------------------------------------------------------
// CMdaAudioToneUtility::Stop()
// -----------------------------------------------------------------------------
//
void CMdaAudioToneUtility::Stop()
    {
     CTestFunctionCallData* data( NULL );
    TRAP_IGNORE( data = CTestFunctionCallData::NewL( CMdaAudioToneUtility::EStop ) );
    CMdaAudioToneUtility::iStaticTestData.Append( data );   
    }

// -----------------------------------------------------------------------------
// CMdaAudioToneUtility::ConstructL()
// -----------------------------------------------------------------------------
//
void CMdaAudioToneUtility::ConstructL()
    {
    iTimer.CreateLocal();
    
    if ( CMdaAudioToneUtility::iDelayedAudioLoading )
        {
        iTimer.After( iStatus, KDelayedLoadingTime );
        }
    else
        {
        iTimer.After( iStatus, KNormalLoadingTime );
        }
    SetActive();
    }

