
#include <eunitmacros.h>
#include <eunitdecorators.h>

#include "tphonecmdparamringtone.h"
#include <phoneui.pan>
#include "mt_phoneringingtoneplayer.h"
#include <profile.hrh>


const TTimeIntervalMicroSeconds32 KInitDelay( 7000000 );

const TTimeIntervalMicroSeconds32 KPlayDelay( 1000000 );

const TTimeIntervalMicroSeconds32 KDelayToPlaybackInitPhase( 1000000 );

const TInt KPhoneRingingRepeatsTrailPause = 1000000;
const TInt KPhoneMdaAudioToneRepeatForever = -2;


_LIT( KTestDefaultTone, "z:\\data\\sounds\\digital\\Nokia tune.aac" );

_LIT( KRandomSample, "z:\\data\\sounds\\gee.mp3" );

_LIT( KRandomTone, "z:\\data\\sounds\\ring.rng" );


_LIT8( KPhoneNoSoundSequence, "\x00\x11\x06\x05\xFC\x0A\x08\x40\x32\x0A\xF7\x40\x64\x06\x0B" );
//_LIT8( KPhoneBeepSequence, "\x00\x11\x0A\x0A\x08\x73\x0A\x40\x28\x0A\xF7\x05\xFC\x40\x64\x0A\x08\x40\x32\x0A\xF7\x06\x0B" );


MT_PhoneRingingTonePlayer* MT_PhoneRingingTonePlayer::NewL()
    {
    MT_PhoneRingingTonePlayer* self = MT_PhoneRingingTonePlayer::NewLC();
    CleanupStack::Pop();
    return self;
    }

MT_PhoneRingingTonePlayer* MT_PhoneRingingTonePlayer::NewLC()
    {
    MT_PhoneRingingTonePlayer* self = new( ELeave ) MT_PhoneRingingTonePlayer();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


MT_PhoneRingingTonePlayer::~MT_PhoneRingingTonePlayer()
    {
    Teardown();
    }


MT_PhoneRingingTonePlayer::MT_PhoneRingingTonePlayer()
    {
    
    }

void MT_PhoneRingingTonePlayer::ConstructL()
    {
    CEUnitTestSuiteClass::ConstructL();
    }

    
void MT_PhoneRingingTonePlayer::SetupL()
    {
    iClient.Connect();
    }

     
void MT_PhoneRingingTonePlayer::Teardown()
    {
    CMdaAudioPlayerUtility::ResetTestData();
    CMdaAudioToneUtility::ResetTestData();
    iClient.Close();
    User::After( KInitDelay );
    }

void MT_PhoneRingingTonePlayer::EmptySetupL()
    {
        
    }
    
void MT_PhoneRingingTonePlayer::EmptyTeardown()
    {
        
    }

void MT_PhoneRingingTonePlayer::MT_Empty()
    {
    
    }
void MT_PhoneRingingTonePlayer::MT_CreateServerInstanceAndConnectL()
    {
    RPhoneToneClient client;
    TInt error = client.Connect();
    User::After( KInitDelay );
    EUNIT_ASSERT_EQUALS( KErrNone, error );  
    CMdaAudioPlayerUtility::ResetTestData();
    CMdaAudioToneUtility::ResetTestData();
    client.Close();
    
    }

void MT_PhoneRingingTonePlayer::MT_CreateServerInstanceAndConnectTwiceL() // Inter-process communication fuzzing ( IPC - fuzz )
    {
    // SetupL creates a server instance and connects to it.
    RPhoneToneClient client;
    TInt error = client.Connect();
    User::After( KInitDelay );
    // Assert if server accepts multiple connections. 
    // Server should be uniquely connectable.
    EUNIT_ASSERT_EQUALS( KErrNone != error, ETrue );
    
    if ( !error )
        {
        client.Close();
        }
    }

void MT_PhoneRingingTonePlayer::MT_PlayDefaultL()
    {
    User::After( KInitDelay );
    TInt randomVolume( 8 );
    TPhoneCmdParamRingTone ringToneParam;
    ringToneParam.SetVolume( randomVolume );
    ringToneParam.SetRingingType( EProfileRingingTypeRinging );
    iClient.PlayRingingToneL( &ringToneParam );
    User::After( KInitDelay ); 
    HBufC* tonePath = HBufC::NewL( KTestDefaultTone().Length() );
    *tonePath = KTestDefaultTone();
    CleanupStack::PushL( tonePath );
    CTestFunctionCallData* data1 = CTestFunctionCallData::NewL( 
            CMdaAudioPlayerUtility::EPlay, 
            tonePath, 0, 0 ); // Transfer tonePath ownership
    CleanupStack::Pop( tonePath ); 
    CleanupStack::PushL( data1 );
    CTestFunctionCallData* data2 = CTestFunctionCallData::NewL( 
            CMdaAudioPlayerUtility::ESetVolume , randomVolume );
    EUNIT_ASSERT_EQUALS( SamplePlayerContainsCallData( data1 ) , ETrue );
    EUNIT_ASSERT_EQUALS( SamplePlayerContainsCallData( data2 ) , ETrue );
    delete data2;
    CleanupStack::PopAndDestroy( data1 );
    }

void MT_PhoneRingingTonePlayer::MT_PlayCustomSampleL()
    {
    User::After( KInitDelay );
    TInt randomVolume( 8 );
    TPhoneCmdParamRingTone ringToneParam;
    ringToneParam.SetVolume( randomVolume );
    ringToneParam.SetRingingType(  EProfileRingingTypeRinging );
    ringToneParam.SetRingTone( KRandomSample() );
    iClient.PlayRingingToneL( &ringToneParam );
    //Wait that ringingtone is prepared.
    User::After( KInitDelay );
    HBufC* tonePath = HBufC::NewL( KRandomSample().Length() );
    *tonePath = KRandomSample();
    CleanupStack::PushL( tonePath );
    CTestFunctionCallData* data1 = CTestFunctionCallData::NewL(
            CMdaAudioPlayerUtility::EPlay, 
            tonePath, 0, 0 ); // Transfer tonePath ownership
    CleanupStack::Pop( tonePath ); 
    CleanupStack::PushL( data1 );
    CTestFunctionCallData* data2 = CTestFunctionCallData::NewL( 
            CMdaAudioPlayerUtility::ESetVolume , 
            randomVolume );
    EUNIT_ASSERT_EQUALS( SamplePlayerContainsCallData( data1 ) , ETrue );
    EUNIT_ASSERT_EQUALS( SamplePlayerContainsCallData( data2 ) , ETrue );
    delete data2;
    CleanupStack::PopAndDestroy( data1 );
    }

void MT_PhoneRingingTonePlayer::MT_PlayCustomSampleWithDelayL()
    {
    User::After( KInitDelay );
    
    CMdaAudioPlayerUtility::DelayAudioLoading();
        
    TInt randomVolume( 10 );
    TPhoneCmdParamRingTone ringToneParam;
    ringToneParam.SetVolume( randomVolume );
    ringToneParam.SetRingingType(  EProfileRingingTypeRinging );
    ringToneParam.SetRingTone( KRandomSample() );
    iClient.PlayRingingToneL( &ringToneParam );
    User::After( KInitDelay );
    HBufC* tonePath = HBufC::NewL( KTestDefaultTone().Length() );
    *tonePath = KTestDefaultTone();
    CleanupStack::PushL( tonePath );
    CTestFunctionCallData* data1 = CTestFunctionCallData::NewL( 
            CMdaAudioPlayerUtility::EPlay, 
            tonePath, 0, 0 ); // Transfer tonePath ownership
    CleanupStack::Pop( tonePath ); 
    CleanupStack::PushL( data1 );
    CTestFunctionCallData* data2 = CTestFunctionCallData::NewL( 
            CMdaAudioPlayerUtility::ESetVolume , randomVolume );
    EUNIT_ASSERT_EQUALS( SamplePlayerContainsCallData( data1 ) , ETrue );
    EUNIT_ASSERT_EQUALS( SamplePlayerContainsCallData( data2 ) , ETrue );
    delete data2;
    CleanupStack::PopAndDestroy( data1 );
    }

// Switch to default playback
void MT_PhoneRingingTonePlayer::MT_PlayCustomSampleWithErrorL() 
    {
    User::After( KInitDelay );
    
    CMdaAudioPlayerUtility::SimulateErrorInAudioLoading();
        
    TInt randomVolume( 8 );
    TPhoneCmdParamRingTone ringToneParam;
    ringToneParam.SetVolume( randomVolume );
    ringToneParam.SetRingingType(  EProfileRingingTypeRinging );
    ringToneParam.SetRingTone( KRandomSample() );
    iClient.PlayRingingToneL( &ringToneParam );
    User::After( KInitDelay );
    
    //Error will occur in initialization. Thus default tone should be played.
    HBufC* tonePath = HBufC::NewL( KTestDefaultTone().Length() );
    *tonePath = KTestDefaultTone();
    CleanupStack::PushL( tonePath );
    CTestFunctionCallData* data1 = CTestFunctionCallData::NewL( 
            CMdaAudioPlayerUtility::EPlay, 
            tonePath, 0, 0 ); // Transfer tonePath ownership
    CleanupStack::Pop( tonePath ); 
    CleanupStack::PushL( data1 );
    CTestFunctionCallData* data2 = CTestFunctionCallData::NewL( 
            CMdaAudioPlayerUtility::ESetVolume , 
            randomVolume );
    EUNIT_ASSERT_EQUALS( SamplePlayerContainsCallData( data1 ) , ETrue );
    EUNIT_ASSERT_EQUALS( SamplePlayerContainsCallData( data2 ) , ETrue );
    delete data2;
    CleanupStack::PopAndDestroy( data1 );
    }

// backup timer callback
void MT_PhoneRingingTonePlayer::MT_PlayCustomSampleWithDelayAndErrorL() 
    {
    User::After( KInitDelay );
    
    CMdaAudioPlayerUtility::DelayAudioLoading();
    CMdaAudioPlayerUtility::SimulateErrorInAudioLoading();
    
    TInt randomVolume( 8 );
    TPhoneCmdParamRingTone ringToneParam;
    ringToneParam.SetVolume( randomVolume );
    ringToneParam.SetRingingType(  EProfileRingingTypeRinging );
    ringToneParam.SetRingTone( KRandomSample() );
    iClient.PlayRingingToneL( &ringToneParam );
    User::After( KInitDelay );
    HBufC* tonePath = HBufC::NewL( KTestDefaultTone().Length() );
    *tonePath = KTestDefaultTone();
    CleanupStack::PushL( tonePath );
    CTestFunctionCallData* data1 = CTestFunctionCallData::NewL( 
            CMdaAudioPlayerUtility::EPlay, 
            tonePath, 0, 0 ); // Transfer tonePath ownership
    CleanupStack::Pop( tonePath ); 
    CleanupStack::PushL( data1 );
    CTestFunctionCallData* data2 = CTestFunctionCallData::NewL( 
            CMdaAudioPlayerUtility::ESetVolume, 
            randomVolume );
    
    EUNIT_ASSERT_EQUALS( SamplePlayerContainsCallData( data1 ) , ETrue );
    EUNIT_ASSERT_EQUALS( SamplePlayerContainsCallData( data2 ) , ETrue );
    EUNIT_ASSERT_EQUALS( SamplePlayerContainsOnlyOneCall( CMdaAudioPlayerUtility::EPlay ) , ETrue );
    delete data2;
    CleanupStack::PopAndDestroy( data1 );
    }
    

/*******************************
/ Next tests for rng file type 
/*******************************/

void MT_PhoneRingingTonePlayer::MT_PlayCustomToneL()
    {
    User::After( KInitDelay );
    TInt randomVolume( 1 );
    TPhoneCmdParamRingTone ringToneParam;
    ringToneParam.SetVolume( randomVolume );
    ringToneParam.SetRingingType( EProfileRingingTypeRinging );
    ringToneParam.SetRingTone( KRandomTone() );
    iClient.PlayRingingToneL( &ringToneParam );
    User::After( KInitDelay );
    HBufC* tonePath = HBufC::NewL( KRandomTone().Length() );
    *tonePath = KRandomTone();
    CleanupStack::PushL( tonePath );
    
    CTestFunctionCallData* data1 = CTestFunctionCallData::NewL( 
            CMdaAudioToneUtility::EPrepareToPlayFileSequence, tonePath,0,0 ); // Transfer tonePath ownership

    CleanupStack::Pop( tonePath ); 
    
    CleanupStack::PushL( data1 );
    CTestFunctionCallData* data2 = CTestFunctionCallData::NewL( 
            CMdaAudioToneUtility::EPlay ); 

    CleanupStack::PushL( data2 );
    CTestFunctionCallData* data3 = CTestFunctionCallData::NewL( 
            CMdaAudioToneUtility::ESetVolume, randomVolume );
    /* do repeat check */
//    KPhoneMdaAudioToneRepeatForever,
//        TTimeIntervalMicroSeconds( KPhoneRingingRepeatsTrailPause )
    
    EUNIT_ASSERT_EQUALS( TonePlayerContainsCallData( data1 ) , ETrue );
    EUNIT_ASSERT_EQUALS( TonePlayerContainsCallData( data2 ) , ETrue );
    EUNIT_ASSERT_EQUALS( TonePlayerContainsCallData( data3 ) , ETrue );
    delete data3;
    CleanupStack::PopAndDestroy( data2 );
    CleanupStack::PopAndDestroy( data1 );
    }

void MT_PhoneRingingTonePlayer::MT_PlayCustomToneWithDelayL()
    {
    User::After( KInitDelay );
    
    CMdaAudioToneUtility::DelayAudioLoading();

    TInt randomVolume( 8 );
    TPhoneCmdParamRingTone ringToneParam;
    ringToneParam.SetVolume( randomVolume );
    ringToneParam.SetRingingType(  EProfileRingingTypeRinging );
    ringToneParam.SetRingTone( KRandomTone() );
    iClient.PlayRingingToneL( &ringToneParam );
    User::After( KInitDelay );
    HBufC* tonePath = HBufC::NewL( KTestDefaultTone().Length() );
    *tonePath = KTestDefaultTone();
    CleanupStack::PushL( tonePath );
    CTestFunctionCallData* data1 = CTestFunctionCallData::NewL( 
            CMdaAudioPlayerUtility::EPlay, 
            tonePath, 0, 0 );  // Transfer tonePath ownership
    CleanupStack::Pop( tonePath ); 
    CleanupStack::PushL( data1 );
    CTestFunctionCallData* data2 = CTestFunctionCallData::NewL( 
            CMdaAudioPlayerUtility::ESetVolume , randomVolume );
    EUNIT_ASSERT_EQUALS( SamplePlayerContainsCallData( data1 ) , ETrue );
    EUNIT_ASSERT_EQUALS( SamplePlayerContainsCallData( data2 ) , ETrue );
    delete data2;
    CleanupStack::PopAndDestroy( data1 );
    }

void MT_PhoneRingingTonePlayer::MT_PlayCustomToneWithErrorL()
    {
    User::After( KInitDelay );
    
    CMdaAudioToneUtility::SimulateErrorInAudioLoading();

    TInt randomVolume( 4 );
    TPhoneCmdParamRingTone ringToneParam;
    ringToneParam.SetVolume( randomVolume );
    ringToneParam.SetRingingType( EProfileRingingTypeRinging );
    ringToneParam.SetRingTone( KRandomTone() );
    iClient.PlayRingingToneL( &ringToneParam );
    User::After( KInitDelay );
    HBufC* tonePath = HBufC::NewL( KTestDefaultTone().Length() );
    *tonePath = KTestDefaultTone();
    CleanupStack::PushL( tonePath );
    CTestFunctionCallData* data1 = CTestFunctionCallData::NewL( 
            CMdaAudioPlayerUtility::EPlay, tonePath, 0, 0 ); // Transfer tonePath ownership
    CleanupStack::Pop( tonePath ); 
    CleanupStack::PushL( data1 );
    CTestFunctionCallData* data2 = CTestFunctionCallData::NewL( 
            CMdaAudioPlayerUtility::ESetVolume, randomVolume );
    EUNIT_ASSERT_EQUALS( SamplePlayerContainsCallData( data1 ) , ETrue );
    EUNIT_ASSERT_EQUALS( SamplePlayerContainsCallData( data2 ) , ETrue );
    delete data2;
    CleanupStack::PopAndDestroy( data1 );
    }

void MT_PhoneRingingTonePlayer::MT_PlayCustomToneWithDelayAndErrorL()
    {
    User::After( KInitDelay );
    
    CMdaAudioToneUtility::DelayAudioLoading();
    CMdaAudioToneUtility::SimulateErrorInAudioLoading();

    TInt randomVolume( 2 );
    TPhoneCmdParamRingTone ringToneParam;
    ringToneParam.SetVolume( randomVolume );
    ringToneParam.SetRingingType(  EProfileRingingTypeRinging );
    ringToneParam.SetRingTone( KRandomTone() );
    iClient.PlayRingingToneL( &ringToneParam );
    User::After( KInitDelay );
    HBufC* tonePath = HBufC::NewL( KTestDefaultTone().Length() );
    *tonePath = KTestDefaultTone();
    
    CleanupStack::PushL( tonePath );
    CTestFunctionCallData* data1 = CTestFunctionCallData::NewL( 
            CMdaAudioPlayerUtility::EPlay, 
            tonePath, 0, 0 ); // Transfer tonePath ownership
    CleanupStack::Pop( tonePath ); 
    
    CleanupStack::PushL( data1 );
    CTestFunctionCallData* data2 = CTestFunctionCallData::NewL( 
            CMdaAudioPlayerUtility::ESetVolume , randomVolume );
    
    CleanupStack::PushL( data2 );
    
    CTestFunctionCallData* data3 = CTestFunctionCallData::NewL( 
            CMdaAudioToneUtility::ECancelPrepare );

    EUNIT_ASSERT_EQUALS( SamplePlayerContainsCallData( data1 ) , ETrue );
    EUNIT_ASSERT_EQUALS( SamplePlayerContainsCallData( data2 ) , ETrue );
    EUNIT_ASSERT_EQUALS( TonePlayerContainsCallData( data3 ) , ETrue );
    delete data3;
    CleanupStack::PopAndDestroy( data2 );
    CleanupStack::PopAndDestroy( data1 );
    }

void MT_PhoneRingingTonePlayer::MT_PlayAndStopL()
    {
    User::After( KInitDelay );
    TInt randomVolume( 8 );
    TPhoneCmdParamRingTone ringToneParam;
    ringToneParam.SetVolume( randomVolume );
    ringToneParam.SetRingingType( EProfileRingingTypeRinging );
    iClient.PlayRingingToneL( &ringToneParam );

    User::After( KInitDelay );
        
    iClient.StopPlayingL();
    
    HBufC* tonePath = HBufC::NewL( KTestDefaultTone().Length() );
    *tonePath = KTestDefaultTone();
    CleanupStack::PushL( tonePath );
    CTestFunctionCallData* data1 = CTestFunctionCallData::NewL( 
            CMdaAudioPlayerUtility::EPlay, 
            tonePath, 0, 0 ); // Transfer tonePath ownership
    CleanupStack::Pop( tonePath ); 
    CleanupStack::PushL( data1 );
    CTestFunctionCallData* data2 = CTestFunctionCallData::NewL( 
            CMdaAudioPlayerUtility::ESetVolume , randomVolume );
    EUNIT_ASSERT_EQUALS( SamplePlayerContainsCallData( data1 ) , ETrue );
    EUNIT_ASSERT_EQUALS( SamplePlayerContainsCallData( data2 ) , ETrue );
    EUNIT_ASSERT_EQUALS( SamplePlayerContainsCall( CMdaAudioPlayerUtility::EStop ) , ETrue );
    delete data2;
    CleanupStack::PopAndDestroy( data1 );
    }

void MT_PhoneRingingTonePlayer::MT_PlayAndStopDuringInitL()
    {
    User::After( KInitDelay );
        
    CMdaAudioToneUtility::DelayAudioLoading();
    
    TInt randomVolume( 8 );
    TPhoneCmdParamRingTone ringToneParam;
    ringToneParam.SetVolume( randomVolume );
    ringToneParam.SetRingTone( KRandomSample() );
    iClient.PlayRingingToneL( &ringToneParam );
    User::After( KDelayToPlaybackInitPhase );

    iClient.StopPlayingL();
    
    User::After( KPlayDelay );
    EUNIT_ASSERT_EQUALS( SamplePlayerContainsCall( 
            CMdaAudioPlayerUtility::ENewFilePlayerL ) , ETrue );
    
    // Shouldn't contain 'Play' call.
    EUNIT_ASSERT_EQUALS( SamplePlayerContainsCall( 
            CMdaAudioPlayerUtility::EPlay ) , EFalse ); 
    }

void MT_PhoneRingingTonePlayer::MT_PlaySilentL()
    {
    User::After( KInitDelay );
    TPhoneCmdParamRingTone ringToneParam;
    ringToneParam.SetRingingType( EProfileRingingTypeSilent );
    iClient.PlayRingingToneL( &ringToneParam );
    User::After( KPlayDelay );
     
     HBufC8* sequence( NULL );
     TRAP_IGNORE( sequence = HBufC8::NewL( KPhoneNoSoundSequence().Length() ) );
     if ( sequence )
        {
        *sequence = KPhoneNoSoundSequence();
        }
     
    CTestFunctionCallData* data( NULL ); 
    data = CTestFunctionCallData::NewL( 
             CMdaAudioToneUtility::EPrepareToPlayDesSequence, sequence );
     
    CleanupStack::PushL( data );
    CTestFunctionCallData* data1( NULL );
    data1 = CTestFunctionCallData::NewL( CMdaAudioToneUtility::EPlay );
    
    EUNIT_ASSERT_EQUALS( TonePlayerContainsCallData( data1 ) , ETrue );
    EUNIT_ASSERT_EQUALS( TonePlayerContainsCallData( data ), ETrue );
    delete data1;
    CleanupStack::PopAndDestroy( data );
    }

//*******************
/* End of test cases
********************/



TBool MT_PhoneRingingTonePlayer::SamplePlayerContainsCall( CMdaAudioPlayerUtility::TFunctionCall aCall )
    {
    RPointerArray< CTestFunctionCallData >& array = CMdaAudioPlayerUtility::TestData();
    TBool found( EFalse );
    for( TInt i( 0 ); i < array.Count(); i++ )
        {
        CTestFunctionCallData* ptr = array[ i ];
        if ( ptr->FunctionIndex() == aCall )
            {
            found = ETrue;
            i = array.Count();
            }
        }
    return found;
    }

TBool MT_PhoneRingingTonePlayer::SamplePlayerContainsCallData( 
        CTestFunctionCallData* aData )
    {
    if ( !aData )
        {
        return EFalse;
        }
        
    RPointerArray< CTestFunctionCallData >& array = CMdaAudioPlayerUtility::TestData();
    TBool found( EFalse );
    for( TInt i( 0 ); i < array.Count(); i++ )
        {
        CTestFunctionCallData* ptr = array[ i ];
        if ( ( *ptr ) == *aData )
            {
            found = ETrue;
            i = array.Count();
            }
        }
    return found;
    }
    
TBool MT_PhoneRingingTonePlayer::SamplePlayerContainsOnlyOneCall( 
        CMdaAudioPlayerUtility::TFunctionCall aCall )
    {
    TInt count( 0 );
    for( TInt i( 0 ); i < CMdaAudioPlayerUtility::TestData().Count(); i++ )
        {
        if ( CMdaAudioPlayerUtility::TestData()[ i ]->FunctionIndex() == aCall )
            {
            count++;
            }
        }
    return ( 1 == count );
    }
    
TBool MT_PhoneRingingTonePlayer::TonePlayerContainsOnlyOneCall( CMdaAudioToneUtility::TFunctionCall aCall )
    {
    TInt count( 0 );
    for( TInt i( 0 ); i < CMdaAudioToneUtility::TestData().Count(); i++ )
        {
        if ( CMdaAudioToneUtility::TestData()[ i ]->FunctionIndex() == aCall )
            {
            count++;
            }
        }
    return ( 1 == count );
    }

TBool MT_PhoneRingingTonePlayer::TonePlayerContainsCall( 
        CMdaAudioToneUtility::TFunctionCall aCall )
    {
    RPointerArray< CTestFunctionCallData >& array = CMdaAudioToneUtility::TestData();
    TBool found( EFalse );
    for( TInt i( 0 ); i < array.Count(); i++ )
        {
        CTestFunctionCallData* ptr = array[ i ];
        if ( ptr->FunctionIndex() == aCall )
            {
            found = ETrue;
            i = array.Count();
            }
        }
    return found;
    }

TBool MT_PhoneRingingTonePlayer::TonePlayerContainsCallData( 
        CTestFunctionCallData* aData )
    {
    if ( !aData )
        {
        return EFalse;
        }
    RPointerArray< CTestFunctionCallData >& array = CMdaAudioToneUtility::TestData();
    TBool found( EFalse );
    for( TInt i( 0 ); i < array.Count(); i++ )
        {
        CTestFunctionCallData* ptr = array[ i ];
        if ( ( *ptr ) == *aData )
            {
            found = ETrue;
            i = array.Count();
            }
        }
    return found;
    }



///////////////////////////////////////////////////////////////////////////////////
////  TEST TABLE
///////////////////////////////////////////////////////////////////////////////////
EUNIT_BEGIN_TEST_TABLE(
    MT_PhoneRingingTonePlayer,
    "Add test suite description here.",
    "MODULE" )
    

//    EUNIT_TEST(
//        "PTP:Init and connect",
//        "RPhoneToneClient",
//        "Connect",
//        "FUNCTIONALITY",
//        EmptySetupL, MT_Empty, EmptyTeardown )

EUNIT_TEST(
    "PTP:Init and connect",
    "RPhoneToneClient",
    "Connect",
    "FUNCTIONALITY",
    EmptySetupL, MT_CreateServerInstanceAndConnectL, EmptyTeardown )
    
    
EUNIT_TEST(
    "PTP:Init and connect twice",
    "RPhoneToneClient",
    "ConnectTwice",
    "FUNCTIONALITY",
    SetupL, MT_CreateServerInstanceAndConnectTwiceL, Teardown )


EUNIT_TEST(
    "PTP:Play default",
    "RPhoneToneClient",
    "Play",
    "FUNCTIONALITY",
    SetupL, MT_PlayDefaultL, Teardown )


EUNIT_TEST(
    "PTP:Play custom sample",
    "RPhoneToneClient",
    "Play",
    "FUNCTIONALITY",
    SetupL, MT_PlayCustomSampleL, Teardown )
    
    
EUNIT_TEST(
    "PTP:Play custom sample with delay",
    "RPhoneToneClient",
    "Play",
    "FUNCTIONALITY",
    SetupL, MT_PlayCustomSampleWithDelayL, Teardown )


EUNIT_TEST(
    "PTP:Play tone with error init",
    "RPhoneToneClient",
    "PlayWithError",
    "FUNCTIONALITY",
    SetupL, MT_PlayCustomSampleWithErrorL, Teardown )
    
   
EUNIT_TEST(
    "PTP:Play tone with delayed init and error",
    "RPhoneToneClient",
    "PlayWithDelay",
    "FUNCTIONALITY",
    SetupL, MT_PlayCustomSampleWithDelayAndErrorL, Teardown )
    

EUNIT_TEST(
    "PTP:Play custom tone",
    "RPhoneToneClient",
    "PlayCustomTone",
    "FUNCTIONALITY",
    SetupL, MT_PlayCustomToneL, Teardown )


EUNIT_TEST(
    "PTP:Play custom tone with delayed init",
    "RPhoneToneClient",
    "PlayCustomToneWithDelay",
    "FUNCTIONALITY",
    SetupL, MT_PlayCustomToneWithDelayL, Teardown )


EUNIT_TEST(
    "PTP:Play tone with error",
    "RPhoneToneClient",
    "PlayCustomToneWithError",
    "FUNCTIONALITY",
    SetupL, MT_PlayCustomToneWithErrorL, Teardown )


EUNIT_TEST(
    "PTP:Play custom tone with delay and error",
    "RPhoneToneClient",
    "PlayCustomToneWithDelayAndError",
    "FUNCTIONALITY",
    SetupL, MT_PlayCustomToneWithDelayAndErrorL, Teardown )


EUNIT_TEST(
    "PTP:Play tone and stop",
    "RPhoneToneClient",
    "PlayAndStop",
    "FUNCTIONALITY",
    SetupL, MT_PlayAndStopL, Teardown )


EUNIT_TEST(
    "PTP:Play tone and stop during playback init",
    "RPhoneToneClient",
    "PlayAndStop",
    "FUNCTIONALITY",
    SetupL, MT_PlayAndStopDuringInitL, Teardown )


EUNIT_TEST(
    "PTP:Play silent tone",
    "RPhoneToneClient",
    "PlaySilent",
    "FUNCTIONALITY",
    SetupL, MT_PlaySilentL, Teardown )


EUNIT_END_TEST_TABLE

// End of file
